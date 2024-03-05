/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAbnormalityGroupSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/BSGsAbnormalityGroupSet"))
struct DATACENTER_API FGsSchemaAbnormalityGroupSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int32 groupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupDesc"))
	FString groupDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="abnormalityId"))
	TArray<int32> abnormalityId;
public:
	FGsSchemaAbnormalityGroupSet();
	
protected:
	bool operator== (const FGsSchemaAbnormalityGroupSet& __Other) const
	{
		return FGsSchemaAbnormalityGroupSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAbnormalityGroupSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAbnormalityGroupSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAbnormalityGroupSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAbnormalityGroupSetRow() : Super()
	{
	}
	
	const FGsSchemaAbnormalityGroupSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAbnormalityGroupSet* Row = nullptr;
		return Table->FindRow<FGsSchemaAbnormalityGroupSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAbnormalityGroupSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAbnormalityGroupSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAbnormalityGroupSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Abnormality");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaAbnormalityGroupSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaAbnormalityGroupSet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaAbnormalityGroupSet*& OutRow) const
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
		const FGsSchemaAbnormalityGroupSet* Temp;
		return !FindRowById(FGsSchemaAbnormalityGroupSet().groupId, Temp);
	}
#endif
};
