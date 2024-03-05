// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupInventory.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"
#include "Components/ScrollBox.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsCheckBox.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPayInventory.h"
#include "UI/UIContent/Popup/GsUIPopupCalculator.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"

#include "Item/GsItem.h"
#include "Item/GsItemConsumable.h"
#include "Item/GsItemHelper.h"
#include "Item/GsItemManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsInvenMessageParam.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsInventoryManager.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Message/MessageParam/GsInvenMessageParam.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"

#include "Currency/GsCostPackage.h"
#include "Net/GsNetSendServiceItem.h"

//#include "T1Project.h"



void UGsUIPopupInventory::BeginDestroy()
{
	if (nullptr != _scrollHelper)
	{
		_scrollHelper->Clear();
		_scrollHelper = nullptr;
	}

	_toggleGroup.Clear();

	Super::BeginDestroy();
}

void UGsUIPopupInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_invenMgrPtr = GSInventory();
	_createSlotCountEveryTick = _invenMgrPtr->CREATE_EVERY_TICK_COUNT_DEFAULT;

	if (nullptr == _scrollHelper)
	{
		_scrollHelper = NewObject<UGsDynamicIconSlotHelper>(this);
		_scrollHelper->Initialize(_iconSelector);
		_scrollHelper->OnRefreshIcon.AddDynamic(this, &UGsUIPopupInventory::OnRefreshIconWrapBox);
	}

	_toggleGroup.Clear();
	_toggleGroup.OnSlotClicked.BindUObject(this, &UGsUIPopupInventory::OnSlotClickedTab);
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupInventory::OnSelectChangedTab);
	_toggleGroup.AddToggleWidgetByParentPanel(_panelInvenTab);

	_btn_close->OnClicked.AddDynamic(this, &UGsUIPopupInventory::OnClickClose);
	_btnSortList->OnClicked.AddDynamic(this, &UGsUIPopupInventory::OnClickSortList);

	_btnDefaultSort->OnClicked.AddDynamic(this, &UGsUIPopupInventory::OnClickDefaultSortList);
	_btnGradeSort->OnClicked.AddDynamic(this, &UGsUIPopupInventory::OnClickGradeSortList);
	_btnWeightSort->OnClicked.AddDynamic(this, &UGsUIPopupInventory::OnClickWeightSortList);
	_btnItemSlotLock->OnClicked.AddDynamic(this, &UGsUIPopupInventory::OnClickSlotLock);
	_btnDecompose->OnClicked.AddDynamic(this, &UGsUIPopupInventory::OnClickDecompose);

	_scrollBox->OnUserScrolled.AddDynamic(this, &UGsUIPopupInventory::OnScrollList);
}

void UGsUIPopupInventory::NativeConstruct()
{
	Super::NativeConstruct();

	if (false == _invenMgrPtr.IsValid())
	{
		_invenMgrPtr = GSInventory();
	}

	_isDimmedState = false;
	_createMaxSlotCountByTick = 0;
	_selectionItemDbid = 0;
	GSQuickSlot()->SetInvenSelectionItem(_selectionItemDbid);

	// 상단 탭 초기 설정
	_invenTabIndex = 0;
	_toggleGroup.SetSelectedIndex(_invenTabIndex);

	// 상단 탭 레드닷 초기 설정
	_redDotImageList.Add(EInventoryTab::AllTab, _allTabRedDot);
	_redDotImageList.Add(EInventoryTab::EquipmentTab, _equipmentTabRedDot);
	_redDotImageList.Add(EInventoryTab::ConsumableTab, _consumableTabRedDot);
	_redDotImageList.Add(EInventoryTab::MaterialTab, _etcItemTypeRedDot);
	CheckRedDot();
	
	CloseSortItemListActive();

	InvalidateSlotCount();
	InvalidateExpandButton();

	_invenMgrPtr->UpdateInvenWeightRate();

	BindMessage();

	_emptyItemListWarnRoot->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIPopupInventory::NativeDestruct()
{
	// 인벤토리 닫을 때 레드닷 전체 OFF
	UGsItemManager* itemMgr = GItem();
	itemMgr->AllOffItemRedDotByType(ItemType::EQUIP);
	itemMgr->AllOffItemRedDotByType(ItemType::CONSUME);
	itemMgr->AllOffItemRedDotByType(ItemType::INGREDIENT);
	itemMgr->AllOffItemRedDotByType(ItemType::CURRENCY);
	_redDotImageList.Empty();
	
	_diplayItemList.Empty();

	if (nullptr != _iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	// 튜토리얼 관련
	SetTutorialItemDBId(0);

	if (false == _invenMgrPtr.IsValid())
	{
		_invenMgrPtr = GSInventory();
	}
	_invenMgrPtr->CloseInventoryPopup(true, false);

	UnBindMessage();

	_invenMgrPtr.Reset();

	_tempScrollTargetItem = INVALID_ITEM_DB_ID;

	if (UGsUIManager* uiManager = GUI())
	{
		uiManager->CloseByKeyName(TEXT("PopupItemDecompose"));
	}

	Super::NativeDestruct();
}

// Only Change From ServerPacket Data
void UGsUIPopupInventory::SetWeightPenaltyType(WeightPenaltyGrade InPenaltyGradeType)
{
	_weightSwitcher->SetActiveWidgetIndex((uint8)InPenaltyGradeType);
}

void UGsUIPopupInventory::SetWeightPercentInfo(int32 InWeight, float InWeightPercent)
{
	SetWeightPenaltyType(GItem()->GetWeightPenaltyGradeType());

	FText weightPercentText;
	FGsItemHelper::GetWeightPercentText(InWeightPercent, weightPercentText);

	_normalWeightText->SetText(weightPercentText);
	_panaltyOneWeightText->SetText(weightPercentText);
	_panaltyTwoWeightText->SetText(weightPercentText);
}

void UGsUIPopupInventory::SelectSortItemList(ESortUIList InListType , bool InNeedRefreshAll)
{
	if (false == _invenMgrPtr.IsValid())
		return;

	TArray<ItemDBId> itemList = _invenMgrPtr->GetDisplayItemList(EInventoryTab::AllTab);
	UGsItemManager* itemMgr = GItem();
	switch (InListType)
	{
	case ESortUIList::DefaultType:
		itemMgr->SortItemListByDBID(ESortUIList::DefaultType, itemList);
		break;
	case ESortUIList::GradeType:
		itemMgr->SortItemListByDBID(ESortUIList::GradeType, itemList);
		break;
	case ESortUIList::WeightType:
		itemMgr->SortItemListByDBID(ESortUIList::WeightType, itemList);
		break;
	}

	if (true == InNeedRefreshAll)
	{
		RefreshRealIconSlot();
		itemMgr->TrySendInvenSlotIndex(itemList,true);
	}
}

void UGsUIPopupInventory::SortServerIndex()
{
	if (_invenMgrPtr.IsValid())
	{
		GItem()->SortItemListByDBID(ESortUIList::ServerSlotType, _diplayItemList);
	}
}

void UGsUIPopupInventory::InvalidateItemList()
{
	RefreshRealIconSlot();
}

// 비어있는 아이콘 슬롯은 제외하고 실제 아이템정보가 있는 아이콘슬롯만
// 갱신함
void UGsUIPopupInventory::RefreshRealIconSlot()
{
	for (int32 i = 0; i < _diplayItemList.Num(); ++i)
	{
		_scrollHelper->RefreshByIndex(i);
	}
}

void UGsUIPopupInventory::OnClickDefaultSortList()
{
	ResetIconSelection();
	SelectSortItemList(ESortUIList::DefaultType, true);
	CloseSortItemListActive();
}

void UGsUIPopupInventory::OnClickGradeSortList()
{
	ResetIconSelection();
	SelectSortItemList(ESortUIList::GradeType, true);
	CloseSortItemListActive();
}

void UGsUIPopupInventory::OnClickWeightSortList()
{
	ResetIconSelection();
	SelectSortItemList(ESortUIList::WeightType, true);
	CloseSortItemListActive();
}

void UGsUIPopupInventory::OnClickSortList()
{
	ESlateVisibility visibilityType = _sortItemList->GetVisibility();
	_sortItemList->SetVisibility(ESlateVisibility::Visible == visibilityType ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UGsUIPopupInventory::OnSlotClickedTab(int32 InIndex, bool bInIsSelected)
{
	_scrollBox->ScrollToStart();
	_scrollBox->EndInertialScrolling();
}

// 0: AllItem  1: Equipment  2: Consumable  3: etc
void UGsUIPopupInventory::OnSelectChangedTab(int32 InIndex)
{
	if (_invenTabIndex == InIndex)
		return;

	ResetIconSelection();
	_invenTabIndex = InIndex;
	_invenMgrPtr->OnChangeInvenTab((EInventoryTab)_invenTabIndex);

	//ResetIconSelection();
	CloseSortItemListActive();	// 정렬 툴팁 닫기	

	// 열려있는 상세정보 닫기
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_CLOSE, nullptr);

	GSQuickSlot()->SetInvenSelectionItem(_selectionItemDbid);
}

void UGsUIPopupInventory::CheckRedDot()
{
	UGsItemManager* itemMgr = GItem();
	bool equip = itemMgr->CheckRedDotItemByItemType(ItemType::EQUIP);
	bool consume = itemMgr->CheckRedDotItemByItemType(ItemType::CONSUME);
	bool ingredient = itemMgr->CheckRedDotItemByItemType(ItemType::INGREDIENT) || itemMgr->CheckRedDotItemByItemType(ItemType::CURRENCY);

	SetInvenTabRedDot(EInventoryTab::AllTab, (equip || consume || ingredient));
	SetInvenTabRedDot(EInventoryTab::EquipmentTab, equip);
	SetInvenTabRedDot(EInventoryTab::ConsumableTab, consume);
	SetInvenTabRedDot(EInventoryTab::MaterialTab, ingredient);

	GMessage()->GetHudRedDot().SendMessage(MessageContentHUDRedDot::INVEN, (equip || consume || ingredient));
}

void UGsUIPopupInventory::CheckOwnedItemWarnText()
{
	//GSLOG(Error, TEXT("[CheckOwnedItemWarnText] - _diplayItemList.Num() : %d "), _diplayItemList.Num());
	_emptyItemListWarnRoot->SetVisibility(0 < _diplayItemList.Num() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);
}

void UGsUIPopupInventory::OnClickClose()
{
	Close();
}

void UGsUIPopupInventory::InvalidateSlotCount()
{
	UGsItemManager* itemMgr = GItem();
	// 수량 텍스트 및 색상 설정
	FText slotCountText;
	FGsUIStringHelper::GetTwoValueSlashedText(itemMgr->GetOwnedItemCount(), _invenMgrPtr->GetInvenSlotMaxCount(), slotCountText);
	EGsUIColorType colorType = (itemMgr->GetOwnedItemCount() > _invenMgrPtr->GetInvenSlotMaxCount()) ?
		EGsUIColorType::DEFAULT_LACK : EGsUIColorType::DEFAULT_ENOUGH;

	_slotCountText->SetText(slotCountText);
	_slotCountText->SetColorAndOpacity(FGsUIColorHelper::GetColor(colorType));

	// 수량 체크 후 티커 출력
	//invenMgr->CheckItemSlotCount();

	// 아이템 수량이 0개 일때 미보유 텍스트 출력
	CheckOwnedItemWarnText();
}

void UGsUIPopupInventory::InvalidateExpandButton()
{
	uint8 enoughCount = _invenMgrPtr->GetEnoughExtendSlotCount();
	_btnItemSlotLock->SetVisibility(0 >= enoughCount ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

bool UGsUIPopupInventory::IsMatchedCurTab(const EInventoryTab InCurTabType, const ItemType InItemType)
{
	switch (InCurTabType)
	{
	case EInventoryTab::AllTab:
		return true;
	case EInventoryTab::EquipmentTab:
		return ItemType::EQUIP == InItemType;
	case EInventoryTab::ConsumableTab:
		return ItemType::CONSUME == InItemType;
	case EInventoryTab::MaterialTab:
		return (ItemType::INGREDIENT == InItemType || ItemType::CURRENCY == InItemType || ItemType::ETC == InItemType);
	}
	return false;
}

// 신규로 추가되는 아이템은 무조건 맨뒤에 add 된다고 가정
void UGsUIPopupInventory::AddItemList(TArray<TWeakPtr<FGsItem>> InAddItemList)
{
	for (TWeakPtr<FGsItem> iter : InAddItemList)
	{
		if (false == iter.IsValid())
			continue;

		bool isAddItem = false;
		EInventoryTab tabType = ConvertTabTypeByIndex(_invenTabIndex);
		if (IsMatchedCurTab(tabType, iter.Pin()->GetType()))
		{
			isAddItem = true;
		}
		if (isAddItem)
		{
			if (false == _diplayItemList.Contains(iter.Pin()->GetDBID()))
			{
				_diplayItemList.Emplace(iter.Pin()->GetDBID());
				// itemicon 모두 로드된 상태
				// nullptr 이라면 로드후에 알아서 갱신 처리함
				if (nullptr != _scrollHelper->GetIcon(_diplayItemList.Num() - 1))
				{
					_scrollHelper->RefreshByIndex(_diplayItemList.Num() - 1);
				}
			}
			else
			{
#if WITH_EDITOR
				GSLOG(Warning, TEXT("[UGsUIPopupInventory::AddItem] InItem is Already Exist Item !!! "));
#endif
			}
		}
	}

	InvalidateSlotCount();
}

void UGsUIPopupInventory::UpdateItemList(const TArray<ItemDBId>& InItemDBIdList)
{
	CheckRedDot();

	for (ItemDBId updatedItemDBId : InItemDBIdList)
	{
		int32 index = _diplayItemList.Find(updatedItemDBId);
		if (INDEX_NONE == index)
		{
			continue;
		}

		_scrollHelper->RefreshByIndex(index);
	}
}

void UGsUIPopupInventory::RemoveItemList(const TArray<ItemDBId>& InItemDBIdList)
{
	bool refreshInventory = false;
	for (ItemDBId itemDBId : InItemDBIdList)
	{
		if (_diplayItemList.Contains(itemDBId))
		{
			int32 index = _diplayItemList.Find(itemDBId);
			if (INDEX_NONE != index)
			{
				if (_selectionItemDbid == itemDBId)
				{
					ResetIconSelection();
				}
				_diplayItemList.Remove(itemDBId);
				refreshInventory = true;
			}
		}
	}

	if (refreshInventory)
	{
		_invenTotalSlotCount = _invenMgrPtr->GetInvenSlotMaxCountWithTemp();
		_scrollHelper->RefreshAll(_invenTotalSlotCount);
	}
	
	InvalidateSlotCount();
}

void UGsUIPopupInventory::OnChangeSlotIndex()
{
	GItem()->SortItemListByDBID(ESortUIList::ServerSlotType, _diplayItemList);
	RefreshRealIconSlot();
}

void UGsUIPopupInventory::CreateItemList(TArray<ItemDBId> InItemList, bool InIconDimmedState)
{
	_isDimmedState = InIconDimmedState;

	_diplayItemList.Empty();
	_diplayItemList = MoveTemp(InItemList);

	_invenTotalSlotCount = _invenMgrPtr->GetInvenSlotMaxCountWithTemp();
	_createMaxSlotCountByTick = _invenTotalSlotCount;

	InvalidateSlotCount();
	CheckRedDot();
}

void UGsUIPopupInventory::OnChangeTab(EInventoryTab InTabType, bool InIsNeedInvalidate)
{
	_invenTabIndex = (uint8)InTabType;
	if (InIsNeedInvalidate)
	{
		_toggleGroup.SetSelectedIndex(_invenTabIndex);
	}
	else
	{
		_toggleGroup.SetSelectedIndexNoEvent(_invenTabIndex);
	}
}

// 한번에 많은양의 icon 을 그리면서 부하가 생기는 이슈가 발생하여
// tick 마다 분할 로드 하는 방식
void UGsUIPopupInventory::CreateIconSlot(float InDeltaTime)
{
	if (0 < _createMaxSlotCountByTick)
	{
		if (InDeltaTime > _invenMgrPtr->FRAME_RATE) // 저사양
		{
			// 틱당 생성하는 슬롯 갯수를 줄인다
			if (_createSlotCountEveryTick > _invenMgrPtr->CREATE_MIN_COUNT_BY_TICK)
			{
				// 틱당 생성하는 슬롯 갯수가 최소 갯수보다 적으면 최소 갯수로 설정함
				_createSlotCountEveryTick = _invenMgrPtr->CREATE_MIN_COUNT_BY_TICK >= (_createSlotCountEveryTick - _invenMgrPtr->BALANCE_COUNT) ?
					_invenMgrPtr->CREATE_MIN_COUNT_BY_TICK : _createSlotCountEveryTick - _invenMgrPtr->BALANCE_COUNT;
			}
		}
		else // 고사양
		{
			// 틱당 생성하는 슬롯 늘린다
			if (_createSlotCountEveryTick < _invenMgrPtr->CREATE_MAX_COUNT_BY_TICK)
			{
				// 틱당 생성하는 슬롯 갯수가 최대 갯수보다 많으면 최대 갯수로 설정함
				_createSlotCountEveryTick = _invenMgrPtr->CREATE_MAX_COUNT_BY_TICK <= (_createSlotCountEveryTick + _invenMgrPtr->BALANCE_COUNT) ?
					_invenMgrPtr->CREATE_MAX_COUNT_BY_TICK : _createSlotCountEveryTick + _invenMgrPtr->BALANCE_COUNT;
			}
		}
		_scrollHelper->CreateIconByTick(_createSlotCountEveryTick, _invenTotalSlotCount);
		_scrollHelper->RefreshIconByTick(_createSlotCountEveryTick , (_invenTotalSlotCount - _createMaxSlotCountByTick));
		_createMaxSlotCountByTick -= _createSlotCountEveryTick;

		// 마지막 생성 틱 - 로드는 끝났상태
		if (0 >= _createMaxSlotCountByTick)
		{
			_createMaxSlotCountByTick = 0;

			// 특정 위치로 스크롤
			if (INVALID_ITEM_DB_ID != _tempScrollTargetItem)
			{
				ScrollToTargetItem(_tempScrollTargetItem);
				_tempScrollTargetItem = INVALID_ITEM_DB_ID;
			}
		}
	}
}

void UGsUIPopupInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	CreateIconSlot(InDeltaTime);

	/*
	float test = 3.0f;
	float aaa = 5.0f;
	// Test 
	// 3초에 한번씩 호출된다고 가정 (프레임 최악의 상태일때 라고 가정)
	aaa += InDeltaTime;
	if (aaa > test)
	{
		CreateIconSlot();
		aaa = 0.f;
	}
	*/

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIPopupInventory::BindMessage()
{
	// 이벤트 등록
	FGsMessageHolder* message = GMessage();

	// Window 가 열려 있는 상태에서 신규 가이드 퀘스트 해금 시 해금 상태를 바로 갱신해주기 위해서 (정렬 X)
	_listInvenDelegate.Emplace(
		message->GetInventoryEvent().AddUObject(MessageContentInven::UPDATE_INVEN_EXTEND_SCROLL, this, &UGsUIPopupInventory::OnInvenSlotExtended));
}

void UGsUIPopupInventory::UnBindMessage()
{
	FGsMessageHolder* MessageManager = GMessage();
	if (nullptr == MessageManager)
	{
		GSLOG(Error, TEXT("nullptr == MessageManager"));
		return;
	}

	if (_listInvenDelegate.Num() != 0)
	{
		for (auto iter : _listInvenDelegate)
		{
			MessageManager->GetInventoryEvent().Remove(iter);
		}

		_listInvenDelegate.Empty();
	}
}

void UGsUIPopupInventory::OnClickIconSlot(UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}
	if (itemIcon->IsDimmed())
	{
		return;
	}

	UGsQuickSlotManager* quickSlotManager = GSQuickSlot();
	if (nullptr == quickSlotManager)
		return;
	
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	FGsItem* item = itemIcon->GetItemIconData();
	if (nullptr == item)
	{
		ResetIconSelection();
		quickSlotManager->SetInvenSelectionItem(_selectionItemDbid);
		return;
	}
	
	CloseSortItemListActive();

	if (_selectionItemDbid == item->GetDBID())
	{
		if (_invenMgrPtr->IsUseLongPress())
		{
			// 아이템 사용 판단 로직
			bool isUseItem = true;

			switch (item->GetSubCategory())
			{
			case ItemCategorySub::SUMMON_FAIRY:
			case ItemCategorySub::SUMMON_COSTUME:
			{
				// 소환 상품이면 retry 정보 여기서 세팅한다
				if (FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler())
				{
					// 퀵슬롯 1, 2번에 등록한뒤(1: 페어리, 2: 코스튬)
					// 1, 2 연속해서 누르면
					// 1번(페어리) 성공, 2번(코스튬) 실패 
					// 하지만 다시뽑기 데이터는 2번이 저장되어서
					// 두번째는 코스튬뽑게됨
					// 유효하지 않으면 저장 안하게 처리
					//	https://jira.com2us.com/jira/browse/C2URWQ-5204	
					summonHandler->SetRetryDataByItemInfo(item);
				}
				break;
			}
			case ItemCategorySub::REWARD_BOX:
			{
				bool isPickOneType = FGsItemHelper::IsPickOneTypeRewardItem(item);
				if (isPickOneType)
				{
					// 선택형 보상 상자는 아이템을 사용하지 않고 아이템 선택 팝업을 출력한다.
					isUseItem = false;
					if (itemManager->TryUseItem(item, 0, true))
					{
						// 수량이 하나뿐이면 단일 선택 팝업 출력
						if (item->GetAmount() == 1)
						{
							int32 rewardId = item->GetPickOneTypeRewardID();
							struct FGsOpenPickOneBox paramData(item->GetDBID(), rewardId, false);
							GMessage()->GetItemContents().SendMessage(MessageContentItem::PICKONEBOX_OPEN, &paramData);
						}
						// 수량이 여러개일땐 일괄 선택 팝업 출력
						else if (item->GetAmount() > 1)
						{
							int32 rewardId = item->GetPickOneTypeRewardID();
							struct FGsOpenPickOneBox paramData(item->GetDBID(), rewardId, true);
							GMessage()->GetItemContents().SendMessage(MessageContentItem::PICKONEBOX_OPEN, &paramData);
						}
						else
						{
							GSLOG(Error, TEXT("ItemId : %d, ItemDBId : %d, Item Amount is Zero"), item->GetTID(), item->GetDBID());
						}
					}
				}
				else
				{
					// 수량이 하나뿐이면 바로 사용한다.
					if (item->GetAmount() == 1)
					{
						// 별도 처리 없음.
					}
					// 수량이 여러개일땐 계산기 팝업 출력
					else if (item->GetAmount() > 1)
					{
						isUseItem = false;

						if (itemManager->TryUseItem(item, 0, true))
						{
							TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCalculator"));
							if (widget.IsValid())
							{
								if (UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(widget.Get()))
								{
									uint64 itemDBID = item->GetDBID();
									FText findText;
									FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Button_UseAll"), findText);
									popup->SetDimmed(true);
									popup->SetTitleText(findText);
									popup->SetDefaultCount(item->GetAmount());
									popup->SetMinimumCount(1);
									popup->SetMaximumCount(item->GetAmount());
									popup->InvalidateNumText();
									popup->SetButtonCallBack([&, this, popup, itemDBID](bool state)
										{
											if (state)
											{
												if (0 >= itemDBID || 0 >= popup->GetCurrentNum())
													return;

												GItem()->TryUseItem(itemDBID, popup->GetCurrentNum());
											}
										});
								}
							}
						}
					}
					else
					{
						GSLOG(Error, TEXT("ItemId : %d, ItemDBId : %d, Item Amount is Zero"), item->GetTID(), item->GetDBID());
					}
				}
				break;
			}
			}

			if (isUseItem)
			{
				itemManager->TryUseItem(item);
				ResetIconSelection();
			}
		}
	}
	else
	{
		ResetIconSelection();
		_selectionItemDbid = item->GetDBID();
		itemIcon->SetSlotSelectionImage(true);
		CheckRedDot();
	}	

	// 튜토리얼을 위해 클릭이나 롱프레스나 전부 상세창을 띄우도록 처리
	if (false == _invenMgrPtr->IsUseLongPress() || IsTutorial())
	{
		FGsItemDetailOpenMessageParamEX::DetailOption option;
		option._bHideBG = true;
		option._bUseCollectionMarking = true;

		FGsItemDetailOpenMessageParamEX param(item, FGsItemDetailOpenMessageParamEX::PosDir::RightPos, option);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);
	}
	else
	{
		UpdateSlotState(itemIcon);
	}

	if (IsTutorialInteraction())
	{
		OnTutorialInteraction.ExecuteIfBound();
	}

	quickSlotManager->SetInvenSelectionItem(_selectionItemDbid);
}

void UGsUIPopupInventory::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(&InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}
	FGsItem* item = itemIcon->GetItemIconData();
	if (nullptr == item)
	{
		ResetIconSelection();
		GSQuickSlot()->SetInvenSelectionItem(_selectionItemDbid);
		return;
	}

	if (_selectionItemDbid != item->GetDBID())
	{
		ResetIconSelection();
	}
	CloseSortItemListActive();

	_selectionItemDbid = item->GetDBID();
	itemIcon->SetSlotSelectionImage(true);
	CheckRedDot();

	FGsItemDetailOpenMessageParamEX::DetailOption option;
	option._bHideBG = true;
	option._bUseCollectionMarking = true;

	FGsItemDetailOpenMessageParamEX param(InIcon.GetItemIconData(), FGsItemDetailOpenMessageParamEX::PosDir::RightPos, option);
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM_EX, &param);

	//FGsItemDetailOpenMessageParam param(InIcon.GetItemIconData(), false, true, UGsUIPopupItemDetailMain::CenterPosType::RightPos);
	//GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);

	// 튜토리얼을 위해 클릭이나 롱프레스나 전부 상세창을 띄우도록 처리
	if (IsTutorial())
	{
		if (IsTutorialInteraction())
		{
			OnTutorialInteraction.ExecuteIfBound();
		}
	}

	GSQuickSlot()->SetInvenSelectionItem(_selectionItemDbid);
}

void UGsUIPopupInventory::ResetIconSelection()
{	
	int32 index = _diplayItemList.Find(_selectionItemDbid);
	if (INDEX_NONE != index && _scrollHelper->GetIcon(index))
	{
		UGsUIIconItemInventory* itemIconInven = Cast<UGsUIIconItemInventory>(_scrollHelper->GetIcon(index));
		if (itemIconInven)
		{
			itemIconInven->ResetSlotAction();
			itemIconInven->SetSlotSelectionImage(false);
		}
	}
	_selectionItemDbid = 0;
}

EInventoryTab UGsUIPopupInventory::ConvertTabTypeByIndex(int32 InIndex)
{
	switch (InIndex)
	{
	case 0:
		return EInventoryTab::AllTab;
		break;
	case 1:
		return EInventoryTab::EquipmentTab;
		break;
	case 2:
		return EInventoryTab::ConsumableTab;
		break;
	case 3:
		return EInventoryTab::MaterialTab;
		break;


	default:
		return EInventoryTab::AllTab;
	}
}

// 리니지2M 인벤토리 방식
// OnClick 들어왔을때, 
// 장비 아이템이라면 장착/해지 프로세스
// consumable 이라면 사용
// 이외는 그냥 selection 해지
void UGsUIPopupInventory::UpdateSlotState(UGsUIIconItemInventory* InItemIcon)
{	
	if (0 >= _selectionItemDbid)
		return;

	if (InItemIcon)
	{
		const FGsItem* itemData = InItemIcon->GetItemIconData();
		if (nullptr == itemData)
			return;

		UGsItemManager* itemMgr = GItem();
		UGsUIIconItemInventory::SlotActionType type = UGsUIIconItemInventory::SlotActionType::None;
		bool isUseItem = (true == itemData->IsUseItem() && false == itemData->IsEquipment());
		bool isEquipped = (true == itemData->IsEquipment()) && (false == itemMgr->IsCurrentPresetItem(itemData) && false == itemData->IsCollectableType());
		bool isUnEquipped = (true == itemData->IsEquipment()) && (true == itemMgr->IsCurrentPresetItem(itemData) && false == itemData->IsCollectableType());

		InItemIcon->SetSlotUseImage(isUseItem);
		InItemIcon->SetSlotEquipImage(isEquipped);
		InItemIcon->SetSlotUnEquipImage(isUnEquipped);

		if (true == isUseItem)
		{
			type = UGsUIIconItemInventory::SlotActionType::Use;
		}
		if (true == isEquipped)
		{
			type = UGsUIIconItemInventory::SlotActionType::Equip;
		}
		if (true == isUnEquipped)
		{
			type = UGsUIIconItemInventory::SlotActionType::UnEquip;
		}
		InItemIcon->SetSlotActionType(type);
	}
}

void UGsUIPopupInventory::SetInvenTabRedDot(EInventoryTab InTab, bool InActiveState)
{
	if (true == _redDotImageList.Contains(InTab))
	{
		if (_redDotImageList[InTab].IsValid())
		{
			ESlateVisibility slateType = true == InActiveState ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden;
			_redDotImageList[InTab]->SetVisibility(slateType);
		}
	}
}

void UGsUIPopupInventory::OnRefreshIconWrapBox(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	if (false == _diplayItemList.IsValidIndex(InIndex))
	{
		itemIcon->SetEmptyIcon();
		itemIcon->SetItemNameToolTipFlag(false);
		return;
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIPopupInventory::OnLongPressIcon);
	}
	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIPopupInventory::OnClickIconSlot);
	}

	itemIcon->SetOwnedItem(_diplayItemList[InIndex]);
	itemIcon->SetActiveIconBg(true);
	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetDimmedImage(((_isDimmedState) && (itemIcon->GetItemIconData()) && (_invenMgrPtr->IsReactionLockTypeItem(itemIcon->GetItemIconData()->GetSubCategory()))));
	itemIcon->SetOnClickLockSec(0.1f);
	itemIcon->SetSlotSelectionImage(_diplayItemList[InIndex] == _selectionItemDbid);
#if WITH_EDITOR || PLATFORM_WINDOWS	
	itemIcon->SetItemNameToolTipFlag(true);
	itemIcon->SetItemNameToolTipHoverMode(true);
#else
	itemIcon->SetItemNameToolTipFlag(_invenMgrPtr->IsUseLongPress());
#endif
	itemIcon->SetOnChangeDisplayDetailWndType(_invenMgrPtr->IsUseLongPress() ?
		UGsUIIconItem::UseDisplayDetailWndType::LongPressType : UGsUIIconItem::UseDisplayDetailWndType::OnClickType);
	itemIcon->SetItemCollectionMarking(itemIcon->GetItemIconData(), true);

	if (IsTutorial())
	{
		if (0 == InIndex &&
			_tutorialItemDBId == _diplayItemList[InIndex])
		{
			OnTutorialInventoryListEvent.ExecuteIfBound(InIcon);
		}
	}
}

void UGsUIPopupInventory::CloseSortItemListActive()
{
	_sortItemList->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIPopupInventory::OnChangeInvenSlotCount(uint32 InMaxSlotCount, uint32 InTotalExtendCount)
{
	if (_invenTotalSlotCount > InMaxSlotCount) // 확장해도 임시슬롯이 더 많음
	{
	}
	else
	{
		uint32 calc = InMaxSlotCount - _invenTotalSlotCount;
		_scrollHelper->CreateIcon(InMaxSlotCount);
		for (uint32 i = 0; i < calc; ++i)
		{
			_scrollHelper->RefreshByIndex(_invenTotalSlotCount + i);
		}
		_invenTotalSlotCount = InMaxSlotCount;
	}
	InvalidateSlotCount();
	InvalidateExpandButton();
}

void UGsUIPopupInventory::ScrollToTargetItem(const ItemDBId InItemDBId)
{
	// 튜토리얼 중엔 스크롤 복구기능 무시
	if (IsTutorial())
	{
		return;
	}

	for (int i = 0; i < _diplayItemList.Num(); ++i)
	{
		if (InItemDBId == _diplayItemList[i])
		{
			if (UGsUIIconBase* icon = _scrollHelper->GetIcon(i))
			{
				_scrollBox->ScrollWidgetIntoView(icon);
			}
			else
			{
				_tempScrollTargetItem = InItemDBId;
			}
		}
	}
}

void UGsUIPopupInventory::OnClickSlotLock()
{
	uint8 enoughCount = _invenMgrPtr->GetEnoughExtendSlotCount();
	if (0 >= enoughCount)
	{
		// 이미 다 뚫었을 경우, 티커 메시지 출력?
		// InvalidateExpandButton 를 보면 이미 Lock 버튼을 숨기고 있음 (버튼이 있으면 에러)
		GSLOG(Error, TEXT("[PopupInventory] OnClickSlotLock should be hidden!"));
		return;
	}

	if (UGsUIPopupCommonPayInventory* popupCommonPayInventory =
		Cast<UGsUIPopupCommonPayInventory>(GUI()->OpenAndGetWidget(TEXT("PopupCommonPayInventory"))))
	{

		FText findText;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_ExtendSlot_Check"), findText);

		Level pastCount = _invenMgrPtr->GetInvenTotalExtendCount();

		popupCommonPayInventory->SetData(findText, pastCount, enoughCount, false,
			[](bool InIsOk, bool InIsEnough, Level InExtendLevel)
			{
				if (!InIsOk)
					return;

				// 재화 충분할 경우, 확장 (선택한 레벨만큼)
				if (InIsEnough)
				{
					if (UGsInventoryManager* inventoryManager = GSInventory())
					{
						inventoryManager->SendExtendSlot(InExtendLevel);
					}
					return;
				}

				// 재화 모자를 경우 ticker 출력
				FText Text;
				FText::FindText(TEXT("DepotText"), TEXT("Alert_NoCost"), Text);
				FGsUIHelper::TrayMessageTicker(Text);
			}
		);

	}
}

void UGsUIPopupInventory::SetItemDimmed(const bool InIsLock, TArray<ItemCategorySub>& InDimmedTypeList)
{
	for (int32 i = 0; i < _diplayItemList.Num(); ++i)
	{
		TWeakObjectPtr<UWidget> icon = _iconSelector->GetChildAt(i);
		if (icon.IsValid())
		{
			UGsUIIconItemInventory* inventoryIcon = Cast<UGsUIIconItemInventory>(icon);
			if (nullptr == inventoryIcon)
				continue;

			FGsItem* iconItemData = inventoryIcon->GetItemIconData();
			if (nullptr == iconItemData)
				continue;

			inventoryIcon->SetDimmedImage((InIsLock) && (INDEX_NONE != InDimmedTypeList.Find(iconItemData->GetSubCategory())));
		}
	}
}

void UGsUIPopupInventory::OnScrollList(float InCurrentOffset)
{
	// 튜토리얼 스크롤 락
	if (IsTutorial())
	{
		_scrollBox->SetScrollOffset(0.f);
	}
}

void UGsUIPopupInventory::SetTutorialItemDBId(uint64 InItemDBId)
{
	_tutorialItemDBId = InItemDBId;

	if (0 == _tutorialItemDBId)
	{
		_scrollBox->SetAllowOverscroll(true);
		OnTutorialInventoryListEvent.Unbind();
	}
	else
	{
		// 강제로 맨 앞으로 위치
		_diplayItemList.Remove(_tutorialItemDBId);
		_diplayItemList.Insert(_tutorialItemDBId, 0);

		_scrollBox->SetAllowOverscroll(false);
		_scrollBox->ScrollToStart();
		_scrollHelper->RefreshAll(_invenTotalSlotCount);
	}
}

void UGsUIPopupInventory::OnInvenSlotExtended(const IGsMessageParam* InParam)
{
	_scrollBox->ScrollToEnd();
	_scrollBox->EndInertialScrolling();
}

void UGsUIPopupInventory::OnInputCancel()
{
	OnClickClose();
}

void UGsUIPopupInventory::OnClickDecompose()
{
	GMessage()->GetItemContents().SendMessage(MessageContentItem::DECOMPOSE_MULTI, nullptr);
}