// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildDivisionComboBox.h"

#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

#include "Management/ScopeGlobal/GsGuildManager.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"



void UGsUIGuildDivisionComboBox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_btnMenu)
	{
		_btnMenu->OnClicked.AddDynamic(this, &UGsUIGuildDivisionComboBox::OnClickMenu);
	}

	if (_btnCombo1)
	{
		_btnCombo1->OnClicked.AddDynamic(this, &UGsUIGuildDivisionComboBox::OnClickCombo1);
	}
	if (_btnCombo2)
	{
		_btnCombo2->OnClicked.AddDynamic(this, &UGsUIGuildDivisionComboBox::OnClickCombo2);
	}
	if (_btnCombo3)
	{
		_btnCombo3->OnClicked.AddDynamic(this, &UGsUIGuildDivisionComboBox::OnClickCombo3);
	}
	if (_btnCombo4)
	{
		_btnCombo4->OnClicked.AddDynamic(this, &UGsUIGuildDivisionComboBox::OnClickCombo4);
	}
}

void UGsUIGuildDivisionComboBox::OnClickMenu()
{
	const int32 OPENED = _btnMenu->_slotIndexSelected;
	const int32 CLOSED = _btnMenu->_slotIndexUnselected;

	if (false == _isDivision)
	{
		if (false == GetIsEnabled())
		{
			_btnMenu->SetSwitcherIndex(CLOSED);
			_panelComboList->SetVisibility(ESlateVisibility::Collapsed);
			return;
		}
	}

	bool isPanelVisible = _panelComboList->IsVisible();
	_btnMenu->SetSwitcherIndex(isPanelVisible ? CLOSED : OPENED);
	_panelComboList->SetVisibility(isPanelVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	OnClickComboBox.ExecuteIfBound();
}

void UGsUIGuildDivisionComboBox::OnClickCombo1()
{
	if (_isDivision)
	{
		OnClickDivision(GuildAuctionType::NORMAL);
	}
	else
	{
		OnClickResult(GuildAuctionResultType::NORMAL);
	}
}

void UGsUIGuildDivisionComboBox::OnClickCombo2()
{
	if (_isDivision)
	{
		OnClickDivision(GuildAuctionType::PARTICIPANTS);
	}
	else
	{
		OnClickResult(GuildAuctionResultType::PARTICIPANTS);
	}
}

void UGsUIGuildDivisionComboBox::OnClickCombo3()
{
	OnClickDivision(GuildAuctionType::APPOINTMENT);
}

void UGsUIGuildDivisionComboBox::OnClickCombo4()
{
	OnClickDivision(GuildAuctionType::DIRECT_DISTRIBUTION);
}

void UGsUIGuildDivisionComboBox::OnClickDivision(GuildAuctionType InDivisionType)
{
	if (InDivisionType == _divisionType)
	{
		Hide();
		return;
	}

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	_divisionType = InDivisionType;

	// 분배 방식 변경 시 선택된 모든 체크 정보 초기화
	auctionData->RemoveCheckedItemAll();
	
	OnChangeComboBox.ExecuteIfBound();
	Hide();
	InvalidateComboBox();
}

void UGsUIGuildDivisionComboBox::OnClickResult(GuildAuctionResultType InResultType)
{
	if (InResultType == _resultType)
	{
		Hide();
		return;
	}

	FGsGuildManager* guildManager = GGuild();
	if (nullptr == guildManager)
		return;

	FGsGuildAuctionData* auctionData = guildManager->GetMyGuildAuctionData();
	if (nullptr == auctionData)
		return;

	_resultType = InResultType;

	// 정산 방식 변경 시 선택된 모든 체크 정보 초기화
	auctionData->RemoveCheckedItemAll();
	
	OnChangeComboBox.ExecuteIfBound();
	Hide();
	InvalidateComboBox();
}

void UGsUIGuildDivisionComboBox::SetDivisionType()
{
	// 분배 방식
	_isDivision = true;
	_divisionType = GuildAuctionType::NORMAL;

	_btnCombo3->SetVisibility(ESlateVisibility::Visible);
	_btnCombo4->SetVisibility(ESlateVisibility::Visible);

	FText findText;
	// TEXT: 분배 방식
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Division_Type"), findText);
	_textBlockType->SetText(findText);
	// TEXT: 일반 경매
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Division_Type_Common"), findText);
	_textBlockCombo1->SetText(findText);
	_textBlockComboSelected1->SetText(findText);
	// TEXT: 참여자 경매
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Division_Type_Acquisition"), findText);
	_textBlockCombo2->SetText(findText);
	_textBlockComboSelected2->SetText(findText);
	// TEXT: 지정 경매
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Division_Type_Appointment"), findText);
	_textBlockCombo3->SetText(findText);
	_textBlockComboSelected3->SetText(findText);
	// TEXT: 지정 지급
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Appointment_Provide_PopUp_Title"), findText);
	_textBlockCombo4->SetText(findText);
	_textBlockComboSelected4->SetText(findText);

	Hide();
	InvalidateComboBox();
}

void UGsUIGuildDivisionComboBox::SetResultType()
{
	// 정산 방식
	_isDivision = false;
	_resultType = GuildAuctionResultType::NORMAL;

	_btnCombo3->SetVisibility(ESlateVisibility::Collapsed);
	_btnCombo4->SetVisibility(ESlateVisibility::Collapsed);

	FText findText;
	// TEXT: 정산 방식
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Calculate_Type"), findText);
	_textBlockType->SetText(findText);
	// TEXT: 일반 정산
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Calculate_Type_Common"), findText);
	_textBlockCombo1->SetText(findText);
	_textBlockComboSelected1->SetText(findText);
	// TEXT: 참여자 정산
	FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Calculate_Type_Acquisition"), findText);
	_textBlockCombo2->SetText(findText);
	_textBlockComboSelected2->SetText(findText);

	Hide();
	InvalidateComboBox();
}

void UGsUIGuildDivisionComboBox::Hide()
{
	const int32 CLOSED = _btnMenu->_slotIndexUnselected;

	_panelComboList->SetVisibility(ESlateVisibility::Collapsed);
	_btnMenu->SetSwitcherIndex(CLOSED);
}

void UGsUIGuildDivisionComboBox::InvalidateComboBox()
{
	const int32 SELECTED = _btnCombo1->_slotIndexSelected;
	const int32 UNSELECTED = _btnCombo1->_slotIndexUnselected;

	FText findText;
	if (_isDivision)
	{
		// 분배 방식
		_btnCombo1->SetSwitcherIndex(_divisionType == GuildAuctionType::NORMAL ? SELECTED : UNSELECTED);
		_btnCombo2->SetSwitcherIndex(_divisionType == GuildAuctionType::PARTICIPANTS ? SELECTED : UNSELECTED);
		_btnCombo3->SetSwitcherIndex(_divisionType == GuildAuctionType::APPOINTMENT ? SELECTED : UNSELECTED);
		_btnCombo4->SetSwitcherIndex(_divisionType == GuildAuctionType::DIRECT_DISTRIBUTION ? SELECTED : UNSELECTED);

		switch (_divisionType)
		{
		case GuildAuctionType::NORMAL:
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Division_Type_Common"), findText);
			break;
		case GuildAuctionType::PARTICIPANTS:
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Division_Type_Acquisition"), findText);
			break;
		case GuildAuctionType::APPOINTMENT:
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Division_Type_Appointment"), findText);
			break;
		case GuildAuctionType::DIRECT_DISTRIBUTION:
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Appointment_Provide_PopUp_Title"), findText);
			break;
		default:
			break;
		}
	}
	else
	{
		// 정산 방식
		_btnCombo1->SetSwitcherIndex(_resultType == GuildAuctionResultType::NORMAL ? SELECTED : UNSELECTED);
		_btnCombo2->SetSwitcherIndex(_resultType == GuildAuctionResultType::PARTICIPANTS ? SELECTED : UNSELECTED);

		switch (_resultType)
		{
		case GuildAuctionResultType::NORMAL:
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Calculate_Type_Common"), findText);
			break;
		case GuildAuctionResultType::PARTICIPANTS:
			FText::FindText(TEXT("GuildAuctionUIText"), TEXT("Calculate_Type_Acquisition"), findText);
			break;
		default:
			break;
		}
	}
	_textBlockMenu->SetText(findText);
}
