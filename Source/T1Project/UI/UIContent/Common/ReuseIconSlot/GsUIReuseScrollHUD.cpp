// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIReuseScrollHUD.h"

#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Widget.h"

#include "Input/GsInputEventMsgBase.h"
#include "Item/GsItemManager.h"
#include "DataCenter/Public/Option/GsOptionEnumType.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsQuickSlotManager.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Common/ReuseIconSlot/GsUIReuseItemIconSlotHud.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Message/MessageParam/GsMessageParam.h"


void UGsUIReuseScrollHUD::BeginDestroy()
{
	_rootPanelList.Empty();

	for (uint8 i = 0; i < _panelSlotHelperList.Num(); ++i)
	{
		if (nullptr != _panelSlotHelperList[i])
		{
			_panelSlotHelperList[i]->OnRefreshEntry.RemoveDynamic(this, &UGsUIReuseScrollHUD::OnRefreshEntry);
			_panelSlotHelperList[i]->OnCreateEntry.RemoveDynamic(this, &UGsUIReuseScrollHUD::OnCreateEntry);
			_panelSlotHelperList[i] = nullptr;
		}
	}

	Super::BeginDestroy();
}

void UGsUIReuseScrollHUD::UpdateManagerTick(float InDeltaTime)
{
	Super::UpdateManagerTick(InDeltaTime);


}

void UGsUIReuseScrollHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_rootPanelList.Empty();
	// 하단 스크롤
	_rootPanelList.Emplace(_firstPageRootPanel);
	_rootPanelList.Emplace(_secondPageRootPanel);
	_rootPanelList.Emplace(_thirdPageRootPanel);
	_rootPanelList.Emplace(_fourthPageRootPanel);
	_rootPanelList.Emplace(_fifthPageRootPanel);
	_rootPanelList.Emplace(_sixthPageRootPanel);

	// 상단 확장 스크롤
	_rootPanelList.Emplace(_firstPageRootPanelExpansion);
	_rootPanelList.Emplace(_secondPageRootPanelExpansion);
	_rootPanelList.Emplace(_thirdPageRootPanelExpansion);
	_rootPanelList.Emplace(_fourthPageRootPanelExpansion);
	_rootPanelList.Emplace(_fifthPageRootPanelExpansion);
	_rootPanelList.Emplace(_sixthPageRootPanelExpansion);
}

void UGsUIReuseScrollHUD::NativeConstruct()
{
	Super::NativeConstruct();

	_maxSlotCount = (uint8)_maxSlotCountType + 1;

	_panelSlotHelperList.SetNum(_rootPanelList.Num());

	CreateEntryReuseSlot();

	_slotIndex = 1;
	InvalidateAllSlot();

	InitMsg();
}

void UGsUIReuseScrollHUD::NativeDestruct()
{
	RemoveMsg();

	_panelSlotHelperList.Empty();

	Super::NativeDestruct();
}

void UGsUIReuseScrollHUD::InitMsg()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	_inputMsgHandlerList.Emplace(msg->GetWindowInput().AddUObject(MessageInput::Window_keyboard_Action,
		this, &UGsUIReuseScrollHUD::OnWindowKeyMapping));

	_listInputDelegate.Emplace(msg->GetInput().AddUObject(MessageInput::TouchRelease,
		this, &UGsUIReuseScrollHUD::OnTouchRelease));

	_listSystemDelegate.Emplace(msg->GetSystemParam().AddUObject(MessageSystem::TOUCH_SCREEN,
		this, &UGsUIReuseScrollHUD::OnTouchScreen));

	_listUIDelegate.Emplace(msg->GetUI().AddUObject(MessageUI::OPTION_SAVE_TO_CLIENT_COMPLETED,
		this, &UGsUIReuseScrollHUD::OnOptionSaveCompleted));
}

void UGsUIReuseScrollHUD::RemoveMsg()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
		return;

	for (MsgInputHandle& elem : _inputMsgHandlerList)
	{
		msg->GetWindowInput().Remove(elem);
	}
	_inputMsgHandlerList.Empty();

	for (MsgSystemHandle& elem : _listSystemDelegate)
	{
		msg->GetSystemParam().Remove(elem);
	}
	_listSystemDelegate.Empty();

	for (TPair<MessageUI, FDelegateHandle>& elem : _listUIDelegate)
	{
		msg->GetUI().Remove(elem);
	}
	_listUIDelegate.Empty();
}

void UGsUIReuseScrollHUD::OnWindowKeyMapping(const struct IGsMessageParam* InParam)
{
	const FGsInputEventMsgWindowKey* inputEventMsgWindowKey = InParam->Cast<const FGsInputEventMsgWindowKey>();
	if (nullptr == inputEventMsgWindowKey)
		return;

	switch (static_cast<EGsKeyMappingType>(inputEventMsgWindowKey->WindowKeyType))
	{
	case EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_1:
		InputKey(1);
		break;
	case EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_2:
		InputKey(2);
		break;
	case EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_3:
		InputKey(3);
		break;
	case EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_4:
		InputKey(4);
		break;
	case EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_5:
		InputKey(5);
		break;
	case EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_6:
		InputKeyExpansion(1);
		break;
	case EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_7:
		InputKeyExpansion(2);
		break;
	case EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_8:
		InputKeyExpansion(3);
		break;
	case EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_9:
		InputKeyExpansion(4);
		break;
	case EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_10:
		InputKeyExpansion(5);
		break;
	case EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_LEFT:
		InputAutoMoveScroll(0);
		break;
	case EGsKeyMappingType::KEY_MAPPING_QUICKSLOT_RIGHT:
		InputAutoMoveScroll(1);
		break;
	}
}

// 0 : Left , 1: Right
void UGsUIReuseScrollHUD::InputAutoMoveScroll(const uint8 InNum)
{
	if (0 == InNum)
	{
		WindowskeyInputLeft();
	}
	else
	{
		WindowskeyInputRight();
	}
}

void UGsUIReuseScrollHUD::InputKey(const uint8 InSlotNum)
{
	uint8 curPage = (uint8)GetCurrentPageType();
	uint8 resultSlotNum = curPage * _maxSlotCount + InSlotNum;
	uint8 index = 1;
	for (uint8 i = 0; i < _panelSlotHelperList.Num(); ++i)
	{
		if (nullptr == _panelSlotHelperList[i])
			continue;

		for (uint8 j = 0; j < _maxSlotCount; ++j)
		{
			if (nullptr == _panelSlotHelperList[i]->GetEntry(j))
				continue;

			if (resultSlotNum == index)
			{
				UGsUIReuseItemIconSlotHud* entry = Cast<UGsUIReuseItemIconSlotHud>(_panelSlotHelperList[i]->GetEntry(j));
				if (nullptr == entry)
					continue;

				entry->OnClickIconWindowInput();
				//GSLOG(Error, TEXT("[UGsUIReuseScrollHUD::InputKey] - InSlotNum : %d"), InSlotNum);
				return;
			}
			++index;
		}
	}
}

void UGsUIReuseScrollHUD::InputKeyExpansion(const uint8 InSlotNum)
{
	UGsGameUserSettings* gameUserSettings = GGameUserSettings();
	if (nullptr == gameUserSettings)
		return;

	bool useSlotExpansion = (bool)gameUserSettings->GetCombatSetting(EGsOptionCombat::ITEM_SLOT_EXPANSION);
	if (false == useSlotExpansion)
		return;

	uint8 EXPANSION_SLOT_START = MAX_QUICK_SLOT_SIZE / 2;
	InputKey(EXPANSION_SLOT_START + InSlotNum);
}

void UGsUIReuseScrollHUD::SetTargetPage(const EGsIconSlotCountType InTargetPageType)
{
	_reserveTargetPageType = (uint8)InTargetPageType > (uint8)_maxSlotPageType ? EGsIconSlotCountType::One : InTargetPageType;
}

void UGsUIReuseScrollHUD::InvalidateAll()
{
	InvalidateAllSlot();
}

void UGsUIReuseScrollHUD::InvalidateSlot(TArray<uint8>& InSlotIndexList)
{
	InvalidateSlotList(InSlotIndexList);
}

void UGsUIReuseScrollHUD::InvalidateAllSlot()
{
	for (uint8 i = 0; i < _panelSlotHelperList.Num(); ++i)
	{
		if (false == _panelSlotHelperList.IsValidIndex(i))
			break;

		if (nullptr == _panelSlotHelperList[i])
			continue;

		_panelSlotHelperList[i]->RefreshAll(_maxSlotCount);
	}

	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		bool useSlotExpansion = (bool)gameUserSettings->GetCombatSetting(EGsOptionCombat::ITEM_SLOT_EXPANSION);
		ESlateVisibility visibility = useSlotExpansion ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;
		_rootExpansion->SetVisibility(visibility);
		_spacerExpansion->SetVisibility(visibility);
	}
}

void UGsUIReuseScrollHUD::InvalidateSlotList(TArray<uint8>& InSlotIndexList)
{
	bool foundException = false;
	for (uint8 slotIndex : InSlotIndexList)
	{
		int32 pageIndex = GetFocusingPageIndex(slotIndex);
		if (0 > pageIndex)
		{
			foundException = true;
			break;
		}

		if (false == _panelSlotHelperList.IsValidIndex(pageIndex))
		{
			foundException = true;
			break;
		}

		int32 resultNum = slotIndex - pageIndex *_maxSlotCount - 1;

		_panelSlotHelperList[pageIndex]->RefreshByIndex(resultNum);
	}

	if (true == foundException)
	{
		InvalidateAllSlot();
	}
}

void UGsUIReuseScrollHUD::InvalidateAutoState(TArray<uint8>& InSlotIndexList)
{
	// InvalidateSlotList 참고하여 구현
	for (uint8 i = 0; i < InSlotIndexList.Num(); ++i)
	{
		int32 pageIndex = GetFocusingPageIndex(InSlotIndexList[i]);
		if (0 > pageIndex)
		{
			continue;
		}

		if (false == _panelSlotHelperList.IsValidIndex(pageIndex))
		{
			continue;
		}

		int32 maxNum = (pageIndex + 1) * _maxSlotCount;
		int32 calc = (InSlotIndexList[i] - maxNum) + _maxSlotCount;
		int32 resultNum = calc - 1;

		if (UWidget* widget = _panelSlotHelperList[pageIndex]->GetEntry(resultNum))
		{
			if (UGsUIReuseItemIconSlotHud* entry = Cast<UGsUIReuseItemIconSlotHud>(widget))
			{
				entry->InvalidateAutoState();
			}
		}
	}
}

void UGsUIReuseScrollHUD::OnChangeEditMode(const bool InIsEdit)
{
	for (uint8 i = 0; i < _panelSlotHelperList.Num(); ++i)
	{
		if(nullptr == _panelSlotHelperList[i])
			continue;

		for (uint8 j = 0; j < _maxSlotCount; ++j)
		{
			if(nullptr == _panelSlotHelperList[i]->GetEntry(j))
				continue;

			UGsUIReuseItemIconSlotHud* entry = Cast<UGsUIReuseItemIconSlotHud>(_panelSlotHelperList[i]->GetEntry(j));
			if (nullptr == entry)
				continue;

			entry->SetEditModeEffect(InIsEdit);
			//entry->OnChangeSlotRemoveBtnVisible();
		}
	}
}

void UGsUIReuseScrollHUD::ResetSelection(const QuickSlotId InSlotID)
{
	uint8 curPage = (uint8)GetCurrentPageType();
	if (false == _panelSlotHelperList.IsValidIndex(curPage))
		return;

	UGsDynamicPanelSlotHelper* panal = _panelSlotHelperList[curPage];
	if (nullptr == panal)
		return;

	for (uint8 i = 0; i < _maxSlotCount; ++i)
	{
		UWidget* widget = panal->GetEntry(i);
		if (nullptr == widget)
			continue;

		UGsUIReuseItemIconSlotHud* entry = Cast<UGsUIReuseItemIconSlotHud>(widget);
		if (nullptr == entry)
			continue;
		
		if (entry->GetSlotIndex() == InSlotID)
		{
			entry->ResetSelection();
		}
	}
}

void UGsUIReuseScrollHUD::ResetAllSelection()
{
	for (uint8 i = 0; i < _panelSlotHelperList.Num(); ++i)
	{
		if (nullptr == _panelSlotHelperList[i])
			continue;

		for (uint8 j = 0; j < _maxSlotCount; ++j)
		{
			if (nullptr == _panelSlotHelperList[i]->GetEntry(j))
				continue;

			UGsUIReuseItemIconSlotHud* entry = Cast<UGsUIReuseItemIconSlotHud>(_panelSlotHelperList[i]->GetEntry(j));
			if (nullptr == entry)
				continue;

			entry->ResetSelection();
		}
	}
}

void UGsUIReuseScrollHUD::SetSlotDimmed(const bool InState, TArray<ItemCategorySub> InDimmedTypeList)
{
	UGsItemManager* itemMgr = GItem();
	for (uint8 i = 0; i < _panelSlotHelperList.Num(); ++i)
	{
		if (nullptr == _panelSlotHelperList[i])
			continue;

		for (uint8 j = 0; j < _maxSlotCount; ++j)
		{
			if (nullptr == _panelSlotHelperList[i]->GetEntry(j))
				continue;

			UGsUIReuseItemIconSlotHud* entry = Cast<UGsUIReuseItemIconSlotHud>(_panelSlotHelperList[i]->GetEntry(j));
			if (nullptr == entry)
				continue;

			const FGsItem* itemData = entry->GetIconItemData();
			entry->SetSlotDimmed(((InState) && (itemData) && (itemMgr->IsReactionLockTypeItem(itemData->GetSubCategory()))));
		}
	}
}

int32 UGsUIReuseScrollHUD::GetFocusingPageIndex(const uint8 InSlotIndex)
{
	if (0 >= InSlotIndex)
		return -1;

	if (0 == _maxSlotCount)
		return -1;

	return (InSlotIndex - 1) / _maxSlotCount;
}

void UGsUIReuseScrollHUD::OnTouchScreen(const IGsMessageParam* inParam)
{
	const FGsPrimitivePairFloat* Param = inParam->Cast<const FGsPrimitivePairFloat>();
	if (nullptr == Param)
		return;

}

void UGsUIReuseScrollHUD::OnOptionSaveCompleted(const IGsMessageParam*)
{
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		bool prevUseSlotExpansion = _rootExpansion->IsVisible();
		bool currentUseSlotExpansion = (bool)gameUserSettings->GetCombatSetting(EGsOptionCombat::ITEM_SLOT_EXPANSION);
		if (prevUseSlotExpansion != currentUseSlotExpansion)
		{
			InvalidateAllSlot();
		}
	}
}

void UGsUIReuseScrollHUD::OnTouchRelease(const FGsInputEventMsgBase& InMsg)
{
	UGsQuickSlotManager* slotManager = GSQuickSlot();
	if (nullptr == slotManager)
		return;

	slotManager->ResetQuickSlotSelectionInfo();
	ResetAllSelection();
}

void UGsUIReuseScrollHUD::OnChangeMainScrollEvent(float InCurOffset)
{
	if (IsTutorial())
	{
		_scrollBox->SetScrollOffset(_tutorialScrollOffset);
		return;
	}

	Super::OnChangeMainScrollEvent(InCurOffset);
}

void UGsUIReuseScrollHUD::SetSlotMove(int32 InMoveTargetPageIndex, int32 InDestPageIndex)
{
	TSharedRef<SWidget> sharedRef = _root->TakeWidget();
	if (SHorizontalBox* horizontalBox = static_cast<class SHorizontalBox*>(&sharedRef.Get()))
	{
		FChildren* children = horizontalBox->GetChildren();
		TPanelChildren<SHorizontalBox::FSlot>* PanelChildren = static_cast<TPanelChildren<SHorizontalBox::FSlot>*>(children);
		PanelChildren->Move(InMoveTargetPageIndex, InDestPageIndex);
	}

	TSharedRef<SWidget> sharedRefExpansion = _rootExpansion->TakeWidget();
	if (SHorizontalBox* horizontalBox = static_cast<class SHorizontalBox*>(&sharedRefExpansion.Get()))
	{
		FChildren* children = horizontalBox->GetChildren();
		TPanelChildren<SHorizontalBox::FSlot>* PanelChildren = static_cast<TPanelChildren<SHorizontalBox::FSlot>*>(children);
		PanelChildren->Move(InMoveTargetPageIndex, InDestPageIndex);
	}
}

void UGsUIReuseScrollHUD::CreateEntryReuseSlot()
{
	for (uint8 i = 0; i < _rootPanelList.Num(); ++i)
	{
		if (false == _rootPanelList.IsValidIndex(i))
			continue;

		if (i > (uint8)_maxSlotPageType && _switcherPanelList.IsValidIndex(i))
		{
			_switcherPanelList[i]->SetVisibility(ESlateVisibility::Collapsed);
		}

		if (nullptr == _panelSlotHelperList[i])
		{
			_panelSlotHelperList[i] = NewObject<UGsDynamicPanelSlotHelper>(this);
			_panelSlotHelperList[i]->Initialize(_entryWidgetClassList[(uint8)EGsIconSlotType::HudType], _rootPanelList[i]);
			_panelSlotHelperList[i]->OnCreateEntry.AddDynamic(this, &UGsUIReuseScrollHUD::OnCreateEntry);
			_panelSlotHelperList[i]->OnRefreshEntry.AddDynamic(this, &UGsUIReuseScrollHUD::OnRefreshEntry);
		}
	}
}

void UGsUIReuseScrollHUD::OnCreateEntry(UWidget* InEntry)
{
	UGsUIReuseItemIconSlotHud* entry = Cast<UGsUIReuseItemIconSlotHud>(InEntry);
	if (nullptr == entry)
		return;

	entry->Init();
	entry->SetSlotIndex(_slotIndex);
	++_slotIndex;
}

void UGsUIReuseScrollHUD::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUIReuseItemIconSlotHud* entry = Cast<UGsUIReuseItemIconSlotHud>(InEntry);
	if (nullptr == entry)
		return;

	UGsQuickSlotManager* slotManager = GSQuickSlot();
	if (nullptr == slotManager)
		return;

	uint8 slotIndex = entry->GetSlotIndex();
	const QuickSlotInfo* quickSlotInfo = slotManager->GetQuickSlotInfoBySlotID(slotIndex);
	if (nullptr == quickSlotInfo)
	{
		entry->SetSlotItemIcon(0, 0, 0, 0);
		return;
	}

	entry->SetSlotItemIcon(quickSlotInfo->mItemDBId, quickSlotInfo->mTemplateId, 0, 0);
}

UGsUIReuseItemIconSlotHud* UGsUIReuseScrollHUD::SetTutorialTarget(int32 InSlotIndex)
{
	_tutorialSlotIndex = InSlotIndex;

	if (0 > _tutorialSlotIndex ||
		0 == _rootPanelList.Num())
	{
		_tutorialScrollOffset = 0.f;
		_scrollBox->SetAllowOverscroll(true);
		return nullptr;
	}

	_scrollBox->SetAllowOverscroll(false);
	_tutorialScrollOffset = _scrollBox->GetScrollOffset();
	
	int32 index = static_cast<int32>(GetCurrentPageType());
	return Cast<UGsUIReuseItemIconSlotHud>(_rootPanelList[index]->GetChildAt(InSlotIndex));
}