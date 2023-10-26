#pragma once
class Tracker
{
public:
	Tracker();
	~Tracker();
	static void AddBytesAllocated(size_t numberOfBytes);
	static void RemoveBytesAllocated(size_t numberOfBytes);
	static size_t GetTotalBytesAllocated();

private:
	static size_t totalBytesUsed;

};

