// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UMG/Public/Animation/WidgetAnimation.h"
#include "UI/UILib/Base/GsUITicker.h"
#include "GsUITrayTickerBoss.generated.h"

/**
 * 시스템 메시지 출력
 * - Depth 높음, 로딩중 사라지지 않음
 */
class UWidgetSwitcher;
class UTextBlock;
class UImage;
class UWidgetAnimation;

UENUM()
enum class EGsBossTickerStateType
{
	Spawn,
	Die,
	Max
};

UCLASS()
class T1PROJECT_API UGsUITrayTickerBoss : public UGsUITicker
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		UWidgetSwitcher* _bossWidgetSwitcher;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
		UTextBlock* _bossNameText;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
		UImage* _bossImage;
	UPROPERTY(EditDefaultsOnly, Transient, meta = (BindWidgetAnim))
		UWidgetAnimation* _bossTickerAnim;

private:
	FWidgetAnimationDynamicEvent _finishDelegate;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	// System 메시지는 위에 출력될 수 있도록 기본 Depth를 높게 설정
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_SYSTEM_TRAY; }

	// 로딩 중 사라지지 않음
	virtual bool IsNotDestroyedDuringLoading() const { return true; }

	// UI_HIDE_SYSTEM_TRAY Flag에 의해 Hide
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_SYSTEM_TRAY; }

public:
	void SetData(EGsBossTickerStateType inType, int32 creatureId = 0);
	void InvalidBossImage(int32 inCreatrueId);	
	UFUNCTION()
	void OnAnimationFinish();
};
