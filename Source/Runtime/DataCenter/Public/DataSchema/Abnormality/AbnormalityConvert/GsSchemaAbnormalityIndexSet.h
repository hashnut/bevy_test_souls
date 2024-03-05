/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/AbnormalityConvert/GsSchemaAbnormalityIndexLevelInfo.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAbnormalityIndexSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Abnormality/AbnormalityConvert/BSGsAbnormalityIndexSet"))
struct DATACENTER_API FGsSchemaAbnormalityIndexSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="AbnormalitySet Id와 동일", DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectType"))
	AbnormalityEffectType effectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelList"))
	TArray<FGsSchemaAbnormalityIndexLevelInfo> levelList;
public:
	FGsSchemaAbnormalityIndexSet();
	
protected:
	bool operator== (const FGsSchemaAbnormalityIndexSet& __Other) const
	{
		return FGsSchemaAbnormalityIndexSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAbnormalityIndexSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAbnormalityIndexSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAbnormalityIndexSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAbnormalityIndexSetRow() : Super()
	{
	}
	
	const FGsSchemaAbnormalityIndexSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAbnormalityIndexSet* Row = nullptr;
		return Table->FindRow<FGsSchemaAbnormalityIndexSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAbnormalityIndexSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAbnormalityIndexSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAbnormalityIndexSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Abnormality/AbnormalityConvert");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaAbnormalityIndexSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaAbnormalityIndexSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaAbnormalityIndexSet*& OutRow) const
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
		const FGsSchemaAbnormalityIndexSet* Temp;
		return !FindRowById(FGsSchemaAbnormalityIndexSet().id, Temp);
	}
#endif
};
