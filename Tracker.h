#pragma once
#include <iostream>
#include <vector>
#include "Common.h"

using namespace std;

class Tracker
{
public:
	static void AddBytesAlloced(size_t bytes, Header* h);
	static void RemoveBytesAlloced(size_t bytes, Header* h);
	static size_t GetBytesAlloced(int type);
	static void PrintAllBytesAlloced();

	static Header* GetPreviousHeader();
	static void SetPreviousHeader(Header* pHeader);
	static Header* GetFirstHeader();
	static void SetFirstHeader(Header* pHeader);

	static void WalkTheHeap();

	enum Type
	{
		base,
		box,
		pool,
		count
	};

private:
	static size_t bytesAlloced[count];
	static Header* pPrevHeader;
	static Header* pFirstHeader;
};

