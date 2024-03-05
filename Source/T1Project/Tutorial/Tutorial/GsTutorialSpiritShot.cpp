// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialSpiritShot.h"
#include "Management/ScopeGame/GsSpiritShotmanager.h"
#include "SpiritShot/GsSpiritShotCategoryData.h"


bool FGsTutorialSpiritShot::OnPrevStart()
{
	FGsSpiritShotManager* spiritShotMgr = GSSpiritShot();
	if (nullptr == spiritShotMgr)
	{
		return false;
	}

	SpiritShotCategoryId firstCategoryId = spiritShotMgr->GetCategoryIdAtIndex(0);
	const FGsSpiritShotCategoryData* categoryData = spiritShotMgr->GetCategoryData(firstCategoryId);
	if (nullptr == categoryData)
	{
		return false;
	}

	if (EGsSpiritShotEnchantState::Completed == categoryData->GetCurrentState())
	{
		return false;
	}

	return Super::OnPrevStart();
}
