/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/SpaceCrack/GsSchemaSpaceCrackConfigBossPortal.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/SpaceCrack/GsSchemaSpaceCrackConfigDeponMonsterGroup.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/SpaceCrack/GsSchemaSpaceCrackConfigPatternPortal.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSpaceCrackConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/SpaceCrack/BSGsSpaceCrackConfig"))
struct DATACENTER_API FGsSchemaSpaceCrackConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="deponMonster"))
	FGsSchemaSpaceCrackConfigDeponMonsterGroup deponMonster;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="wavePatternPortalList"))
	TArray<FGsSchemaSpaceCrackConfigPatternPortal> wavePatternPortalList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bossPortalList"))
	TArray<FGsSchemaSpaceCrackConfigBossPortal> bossPortalList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="activationStartTimeCron"))
	FString activationStartTimeCron;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="durationTime"))
	int32 durationTime;
public:
	FGsSchemaSpaceCrackConfig();
	
protected:
	bool operator== (const FGsSchemaSpaceCrackConfig& __Other) const
	{
		return FGsSchemaSpaceCrackConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSpaceCrackConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSpaceCrackConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSpaceCrackConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSpaceCrackConfigRow() : Super()
	{
	}
	
	const FGsSchemaSpaceCrackConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSpaceCrackConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaSpaceCrackConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSpaceCrackConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSpaceCrackConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSpaceCrackConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/SpaceCrack");
	}
	
};
