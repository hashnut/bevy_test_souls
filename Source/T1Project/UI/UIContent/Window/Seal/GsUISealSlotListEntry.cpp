#include "GsUISealSlotListEntry.h"

#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "GameObject/Stat/GsStatHelper.h"
#include "Seal/GsSealSlot.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "T1Project.h"
#include "UI/UIContent/Window/GsUIWindowSeal.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UILib/Base/GsUIVFX.h"

void UGsUISealSlotListItem::SetSlotIndex(const int32 InSlotIndex)
{
	_slotIndex = InSlotIndex;
}

void UGsUISealSlotListItem::ResetData()
{
	_activated = false;
	_title = FText::GetEmpty();
	_unlockStat = true;
	_unlockColor = true;
}

int32 UGsUISealSlotListItem::GetCurrentSealRerollLockCount() const
{
	return (_unlockStat ? 0 : 1) + (_unlockColor ? 0 : 1);
}

void UGsUISealSlotListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr != _unlockStatBtn)
	{
		_unlockStatBtn->OnClicked.AddUniqueDynamic(this, &UGsUISealSlotListEntry::OnClickLockStatBtn);
	}

	if (nullptr != _unlockColorBtn)
	{
		_unlockColorBtn->OnClicked.AddUniqueDynamic(this, &UGsUISealSlotListEntry::OnClickLockColorBtn);
	}
}

void UGsUISealSlotListEntry::Invalidate(const bool InClearEffect)
{
	if (InClearEffect && nullptr != _casting)
	{
		_casting->HideVFX();
	}

	const UGsUISealSlotListItem* ListItem = (_listItem.IsValid()) ? _listItem.Get() : nullptr;
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	if (nullptr != _colorSwitcher)
	{
		int32 ColorSwitcherIndex = 0;
		switch (ListItem->_sealColor)
		{
		case SealSlotColor::RARE:
			ColorSwitcherIndex = 1;
			break;
		case SealSlotColor::EPIC:
			ColorSwitcherIndex = 2;
			break;
		case SealSlotColor::UNIQUE:
			ColorSwitcherIndex = 3;
			break;
		}

		_colorSwitcher->SetActiveWidgetIndex(ColorSwitcherIndex);
	}

	if (nullptr != _headerTxt)
	{
		_headerTxt->SetText(ListItem->_header);
	}

	if (nullptr != _titleTxt)
	{
		_titleTxt->SetText(ListItem->_title);
	}

	if (nullptr != _unlockStatSwitcher)
	{
		_unlockStatSwitcher->SetActiveWidgetIndex(ListItem->_unlockStat ? 0 : 1);
	}

	if (nullptr != _unlockColorSwitcher)
	{
		_unlockColorSwitcher->SetActiveWidgetIndex(ListItem->_unlockColor ? 0 : 1);
	}
}

void UGsUISealSlotListEntry::SetListItem(TWeakObjectPtr<UGsUISealSlotListItem> InSealSlotListItem)
{
	_listItem = InSealSlotListItem;
}

UGsUISealSlotListItem* UGsUISealSlotListEntry::GetListItem() const
{
	// 구현 금지
	check(false);
	return nullptr;
}

void UGsUISealSlotListEntry::OnClickLockStatBtn()
{
	UGsUISealSlotListItem* ListItem = (_listItem.IsValid()) ? _listItem.Get() : nullptr;
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	if (ListItem->_onClickUnlockStat.IsBound())
	{
		ListItem->_onClickUnlockStat.Execute(ListItem->_slotIndex, EGsSealRerollUnlockTarget::Stat);
	}
}

void UGsUISealSlotListEntry::OnClickLockColorBtn()
{
	UGsUISealSlotListItem* ListItem = (_listItem.IsValid()) ? _listItem.Get() : nullptr;
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	if (ListItem->_onClickUnlockColor.IsBound())
	{
		ListItem->_onClickUnlockColor.Execute(ListItem->_slotIndex, EGsSealRerollUnlockTarget::Color);
	}
}

void UGsUISealSlotListEntry::OnUIEvent(const FGsUIWindowSealEventParameters& InEventParameters)
{
	const UGsUISealSlotListItem* ListItem = (_listItem.IsValid()) ? _listItem.Get() : nullptr;
	if (nullptr == ListItem)
	{
		GSLOG(Error, TEXT("nullptr == ListItem"));
		return;
	}

	switch (InEventParameters._event)
	{
	case EGsUISealWindowEvent::Reset:
		if (nullptr != _casting)
		{
			_casting->HideVFX();
		}
		break;

	case EGsUISealWindowEvent::SealSlot0:
	case EGsUISealWindowEvent::SealSlot1:
	case EGsUISealWindowEvent::SealSlot2:
		if (nullptr != _casting && InEventParameters._nodeId == ListItem->_sealNodeId && InEventParameters._slotIndex == ListItem->_slotIndex)
		{
			// 하나라도 열린 경우에 연출 출력. 모두 잠금인 경우, 연출 불필요.
			if (ListItem->_unlockColor || ListItem->_unlockStat)
			{
				_casting->ShowVFX();
			}
		}
		break;
	}
}
