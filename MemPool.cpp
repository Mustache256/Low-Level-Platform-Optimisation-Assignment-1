#include "MemPool.h"



MemPool::MemPool(size_t sizeOfSlice, int numOfSlices)
{
	mNumOfSlices = numOfSlices;
	mSizeOfSlice = sizeOfSlice;
	
	pMemStart = (char*)::operator new(sizeof(mSizeOfSlice * mNumOfSlices));

	mNumSlicesInitialised = 0;
	mNumFreeSlices = numOfSlices;
	pNext = pMemStart;
}

char* MemPool::AddrFromIndex(int i) const
{
	return pMemStart + (i * mSizeOfSlice);
}

int MemPool::IndexFromAddr(const char* p) const
{
	return(((int)(p - pMemStart)) / mSizeOfSlice);
}

void* MemPool::Alloc()
{
	if (mNumSlicesInitialised < mNumOfSlices)
	{
		int* p = (int*)AddrFromIndex(mNumSlicesInitialised);
		*p = mNumSlicesInitialised - 1;
		mNumSlicesInitialised++;
	}

	void* ret = NULL;

	if (mNumFreeSlices > 0)
	{
		ret = (void*)pNext;
		--mNumFreeSlices;

		if (mNumFreeSlices != 0)
		{
			pNext = AddrFromIndex(*((int*)pNext));
		}
		else
		{
			pNext = NULL;
		}
	}

	return ret;
}

void MemPool::Dealloc(void* p)
{
	if (pNext != NULL)
	{
		(*(int*)p) = IndexFromAddr(pNext);
		pNext = (char*)p;
	}
	else
	{
		(*(int*)p) = IndexFromAddr(pNext);
		pNext = (char*)p;
	}

	mNumFreeSlices++;
}

void MemPool::DeletePool()
{
	delete[] pMemStart;
	pMemStart = NULL;
}

void* MemPool::operator new(size_t size, MemPool* pMemPool)
{
	cout << "\n-------------\n";
	cout << "\nAllocating to Memory Pool...\n";
	char* pMem = (char*)pMemPool->Alloc();

	if (pMem == nullptr)
	{
		cout << "\nNo free slices avaiable in Memory Pool, allocating outside pool instead...\n";
		return ::operator new(size);
	}

	if (size > pMemPool->mSizeOfSlice)
	{
		cerr << "\nCannot allocate to pool as object is larger than slice's size, allocating outside pool instead...\n";
		return ::operator new(size);
	}

	cout << "\nAllocated " << size << " bytes to Memory Pool at address " << pMem << "\n";

	return pMem;
}

void* MemPool::operator new(size_t size)
{
	cout << "\n-------------\n";
	cout << "\nMemPool new being used\n";
	size_t totalBytes = size + sizeof(Header) + sizeof(Footer);

	char* pMem = (char*)malloc(totalBytes);
	Header* pHeader = (Header*)pMem;
	void* pStartMemAlloced = pMem + sizeof(Header);
	Footer* pFooter = (Footer*)(pMem + sizeof(Header) + size);

	pHeader->size = size;
	pHeader->type = Tracker::Type::pool;
	pHeader->checkValue = 0xDEADC0DE;

	if (Tracker::GetPreviousHeader() != nullptr)
	{
		pHeader->pNextHeader = nullptr;
		pHeader->pPrevHeader = Tracker::GetPreviousHeader();
		pHeader->pPrevHeader->pNextHeader = pHeader;
		Tracker::SetPreviousHeader(pHeader);
	}
	else
	{
		pHeader->pPrevHeader = nullptr;
		pHeader->pNextHeader = nullptr;
		Tracker::SetPreviousHeader(pHeader);
		Tracker::SetFirstHeader(pHeader);
	}

	pFooter->checkValue = 0xDEADBEEF;

	Tracker::AddBytesAlloced(totalBytes, pHeader);
	return pStartMemAlloced;
}

void MemPool::operator delete(void* pMem)
{
	cout << "\n-------------\n";
	cout << "\nMemPool delete being used\n";
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);

	if (pHeader->pPrevHeader != nullptr)
	{
		pHeader->pPrevHeader->pNextHeader = pHeader->pNextHeader;

		if (pHeader->pNextHeader != nullptr)
			pHeader->pNextHeader->pPrevHeader = pHeader->pPrevHeader;
	}
	else
	{
		if (pHeader->pNextHeader != nullptr)
		{
			pHeader->pNextHeader->pPrevHeader = nullptr;
			Tracker::SetFirstHeader(pHeader->pNextHeader);
		}
		else
			Tracker::SetFirstHeader(nullptr);
	}

	if (pHeader->checkValue != 0xDEADC0DE)
		cout << "\nHeader checkValue does not match expected value for " << pHeader << ", something has gone wrong\n";
	else
	{
		cout << "\nHeader " << pHeader << " checkValue correct, freeing memory...\n";
		Tracker::RemoveBytesAlloced(sizeof(pHeader), pHeader);

		free(pHeader);
	}
}

void MemPool::operator delete(void* pMem, MemPool* pMemPool)
{
	cout << "\n-------------\n";
	cout << "\nDe-Allocating from Memory Pool...\n";
	pMemPool->Dealloc(pMem);
}