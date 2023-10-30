#pragma once
#include "Tracker.h"

void* operator new(size_t size);
void* operator new(size_t size, Tracker* pTracker);

void operator delete(void* pMem);
void operator delete(void* pMem, Tracker* pTracker);

void LinkList(Header* pHeader);
void LinkList(Header* pHeader, Tracker* pTracker);
void FixListGap(Header* pHeader);

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