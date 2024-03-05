// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsUIWidgetBase.h"
#include "UI/UILib/Manager/GsUITickerHandler.h"
#include "GsUITray.generated.h"

/**
 * 스택관리 안하는 UI들의 기본 클래스
 */
UENUM()
enum class TrayCondition : uint8
{
	TC_AlwaysShow = 0,	// 일반 Tray
	TC_Active,			// Private Condition Tray가 Add 되면 Hide, Private Tray 제거후에 Show - 획득 티커
	TC_Priority,		// 앞으로 insert되어 우선적으로 사용되는 Tray - 필드보스 티커
	TC_Close,			// Private Condition Tray가 Add 되면 종료됨 - 일반 메세지 티커
	TC_Private,			// 생성시 다른 TrayInstance들을 Hide 또는 중지시키고 혼자만 뜨는 Tray - Dialog
};

UCLASS()
class T1PROJECT_API UGsUITray : public UGsUIWidgetBase
{
	GENERATED_BODY()

	friend class FGsUITickerHandler;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GsUI")
	bool _bCanMultipleInstance = false;

	UPROPERTY(EditAnywhere, Category = "GsUI")
	TrayCondition _eCondition = TrayCondition::TC_AlwaysShow;

	ESlateVisibility _restoreVisibility{ ESlateVisibility::SelfHitTestInvisible };

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "GsUI")
	bool _animationPause{ false };

public:
	virtual bool IsStackable() const final { return false; }
	virtual bool IsWindow() const final { return false; }
	virtual bool CanMultipleInstance() const override { return _bCanMultipleInstance; }
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_TRAY; }
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_TRAY; }

//Ticker 관리를 위한 추가
	virtual TrayCondition GetTrayCondition() const { return _eCondition; }

	virtual void SetHideVisibility();
	virtual void RestoreShowVisibility();

protected:
	virtual void PauseAllAnimations() {}
	virtual void RestartAllAnimations() {}
	virtual void ClearAllPauseAnimations() {}

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "User Interface|Animation")
	bool IsAnimationPause() { return _animationPause;  }
};
