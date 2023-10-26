#pragma once
#include "Tracker.h"

void* operator new(size_t size);
void* operator new(size_t size, Tracker* pTracker);

void operator delete(void* pMem);
void operator delete(void* pMem, Tracker* pTracker);

struct Header
{
	int size;
	int checkValue;
	Tracker* pTracker;
	Header* pNextHeader;
	Header* pPrevHeader;
};

struct Footer
{
	int reserved;
	int checkValue;
};