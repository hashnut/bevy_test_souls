/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "GsSchemaItemEffectRestoreStat.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Stat/BSGsItemEffectRestoreStat"))
struct DATACENTER_API FGsSchemaItemEffectRestoreStat
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId", Tooltip))
	int32 effectId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="damageStatType", Tooltip))
	DamageStatType damageStatType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="value", Tooltip))
	int32 value;
	
public:
	FGsSchemaItemEffectRestoreStat();
	
protected:
	bool operator== (const FGsSchemaItemEffectRestoreStat& __Other) const
	{
		return FGsSchemaItemEffectRestoreStat::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectRestoreStat::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectRestoreStat::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectRestoreStatRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectRestoreStatRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectRestoreStat* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectRestoreStat* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectRestoreStat>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectRestoreStatRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectRestoreStatRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectRestoreStat final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Stat");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEffectRestoreStat*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectRestoreStat>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectRestoreStat*& OutRow) const
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
		const FGsSchemaItemEffectRestoreStat* Temp;
		return !FindRowById(FGsSchemaItemEffectRestoreStat().effectId, Temp);
	}
#endif
};
