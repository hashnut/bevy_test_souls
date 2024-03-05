// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Core/Public/Containers/StackTracker.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "GsClassUtil.generated.h"

/**
 * UObject형 클래스 유틸 처리
 */
UCLASS()
class T1PROJECT_API UGsClassUtil : public UObject
{
	GENERATED_BODY()
	
public:
	// UObject 클래스 타입 캐스팅후 check 처리
	template <typename tCast, typename tOrigin>
	static tCast* UCheckCast(tOrigin* Src)
	{
		tCast* dest = Cast<tCast>(Src);
		check(dest);
		return dest;
	}

	// UObject 클래스 타입 캐스팅후 check 처리 (const 형)
	template <typename tCast, typename tOrigin>
	static const tCast* UCheckCast(const tOrigin* Src)
	{
		tCast* dest = Cast<tCast>(Src);
		check(dest);
		return dest;
	}
};

/**
 * 일반 클래스 유틸 처리
 */
class T1PROJECT_API FGsClassUtil
{

public:
	// 일반 클래스 static 캐스팅후 check처리
	template <typename tCast, typename tOrigin>
	static tCast* FCheckStaticCast(tOrigin* Src)
	{
		static_assert(TIsDerivedFrom<tCast, tOrigin>::IsDerived, "tCast must be derived from tOrigin");

		if (nullptr == Src)
			return nullptr;

		tCast* dest = static_cast<tCast*>(Src);
		check(dest);
		return dest;
	}

	// 콜스택 로그창에 출력
	static void PrintCallStack()
	{
		FStackTracker stackTracker(nullptr, nullptr, true);
		stackTracker.CaptureStackTrace();
		stackTracker.DumpStackTraces(0, *GLog);
		stackTracker.ResetTracking();
	}
};

class T1PROJECT_API FGsMemStreamWriter
{
public:
	FGsMemStreamWriter() {}
	explicit FGsMemStreamWriter(size_t capacity) : m_data(new char[capacity])
		, m_capacity(capacity)
		, m_offset(0u)
	{
	}
	~FGsMemStreamWriter(void)
	{
		delete[] m_data;
	}

	void Reset(size_t capacity)
	{
		if (m_data) delete[] m_data;
		m_data = new char[capacity];
		m_capacity = capacity;
		m_offset = 0u;
	}

	void Write(const void* data, size_t size)
	{
		checkf(m_offset + size <= m_capacity, TEXT("Not enough space to write data."));
		memcpy(m_data + m_offset, data, size);
		m_offset += size;
	}

	template <typename T>
	inline void Write(const T& data)
	{
		Write(&data, sizeof(T));
	}

	inline const void* GetData(void) const
	{
		return m_data;
	}

	inline size_t GetSize(void) const
	{
		return m_offset;
	}

private:
	char* m_data;
	size_t m_capacity;
	size_t m_offset;
};