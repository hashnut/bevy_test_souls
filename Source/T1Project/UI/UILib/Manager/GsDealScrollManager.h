// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"
#include "Containers/Queue.h"
#include "GsDealScrollManager.generated.h"

class UGsUIDealScrollBase;

/**
 * 딜스크롤 출력 관리
 */
UCLASS()
class T1PROJECT_API UGsDealScrollManager :
	public UObject, 
	public IGsManager
{
	GENERATED_BODY()
	
private:
	TWeakObjectPtr<class UGsWidgetPoolManager> _poolManager;

	// 풀에서 해제되었을 때 불릴 함수
	TFunction<void(class UUserWidget*)> _onReleaseCompleteFunc = nullptr;

	// 속도 조절용으로 사용
	TMap<int64, TWeakObjectPtr<UGsUIDealScrollBase>> _prevWidgetMap; // key: GameId

	int64 _speedCheckTick = 0;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	void Reset();
	UGsUIDealScrollBase* AddWidget(const FName& InKey, class USceneComponent* InTarget,
		const FString& InValue, FVector InDir = FVector::ZeroVector, int64 InGameId = 0);

	void ReleaseWidget(UGsUIDealScrollBase* InWidget);

private:
	const struct FGsSchemaDealScrollData* GetTableRow(const FName& InKey) const;

	UGsUIDealScrollBase* Claim(UClass* InClass);
};
