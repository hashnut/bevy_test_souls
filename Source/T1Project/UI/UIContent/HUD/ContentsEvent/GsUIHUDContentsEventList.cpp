#include "GsUIHUDContentsEventList.h"

#include "ContentsEvent/EGsContentsEventState.h"
#include "ContentsEvent/GsContentsEventGroup.h"
#include "Management/GsMessageHolder.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsContentsEventManager.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsContentsEventMessageParam.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconContentsEvent.h"
#include "UI/UIContent/HUD/ContentsEvent/GsUIContentsEventTooltip.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"

void UGsUIHUDContentsEventList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	MContentsEvent* ContentsEvent = FGsContentsEventMessageParamContentsEventGroupStateChanged::GetMessageHandler();
	if (nullptr == ContentsEvent)
	{
		GSLOG(Error, TEXT("nullptr == ContentsEvent"));
		return;
	}
	else
	{
		_contentsEventHandle = ContentsEvent->AddUObject(MessageContentContentsEvent::CONTENTS_EVENT_GROUP_STATE_CHANGED, this, &UGsUIHUDContentsEventList::OnContentsEventGroupStateChanged);
	}

	_slotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_slotHelper->Initialize(_iconSelector);
	_slotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIHUDContentsEventList::OnRefreshIcon);
}

void UGsUIHUDContentsEventList::NativeConstruct()
{
	Super::NativeConstruct();

	ResetSelectedIndex();
	
	UpdateIconParameters();
	InvalidateIcons();
}

void UGsUIHUDContentsEventList::NativeDestruct()
{
	ResetSelectedIndex();

	MContentsEvent* ContentsEvent = FGsContentsEventMessageParamContentsEventGroupStateChanged::GetMessageHandler();
	if (nullptr != ContentsEvent)
	{
		ContentsEvent->Remove(_contentsEventHandle);
	}

	Super::NativeDestruct();
}

void UGsUIHUDContentsEventList::SetHUDMode(EGsUIHUDMode InMode)
{
	InvalidateIcons();
}

void UGsUIHUDContentsEventList::UpdateIconParameters()
{
	UGsContentsEventManager* ContentsEventManager = GSContentsEvent();
	if (nullptr == ContentsEventManager)
	{
		GSLOG(Error, TEXT("nullptr == ContentsEventManager"));
		return;
	}

	const TArray<TWeakPtr<const FGsContentsEventGroup>> ContentsEventGroups = ContentsEventManager->GetVisibleContentsEventGroups();
	_iconParameters.Empty(ContentsEventGroups.Num());
	int32 IconParameterIndex = 0;
	for (const TWeakPtr<const FGsContentsEventGroup>& ContentsEventGroup : ContentsEventGroups)
	{
		if (!ContentsEventGroup.IsValid())
		{
			GSLOG(Error, TEXT("!ContentsEventGroup.IsValid()"));
			continue;
		}

		_iconParameters.EmplaceAt(IconParameterIndex, *ContentsEventGroup.Pin());
		++IconParameterIndex;
	}
}

void UGsUIHUDContentsEventList::InvalidateIcons()
{
	_slotHelper->RefreshAll(_iconParameters.Num());
}

void UGsUIHUDContentsEventList::InvalidateTooltip()
{
	if (_iconParameters.IsValidIndex(_selectedIndex))
	{
		const UGsUIIconContentsEvent::Parameters& SelectedParameter = _iconParameters[_selectedIndex];

		_tooltip->SetParameters({
			SelectedParameter._title,
			SelectedParameter._desc,
			SelectedParameter._subDesc
			});

		_tooltip->Open();
	}
	else
	{
		_tooltip->Close();
	}
}

void UGsUIHUDContentsEventList::ResetSelectedIndex()
{
	_selectedIndex = -1;
}

void UGsUIHUDContentsEventList::OnContentsEventGroupStateChanged(const IGsMessageParam&)
{
	UpdateIconParameters();
	InvalidateIcons();
}

void UGsUIHUDContentsEventList::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconContentsEvent* Icon = Cast<UGsUIIconContentsEvent>(InIcon);
	if (nullptr == Icon)
	{
		GSLOG(Error, TEXT("nullptr == Icon, InIndex:%d"), InIndex);
		return;
	}

	if (!_iconParameters.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("!_iconParameters.IsValidIndex(InIndex), InIndex:%d"), InIndex);
		return;
	}

	Icon->SetParameters(_iconParameters[InIndex]);
	Icon->OnClickIcon.BindUObject(this, &UGsUIHUDContentsEventList::OnClickIcon, InIndex);
}

void UGsUIHUDContentsEventList::OnClickIcon(const int32 InIndex)
{
	if (_selectedIndex == InIndex)
	{
		ResetSelectedIndex();
	}
	else
	{
		_selectedIndex = InIndex;
	}

	InvalidateTooltip();
}
