#pragma once

#include "CoreMinimal.h"

//bak1210 
//client -side에서 1회성 버프 발송하기위한 클래스 (패킷 조립을 용이하게 하기위함)

class FGsNetBuffer
{
	uint8_t*		Buffer = nullptr;
	const int32_t	Size = 0;

public:
	explicit FGsNetBuffer(int32_t size);
	~FGsNetBuffer();

public:
	void* operator new(size_t size)
	{
		return FMemory::Malloc(size);
	}
	void operator delete(void* ptr)
	{
		return FMemory::Free(ptr);
	}

public:
	uint8_t* GetBuffer() { return Buffer; }
	const uint8_t* GetBuffer() const { return Buffer; }
	int32_t GetSize() const { return Size; }
};