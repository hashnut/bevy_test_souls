#pragma once

#include "CoreMinimal.h"
#include "Guild/GsGuildItemCollectionData.h"
#include "Item/Collection/GsItemCollectionFilterBase.h"
#include "Item/Collection/EItemCollectionEnumData.h"
#include "UI/UIContent/Window/Guild/GsUIGuildContentBase.h"
#include "UI/UIContent/Common/Inventory/GsInventoryLogicItemCollection.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "Runtime/SlateCore/Public/Types/SlateEnums.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "GsUIGuildContentItemCollection.generated.h"


class UListView;
class UScrollBox;
class UGsButton;
class UEditableTextBox;
class UEditableTextBox;
class UTextBlock;
class UWidgetSwitcher;
class UGsDynamicPanelSlotHelper;
class UGsUIInventoryCommon;
class UScrollBox;


UCLASS()
class T1PROJECT_API UGsUIGuildContentItemCollection : public UGsUIGuildContentBase
{
	GENERATED_BODY()

	const static int32 INVALID_TAB_INDEX = -1;

	/************************************************************************/
	/* Blue Print Bind                                                      */
	/************************************************************************/
protected:
	
	/**
	 * sub Tab 
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UScrollBox* _scrollBtnTabSubList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _subClassOfSubTabBtnEntry;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _slotHelperBtnTabSub;

	/**
	 * Item Collections
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget, EntryClass = UGsUIItemCollectionBase))
	UListView* _listViewCollectionItems;


	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherItemCollectionList;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textItemCollectionList;

	/**
	 * Collection Status Button
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCollectionStatus;
	
	/**
	 * Text Search Edit Box
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UEditableTextBox* _textSearch;

	/**
	 * Reset Button
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSearchReset;

	/**
	 * Register Use Count Text Block
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textUseCount;

	/**
	 * Inventory
	 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIInventoryCommon* _inventoryUI;

	// 2022/01/05 PKT - collection list Data Entry Buffer
	UPROPERTY()
	class UGsPoolUObject* _poolCollectionData;

	// 2021/09/28 PKT - Inventory Data
	TUniquePtr<FGsInventoryLogicItemCollection> _inventoryLogic = nullptr;


	/************************************************************************/
	/* Data 멤버                                                            */
	/************************************************************************/
	// 2021/09/29 PKT - SubTab Category 순서
	enum class ESubTabSequence
	{
		All				// 2021/09/29 PKT - "전체"
		, Completed		// 2021/09/29 PKT - "완료"
		, InCompleted	// 2021/09/29 PKT - "미완료"
		, BookMark		// 2021/09/29 PKT - "즐겨찾기"

		, Max
	};

	// 2021/10/27 PKT - 포커스 Data
	struct FGsFocusData
	{
		CollectionId collectionId = INVALID_COLLECTION_ID;
		CollectionConditionId collectionConditionId = INVALID_COLLECTION_CONDITION_ID;
		TArray<ItemDBId> itemDBIdQueue;

		void Clear()
		{
			collectionId = INVALID_COLLECTION_ID;
			collectionConditionId = INVALID_COLLECTION_CONDITION_ID;
			itemDBIdQueue.Empty();
		}
	};

	// 2021/09/29 PKT - Sub tab Toggle
	FGsToggleGroup _toggleGroupSubTabs;
	
	// 2021/09/29 PKT - trim Search Text
	FString _trimSearchText;

	// 2021/08/03 PKT - detail popup data ( Selected Item Collection innser Slot )
	FGsFocusData _focusData;

	// 2021/10/27 PKT - 뒤로가기 시 필요한 백업 Data
	int32 _lastSubTabSelectedIndex = INVALID_TAB_INDEX;

	// 2021/10/27 PKT - 뒤로가기 시 필요한 백업 Data
	float _lastItemCollectionScrollOffset = 0.f;

private:

	TArray<TPair<MessageContentItem, FDelegateHandle>>	_itemContentsActionTypeDelegates;

	TPair<MessageContentGuild, FDelegateHandle> _activePopupRewardData;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	
private:
	void RegisterMessages();
	void UnregisterMessages();

public:	
	virtual void Show(bool bIsMyGuild, bool IsStackRecoverFlag = false);
	virtual void Hide();

	virtual void OnResetTimeContents(ResetTimeTicket InType) override;

	/************************************************************************/
	/* Invalidate or Refresh												*/
	/************************************************************************/
private:
	// 2021/09/29 PKT - sub tab refresh
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnRefreshSubTabEntry(int32 InIndex, UWidget* InEntry);

	// 2021/10/05 PKT - Item Collection 전체 갱신
	void InvalidateItemCollectionAll();
	
	// 2021/10/05 PKT - Item Collection 부분 갱신
	void InvalidateItemCollection(const TArray<CollectionId>& InUpdatedCollectionIds);

	// 2021/09/29 PKT - 등록 가능한 갯수
	void InvalidateUsedCount();

	// 2021/10/08 PKT - Inventory 갱신
	void InvaildateInventory();

	// 2021/10/09 PKT - RedDot 갱신
	void InvaildateRedDot();

	// 2021/10/27 PKT - Item Detail 갱신
	void InvalidateItemCollectionSlotItemDetail();


	/************************************************************************/
	/* Logic                                                                */
	/************************************************************************/
private:
	// 2021/08/06 PKT - make filter ..
	TArray<TSharedPtr<GuildItemCollectionFilter>> MakeItemCollectionViewList() const;

	// 2021/10/05 PKT - sub Tab Type에 따른 Filter
	TSharedPtr<GuildItemCollectionFilter> MakeSubTabFilter() const;

	// 2021/10/05 PKT - Search Text 에 따른 Filter
	TSharedPtr<GuildItemCollectionFilter> MakeSearchFilter() const;

	// 2021/10/05 PKT - 유효 기간에 따른 Filter
	TSharedPtr<GuildItemCollectionFilter> MakeValidPeriodFilter() const;

	void OpenItemCollectionSlotItemDetail();

	// 2021/10/27 PKT - Scroll 위치 셋팅
	void ForceScrollOffset(float InOffset);

	// 2021/10/27 PKT - 열려 있는 아이템 상세 정보 창을 닫아준다.
	void CloseItemCollectionSlotItemDetail();

	// 2021/10/27 PKT - inventory에 선택 효과를 준다.
	void InventorySelected(ItemDBId itemDBId);

	// 2021/10/27 PKT - Item 등록
	void RegisterItem(const FGsItem* InItem);

	// 2021/10/29 PKT - 화면을 초기화 시켜야 할 때가 있음.(주마다 리셋)
	void CleanUp();

	
	/************************************************************************/
	/* Bind Event                                                           */
	/************************************************************************/
private:
	// 2021/09/29 PKT - Item Collection Status
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickedCollectionStatus();

	// 2021/09/29 PKT - Search
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod);

	// 2021/09/29 PKT - Search Reset Button
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickedSearchReset();

	// 2021/08/03 PKT - Item Collection Inner Slot Clecked	
	UFUNCTION()
	void OnClickSlotItemCollectionItemIcon(const UGsUIIconItem* InIcon, const uint32 InCollectionId, const uint32 InCollectionConditionId);

	UFUNCTION()
	void OnTextChangedMsg(const FText& InText);

	UFUNCTION()
	void OnScrollCollectionItemListView(float InOffsetInItems, float InDistanceRemaining);

	// 2021/09/29 PKT - Inventory Item Icon Clicked
	void OnClickInventoryItem(ItemDBId InItemDBId);

	// 2021/09/29 PKT - Changed Sub tab
	void OnChangedToggleSubTabGroup(int32 InIndex);

	// 2021/10/27 PKT - Item Detail 닫히면 호출
	void OnItemDetailPopupClose();

	// 2021/08/03 PKT - 교체 버튼
	void OnCollectionItemInventoryShiftSelected(const struct IGsMessageParam* InParam);	

	// 2021/08/06 PKT - Innser Slot에 Item을 등록 하겠다.
	void OnCollectRegisterItem(const struct IGsMessageParam* InParam);

	// 2021/10/27 PKT - 등록시 보상 팝업 활성화
	void ActivePopupRewardData(const struct IGsMessageParam* InParam);


	/************************************************************************/
	/* External Event                                                       */
	/************************************************************************/
public:
	// 2021/10/05 PKT - Item Collection 전체 갱신
	void OnInvalidateItemCollectionListAll();

	// 2021/10/05 PKT - Item Collection 전체 갱신
	void OnCleanUp();

	// 2021/10/05 PKT - 단일 슬롯 아이템에 대한 갱신
	void OnUpdateItemCollectionList(ECollectionStateChangeType InTypye, const TArray<CollectionId>& InUpdatedCollections, bool InInventoryRefresh);

	// 2021/10/05 PKT - 완료한 Item Collection 에 대한 스탯 정보 업데이트
	void OnInvalidateStatStatus();

	// 2021/10/29 PKT - 
	void OnShotcut(ItemId InItemId, const FText& InItemName);
};