#pragma once
#include "global.h"

class Tracker
{
public:
	Tracker();
	~Tracker();
	static void AddBytesAllocated(size_t numberOfBytes);
	static void RemoveBytesAllocated(size_t numberOfBytes);
	static size_t GetTotalBytesAllocated();
	static Header* GetPreviousHeader();
	static void SetPreviousHeader(Header* pHeader);

private:
	static size_t totalBytesUsed;
	static Header* pPrevHeader;
};

