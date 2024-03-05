// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Fence/GsSchemaFenceElement.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Map/Fence/GsSchemaFenceData.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Map/GsSchemaMapTerritoryInfo.h"
#include "GsServerFence.h"
#include "Message/GsMessageInvasion.h"

struct T1PROJECT_API FGsTownChaosFence : public FGsServerFence
{
private:
	bool _isChaos;
	TArray<MsgInvasionHandle> _messageInvasionList;

public:
	FGsTownChaosFence();
	virtual ~FGsTownChaosFence();

public:
	virtual TerritoryType GetTerritoryType() const override { return TerritoryType::TOWN_CHAOS; }

public:
	bool operator== (const FGsTownChaosFence& inData) const
	{
		return (_territory->territoryId == inData._territory->territoryId);
	}
	void SetChaos(bool inChaos);
	bool IsChaos() const { return _isChaos; }

public:
	virtual void Initialize(const struct FGsSchemaMapTerritoryInfo* inMapTerritoryInfo, const struct FGsSchemaFenceElement* inFence) override;
	virtual bool Enter() override;
	virtual bool Leave() override;

public:
	void OnActiveChaosFence(const struct IGsMessageParam* InParam);
	void OnDeactiveChaosFence(const struct IGsMessageParam* InParam);
	void OnChangeChaosFence(const struct IGsMessageParam* InParam);
};
