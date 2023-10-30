#include <iostream>

#include "global.h"

using namespace std;

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
	pHeader->pTracker = nullptr;
	pHeader->checkValue = 0xDEADC0DE;

	LinkList(pHeader);

	pFooter->checkValue = 0xDEADBEEF;

	Tracker::AddBytesAllocated(totalBytes);
	return pStartMemAlloced;
}

void* operator new(size_t size, Tracker* pTracker)
{
	cout << "global new with tracker being used\n";
	size_t totalBytes = size + sizeof(Header) + sizeof(Footer);
	
	
	char* pMem = (char*)malloc(totalBytes);
	Header* pHeader = (Header*)pMem;
	void* pStartMemAlloced = pMem + sizeof(Header);
	Footer* pFooter = (Footer*)(pMem + sizeof(Header) + size);
	
	pHeader->size = size;
	pHeader->pTracker = pTracker;

	LinkList(pHeader, pTracker);

	pTracker->AddBytesAllocated(totalBytes);
	return pStartMemAlloced;
}

void operator delete(void* pMem)
{
	cout << "global delete being used\n";
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);

	FixListGap(pHeader);

	if (pHeader->checkValue != 0xDEADC0DE)
		cout << "Header checkValue does not match expected value for " << pHeader << ", something has gone wrong\n";
	else
	{
		cout << "Header " << pHeader << " checkValue correct, freeing memory...\n";
		Tracker::RemoveBytesAllocated(sizeof(pHeader));

		free(pHeader);
	}

}

void operator delete(void* pMem, Tracker* pTracker)
{
	cout << "global delete with tracker being used\n";
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);

	FixListGap(pHeader);

	if(pHeader->checkValue != 0xDEADC0DE)
		cout << "Header checkValue does not match expected value for " << pHeader << ", something has gone wrong\n";
	else
	{
		cout << "Header " << pHeader << " checkValue correct, freeing memory...\n";
		pTracker->RemoveBytesAllocated(sizeof(pHeader));
		free(pHeader);
	}
}
//#endif // DEBUG

void LinkList(Header* pHeader)
{
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
}

void LinkList(Header* pHeader, Tracker* pTracker)
{
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
}

void FixListGap(Header* pHeader)
{
	pHeader->pPrevHeader->pNextHeader = pHeader->pNextHeader;
	pHeader->pNextHeader->pPrevHeader = pHeader->pPrevHeader;
}