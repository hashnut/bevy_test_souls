// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Fence/GsSchemaFenceElement.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Map/Fence/GsSchemaFenceData.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Map/GsSchemaMapTerritoryInfo.h"
#include "Math/Vector2D.h"
#include "GsClientFence.h"
#include "DataSchema/Map/GsSchemaMapTerritoryEnv.h"


struct T1PROJECT_API FGsEnvFence : public FGsClientFence
{
private:
	const struct FGsSchemaMapTerritoryEnv* _env;	

public:
	FGsEnvFence() {}
	virtual ~FGsEnvFence() {}

public:
	bool operator== (const FGsEnvFence& inData) const
	{
		return (_env->territoryId == inData._env->territoryId);
	}

public:
	virtual bool Enter() override;
	virtual bool Leave() override;

public:
	void InitEnvFence(const FGsSchemaMapTerritoryEnv* inEnv, const struct FGsSchemaFenceElement* inFence);
	virtual bool IsValid() const override { return _env && _fence; }	
	virtual FString GetFenceDescription() override;
	virtual int64 GetTerritoryID() const override { return _env->fenceId; }
	virtual TerritoryType GetTerritoryType() const override { return TerritoryType::Env; }
	virtual bool IsInFence(const FVector& inLocation) const override;

public:
	const struct FGsSchemaMapTerritoryEnv* GetEnv() const { return _env; }
	virtual bool HaveActiveCondition() override;
	virtual bool IsActive() const override;
	bool TryGetActiveEnv(OUT EGsEnvEvent& inEvent) const;
};
