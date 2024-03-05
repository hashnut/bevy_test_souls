/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaContentsEventMapPassivity.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="컨텐츠이벤트 - 맵 패시비티(=핫타임). 특정 맵 혹은 전체 맵에 버프 효과를 제공.기존 맵 환경 효과 + 맵 단위 효과(Passivity)", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/ContentsEvent/BSGSContentsEventMapPassivity"))
struct DATACENTER_API FGsSchemaContentsEventMapPassivity
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mapDatas"))
	TArray<FGsSchemaMapDataRow> mapDatas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passivitySets"))
	TArray<FGsSchemaPassivitySetRow> passivitySets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="시작 Cron", DisplayName="startCron"))
	FString startCron;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="유지 시간(분)", DisplayName="durationMinutes"))
	int32 durationMinutes;
public:
	FGsSchemaContentsEventMapPassivity();
	
protected:
	bool operator== (const FGsSchemaContentsEventMapPassivity& __Other) const
	{
		return FGsSchemaContentsEventMapPassivity::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaContentsEventMapPassivity::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaContentsEventMapPassivity::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaContentsEventMapPassivityRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaContentsEventMapPassivityRow() : Super()
	{
	}
	
	const FGsSchemaContentsEventMapPassivity* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaContentsEventMapPassivity* Row = nullptr;
		return Table->FindRow<FGsSchemaContentsEventMapPassivity>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaContentsEventMapPassivityRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaContentsEventMapPassivityRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableContentsEventMapPassivity final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("ContentsEvent");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaContentsEventMapPassivity*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaContentsEventMapPassivity>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaContentsEventMapPassivity*& OutRow) const
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
		const FGsSchemaContentsEventMapPassivity* Temp;
		return !FindRowById(FGsSchemaContentsEventMapPassivity().id, Temp);
	}
#endif
};
