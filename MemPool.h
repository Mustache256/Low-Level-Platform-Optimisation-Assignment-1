#pragma once
#include <iostream>
#include <vector>

using namespace std;

class MemPool
{
public:
	MemPool(size_t objectSize, size_t poolSize);
	~MemPool();

	void* Alloc(size_t size);
	void Free(void* p, size_t size);

	struct slice
	{
		void* startOfSlice;
		bool inUse;
		size_t size;
	};

private:
	size_t sliceSize;
	vector<slice> slices;
};

