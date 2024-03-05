// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UI/UILib/Base/GsUITray.h"
#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
#include "GsUITraySaveBattery.generated.h"


class UGsUIHUDPlayerInfo;
class UGsUIHUDAbnormality;
class UGsUIHUDSystemInfo;
class UGsUIHUDConsumable;
class UGsUIHUDSpiritShot;
class UGsUISaveBatteryMode;
class UGsUIInventorySaveBattery;
class UTextBlock;
class UGsButton;
class UGsSwitcherButton;
class FDelegateHandle;
class UWidgetSwitcher;
class UGsHorizontalBoxIconSelector;
class UGsUIIconItemSimple;
class UGsUIIconItem;
class FGsSaveBatteryHandler;
class UGsUIHUDCounterAttackButton;
class UGsDynamicPanelSlotHelper;
class UScrollBox;
/**
 * 절전모드 UI
 */
UCLASS()
class T1PROJECT_API UGsUITraySaveBattery : public UGsUITray, public IGsInvalidateUIInterface
{
	GENERATED_BODY()

protected:
	enum AnimationType : uint8
	{
		Battle = 0,
		Idle,
		Dead,

		MAX
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDPlayerInfo* _playerInfoUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDAbnormality* _abnormalityUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRemainTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMap;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGold;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockExp;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnOffLinePlay;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDSystemInfo* _systemInfoUI;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UWidgetSwitcher* _switcherMode;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUISaveBatteryMode* _modeTextUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDConsumable* _consumableUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDSpiritShot* _spiritShotUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelectorReturn;

	UPROPERTY()
	UGsUIIconItemSimple* _iconReturn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIInventorySaveBattery* _invenUI;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherWeightPenalty;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _weightLowText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _weightMidText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _weightHighText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockSlotCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDCounterAttackButton* _counterAttackButton;

	// 경계 대상 정보
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDetectionTarget;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDetectionTargetImage;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelDetectionTarget;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperDetectionTargetData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfDetectionTargetDataEntry;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBoxDetectionTargetSet;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherDetectionTarget;

protected:
	FGsSaveBatteryHandler* _saveBatteryHandler = nullptr;

	/************************************************************************/
	/* Close                                                                */
	/************************************************************************/
	FVector2D _startPoint = FVector2D::ZeroVector;
	FVector2D _endPoint = FVector2D::ZeroVector;

	float _closeDistance = 50.0f;

	/************************************************************************/
	/* Time                                                                 */
	/************************************************************************/
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _updateSec = 1.0f;

	FTimerHandle _timerHandler;
	FDateTime* _startTime = nullptr;

	FTimerHandle _remainTimerHandler;
	FText _remainTimeFormatText;

	/************************************************************************/
	/* Etc..                                                                */
	/************************************************************************/
protected:
	AnimationType _animationState = AnimationType::Idle;
	ItemId _returnScrollId = 0;

	/************************************************************************/
	/* overrides                                                            */
	/************************************************************************/
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

protected:
	virtual bool IsEnableManagerTick() const override { return true; }
	virtual void OnManagerTick(float InDeltaTime) override;

public:
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_TRAY; }

	virtual void InvalidateAllInternal() override;

	/************************************************************************/
	/* 기타                                                                 */
	/************************************************************************/
public:
	void SetHandler(FGsSaveBatteryHandler* InHandler) { _saveBatteryHandler = InHandler; }

	/************************************************************************/
	/* 초기화                                                                */
	/************************************************************************/
protected:
	void InitializeReturnScroll();

	/************************************************************************/
	/* 갱신                                                                 */
	/************************************************************************/
public:
	void InvalidateMapName();
	void InvalidateReturnScroll();
	void InvalidateItemWeightRate();
	void InvalidateInventoryCount();
	void InvalidateOfflinePlay();
	void InvalidateDetectionTarget();

	void InvalidateMode();
	void OnLocalPlayerDie();
	
	void OnIncreasePlayerExp(Exp InExp);
	void OnIncreaseGold(Currency InAmount);
	void OnUpdateItemWeight(float InWeightPercent);
	void OnUpdateWeightPenaltyType(WeightPenaltyGrade InPenaltyGradeType);

	/************************************************************************/
	/* Events                                                         */
	/************************************************************************/
public:
	void OnClickReturn(UGsUIIconItem& InIcon);
	UFUNCTION()
	void OnClickOfflinePlay();

	UFUNCTION()
	void OnClickDetectionTarget();
	UFUNCTION()
	void OnRefreshEntryDetectionTarget(int32 InIndex, UWidget* InEntry);

	/************************************************************************/
	/* Time                                                                 */
	/************************************************************************/
protected:
	void UpdateTime();
	void UpdateRemainTime();

public:
	void PlayRemainTime();
	void PauseRemainTime();
	void StopRemainTime();

	/************************************************************************/
	/* 세부 UI 접근                                                          */
	/************************************************************************/
public:
	UGsUIHUDPlayerInfo* GetPlayerInfoUI() const { return _playerInfoUI; }
	UGsUIHUDConsumable* GetConsumableUI() const { return _consumableUI; }
	UGsUIHUDSpiritShot* GetSpiritShotUI() const { return _spiritShotUI; }
	UGsUIHUDSystemInfo* GetSystemInfoUI() const { return _systemInfoUI; }
	UGsUIInventorySaveBattery* GetInventoryUI() const { return _invenUI; }
	UGsUIHUDCounterAttackButton* GetCounterAttackButtonUI() const { return _counterAttackButton; }

public:
	virtual TrayCondition GetTrayCondition() const { return _eCondition; }
};