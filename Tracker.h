#pragma once
#include <iostream>
#include <vector>
#include "global.h"

using namespace std;

class Tracker
{
public:
	static void AddBytesAlloced(size_t bytes, Header* h);
	static void RemoveBytesAlloced(size_t bytes, Header* h);
	static size_t GetBytesAlloced(int type);
	static size_t GetAllBytesAlloced();

	static Header* GetPreviousHeader();
	static void SetPreviousHeader(Header* pHeader);
	static Header* GetFirstHeader();
	static void SetFirstHeader(Header* pHeader);

	static void WalkTheHeap();

	enum Type
	{
		base,
		box,
		sound
	};

private:
	static vector<size_t> bytesAlloced;
	//static size_t baseBytesAlloced;
	//static size_t boxBytesAlloced;
	//static size_t soundBytesAlloced;
	static Header* pPrevHeader;
	static Header* pFirstHeader;
};

