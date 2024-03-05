#pragma once

#ifdef _SERVER
#include "TechShared/Shared/SharedInclude/SharedPrimitiveTypes.h"
#include "TechShared/Shared/SharedInclude/PacketIterator.hpp"
#else
#include <wchar.h>
#include "SharedPrimitiveTypes.h"
#include "PacketIterator.hpp"
#endif

#ifndef _SERVER
#	ifndef _ASSERT_CRASH
#		define _ASSERT_CRASH(...)
#	endif
#	ifndef _CLIENT_NETWORK_PACKET_ERROR
#		define _CLIENT_NETWORK_PACKET_ERROR(clientNetworkPacketErrorTypeValue) UE_LOG(LogTemp, Error, TEXT("%s"), *ClientNetworkPacketErrorTypeEnumToString(clientNetworkPacketErrorTypeValue))
#	endif
#else
#	ifndef _CLIENT_NETWORK_PACKET_ERROR
#		define _CLIENT_NETWORK_PACKET_ERROR(...)
#	endif
#	ifndef UENUM
#		define UENUM(...)
#	endif
#endif

UENUM()
enum class ClientNetworkPacketErrorType : uint8
{
	NONE,
	PACKET_SIZE_TOO_SMALL = 0,			// 패킷 최소 크기보다 작음.
	PACKET_SIZE_TOO_BIG,				// 패킷 크기가 너무 커졌음.
	PACKET_SIZE_TOO_BIG_WRITE_STRING,	// 문자열 쓰기 중, 패킷 크기가 너무 커졌음.
	STRING_LENGTH_TOO_LONG,				// 문자열 길이가 너무 길다.
	PACKET_SIZE_TOO_BIG_WRITE_BINARY,	// 바이너리 쓰기 중, 패킷 크기가 너무 커졌음.
	MAX
};

#ifndef _SERVER
inline FString ClientNetworkPacketErrorTypeEnumToString(ClientNetworkPacketErrorType value)
{
	switch (value)
	{
	case ClientNetworkPacketErrorType::PACKET_SIZE_TOO_SMALL: return TEXT("PACKET_SIZE_TOO_SMALL, 패킷 최소 크기보다 작음");
	case ClientNetworkPacketErrorType::PACKET_SIZE_TOO_BIG: return TEXT("PACKET_SIZE_TOO_BIG, 패킷 크기가 너무 커졌음");
	case ClientNetworkPacketErrorType::PACKET_SIZE_TOO_BIG_WRITE_STRING: return TEXT("PACKET_SIZE_TOO_BIG_WRITE_STRING, 문자열 쓰기 중, 패킷 크기가 너무 커졌음.");
	case ClientNetworkPacketErrorType::STRING_LENGTH_TOO_LONG: return TEXT("STRING_LENGTH_TOO_LONG, 문자열 길이가 너무 길다.");
	case ClientNetworkPacketErrorType::PACKET_SIZE_TOO_BIG_WRITE_BINARY: return TEXT("PACKET_SIZE_TOO_BIG_WRITE_BINARY, 바이너리 쓰기 중, 패킷 크기가 너무 커졌음.");
	default: break;
	}
	return FString();
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 패킷 쓰기 기능 구조체
// 자동 생성되는 패킷 쓰기 구조체("패킷이름_WRITE")에서 이 구조체를 사용하여 메모리에 패킷 내용을 입력한다.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct PktWriter
{
public:
	static constexpr int MAX_STRING_LENGTH = 16384;	// 문자열 최대 길이 (인앱 결제 Bypass 영수증 정보가 800자 넘음)


public:
	uchar* mBuf;			// 쓰기용 메모리 포인터 시작 지점
	uint16* mCurrentSize;	// 쓰기 진행 중인 오프셋 용도이자 패킷의 크기(버퍼의 첫 2바이트)
	unsigned int	mBufSize;		// 버퍼의 최대 사이즈


public:
	explicit PktWriter(void* buf, unsigned int bufSize)
		:
		mBuf(reinterpret_cast<uchar*>(buf)),
		mCurrentSize((uint16*)buf),
		mBufSize(bufSize)
	{
		if (bufSize < 4)
		{
			_CLIENT_NETWORK_PACKET_ERROR(ClientNetworkPacketErrorType::PACKET_SIZE_TOO_SMALL); // 패킷 최소 크기보다 작음
			_ASSERT_CRASH(false);
		}

		*mCurrentSize = 0;
		*this << *mCurrentSize;
	}


public:
	template <typename T>
	PktWriter& operator << (const T& a)
	{
		if (*mCurrentSize + sizeof(T) > mBufSize)
		{
			_CLIENT_NETWORK_PACKET_ERROR(ClientNetworkPacketErrorType::PACKET_SIZE_TOO_BIG); // 패킷 크기가 너무 커졌음
			_ASSERT_CRASH(false);
		}

		*((T*)(mBuf + *mCurrentSize)) = a;
		*mCurrentSize += sizeof(T);

		return *this;
	}


public:
#ifdef _SERVER
	// "string" 노드에대한 처리 용도. 서버용 문자열 쓰기 함수.
	PktWriter& WriteString(const wchar_t* str)
	{
		size_t len = wcslen(str) + 1;

		// 글자수 * 2바이트로 만들어주기
		len *= sizeof(wchar_t);

		if (*mCurrentSize + len > mBufSize)
		{
			// 서버용 함수, _CLIENT_NETWORK_PACKET_ERROR 필요 없음.
			_ASSERT_CRASH_FMT(L"");
		}

		wcscpy_s((wchar_t*)(mBuf + *mCurrentSize), len, str);
		*mCurrentSize += static_cast<uint16>(len);

		return *this;
	}
#else
	PktWriter& WriteString(const wchar_t* str)
	{
		return WriteString(ConvertToFString(str));
	}

	PktWriter& WriteString(const FString& str)
	{
		int32 SrcLen = str.Len() + 1;
		int32 DestLen = FPlatformString::ConvertedLength<WIDECHAR>(*str, SrcLen);

		if (*mCurrentSize + (sizeof(WIDECHAR) * DestLen) > mBufSize)
		{
			_CLIENT_NETWORK_PACKET_ERROR(ClientNetworkPacketErrorType::PACKET_SIZE_TOO_BIG_WRITE_STRING); // 문자열 쓰기 중, 패킷 크기가 너무 커졌음.
			_ASSERT_CRASH(false);
		}

		if (MAX_STRING_LENGTH <= DestLen)
		{
			_CLIENT_NETWORK_PACKET_ERROR(ClientNetworkPacketErrorType::STRING_LENGTH_TOO_LONG); // 문자열 길이가 너무 길다
			_ASSERT_CRASH(false);

			FMemory::Memcpy((WIDECHAR*)(mBuf + *mCurrentSize), L"", sizeof(WIDECHAR));
			*mCurrentSize += sizeof(WIDECHAR);
			return *this;
		}

		TArray<WIDECHAR> Converted;
		Converted.AddUninitialized(DestLen);
		FPlatformString::Convert(Converted.GetData(), DestLen, *str, SrcLen);

		FMemory::Memcpy((WIDECHAR*)(mBuf + *mCurrentSize), Converted.GetData(), sizeof(WIDECHAR) * (DestLen));
		*mCurrentSize += static_cast<uint16>(sizeof(WIDECHAR) * DestLen);

		return *this;
	}
#endif


public:
	// "bin" 노드에 대한 처리 용도.
	void WriteBinary(uint16 size, const uchar* binData)
	{
		if (size == 0)
			return;

		if ((*mCurrentSize + size) > uint16(mBufSize))
		{
			_CLIENT_NETWORK_PACKET_ERROR(ClientNetworkPacketErrorType::PACKET_SIZE_TOO_BIG_WRITE_BINARY); // 바이너리 쓰기 중, 패킷 크기가 너무 커졌음.
			_ASSERT_CRASH(false);
		}

#ifdef _SERVER
		memcpy_s(mBuf + *mCurrentSize, mBufSize - *mCurrentSize, binData, size);
#else
		FMemory::Memcpy(mBuf + *mCurrentSize, binData, size);
#endif
		* mCurrentSize = *mCurrentSize + size;
	}

	// 내용을 쓰지않고, 건너뛰는 기능.
	void Skip(unsigned int size) { *mCurrentSize = static_cast<uint16>(size + *mCurrentSize); }


public:
	// 현재 쓰기 진행 중인 위치의 메모리 포인터를 반환한다.
	uchar* GetBuffer() const noexcept { return (mBuf + *mCurrentSize); }

	// 패킷의 크기(첫 2바이트)를 반환한다.
	int GetPktSize() const noexcept { return *mCurrentSize; }
#ifndef _SERVER


public:
	static const FString ConvertToFString(const wchar_t* from)
	{
		return FString::Printf(TEXT("%s"), from);
	}
#endif

	uint16 GetPacketId() const noexcept
	{
		uint16 pktId = *(uint16*)(mBuf + sizeof(uint16));
		return pktId;
	}

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool ValidateString(uint16 offset, uint16 maxLen, const uchar* start, uint16 pktSize, uint16& dataSize)
{
	if (!offset || pktSize < offset)
		return false;

	uint16 len = 0;

	// sizeof(wchar_t)는 IOS에서 4바이트로 잡히기 때문에 sizeof(uint16)으로 조치합니다.
	for (uint16 i = offset; true; i += sizeof(uint16))
	{
		if (i >= pktSize)
			return false;

		if (*((uint16*)(start + i)) == 0)
		{
			// sizeof(wchar_t)는 IOS에서 4바이트로 잡히기 때문에 sizeof(uint16)으로 조치합니다.
			len = (i - offset) / sizeof(uint16);
			break;
		}
	}

	if (maxLen && len > maxLen)
		return false;

	// sizeof(wchar_t)는 IOS에서 4바이트로 잡히기 때문에 sizeof(uint16)으로 조치합니다.
	uint16 recvLen = 0;
	if (len == 0)
		recvLen = 1;	// 빈 문자열이여도 널값 때문에 한글자
	else
		recvLen = len + 1;	// 문자열이 존재해도 마지막 널값 대문에 한글자 추가.

	dataSize = dataSize + (recvLen * sizeof(uint16));

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool ValidateBinary(uint16 offset, uint16 len, uint16 maxLen, uint16 pktSize, uint16& dataSize)
{
	if (0 == offset || pktSize < offset)
		return false;

	if (len)
	{
		if (offset + len <= offset || pktSize < offset + len)
			return false;

		if (maxLen && len > maxLen)
			return false;

		dataSize = dataSize + len;
	}

	return true;
}


