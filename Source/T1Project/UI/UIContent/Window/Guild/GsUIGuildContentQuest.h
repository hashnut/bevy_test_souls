// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
#include "GsUIGuildContentQuest.generated.h"

class UScrollBox;
class UTextBlock;
class UPanelWidget;
class UWidgetSwitcher;
class UGsButton;
class UGsDynamicPanelSlotHelper;
class UGsUITooltipDesc;
class UGsUICurrencyRefreshCountButton;
class UGsUIGuildQuestPointEffect;
struct FGsSchemaGuildZealPointReward;
struct FGsGuildQuestData;
struct IGsMessageParam;

/**
 * 기사단 비동기 협동 퀘스트 페이지 
 */
UCLASS()
class T1PROJECT_API UGsUIGuildContentQuest : public UGsUIGuildContentBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _pointWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<class UUserWidget> _questWidgetClass;

	//UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	//UGsUIGuildQuestPointEffect* _uiPointEffect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltipZealPoint;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipZealPoint;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbWeeklyPoint;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootPointRemainTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbRewardRemainTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollPoint;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootPoint;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherQuest;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbLockDesc;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTooltipQuest;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltipQuest;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbQuestCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootQuestRemainTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _tbResetRemainTime;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollQuest;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootQuest;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelRootQuestEmpty;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyRefreshCountButton* _btnRefresh;

protected:
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperPoint;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperQuest;

protected:
	bool _bIsMyGuild = false;
	TArray<const FGsSchemaGuildZealPointReward*> _pointList;
	TArray<FGsGuildQuestData*> _questList;
	int64 _currZealPoint = 0;
	int32 _guildLevel = 0;
	int32 _maxZealPointLevel = 0;
	float _checkTimeSec = 0.f;
	time_t _endPointResetSec = 0;
	time_t _endQuestResetSec = 0;

public:
	virtual FString GetTabName() const override { return TEXT("Quest"); }

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// UGsUIGuildContentBase override
public:
	virtual void SendRequestPacket() override;
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false) override;
	virtual void Hide() override;
	
	virtual void OnResetTimeContents(ResetTimeTicket InType) override;

	// 길드 레벨변경 받아야함
	virtual void OnChangeGuildLevel() override;

protected:
	virtual void InvalidateAllInternal() override;

protected:
	void InvalidateGuildZealPoint();
	void InvalidateRemainTime();
	void InvalidateQuestList();
	void InvalidateRefreshInfo();
	void InvalidateCompleteCount();

	void SetUIRemainTime(const FTimespan& InTime, OUT UTextBlock* OutTextBlock);
	void SetUIQuestLock(bool bIsLocked);
	void ScrollToCurrZealPoint();

	float GetZealPointProgressRate(int32 InIndex);
	bool IsLockedByGuildJoinDay();	

protected:
	UFUNCTION()
	void OnRefreshEntryPoint(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnRefreshEntryQuest(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnPressTooltipZealPoint();
	UFUNCTION()
	void OnPressTooltipQuest();

	void OnClickRefresh();

public:
	void OnReceiveQuestInfo();
	void OnUpdateQuest(const IGsMessageParam* InParam);
	void OnReceiveQuestRefresh();

public:
	// 테스트함수
	UFUNCTION(BlueprintCallable)
	void TestSetZealPoint(int32 InPoint);
};
