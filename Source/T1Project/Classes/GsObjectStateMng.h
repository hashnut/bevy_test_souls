// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsObjectState.h"
#include "GsObjectStateMng.generated.h"

/**
 * UObject State를 관리하는 StateManager.
 */
UCLASS()
class T1PROJECT_API UGsObjectStateMng : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	TWeakObjectPtr<UGsObjectState> CurrentState;

	UPROPERTY()
	TArray<UGsObjectState*> StateList;

public:
	virtual void InitState() {} // 상속 클래스에서 인스턴스 생성 MakeInstance

	virtual void RemoveAll()
	{
		CurrentState = nullptr;
		StateList.Empty();
	}
	
	void ChangeState(uint8 InState)
	{
		UGsObjectState* nextState = Find(InState);

		if (CurrentState.IsValid())
		{
			// 동일한 State를 중복처리하지 않음
			if (CurrentState->GetType() == InState)
			{
				return;
			}

			CurrentState->Exit();
		}

		CurrentState = nextState;

		if (CurrentState.IsValid())
		{
			CurrentState->Enter();
		}
	}

	UGsObjectState* Find(uint8 InType)
	{
		if (InType < StateList.Num())
		{
			return StateList[InType];
		}

		return nullptr;
	}

protected:
	UGsObjectState* GetCurrentState() const { return CurrentState.IsValid() ? CurrentState.Get() : nullptr; };

	template <typename T>
	T* GetCurrentState() const
	{
		return Cast<T>(GetCurrentState());
	}
};
