#include "GsNetBuffer.h"

FGsNetBuffer::FGsNetBuffer(int32_t size)
	:
	Size(size)
{	
	Buffer = static_cast<uint8_t*>(FMemory::Malloc(Size));
}

FGsNetBuffer::~FGsNetBuffer()
{
	if (nullptr != Buffer)
	{
		FMemory::Free(Buffer);
		Buffer = nullptr;
	}
}
