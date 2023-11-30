#include "Quad.h"

Quad::Quad(float minSize)
{
	topLeftX = 0.0f;
	topLeftZ = 0.0f;
	botRightX = 0.0f;
	botRightZ = 0.0f;

	boxesInQuad = std::vector<Box*>();

	minQuadSize = minSize;

	if (abs(topLeftX - botRightX) <= minSize && abs(topLeftZ - botRightZ) <= minSize)
	{
		topLeftTree = NULL;
		topRightTree = NULL;
		botLeftTree = NULL;
		botRightTree = NULL;
		return;
	}
	else
	{
		topLeftTree = new Quad(topLeftX, topLeftZ, AddPositionsAndDivide(topLeftX, botRightX), AddPositionsAndDivide(topLeftZ, botRightZ), minQuadSize);
		topRightTree = new Quad(AddPositionsAndDivide(topLeftX, botRightX), topLeftZ, botRightX, AddPositionsAndDivide(topLeftZ, botRightZ), minQuadSize);
		botLeftTree = new Quad(topLeftX, AddPositionsAndDivide(topLeftZ, botRightZ), AddPositionsAndDivide(topLeftX, botRightX), botRightZ, minQuadSize);
		botRightTree = new Quad(AddPositionsAndDivide(topLeftX, botRightX), AddPositionsAndDivide(topLeftZ, botRightZ), botRightX, botRightZ, minQuadSize);
	}
}

Quad::Quad(float topLeft_x, float topleft_z, float botRight_x, float botRight_z, float minSize)
{
	topLeftX = topLeft_x;
	topLeftZ = topleft_z;
	botRightX = botRight_x;
	botRightZ = botRight_z;

	boxesInQuad = std::vector<Box*>();

	minQuadSize = minSize;

	if (abs(topLeftX - botRightX) <= minSize && abs(topLeftZ - botRightZ) <= minSize)
	{
		topLeftTree = NULL;
		topRightTree = NULL;
		botLeftTree = NULL;
		botRightTree = NULL;
		return;
	}
	else 
	{
		topLeftTree = new Quad(topLeftX, topLeftZ, AddPositionsAndDivide(topLeftX, botRightX), AddPositionsAndDivide(topLeftZ, botRightZ), minQuadSize);
		topRightTree = new Quad(AddPositionsAndDivide(topLeftX, botRightX), topLeftZ, botRightX, AddPositionsAndDivide(topLeftZ, botRightZ), minQuadSize);
		botLeftTree = new Quad(topLeftX, AddPositionsAndDivide(topLeftZ, botRightZ), AddPositionsAndDivide(topLeftX, botRightX), botRightZ, minQuadSize);
		botRightTree = new Quad(AddPositionsAndDivide(topLeftX, botRightX), AddPositionsAndDivide(topLeftZ, botRightZ), botRightX, botRightZ, minQuadSize);
	}

}

Quad::~Quad()
{
	delete topLeftTree;
	delete topRightTree;
	delete botLeftTree;
	delete botRightTree;

	topLeftTree = nullptr;
	topRightTree = nullptr;
	botLeftTree = nullptr;
	botRightTree = nullptr;
}

void Quad::Insert(Box* box)
{
	if (box == NULL)
		return;

	if (!InBoundary(box->position.x, box->position.z))
	{
		//std::cout << "\nBox outside of quad\n";
		return;
	}
		
	if (abs(topLeftX - botRightX) <= minQuadSize && abs(topLeftZ - botRightZ) <= minQuadSize)
	{
		for (Box* boxInVector : boxesInQuad)
		{
			if (box == boxInVector)
				return;
		}
		
		boxesInQuad.push_back(box);
	}

	if (AddPositionsAndDivide(topLeftX, botRightX) >= box->position.x)
	{
		//Indicated topLeftTree
		if (AddPositionsAndDivide(topLeftZ, botRightZ) >= box->position.z)
		{
			if (topLeftTree == NULL)
			{
				std::cerr << "\nAttempted to access topLeftTree that it null, somehow got passed quad of smallest size\n";
				return;
				//topLeftTree = new Quad(topLeftX, topLeftZ, AddPositionsAndDivide(topLeftX, botRightX), AddPositionsAndDivide(topLeftZ, botRightZ));
			}

			topLeftTree->Insert(box);
		}
		else
		{
			//Indicates botLeftTree
			if (botLeftTree == NULL)
			{
				std::cerr << "\nAttempted to access botLeftTree that it null, somehow got passed quad of smallest size\n";
				return;
				//botLeftTree = new Quad(topLeftX, AddPositionsAndDivide(topLeftZ, botRightZ), AddPositionsAndDivide(topLeftX, botRightX), botRightZ);
			}

			botLeftTree->Insert(box);
		}
	}
	else
	{
		//Indicates topRightTree
		if (AddPositionsAndDivide(topLeftZ, botRightZ) >= box->position.z)
		{
			if (topRightTree == NULL)
			{
				std::cerr << "\nAttempted to access topRightTree that it null, somehow got passed quad of smallest size\n";
				return;
				//topRightTree = new Quad(AddPositionsAndDivide(topLeftX, botRightX), topLeftZ, botRightX, AddPositionsAndDivide(topLeftZ, botRightZ));
			}

			topRightTree->Insert(box);
		}
		else
		{
			//Indicates botRightTree
			if (botRightTree == NULL)
			{
				std::cerr << "\nAttempted to access bopRightTree that it null, somehow got passed quad of smallest size\n";
				return;
				//botRightTree = new Quad(AddPositionsAndDivide(topLeftX, botRightX), AddPositionsAndDivide(topLeftZ, botRightZ), botRightX, botRightZ);
			}

			botRightTree->Insert(box);
		}
	}
}

std::vector<Box*> Quad::Search(float x, float z)
{
	std::vector<Box*> emptyReturn;

	if (!InBoundary(x, z))
		return emptyReturn;

	if (boxesInQuad.size() > 0)
		return boxesInQuad;

	if (AddPositionsAndDivide(topLeftX, botRightX) >= x)
	{
		//Indicates topLeftTree
		if (AddPositionsAndDivide(topLeftZ, botRightZ) >= z)
		{
			if (topLeftTree == NULL)
				return emptyReturn;

			return topLeftTree->Search(x, z);
		}
		//Indicates botLeftTree
		else
		{
			if (botLeftTree == NULL)
				return emptyReturn;

			return botLeftTree->Search(x, z);
		}
	}
	else
	{
		//Indicates topRightTree
		if (AddPositionsAndDivide(topLeftZ, botRightZ) >= z)
		{
			if (topRightTree == NULL)
				return emptyReturn;

			return topRightTree->Search(x, z);
		}
		//Indicates botRighTree
		else
		{
			if (botRightTree == NULL)
				return emptyReturn;

			return botRightTree->Search(x, z);
		}
	}
}

bool Quad::InBoundary(float x, float z)
{
	return (x >= topLeftX && x <= botRightX && z >= topLeftZ && z <= botRightZ);
}

float Quad::AddPositionsAndDivide(float pos1, float pos2)
{
	return (pos1 + pos2) / 2;
}

void Quad::UpdateQuadtree()
{
	if(!boxesInQuad.empty())
		boxesInQuad.clear();
	
	if (topLeftTree != NULL)
		topLeftTree->UpdateQuadtree();

	if (topRightTree != NULL)
		topRightTree->UpdateQuadtree();

	if (botLeftTree != NULL)
		botRightTree->UpdateQuadtree();

	if (botRightTree != NULL)
		botRightTree->UpdateQuadtree();
}

void* Quad::operator new(size_t size)
{
	return ::operator new(size, Tracker::Type::quad);
}