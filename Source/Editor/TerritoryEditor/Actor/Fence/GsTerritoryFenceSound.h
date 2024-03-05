// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GsTerritoryFenceEnv.h"
#include "DataSchema/Map/Fence/GsSchemaFenceElement.h"
#include "DataSchema/Map/GsSchemaMapTerritorySound.h"
#include "GsTerritoryFenceSound.generated.h"


UCLASS()
class TERRITORYEDITOR_API AGsTerritoryFenceSound : public AGsTerritoryFenceBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, Category = "Territory", meta = (EditCondition = "(_territoryType == TOWN) || (_territoryType == SAFE_ZONE)"))
		TArray<FGsQuestSound> _questSoundArray;

public:
	virtual void InvalidDestription() override;

public:
	void InitFenceSound(const FText& inName, const TArray<FGsQuestSound>& inQuestSoundArray);

	void SetQuestSound(const TArray<FGsQuestSound>& inQuestSoundArray);

public:
	void Export(OUT FGsSchemaMapTerritorySound& outMapSpaceCrack, OUT FGsSchemaFenceElement& outFenceElement);
	void Import(const FGsSchemaMapTerritorySound& inTerritoryInfo, const FGsSchemaFenceElement& inFence);
	void Import(const FGsSchemaMapTerritoryInfo& inTerritoryInfo, const FGsSchemaFenceElement& inFence);

public:
	virtual TerritoryType GetFenceType() override { return TerritoryType::SOUND; }
};
