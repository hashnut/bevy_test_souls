// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Fence/GsSchemaFenceElement.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Map/Fence/GsSchemaFenceData.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Map/GsSchemaMapTerritoryInfo.h"
#include "DataSchema/Map/GsSchemaMapTerritoryEnv.h"
#include "DataSchema/Map/GsSchemaMapTerritorySound.h"
#include "DataSchema/Map/GsSchemaMapTerritorySpaceCrack.h"
#include "Math/Vector2D.h"
#include "GsClientFence.h"

struct T1PROJECT_API FGsSpaceCrackFence : public FGsClientFence
{
private:
	const struct FGsSchemaMapTerritorySpaceCrack* _spaceCrack;

public:
	FGsSpaceCrackFence() {}
	virtual ~FGsSpaceCrackFence() {}

public:
	bool operator== (const FGsSpaceCrackFence& inData) const
	{
		return (_spaceCrack->territoryId == inData._spaceCrack->territoryId);
	}

public:
	void InitSpaceCrack(const FGsSchemaMapTerritorySpaceCrack* inSpaceCrack, const struct FGsSchemaFenceElement* inFence);
	virtual bool Enter() override;
	virtual bool Leave() override;

public:
	virtual bool IsValid() const override { return _spaceCrack && _fence; }
	virtual FString GetFenceDescription() override;
	virtual int64 GetTerritoryID() const override { return _spaceCrack->fenceId; }
	virtual TerritoryType GetTerritoryType() const override { return TerritoryType::SPACE_CRACK; }

public:
	virtual bool HaveActiveCondition() override;
	virtual bool IsActive() const override;
	const struct FGsSchemaMapTerritorySpaceCrack* GetSpaceCrackData() const { return _spaceCrack; }
};