/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaContentsEventReward.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="컨텐츠이벤트 - 추가 보상. 이벤트 기간 동안 특정 맵 혹은 몬스터에 추가적인 이벤트 보상을 걸 수 있다.", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/ContentsEvent/BSGSContentsEventReward"))
struct DATACENTER_API FGsSchemaContentsEventReward
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="0개 이면 모든 맵", DisplayName="mapDatas"))
	TArray<FGsSchemaMapDataRow> mapDatas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="제외할 NPC", DisplayName="exceptNpcBalances"))
	TArray<FGsSchemaNpcBalanceRow> exceptNpcBalances;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="해당 NPC의 기존 Reward + 이벤트로 추가된 Reward 동시 획득", DisplayName="rewardDatas"))
	TArray<FGsSchemaRewardDataRow> rewardDatas;
public:
	FGsSchemaContentsEventReward();
	
protected:
	bool operator== (const FGsSchemaContentsEventReward& __Other) const
	{
		return FGsSchemaContentsEventReward::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaContentsEventReward::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaContentsEventReward::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaContentsEventRewardRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaContentsEventRewardRow() : Super()
	{
	}
	
	const FGsSchemaContentsEventReward* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaContentsEventReward* Row = nullptr;
		return Table->FindRow<FGsSchemaContentsEventReward>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaContentsEventRewardRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaContentsEventRewardRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableContentsEventReward final : public UGsTable
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
		TMap<FName, const FGsSchemaContentsEventReward*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaContentsEventReward>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaContentsEventReward*& OutRow) const
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
		const FGsSchemaContentsEventReward* Temp;
		return !FindRowById(FGsSchemaContentsEventReward().id, Temp);
	}
#endif
};
