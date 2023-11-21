#include "Tracker.h"

size_t Tracker::bytesAlloced[count];
Header* Tracker::pPrevHeader;
Header* Tracker::pFirstHeader;

void Tracker::AddBytesAlloced(size_t bytes, Header* h)
{
	bytesAlloced[h->type] += bytes;
	//cout << "\nAdded " << bytes << " bytes to the " << h->type << " tracker total\n";
}

void Tracker::RemoveBytesAlloced(size_t bytes, Header* h)
{
	if ((bytesAlloced[h->type] - bytes) < 0)
	{
		cerr << "\nTrying to remove more memory from the " << h->type << " tracker and has been counted by that tracker\n";
	}
	else
	{
		bytesAlloced[h->type] += bytes;
		//cout << "\nRemoved " << bytes << " bytes from the " << h->type << " tracker total\n";
	}
}

size_t Tracker::GetBytesAlloced(int type)
{
	return bytesAlloced[type];
}

void Tracker::PrintAllBytesAlloced()
{
	cout << "\n-------------\n";
	cout << "\nBase Bytes Allocated: " << bytesAlloced[base];
	cout << "\nBox Bytes Allocated: " << bytesAlloced[box];
	cout << "\n\n-------------\n";
}

Header* Tracker::GetPreviousHeader()
{
	return pPrevHeader;
}

void Tracker::SetPreviousHeader(Header* pHeader)
{
	pPrevHeader = pHeader;
}

Header* Tracker::GetFirstHeader()
{
	return pFirstHeader;
}

void Tracker::SetFirstHeader(Header* pHeader)
{
	pFirstHeader = pHeader;
}

void Tracker::WalkTheHeap()
{
	chrono::steady_clock::time_point begin = chrono::steady_clock::now();

	Header* pHeader = Tracker::GetFirstHeader();

	cout << "\n-------------\nWalking...\n";

	if (pHeader == nullptr)
	{
		cerr << "\nPointer to first header in list is null, check pointer logic\n";
		return;
	}

	while (pHeader != nullptr)
	{
		if (pHeader->checkValue == 0xDEADC0DE)
		{
			pHeader = pHeader->pNextHeader;
		}
		else
		{
			cerr << "\nIncorrect checkValue found for header address " << pHeader << ", something has gone wrong\n-------------\n";
			return;
		}
	}

	cout << "\nAll headers have correct checkValues, memory integrity maintained\n";

	chrono::steady_clock::time_point end = chrono::steady_clock::now();

	cout << "\nTook: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " microseconds to walk\n-------------\n";
}