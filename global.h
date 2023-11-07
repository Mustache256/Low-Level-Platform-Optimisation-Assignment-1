#pragma once
#include "Tracker.h"
#include "Common.h"

void* operator new(size_t size);
void* operator new(size_t size, Tracker::Type type);

void operator delete(void* pMem);