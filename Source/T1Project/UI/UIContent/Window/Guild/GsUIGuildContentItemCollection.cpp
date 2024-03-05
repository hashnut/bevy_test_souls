


#include "GsUIGuildContentItemCollection.h"
#include "Guild/GsGuildHelper.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Guild/GsGuildItemCollectionData.h"
#include "Reward/GsRewardHelper.h"
#include "UTIL/GsText.h"

#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "DataSchema/Reward/GsSchemaRewardBox.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsDepotManager.h"

#include "Message/MessageParam/GsItemCollectionMessageParam.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsGuildMessageParam.h"

#include "Classes/GsPoolUObject.h"

#include "Currency/GsCostPackage.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/Inventory/GsUIInventoryCommon.h"
#include "UI/UIContent/Common/GsUITabButtonCommon.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Popup/Guild/GsUIPopupGuildItemCollectionStatus.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Common/Inventory/GsUIInventoryCommon.h"
#include "UI/UIContent/Window/ItemCollection/GsUIItemCollectionListViewEntry.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Popup/GsUIPopupCommonItemAction.h"

#include "Runtime/UMG/Public/Components/ScrollBox.h"
#include "Runtime/UMG/Public/Components/EditableTextBox.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/ListView.h"

#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

void UGsUIGuildContentItemCollection::BeginDestroy()
{
	if (_poolCollectionData)
	{
		_poolCollectionData->RemovePool();
		_poolCollectionData = nullptr;
	}

	if (_slotHelperBtnTabSub)
	{	// 2021/10/27 PKT - 서브 탭
		_slotHelperBtnTabSub->OnRefreshEntry.RemoveDynamic(this, &UGsUIGuildContentItemCollection::OnRefreshSubTabEntry);
		_slotHelperBtnTabSub = nullptr;
	}

	if (_btnCollectionStatus)
	{	// 2021/10/27 PKT - Item collection status
		_btnCollectionStatus->OnClicked.RemoveDynamic(this, &UGsUIGuildContentItemCollection::OnClickedCollectionStatus);
		_btnCollectionStatus = nullptr;
	}

	if (_textSearch)
	{	// 2021/10/27 PKT - search
		_textSearch->OnTextCommitted.RemoveDynamic(this, &UGsUIGuildContentItemCollection::OnTextCommitted);
		_textSearch->OnTextChanged.RemoveDynamic(this, &UGsUIGuildContentItemCollection::OnTextChangedMsg);
		_textSearch = nullptr;
	}

	if (_btnSearchReset)
	{	// 2021/10/27 PKT - reset
		_btnSearchReset->OnClicked.RemoveDynamic(this, &UGsUIGuildContentItemCollection::OnClickedSearchReset);
		_btnSearchReset = nullptr;
	}

	if (_inventoryLogic)
	{	// 2021/10/27 PKT - inventory
		_inventoryLogic.Reset();
	}
	_inventoryLogic = nullptr;
	

	Super::BeginDestroy();
}


void UGsUIGuildContentItemCollection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	/**
	 * sub tab
	 */
	_slotHelperBtnTabSub = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperBtnTabSub->Initialize(_subClassOfSubTabBtnEntry, _scrollBtnTabSubList);
	_slotHelperBtnTabSub->OnRefreshEntry.AddDynamic(this, &UGsUIGuildContentItemCollection::OnRefreshSubTabEntry);

	/**
	 * Item Collection Status
	 */
	_btnCollectionStatus->OnClicked.AddDynamic(this, &UGsUIGuildContentItemCollection::OnClickedCollectionStatus);

	/**
	 * Text Search
	 */
	_textSearch->OnTextCommitted.AddDynamic(this, &UGsUIGuildContentItemCollection::OnTextCommitted);
	_textSearch->OnTextChanged.AddDynamic(this, &UGsUIGuildContentItemCollection::OnTextChangedMsg);

	/**
	 * Text Search Reset
	 */
	_btnSearchReset->OnClicked.AddDynamic(this, &UGsUIGuildContentItemCollection::OnClickedSearchReset);

	/**
	 * toggle
	 */
	_toggleGroupSubTabs.OnSelectChanged.BindUObject(this, &UGsUIGuildContentItemCollection::OnChangedToggleSubTabGroup);	

	/**
	 * 	item collection list	
	 */
	_listViewCollectionItems->OnListViewScrolled().AddUObject(this, &UGsUIGuildContentItemCollection::OnScrollCollectionItemListView);

	/**
	 * Inventory Logic
	 */
	if (nullptr == _inventoryLogic)
	{
		_inventoryLogic = MakeUnique<FGsInventoryLogicItemCollection>();
	}

	_poolCollectionData = NewObject<UGsPoolUObject>(this); // UGsMarketItemGroupListItem
}


void UGsUIGuildContentItemCollection::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (_inventoryLogic.IsValid())
	{	// 2021/10/05 PKT - Set Inventory
		/**
		* Inventory( 속성 : 아이템에 대한 이벤트를 받지 않고, 아이템 갯수에 대한 표기를 하지 않겠다. )
		*/
		_inventoryUI->SetProPerty(EInventoryCommonProperty::ItemActionMessageUnBind | EInventoryCommonProperty::HideInventoryCount);
		_inventoryUI->SetData(_inventoryLogic.Get());
		_inventoryUI->OnClickItem().BindUObject(this, &UGsUIGuildContentItemCollection::OnClickInventoryItem);
	}
}


void UGsUIGuildContentItemCollection::RegisterMessages()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	_itemContentsActionTypeDelegates.Emplace(MessageManager->GetItemContents().AddUObject(MessageContentItem::DETAIL_REGISTER_ITEM_COLLECTION
		, this, &UGsUIGuildContentItemCollection::OnCollectRegisterItem));
	_itemContentsActionTypeDelegates.Emplace(MessageManager->GetItemContents().AddUObject(MessageContentItem::DETAIL_SHIFT_ITEM_COLLECTION
		, this, &UGsUIGuildContentItemCollection::OnCollectionItemInventoryShiftSelected));

	_activePopupRewardData = MessageManager->GetGuild().AddUObject(MessageContentGuild::ITEM_COLLECTION_OPEN_POPUP_REWARD
		, this, &UGsUIGuildContentItemCollection::ActivePopupRewardData);
}

void UGsUIGuildContentItemCollection::UnregisterMessages()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	for (const TPair<MessageContentItem, FDelegateHandle>& ItemDelegate : _itemContentsActionTypeDelegates)
	{
		MessageManager->GetItemContents().Remove(ItemDelegate);
	}
	_itemContentsActionTypeDelegates.Empty();	

	MessageManager->GetGuild().Remove(_activePopupRewardData);
}


void UGsUIGuildContentItemCollection::Show(bool bIsMyGuild, bool IsStackRecoverFlag /*= false*/)
{
	Super::Show(bIsMyGuild, IsStackRecoverFlag);

	RegisterMessages();

	if (false == IsStackRecoverFlag)
	{
		// 2021/10/14 PKT - 검색 정보 초기화
		_textSearch->SetText(FText::GetEmpty());
		_trimSearchText = _textSearch->GetText().ToString();

		_lastItemCollectionScrollOffset = 0.f;

		_focusData.Clear();

		_lastSubTabSelectedIndex = INVALID_TAB_INDEX;
	}
	else
	{
		// 2021/10/20 PKT - 현재 팝업창이 닫히는 이유에 대해 판별할 수 있는 근거가 없어. 뒤로가기후 되돌아 와도 팝업창을 열어 줄수가 없음.
		//					억지로 마지막에 캡처하는 방식을 했지만 그래도 다른 버그가 생겨서 일단은 기능을 막음.
		_focusData.Clear();
		/**
		 *
		 */
	}

	_focusData.Clear();

	// 2021/10/27 PKT - Sub Tab은 Main MainCategory가 결정 된 이후 Setting 되어야 한다.
	_toggleGroupSubTabs.Clear();

	static const int32 subTabMaxCount = StaticCast<int32>(ESubTabSequence::Max);
	_slotHelperBtnTabSub->RefreshAll(subTabMaxCount);

	// 2021/10/27 PKT - Sub Tab 이동
	// 2021/10/14 PKT - Main Tab 이동( Make View List )
	const static int32 BEGIN_INDEX = 0;
	int32 nextSelectredIndex = BEGIN_INDEX;
	if (INVALID_TAB_INDEX < _lastSubTabSelectedIndex)
	{
		nextSelectredIndex = _lastSubTabSelectedIndex;
		_lastSubTabSelectedIndex = INVALID_TAB_INDEX;
	}
	// 2021/10/27 PKT -  Sub Tab 이동 및 화면 갱신
	_toggleGroupSubTabs.SetSelectedIndex(nextSelectredIndex, true);

	// 2021/10/27 PKT - 이전 스크롤을 움직인 상황이라면 스크롤을 움직여 준다.
	ForceScrollOffset(_lastItemCollectionScrollOffset);

	// 2021/10/27 PKT - 이번에 팝업이 열린 상태라면 팝업을 열어준다.
	OpenItemCollectionSlotItemDetail();

	// 2021/10/27 PKT - 등록 가능 갯수 
	InvalidateUsedCount();
}


void UGsUIGuildContentItemCollection::Hide()
{
	Super::Hide();

	_lastSubTabSelectedIndex = _toggleGroupSubTabs.GetSelectedIndex();

	UnregisterMessages();
}


void UGsUIGuildContentItemCollection::OnResetTimeContents(ResetTimeTicket InType)
{
	if (ResetTimeTicket::RTT_WEEKLY != InType)
	{
		// 2021/10/27 PKT - 주간 리셋만 처리 한다.
		return;
	}

	/**
	 * 화면에 있는 모든 정보를 날린다.
	 */

	// 2021/10/14 PKT - 검색 정보 초기화
	_textSearch->SetText(FText::GetEmpty());
	_trimSearchText = _textSearch->GetText().ToString();

	_lastItemCollectionScrollOffset = 0.f;
	_lastSubTabSelectedIndex = INVALID_TAB_INDEX;

	_focusData.Clear();

	// 2021/10/27 PKT -  Sub Tab 이동 및 화면 갱신
	const static int32 BEGIN_INDEX = 0;
	_toggleGroupSubTabs.SetSelectedIndex(BEGIN_INDEX);

	OnInvalidateStatStatus();
}


void UGsUIGuildContentItemCollection::OnRefreshSubTabEntry(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUITabButtonCommon> tabButton = Cast<UGsUITabButtonCommon>(InEntry);
	if (false == tabButton.IsValid())
	{
		GSLOG(Warning, TEXT("false == tabButton.IsValid()"));
		return;
	}

	// 2021/09/29 PKT - sub category Key Tabel
	const static TMap<ESubTabSequence, FString> TabButtonTitleKeyStringTable =
	{
		{ ESubTabSequence::All,			TEXT("UI_ItemCollection_SubCategory_Name_All") }		// 2021/09/29 PKT - "전체"
		, {ESubTabSequence::Completed,	TEXT("UI_ItemCollection_SubCategory_Name_Completed") }	// 2021/09/29 PKT - "완료"
		, {ESubTabSequence::InCompleted, TEXT("UI_ItemCollection_SubCategory_Name_Incomplete") }// 2021/09/29 PKT - "미완료"
		, {ESubTabSequence::BookMark,	TEXT("UI_ItemCollection_SubCategory_Name_BookMark") }	// 2021/09/29 PKT - "즐겨찾기"
	};

	if (true == TabButtonTitleKeyStringTable.Contains(StaticCast<ESubTabSequence>(InIndex) ))
	{
		const FString& keyString = TabButtonTitleKeyStringTable[StaticCast<ESubTabSequence>(InIndex)];
		
		FText titleName;
		FGsGuildHelper::FindItemCollectionSubTabName(keyString, titleName);

		if (false == titleName.IsEmpty())
		{	// 2021/09/29 PKT - 카테고리 이름 설정
			tabButton->SetTitleText(titleName);
		}
	}	

	// 2021/09/29 PKT - toggleGroupSubTabs 내부에서 중복 검사 해줌.
	_toggleGroupSubTabs.AddToggleWidget(tabButton->GetToggleGroupEntry());
}


void UGsUIGuildContentItemCollection::OnClickedCollectionStatus()
{
	// 2021/09/29 PKT - Item Collection Status PopUp Open
	UGsUIManager* UiManager = GUI();
	if (nullptr == UiManager)
	{
		GSLOG(Error, TEXT("nullptr == UiManager"));
		return;
	}
	// 2021/10/05 PKT - 완료한 스탯 상세 정보창
	UiManager->Open("PopupGuildItemCollectionStatus");
}


void UGsUIGuildContentItemCollection::OnTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod)
{
	if (ETextCommit::OnEnter != InCommitMethod)
	{
		return;
	}

	FString trimText = _textSearch->GetText().ToString().TrimStartAndEnd();	
	// 2021/10/05 PKT - 공백 제거
	trimText.ReplaceInline(TEXT(" "), TEXT(""));
	// 2021/10/08 PKT - 공백 제거 이후 아무런 검색어가 없다면
	if (trimText.IsEmpty())
	{	
		FText textMessageTicker;
		FText::FindText(TEXT("GuildText"), TEXT("Search_Input_Text_Empty"), textMessageTicker);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMessageTicker);

		return;
	}
	else
	{
		_trimSearchText = trimText;
	}
	
	// 2021/10/05 PKT - 검색어에 맞게 컬렉션 리스트를 다시 조합한다.
	InvalidateItemCollectionAll();
}


void UGsUIGuildContentItemCollection::OnClickedSearchReset()
{
	if (true == _trimSearchText.IsEmpty())
	{
		// 2021/12/29 PKT - 검색란에 검색어 입력 후 엔터치기 전 리셋 버튼을 누르면 Text가 초기화 되지 않는 현상이 있어 이곳에서 초기화
		_textSearch->SetText(FText::GetEmpty());

		return;
	}

	// 2021/10/12 PKT - 초기화
	_textSearch->SetText(FText::GetEmpty());
	_trimSearchText.Empty();

	InvalidateItemCollectionAll();
}


void UGsUIGuildContentItemCollection::OnClickInventoryItem(ItemDBId InItemDBId)
{
	if (INVALID_ITEM_DB_ID == InItemDBId)
	{
		GSLOG(Warning, TEXT("INVALID_ITEM_DB_ID == InItemDBId"));
		return;
	}

	// 2021/09/29 PKT - Inventory Item
	int32 index = _inventoryLogic->GetDataIndex(InItemDBId);
	const FGsItem* searchItem = (INDEX_NONE != index) ? _inventoryLogic->GetItemByIndex(index) : nullptr;
	
	// 2021/10/08 PKT - Item 있고, 이전 검색어와 같지 않다면
	if (nullptr != searchItem && 0 != _textSearch->GetText().CompareTo(searchItem->GetName()) )
	{
		_textSearch->SetText(searchItem->GetName());
		OnTextCommitted(searchItem->GetName(), ETextCommit::OnEnter);
	}
}


void UGsUIGuildContentItemCollection::OnChangedToggleSubTabGroup(int32 InIndex)
{
	// 2021/10/05 PKT - Item Collection List Refresh
	InvalidateItemCollectionAll();

	InvaildateRedDot();
}

void UGsUIGuildContentItemCollection::OnItemDetailPopupClose()
{
	_focusData.Clear();
	InventorySelected(INVALID_ITEM_DB_ID);
}

void UGsUIGuildContentItemCollection::OnCollectionItemInventoryShiftSelected(const IGsMessageParam* InParam)
{
	const FGsItemItemCollectionShiftParam* param = StaticCast<const FGsItemItemCollectionShiftParam*>(InParam);
	check(param);

	if (1 >= _focusData.itemDBIdQueue.Num())
	{
		_focusData.Clear();

		GSLOG(Error, TEXT("1 >= _focusData.itemDBIdQueue.Num()"));
		return;
	}

	ItemDBId prevItemDBId = _focusData.itemDBIdQueue[0];
	_focusData.itemDBIdQueue.RemoveAt(0);
	_focusData.itemDBIdQueue.Emplace(prevItemDBId);

	// 2022/06/30 PKT - 큐에 있는 DBItem이 실제 존재 하는지 다시 한번 체크.
	while (0 < _focusData.itemDBIdQueue.Num())
	{
		ItemDBId findDBId = _focusData.itemDBIdQueue[0];
		const FGsItem* findItemData = _inventoryLogic->GetItemByDBId(findDBId);
		if (nullptr == findItemData)
		{
			_focusData.itemDBIdQueue.RemoveAt(0);

			continue;
		}
		else
		{
			OpenItemCollectionSlotItemDetail();

			break;
		}
	}

	if (0 >= _focusData.itemDBIdQueue.Num())
	{
		CloseItemCollectionSlotItemDetail();
	}
}


void UGsUIGuildContentItemCollection::OnCollectRegisterItem(const IGsMessageParam* InParam)
{
	const FGsItemItemCollectionRegisterParam* param = StaticCast<const FGsItemItemCollectionRegisterParam*>(InParam);
	check(param);

	do
	{
		/**
		* 등록 패킷 보내기 전 마지막으로 검수 한번..
		*/
		if (0 >= _focusData.itemDBIdQueue.Num() || _focusData.itemDBIdQueue[0] != param->_itemDBId)
		{
			GSLOG(Error, TEXT("0 >= _focusData.itemDBIdQueue.Num() || _focusData.itemDBIdQueue[0] != param->_itemDBId"));
			break;
		}

		UGsItemManager* itemManager = GItem();
		TWeakPtr<FGsItem> targetItem = itemManager->FindItem(param->_itemDBId);
		if (false == targetItem.IsValid())
		{
			// 2021/08/17 PKT - Inventory에서 없다면 창고를 한번 더 살펴 본다.
			FGsDepotManager* depotManager = GSDepot();
			targetItem = (nullptr != depotManager) ? depotManager->FindDepotItemByDBID(param->_itemDBId) : nullptr;
			if (nullptr == targetItem)
			{
				GSLOG(Warning, TEXT("not find Owned Item [%u]"), param->_itemDBId);
				break;;
			}
		}

		const FGsGuildItemCollectionData* itemCollectionData = GGuildItemCollection();
		if (nullptr == itemCollectionData)
		{
			GSLOG(Error, TEXT("nullptr == itemCollectionData"));
			break;
		}

		if (true == itemCollectionData->IsRegisterOverCount())
		{	// 2021/10/27 PKT - 등록 버튼 자체를 활성화 하진 않지만 그래도 한번 더 검사를 해준다.
			break;
		}

		const FGsGuildItemCollectionData::ArrayCollectionList_CPtr& collectionViewList = itemCollectionData->ItemCollectionViewList();
		const FGsItemCollectionSlotState* findItemCollectionSlot = nullptr;
		for (const FGsGuildItemCollectionState* itemCollection : collectionViewList)
		{
			if (itemCollection->GetCollectionId() == _focusData.collectionId)
			{
				findItemCollectionSlot = itemCollection->FindInnerSlotByCollectionConditionId(_focusData.collectionConditionId);
			}
		}
		if (nullptr == findItemCollectionSlot)
		{
			GSLOG(Error, TEXT("nullptr == findItemCollectionSlot"));
			break;;
		}

		if (false == findItemCollectionSlot->IsCanCollect(targetItem.Pin().Get()))
		{
			GSLOG(Error, TEXT("false == findItemCollectionSlot->IsCanCollectByItem(targetItem)"));
			break;
		}

		RegisterItem(targetItem.Pin().Get());

	} while (0);

	// 2021/07/25 PKT - 디테일 창을 닫아준다.
	CloseItemCollectionSlotItemDetail();
}

void UGsUIGuildContentItemCollection::ActivePopupRewardData(const IGsMessageParam* InParam)
{
	const FGsGuildMsgParamItemCollectionReward* param = StaticCast<const FGsGuildMsgParamItemCollectionReward*>(InParam);
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	const FGsGuildItemCollectionData* itemCollectionData = GGuildItemCollection();
	if (nullptr == itemCollectionData)
	{
		GSLOG(Error, TEXT("nullptr == itemCollectionData"));
		return;
	}

	const FGsGuildItemCollectionState* itemCollectionState = itemCollectionData->FindItemCollectionByCollectionConditionId(param->collectionConditionId);
	if (nullptr == itemCollectionState)
	{
		GSLOG(Error, TEXT("nullptr == itemCollectionState"));
		return;
	}

	int32 findIndex = itemCollectionState->FindInnerSlotIndexByCollectionConditionId(param->collectionConditionId);
	const FGsItemCollectionSlotState* itemCollectionSlot = itemCollectionState->FindInnerSlotByIndex(findIndex);
	if (nullptr == itemCollectionSlot)
	{
		GSLOG(Error, TEXT("nullptr == itemCollectionSlot"));
		return;
	}
	
	const FGsSchemaRewardData* rewardData = itemCollectionSlot->GetRewardData();
	if (nullptr == rewardData)
	{
		GSLOG(Error, TEXT("nullptr == rewardData"));
		return;
	}

	FGsRewardUnboxingData outUnBoxingData;
	FGsRewardHelper::UnboxingRewardData(rewardData->id, outUnBoxingData);

	CurrencyType currencyType = CurrencyType::NONE;
	Currency currencyAmount = 0;
	TArray<TSharedPtr<FGsItem>> rewarditemList;
	TArray<FGsItem*> itemList;
	
	for (const FGsSchemaRewardItemBagData* bagData : outUnBoxingData._itemDataList)
	{
		if (CurrencyType::NONE != bagData->currencyType)
		{	// 2021/10/28 PKT - 재화 정보가 있다면 RewardData 에는 보상 정보가 없어야 한다.
			currencyType = bagData->currencyType;
			currencyAmount = bagData->currencyAmountMin;
		}
		else
		{
			if (0 < bagData->itemId)
			{
				auto ref = rewarditemList.Emplace_GetRef(FGsItem::Create(bagData->itemId, bagData->itemCountMin));
				itemList.Emplace(ref.Get());
			}			
		}
	}
	if (CurrencyType::NONE == currencyType)
	{	// 2021/10/28 PKT - 재화 정보가 있다면 RewardData 에는 보상 정보가 없어야 한다.
		currencyType = outUnBoxingData._currencyType;
		currencyAmount = outUnBoxingData.GetCommonCurrencyAmount();
	}

	UGsUIPopupCommonItemAction* popupItemAction = Cast<UGsUIPopupCommonItemAction>(GUI()->OpenAndGetWidget(FName(TEXT("PopupCommonItemAction"))).Get());
	if (nullptr == popupItemAction)
	{
		GSLOG(Error, TEXT("nullptr == popupItemAction"));
		return;
	}
	
	FText textDesc;
	FText::FindText(TEXT("GuildText"), TEXT("UI_ITEM_COLLECTION_REWARD_NOTICE"), textDesc);

	CollectionConditionId collectionConditionId = param->collectionConditionId;
	ItemDBId itemDBId = param->itemDBId;
	popupItemAction->SetItemList(itemList);
	popupItemAction->SetDescription(textDesc);
	popupItemAction->SetCurrencyData(currencyType, currencyAmount);
	popupItemAction->SetCallBack([collectionConditionId, itemDBId](bool InIsOk)
		{
			if (false == InIsOk)
			{
				return;
			}

			GGuildItemCollection()->Request_ItemCollection_Register(collectionConditionId, itemDBId);
		});
}

void UGsUIGuildContentItemCollection::OnClickSlotItemCollectionItemIcon(const UGsUIIconItem* InIcon
																		, const uint32 InCollectionId
																		, const uint32 InCollectionConditionId)
{
	const FGsGuildItemCollectionData* itemCollectionData = GGuildItemCollection();
	if (nullptr == itemCollectionData)
	{
		GSLOG(Warning, TEXT("nullptr == itemCollectionData"));
		return;
	}

	const FGsGuildItemCollectionData::ArrayCollectionList_CPtr& collectionList = itemCollectionData->ItemCollectionViewList();
	int32 index = collectionList.IndexOfByPredicate([InCollectionId](const FGsGuildItemCollectionState* InCollectionState)
		{
			return (InCollectionState->GetCollectionId() == InCollectionId);
		}
	);

	if (INDEX_NONE >= index)
	{
		GSLOG(Warning, TEXT("INDEX_NONE == index"));
		return;
	}

	const FGsGuildItemCollectionState* collection = collectionList[index];
	const FGsItemCollectionSlotState* CollectionSlot = (nullptr != collection)
		? collection->FindInnerSlotByCollectionConditionId(InCollectionConditionId) : nullptr;
	if (nullptr == CollectionSlot)
	{
		GSLOG(Error, TEXT("nullptr == CollectionSlot"));
		return;
	}

	// 2022/06/30 PKT - 현재 화면에 보여지고 있는 컬렉션 List
	TArray<TPair<EItemCollectionSlotState, FGsItemCollectionSlotState::MaterialData>> materialSet;
	// 2022/06/30 PKT - 즉시 등록 가능한 아이템을 가져 온다.
	CollectionSlot->GetHasMaterialItemIdSet(materialSet);

	_focusData.collectionId = InCollectionId;
	_focusData.collectionConditionId = CollectionSlot->GetItemCollectionConditionId();
	_focusData.itemDBIdQueue.Empty();

	if (materialSet.Num())
	{	// 2022/06/30 PKT - 즉시 / 조건부 등록 가능한 아이템이 있는 경우		
		_inventoryLogic->FindItemSet(materialSet, &_focusData.itemDBIdQueue);
	}

	OpenItemCollectionSlotItemDetail();
}

void UGsUIGuildContentItemCollection::OnTextChangedMsg(const FText& InText)
{
	FString strTemp = InText.ToString();
	int32 length = strTemp.Len();

	// 지정 글자수 이상 입력하지 못하게 막음
	static const int32 maxLength = 50;
	if (maxLength < length)
	{
		strTemp.RemoveAt(strTemp.Len() - 1);
	}

	_textSearch->SetText(FText::FromString(strTemp));
}

void UGsUIGuildContentItemCollection::OnScrollCollectionItemListView(float InOffsetInItems, float InDistanceRemaining)
{
	_lastItemCollectionScrollOffset = InOffsetInItems;
}


void UGsUIGuildContentItemCollection::InvalidateItemCollectionAll()
{
	const FGsGuildItemCollectionData* collectionData = GGuildItemCollection();
	if (nullptr == collectionData)
	{
		GSLOG(Error, TEXT("nullptr == collectionData"));
		return;
	}

	FGsGuildItemCollectionData::ECategoryValue categoryValue = FGsGuildItemCollectionData::ECategoryValue::All;
	switch (StaticCast<ESubTabSequence>(_toggleGroupSubTabs.GetSelectedIndex()))
	{
	case ESubTabSequence::All:
		categoryValue = FGsGuildItemCollectionData::ECategoryValue::All;
		break;
	case ESubTabSequence::InCompleted:
		categoryValue = FGsGuildItemCollectionData::ECategoryValue::Incomplete;
		break;
	case ESubTabSequence::Completed:
		categoryValue = FGsGuildItemCollectionData::ECategoryValue::Complete;
		break;
	case ESubTabSequence::BookMark:
		categoryValue = FGsGuildItemCollectionData::ECategoryValue::BookMark;
		break;
	};

	// 2021/10/05 PKT - Collection Filter를 만들어 준다.
	const int32 CollectionListCount = collectionData->BuildItemCollectionViewList(categoryValue, MakeItemCollectionViewList());

	const static int32 EMPTY_COUNT = 0;
	const static int32 FILL_LIST_SWITCHER_INDEX = 0;
	const static int32 EMPTY_LIST_SWITCHER_INDEX = 1;

	if (EMPTY_COUNT >= CollectionListCount)
	{
		FText MessageText;
		FText::FindText(TEXT("GuildText"), TEXT("ItemCollectionEmpty"), MessageText);
		_textItemCollectionList->SetText(MessageText);
		_switcherItemCollectionList->SetActiveWidgetIndex(EMPTY_LIST_SWITCHER_INDEX);
	}
	else
	{
		_switcherItemCollectionList->SetActiveWidgetIndex(FILL_LIST_SWITCHER_INDEX);

		const TArray<const FGsGuildItemCollectionState*>& collectionViewList = collectionData->ItemCollectionViewList();

		// 2022/01/04 PKT - List를 한번에 셋팅
		TArray<UGsItemIconCollectionListViewEntryData*> entryList;

		_poolCollectionData->ReleaseAll();

		for (int32 idx = 0; idx < collectionViewList.Num(); ++idx)
		{
			UGsItemIconCollectionListViewEntryData* entry = _poolCollectionData->Claim<UGsItemIconCollectionListViewEntryData>();
			entry->_collectionData = collectionViewList[idx];
			if (false == entry->OnClickedItemIcon.IsBoundToObject(this))
			{
				entry->OnClickedItemIcon.BindUObject(this, &UGsUIGuildContentItemCollection::OnClickSlotItemCollectionItemIcon);
			}

			entryList.Emplace(entry);
		}
		// 2022/01/04 PKT - List 초기화
		_listViewCollectionItems->SetListItems(entryList);
		_listViewCollectionItems->RegenerateAllEntries();
		_listViewCollectionItems->ScrollToTop();
		_listViewCollectionItems->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	}

	// 2021/10/09 PKT - Inventory
	InvaildateInventory();
}


void UGsUIGuildContentItemCollection::InvalidateItemCollection(const TArray<CollectionId>& InUpdatedCollectionIds)
{
	// 2022/01/06 PKT - 직접 갱신
	const TArray<UUserWidget*>& displayedWidgetList = _listViewCollectionItems->GetDisplayedEntryWidgets();
	for (UUserWidget* displayedWidget : displayedWidgetList)
	{
		//TWeakObjectPtr<UGsUIItemCollectionListViewEntry> collection = Cast<UGsUIItemCollectionListViewEntry>(displayedWidget);
		//if (false == collection.IsValid())
		//{
		//	continue;
		//}

		//if (collection->GetData() && InUpdatedCollectionIds.Contains(collection->GetData()->GetCollectionId()))
		//{
		//	collection->Refresh();
		//}
	}
}


void UGsUIGuildContentItemCollection::InvalidateUsedCount()
{
	int32 registerCount = 0;
	int32 registerCountMax = 0;

	const FGsGuildItemCollectionData* collectionData = GGuildItemCollection();
	if (nullptr != collectionData)
	{	// 2021/09/29 PKT - 자신이 등록한 갯수 / 최대 등록 가능 갯수
		collectionData->GetRegisterUsedCount(&registerCount, &registerCountMax);
	}

	registerCount = FMath::Min(registerCount, registerCountMax);

	_textUseCount->SetText(FText::FromString(FString::Format(TEXT("{0}/{1}"), { registerCount, registerCountMax })));

	FLinearColor color = FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_VALID);
	if (registerCount >= registerCountMax)
	{
		color = FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_INVALID);
	}
	_textUseCount->SetColorAndOpacity(color);
}

void UGsUIGuildContentItemCollection::InvaildateInventory()
{
	const FGsGuildItemCollectionData* collectionData = GGuildItemCollection();

	if (_inventoryLogic && collectionData)
	{
		TArray<TPair<EItemCollectionSlotState, FGsItemCollectionSlotState::MaterialData>> materialSet;

		if (false == collectionData->IsRegisterOverCount())
		{
			for (const FGsItemCollectionBase* itemCollection : collectionData->ItemCollectionViewList())
			{
				const int32 maxCount = itemCollection->GetCollectionItemsCount();
				for (int32 idx = 0; idx < maxCount; ++idx)
				{
					const FGsItemCollectionSlotState* innerSlot = itemCollection->FindInnerSlotByIndex(idx);
					if (nullptr == innerSlot)
					{
						GSLOG(Error, TEXT("nullptr == innerSlot"));
						return;
					}

					innerSlot->GetHasMaterialItemIdSet(materialSet);
				}
			}

			FText emptyDesc;
			FText::FindText(TEXT("GuildText"), TEXT("GuildItemCollectEmptInventoryList"), emptyDesc);
			_inventoryUI->SetEmptyDesc(emptyDesc);
		}
		else
		{
			materialSet.Empty();

			FText emptyDesc;
			FText::FindText(TEXT("GuildText"), TEXT("GuildItemCollectDailyCompleteText"), emptyDesc);
			_inventoryUI->SetEmptyDesc(emptyDesc);
		}

		_inventoryLogic->InvalidateList(materialSet);
	}

	InvalidateItemCollectionSlotItemDetail();
}


void UGsUIGuildContentItemCollection::InvaildateRedDot()
{
	// 2021/10/09 PKT - 서브 탭 레드닷은 "전체" 카테고리만 대상임.
	int32 targetIndex = StaticCast<int32>(ESubTabSequence::All);
	TWeakObjectPtr<UGsUITabButtonCommon> item
		= StaticCast<UGsUITabButtonCommon*>(_slotHelperBtnTabSub->GetEntry(targetIndex));
	if (false == item.IsValid())
	{
		GSLOG(Error, TEXT("false == entry.IsValid()"));
		return;
	}

	bool IsActiveRedDot = GGuildItemCollection()->IsRedDot();
	item->SetIsRedDot(EGsIconRedDotType::NORMAL, IsActiveRedDot);
}


void UGsUIGuildContentItemCollection::InvalidateItemCollectionSlotItemDetail()
{
	_inventoryLogic->InvalidateSelected(INVALID_ITEM_DB_ID);

	if (_focusData.collectionId != INVALID_COLLECTION_ID && _focusData.collectionConditionId != INVALID_COLLECTION_CONDITION_ID)
	{
		OpenItemCollectionSlotItemDetail();
	}
}


TArray<TSharedPtr<GuildItemCollectionFilter>> UGsUIGuildContentItemCollection::MakeItemCollectionViewList() const
{
	TArray<TSharedPtr<GuildItemCollectionFilter>> outFilters;

	auto AddFilter = [](TSharedPtr<GuildItemCollectionFilter> filter
		, TArray<TSharedPtr<GuildItemCollectionFilter>>& outFilters)
	{
		if (filter.IsValid())
		{
			outFilters.Emplace(filter);
		}
	};

	// 2021/10/05 PKT - Sub Tab Filter
	AddFilter(MakeSubTabFilter(), outFilters);
	// 2021/10/05 PKT - Search Text Filter
	AddFilter(MakeSearchFilter(), outFilters);
	// 2021/10/05 PKT - 유효 기간 검사
	AddFilter(MakeValidPeriodFilter(), outFilters);

	// 2021/10/05 PKT - Filter Return
	return outFilters;
}


TSharedPtr<GuildItemCollectionFilter> UGsUIGuildContentItemCollection::MakeSubTabFilter() const
{
	TSharedPtr<GuildItemCollectionFilter> outFilter = MakeShared<GuildItemCollectionFilter>();

	ESubTabSequence subTabSequence = StaticCast<ESubTabSequence>(_toggleGroupSubTabs.GetSelectedIndex());

	outFilter->Condition = [subTabSequence](const FGsGuildItemCollectionState* InItemCollectionState) -> bool
	{
		if (ESubTabSequence::Completed == subTabSequence && InItemCollectionState->GetType() != EItemCollectionState::Completed)
		{	// 2021/10/05 PKT - 완료 상태 Tab일때는 완료 상태의 콜렉션만
			return false;
		}
		else if (ESubTabSequence::InCompleted == subTabSequence && InItemCollectionState->GetType() == EItemCollectionState::Completed)
		{	// 2021/10/05 PKT - 진행 중 탭일때는 진행중인 콜렉션만
			return false;
		}
		else if (ESubTabSequence::BookMark == subTabSequence && false == InItemCollectionState->IsOnBookMark())
		{	// 2021/10/05 PKT - 즐겨찾기 항목 일때는 즐겨찾기 설정이 되어 있는 것만
			return false;
		}

		// 2021/10/05 PKT - 그 외 모두 통과
		return true;
	};
	
	return outFilter;
}


TSharedPtr<GuildItemCollectionFilter> UGsUIGuildContentItemCollection::MakeSearchFilter() const
{
	if (true == _trimSearchText.IsEmpty())
	{	// 2021/10/05 PKT - 검색어가 없으면 nullptr 리턴
		return nullptr;
	}

	TSharedPtr<GuildItemCollectionFilter> outFilter = MakeShared<GuildItemCollectionFilter>();

	FString TrimText = _trimSearchText;
	outFilter->Condition = [TrimText](const FGsGuildItemCollectionState* InItemCollectionState) -> bool
	{	// 2021/10/05 PKT - 검색어에 포함 되어 있는가를 검사.
		return InItemCollectionState->TitleAndCollectionItemNameContains(TrimText);
	};

	return outFilter;
}


TSharedPtr<GuildItemCollectionFilter> UGsUIGuildContentItemCollection::MakeValidPeriodFilter() const
{
	TSharedPtr<GuildItemCollectionFilter> outFilter = MakeShared<GuildItemCollectionFilter>();

	const FDateTime& serverNowDateTimeUTC = FGsTimeSyncUtil::GetServerNowDateTimeUTC();

	outFilter->Condition = [serverNowDateTimeUTC](const FGsGuildItemCollectionState* InItemCollectionState) -> bool
	{	// 2021/10/05 PKT - 유효기간
		return InItemCollectionState->IsValidPeriod(serverNowDateTimeUTC);
	};

	return outFilter;
}

void UGsUIGuildContentItemCollection::OpenItemCollectionSlotItemDetail()
{
	bool isClearFocusData = true;
	bool isClosePopup = true;
	do
	{
		if (_focusData.collectionId == INVALID_COLLECTION_ID || _focusData.collectionConditionId == INVALID_COLLECTION_CONDITION_ID)
		{
			break;
		}

		CollectionId findCollectionId = _focusData.collectionId;

		const TArray<const FGsGuildItemCollectionState*>& collectionViewList = GGuildItemCollection()->ItemCollectionViewList();
		const FGsGuildItemCollectionState* itemCollection = *(collectionViewList.FindByPredicate([findCollectionId](const FGsGuildItemCollectionState* InItemCollection)
			{
				return (InItemCollection->GetCollectionId() == findCollectionId);
			}
		));
		if (nullptr == itemCollection)
		{
			break;
		}

		const FGsItemCollectionSlotState* CollectionSlot = itemCollection->FindInnerSlotByCollectionConditionId(_focusData.collectionConditionId);
		if (nullptr == CollectionSlot)
		{
			break;;
		}


		const static int32 BEGIN_INDEX = 0;
		ItemDBId findItemDBId = (BEGIN_INDEX < _focusData.itemDBIdQueue.Num()) ? _focusData.itemDBIdQueue[BEGIN_INDEX] : INVALID_ITEM_DB_ID;
		const FGsItem* findItemData = _inventoryLogic->GetItemByDBId(findItemDBId);

		if (nullptr == findItemData)
		{
			FGsItemCollectionSlotState::MaterialData mainMaterialData = CollectionSlot->GetMainMaterialData();
			const FGsItemDetailOpenMessageParamTID parm(mainMaterialData.Key, true, true, UGsUIPopupItemDetailMain::CenterPosType::CenterPos);
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_TID, &parm);
			
			isClosePopup = false;
		}
		else
		{
			// 2022/06/30 PKT - 관련 아이템이 복수개가 존재 하면 교체 버튼이 활성화 되야 한다.
			bool isActiveShiftAbleBtn = (1 < _focusData.itemDBIdQueue.Num()) ? true : false;

			// 2021/08/03 PKT - 즉시 등록 & 조건부 등록 가능한 대상인가?
			bool isAVailable = itemCollection->IsItemAvailableItem(findItemData);

			// 2021/10/15 PKT - 즉시 등록 가능한 아이템인가?
			bool isCanCollect = itemCollection->IsItemCanCollectItem(findItemData);

			// 2021/08/03 PKT - 인첸트 활성화
			bool isEnchant = (true == isAVailable || true == isCanCollect) && (false == findItemData->IsDepotItem());

			const FGsItemDetailItemCollectionOpenMsgParam parm(findItemData, true, isEnchant, isActiveShiftAbleBtn, isCanCollect);
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_ITEM_COLLECTION, &parm);

			// 2021/10/15 PKT - 인벤토리에 선택 효과를 갱신
			InventorySelected(findItemData->GetDBID());

			isClearFocusData = false;

			isClosePopup = false;
		}

		if (false == isClosePopup)
		{
			FText textDesc;
			if (EItemCollectionSlotState::Completed == CollectionSlot->GetType())
			{
				FText buff;
				// 2022/02/24 PKT - "{0}님이 등록을 완료하였습니다."
				FText::FindText(TEXT("GuildText"), TEXT("guildItemCollectionRegisterText"), buff);

				const FText& userName = CollectionSlot->GetTextBuffer();
				textDesc = FText::FromString(FString::Format(*buff.ToString(), { userName.ToString() }));
			}
			else
			{
				// 2022/02/24 PKT - "등록이 완료되지 않은 아이템입니다."
				FText::FindText(TEXT("GuildText"), TEXT("guildItemCollectionUnregisteredText"), textDesc);
			}

			// 2022/07/01 PKT - Set Text Message
			const FGsItemDetailTextPropertyParam textParam(&textDesc);
			GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_ITEM_COLLECTION_TEXT, &textParam);
		}		

	} while (0);

	

	if (isClearFocusData)
	{
		_focusData.Clear();
	}

	if (true == isClosePopup)
	{
		CloseItemCollectionSlotItemDetail();
	}
	else
	{	// 2021/08/05 PKT - Item Detail Popup Close Message Bind
		UGsUIManager* uiManager = GUI();
		if (nullptr != uiManager->GetWidgetByKey(TEXT("PopupItemDetail")))
		{
			UGsUIPopupItemDetailMain* popup = Cast<UGsUIPopupItemDetailMain>(uiManager->GetWidgetByKey(TEXT("PopupItemDetail")));
			if (false == popup->OnCloseItemDetailMainDelegate.IsBoundToObject(this))
			{
				popup->OnCloseItemDetailMainDelegate.AddUObject(this, &UGsUIGuildContentItemCollection::OnItemDetailPopupClose);
			}
		}
	}
}

void UGsUIGuildContentItemCollection::ForceScrollOffset(float InOffset)
{
	_listViewCollectionItems->SetScrollOffset(InOffset);
}



void UGsUIGuildContentItemCollection::CloseItemCollectionSlotItemDetail()
{
	_focusData.Clear();
	InventorySelected(INVALID_ITEM_DB_ID);

	// 2021/07/24 PKT - Item Detail Close
	FGsItemDetailCloseMessageParam param;
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, &param);
}


void UGsUIGuildContentItemCollection::InventorySelected(ItemDBId itemDBId)
{
	if (nullptr == _inventoryLogic)
	{
		GSLOG(Error, TEXT("nullptr == _inventoryLogic"));
		return;
	}

	_inventoryLogic->InvalidateSelected(itemDBId);
}

void UGsUIGuildContentItemCollection::RegisterItem(const FGsItem* InItem)
{
	if (nullptr == InItem)
	{
		GSLOG(Error, TEXT("nullptr == InItem"));
		return;
	}

	FText PopupMessage;
	FText::FindText(TEXT("CollectionText"), TEXT("Item_Collect_Alert"), PopupMessage);

	const CollectionConditionId collectionConditionId = _focusData.collectionConditionId;
	const ItemDBId targetItemDBId = InItem->GetDBID();

	if (InItem->IsDepotItem())
	{
		UGsUIPopupCommonPay* PopupCommonPay = Cast<UGsUIPopupCommonPay>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPay")).Get());
		if (nullptr == PopupCommonPay)
		{
			GSLOG(Error, TEXT("nullptr == PopupCommonPay"));
			return;
		}
		else
		{
			FGsDepotManager* DepotManager = GSDepot();
			if (nullptr == DepotManager)
			{
				GSLOG(Error, TEXT("nullptr == DepotManager"));
				return;
			}
			else
			{
				PopupCommonPay->SetData(PopupMessage,
					FGsCostPackage::CreateSimple(CurrencyType::GOLD, DepotManager->GetDepotWithdrawCost()),
					[collectionConditionId, targetItemDBId](bool InIsOk)
					{
						if (!InIsOk)
						{
							return;
						}

						if (GSDepot()->GetDepotWithdrawCost() <= GGameData()->GetCurrency(CurrencyType::GOLD))
						{							
							const FGsGuildMsgParamItemCollectionReward param(collectionConditionId, targetItemDBId);
							GMessage()->GetGuild().SendMessage(MessageContentGuild::ITEM_COLLECTION_OPEN_POPUP_REWARD, &param);
							return;
						}

						// 재화 모자름 ticker 출력
						FText Text;
						FText::FindText(TEXT("DepotText"), TEXT("Alert_NoCost"), Text);
						FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, Text);
					});
			}
		}
	}
	else
	{
		const FGsGuildMsgParamItemCollectionReward param(collectionConditionId, targetItemDBId);
		GMessage()->GetGuild().SendMessage(MessageContentGuild::ITEM_COLLECTION_OPEN_POPUP_REWARD, &param);
	}
}


void UGsUIGuildContentItemCollection::CleanUp()
{
	// 2021/10/29 PKT - 디테일 창 닫기
	CloseItemCollectionSlotItemDetail();

	UGsUIManager* UiManager = GUI();
	if (nullptr != UiManager)
	{
		auto closePopUp = [=](const FName& InKey)
		{
			if (true == UiManager->IsActiveWidget(InKey))
			{
				TWeakObjectPtr<UGsUIWidgetBase> widget = UiManager->GetWidgetByKey(InKey);
				if (widget.IsValid())
				{
					widget->Close();
				}
			}
		};

		// 2021/10/29 PKT - 길드 아이템 콜렉션에서 사용 중인 팝업창 모두 닫아준다.
		closePopUp("PopupCommonPay");
		closePopUp("PopupCommonItemAction");
		closePopUp("PopupGuildItemCollectionStatus");
	}

	// 2021/10/14 PKT - 검색 정보 초기화
	_textSearch->SetText(FText::GetEmpty());
	_trimSearchText = _textSearch->GetText().ToString();

	_lastItemCollectionScrollOffset = 0.f;
	_lastSubTabSelectedIndex = INVALID_TAB_INDEX;

	ForceScrollOffset(_lastItemCollectionScrollOffset);
}

// 2021/10/05 PKT - Item Collection 전체 갱신
void UGsUIGuildContentItemCollection::OnInvalidateItemCollectionListAll()
{
	CleanUp();
	
	// 2021/10/29 PKT - 서브 탭을 강제로 "전체"에 맞춰 준다.
	_toggleGroupSubTabs.SetSelectedIndex(StaticCast<int32>(ESubTabSequence::All), true);
}

// 2021/10/29 PKT - Item Collection 상태 초기화
void UGsUIGuildContentItemCollection::OnCleanUp()
{
	CleanUp();

	InvaildateRedDot();
}

void UGsUIGuildContentItemCollection::OnUpdateItemCollectionList(ECollectionStateChangeType InTypye
																, const TArray<CollectionId>& InUpdatedCollections
																, bool InInventoryRefresh)
{
	const ESubTabSequence subTabSequence = StaticCast<ESubTabSequence>(_toggleGroupSubTabs.GetSelectedIndex());
	if (ESubTabSequence::BookMark == subTabSequence && ECollectionStateChangeType::EBookMark == InTypye)
	{	// 2021/10/14 PKT - 현재 북마크를 리스트를 보여주고 있는 중... 북마크 관련 갱신이 온다는 것은 북마크를 해제 한것 뿐이 없음.
		InvalidateItemCollectionAll();

		// 2021/10/14 PKT - 재 갱신을 요청 했기 때문에 인벤토리는 당연히 갱신.
		InInventoryRefresh = true;
	}
	else
	{
		InvalidateItemCollection(InUpdatedCollections);
	}

	InvalidateUsedCount();

	if (true == InInventoryRefresh)
	{
		InvaildateInventory();
	}

	InvaildateRedDot();
}


void UGsUIGuildContentItemCollection::OnInvalidateStatStatus()
{
	// 2021/09/29 PKT - Item Collection Status PopUp Open
	UGsUIManager* UiManager = GUI();
	if (nullptr == UiManager)
	{
		GSLOG(Error, TEXT("nullptr == UiManager"));
		return;
	}
	// 2021/10/05 PKT - 완료한 스탯 상세 정보창
	TWeakObjectPtr<UGsUIWidgetBase> widget = UiManager->GetWidgetByKey("PopupGuildItemCollectionStatus");
	TWeakObjectPtr<UGsUIPopupGuildItemCollectionStatus> itemCollectionStatus =
		(true == widget.IsValid()) ? Cast<UGsUIPopupGuildItemCollectionStatus>(widget.Get()) : nullptr;

	// 2021/10/05 PKT - 상세 정보창이 활성화 중이라면 갱신함.
	if (itemCollectionStatus.IsValid() && itemCollectionStatus->IsActive())
	{
		itemCollectionStatus->InvalidateAll();
	}
}


void UGsUIGuildContentItemCollection::OnShotcut(ItemId InItemId, const FText& InItemName)
{
	_toggleGroupSubTabs.SetSelectedIndex(StaticCast<int32>(ESubTabSequence::All), false);
	_textSearch->SetText(InItemName);
	OnTextCommitted(InItemName, ETextCommit::OnEnter);
}