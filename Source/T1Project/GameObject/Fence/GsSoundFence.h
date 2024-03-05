// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Fence/GsSchemaFenceElement.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Map/Fence/GsSchemaFenceData.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Map/GsSchemaMapTerritoryInfo.h"
#include "GsClientFence.h"
#include "DataSchema/Map/GsSchemaMapTerritorySound.h"

struct T1PROJECT_API FGsSoundFence : public FGsClientFence
{
private:
	const struct FGsSchemaMapTerritorySound* _sound;	

public:
	FGsSoundFence() = default;
	virtual ~FGsSoundFence();

public:
	bool operator== (const FGsSoundFence& inData) const
	{
		return (_sound->territoryId == inData._sound->territoryId);
	}

public:		
	void InitSoundFence(const FGsSchemaMapTerritorySound* inSound, const struct FGsSchemaFenceElement* inFence);
	virtual bool Enter() override;
	virtual bool Leave() override;

public:
	virtual bool IsValid() const override { return _sound && _fence; }
	virtual int64 GetTerritoryID() const override { return _sound->fenceId; }
	virtual TerritoryType GetTerritoryType() const override { return TerritoryType::SOUND; }
	virtual FString GetFenceDescription() override;

public:
	virtual bool IsCanSoundPlay() const { return true; }
	virtual bool TryGetActiveSound(OUT FGsSchemaSoundResDataRow& outSound) const override;

public:		
	const struct FGsSchemaMapTerritorySound* GetSoundData() const { return _sound; }	
	virtual bool HaveActiveCondition() override;
	virtual bool IsActive() const override;	
};
