/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCreatureRaceData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/BSGsCreatureRaceData"))
struct DATACENTER_API FGsSchemaCreatureRaceData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="id 변경금지", Tooltip="CreatureRaceType Enum값과 동일인덱스여야 함"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="raceType"))
	CreatureRaceType raceType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weaknessItemMaterial"))
	TArray<ItemMaterial> weaknessItemMaterial;
public:
	FGsSchemaCreatureRaceData();
	
protected:
	bool operator== (const FGsSchemaCreatureRaceData& __Other) const
	{
		return FGsSchemaCreatureRaceData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCreatureRaceData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCreatureRaceData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCreatureRaceDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCreatureRaceDataRow() : Super()
	{
	}
	
	const FGsSchemaCreatureRaceData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCreatureRaceData* Row = nullptr;
		return Table->FindRow<FGsSchemaCreatureRaceData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCreatureRaceDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCreatureRaceDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCreatureRaceData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CreatureRaceType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCreatureRaceData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCreatureRaceData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->raceType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const CreatureRaceType& InId, OUT const FGsSchemaCreatureRaceData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const CreatureRaceType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaCreatureRaceData* Temp;
		return !FindRowById(FGsSchemaCreatureRaceData().raceType, Temp);
	}
#endif
};
