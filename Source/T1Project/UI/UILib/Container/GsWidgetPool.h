// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsWidgetPool.generated.h"

// FWorldPSCPool쪽 풀 비우기 로직 참고
USTRUCT()
struct FGsWidgetPoolElement
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient)
	UUserWidget* _widget;

	float _lastUsedTime = 0.f;

public:
	FGsWidgetPoolElement()
		:_widget(nullptr), _lastUsedTime(0.f)
	{
	}

	FGsWidgetPoolElement(UUserWidget* InWidget, float InLastUsedTime)
		:_widget(InWidget), _lastUsedTime(InLastUsedTime)
	{
	}
};

/**
 * FUserWidgetPool을 참고하여 편의에 맞게 수정
 * AddToViewport, RemoveFromParent는 여기서 수행하지 않고 받은곳에서 각자 진행한다. 
 * 따라서 반납 전에 RemoveFromParent 호출에 주의.
 */
UCLASS()
class T1PROJECT_API UGsWidgetPool : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	TSubclassOf<UUserWidget> _widgetClass;

	UPROPERTY(Transient)
	TArray<UUserWidget*> _activeArray;	
	UPROPERTY(Transient)
	TArray<FGsWidgetPoolElement> _inactiveArray;

	TFunction<void(UUserWidget*)> _releaseFunc = nullptr;
	float _lastCleanTime = 0.f;

public:
	void Initialize(TSubclassOf<UUserWidget> InWidgetClass, int32 InWarmUpCount = 0, TFunction<void(UUserWidget*)> InReleaseFunc = nullptr);
	void Release(UUserWidget* InWidget);
	void ReleaseAll();
	void Empty();
	void WarmUp(int32 InWarmupCount);
	void SetReleaseFunc(TFunction<void(UUserWidget*)> InReleaseFunc);
	void KillUnusedWidgets(float InKillTime);
	int32 GetTotalRunningCount();
	float GetLastCleanTime() const { return _lastCleanTime; }	

	UUserWidget* Claim()
	{
		return CreateOrGetInstance();
	}

	template <typename UserWidgetT = UUserWidget>
	UserWidgetT* Claim()
	{
		if (UUserWidget* widgetInstance = CreateOrGetInstance())
		{
			return Cast<UserWidgetT>(widgetInstance);
		}

		return nullptr;
	}

	template <typename UserWidgetT = UUserWidget>
	UserWidgetT* Claim(TFunctionRef<void(UUserWidget*)> InClaimFunc)
	{
		if (UUserWidget* widgetInstance = CreateOrGetInstance())
		{
			InClaimFunc(widgetInstance);

			return Cast<UserWidgetT>(widgetInstance);
		}
		
		return nullptr;
	}

private:	
	UUserWidget* CreateOrGetInstance();
	UUserWidget* CreateWidgetInternal();

	float GetCurrTime() const;
};
