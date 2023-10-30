#include <iostream>

#include "global.h"

using namespace std;

Header* pPrevHeader;

//#ifdef DEBUG
void* operator new(size_t size)
{
	cout << "global new being used\n";
	size_t totalBytes = size + sizeof(Header) + sizeof(Footer);
	
	char* pMem = (char*)malloc(totalBytes);
	Header* pHeader = (Header*)pMem;
	void* pStartMemAlloced = pMem + sizeof(Header);
	Footer* pFooter = (Footer*)(pMem + sizeof(Header) + size);

	pHeader->size = size;
	pHeader->pTrackerType = Tracker::base;
	pHeader->checkValue = 0xDEADC0DE;

	if (Tracker::GetPreviousHeader() != nullptr)
	{
		pHeader->pPrevHeader = Tracker::GetPreviousHeader();
		pHeader->pPrevHeader->pNextHeader = pHeader;
		Tracker::SetPreviousHeader(pHeader);
	}
	else
	{
		pHeader->pPrevHeader = nullptr;
		Tracker::SetPreviousHeader(pHeader);
	}

	pFooter->checkValue = 0xDEADBEEF;

	MemoryTracker::pBaseTracker->AddBytesAllocated(totalBytes);
	return pStartMemAlloced;
}

/*void* operator new(size_t size, Tracker* pTracker)
{
	cout << "global new with tracker being used\n";
	size_t totalBytes = size + sizeof(Header) + sizeof(Footer);
	
	char* pMem = (char*)malloc(totalBytes);
	Header* pHeader = (Header*)pMem;
	void* pStartMemAlloced = pMem + sizeof(Header);
	Footer* pFooter = (Footer*)(pMem + sizeof(Header) + size);
	
	pHeader->size = size;
	pHeader->pTracker = pTracker;

	if (pTracker->GetPreviousHeader() != nullptr)
	{
		pHeader->pPrevHeader = pTracker->GetPreviousHeader();
		pHeader->pPrevHeader->pNextHeader = pHeader;
		pTracker->SetPreviousHeader(pHeader);
	}
	else
	{
		pHeader->pPrevHeader = nullptr;
		pTracker->SetPreviousHeader(pHeader);
	}

	pTracker->AddBytesAllocated(totalBytes);
	return pStartMemAlloced;
}*/

void operator delete(void* pMem)
{
	cout << "global delete being used\n";
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);

	pHeader->pPrevHeader->pNextHeader = pHeader->pNextHeader;
	pHeader->pNextHeader->pPrevHeader = pHeader->pPrevHeader;

	if (pHeader->checkValue != 0xDEADC0DE)
		cout << "Header checkValue does not match expected value for " << pHeader << ", something has gone wrong\n";
	else
	{
		cout << "Header " << pHeader << " checkValue correct, freeing memory...\n";
		MemoryTracker::pBaseTracker->RemoveBytesAllocated(sizeof(pHeader));

		free(pHeader);
	}

}
//#endif // DEBUG