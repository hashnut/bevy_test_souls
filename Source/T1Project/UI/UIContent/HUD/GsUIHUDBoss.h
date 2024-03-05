// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "UTIL/GsTimeSyncUtil.h"
#include "Message/GsMessageUserInfo.h"
#include "Message/GsMessageBoss.h"
#include "Message/GsMessageGameObject.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UTIL/GsTimeUtil.h"
#include "Map/Minimap/GsMinimapDefine.h"
#include "GsUIHUDBoss.generated.h"

/**
 * HUD 로컬플레이어 카오틱 상태 표시
 */

UENUM(meta = (DisableNativeTick))
enum class EGsBossIconStateType
{
	Spawn,
	Deactive,
	Ready,
};

UCLASS()
class T1PROJECT_API UGsUIHUDBoss : public UGsUIHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UWidgetSwitcher* _bossWidgetSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _remainTimeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UGsButton* _button;

private:
	FText _spawnTimeTextFromat;	
	WorldBossId _bossId;
	FDateTime _endTime;
	FTimespan _timeSpan;
	MsgBossHandleArray _msgBossHandleList;	
	EGsBossIconStateType _bossState;
	float _time = 0; 
	FString _remainTime;
	int32 _readyTime;	
	FGsDateTimer _dateTimer;
	
	EGsMinimapSideButtonMode _currentMinimapSideMode = EGsMinimapSideButtonMode::Field;

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:		
	UFUNCTION()
	void OnClickButton();
	//필드에서 보스 룸 스폰 레디
	void OnWorldBossRoomReady(const struct IGsMessageParam* InParam);

	//던전 내 패킷
public:
	//던전 안에서 보스 룸 레디
	void OnWorldBossSpawnReadyState(const struct IGsMessageParam* InParam);
	//던전 안에서 보스 스폰
	void OnWorldBossSpawn(const struct IGsMessageParam* InParam);	

private:	
	void InvalidTime();
	void InvalidHudState();
	void InvalidRemainTime();
	void InvalidVisibility();

public:
	void SetMinimapSideButtonMode(EGsMinimapSideButtonMode In_mode);
};
