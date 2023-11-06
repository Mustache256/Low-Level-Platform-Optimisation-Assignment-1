#include "MemPool.h"

MemPool::MemPool(size_t objectSize, size_t poolSize)
{
	char* pMem = (char*)malloc(poolSize);
	sliceSize = objectSize;
	
	int totalSlices = poolSize / objectSize;

	for (int i = 0; i < totalSlices; i++)
	{
		Slice newSlice;

		if (i != 0)
		{
			//newSlice.startOfSlice = slices[i - 1].startOfSlice + sliceSize;
		}
		else
		{
			newSlice.startOfSlice = pMem;
		}

		newSlice.size = sliceSize;
		newSlice.inUse = false;

		slices.push_back(newSlice);
	}
}

MemPool::~MemPool()
{

}

void* MemPool::Alloc(size_t size)
{
	return nullptr;
}

void MemPool::Free(void* p, size_t size)
{

}