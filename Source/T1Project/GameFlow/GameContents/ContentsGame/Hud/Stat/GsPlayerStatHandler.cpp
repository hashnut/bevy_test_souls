// Fill out your copyright notice in the Description page of Project Settings.


#include "GsPlayerStatHandler.h"
#include "GsPlayerStatDistributionController.h"
#include "GsPlayerBookmarkStatController.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Stat/GsStatHelper.h"

void FGsPlayerStatHandler::Init()
{
	_distributionStatHandler = TUniquePtr<FGsPlayerStatDistributionController>(new FGsPlayerStatDistributionController());
	_distributionStatHandler->Init();

	_bookmarkStatHandler = TUniquePtr<FGsPlayerBookmarkStatController>(new FGsPlayerBookmarkStatController());
	_bookmarkStatHandler->Init();
}

void FGsPlayerStatHandler::Close()
{
	_distributionStatHandler->Close();
	_distributionStatHandler.Reset();

	_bookmarkStatHandler->Close();
	_bookmarkStatHandler.Reset();
}

void FGsPlayerStatHandler::Reset()
{

}

bool FGsPlayerStatHandler::TryGetMainStat(OUT TMap<StatType, const FGsStatInfo*>& outStatMap)
{
	outStatMap.Empty();
	const FGsStatInfo* stat = nullptr;
	for (StatType statType : MAIN_STAT_ARRAY)
	{
		stat = GetPlayerStat(statType);
		if (stat)
		{
			outStatMap.Emplace(statType, stat);
		}		
	}

	return outStatMap.Num() > 0;
}

class FGsPlayerStatDistributionController* FGsPlayerStatHandler::GetStatDistributionController()
{
	return _distributionStatHandler.Get();
}

class FGsPlayerBookmarkStatController* FGsPlayerStatHandler::GetBookmarkStatController()
{
	return _bookmarkStatHandler.Get();
}

const FGsStatInfo* FGsPlayerStatHandler::GetPlayerStat(StatType inStatType)
{
	UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (nullptr == localPlayer)
	{
		return nullptr;
	}
	
	return localPlayer->CastGameObject<UGsGameObjectLocalPlayer>()->GetCreatureStatInfoByType(inStatType);
}

bool FGsPlayerStatHandler::TryGetPlayerStatText(StatType inStatType, OUT FText& outPlayerText)
{
	const FGsStatInfo* statInfo = GetPlayerStat(inStatType);
	if (nullptr == statInfo)
	{
		return false;
	}

	FText statValueText;
	if (false == FGsStatHelper::GetDisPlayStatValueWithMarkFormat(inStatType, statInfo->GetStatValue(), statValueText))
	{
		FString statName = FGsTextUtil::GetStringFromEnum(TEXT("StatType"), inStatType);
		GSLOG(Error, TEXT("Stat format not exist - stat name : %s"), *statName);
		return false;
	}

	outPlayerText = statValueText;

	return true;
}

FGsPlayerStatHandler* FGsPlayerStatHandler::GetInstance()
{
	//update stat distribution handler
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return nullptr;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return nullptr;
	}

	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return nullptr;
	}

	return hud->GetPlayerStatHandler();
}
