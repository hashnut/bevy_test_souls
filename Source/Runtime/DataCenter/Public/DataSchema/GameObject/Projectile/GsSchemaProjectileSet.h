/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Common/GsSchemaScreenFilterInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Projectile/GsSchemaProjectileActionInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Projectile/GsSchemaProjectileCollisionInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillTargetFilter.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaShapeData.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedProjectileEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UGsHitEffectData;
#include "GsSchemaProjectileSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Projectile/BSGsProjectileSet"))
struct DATACENTER_API FGsSchemaProjectileSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기획용", DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="발사체 스폰 타입 (캐스터 인접 위치, 타겟 위치, 랜덤 위치 등등...)", DisplayName="spawnType"))
	ProjectileSpawnType spawnType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="발사체 이동 타입 (고정형, 등속 직선 이동형, 가속 직선 이동형, 등속 선형 이동형 등등...)", DisplayName="moveType"))
	ProjectileMoveType moveType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="endTriggerType"))
	ProjectileTriggerType endTriggerType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최대 수명", DisplayName="lifeTime"))
	float lifeTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="속도", DisplayName="speed"))
	int32 speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="최대 히트 수", DisplayName="maxHitCount"))
	int32 maxHitCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="외형, 판정영역 모두에 영향을 주는 Scale", DisplayName="scale"))
	float scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스킬 효과의 collisionInfoList 를 참조할 것인지, 발사체의 collisionInfoList 를 참조할 것인지 여부", DisplayName="useSkillNotifyCollision"))
	bool useSkillNotifyCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="useTargetCollision"))
	bool useTargetCollision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="액션 기본 정보", DisplayName="actionInfoList"))
	TArray<FGsSchemaProjectileActionInfo> actionInfoList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="collisionInfoList"))
	TArray<FGsSchemaProjectileCollisionInfo> collisionInfoList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="targetFilterInfo"))
	FGsSchemaSkillTargetFilter targetFilterInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="외형 정보", DisplayName="shapeId"))
	FGsSchemaShapeDataRow shapeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="hiteffectData"))
	UGsHitEffectData* hiteffectData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hitSoundData"))
	FGsSchemaSoundResDataRow hitSoundData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invisibleEnemy"))
	bool invisibleEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="screenFilter"))
	FGsSchemaScreenFilterInfo screenFilter;
public:
	FGsSchemaProjectileSet();
	
protected:
	bool operator== (const FGsSchemaProjectileSet& __Other) const
	{
		return FGsSchemaProjectileSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaProjectileSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaProjectileSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaProjectileSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaProjectileSetRow() : Super()
	{
	}
	
	const FGsSchemaProjectileSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaProjectileSet* Row = nullptr;
		return Table->FindRow<FGsSchemaProjectileSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaProjectileSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaProjectileSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableProjectileSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/Projectile");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaProjectileSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaProjectileSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaProjectileSet*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaProjectileSet* Temp;
		return !FindRowById(FGsSchemaProjectileSet().id, Temp);
	}
#endif
};
