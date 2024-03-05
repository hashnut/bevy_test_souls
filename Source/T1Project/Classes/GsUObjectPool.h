#pragma once

#include "CoreMinimal.h"

#include "GsUObjectPool.generated.h"

/**
 * UObject 풀링 클래스
 * 각 클래스별로 풀을 관리 (풀의 기본 사이즈는 4 로 잡았다) -> 사용처가 '단일'일때만 정상동작함에 유의
 * 오브젝트 풀링의 형식을 띄고 있으므로, 원칙적으로는 풀에 들어갈 요소의 타입이 정확히 일치하는지 검사해야 하나,
 * 상속 구조를 활용하고 싶을 수도 있으므로 타입을 엄격하게 검사하지는 않기로 했다.
*/
#define DEFAULT_UOBJECT_POOL_SIZE 4

UCLASS()
class T1PROJECT_API UGsUObjectPool final : public UObject
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TArray<UObject*> Pool;

	/** 현재 사용 가능한 Index */
	int32 UseIndex = -1;

public:
	template <typename Type>
	void InitPool(int32 InSize = DEFAULT_UOBJECT_POOL_SIZE)
	{
		static_assert(TIsDerivedFrom<Type, UObject>::IsDerived, "Type should be derived from UObject type!");
		Pool.Empty(InSize);
		for (int32 i = 0; i < InSize; ++i)
		{
			Pool.Emplace(NewObject<Type>(this));
		}
		UseIndex = 0;
	}

	/** 객체가 필요하면 Pop 을 사용하면 된다 */
	template <typename Type>
	Type* Pop()
	{
		static_assert(TIsDerivedFrom<Type, UObject>::IsDerived, "Type should be derived from UObject type!");
		Type* obj = nullptr;

		// 풀을 사용함
		if (UseIndex > -1 && UseIndex < Pool.Num())
		{
			obj = Pool[UseIndex];
			++UseIndex;
		}
		// 풀이 비어 있거나 꽉 참
		else
		{
			obj = NewObject<Type>(this);
			Pool.Emplace(obj);
			UseIndex = Pool.Num();
		}
		
		return obj;
	}

	/** 풀에 객체를 반납한다 */
	template <typename Type>
	void Push(Type* InObj)
	{
		static_assert(TIsDerivedFrom<Type, UObject>::IsDerived, "Type should be derived from UObject type!");
		--UseIndex;
	}

	/** 풀 자체를 관리하는 클래스가 있다면, 사용완료 시 명시적으로 풀을 비워줘도 된다 */
	void EmptyPool()
	{
		Pool.Empty();

		// 풀은 재활용이 가능하다
		UseIndex = -1;
	}
};