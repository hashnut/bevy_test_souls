/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDungeonData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Dungeon/BSGsDungeonData"))
struct DATACENTER_API FGsSchemaDungeonData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="dungeonCategory"))
	DungeonCategory dungeonCategory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapId"))
	FGsSchemaMapDataRow mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="indexValue"))
	int32 indexValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="visible"))
	bool visible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="즉시 이동 시 도착할 좌표스팟 ID", DisplayName="spotId"))
	int32 spotId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="던전 입장 제한 레벨 - 입력된 숫자와 동일 레벨이 되면 입장가능", DisplayName="unlockLevel"))
	int32 unlockLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="몬스터 레벨 보정치 - 몬스터의 레벨을 이 값만큼 증가시켜 스폰 함", DisplayName="addNpcLevel"))
	int32 addNpcLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="협동 파티 던전에서 사용 할 던전 난이도", DisplayName="levelOfDifficulty"))
	DungeonDifficulty levelOfDifficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spotIdList"))
	TArray<int32> spotIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hiddenSpotIdList"))
	TArray<int32> hiddenSpotIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="basicRewardList"))
	TArray<FGsSchemaRewardDataRow> basicRewardList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mainRewardList"))
	TArray<FGsSchemaRewardDataRow> mainRewardList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bossId"))
	FGsSchemaNpcDataRow bossId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="timeLimitM"))
	int32 timeLimitM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isRandomRewardNormal"))
	bool isRandomRewardNormal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isRandomRewardElite"))
	bool isRandomRewardElite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isRandomRewardNamed"))
	bool isRandomRewardNamed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="randomHitNamedRewardList"))
	TArray<FGsSchemaRewardDataRow> randomHitNamedRewardList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="randomHitBossRewardList"))
	TArray<FGsSchemaRewardDataRow> randomHitBossRewardList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="lastHitNamedRewardList"))
	TArray<FGsSchemaRewardDataRow> lastHitNamedRewardList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="lastHitBossRewardList"))
	TArray<FGsSchemaRewardDataRow> lastHitBossRewardList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isRandomRewardDesc"))
	FText isRandomRewardDesc;
public:
	FGsSchemaDungeonData();
	
protected:
	bool operator== (const FGsSchemaDungeonData& __Other) const
	{
		return FGsSchemaDungeonData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaDungeonData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaDungeonData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaDungeonDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaDungeonDataRow() : Super()
	{
	}
	
	const FGsSchemaDungeonData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaDungeonData* Row = nullptr;
		return Table->FindRow<FGsSchemaDungeonData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaDungeonDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaDungeonDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableDungeonData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Dungeon");
	}
	
};
