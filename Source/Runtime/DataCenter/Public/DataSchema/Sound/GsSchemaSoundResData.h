/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSoundResData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Sound/BSGsSoundResData"))
struct DATACENTER_API FGsSchemaSoundResData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EGsSoundPlayType playType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSoftObjectPath path;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float volumeMultiplier;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float pitchMultiplier;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bFollow;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName attachName;
	
public:
	FGsSchemaSoundResData();
	
protected:
	bool operator== (const FGsSchemaSoundResData& __Other) const
	{
		return FGsSchemaSoundResData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSoundResData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSoundResData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSoundResDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSoundResDataRow() : Super()
	{
	}
	
	const FGsSchemaSoundResData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSoundResData* Row = nullptr;
		return Table->FindRow<FGsSchemaSoundResData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSoundResDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSoundResDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSoundResData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Sound");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSoundResData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSoundResData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSoundResData*& OutRow) const
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
		const FGsSchemaSoundResData* Temp;
		return !FindRowById(FGsSchemaSoundResData().id, Temp);
	}
#endif
};
