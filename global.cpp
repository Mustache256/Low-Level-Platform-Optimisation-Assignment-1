#include "Global.h"

using namespace std;

//#ifdef DEBUG
void* operator new(size_t size)
{
	cout << "\n-------------\n";
	cout << "\nGlobal new being used\n";
	size_t totalBytes = size + sizeof(Header) + sizeof(Footer);
	
	char* pMem = (char*)malloc(totalBytes);
	Header* pHeader = (Header*)pMem;
	void* pStartMemAlloced = pMem + sizeof(Header);
	Footer* pFooter = (Footer*)(pMem + sizeof(Header) + size);

	pHeader->size = size;
	pHeader->type = Tracker::base;
	pHeader->checkValue = 0xDEADC0DE;

	if (Tracker::GetPreviousHeader() != nullptr)
	{
		pHeader->pNextHeader = nullptr;
		pHeader->pPrevHeader = Tracker::GetPreviousHeader();
		pHeader->pPrevHeader->pNextHeader = pHeader;
		Tracker::SetPreviousHeader(pHeader);
	}
	else
	{
		pHeader->pPrevHeader = nullptr;
		pHeader->pNextHeader = nullptr;
		Tracker::SetPreviousHeader(pHeader);
		Tracker::SetFirstHeader(pHeader);
	}

	pFooter->checkValue = 0xDEADBEEF;

	Tracker::AddBytesAlloced(totalBytes, pHeader);
	return pStartMemAlloced;
}

void* operator new(size_t size, Tracker::Type type)
{
	cout << "\n-------------\n";
	cout << "\nGlobal new being used\n";
	size_t totalBytes = size + sizeof(Header) + sizeof(Footer);

	char* pMem = (char*)malloc(totalBytes);
	Header* pHeader = (Header*)pMem;
	void* pStartMemAlloced = pMem + sizeof(Header);
	Footer* pFooter = (Footer*)(pMem + sizeof(Header) + size);

	pHeader->size = size;
	pHeader->type = type;
	pHeader->checkValue = 0xDEADC0DE;

	if (Tracker::GetPreviousHeader() != nullptr)
	{
		pHeader->pNextHeader = nullptr;
		pHeader->pPrevHeader = Tracker::GetPreviousHeader();
		pHeader->pPrevHeader->pNextHeader = pHeader;
		Tracker::SetPreviousHeader(pHeader);
	}
	else
	{
		pHeader->pPrevHeader = nullptr;
		pHeader->pNextHeader = nullptr;
		Tracker::SetPreviousHeader(pHeader);
		Tracker::SetFirstHeader(pHeader);
	}

	pFooter->checkValue = 0xDEADBEEF;

	Tracker::AddBytesAlloced(totalBytes, pHeader);
	return pStartMemAlloced;
}

void operator delete(void* pMem)
{
	cout << "\n-------------\n";
	cout << "\nglobal delete being used\n";
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);

	if (pHeader->pPrevHeader != nullptr)
	{
		pHeader->pPrevHeader->pNextHeader = pHeader->pNextHeader;

		if(pHeader->pNextHeader != nullptr)
			pHeader->pNextHeader->pPrevHeader = pHeader->pPrevHeader;
	}
	else
	{
		if (pHeader->pNextHeader != nullptr)
		{
			pHeader->pNextHeader->pPrevHeader = nullptr;
			Tracker::SetFirstHeader(pHeader->pNextHeader);
		}
		else
			Tracker::SetFirstHeader(nullptr);
	}

	if (pHeader->checkValue != 0xDEADC0DE)
		cout << "\nHeader checkValue does not match expected value for " << pHeader << ", something has gone wrong\n";
	else
	{
		cout << "\nHeader " << pHeader << " checkValue correct, freeing memory...\n";
		Tracker::RemoveBytesAlloced(sizeof(pHeader), pHeader);

		free(pHeader);
	}
}
//#endif // DEBUG