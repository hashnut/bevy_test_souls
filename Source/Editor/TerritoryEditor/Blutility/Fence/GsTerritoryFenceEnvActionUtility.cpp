// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryFenceEnvActionUtility.h"
#include "Actor/Fence/GsTerritoryFenceEnv.h"


void UGsTerritoryFenceEnvActionUtility::EditEnv(const TArray<FGsEnvCondition>& inEnvConditionArray)
{
	AGsTerritoryFenceEnv* fence = Cast<AGsTerritoryFenceEnv>(GetSelectedFence());
	if (nullptr == fence)
	{
		return;
	}

	fence->SetEnvConditionArray(inEnvConditionArray);
}
