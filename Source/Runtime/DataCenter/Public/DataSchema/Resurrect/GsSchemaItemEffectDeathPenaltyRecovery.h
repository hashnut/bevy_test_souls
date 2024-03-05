/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEffectDeathPenaltyRecovery.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Resurrect/BSGsItemEffectDeathPenaltyRecovery"))
struct DATACENTER_API FGsSchemaItemEffectDeathPenaltyRecovery
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId"))
	int32 effectId;
public:
	FGsSchemaItemEffectDeathPenaltyRecovery();
	
protected:
	bool operator== (const FGsSchemaItemEffectDeathPenaltyRecovery& __Other) const
	{
		return FGsSchemaItemEffectDeathPenaltyRecovery::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectDeathPenaltyRecovery::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectDeathPenaltyRecovery::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectDeathPenaltyRecoveryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectDeathPenaltyRecoveryRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectDeathPenaltyRecovery* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectDeathPenaltyRecovery* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectDeathPenaltyRecovery>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectDeathPenaltyRecoveryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectDeathPenaltyRecoveryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectDeathPenaltyRecovery final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Resurrect");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEffectDeathPenaltyRecovery*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectDeathPenaltyRecovery>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->effectId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectDeathPenaltyRecovery*& OutRow) const
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
		const FGsSchemaItemEffectDeathPenaltyRecovery* Temp;
		return !FindRowById(FGsSchemaItemEffectDeathPenaltyRecovery().effectId, Temp);
	}
#endif
};
