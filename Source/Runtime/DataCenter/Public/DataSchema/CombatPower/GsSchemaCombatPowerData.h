/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/GameObject/Define/EGsStatFormatType.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCombatPowerData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/CombatPower/BSGsCombatPowerData"))
struct DATACENTER_API FGsSchemaCombatPowerData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType"))
	FGsStatTypeNameSelector statType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weaponType"))
	TArray<CreatureWeaponType> weaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="true면 확률, 빈값 이면 정수", DisplayName="makeFormat"))
	EGsStatFormatType makeFormat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="uiOrderNum"))
	int32 uiOrderNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="combatPowerValue"))
	int32 combatPowerValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	int32 name;
public:
	FGsSchemaCombatPowerData();
	
protected:
	bool operator== (const FGsSchemaCombatPowerData& __Other) const
	{
		return FGsSchemaCombatPowerData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCombatPowerData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCombatPowerData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCombatPowerDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCombatPowerDataRow() : Super()
	{
	}
	
	const FGsSchemaCombatPowerData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCombatPowerData* Row = nullptr;
		return Table->FindRow<FGsSchemaCombatPowerData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCombatPowerDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCombatPowerDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCombatPowerData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<FGsStatTypeNameSelector, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("CombatPower");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCombatPowerData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCombatPowerData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->statType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const FGsStatTypeNameSelector& InId, OUT const FGsSchemaCombatPowerData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const FGsStatTypeNameSelector& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaCombatPowerData* Temp;
		return !FindRowById(FGsSchemaCombatPowerData().statType, Temp);
	}
#endif
};
