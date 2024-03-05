// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsPoolUObject.generated.h"

/**
* UObject 상속 객체 Pool. 
* - 주의: Claim 시 같은 클래스를 사용해야 함
*/
UCLASS()
class T1PROJECT_API UGsPoolUObject final : public UObject
{
	GENERATED_BODY()

private:
	// 데이터 원본
	UPROPERTY()
	TArray<UObject*> _poolArray;

	// 활성화 되지 않은 오브젝트(포인터만 저장함)
	UPROPERTY()
	TArray<UObject*> _inactiveArray;

public:
	virtual void BeginDestroy() override
	{
		RemovePool();
		Super::BeginDestroy();
	}

public:
	template <typename T = UObject>
	T* Claim()
	{
		T* data = nullptr;

		if (0 == _inactiveArray.Num())
		{
			data = NewObject<T>(this);
			_poolArray.Emplace(data);
		}
		else
		{		
			UObject* dataObject = _inactiveArray.Pop();
			data = Cast<T>(dataObject);
		}

		return data;
	}

	template <typename T = UObject>
	void WarmUp(int32 InCount)
	{
		for (int32 i = 0; i < InCount; ++i)
		{
			T* data = data = NewObject<T>(this);
			_poolArray.Emplace(data);
			_inactiveArray.Emplace(data);
		}
	}

	// 하나만 반납
	void ReleaseOne(UObject* InData)
	{
		_inactiveArray.Emplace(InData);
	}

	// 모두 반납
	void ReleaseAll()
	{
		_inactiveArray.Empty();
		
		for (int32 i = 0, maxCount = _poolArray.Num(); i < maxCount; ++i)
		{
			_inactiveArray.Emplace(_poolArray[i]);
		}
	}

	// 풀 내용 전부 삭제
	void RemovePool()
	{
		_inactiveArray.Empty();
		for (UObject* data : _poolArray)
		{
			data = nullptr;
		}
		_poolArray.Empty();
	}
};