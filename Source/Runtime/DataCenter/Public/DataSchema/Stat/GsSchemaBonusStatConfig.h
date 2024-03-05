/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Stat/GsSchemaBonusStatItemConsumeMaxExtend.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBonusStatConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Stat/BSGsBonusStatConfig"))
struct DATACENTER_API FGsSchemaBonusStatConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bonusStatDistributionMax"))
	int32 bonusStatDistributionMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bonusStatItem"))
	int32 bonusStatItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resetStatItem"))
	int32 resetStatItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bonusStatItemConsumeMax"))
	int32 bonusStatItemConsumeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bonusStatItemConsumeMaxExtendList"))
	TArray<FGsSchemaBonusStatItemConsumeMaxExtend> bonusStatItemConsumeMaxExtendList;
public:
	FGsSchemaBonusStatConfig();
	
protected:
	bool operator== (const FGsSchemaBonusStatConfig& __Other) const
	{
		return FGsSchemaBonusStatConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBonusStatConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBonusStatConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBonusStatConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBonusStatConfigRow() : Super()
	{
	}
	
	const FGsSchemaBonusStatConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBonusStatConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaBonusStatConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBonusStatConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBonusStatConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBonusStatConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Stat");
	}
	
};
