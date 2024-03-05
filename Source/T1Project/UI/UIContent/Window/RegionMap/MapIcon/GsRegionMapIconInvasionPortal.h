// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconFixed.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsRegionMapIconInvasionPortal.generated.h"

class UGsUIIconRegionMapInvasion;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsRegionMapIconInvasionPortal : public UGsRegionMapIconFixed
{
	GENERATED_BODY()

protected:
	FDateTime _endTIme;

	UPROPERTY()
	UGsUIIconRegionMapInvasion* _invasionIcon;

protected:
	void SetUpdateTime();

public:
	virtual void Tick(float InDelta) override;

	void SetRemainTime(FDateTime endTime);
	void SetActivePortal(bool isactive);
};
