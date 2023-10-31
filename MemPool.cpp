#include "MemPool.h"

MemPool::MemPool(size_t objectSize, size_t poolSize)
{
	sliceSize = objectSize;


}

MemPool::~MemPool()
{

}

void* MemPool::Alloc(size_t size)
{
	return nullptr;
}

void MemPool::Free(void* p, size_t size)
{

}