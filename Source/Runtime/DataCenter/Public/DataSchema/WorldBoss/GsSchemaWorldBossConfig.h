/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Mail/GsSchemaMailType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaWorldBossConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/WorldBoss/BSGsWorldBossConfig"))
struct DATACENTER_API FGsSchemaWorldBossConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="basicRewardMailType"))
	FGsSchemaMailTypeRow basicRewardMailType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rankRewardMailType"))
	FGsSchemaMailTypeRow rankRewardMailType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="lastHitRewardMailType"))
	FGsSchemaMailTypeRow lastHitRewardMailType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="consolationRewardMailType"))
	FGsSchemaMailTypeRow consolationRewardMailType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="DailyMaxRewardEarned"))
	int32 DailyMaxRewardEarned;
public:
	FGsSchemaWorldBossConfig();
	
protected:
	bool operator== (const FGsSchemaWorldBossConfig& __Other) const
	{
		return FGsSchemaWorldBossConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWorldBossConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWorldBossConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWorldBossConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWorldBossConfigRow() : Super()
	{
	}
	
	const FGsSchemaWorldBossConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWorldBossConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaWorldBossConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWorldBossConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWorldBossConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWorldBossConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("WorldBoss");
	}
	
};
