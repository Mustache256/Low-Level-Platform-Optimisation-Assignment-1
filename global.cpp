#include "Global.h"

using namespace std;

//#ifdef DEBUG
void* operator new(size_t size)
{
	//Debugging
	//cout << "\n-------------\n";
	//cout << "\nGlobal new being used\n";
	//Calculate total size to allocate with header and footer 
	size_t totalBytes = size + sizeof(Header) + sizeof(Footer);
	
	//Malloc those bytes and get a pointer to the start of the header and footer memory
	char* pMem = (char*)malloc(totalBytes);
	Header* pHeader = (Header*)pMem;
	void* pStartMemAlloced = pMem + sizeof(Header);
	Footer* pFooter = (Footer*)(pMem + sizeof(Header) + size);

	//Assign header variables
	pHeader->size = size;
	pHeader->type = Tracker::base;
	pHeader->checkValue = 0xDEADC0DE;

	//Update tracker previous header and first header pointers depending on whether this is the first header to be made or not, creates the doubly linked list
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

	//Set footer checkValue
	pFooter->checkValue = 0xDEADBEEF;

	//Add allocated bytes to tracker and return pointer to start of memory
	Tracker::AddBytesAlloced(totalBytes, pHeader);
	return pStartMemAlloced;
}

void* operator new(size_t size, Tracker::Type type)
{
	//Debugging
	// << "\n-------------\n";
	//cout << "\nGlobal new being used\n";
	//Calculate total size to allocate with header and footer 
	size_t totalBytes = size + sizeof(Header) + sizeof(Footer);

	//Malloc those bytes and get a pointer to the start of the header and footer memory
	char* pMem = (char*)malloc(totalBytes);
	Header* pHeader = (Header*)pMem;
	void* pStartMemAlloced = pMem + sizeof(Header);
	Footer* pFooter = (Footer*)(pMem + sizeof(Header) + size);

	//Assign header variables
	pHeader->size = size;
	pHeader->type = type;
	pHeader->checkValue = 0xDEADC0DE;

	//Update tracker previous header and first header pointers depending on whether this is the first header to be made or not, creates the doubly linked list
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

	//Set footer checkValue
	pFooter->checkValue = 0xDEADBEEF;

	//Add allocated bytes to tracker and return pointer to start of memory
	Tracker::AddBytesAlloced(totalBytes, pHeader);
	return pStartMemAlloced;
}

void operator delete(void* pMem)
{
	//Debugging
	//cout << "\n-------------\n";
	//cout << "\nglobal delete being used\n";
	//Get pointer to start of memory and to start of footer
	Header* pHeader = (Header*)((char*)pMem - sizeof(Header));
	Footer* pFooter = (Footer*)((char*)pMem + pHeader->size);

	//Update linked list to fix gap that would be left by deleting this boject
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

	//Check header checkValue to ensure no memory has been overwritten
	if (pHeader->checkValue != 0xDEADC0DE)
		cout << "\nHeader checkValue does not match expected value for " << pHeader << ", something has gone wrong\n";
	else
	{
		//Debugging
		//cout << "\nHeader " << pHeader << " checkValue correct, freeing memory...\n";
		//Remove bytes from tracker
		Tracker::RemoveBytesAlloced(sizeof(pHeader), pHeader);
		//Free bytes that were being used by this object
		free(pHeader);
	}
}
//#endif // DEBUG