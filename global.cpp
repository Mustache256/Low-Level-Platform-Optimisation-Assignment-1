#include <iostream>

#include "global.h"

using namespace std;

//#ifdef DEBUG
void* operator new(size_t size)
{
	cout << "global new being used\n";
	size_t nRequestedBytes = size + sizeof(Header) + sizeof(Footer);
	Tracker::AddBytesAllocated(nRequestedBytes);
	char* pMem = (char*)malloc(nRequestedBytes);
	Header* pHeader = (Header*)pMem;

	pHeader->size = size;
	pHeader->checkValue = 0xDEADC0DE;
	pHeader->pTracker = nullptr;

	void* pFooterAddr = pMem + sizeof(Header) + size;
	Footer* pFooter = (Footer*)pFooterAddr;

	pFooter->checkValue = 0xDEADBEEF;

	void* pStartMemBlock = pMem + sizeof(Header);
	return pStartMemBlock;
}

void* operator new(size_t size, Tracker* pTracker)
{
	cout << "global new with tracker being used\n";
	size_t nRequestedBytes = size + sizeof(Header) + sizeof(Footer);
	pTracker->AddBytesAllocated(nRequestedBytes);
	char* pMem = (char*)malloc(nRequestedBytes);
	Header* pHeader = (Header*)pMem;

	pHeader->size = size;
	pHeader->pTracker = pTracker;

	void* pFooterAddr = pMem + sizeof(Header) + size;
	Footer* pFooter = (Footer*)pFooterAddr;

	void* pStartMemBlock = pMem + sizeof(Header);
	return pStartMemBlock;
}

void operator delete(void* pMem)
{
	cout << "global delete being used\n";
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);

	if (pHeader->checkValue != 0xDEADC0DE)
		cout << "Header checkValue does not match expected value for " << pHeader << ", something has gone wrong\n";
	else
	{
		cout << "Header " << pHeader << " checkValue correct, freeing memory...\n";

		if (pHeader->pTracker != nullptr)
			pHeader->pTracker->RemoveBytesAllocated(sizeof(pHeader));

		free(pHeader);
	}
		
}

void operator delete(void* pMem, Tracker* pTracker)
{
	cout << "global delete with tracker being used\n";
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);

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