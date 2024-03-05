// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "GsWidgetPoolManager.generated.h"

class UUserWidget;
class UGsWidgetPool;

/**
 * UserWidget 풀 관리. key는 해당 위젯의 pathName
 */
UCLASS()
class T1PROJECT_API UGsWidgetPoolManager :
	public UObject,
	public IGsManager
{
	GENERATED_BODY()

	// Pool 생성 제한개수
	const int32 LimitPoolCount = 1000;

	// TMap은 UPROPERTY 지원이 안되므로 관리를 위해 Pool을 TArray에 저장. PoolMap은 검색용으로 사용.
	UPROPERTY()
	TArray<UGsWidgetPool*> PoolList;

	UPROPERTY()
	TMap<FName, TWeakObjectPtr<UGsWidgetPool>> PoolMap; // key: PathName

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	// 모든 Pool을 제거
	void RemoveAll();

	// 사용 중이던 위젯을 모두 Pool에 반납
	void ReleaseAll();

	// 씬전환 등의 상황에서 ReleaseAll()에 의해 풀에 강제 수거될 때의 처리를 위해 InReleaseFunc를 받아서 저장한다.
	UGsWidgetPool* CreatePool(TSubclassOf<UUserWidget> InWidgetClass,
		TFunction<void(UUserWidget*)> InReleaseFunc = nullptr, int32 InWarmUpCount = 0);

	UGsWidgetPool* FindPool(TSubclassOf<UUserWidget> InWidgetClass);

	// Pool을 Find하여 반납처리한다.
	void ReleaseWidget(TSubclassOf<UUserWidget> InWidgetClass, UUserWidget* Widget);

private:
	float GetCurrTime() const;

public:
	void PrintLog();
};