/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAutoDepotConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BSGsAutoDepotConfig"))
struct DATACENTER_API FGsSchemaAutoDepotConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int32 groupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="groupName"))
	FText groupName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="groupDesc"))
	FText groupDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="groupIcon"))
	FSoftObjectPath groupIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="itemIdList"))
	TArray<FGsSchemaItemCommonRow> itemIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="minGrade"))
	ItemGrade minGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxGrade"))
	ItemGrade maxGrade;
public:
	FGsSchemaAutoDepotConfig();
	
protected:
	bool operator== (const FGsSchemaAutoDepotConfig& __Other) const
	{
		return FGsSchemaAutoDepotConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAutoDepotConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAutoDepotConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAutoDepotConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAutoDepotConfigRow() : Super()
	{
	}
	
	const FGsSchemaAutoDepotConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAutoDepotConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaAutoDepotConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAutoDepotConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAutoDepotConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAutoDepotConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaAutoDepotConfig*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaAutoDepotConfig>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->groupId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaAutoDepotConfig*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaAutoDepotConfig* Temp;
		return !FindRowById(FGsSchemaAutoDepotConfig().groupId, Temp);
	}
#endif
};
