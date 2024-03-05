#pragma once

#include "CoreMinimal.h"
#include "T1Project.h"
#if PLATFORM_WINDOWS 
#include <typeinfo>
#endif

//----------------------------------------------------------------
// T타입의 객체를 SharedPtr로 묶어 관리하는 TArray
// 참고)
//	1. 제대로 소멸되지 않았을때 로그 출력함
//	2. 내부에서 TShardedPtr를 통해 관리함으로 가져다 쓰는쪽에서는 Ref카운트를 주의해주세요 (다쓰고 NULL만 호출하면됨)
//  *) Local에선 자연스레 Scope 종료시 RefCount가 감소되지만, 멤버로 가질 경우엔 확실히 RefCount 감소처리를 해주어야 함
//----------------------------------------------------------------

template <typename T>
class TGsSharedArray
{
#if PLATFORM_WINDOWS 
	static_assert(std::is_default_constructible<T>::value, "T must have default-constructor");
#endif

private:
	TArray<TSharedPtr<T>>	Container;

public:
	//소멸처리
	virtual ~TGsSharedArray()
	{
		Clear();
	}

public:
	//----------------------------------------------------------------
	// 소멸시 전체 소멸 
	void Clear()
	{
#if PLATFORM_WINDOWS
		for (auto& a : Container)
		{
			if (false == a.IsUnique())
			{
				GSLOG(Error, TEXT("TGsSharedArray : RefCount Not 1, Reset Call %s"), typeid(T).name());
			}
			else if (false == a.IsValid())
			{
				GSLOG(Error, TEXT("TGsSharedArray : SharedPtr is invalid %s"), typeid(T).name());
			}
		}
#endif
		Container.Empty();
	}

public:
	// ----------------------------------------------------------------
	// 접근자 추가
	TSharedRef<T> operator[] (int index)
	{
		return Container.GetData()[index].ToSharedRef();
	}

public:
	//----------------------------------------------------------------
	const TArray<TSharedPtr<T>>& Get()
	{
		return Container;
	}

public:
	//----------------------------------------------------------------
	// 내부 인스턴스 생성하여 추가후 반환
	virtual TSharedRef<T> MakeInstance()
	{
		T* instance = new T();
		//GSCHECK(instance);
		Container.Add(MakeShareable(instance));
		return Container.Last().ToSharedRef();
	}
	//----------------------------------------------------------------
	// 외부 인스턴스 추가
	virtual void InsertInstance(T* instance)
	{
		GSCHECK(instance);
		Container.Add(MakeShareable(instance));
	}

	//----------------------------------------------------------------
	virtual void Remove(TSharedPtr<T> instance)
	{
		Container.Remove(instance);
	}
};