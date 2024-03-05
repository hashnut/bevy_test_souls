// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupPlayerStatDetail.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Components/WidgetSwitcher.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatHandler.h"


void UGsUIPopupPlayerStatDetail::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupPlayerStatDetail::OnClickTab);

	_closeButton->OnClicked.AddDynamic(this, &UGsUIPopupPlayerStatDetail::OnClickClose);
}

void UGsUIPopupPlayerStatDetail::NativeConstruct()
{
	Super::NativeConstruct();
	_toggleGroup.AddToggleWidgetByParentPanel(_panelList, false);
}

void UGsUIPopupPlayerStatDetail::NativeDestruct()
{
	_toggleGroup.Clear();

	Super::NativeDestruct();
}

void UGsUIPopupPlayerStatDetail::SetTab(EGsPlayerStatDetailType inTabType)
{
	_toggleGroup.SetSelectedIndexNoEvent((int32)inTabType);
	OnClickTab((int32)inTabType);
}

void UGsUIPopupPlayerStatDetail::OnClickTab(int32 InIndex)
{
	//select tab
	_panelWidgetSwitcher->SetActiveWidgetIndex(InIndex);
}

void UGsUIPopupPlayerStatDetail::OnClickClose()
{
	// 분배 스탯 정보 초기화
	if (FGsPlayerStatHandler* statHandler = FGsPlayerStatHandler::GetInstance())
	{
		if (FGsPlayerStatDistributionController* distributionController = statHandler->GetStatDistributionController())
		{
			distributionController->UpdateStatPoint();
		}
	}

	if (FGsMessageHolder* holder = GMessage())
	{
		holder->GetUI().SendMessage(MessageUI::INVALID_PLAYER_STAT_DISTRIBUTION, nullptr);
	}

	Close();
}

void UGsUIPopupPlayerStatDetail::OnInputCancel()
{
	OnClickClose();
}