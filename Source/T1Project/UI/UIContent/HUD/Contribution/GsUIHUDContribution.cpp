// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDContribution.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Management/GsMessageHolder.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "GameFlow/Stage/GsStageMode.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/GsGameFlowGame.h"


void UGsUIHUDContribution::NativeOnInitialized()
{
	Super::NativeOnInitialized();	

	_openButton->OnClicked.AddDynamic(this, &UGsUIHUDContribution::OnShowContribution);
	_closeButton->OnClicked.AddDynamic(this, &UGsUIHUDContribution::OnHideContribution);
}

void UGsUIHUDContribution::NativeConstruct()
{
	Super::NativeConstruct();

	_isOpen = false;

	MGuildDungeon& guildDungeon = GMessage()->GetGuildDungeon();

	//guild dungeon event
	_listGuildDungeonDelegate.Emplace(guildDungeon.AddUObject(MessageContentGuildDungeon::ENTER,
		this, &UGsUIHUDContribution::OnShowContribution));

	_listGuildDungeonDelegate.Emplace(guildDungeon.AddUObject(MessageContentGuildDungeon::EXIT,
		this, &UGsUIHUDContribution::OnHideContribution));

	InvalidateAllInternal();
}

void UGsUIHUDContribution::NativeDestruct()
{
	FGsMessageHolder* msg = GMessage();

	for (MsgGuildDungeonHandle& iter : _listGuildDungeonDelegate)
	{
		msg->GetGuildDungeon().Remove(iter);
	}
	_listGuildDungeonDelegate.Empty();

	Super::NativeDestruct();
}

void UGsUIHUDContribution::InvalidateAllInternal()
{
	if (EGsUIHUDMode::CAMERA == _hudMode)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		if (GLevel()->IsGuildDungeonMap())
		{
			SetVisibility(ESlateVisibility::Visible);

			if (_isOpen)
			{
				OnShowContribution();
			}
			else
			{
				OnHideContribution();
			}
		}
		else
		{
			SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UGsUIHUDContribution::OnShowContribution()
{
	//_isOpen = true;
	//
	//_buttonSwitcher->SetActiveWidgetIndex(1);

	//TArray<UGsContributionListViewEntryData*> dataList;

	//int32 num = 5;
	//for (int32 i = 0; i < num; ++i)
	//{
	//	UGsContributionListViewEntryData* data = NewObject<UGsContributionListViewEntryData>();
	//	data->_rank = i;
	//	data->_userName = TEXT("테스트");
	//	data->_contribution = 0.5f;
	//}

	//_listView->SetListItems(dataList);

	//UGsContributionListViewEntryData* playerData = NewObject<UGsContributionListViewEntryData>();
	//playerData->_rank = 0;
	//playerData->_userName = TEXT("플레이어");
	//playerData->_contribution = 0.5f;
	//_playerContribution->NativeOnListItemObjectSet(playerData);
}

void UGsUIHUDContribution::OnHideContribution()
{
	_buttonSwitcher->SetActiveWidgetIndex(0);

	_isOpen = false;
}

void UGsUIHUDContribution::SetHUDMode(EGsUIHUDMode inMode)
{
	if (IsHideMode(inMode)
		|| inMode == EGsUIHUDMode::MENU
		|| false == _isVisible)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FGsStageManagerGame* stage = gameFlow->GetStageManagerGame();
	if (nullptr == stage)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	if (stage->GetCurrentStageMode() != FGsStageMode::Game::GUILD_DUNGEON)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIHUDContribution::OnShowContribution(const IGsMessageParam* inParam)
{
	_isVisible = true;
	InvalidateAllInternal();
}

void UGsUIHUDContribution::OnHideContribution(const IGsMessageParam* inParam)
{
	_isVisible = false;
	InvalidateAllInternal();
}