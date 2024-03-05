/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/StarLegacy/GsSchemaStarLegacyIdentifyChance.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaStarLegacyConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/StarLegacy/BSGsStarLegacyConfig"))
struct DATACENTER_API FGsSchemaStarLegacyConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="identifyChance"))
	TArray<FGsSchemaStarLegacyIdentifyChance> identifyChance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="autoRouletteMin"))
	int32 autoRouletteMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="autoRouletteMax"))
	int32 autoRouletteMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="룰렛 회전 횟수(n바퀴)", DisplayName="numberOfRouletteSpins"))
	int32 numberOfRouletteSpins;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="한칸 넘어갈때 대기 시간", DisplayName="arrayWaitingTimeToPass"))
	TArray<float> arrayWaitingTimeToPass;
public:
	FGsSchemaStarLegacyConfig();
	
protected:
	bool operator== (const FGsSchemaStarLegacyConfig& __Other) const
	{
		return FGsSchemaStarLegacyConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaStarLegacyConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaStarLegacyConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaStarLegacyConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaStarLegacyConfigRow() : Super()
	{
	}
	
	const FGsSchemaStarLegacyConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaStarLegacyConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaStarLegacyConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaStarLegacyConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaStarLegacyConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableStarLegacyConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("StarLegacy");
	}
	
};
