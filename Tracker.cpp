#include "Tracker.h"

size_t Tracker::bytesAlloced;
Header* Tracker::pPrevHeader;

void Tracker::AddBytesAllocated(size_t bytes)
{
	bytesAlloced += bytes;
}

void Tracker::RemoveBytesAllocated(size_t bytes)
{
	bytesAlloced -= bytes;
}

size_t Tracker::GetBytesAllocated()
{
	return bytesAlloced;
}

Header* Tracker::GetPreviousHeader()
{
	return pPrevHeader;
}

void Tracker::SetPreviousHeader(Header* pHeader)
{
	pPrevHeader = pHeader;
}