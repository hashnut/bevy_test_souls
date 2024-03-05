// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupItemDecompose.h"
#include "GsUIIconItemDecomposeEntry.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Popup/ItemDecompose/GsUIIconItemDecomposeEntry.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Popup/GsUIPopupCalculator.h"

#include "Item/GsItem.h"
#include "Item/GsItemManager.h"

#include "Net/GsNetSendServiceItem.h"

#include "Message/MessageParam/GsItemMessageParam.h"

#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"

#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "T1Project/UI/UIContent/Helper/GsUIHelper.h"



void UGsUIPopupItemDecompose::BeginDestroy()
{
	if (nullptr != _panelSlotHelper)
	{
		_panelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupItemDecompose::ItemListRefreshEntry);
		_panelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIPopupItemDecompose::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnGradeNormal->OnClicked.AddDynamic(this, &UGsUIPopupItemDecompose::OnClickNormalGrade);
	_btnGradeMagic->OnClicked.AddDynamic(this, &UGsUIPopupItemDecompose::OnClickMagicGrade);
	_btnGradeRare->OnClicked.AddDynamic(this, &UGsUIPopupItemDecompose::OnClickRareGrade);

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupItemDecompose::OnClickClose);
	_btnConfirm->OnClicked.AddDynamic(this, &UGsUIPopupItemDecompose::OnClickConfirm);

	_toggleGroup.Clear();
	_toggleGroup.OnSlotClicked.BindUObject(this, &UGsUIPopupItemDecompose::OnSlotClickedTab);
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupItemDecompose::OnSelectChangedTab);
	_toggleGroup.AddToggleWidget(_equipDecompSwitcherBtn);
	_toggleGroup.AddToggleWidget(_skillbookDecompSwitcherBtn);
	_toggleGroup.AddToggleWidget(_slateDecompSwitcherBtn);

	if (nullptr == _panelSlotHelper)
	{
		_panelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_panelSlotHelper->Initialize(_entryWidgetClass, _verticalBox);
		_panelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupItemDecompose::ItemListRefreshEntry);
	}	
}

void UGsUIPopupItemDecompose::NativeConstruct()
{
	Super::NativeConstruct();

	SetGradeSwitcher(false, QuickSelectGradeType::Normal);
	SetGradeSwitcher(false, QuickSelectGradeType::Magic);
	SetGradeSwitcher(false, QuickSelectGradeType::Rare);

	for (uint8 i = 0; i < QuickSelectGradeType::Max; ++i)
	{
		_quickSelectGradeFlagList.Emplace(false);
	}

	_currentTabIndex = 0;
	_toggleGroup.SetSelectedIndex(_currentTabIndex);

	InitMsg();
}

void UGsUIPopupItemDecompose::NativeDestruct()
{
	_quickSelectGradeFlagList.Empty();
	_selectTargetItemAmountPairList.Empty();
	_targetItemMap.Empty();

	RemoveMsg();

	Super::NativeDestruct();
}

void UGsUIPopupItemDecompose::InitMsg()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr != msg)
	{
		_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::UPDATEITEMLIST, this, &UGsUIPopupItemDecompose::OnUpdateItemList));
		_listItemDelegate.Emplace(msg->GetItem().AddUObject(MessageItem::REMOVEITEMS, this, &UGsUIPopupItemDecompose::OnRemoveItemList));
	}
}

void UGsUIPopupItemDecompose::RemoveMsg()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr != msg)
	{
		for (TPair<MessageItem, FDelegateHandle>& elem : _listItemDelegate)
		{
			msg->GetItem().Remove(elem);
		}
		_listItemDelegate.Empty();
	}
}

void UGsUIPopupItemDecompose::OnUpdateItemList(struct FGsItemMessageParamBase& InParam)
{
	UGsItemManager* itemManager = GItem();
	if (nullptr == itemManager)
		return;

	const FGsUpdateItemList* Param = StaticCast<const FGsUpdateItemList*>(&InParam);
	if (nullptr == Param)
		return;

	if (ItemStorageType::INVENTORY != Param->_itemStorageType)
		return;

	for (ItemDBId itemDBId : Param->_itemDbidList)
	{
		TWeakPtr<FGsItem> findItem = itemManager->FindItem(itemDBId);
		if (false == findItem.IsValid())
			continue;

		if (false == _targetItemMap.Contains(findItem.Pin()->GetType()))
			continue;
		
		const TArray<ItemDBId>& dbidList = _targetItemMap[findItem.Pin()->GetType()];
		if (dbidList.Contains(findItem.Pin()->GetDBID()))
		{
			// 아이템 목록의 데이터가 변경된 이후이므로 한번만 갱신하면 충분하다.
			OnSelectChangedTab(_toggleGroup.GetSelectedIndex());
			return;
		}
	}
}

void UGsUIPopupItemDecompose::OnRemoveItemList(struct FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItemList* Param = StaticCast<const FGsRemoveItemList*>(&InParam);
	if (nullptr != Param)
	{
		for (ItemDBId itemDBId : Param->_itemDBIdList)
		{
			for (TPair<ItemType, TArray<ItemDBId>>& iter : _targetItemMap)
			{
				TArray<ItemDBId>& dbidList = iter.Value;
				dbidList.Remove(itemDBId);
			}
		}
	}

	OnSelectChangedTab(_toggleGroup.GetSelectedIndex());
}

void UGsUIPopupItemDecompose::OnSlotClickedTab(int32 InIndex, bool bInIsSelected)
{
	_scrollBox->ScrollToStart();
	_scrollBox->EndInertialScrolling();
}

void UGsUIPopupItemDecompose::OnSelectChangedTab(int32 InIndex)
{
	// 0 ItemType::EQUIP
	// 1 ItemType::CONSUME
	// 2 ItemType::INGREDIENT
	_currentTabIndex = InIndex;

	ResetData();
	
	ItemType targetItemType = GetItemTypeByIndex(_currentTabIndex);
	if (_targetItemMap.Contains(targetItemType))
	{
		_panelSlotHelper->RefreshAll(_targetItemMap[targetItemType].Num());
		_emptyListWarnRoot->SetVisibility(0 >= _targetItemMap[targetItemType].Num() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
	}
}

void UGsUIPopupItemDecompose::SetMultiDecomposeItemList(TMap<ItemType, TArray<ItemDBId>> InItemMap)
{
	_targetItemMap.Empty();
	for (TPair<ItemType, TArray<ItemDBId>>& iter : InItemMap)
	{
		_targetItemMap.Emplace(iter.Key, iter.Value);
	}

	OnSelectChangedTab(_toggleGroup.GetSelectedIndex());
}

void UGsUIPopupItemDecompose::OnClickClose()
{
	Close();
}

void UGsUIPopupItemDecompose::OnClickConfirm()
{
	FGsUnlockManager* unlockManager = GSUnlock();
	if (nullptr == unlockManager)
		return;

	if (false == unlockManager->IsContentsUnlock(EGsUnlockContentsType::DECOMPOSE, true))
	{
		OnClickClose();
		return;
	}

	if (0 >= _selectTargetItemAmountPairList.Num())
	{
		// 선택된 아이템이 없습니다.
		FText findText;
		FText::FindText(TEXT("DecomposeText"), TEXT("Notice_ChoiceItem"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	UGsItemManager* itemMgr = GItem();
	for (ItemDBIdAmountPair iter : _selectTargetItemAmountPairList)
	{
		// 분해 아이템들 중, 하나라도 유효하지 않는 아이템이면 아무것도 하지 않고
		// 창을 닫아버림 - 재오픈 유도 함으로써 자동 갱신처리 로직 태움
		TWeakPtr<FGsItem> findItem = itemMgr->FindItem(iter.mItemDBId);
		if (false == findItem.IsValid())
		{
			// 분해할 수 없습니다
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Decompose_Notice_Cannot"), findText);
			FGsUIHelper::TrayMessageTicker(findText);

			OnClickClose();
			return;
		}

		if (findItem.Pin()->GetLock())
		{
			// 티커 출력 : 잠긴 아이템은 분해할 수 없습니다.
			FText findText;
			FText::FindText(TEXT("ItemUIText"), TEXT("ItemUi_Decompose_Notice_Locked"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
			return;
		}

		//GSLOG(Error, TEXT("[UGsUIPopupItemDecompose::OnClickConfirm] - GetName : %s"), *findItem.Pin()->GetName().ToString());
		//GSLOG(Error, TEXT("[UGsUIPopupItemDecompose::OnClickConfirm] - iter.mItemAmount: %d"), iter.mItemAmount);
	}

	// 확정 팝업 없이 바로 분해
	FGsNetSendServiceItem::SendItemDecompose(_selectTargetItemAmountPairList);
}

// 0 : on   1 : off
void UGsUIPopupItemDecompose::SetGradeSwitcher(const bool InFlag, const QuickSelectGradeType InType)
{
	switch (InType)
	{
	case QuickSelectGradeType::Normal:
		_gradeNoramlSwitcher->SetActiveWidgetIndex(!InFlag);
		break;
	case QuickSelectGradeType::Magic:
		_gradeMagicSwitcher->SetActiveWidgetIndex(!InFlag);
		break;
	case QuickSelectGradeType::Rare:
		_gradeRareSwitcher->SetActiveWidgetIndex(!InFlag);
		break;
	}
	
	if (false != _quickSelectGradeFlagList.IsValidIndex(InType))
	{
		_quickSelectGradeFlagList[InType] = InFlag;
	}
}

void UGsUIPopupItemDecompose::OnConfirmCalculator(uint64 InItemDBID, uint32 InInputNum, int32 InWidgetIndex, bool InCalculatorState)
{
	UWidget* widget = _panelSlotHelper->GetEntry(InWidgetIndex);
	if (nullptr == widget)
		return;

	UGsUIIconItemDecomposeEntry* entry = Cast<UGsUIIconItemDecomposeEntry>(widget);
	if (nullptr == entry)
		return;

	if (0 >= InInputNum || false == InCalculatorState)
	{
		entry->SetSelectionInfo(false);
		return;
	}

	entry->SetStackCount(InInputNum);

	ItemDBIdAmountPair pairData(InItemDBID, InInputNum);
	_selectTargetItemAmountPairList.Emplace(pairData);
}

void UGsUIPopupItemDecompose::ItemListRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUIIconItemDecomposeEntry* entry = Cast<UGsUIIconItemDecomposeEntry>(InEntry);
	if (nullptr == entry)
		return;

	ItemType targetItemType = GetItemTypeByIndex(_currentTabIndex);
	if (false == _targetItemMap.Contains(targetItemType))
		return;

	const TArray<ItemDBId>& dbidList = _targetItemMap[targetItemType];
	if (false == dbidList.IsValidIndex(InIndex))
		return;

	if (false == entry->OnChangeSelectDelegate.IsBoundToObject(this))
	{
		entry->OnChangeSelectDelegate.AddUObject(this, &UGsUIPopupItemDecompose::OnAutoSelectEntryWidget);
	}
	if (false == entry->OnChangeUserSelectDelegate.IsBoundToObject(this))
	{
		entry->OnChangeUserSelectDelegate.AddUObject(this, &UGsUIPopupItemDecompose::OnUserSelectEntryWidget);
	}
	entry->SetItemInfo(dbidList[InIndex], InIndex);
}

// 노멀 / 매직 / 레어 버튼을 눌러 한 번에 엔트리들을 선택 시도할 경우, 각 엔트리들의 동작
void UGsUIPopupItemDecompose::OnAutoSelectEntryWidget(const uint64 InSelectItemDBID, const bool InFlag, const int32 InIndex)
{
	if (false == FGsItem::IsValidItemByDBID(InSelectItemDBID))
		return;

	UWidget* widget = _panelSlotHelper->GetEntry(InIndex);
	if (nullptr == widget)
		return;

	UGsUIIconItemDecomposeEntry* entry = Cast<UGsUIIconItemDecomposeEntry>(widget);
	if (nullptr == entry)
		return;

	// 선택 -> 선택 해제
	if (false == InFlag)
	{
		for (int32 i = 0; i < _selectTargetItemAmountPairList.Num(); ++i)
		{
			if (InSelectItemDBID == _selectTargetItemAmountPairList[i].mItemDBId)
			{
				_selectTargetItemAmountPairList.RemoveAt(i);
				break;
			}
		}

		// 선택했던 엔트리를 강제로 다시 선택해제한다
		entry->SetSelectionInfo(false, false);
		entry->SetStackCount(0);
	}
	// 선택 (선택 상태에서 들어올 수도 있고, 선택 해제 상태에서 들어올 수도 있다)
	else
	{
		TWeakPtr<FGsItem> findData = GItem()->FindItem(InSelectItemDBID);
		if (false == findData.IsValid())
			return;

		entry->SetStackCount(findData.Pin()->GetAmount());

		// 이미 선택되었었다면 갱신
		bool isFound = false;
		for (int32 i = 0; i < _selectTargetItemAmountPairList.Num(); ++i)
		{
			if (InSelectItemDBID == _selectTargetItemAmountPairList[i].mItemDBId)
			{
				_selectTargetItemAmountPairList[i].mItemAmount = findData.Pin()->GetAmount();
				isFound = true;
				break;
			}
		}

		if (!isFound)
		{
			ItemDBIdAmountPair pairData(InSelectItemDBID, findData.Pin()->GetAmount());
			_selectTargetItemAmountPairList.Emplace(pairData);
		}
	}
}

void UGsUIPopupItemDecompose::OnUserSelectEntryWidget(const uint64 InSelectItemDBID, const bool InFlag, const int32 InIndex)
{
	if (false == FGsItem::IsValidItemByDBID(InSelectItemDBID))
		return;

	UWidget* widget = _panelSlotHelper->GetEntry(InIndex);
	if (nullptr == widget)
		return;

	UGsUIIconItemDecomposeEntry* entry = Cast<UGsUIIconItemDecomposeEntry>(widget);
	if (nullptr == entry)
		return;

	// 여기가 선택 해제 하는 부분임
	if (false == InFlag)
	{
		// 스택 여부 관계없이 리스트에서 제거해줌
		for (int32 i = 0; i < _selectTargetItemAmountPairList.Num(); ++i)
		{
			if (InSelectItemDBID == _selectTargetItemAmountPairList[i].mItemDBId)
			{
				_selectTargetItemAmountPairList.RemoveAt(i);
				break;
			}
		}

		entry->SetSelectionInfo(false, false);
		entry->SetStackCount(0);
	}
	// 선택 시도
	else
	{
		// 엔트리 갯수가 리미트를 초과했을 경우, 티커 띄워주고 리턴
		if (_selectTargetItemAmountPairList.Num() >= MAX_ITEM_PROCESS_LIMIT_COUNT)
		{
			OpenTrayTickerLimitCount();

			// 강제로 선택 해제 처리
			entry->SetSelectionInfo(false, false);
			entry->SetStackCount(0);
			return;
		}

		// stackable 이면 계산기 출력
		TWeakPtr<FGsItem> findData = GItem()->FindItem(InSelectItemDBID);
		if (false == findData.IsValid())
			return;
				
		// 스택가능한가
		if (findData.Pin()->IsStackable())
		{
			// 스택가능하며 갯수가 1개보다 많을 경우
			if (1 < findData.Pin()->GetAmount())
			{
				// 계산기 출력
				TWeakObjectPtr<UGsUIWidgetBase> calWidget = GUI()->OpenAndGetWidget(TEXT("PopupCalculator"));
				if (false == calWidget.IsValid())
					return;

				UGsUIPopupCalculator* popup = Cast<UGsUIPopupCalculator>(calWidget.Get());
				if (nullptr == popup)
					return;

				FText findText;
				FText::FindText(TEXT("DecomposeText"), TEXT("Calculator_Title"), findText);
				popup->SetDimmed(true);
				popup->SetTitleText(findText);
				popup->SetDefaultCount(0);
				popup->SetMinimumCount(0);
				popup->SetMaximumCount(findData.Pin()->GetAmount());
				popup->SetButtonCallBack([&, this, popup, InSelectItemDBID, InIndex](bool state)
					{
						// PairList 에 아이템 Pair 추가 및 엔트리 갯수 설정
						OnConfirmCalculator(InSelectItemDBID, popup->GetCurrentNum(), InIndex, state);
					});
			}
			// 갯수가 1개이면 계산기를 띄워주지 않고 갯수를 1개로 세팅한다
			else
			{				
				// 새로운 아이템 추가
				ItemDBIdAmountPair pairData(InSelectItemDBID, 1);
				_selectTargetItemAmountPairList.Emplace(pairData);

				// entry 의 갯수는 1개로 설정
				entry->SetStackCount(1);
			}
		}
		// 스택가능하지 않을 경우
		else
		{
			// 새로운 아이템 추가
			ItemDBIdAmountPair pairData(InSelectItemDBID, 1);
			_selectTargetItemAmountPairList.Emplace(pairData);
		}	
	}
}

// 노멀 / 매직 / 레어 버튼을 눌러 한 번에 엔트리들을 선택 시도
void UGsUIPopupItemDecompose::QuickSelectState(const QuickSelectGradeType InType, const bool InFlag)
{
	// FIX : 같은 역할을 하는 타입 2개를 둔 건 코드 중복이다
	ItemGrade convertGrade = ConvertByQuickSelectType(InType);
	if (ItemGrade::NONE == convertGrade)
		return;

	// 엔트리 리스트를 순회하면서 잔여 분해 도구 갯수를 갱신할 것이다.
	// 만약 잔여 도구 갯수가 부족하다면, 채울 수 있는 만큼까지만 갱신하고 멈춘다.
	for (int32 i = 0;i < _panelSlotHelper->GetChildrenCount(); ++i)
	{
		UWidget* widget = _panelSlotHelper->GetEntry(i);
		if (nullptr == widget)
			continue;
		
		UGsUIIconItemDecomposeEntry* entry = Cast<UGsUIIconItemDecomposeEntry>(widget);
		if (nullptr == entry)
			continue;

		if (convertGrade != entry->GetSlotItemGrade())
			continue;

		if (entry->GetSelectionInfo())
		{
		}
		// entry 를 선택하는 상황일 경우, 최대 분해 가능 종류를 체크해 주어야 한다
		else
		{
			// 만약 선택된 종류가 이미 리미트에 도달했을 경우, 티커 띄워주고 루프 중지
			if (_selectTargetItemAmountPairList.Num() >= MAX_ITEM_PROCESS_LIMIT_COUNT)
			{
				OpenTrayTickerLimitCount();
				break;
			}
		}

		// SetSelectionInfo 에서 OnChangeSelectDelegate 에 바인딩된 함수(OnAutoSelectEntryWidget)를 호출함
		entry->SetSelectionInfo(InFlag);
	}
}

ItemGrade UGsUIPopupItemDecompose::ConvertByQuickSelectType(const QuickSelectGradeType InType)
{
	switch (InType)
	{
	case QuickSelectGradeType::Normal:
		return ItemGrade::NORMAL;
	case  QuickSelectGradeType::Magic:
		return ItemGrade::MAGIC;
	case QuickSelectGradeType::Rare:
		return ItemGrade::RARE;
	}

	return ItemGrade::NONE;
}

void UGsUIPopupItemDecompose::OnClickGrade(QuickSelectGradeType InGradeType)
{
	if (false != _quickSelectGradeFlagList.IsValidIndex(InGradeType))
	{
		bool flag = _quickSelectGradeFlagList[InGradeType];

		_quickSelectGradeFlagList[InGradeType] = !flag;
		QuickSelectState(InGradeType, !flag);
		SetGradeSwitcher(!flag, InGradeType);
	}
}

ItemType UGsUIPopupItemDecompose::GetItemTypeByIndex(int32 InIndex)
{
	switch (InIndex)
	{
	case 0:
		return ItemType::EQUIP;
	case 1:
		return ItemType::CONSUME;
	case 2:
		return ItemType::INGREDIENT;
	}

	return ItemType::MAX;
}

void UGsUIPopupItemDecompose::OnClickNormalGrade()
{
	OnClickGrade(QuickSelectGradeType::Normal);
}

void UGsUIPopupItemDecompose::OnClickMagicGrade()
{
	OnClickGrade(QuickSelectGradeType::Magic);
}

void UGsUIPopupItemDecompose::OnClickRareGrade()
{
	OnClickGrade(QuickSelectGradeType::Rare);
}

void UGsUIPopupItemDecompose::ClosePopup()
{
	Close();
}

void UGsUIPopupItemDecompose::ResetData()
{
	_scrollBox->ScrollToStart();
	_scrollBox->EndInertialScrolling();

	SetGradeSwitcher(false, QuickSelectGradeType::Normal);
	SetGradeSwitcher(false, QuickSelectGradeType::Magic);
	SetGradeSwitcher(false, QuickSelectGradeType::Rare);

	QuickSelectState(QuickSelectGradeType::Normal, false);
	QuickSelectState(QuickSelectGradeType::Magic, false);
	QuickSelectState(QuickSelectGradeType::Rare, false);

	_selectTargetItemAmountPairList.Empty();
}

void UGsUIPopupItemDecompose::OpenTrayTickerLimitCount()
{
	// "최대 100개까지 선택 가능합니다."
	// 티커 출력 
	FText text;
	FText::FindText(TEXT("DecomposeText"), TEXT("Notice_MaxSelect"), text);
	FGsUIHelper::TrayMessageTicker(text);
}

void UGsUIPopupItemDecompose::OnInputCancel()
{
	Close();
}
