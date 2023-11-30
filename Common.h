#pragma once
#include <iostream>
#include <chrono>

//Header and footer struct used for memory tracking and heap walking
struct Header
{
	size_t size;
	int checkValue;
	int type;
	//Double linked list header pointers
	Header* pNextHeader;
	Header* pPrevHeader;
};

struct Footer
{
	int checkValue;
};