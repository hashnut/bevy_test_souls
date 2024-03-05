/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaServerMigrateConditionData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BMShop/ServerMigrate/BSGsServerMigrateConditionData"))
struct DATACENTER_API FGsSchemaServerMigrateConditionData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	ServerMigrateConditionType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionText"))
	FText conditionText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="failDesc"))
	FText failDesc;
public:
	FGsSchemaServerMigrateConditionData();
	
protected:
	bool operator== (const FGsSchemaServerMigrateConditionData& __Other) const
	{
		return FGsSchemaServerMigrateConditionData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaServerMigrateConditionData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaServerMigrateConditionData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaServerMigrateConditionDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaServerMigrateConditionDataRow() : Super()
	{
	}
	
	const FGsSchemaServerMigrateConditionData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaServerMigrateConditionData* Row = nullptr;
		return Table->FindRow<FGsSchemaServerMigrateConditionData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaServerMigrateConditionDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaServerMigrateConditionDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableServerMigrateConditionData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<ServerMigrateConditionType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BMShop/ServerMigrate");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaServerMigrateConditionData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaServerMigrateConditionData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->type, Row.Key);
		}
	}
	
public:
	bool FindRowById(const ServerMigrateConditionType& InId, OUT const FGsSchemaServerMigrateConditionData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const ServerMigrateConditionType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaServerMigrateConditionData* Temp;
		return !FindRowById(FGsSchemaServerMigrateConditionData().type, Temp);
	}
#endif
};
