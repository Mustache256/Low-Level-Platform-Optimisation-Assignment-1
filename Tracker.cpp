#include "Tracker.h"

vector<size_t> Tracker::bytesAlloced;
//size_t Tracker::baseBytesAlloced;
//size_t Tracker::boxBytesAlloced;
//size_t Tracker::soundBytesAlloced;
Header* Tracker::pPrevHeader;
Header* Tracker::pFirstHeader;

void Tracker::AddBytesAlloced(size_t bytes, Header* h)
{
	bytesAlloced[h->type] += bytes;
	cout << "\nAdded " << bytes << " bytes to the " << h->type << " tracker total";

	/*switch (h->type)
	{
	case base:
		bytesAlloced[0] += bytes;
		baseBytesAlloced += bytes;
		cout << "\nAdded " << bytes << " bytes to the base tracker total";
		break;
	case box:
		boxBytesAlloced += bytes;
		cout << "\nAdded " << bytes << " bytes to the box tracker total";
		break;
	case sound:
		soundBytesAlloced += bytes;
		cout << "\nAdded " << bytes << " bytes to the sound tracker total";
		break;
	}*/
}

void Tracker::RemoveBytesAlloced(size_t bytes, Header* h)
{
	if ((bytesAlloced[h->type] - bytes) < 0)
	{
		cerr << "\nTrying to remove more memory from the " << h->type << " tracker and has been counted by that tracker";
	}
	else
	{
		bytesAlloced[h->type] += bytes;
		cout << "\nRemoved " << bytes << " bytes from the " << h->type << " tracker total";
	}

	/*switch (h->type)
	{
	case base:
		baseBytesAlloced -= bytes;
		cout << "\Removed " << bytes << " bytes from the base tracker total";
		break;
	case box:
		boxBytesAlloced -= bytes;
		cout << "\Removed " << bytes << " bytes from the box tracker total";
		break;
	case sound:
		soundBytesAlloced -= bytes;
		cout << "\Removed " << bytes << " bytes from the sound tracker total";
		break;
	}*/
}

size_t Tracker::GetBytesAlloced(int type)
{
	return bytesAlloced[type];
}

size_t Tracker::GetAllBytesAlloced()
{
	size_t total;

	for (int i = 0; i < bytesAlloced.size(); i++)
	{
		total += bytesAlloced[i];
	}

	return total;
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
	Header* pHeader = Tracker::GetFirstHeader();

	if (pHeader == nullptr)
	{
		cerr << "\nPointer to first header in list is null, check pointer logic";
		return;
	}

	while (pHeader != nullptr)
	{
		//cout << "\nChecking header for checkValue" << 0xDEADC0DE;

		if (pHeader->checkValue == 0xDEADC0DE)
		{
			//cout << "\nHeader checkValue correct, continuing down list";
			pHeader = pHeader->pNextHeader;
		}
		else
		{
			cerr << "\nIncorrect checkValue found for header address " << pHeader << ", something has gone wrong";
			return;
		}
	}

	cout << "\nAll headers have correct checkValues, memory integrity maintained";
}