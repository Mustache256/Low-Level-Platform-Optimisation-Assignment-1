#pragma once
class MemPool
{
public:
	MemPool(size_t objectSize);
	~MemPool();

	void* Alloc(size_t size);
	void Free(void* p, size_t size);
};

