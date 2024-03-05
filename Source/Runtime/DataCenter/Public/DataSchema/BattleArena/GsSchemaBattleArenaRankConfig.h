/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/BattleArena/GsSchemaBattleArenaSeasonData.h"
#include "Runtime/DataCenter/Public/DataSchema/Ranking/GsSchemaRankingTypeObjective.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedRankEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattleArenaRankConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattleArena/BSGsBattleArenaRankConfig"))
struct DATACENTER_API FGsSchemaBattleArenaRankConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rankSearchType"))
	RankSearchType rankSearchType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="seasonDataId"))
	FGsSchemaBattleArenaSeasonDataRow seasonDataId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rankObjectiveId"))
	FGsSchemaRankingTypeObjectiveRow rankObjectiveId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="updateCycleText"))
	FText updateCycleText;
public:
	FGsSchemaBattleArenaRankConfig();
	
protected:
	bool operator== (const FGsSchemaBattleArenaRankConfig& __Other) const
	{
		return FGsSchemaBattleArenaRankConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattleArenaRankConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattleArenaRankConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattleArenaRankConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattleArenaRankConfigRow() : Super()
	{
	}
	
	const FGsSchemaBattleArenaRankConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattleArenaRankConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaBattleArenaRankConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattleArenaRankConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattleArenaRankConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattleArenaRankConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BattleArena");
	}
	
};
