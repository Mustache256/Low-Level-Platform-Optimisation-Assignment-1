
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <ppl.h>
#include <xmmintrin.h>

#include "Tracker.h"
#include "Global.h"
#include "Vector3.h"
#include "Box.h"
#include "MemPool.h"
#include "Constants.h"
#include "PhysicsManager.h"
#include "Quad.h"

using namespace std::chrono;
using namespace concurrency;
using namespace std;

// gravity - change it and see what happens (usually negative!)
const float gravity = -19.81f;

//Will use this again if I can find a solution to the memory pool exception
//MemPool* boxPool = new MemPool(sizeof(Box), NUMBER_OF_BOXES);

std::vector<Box*> boxes;

//Creating physics manager
PhysicsManager* physManager = new PhysicsManager(-19.81, 0.0f);

//Using bool rather than constant definition as I need check inside of an if statement that already uses a constant definition
bool usingQuadtree = USING_QUADTREE;

//Creating a quad tree
Quad* quadTree = new Quad(minX, maxX, minZ, maxZ, 5.0f);

void initScene(int boxCount) {
    for (int i = 0; i < boxCount; ++i) {
        
        //Will use this again if I can find a solution to the memory pool exception
        //Box* box = (Box*)MemPool::operator new(sizeof(Box), boxPool);
        Box* box = new Box();  

        // Assign random x, y, and z positions within specified ranges
        box->genRandPos(box);

        box->setBoxSize(box, 1.0f, 1.0f, 1.0f);

        // Assign random x-velocity between -1.0f and 1.0f
        box->genRandVel(box);

        // Assign a random color to the box
        box->genRandCol(box);

        boxes.push_back(box);
    }
}

bool rayBoxIntersection(const Vector3& rayOrigin, const Vector3& rayDirection, const Box* box) {
    float tMin = (box->position.x - box->size.x / 2.0f - rayOrigin.x) / rayDirection.x;
    float tMax = (box->position.x + box->size.x / 2.0f - rayOrigin.x) / rayDirection.x;

    if (tMin > tMax) std::swap(tMin, tMax);

    float tyMin = (box->position.y - box->size.y / 2.0f - rayOrigin.y) / rayDirection.y;
    float tyMax = (box->position.y + box->size.y / 2.0f - rayOrigin.y) / rayDirection.y;

    if (tyMin > tyMax) std::swap(tyMin, tyMax);

    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin)
        tMin = tyMin;

    if (tyMax < tMax)
        tMax = tyMax;

    float tzMin = (box->position.z - box->size.z / 2.0f - rayOrigin.z) / rayDirection.z;
    float tzMax = (box->position.z + box->size.z / 2.0f - rayOrigin.z) / rayDirection.z;

    if (tzMin > tzMax) std::swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    return true;
}

// used in the 'mouse' tap function to convert a screen point to a point in the world
Vector3 screenToWorld(int x, int y) {
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels(x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    gluUnProject(winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    return Vector3((float)posX, (float)posY, (float)posZ);
}

void resolveCollision(Box* a, Box* b) {
    Vector3 normal = { a->position.x - b->position.x, a->position.y - b->position.y, a->position.z - b->position.z };
    float length = std::sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

    // Normalize the normal vector
    normal.normalise();

    float relativeVelocityX = a->velocity.x - b->velocity.x;
    float relativeVelocityY = a->velocity.y - b->velocity.y;
    float relativeVelocityZ = a->velocity.z - b->velocity.z;

    // Compute the relative velocity along the normal
    float impulse = relativeVelocityX * normal.x + relativeVelocityY * normal.y + relativeVelocityZ * normal.z;

    // Ignore collision if objects are moving away from each other
    if (impulse > 0) {
        return;
    }

    // Compute the collision impulse scalar
    float e = 0.01f; // Coefficient of restitution (0 = inelastic, 1 = elastic)
    float dampening = 0.9f; // Dampening factor (0.9 = 10% energy reduction)
    float j = -(1.0f + e) * impulse * dampening;

    // Apply the impulse to the boxes' velocities
    a->velocity.x += j * normal.x;
    a->velocity.y += j * normal.y;
    a->velocity.z += j * normal.z;
    b->velocity.x -= j * normal.x;
    b->velocity.y -= j * normal.y;
    b->velocity.z -= j * normal.z;
}

// update the physics: gravity, collision test, collision resolution
void updatePhysics() {
#if USING_PHYSICS_MULTITHREADING
  //Parallel for each loop using the parallel processing library for physics multithreading
    parallel_for_each(begin(boxes), end(boxes), [&](Box* box) {
        //Using the physcis manager to update box attributes resluting from influence of gravity, time and colliding with teh edges of the scene
        physManager->ApplyGravity(box->velocity.y);

        physManager->ApplyVelocityChange(box->position, box->velocity);

        physManager->CheckBoundsCollision(box->position, box->velocity, box->size);
        
        if (usingQuadtree)
         {
          //Add box to quad tree
            quadTree->Insert(box);
        }
        else
        {
            // Check for collisions with other boxes
            for (Box* other : boxes) {
                if (box == other) continue;
                //Using physics manager to check collisions
                if (physManager->CheckOtherCollision(box->position, box->size, other->position, other->size)) {
                    resolveCollision(box, other);
                    //This doesn't work for some reason I don't understand
                    //physManager->ResolveOtherCollision(box.position, box.velocity, other.position, other.velocity);
                    break;
                }
            }
        } 
    });
    if (usingQuadtree)
    {
        parallel_for_each(begin(boxes), end(boxes), [&](Box* box) {

          //Findling all boxes in same quad as current box
            std::vector<Box*> boxesInQuad = quadTree->Search(box->position.x, box->position.z);

            //Chekcing for collision against boxes in same quad
            for (Box* other : boxesInQuad) {
                if (box == other) continue;

                if (physManager->CheckOtherCollision(box->position, box->size, other->position, other->size)) {
                    resolveCollision(box, other);
                    //This doesn't work for some reason I don't understand
                    //physManager->ResolveOtherCollision(box.position, box.velocity, other.position, other.velocity);
                    break;
                }
            }
            });
    }
#else
    for (Box* box : boxes) {
        physManager->ApplyGravity(box->velocity.y);

        physManager->ApplyVelocityChange(box->position, box->velocity);

        physManager->CheckBoundsCollision(box->position, box->velocity, box->size);

        if (usingQuadtree)
        {
            quadTree->Insert(box);
        } 
        else
        {
            // Check for collisions with other boxes
            for (Box* other : boxes) {
                if (box == other) continue;

                if (physManager->CheckOtherCollision(box->position, box->size, other->position, other->size)) {
                    resolveCollision(box, other);
                    //This doesn't work for some reason I don't understand
                    //physManager->ResolveOtherCollision(box.position, box.velocity, other.position, other.velocity);
                    break;
                }
            }
        } 
    }
    if (usingQuadtree)
    {
        for (Box* box : boxes) {
            std::vector<Box*> boxesInQuad = quadTree->Search(box->position.x, box->position.z);

            for (Box* other : boxesInQuad) {
                if (box == other) continue;

                if (physManager->CheckOtherCollision(box->position, box->size, other->position, other->size)) {
                    resolveCollision(box, other);
                    //This doesn't work for some reason I don't understand
                    //physManager->ResolveOtherCollision(box.position, box.velocity, other.position, other.velocity);
                    break;
                }
            }
        }
    }
#endif // USING_PHYSICS_MULTITHREADING
}

// draw the sides of the containing area
void drawQuad(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4) {
    
    glBegin(GL_QUADS);
    glVertex3f(v1.x, v1.y, v1.z);
    glVertex3f(v2.x, v2.y, v2.z);
    glVertex3f(v3.x, v3.y, v3.z);
    glVertex3f(v4.x, v4.y, v4.z);
    glEnd();
}

void drawBox(const Box* box) {

    glPushMatrix();
    glTranslatef(box->position.x, box->position.y, box->position.z);
    GLfloat diffuseMaterial[] = { box->colour.x, box->colour.y, box->colour.z, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
    glScalef(box->size.x, box->size.y, box->size.z);
    glRotatef(-90, 1, 0, 0);
    glutSolidCube(1.0);
    //glutSolidTeapot(1);
    //glutSolidCone(1, 1, 10, 10);
    glPopMatrix();
}

// draw the entire scene
void drawScene() {

    // Draw the side wall
    GLfloat diffuseMaterial[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);

    // Draw the left side wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vector3 leftSideWallV1(minX, 0.0f, maxZ);
    Vector3 leftSideWallV2(minX, 50.0f, maxZ);
    Vector3 leftSideWallV3(minX, 50.0f, minZ);
    Vector3 leftSideWallV4(minX, 0.0f, minZ);
    drawQuad(leftSideWallV1, leftSideWallV2, leftSideWallV3, leftSideWallV4);

    // Draw the right side wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vector3 rightSideWallV1(maxX, 0.0f, maxZ);
    Vector3 rightSideWallV2(maxX, 50.0f, maxZ);
    Vector3 rightSideWallV3(maxX, 50.0f, minZ);
    Vector3 rightSideWallV4(maxX, 0.0f, minZ);
    drawQuad(rightSideWallV1, rightSideWallV2, rightSideWallV3, rightSideWallV4);


    // Draw the back wall
    glColor3f(0.5f, 0.5f, 0.5f); // Set the wall color
    Vector3 backWallV1(minX, 0.0f, minZ);
    Vector3 backWallV2(minX, 50.0f, minZ);
    Vector3 backWallV3(maxX, 50.0f, minZ);
    Vector3 backWallV4(maxX, 0.0f, minZ);
    drawQuad(backWallV1, backWallV2, backWallV3, backWallV4);

#if USING_GRAPHICS_MULTITHREADING
    parallel_for_each(begin(boxes), end(boxes), [&](const Box& box) {
        drawBox(box);
    });
#else
    for (const Box* box : boxes) {
        drawBox(box);
}
#endif
}

// called by GLUT - displays the scene
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(LOOKAT_X, LOOKAT_Y, LOOKAT_Z, LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z, 0, 1, 0);

    drawScene();

    glutSwapBuffers();
}

// called by GLUT when the cpu is idle - has a timer function you can use for FPS, and updates the physics
// see https://www.opengl.org/resources/libraries/glut/spec3/node63.html#:~:text=glutIdleFunc
// NOTE this may be capped at 60 fps as we are using glutPostRedisplay(). If you want it to go higher than this, maybe a thread will help here. 
void idle() {
  //Updating deltaTime through physics manager
    physManager->UpdateDeltaTime();

    //auto start = chrono::steady_clock::now();
    updatePhysics();
    //auto end = chrono::steady_clock::now();
    //chrono::duration<double> elapsedSeconds{end - start};
    //cout << "Time taken to calculate physics: " << elapsedSeconds.count() << "\n";

#if USING_QUADTREE
    //Updates quad tree, clearing out all the boxes from it
    quadTree->UpdateQuadtree();
#endif

    // tell glut to draw - note this will cap this function at 60 fps
    glutPostRedisplay();
}

// called the mouse button is tapped
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Get the camera position and direction
        Vector3 cameraPosition(LOOKAT_X, LOOKAT_Y, LOOKAT_Z); // Replace with your actual camera position
        Vector3 cameraDirection(LOOKDIR_X, LOOKDIR_Y, LOOKDIR_Z); // Replace with your actual camera direction

        // Get the world coordinates of the clicked point
        Vector3 clickedWorldPos = screenToWorld(x, y);

        // Calculate the ray direction from the camera position to the clicked point
        Vector3 rayDirection = clickedWorldPos - cameraPosition;
        rayDirection.normalise();

        // Perform a ray-box intersection test and remove the clicked box
        size_t clickedBoxIndex = -1;
        float minIntersectionDistance = std::numeric_limits<float>::max();

        for (size_t i = 0; i < boxes.size(); ++i) {
            if (rayBoxIntersection(cameraPosition, rayDirection, boxes[i])) {
                // Calculate the distance between the camera and the intersected box
                Vector3 diff = boxes[i]->position - cameraPosition;
                float distance = diff.length();

                // Update the clicked box index if this box is closer to the camera
                if (distance < minIntersectionDistance) {
                    clickedBoxIndex = i;
                    minIntersectionDistance = distance;
                }
            }
        }

        // Remove the clicked box if any
        if (clickedBoxIndex != -1) {
            boxes.erase(boxes.begin() + clickedBoxIndex);
        }
    }
}

// called when the keyboard is used
void keyboard(unsigned char key, int x, int y) {
    const float impulseMagnitude = 20.0f; // Upward impulse magnitude

    if (key == ' ') { // Spacebar key
        for (Box* box : boxes) {
            box->velocity.y += impulseMagnitude;
        }
    }

    if (key == 'w')
    {
      //Can walk the heap by pressing w
        Tracker::WalkTheHeap();
    }

    if (key == 't')
    {
      //Can output all bytes allocated by pressing t
        Tracker::PrintAllBytesAlloced();
    }
}

void DeleteScene()
{
    for (Box* box : boxes)
    {
        delete(box);
    }
}

// the main function. 
int main(int argc, char** argv) {

    srand(static_cast<unsigned>(time(0))); // Seed random number generator
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1920, 1080);
    glutCreateWindow("Simple Physics Simulation");

    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);

    initScene(NUMBER_OF_BOXES);
    glutDisplayFunc(display);
    glutIdleFunc(idle);

    // it will stick here until the program ends. 
    glutMainLoop();

    DeleteScene();

    return 0;
}
