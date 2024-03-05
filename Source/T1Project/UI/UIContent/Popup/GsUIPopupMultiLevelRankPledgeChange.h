// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"

#include "Shared/Client/SharedEnums/SharedStatEnum.h"

#include "UI/UIContent/Helper/GsEffectStringHelper.h"

#include "GsUIPopupMultiLevelRankPledgeChange.generated.h"


class UGsUIMultiLevelRankIcon;
class UTextBlock;
class UGsButton;
class UGsUICurrencyButton;
class UWidgetSwitcher;
class UScrollBox;
class UGsDynamicPanelSlotHelper;

struct FGsMultiLevelRankSlotData;
/**
 * 승급 시스템 서약 변경 팝업
 */
UCLASS()
class T1PROJECT_API UGsUIPopupMultiLevelRankPledgeChange : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSelectOrChange;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;

	// 서약 선택 (이전 서약 없을 경우)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMultiLevelRankIcon* _imageMainIconSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNameSelect;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnPledge;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancelPledge;

	// 서약 변경 (이전 서약 있을 경우)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMultiLevelRankIcon* _imageMainIconPrev;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNamePrev;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIMultiLevelRankIcon* _imageMainIconNext;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNameNext;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnChange;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCancelChange;

	// 상실 스탯 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelLostEmpty;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollLostStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfLostStat;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperLostStat;

	// 획득 스탯 목록
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelAcquiredEmpty;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollAcquiredStat;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfAcquiredStat;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperAcquiredStat;
	
protected:
	TArray<TPair<StatType, int32>> _refreshLostStatBuffer;
	TArray<TPair<StatType, int32>> _refreshAcquiredStatBuffer;

	int32 _selectedGroupId = INDEX_NONE;

public:
	void BeginDestroy() override;

protected:
	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

protected:
	UFUNCTION()
	void OnClickChange();
	UFUNCTION()
	void OnClickPledge();
	UFUNCTION()
	void OnClickCancel();

	UFUNCTION()
	void OnRefreshLostStat(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnRefreshAcquiredStat(int32 InIndex, UWidget* InEntry);

	void ComputeChangedStat(const FGsMultiLevelRankSlotData* InTargetSlot);
	void GetAddedStat(const FGsMultiLevelRankSlotData* InTargetSlot, FGsEffectStringHelper::FGsEffectAddData& OutStatList);
	const FGsMultiLevelRankSlotData* GetSelectedPledgeTopLevelRankSlot();

public:
	void SetNextPledge(int32 InGroupId);

	void InvalidateInfo();
	void InvalidateStat();

	// 2022/10/20 PKT - KeyBoard Mapping
protected:	
	virtual void OnInputCancel();
};
