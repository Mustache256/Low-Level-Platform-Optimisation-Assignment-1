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
	pHeader->type = Tracker::base;
	pHeader->checkValue = 0xDEADC0DE;

	ConstructList(pHeader);

	pFooter->checkValue = 0xDEADBEEF;

	Tracker::AddBytesAlloced(totalBytes, pHeader);
	return pStartMemAlloced;
}

void* operator new(size_t size, Tracker::Type type)
{
	cout << "global new being used\n";
	size_t totalBytes = size + sizeof(Header) + sizeof(Footer);

	char* pMem = (char*)malloc(totalBytes);
	Header* pHeader = (Header*)pMem;
	void* pStartMemAlloced = pMem + sizeof(Header);
	Footer* pFooter = (Footer*)(pMem + sizeof(Header) + size);

	pHeader->size = size;
	pHeader->type = type;
	pHeader->checkValue = 0xDEADC0DE;

	ConstructList(pHeader);

	pFooter->checkValue = 0xDEADBEEF;

	Tracker::AddBytesAlloced(totalBytes, pHeader);
	return pStartMemAlloced;
}

void operator delete(void* pMem)
{
	cout << "global delete being used\n";
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);

	if (pHeader->pPrevHeader != nullptr)
	{
		pHeader->pPrevHeader->pNextHeader = pHeader->pNextHeader;
		pHeader->pNextHeader->pPrevHeader = pHeader->pPrevHeader;
	}
	else
	{
		pHeader->pNextHeader->pPrevHeader = nullptr;
		Tracker::SetFirstHeader(pHeader->pNextHeader);
	}

	if (pHeader->checkValue != 0xDEADC0DE)
		cout << "Header checkValue does not match expected value for " << pHeader << ", something has gone wrong\n";
	else
	{
		cout << "Header " << pHeader << " checkValue correct, freeing memory...\n";
		Tracker::RemoveBytesAlloced(sizeof(pHeader), pHeader);

		free(pHeader);
	}
}

void ConstructList(Header* pHeader)
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
		Tracker::SetFirstHeader(pHeader);
	}
}
//#endif // DEBUG