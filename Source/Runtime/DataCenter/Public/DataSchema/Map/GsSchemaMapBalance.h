/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/GsSchemaMapDataCost.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/MapBalance/GsSchemaNpcDropItem.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Resurrect/GsSchemaResurrectData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMapBalance.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/BSGsMapBalance"))
struct DATACENTER_API FGsSchemaMapBalance
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="EnablePK"))
	bool EnablePK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="EnablePlaceMem"))
	bool EnablePlaceMem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passivityIdList"))
	TArray<FGsSchemaPassivitySetRow> passivityIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="맵에 환경속성으로 표시할 passivityIdList의 인덱스", DisplayName="passivityIdListIndexEnv"))
	int32 passivityIdListIndexEnv;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cost"))
	FGsSchemaMapDataCost cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bossWarpCost"))
	FGsSchemaMapDataCost bossWarpCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="canItemDrop"))
	bool canItemDrop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dropItemInfo"))
	TArray<FGsSchemaNpcDropItem> dropItemInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="resurrectionDataIdx"))
	FGsSchemaResurrectDataRow resurrectionDataIdx;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expMapBonusRate"))
	int32 expMapBonusRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="goldMapBonusRate"))
	int32 goldMapBonusRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcIdleTimeRate"))
	float npcIdleTimeRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcRoamingRangeRate"))
	float npcRoamingRangeRate;
public:
	FGsSchemaMapBalance();
	
protected:
	bool operator== (const FGsSchemaMapBalance& __Other) const
	{
		return FGsSchemaMapBalance::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMapBalance::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMapBalance::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMapBalanceRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMapBalanceRow() : Super()
	{
	}
	
	const FGsSchemaMapBalance* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMapBalance* Row = nullptr;
		return Table->FindRow<FGsSchemaMapBalance>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMapBalanceRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMapBalanceRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMapBalance final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaMapBalance*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMapBalance>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaMapBalance*& OutRow) const
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
		const FGsSchemaMapBalance* Temp;
		return !FindRowById(FGsSchemaMapBalance().id, Temp);
	}
#endif
};
