#include "MemPool.h"



MemPool::MemPool(size_t sizeOfSlice, int numOfSlices)
{
	//Specifying number of slices and size of slices
	mNumOfSlices = numOfSlices;
	mSizeOfSlice = sizeOfSlice;
	
	//Allocating memory for all the slices and getting a pointer to the start of that memory
	pMemStart = (char*)::operator new(sizeof(mSizeOfSlice * mNumOfSlices));

	//Initialising state tracking variables
	mNumSlicesInitialised = 0;
	mNumFreeSlices = numOfSlices;

	//Initialising pointer to next free slice
	pNext = pMemStart;
}

char* MemPool::AddrFromIndex(int i) const
{
	//Get slice address from passed in index
	return pMemStart + (i * mSizeOfSlice);
}

int MemPool::IndexFromAddr(const char* p) const
{
	//Get slice index from passed in address
	return(((int)(p - pMemStart)) / mSizeOfSlice);
}

void* MemPool::Alloc()
{
	//Cehcks that there are free slices
	if (mNumSlicesInitialised < mNumOfSlices)
	{
		//Get pointer to next free slice and update number of slices initialised
		int* p = (int*)AddrFromIndex(mNumSlicesInitialised);
		*p = mNumSlicesInitialised - 1;
		mNumSlicesInitialised++;
	}

	void* ret = NULL;

	//Checks that there are free slices
	if (mNumFreeSlices > 0)
	{
		//Get pointer to next free slice
		ret = (void*)pNext;
		--mNumFreeSlices;

		//Updates the next free slice pointer if htere are any free slices left, otherwise sets it to null
		if (mNumFreeSlices != 0)
		{
			pNext = AddrFromIndex(*((int*)pNext));
		}
		else
		{
			pNext = NULL;
		}
	}

	//Returns pointer to slice
	return ret;
}

void MemPool::Dealloc(void* p)
{
	//Updating pointers depending on if there are slices free or not
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

	//Updating free slice count to represent new free slice
	mNumFreeSlices++;
}

void MemPool::DeletePool()
{
	delete[] pMemStart;
	pMemStart = NULL;
}

//Used for new objects that are created in the pool
void* MemPool::operator new(size_t size, MemPool* pMemPool)
{
	cout << "\n-------------\n";
	cout << "\nAllocating to Memory Pool...\n";
	//Allocating object to memory pool
	char* pMem = (char*)pMemPool->Alloc();

	//Checks that there are free slices, if not creates object outside of pool
	if (pMem == nullptr)
	{
		cout << "\nNo free slices avaiable in Memory Pool, allocating outside pool instead...\n";
		return ::operator new(size);
	}

	//Checks if object can fit in slices, if not creates object outside of pool
	if (size > pMemPool->mSizeOfSlice)
	{
		cerr << "\nCannot allocate to pool as object is larger than slice's size, allocating outside pool instead...\n";
		return ::operator new(size);
	}

	//Returns pointer to allocated memory
	cout << "\nAllocated " << size << " bytes to Memory Pool at address " << pMem << "\n";

	return pMem;
}

//Using this ensures that the memory pool allocated memory starts and ends with a header and footer, however the objects within it do not
void* MemPool::operator new(size_t size)
{
	cout << "\n-------------\n";
	cout << "\nMemPool new being used\n";
	//Calculate total size to allocate with header and footer 
	size_t totalBytes = size + sizeof(Header) + sizeof(Footer);

	//Malloc those bytes and get a pointer to the start of the header and footer memory
	char* pMem = (char*)malloc(totalBytes);
	Header* pHeader = (Header*)pMem;
	void* pStartMemAlloced = pMem + sizeof(Header);
	Footer* pFooter = (Footer*)(pMem + sizeof(Header) + size);

	//Assign header variables
	pHeader->size = size;
	pHeader->type = Tracker::Type::pool;
	pHeader->checkValue = 0xDEADC0DE;

	//Update tracker previous header and first header pointers depending on whether this is the first header to be made or not, creates the doubly linked list
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

	//Set footer checkValue
	pFooter->checkValue = 0xDEADBEEF;

	//Add allocated bytes to tracker and return pointer to start of memory
	Tracker::AddBytesAlloced(totalBytes, pHeader);
	return pStartMemAlloced;
}

void MemPool::operator delete(void* pMem)
{
	cout << "\n-------------\n";
	cout << "\nMemPool delete being used\n";
	//Get pointer to start of memory and to start of footer
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);

	//Update linked list to fix gap that would be left by deleting this boject
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

	//Check header checkValue to ensure no memory has been overwritten
	if (pHeader->checkValue != 0xDEADC0DE)
		cout << "\nHeader checkValue does not match expected value for " << pHeader << ", something has gone wrong\n";
	else
	{
		cout << "\nHeader " << pHeader << " checkValue correct, freeing memory...\n";
		//Remove bytes from tracker
		Tracker::RemoveBytesAlloced(sizeof(pHeader), pHeader);
		//Free bytes that were being used by this object
		free(pHeader);
	}
}

void MemPool::operator delete(void* pMem, MemPool* pMemPool)
{
	//De-allocates slice as object is deleted
	cout << "\n-------------\n";
	cout << "\nDe-Allocating from Memory Pool...\n";
	pMemPool->Dealloc(pMem);
}