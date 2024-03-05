/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/InterServer/World/GsSchemaWorldData.h"
#include "GsSchemaWorldGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/World/BSGsWorldGroup"))
struct DATACENTER_API FGsSchemaWorldGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="worldGroupId"))
	int32 worldGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="worldGroupName"))
	FString worldGroupName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="worldList"))
	TArray<FGsSchemaWorldDataRow> worldList;
public:
	FGsSchemaWorldGroup();
	
protected:
	bool operator== (const FGsSchemaWorldGroup& __Other) const
	{
		return FGsSchemaWorldGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWorldGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWorldGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWorldGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWorldGroupRow() : Super()
	{
	}
	
	const FGsSchemaWorldGroup* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWorldGroup* Row = nullptr;
		return Table->FindRow<FGsSchemaWorldGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWorldGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWorldGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWorldGroup final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("InterServer/World");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaWorldGroup*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaWorldGroup>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->worldGroupId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaWorldGroup*& OutRow) const
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
		const FGsSchemaWorldGroup* Temp;
		return !FindRowById(FGsSchemaWorldGroup().worldGroupId, Temp);
	}
#endif
};
