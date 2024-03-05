/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaContentsEventNpcPrayer.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="컨텐츠이벤트 - 기도사(인터렉션 버프)", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/ContentsEvent/BSGSContentsEventNpcPrayer"))
struct DATACENTER_API FGsSchemaContentsEventNpcPrayer
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="abnormalitySet"))
	FGsSchemaAbnormalitySetRow abnormalitySet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="npcData"))
	FGsSchemaNpcDataRow npcData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="blessName"))
	FText blessName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="blessDesc"))
	FText blessDesc;
public:
	FGsSchemaContentsEventNpcPrayer();
	
protected:
	bool operator== (const FGsSchemaContentsEventNpcPrayer& __Other) const
	{
		return FGsSchemaContentsEventNpcPrayer::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaContentsEventNpcPrayer::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaContentsEventNpcPrayer::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaContentsEventNpcPrayerRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaContentsEventNpcPrayerRow() : Super()
	{
	}
	
	const FGsSchemaContentsEventNpcPrayer* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaContentsEventNpcPrayer* Row = nullptr;
		return Table->FindRow<FGsSchemaContentsEventNpcPrayer>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaContentsEventNpcPrayerRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaContentsEventNpcPrayerRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableContentsEventNpcPrayer final : public UGsTable
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
		TMap<FName, const FGsSchemaContentsEventNpcPrayer*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaContentsEventNpcPrayer>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaContentsEventNpcPrayer*& OutRow) const
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
		const FGsSchemaContentsEventNpcPrayer* Temp;
		return !FindRowById(FGsSchemaContentsEventNpcPrayer().id, Temp);
	}
#endif
};
