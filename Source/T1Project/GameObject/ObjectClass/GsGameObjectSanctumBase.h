// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameObject/ObjectClass/GsGameObjectNonPlayerBase.h"
#include "GameObject/Interaction/GsGameObjectInteractionInterface.h"
#include "Message/MessageParam/GsInteractionMessageParam.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "GsGameObjectSanctumBase.generated.h"

class AGsCampBase;
class FGsMovementHandlerBase;
class FGsAbnormalityHandlerBase;
struct FGsSchemaSanctumConfig;
struct FGsSchemaSanctumData;
struct FGsSchemaSanctumNexusData;
struct FGsSchemaCreatureCommonInfo;

/**
 * 성소 건설 ~ 건설 완료
 */
UCLASS(Abstract)
class T1PROJECT_API UGsGameObjectSanctumBase : public UGsGameObjectNonPlayerBase, public IGsGameObjectInteractionInterface
{
	GENERATED_BODY()

protected:
	FGsInteractionMessageParam _interactionMessageParam;
	bool _isInteractionActive = false; // 서버에서 주는 정보. 점령전 중인가
	SanctumAreaId _sanctumId = INVALID_SANCTUM_AREA_ID;	
	SPAWN_REASON _spawnReasonType = SPAWN_REASON::NORMAL;
	FRotator _spawnRot = FRotator::ZeroRotator;

	// 편의정보. 스캔 등에서 가져갈 이름
	FString _strFormattedName;

public:
	virtual ~UGsGameObjectSanctumBase() = default;
	
public:
	virtual void Initialize(const struct FGsSpawnParam* SpawnParam) override;
	//virtual void InitializeActor(AActor * Owner) override;

	
public:
	virtual void OnIFFChanged() override;

	// UGsGameObjectCreature override
protected:
	virtual TSharedPtr<FGsMovementHandlerBase> CreateMovement() override;
	virtual TSharedPtr<FGsAbnormalityHandlerBase> CreateAbnormality() override;

	// IGsGameObjectInteractionInterface override
public:
	virtual void SetInteractionActive(bool In_isActive, int In_index = 0) override {}
	virtual bool GetIsActiveInteraction() override;
	virtual void OnClickInteract(bool In_isByWidget) override {}

public:
	virtual EGsGameObjectType GetObjectType() const override { return EGsGameObjectType::Sanctum; }
	virtual FText GetNpcName() override { return FText::FromString(_strFormattedName); }

public:
	virtual void OnSanctumSataus(SanctumAreaId InSanctumId, bool bInCanInteract) {}
	virtual void AddInteractionGameId(int64 InGameId) {}
	virtual void RemoveInteractionGameId(int64 InGameId) {}
	virtual void UpdateSanctumName(SanctumAreaId InSanctumId) {}
	virtual void UpdateInteractionState() {}
	virtual void OnBattleEnd() {}
	virtual bool IsNexus() const { return false; }
	virtual GuildDBId GetGuildDBId() const { return INVALID_GUILD_DB_ID; }
	virtual GuildEmblemId GetGuildEmblemId() const { return INVALID_GUILD_EMBLEM_ID; }

public:
	void SetSpawnReasonType(const SPAWN_REASON InType) { _spawnReasonType = InType; }
	void SetCapsuleAndTracePos(AGsCampBase* InActorCamp, const FVector& InSpawnPos, bool bCapsuleActive, 
		const FGsSchemaCreatureCommonInfo* InCreatureCommonInfo);

	FString GetFormattedSanctumName() const { return _strFormattedName; }

public:
	SanctumAreaId GetSanctumId() const { return _sanctumId; }
	int32 GetInteractionRange() const;

	// 공격 가능 거리에서 네비 지형 체크 스킵
	virtual bool IsSkipTerrainCheck() const override { return true; }
};
