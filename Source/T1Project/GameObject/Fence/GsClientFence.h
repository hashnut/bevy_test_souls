// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Fence/GsSchemaFenceElement.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Map/Fence/GsSchemaFenceData.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Map/GsSchemaMapTerritoryInfo.h"
#include "Math/Vector2D.h"
#include "GsFence.h"

struct T1PROJECT_API FGsClientFence : public FGsFence
{
public:
	FGsClientFence() {}
	virtual ~FGsClientFence() {}			

public:
	virtual bool IsClientFence() override { return true; }
};
