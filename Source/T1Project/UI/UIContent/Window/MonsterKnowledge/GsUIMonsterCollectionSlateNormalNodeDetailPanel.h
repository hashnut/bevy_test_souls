// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "GsUIMonsterCollectionSlateNodeDetailPanelBase.h"
#include "GsUIMonsterCollectionSlateNormalNodeDetailPanel.generated.h"

class UGsButton;
class UTextBlock;
class UScrollBox;
class UPanelWidget;
class UWidgetSwitcher;
class UGsSwitcherButton;
class UGsUICurrencyButton;
class UGsDynamicPanelSlotHelper;

UCLASS()
class T1PROJECT_API UGsUIMonsterCollectionSlateNormalNodeDetailPanel : public UGsUIMonsterCollectionSlateNodeDetailPanelBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textSlateTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textCompletedCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textAllCount;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _statSetRootPanel;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _statSetSlotHelper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _statWidgetClass;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switchApplyButton;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnOpenSlate;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUICurrencyButton* _btnCurrencyActiveNode;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnCheckNodeAllSelected;


protected:	
	MonsterKnowledgeSlateSlotId _focusNodeId = INVALID_MONSTER_KNOWLEDGE_SLATE_SLOT_ID;

	TMap<StatType, TPair<StatValue, StatValue >> _statRateDataMap;
	TArray<StatType> _statSortOrderData;

public:
	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnRefreshStatSet(int32 InIndex, UWidget* InEntry);

	UFUNCTION()
	void OnClickedOpenSlate();

	UFUNCTION()
	void OnClickedActiveNode();

	UFUNCTION()
	void OnChangedCheckBox();

public:
	virtual void SetData(const class FGsMonsterKnowledgeSlate* InData, TFunction<void(EMonsterCollectionSlateCallbackReason, uint64)> InCallback) override;
	virtual void Invalidate() override;
	virtual void InvalidateCurrency() override;

	void SetDetailData(const class FGsMonsterKnowledgeSlateNode* InFocusNode);

	bool IsCheckedAllSelected() const;

	MonsterKnowledgeSlateSlotId GetDetailDataId() const { return _focusNodeId; }
};
