#include "Tracker.h"

size_t Tracker::bytesAlloced[count];
Header* Tracker::pPrevHeader;
Header* Tracker::pFirstHeader;

void Tracker::AddBytesAlloced(size_t bytes, Header* h)
{
	//Add bytes to tracker type total
	bytesAlloced[h->type] += bytes;
	//cout << "\nAdded " << bytes << " bytes to the " << h->type << " tracker total\n";
}

void Tracker::RemoveBytesAlloced(size_t bytes, Header* h)
{
	//Checks if there are enough bytes in tracker total to be removed, if so subtracts passed in bytes
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
	//Retrieves all bytes allocated for tracker type
	return bytesAlloced[type];
}

void Tracker::PrintAllBytesAlloced()
{
	cout << "\n-------------\n";
	cout << "\nBase Bytes Allocated: " << bytesAlloced[base];
	cout << "\nBox Bytes Allocated: " << bytesAlloced[box];
	cout << "\nPool Bytes Allocated: " << bytesAlloced[pool];
	cout << "\nPhysics Bytes Allocated: " << bytesAlloced[physics];
	cout << "\nQuad Bytes Allocated: " << bytesAlloced[quad];
	cout << "\n\n-------------\n";
}

Header* Tracker::GetPreviousHeader()
{
	//Retrieve pointer to previously created header
	return pPrevHeader;
}

void Tracker::SetPreviousHeader(Header* pHeader)
{
	//Sets previous header pointer
	pPrevHeader = pHeader;
}

Header* Tracker::GetFirstHeader()
{
	//Get pointer to first header created
	return pFirstHeader;
}

void Tracker::SetFirstHeader(Header* pHeader)
{
	//Set first header pointer
	pFirstHeader = pHeader;
}

void Tracker::WalkTheHeap()
{
	//Loops through the whole linked header list, using the checkValues to ensure memory integrity, if a header checkValue is incorrect, it output an error, otherwise outputs that heap walk was successful
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