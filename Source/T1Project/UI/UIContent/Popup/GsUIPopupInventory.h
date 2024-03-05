// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIPopup.h"
#include "Tutorial/GsTutorialInteractionInterface.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsUIPopupInventory.generated.h"

class FGsItem;
class UGsInventoryManager;
struct IGsMessageParam;

class UImage;
class UTextBlock;
class UInvalidationBox;
class UCanvasPanel;
class UGsButton;
class UScrollBox;
class UWidgetSwitcher;
class UGsUIIconItemInventory;
class UGsUIItemSlotLockWidget;

/**
 * Inventory Main UI
 */
UCLASS()
class T1PROJECT_API UGsUIPopupInventory : public UGsUIPopup, public IGsTutorialInteractionInterface
{
	GENERATED_BODY()

public:
	enum ESortUIList : uint8
	{
		DefaultType = 0,
		GradeType,
		WeightType,
		ServerSlotType,
		None,
		Max,
	};

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;
	UPROPERTY()
	class UGsDynamicIconSlotHelper* _scrollHelper;

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
	TMap<EInventoryTab, TWeakObjectPtr<UImage>> _redDotImageList;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btn_close;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _slotCountText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSortList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnItemSlotLock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDecompose;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _sortItemList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBox;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnDefaultSort;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnGradeSort;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnWeightSort;

public:
	// Weight Panalty
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _weightSwitcher;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _normalWeightText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _panaltyOneWeightText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _panaltyTwoWeightText;

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UCanvasPanel* _emptyItemListWarnRoot;


private:
	// 출력할 아이템들 DBID 리스트 : 상단 탭을 변경하면 같이 변경된다.
	TArray<ItemDBId> _diplayItemList;

private:
	TWeakObjectPtr<UGsInventoryManager> _invenMgrPtr;

private:
	bool _isDimmedState = false;
	ItemDBId _selectionItemDbid = 0;
	uint8 _invenTabIndex = 0; 
	uint16 _invenTotalSlotCount = 0;
	int32 _createSlotCountEveryTick = 0;
	int32 _createMaxSlotCountByTick = 0;

	// ScrollToTarget을 수행할 대상 아이템(인벤토리 복구 시 사용)
	ItemDBId _tempScrollTargetItem = INVALID_ITEM_DB_ID;

private:
	TArray<TPair<MessageContentInven, FDelegateHandle>>	_listInvenDelegate;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	void BindMessage();
	void UnBindMessage();

protected:
	virtual void OnInputCancel() override;

private:
	void UpdateSlotState(UGsUIIconItemInventory* InItemIcon);
	EInventoryTab ConvertTabTypeByIndex(int32 InIndex);
	void InvalidateSlotCount();
	void InvalidateExpandButton();
	void RefreshRealIconSlot();
	void CreateIconSlot(float InDeltaTime);
	void CheckOwnedItemWarnText();

public:
	void CreateItemList(TArray<ItemDBId> InItemList, bool InIconDimmedState);
	void OnChangeTab(EInventoryTab InTabType, bool InIsNeedInvalidate);
	void OnChangeSlotIndex();
	void AddItemList(TArray<TWeakPtr<FGsItem>> InAddItemList);
	void UpdateItemList(const TArray<ItemDBId>& InItemDBIdList);
	void RemoveItemList(const TArray<ItemDBId>& InItemDBIdList);
	void CheckRedDot();
	void SortServerIndex();
	void InvalidateItemList();

public:
	void SetItemDimmed(const bool InIsLock, TArray<ItemCategorySub>& InDimmedTypeList);
	void ResetIconSelection();
	void OnChangeInvenSlotCount(uint32 InMaxSlotCount, uint32 InTotalExtendCount);

	// 특정 아이템 위치로 스크롤. 인벤토리 복구에서 사용함
	void ScrollToTargetItem(ItemDBId InTarget);

public:
	void SetInvenTabRedDot(EInventoryTab InTab, bool InActiveState);

public:
	void OnClickIconSlot(class UGsUIIconItem& InIcon);

	// 정렬 기능 함수
private:
	void CloseSortItemListActive(); // 정렬 기능 툴팁 닫기 기능
	void SelectSortItemList(ESortUIList InListType, bool InNeedRefreshAll = false);
	bool IsMatchedCurTab(const EInventoryTab InCurTabType, const ItemType InItemType);

public:
	// 무게 페널티는 서버 패킷에 의해 변경된다.
	void SetWeightPenaltyType(WeightPenaltyGrade InPenaltyGradeType);
	// 무게는 보유한 아이템들을 토대로 계산된다.
	void SetWeightPercentInfo(int32 InWeight, float InWeightPercent);

public:
	ItemDBId GetSelectionItemDbid() const { return _selectionItemDbid; }


private:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);


public:
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void OnClickSortList();
	UFUNCTION()
	void OnSlotClickedTab(int32 InIndex, bool bInIsSelected);
	UFUNCTION()
	void OnSelectChangedTab(int32 InIndex);
	UFUNCTION()
	void OnRefreshIconWrapBox(int32 InIndex, class UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnClickDefaultSortList();
	UFUNCTION()
	void OnClickGradeSortList();
	UFUNCTION()
	void OnClickWeightSortList();
	UFUNCTION()
	void OnClickSlotLock();
	UFUNCTION()
	void OnClickDecompose();
//----------------------------------------------------------------------------------------------------------------------
	// 튜토리얼에서 인벤토리에 특정 아이템 포커싱 할 때 사용하는 기능(FGsTutorialActionInventorySelectItem)
public:
	DECLARE_DELEGATE_OneParam(FOnTutorialInventoryListEvent, UWidget* /*TargetWidget*/);
	FOnTutorialInventoryListEvent OnTutorialInventoryListEvent;

private:
	uint64 _tutorialItemDBId = 0;

private:
	bool IsTutorial() const { return (0 != _tutorialItemDBId) ? true : false; }
	
	UFUNCTION()
	void OnScrollList(float InCurrentOffset);

public:
	void SetTutorialItemDBId(uint64 InItemDBId);

	// IGsTutorialInteractionInterface
private:
	bool _bIsTutorialInteraction = false;
	FGsOnTutorialInteraction OnTutorialInteraction;

public:
	virtual void SetIsTutorialInteraction(bool bIsTutorial) { _bIsTutorialInteraction = bIsTutorial; }
	virtual bool IsTutorialInteraction() const { return _bIsTutorialInteraction; }
	virtual FGsOnTutorialInteraction& GetTutorialInteraction() { return OnTutorialInteraction; }

	// 인벤토리 확장 시, 제일 아래로 스크롤되도록 만드는 함수
private:
	void OnInvenSlotExtended(const IGsMessageParam* InParam);
};
