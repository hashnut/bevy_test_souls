// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "UI/UILib/Base/GsUIPopup.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"

#include "GameObject/Define/GsGameObjectDefine.h"

#include "GsUIPopupNpcShopInven.generated.h"

struct FGsNpcShopQuickSlotItemInfo;

class UImage;
class UTextBlock;
class UCanvasPanel;
class FGsItem;
class UGsButton;
class UGsUIIconItem;
class UGsUIIconItemInventory;
class FGsNpcShopInvenUIMsgReceiver;
class UScrollBox;

/**
 * NpcShop Inventory
 */
UCLASS()
class T1PROJECT_API UGsUIPopupNpcShopInven : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;
	UPROPERTY()
	class UGsDynamicIconSlotHelper* _iconSlotHelper;

public:
	// RedDot(임시)
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
	UGsButton* _btn_close;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnAutoSell;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _slotCountText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imageDevideLineUnder;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _sizeBoxUnder;

private:
	FGsNpcShopInvenUIMsgReceiver* _uiMsgReceiver;
	// 출력할 아이템들 DBID 리스트
	TArray<uint64> _displayItemList;
	TWeakObjectPtr<UGsUIIconItemInventory> _curSelectIcon;
	TMap<EInventoryTab, UImage*> _redDotImageList;
	EInventoryTab _currentInvenTabIndex = EInventoryTab::AllTab;
	uint16 _invenTotalSlotCount = 0;
	bool _isBuyMode = true;
	NpcFunctionType _shopNpcFunctionType = NpcFunctionType::SHOP;

private:
	int32 _createMaxSlotCountByTick = 0;   // 총 생성해야할 인벤토리 슬롯 갯수
	int32 _createSlotCountEveryTick = 0;   // 매 틱마다 생성할 인벤토리 슬롯 갯수(Default)
	float _frameRate = 0.f;
	uint8 _maxCount = 0; // 틱당 생성하는 itemicon 최대갯수
	uint8 _minCount = 0;  // 틱당 생성하는 itemicon 최소 갯수
	uint8 _balanceCount = 0; // 추가되거나 차감될때 기준이되는 카운트

	const FName POPUP_CALCULATOR = TEXT("PopupCalculator");


private:
	void InvalidateInvenItemList();
	void CreateIconSlot(float InDeltaTime);
	void ConvertItemTypeByInvenTabType(OUT TArray<ItemType>& outTypeList);
	void OpenCalculatorUI(const FGsItem* InTargetItem);
	void CloseCalculatorUI();
	bool IsOpenedCalculatorUI();
	void InitCalculatorUI(const FGsItem* InTargetItem);
	void CloseItemDetailUI();
	void ConfirmItemStackCount(int64 InCount);
	bool IsMatchedCurTab(const ItemType InType);
	bool IsExistItemInShoppingBasket(const uint64 InItemDBID);
	void ResetSelection();

	void OnFinishCalculate(int64 InCount);
	void OnCancelCalculate();

public:
	void SetInvenTabRedDot(EInventoryTab InTab, bool InActiveState);
	void InvalidateInvenTabSlotCount();
	void CheckRedDot();
	void OnClickIconSlot(class UGsUIIconItem& InIcon);
	void ChangeShopMode(const bool InIsBuyMode);
	void ChangeShopType(const NpcFunctionType InNpcFunctionType);

	void InvalidateInventorySimple();	// 탭 정보 갱신없이 인벤토리 UI 갱신

public:
	void AddItem(const uint64 InItemDBID);
	void RemoveItem(const uint64 InItemDBID);
	void UpdateItem(const uint64 InItemDBID);

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void InvalidateAllInternal() override;
	
private:
	void OnLongPressIcon(UGsUIIconItem& InIcon);

public:
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnSlotClickedTab(int32 InIndex, bool bInIsSelected);
	UFUNCTION()
	void OnSelectChangedTab(int32 InIndex);
	UFUNCTION()
	void OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateIconWrapBox(class UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnClickAutoSell();
};
