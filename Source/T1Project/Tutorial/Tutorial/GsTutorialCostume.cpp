// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialCostume.h"
#include "Summon/GsSummonFunc.h"


bool FGsTutorialCostume::OnPrevStart()
{
	if (false == FGsSummonFunc::CheckCostumeSummonWaitCount(false, true))
	{
		return false;
	}

	return Super::OnPrevStart();
}
