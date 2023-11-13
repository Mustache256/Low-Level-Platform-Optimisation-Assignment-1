#pragma once
#include <iostream>
#include <vector>

#include "Tracker.h"
#include "Common.h"

using namespace std;

class MemPool
{
public:
	MemPool(size_t lSizeOfSlice, int lNumOfSlices);
	~MemPool() { DeletePool(); };

	char* AddrFromIndex(int i) const;
	int IndexFromAddr(const char* p) const;

	void* Alloc();
	void Dealloc(void* p);

	void DeletePool();

	void* operator new(size_t size);
	void* operator new(size_t size, MemPool* pMemPool);
	void operator delete(void* pMem);
	void operator delete(void* pMem, MemPool* pMemPool);

	size_t mSizeOfSlice;

private:
	int mNumOfSlices;
	int mNumFreeSlices;
	int mNumSlicesInitialised;
	char* pMemStart;
	char* pNext;
};

