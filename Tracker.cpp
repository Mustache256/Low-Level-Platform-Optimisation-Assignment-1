#include "Tracker.h"

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