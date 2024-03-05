// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Map/Fence/GsSchemaFenceElement.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Map/Fence/GsSchemaFenceData.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Map/GsSchemaMapTerritoryInfo.h"
#include "Math/Vector2D.h"

/**
 * Object 타겟 Base클래스
 * 현재는 가장 심플하게 구현해둔다.
 */
struct T1PROJECT_API FGsFence : public IGsMessageParam
{
public:
	const struct FGsSchemaFenceElement* _fence;	

public:
	FGsFence() {}
	virtual ~FGsFence() {}

public:
	virtual void Initialize(const struct FGsSchemaMapTerritoryInfo* inMapTerritoryInfo, const struct FGsSchemaFenceElement* inFence) {}
	virtual void Clear() {}
	virtual bool Enter() { return false; }
	virtual bool ReEnter();
	virtual bool Leave() { return false; }

public:
	virtual bool IsValid() const {return nullptr != _fence;}
	virtual FString GetFenceDescription() { return ""; }
	virtual TerritoryType GetTerritoryType() const { return TerritoryType::NONE; }
	virtual bool IsClientFence() { return false; }
	const struct FGsSchemaFenceElement* GetFenceData() { return _fence; }
	virtual int64 GetTerritoryID() const { return 0; }
	virtual int64 GetFenceID() { return _fence->FenceId; }

	//sound
public:
	virtual bool IsCanSoundPlay() const { return false; }
	virtual bool TryGetActiveSound(OUT FGsSchemaSoundResDataRow& outSound) const { return false; }

	//condition
public:
	virtual bool HaveActiveCondition() { return false; }
	virtual bool IsActive() const { return true; }

public:
	virtual bool IsActiveNpcList() const { return false; }
	virtual bool IsSafetyZone() const { return false; }
	virtual bool IsArenaReady() const { return false; }	

public:
	virtual bool IsInFence(const FVector& inLocation) const;
	bool TryGetPointArray(OUT TArray<FVector2D>& outPointArray) const;
	static bool IsInPolygon(const TArray<FVector2D>& inPointList, const FVector2D& inLocation);
	static bool IsInRectangle(const TArray<FVector2D>& inPointList, const FVector2D& inLocation);	

public:
	void Draw(UWorld* inWorld);
};
