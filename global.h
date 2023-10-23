#pragma once
void* operator new(size_t size);

void operator delete(void* pMem);

struct Header
{
	int size;
	int checkValue;
	Header* pNext;
	Header* pPrev;
};

struct Footer
{
	int reserved;
};