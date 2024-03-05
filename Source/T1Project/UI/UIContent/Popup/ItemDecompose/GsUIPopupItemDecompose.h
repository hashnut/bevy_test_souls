// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Message/GsMessageItem.h"
#include "GsUIPopupItemDecompose.generated.h"



class UTextBlock;
class UGsButton;
class UVerticalBox;
class UWidgetSwitcher;
class UGsSwitcherButton;
class UImage;
class UScrollBox;
class UPaperSprite;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupItemDecompose : public UGsUIPopup
{
	GENERATED_BODY()

private:
	enum QuickSelectGradeType : uint8
	{
		Normal =0 , 
		Magic ,
		Rare ,
		Max ,
	};
	
protected:	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _verticalBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirm;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGradeNormal;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGradeMagic;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGradeRare;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _gradeNoramlSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _gradeMagicSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _gradeRareSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _emptyListWarnRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _equipDecompSwitcherBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _skillbookDecompSwitcherBtn;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _slateDecompSwitcherBtn;

protected:
	UPROPERTY(EditdefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;
	UPROPERTY()
	class UGsDynamicPanelSlotHelper* _panelSlotHelper;

private:
	FGsToggleGroup _toggleGroup;

private:
	TArray<ItemDBIdAmountPair> _selectTargetItemAmountPairList;
	TArray<bool> _quickSelectGradeFlagList;

public:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

private:
	uint8 _currentTabIndex = 0;
	TMap<ItemType, TArray<ItemDBId>> _targetItemMap;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemDelegate;

public:
	void SetMultiDecomposeItemList(TMap<ItemType,TArray<ItemDBId>> InItemMap);

protected:
	virtual void OnInputCancel() override;


public:
	void OnAutoSelectEntryWidget(const uint64 InSelectItemDBID, const bool InFlag, const int32 InIndex);
	void OnUserSelectEntryWidget(const uint64 InSelectItemDBID, const bool InFlag, const int32 InIndex);
	void ClosePopup();
	void ResetData();

private:
	void InitMsg();
	void RemoveMsg();

private:
	void OnUpdateItemList(struct FGsItemMessageParamBase& InParam);
	void OnRemoveItemList(struct FGsItemMessageParamBase& InParam);

private:
	void OnConfirmCalculator(uint64 InItemDBID, uint32 InInputNum , int32 InWidgetIndex, bool InCalculatorState);
	void SetGradeSwitcher(const bool InFlag , const QuickSelectGradeType InType);
	void QuickSelectState(const QuickSelectGradeType InType , const bool InFlag);
	ItemGrade ConvertByQuickSelectType(const QuickSelectGradeType InType);
	void OpenTrayTickerLimitCount();
	void OnClickGrade(QuickSelectGradeType InGradeType);
	ItemType GetItemTypeByIndex(int32 InIndex);

protected:
	UFUNCTION()
	void ItemListRefreshEntry(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void OnClickNormalGrade();
	UFUNCTION()
	void OnClickMagicGrade();
	UFUNCTION()
	void OnClickRareGrade();
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnClickConfirm();
	UFUNCTION()
	void OnSlotClickedTab(int32 InIndex, bool bInIsSelected);
	UFUNCTION()
	void OnSelectChangedTab(int32 InIndex);
};
