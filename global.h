#pragma once
#include "Tracker.h"

struct Header
{
	size_t size;
	int checkValue;
	int type;
	Header* pNextHeader;
	Header* pPrevHeader;
};

struct Footer
{
	int reserved;
	int checkValue;
};

void* operator new(size_t size);
void* operator new(size_t size, Tracker::Type type);

void operator delete(void* pMem);

void ConstructList(Header* pHeader);