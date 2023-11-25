#pragma once
//Total boxes in the scene
#define NUMBER_OF_BOXES 500

//Camera position and look direction
#define LOOKAT_X 10
#define LOOKAT_Y 10
#define LOOKAT_Z 50

#define LOOKDIR_X 10
#define LOOKDIR_Y 0
#define LOOKDIR_Z 0

//Bounds of the area the boxes are confined to
#define minX -10.0f
#define maxX 30.0f
#define minZ -30.0f
#define maxZ 30.0f

//Toggle to enable and disable physics multithreading
#define USING_PHYSICS_MULTITHREADING true;