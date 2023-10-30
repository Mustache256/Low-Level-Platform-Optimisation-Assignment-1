#pragma once
#include "Tracker.h"
#include "MemoryTracker.h"

struct Header
{
	int size;
	int checkValue;
	Tracker::type pTrackerType;
	Header* pNextHeader;
	Header* pPrevHeader;
};

struct Footer
{
	int reserved;
	int checkValue;
};

void* operator new(size_t size);
//void* operator new(size_t size, Tracker* pTracker);

void operator delete(void* pMem);