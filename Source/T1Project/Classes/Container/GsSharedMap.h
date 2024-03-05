#pragma once

#include "CoreMinimal.h"
#include "T1Project.h"
#if PLATFORM_WINDOWS 
#include <typeinfo>
#endif

//----------------------------------------------------------------
// TObject형 할당자(고정 객체가 아닐수 있기에 할당자를 지정해야함
//----------------------------------------------------------------
template <typename TEnumKey, typename TObject>
class T1PROJECT_API TGsSharedMapAllocator
{
public:
	TGsSharedMapAllocator() {}
	virtual ~TGsSharedMapAllocator() {}

public:
	//----------------------------------------------------------------
	virtual TObject* Alloc(TEnumKey /*type*/)
	{
		return new TObject();
	}
};

//----------------------------------------------------------------
// TEnum의 Key에 해당하는 TObject타입의 객체를 SharedPtr로 묶어 관리하는 TMap
// 참고)
//	1. 제대로 소멸되지 않았을때 로그 출력함
//	2. 내부에서 TShardedPtr를 통해 관리함으로 가져다 쓰는쪽에서는 Ref카운트를 주의해주세요
//  *) Local에선 자연스레 Scope 종료시 RefCount가 감소되지만, 멤버로 가질 경우엔 확실히 RefCount 감소처리를 해주어야 함
//----------------------------------------------------------------
template <typename TEnumKey, typename TObject, typename TAllocator = TGsSharedMapAllocator<TEnumKey, TObject>>
class T1PROJECT_API TGsSharedMap
{
	static_assert(TIsEnum<TEnumKey>::Value, "TEnumKey is not enumerator type");
#if PLATFORM_WINDOWS 
	static_assert(std::is_default_constructible<TObject>::value, "TObject must have default-constructor");
#endif

private:
	TUniquePtr<TAllocator>				Allocator;
	TMap<TEnumKey, TSharedPtr<TObject>> Container;

public:
	TGsSharedMap()
	{ 
		Allocator = TUniquePtr<TAllocator>(new TAllocator());
	}

	virtual ~TGsSharedMap()
	{ 
		Clear();
	}

public:
	//---------------------------------------------------------------
	void Clear()
	{
#if PLATFORM_WINDOWS
		for (auto& a : Container)
		{
			if (false == a.Value.IsUnique())
			{
				GSLOG(
					Error, 
					TEXT("TGsSharedArray : RefCount Not 1, Reset Call Key[%s] Value[%s]"), 
					typeid(TEnumKey).name(), typeid(TObject).name()
				);
			}
			else if (false == a.Value.IsValid())
			{
				GSLOG(
					Error, 
					TEXT("TGsSharedArray : SharedPtr is invalid Key[%s] Value[%s]"), 
					typeid(TEnumKey).name(), typeid(TObject).name()
				);
			}
		}
#endif
		Container.Empty();
	}

public:
	//----------------------------------------------------------------
	virtual TSharedPtr<TObject> Find(TEnumKey inKey)
	{
		auto instance = Container.FindRef(inKey);
		return instance;
	}

public:
	//----------------------------------------------------------------
	// 내부 인스턴스 생성하여 추가후 반환
	virtual TSharedPtr<TObject> MakeInstance(TEnumKey inKey)
	{
		if (false == Allocator.IsValid())
			return nullptr;

		TObject* instance = Allocator.Get()->Alloc(inKey);
		if (nullptr == instance)
		{
#if PLATFORM_WINDOWS
			GSLOG(
				Error, 
				TEXT("TGsSharedMap : _allocator MakeInstance NULL, Call Key[%s] Value[%s]"), 
				typeid(TEnumKey).name(), typeid(TObject).name()
			);
#endif
			return nullptr;
		}

		TSharedPtr<TObject> shared_instance = MakeShareable(instance);
		Container.Add(inKey, shared_instance);

		return shared_instance;
	}

	//----------------------------------------------------------------
	virtual void Remove(TEnumKey inKey)
	{
		Container.FindAndRemoveChecked(inKey);
	}


protected:
	// 외부에서 for 문 돌릴때 사용할려고...
	TMap<TEnumKey, TSharedPtr<TObject>>& GetContainer() { return Container; }
};