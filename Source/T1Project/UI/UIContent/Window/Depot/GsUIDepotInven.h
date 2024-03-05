// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageItem.h"
#include "Message/MessageParam/GsItemMessageParam.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsUIDepotInven.generated.h"


class UTextBlock;
class FGsItem;
class UImage;
class UGsButton;
class UWidgetSwitcher;
class UGsUIIconItem;
class UGsUIIconItemInventory;
class UScrollBox;
/**
 *  창고 인벤토리
 */
UCLASS()
class T1PROJECT_API UGsUIDepotInven : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;
	UPROPERTY()
	class UGsDynamicIconSlotHelper* _iconSlotHelper;

public:
	// RedDot
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _allTabRedDot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _equipmentTabRedDot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _consumableTabRedDot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _etcItemTypeRedDot;

public:
	//Toggle
	UPROPERTY(BlueprintReadOnly, Category = "GsUI_TabToggle", meta = (BindWidget))
	UPanelWidget* _panelInvenTab;

	FGsToggleGroup _toggleGroup;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoDeposit;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _slotCountText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelEmptyItemList;

private:
	TArray<TPair<MessageContentItem, FDelegateHandle>>	_itemContentsDelegateList;
	TArray<TPair<MessageItem, FDelegateHandle>>	_listItemDelegate;

private:
	// 출력할 아이템들 DBID 리스트
	TArray<uint64> _displayItemList;
	TWeakObjectPtr<UGsUIIconItemInventory> _curSelectIcon;

private:
	int32 _invenTotalSlotCount = 0;
	TMap<EInventoryTab, UImage*> _redDotImageList;


private:
	int32 _createMaxSlotCountByTick = 0;   // 총 생성해야할 인벤토리 슬롯 갯수
	int32 _createSlotCountEveryTick = 0;   // 매 틱마다 생성할 인벤토리 슬롯 갯수(Default)
	float _frameRate = 0.f;
	uint8 _maxCount = 0; // 틱당 생성하는 itemicon 최대갯수
	uint8 _minCount = 0;  // 틱당 생성하는 itemicon 최소 갯수
	uint8 _balanceCount = 0; // 추가되거나 차감될때 기준이되는 카운트

private:
	void InvalidateInventory(const IGsMessageParam* InParam);
	void InvalidateIcon(ItemDBId InItemDBId);
	void InvalidateSlotCount();
	void InvalidateEmptyItemList();

public:
	void OnClickIconSlot(class UGsUIIconItem& InIcon);
	void OnChangeStateDeposit(const IGsMessageParam* InParam);
	void OnChangeStateTake(const IGsMessageParam* InParam);
	void AddBasketItems(const IGsMessageParam* InParam);
	void AddBasketItemList(const IGsMessageParam* InParam);
	void RemoveBasketItems(const IGsMessageParam* InParam);

public:
	void RspInvenItemList(const IGsMessageParam* InParam);

private:
	void RegisterUIMsg();
	void RemoveUIMsg();
	void CloseItemDetailUI();
	void OpenCalculatorUI(const FGsItem* InTargetItem);
	void InitCalculatorUI(const FGsItem* InTargetItem);
	void CloseCalculatorUI();
	bool IsOpenedCalculatorUI();
	void ConfirmItemStackCount(int64 InCount);
	void CancelCalculatorPopup();
	void ResetSelection();


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

private:
	void OnLongPressIcon(UGsUIIconItem& InIcon);

private:
	void AddItemList(struct FGsItemMessageParamBase& InParam);
	void UpdateItemList(struct FGsItemMessageParamBase& InParam);
	void RemoveItemList(struct FGsItemMessageParamBase& InParam);

	bool IsMatchedCurTab(const ItemType InType);
	EInventoryTab ConvertInvenTabType(const int32 InTabIndex);

public:
	UFUNCTION()
	void OnClickAutoDeposit();
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnSlotClickedTab(int32 InIndex, bool bInIsSelected);
	UFUNCTION()
	void OnSelectChangedTab(int32 InIndex);
	UFUNCTION()
	void OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon);
};
