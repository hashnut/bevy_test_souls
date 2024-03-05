/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/GsSchemaEnums.h"
#include "GsSchemaWeatherSchedule.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Area/BSGsWeatherSchedule"))
struct DATACENTER_API FGsSchemaWeatherSchedule
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	int32 id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="기획참고용"))
	FString note;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weatherList", Tooltip))
	TArray<EGsEnvWeather> weatherList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="scheduleList", Tooltip))
	TArray<int32> scheduleList;
	
public:
	FGsSchemaWeatherSchedule();
	
protected:
	bool operator== (const FGsSchemaWeatherSchedule& __Other) const
	{
		return FGsSchemaWeatherSchedule::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaWeatherSchedule::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaWeatherSchedule::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaWeatherScheduleRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaWeatherScheduleRow() : Super()
	{
	}
	
	const FGsSchemaWeatherSchedule* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaWeatherSchedule* Row = nullptr;
		return Table->FindRow<FGsSchemaWeatherSchedule>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaWeatherScheduleRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaWeatherScheduleRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableWeatherSchedule final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/Area");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaWeatherSchedule*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaWeatherSchedule>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaWeatherSchedule*& OutRow) const
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
		const FGsSchemaWeatherSchedule* Temp;
		return !FindRowById(FGsSchemaWeatherSchedule().id, Temp);
	}
#endif
};
