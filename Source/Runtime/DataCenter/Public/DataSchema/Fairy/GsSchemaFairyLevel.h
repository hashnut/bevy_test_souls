/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyLevelInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyLevel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyLevel"))
struct DATACENTER_API FGsSchemaFairyLevel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="minLevel"))
	uint8 minLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="Fairy Max Level", DisplayName="maxLevel"))
	uint8 maxLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="baseStatPassivityId"))
	FGsSchemaPassivitySetRow baseStatPassivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="levelInfoList"))
	TArray<FGsSchemaFairyLevelInfo> levelInfoList;
public:
	FGsSchemaFairyLevel();
	
protected:
	bool operator== (const FGsSchemaFairyLevel& __Other) const
	{
		return FGsSchemaFairyLevel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFairyLevel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFairyLevel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFairyLevelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFairyLevelRow() : Super()
	{
	}
	
	const FGsSchemaFairyLevel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFairyLevel* Row = nullptr;
		return Table->FindRow<FGsSchemaFairyLevel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFairyLevelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFairyLevelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFairyLevel final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Fairy");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaFairyLevel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaFairyLevel>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaFairyLevel*& OutRow) const
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
		const FGsSchemaFairyLevel* Temp;
		return !FindRowById(FGsSchemaFairyLevel().id, Temp);
	}
#endif
};
