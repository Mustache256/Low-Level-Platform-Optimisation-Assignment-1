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

	//Tracker types enum
	enum Type
	{
		base,
		box,
		pool,
		physics,
		quad,
		count
	};

private:
	//Array that stores bytes allocated for each tracker type
	static size_t bytesAlloced[count];

	//Header pointers for setting up linked list and finding beginning of list
	static Header* pPrevHeader;
	static Header* pFirstHeader;
};

