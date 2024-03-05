/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Stat/GsSchemaStatInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcDropExpGoldCorrection.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcCorrectionStatData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Stat/BSGsNpcCorrectionStatData"))
struct DATACENTER_API FGsSchemaNpcCorrectionStatData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statCorrectionType"))
	StatCorrectionType statCorrectionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statCalculationType"))
	StatCalcType statCalculationType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="value"))
	int32 value;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statInfo"))
	TArray<FGsSchemaStatInfo> statInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dropExpGoldCorrection"))
	FGsSchemaNpcDropExpGoldCorrectionRow dropExpGoldCorrection;
public:
	FGsSchemaNpcCorrectionStatData();
	
protected:
	bool operator== (const FGsSchemaNpcCorrectionStatData& __Other) const
	{
		return FGsSchemaNpcCorrectionStatData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcCorrectionStatData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcCorrectionStatData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcCorrectionStatDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcCorrectionStatDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcCorrectionStatData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcCorrectionStatData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcCorrectionStatData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcCorrectionStatDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcCorrectionStatDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcCorrectionStatData final : public UGsTable
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
		TMap<FName, const FGsSchemaNpcCorrectionStatData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaNpcCorrectionStatData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaNpcCorrectionStatData*& OutRow) const
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
		const FGsSchemaNpcCorrectionStatData* Temp;
		return !FindRowById(FGsSchemaNpcCorrectionStatData().id, Temp);
	}
#endif
};
