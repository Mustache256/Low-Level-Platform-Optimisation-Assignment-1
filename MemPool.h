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

	//Stores slice size
	size_t mSizeOfSlice;

private:
	//Muber of slices in the pool
	int mNumOfSlices;
	//Number of free slices
	int mNumFreeSlices;
	//Number of slices in use
	int mNumSlicesInitialised;
	//Pointer to the start of the pool memory
	char* pMemStart;
	//Pointer to the next free slice
	char* pNext;
};

