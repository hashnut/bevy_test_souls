/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Common/GsSchemaCurrencyPairData.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaServerMigrateConfigData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BMShop/ServerMigrate/BSGsServerMigrateConfigData"))
struct DATACENTER_API FGsSchemaServerMigrateConfigData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="totalCount"))
	int32 totalCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="accountCostSet"))
	TArray<FGsSchemaCurrencyPairData> accountCostSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="includeGuildCostSet"))
	TArray<FGsSchemaCurrencyPairData> includeGuildCostSet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="limitCharacterLevel"))
	int32 limitCharacterLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="limitGuildLevel"))
	int32 limitGuildLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="accountHelpContents"))
	EGsHelpContents accountHelpContents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="guildHelpContents"))
	EGsHelpContents guildHelpContents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="serverConditionRate"))
	int32 serverConditionRate;
public:
	FGsSchemaServerMigrateConfigData();
	
protected:
	bool operator== (const FGsSchemaServerMigrateConfigData& __Other) const
	{
		return FGsSchemaServerMigrateConfigData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaServerMigrateConfigData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaServerMigrateConfigData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaServerMigrateConfigDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaServerMigrateConfigDataRow() : Super()
	{
	}
	
	const FGsSchemaServerMigrateConfigData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaServerMigrateConfigData* Row = nullptr;
		return Table->FindRow<FGsSchemaServerMigrateConfigData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaServerMigrateConfigDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaServerMigrateConfigDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableServerMigrateConfigData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BMShop/ServerMigrate");
	}
	
};
