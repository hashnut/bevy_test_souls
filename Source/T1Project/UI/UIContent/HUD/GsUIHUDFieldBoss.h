// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageBoss.h"
#include "Message/GsMessageGameObject.h"
#include "GsUIHUDFieldBoss.generated.h"


/**
 * HUD 로컬플레이어 카오틱 상태 표시
 */
class UGsButton;
class UWidgetSwitcher;
class UTextBlock;

UENUM()
enum class EGsFieldBossIconStateType
{
	Spawn,
	Deactive,
	Ready,
};

UCLASS()
class T1PROJECT_API UGsUIHUDFieldBoss : public UGsUIHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UWidgetSwitcher* _fieldBossWidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _remainTimeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _button;

private:
	FText _spawnTimeTextFromat;	
	FDateTime _fieldBossSpawnTime;
	FTimespan _timeSpan;
	MsgBossHandleArray _msgBossHandleList;	
	EGsFieldBossIconStateType _bossState;
	float _time = 0; 
	FString _remainTime;
	int32 _readyTime;
	int32 _levelId;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& inMyGeometry, float inDeltaTime) override;

public:		
	UFUNCTION()
	void OnClickButton();
	void OnFieldBossSpawn(const struct IGsMessageParam* InParam);
	void OnFieldBossDie(const struct IGsMessageParam* InParam);
	void OnFieldInfo(const struct IGsMessageParam* InParam);	

public:
	void UpdateState(float inDelta);	
	virtual void InvalidateAllInternal() override;

private:	
	void InvalidHudState();
	void InvalidRemainTime();	
	void InvalidVisibility();
};
