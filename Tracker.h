#pragma once
#include "global.h"

class Tracker
{
public:
	static void AddBytesAllocated(size_t bytes);
	static void RemoveBytesAllocated(size_t bytes);
	static size_t GetBytesAllocated();

	static Header* GetPreviousHeader();
	static void SetPreviousHeader(Header* pHeader);

	static enum type
	{
		base,
		box,
		sound
	};

private:
	static size_t bytesAlloced;
	static Header* pPrevHeader;
};

