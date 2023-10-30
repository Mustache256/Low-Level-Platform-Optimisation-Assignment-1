#include "Tracker.h"

size_t Tracker::totalBytesUsed;
Header* Tracker::pPrevHeader;

Tracker::Tracker()
{

}

Tracker::~Tracker()
{

}

void Tracker::AddBytesAllocated(size_t numberOfBytes)
{
	totalBytesUsed += numberOfBytes;
}

void Tracker::RemoveBytesAllocated(size_t numberOfBytes)
{
	totalBytesUsed -= numberOfBytes;
}

size_t Tracker::GetTotalBytesAllocated()
{
	return totalBytesUsed;
}

Header* Tracker::GetPreviousHeader()
{
	return pPrevHeader;
}

void Tracker::SetPreviousHeader(Header* pHeader)
{
	pPrevHeader = pHeader;
}