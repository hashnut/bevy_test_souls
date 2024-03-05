// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsFence.h"

struct T1PROJECT_API FGsNoCampZoneFence : public FGsFence
{
public:
	const struct FGsSchemaMapTerritoryInfo* _territory;

public:
	FGsNoCampZoneFence() {}
	virtual ~FGsNoCampZoneFence() {}

public:
	virtual void Initialize(const struct FGsSchemaMapTerritoryInfo* inMapTerritoryInfo, const struct FGsSchemaFenceElement* inFence) override;
	virtual bool IsClientFence() override { return true; }
};
