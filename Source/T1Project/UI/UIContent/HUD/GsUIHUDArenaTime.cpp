// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDArenaTime.h"

#include "Components/TextBlock.h"

#include "DataSchema/Arena/GsSchemaArenaData.h"

#include "GameObject/Fence/GsFenceHandler.h"

#include "Management/ScopeGame/GsArenaManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "UI/UIContent/Helper/GsTimeStringHelper.h"

#include "UTIL/GsTimeSyncUtil.h"


void UGsUIHUDArenaTime::InvalidateAllInternal()
{
	InvalidateArena();
}

bool UGsUIHUDArenaTime::UpdateManagerTick(float InDeltaTime)
{
	if (false == Super::UpdateManagerTick(InDeltaTime))
	{
		return false;
	}

	if (_startTime == 0)
		return false;

	// 경기 시작 후 지나간 시간
	time_t spendTime = FGsTimeSyncUtil::GetServerNowSecond() - _startTime;

	// 남은 시간
	FTimespan remainTime((_maxTime - spendTime) * ETimespan::TicksPerSecond);

	FString outTimeString;
	FGsTimeStringHelper::GetTimeStringCeil(remainTime, outTimeString, true);

	// X분 Y초 or X초
	_textBlockTime->SetText(FText::FromString(outTimeString));

	return true;
}

void UGsUIHUDArenaTime::SetHUDMode(EGsUIHUDMode InMode)
{
	InvalidateArena();
}

void UGsUIHUDArenaTime::InvalidateArena()
{
	FGsArenaManager* arenaManager = GSArena();
	FGsFenceHandler* fenceHandler = GFence();
	if (nullptr == arenaManager || nullptr == fenceHandler)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	if (const FGsSchemaArenaData* arenaData = arenaManager->GetArenaData())
	{
		_maxTime = arenaData->arenaTimeSec;
	}

	if (fenceHandler->IsEnteredTerritoryType(TerritoryType::ARENA) && arenaManager->IsArenaStarted())
	{
		_startTime = arenaManager->GetArenaStartTime();
		SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_startTime = 0;
		SetVisibility(ESlateVisibility::Collapsed);
	}
}
