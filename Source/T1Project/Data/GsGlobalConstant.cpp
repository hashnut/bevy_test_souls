// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGlobalConstant.h"

UGsGlobalConstant::UGsGlobalConstant()
	: GlobalCoolTime(1.0f)
{
	AnimUpdateRateVisibleMaxDistanceFactor.Add(0.24f);
	AnimUpdateRateVisibleMaxDistanceFactor.Add(0.12f);

	LODToFrameSkipMap.FindOrAdd(0) = 0;
	LODToFrameSkipMap.FindOrAdd(1) = 1;
	LODToFrameSkipMap.FindOrAdd(2) = 2;
	LODToFrameSkipMap.FindOrAdd(3) = 3;
	LODToFrameSkipMap.FindOrAdd(4) = 4;
}