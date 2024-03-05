// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialFairy.h"
#include "Summon/GsSummonFunc.h"


bool FGsTutorialFairy::OnPrevStart()
{
	if (false == FGsSummonFunc::CheckFairySummonWaitCount(false, true))
	{
		return false;
	}

	return Super::OnPrevStart();
}
