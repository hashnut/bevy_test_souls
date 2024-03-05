/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/BMShop/ServerMigrate/GsSchemaServerMigrateConditionData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaServerMigrateGroupData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BMShop/ServerMigrate/BSGsServerMigrateGroupData"))
struct DATACENTER_API FGsSchemaServerMigrateGroupData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	ServerMigrateType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="sortNum"))
	int32 sortNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="conditionDataSet"))
	TArray<FGsSchemaServerMigrateConditionDataRow> conditionDataSet;
public:
	FGsSchemaServerMigrateGroupData();
	
protected:
	bool operator== (const FGsSchemaServerMigrateGroupData& __Other) const
	{
		return FGsSchemaServerMigrateGroupData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaServerMigrateGroupData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaServerMigrateGroupData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaServerMigrateGroupDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaServerMigrateGroupDataRow() : Super()
	{
	}
	
	const FGsSchemaServerMigrateGroupData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaServerMigrateGroupData* Row = nullptr;
		return Table->FindRow<FGsSchemaServerMigrateGroupData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaServerMigrateGroupDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaServerMigrateGroupDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableServerMigrateGroupData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BMShop/ServerMigrate");
	}
	
};
