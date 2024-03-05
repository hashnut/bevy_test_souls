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

/**
 * Object 타겟 Base클래스
 * 현재는 가장 심플하게 구현해둔다.
 */
struct T1PROJECT_API FGsServerFence : public FGsFence
{
public:
	const struct FGsSchemaMapTerritoryInfo* _territory;	

public:
	FGsServerFence() = default;
	virtual ~FGsServerFence();

public:
	virtual void Initialize(const struct FGsSchemaMapTerritoryInfo* inMapTerritoryInfo, const struct FGsSchemaFenceElement* inFence) override;
	virtual void Clear() override;
	virtual bool Enter() override;
	virtual bool ReEnter() override;
	virtual bool Leave() override;

public:
	virtual bool IsValid() const override { return _territory && _fence; }
	virtual TerritoryType GetTerritoryType() const override;
	virtual FString GetFenceDescription() override;
	virtual bool IsClientFence() { return false; }		
	virtual int64 GetTerritoryID() const override { return _territory->fenceId; }
	const struct FGsSchemaMapTerritoryInfo* GetTerritoryData() const { return _territory; }	
	
protected:	
	virtual bool TryGetActiveSound(OUT FGsSchemaSoundResDataRow& outSound) const override;

public:
	bool operator== (const FGsServerFence& inData) const
	{
		return (_territory->territoryId == inData._territory->territoryId);
	}

public:
	bool IsActiveNpcList() const;
	bool IsSafetyZone() const;
	bool IsArenaReady() const;
};
