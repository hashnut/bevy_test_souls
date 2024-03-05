// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIServerListSlot.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/TextBlock.h"
#include "UserWidget.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UMG/Public/Components/WidgetSwitcher.h"


void UGsUIServerListSlot::NativeOnInitialized()
{
	_btnNormal->OnClicked.AddDynamic(this, &UGsUIServerListSlot::OnClickNormal);
	_btnSelected->OnClicked.AddDynamic(this, &UGsUIServerListSlot::OnClickSelected);

	Super::NativeOnInitialized();
}

void UGsUIServerListSlot::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_btnNormal->SetVisibility((bInIsSelected) ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
	_btnSelected->SetVisibility((bInIsSelected) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

bool UGsUIServerListSlot::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIServerListSlot::OnClickNormal()
{
	OnClickSlot.ExecuteIfBound(_index);
}

void UGsUIServerListSlot::OnClickSelected()
{
	OnClickSlot.ExecuteIfBound(_index);
}

void UGsUIServerListSlot::SetData(int InIndex, const FString& InServerName, bool InSelected)
{
	_textTempId = FText::FromString(InServerName);

	if (_panelMyCharacter)
	{
		_panelMyCharacter->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (_panelNew)
	{
		_panelNew->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (_SwitcherCongestion)
	{
		_SwitcherCongestion->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (_panelRecommend)
	{
		_panelRecommend->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (_panelCharCreatable)
	{
		_panelCharCreatable->SetVisibility(ESlateVisibility::Collapsed);
	}

	SetIndex(InIndex);
}

void UGsUIServerListSlot::SetData(int InIndex, const FString& InServerName, uint16 InUserCount, uint8 InCongestion, bool InCharCreatable,
	bool InRecommended, bool InNewOpened, bool InSelected)
{
	_textTempId = FText::FromString(InServerName);

	if (_panelMyCharacter)
	{
		_panelMyCharacter->SetVisibility((0 >= InUserCount) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	if (_panelNew)
	{
		_panelNew->SetVisibility((false == InNewOpened) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	if (_SwitcherCongestion)
	{
		_SwitcherCongestion->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		/*int index = 0;
		if (InCongestion <= (int)ChannelCongestionStatus::MEDIUM)
		{
			index = (int)ChannelCongestionStatus::MEDIUM;
		}
		if (InCongestion > (int)ChannelCongestionStatus::FULL)
		{
			index = (int)ChannelCongestionStatus::FULL;
		}*/

		_SwitcherCongestion->SetActiveWidgetIndex(InCongestion);
	}

	if (_panelRecommend)
	{
		_panelRecommend->SetVisibility((false == InRecommended) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}	

	if (_panelCharCreatable)
	{
		_panelCharCreatable->SetVisibility((true == InCharCreatable) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	/*FText findText;
	if (_panelRecommend)
	{
		switch (static_cast<ChannelCongestionStatus>(InCongestion))
		{
		case ChannelCongestionStatus::LOW:
		case ChannelCongestionStatus::MEDIUM:
			FText::FindText(TEXT("UICommonText"), TEXT("Server_Condition_Good"), findText);			
			break;
		case ChannelCongestionStatus::HIGH:
			FText::FindText(TEXT("UICommonText"), TEXT("Server_Condition_Confusion"), findText);
			break;
		case ChannelCongestionStatus::FULL:
			FText::FindText(TEXT("UICommonText"), TEXT("Server_Condition_Bad"), findText);
			break;
		default:
			break;
		}		
	}*/
	
	SetIndex(InIndex);
}


