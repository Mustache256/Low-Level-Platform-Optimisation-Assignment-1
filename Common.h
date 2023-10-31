#pragma once
#include <iostream>
#include <chrono>

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