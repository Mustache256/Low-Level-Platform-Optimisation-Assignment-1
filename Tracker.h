#pragma once
class Tracker
{
public:
	void AddBytesAllocated(size_t numberOfBytes);
	void RemoveBytesAllocated(size_t numberOfBytes);
};

