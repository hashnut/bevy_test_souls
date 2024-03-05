// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWindow.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageContents.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIWindowAccumulateTax.generated.h"

class UTextBlock;
class UProgressBar;
class USlider;
class UScrollBox;
class UPanelWidget;
class UGsButton;
class UGsUITooltipDesc;
struct IGsMessageParam;
class UGsDynamicPanelSlotHelper;
struct FGsSchemaWorldData;

/**
 * 누적 세금 창(정식 공성전 전까지 누적 세금 보여주는 창. 공성전 생기면 없어질 예정)
 */
UCLASS()
class T1PROJECT_API UGsUIWindowAccumulateTax : public UGsUIWindow
{
	GENERATED_BODY()

public:
	using MsgHandleMarket = TPair<MessageContentMarket, FDelegateHandle>;
	using MsgHandleUnlock = TPair<MessageContentUnlock, FDelegateHandle>;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnBack;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scroll;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelScrollRoot;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltip;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _uiTooltip;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UProgressBar* _progressBar;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	USlider* _sliderBar;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockGold;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockDia;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockRemainTime;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelper;

protected:
	const float CHECK_TIME_DURATION_SEC = 5.f;
	MsgSystemHandle _msgHandleSystem;
	TArray<MsgHandleMarket> _msgHandleMarketList;
	TArray<MsgHandleUnlock> _msgHandleUnlockList;
	FTimerHandle _timerHandle;

	time_t _totalTime = 0;
	time_t _accumEndTime = 0;

	TArray<const FGsSchemaWorldData*> _serverList;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual bool SyncContentState() override;

protected:
	void BindMessages();
	void UnbindMessages();

protected:
	// MSystem
	void OnResetTimeContents(const IGsMessageParam* InParam);

	// MMarket
	void OnRcvAccumTaxList(const IGsMessageParam*);

	// MUnlock
	void OnContentsUnlock(const IGsMessageParam* InParam);

protected:
	void InvalidateUI();
	void InvalidateTime();
	void InvalidateServerList();

	const struct FGsSchemaWorldGroupSiegeWarfare* GetMyWorldGroup() const;

	UFUNCTION(BlueprintCallable)
	void SetProgress(float InValue); // 0~1사이값
	void SetTextAccumAmount(Currency InAmount, OUT UTextBlock* OutTextBlock);

protected:
	void OnTimerUpdateTime();

	UFUNCTION()
	void OnPressTooltip();

	UFUNCTION()
	void OnRefreshEntry(int32 InIndex, UWidget* InEntry);
};
