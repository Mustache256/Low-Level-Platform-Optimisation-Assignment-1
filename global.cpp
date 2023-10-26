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

	pTracker->AddBytesAllocated(totalBytes);
	return pStartMemAlloced;
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
		Tracker::RemoveBytesAllocated(sizeof(pHeader));

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