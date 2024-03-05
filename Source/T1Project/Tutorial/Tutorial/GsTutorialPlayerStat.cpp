// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialPlayerStat.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatHandler.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Stat/GsPlayerStatDistributionController.h"


bool FGsTutorialPlayerStat::OnPrevStart()
{
	FGsPlayerStatDistributionController* statHandler = 
		FGsPlayerStatHandler::GetInstance()->GetStatDistributionController();
	if (nullptr == statHandler)
	{
		return false;
	}

	// 스탯 포인트가 없을 경우 튜토리얼 진행 불가
	if (0 == statHandler->GetCurrentRemainBonusStatPoint())
	{
		return false;
	}	

	return Super::OnPrevStart();
}
