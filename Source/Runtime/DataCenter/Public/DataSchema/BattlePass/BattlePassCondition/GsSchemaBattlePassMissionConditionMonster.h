/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassMissionConditionMonster.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattlePass/BattlePassCondition/BSGsBattlePassMissionConditionMonster"))
struct DATACENTER_API FGsSchemaBattlePassMissionConditionMonster
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="monsterId"))
	int32 monsterId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapId"))
	int32 mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="monsterRace"))
	CreatureRaceType monsterRace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcFunction"))
	NpcFunctionType npcFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="monsterGradeMin"))
	CreatureRankType monsterGradeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="monsterGradeMax"))
	CreatureRankType monsterGradeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contributeRank"))
	int32 contributeRank;
public:
	FGsSchemaBattlePassMissionConditionMonster();
	
protected:
	bool operator== (const FGsSchemaBattlePassMissionConditionMonster& __Other) const
	{
		return FGsSchemaBattlePassMissionConditionMonster::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattlePassMissionConditionMonster::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattlePassMissionConditionMonster::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattlePassMissionConditionMonsterRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattlePassMissionConditionMonsterRow() : Super()
	{
	}
	
	const FGsSchemaBattlePassMissionConditionMonster* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattlePassMissionConditionMonster* Row = nullptr;
		return Table->FindRow<FGsSchemaBattlePassMissionConditionMonster>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattlePassMissionConditionMonsterRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattlePassMissionConditionMonsterRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattlePassMissionConditionMonster final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BattlePass/BattlePassCondition");
	}
	
};
