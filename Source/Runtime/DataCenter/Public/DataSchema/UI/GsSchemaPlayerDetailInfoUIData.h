/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Item/GsStatTypeNameSelector.h"
#include "GsSchemaPlayerDetailInfoUIData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/UI/BSGsPlayerDetailInfoUIData"))
struct DATACENTER_API FGsSchemaPlayerDetailInfoUIData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="weaponType"))
	CreatureWeaponType weaponType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="displayStatType"))
	TArray<FGsStatTypeNameSelector> displayStatType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="개발자 코멘트"))
	FString desc;
public:
	FGsSchemaPlayerDetailInfoUIData();
	
protected:
	bool operator== (const FGsSchemaPlayerDetailInfoUIData& __Other) const
	{
		return FGsSchemaPlayerDetailInfoUIData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPlayerDetailInfoUIData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPlayerDetailInfoUIData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPlayerDetailInfoUIDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPlayerDetailInfoUIDataRow() : Super()
	{
	}
	
	const FGsSchemaPlayerDetailInfoUIData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPlayerDetailInfoUIData* Row = nullptr;
		return Table->FindRow<FGsSchemaPlayerDetailInfoUIData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPlayerDetailInfoUIDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPlayerDetailInfoUIDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePlayerDetailInfoUIData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CreatureWeaponType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("UI");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaPlayerDetailInfoUIData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPlayerDetailInfoUIData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->weaponType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const CreatureWeaponType& InId, OUT const FGsSchemaPlayerDetailInfoUIData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const CreatureWeaponType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaPlayerDetailInfoUIData* Temp;
		return !FindRowById(FGsSchemaPlayerDetailInfoUIData().weaponType, Temp);
	}
#endif
};
