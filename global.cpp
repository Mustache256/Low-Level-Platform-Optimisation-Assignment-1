#include <iostream>

#include "global.h"

using namespace std;

void* operator new(size_t size)
{
	cout << "global new being used\n";
	size_t nRequestedBytes = size + sizeof(Header) + sizeof(Footer);
	char* pMem = (char*)malloc(nRequestedBytes);
	Header* pHeader = (Header*)pMem;

	pHeader->size = size;

	void* pFooterAddr = pMem + sizeof(Header) + size;
	Footer* pFooter = (Footer*)pFooterAddr;

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
	free(pHeader);
}

void operator delete(void* pMem, Tracker* pTracker)
{
	cout << "global delete with tracker being used\n";
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);
	free(pHeader);
}