#include "Quad.h"

Quad::Quad()
{
	topLeftX = 0.0f;
	topLeftZ = 0.0f;
	botRightX = 0.0f;
	botRightZ = 0.0f;

	topLeftTree = NULL;
	topRightTree = NULL;
	botLeftTree = NULL;
	botRightTree = NULL;
}

Quad::Quad(float topLeft_x, float topleft_z, float botRight_x, float botRight_z)
{
	topLeftX = topLeft_x;
	topLeftZ = topleft_z;
	botRightX = botRight_x;
	botRightZ = botRight_z;

	topLeftTree = NULL;
	topRightTree = NULL;
	botLeftTree = NULL;
	botRightTree = NULL;
}

void Quad::Insert(Box* box)
{
	if (box == NULL)
		return;

	if (!InBoundary(box->position.x, box->position.z))
		return;

	if (abs(topLeftX - botRightX) <= 1 && abs(topLeftZ - botRightZ) <= 1)
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
				topLeftTree = new Quad(topLeftX, topLeftZ, AddPositionsAndDivide(topLeftX, botRightX), AddPositionsAndDivide(topLeftZ, botRightZ));
			}

			topLeftTree->Insert(box);
		}
		else
		{
			//Indicates botLeftTree
			if (botLeftTree == NULL)
			{
				botLeftTree = new Quad(topLeftX, AddPositionsAndDivide(topLeftZ, botRightZ), AddPositionsAndDivide(topLeftX, botRightX), botRightZ);
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
				topRightTree = new Quad(AddPositionsAndDivide(topLeftX, botRightX), topLeftZ, botRightX, AddPositionsAndDivide(topLeftZ, botRightZ));
			}

			topRightTree->Insert(box);
		}
		else
		{
			//Indicates botRightTree
			if (botRightTree == NULL)
			{
				botRightTree = new Quad(AddPositionsAndDivide(topLeftX, botRightX), AddPositionsAndDivide(topLeftZ, botRightZ), botRightX, botRightZ);
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

	if (boxesInQuad.size() > 1)
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

void Quad::UpdateQuadAndTree()
{
	for (int i = 0; i < boxesInQuad.size(); i++)
	{
		if (!InBoundary(boxesInQuad[i]->position.x, boxesInQuad[i]->position.z))
			boxesInQuad.erase(boxesInQuad.begin() + i);
	}

	if (topLeftTree != NULL)
		topLeftTree->UpdateQuadAndTree();

	if (topRightTree != NULL)
		topRightTree->UpdateQuadAndTree();

	if (botLeftTree != NULL)
		botRightTree->UpdateQuadAndTree();

	if (botRightTree != NULL)
		botRightTree->UpdateQuadAndTree();
}

void* Quad::operator new(size_t size)
{
	return ::operator new(size, Tracker::Type::quad);
}