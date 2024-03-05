/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemEffectAbnormalityAndReward.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Item/BSGsItemEffectAbnormalityAndReward"))
struct DATACENTER_API FGsSchemaItemEffectAbnormalityAndReward
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId"))
	int32 effectId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="abnormalityId"))
	FGsSchemaAbnormalitySetRow abnormalityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardId"))
	FGsSchemaRewardDataRow rewardId;
public:
	FGsSchemaItemEffectAbnormalityAndReward();
	
protected:
	bool operator== (const FGsSchemaItemEffectAbnormalityAndReward& __Other) const
	{
		return FGsSchemaItemEffectAbnormalityAndReward::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemEffectAbnormalityAndReward::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemEffectAbnormalityAndReward::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemEffectAbnormalityAndRewardRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemEffectAbnormalityAndRewardRow() : Super()
	{
	}
	
	const FGsSchemaItemEffectAbnormalityAndReward* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemEffectAbnormalityAndReward* Row = nullptr;
		return Table->FindRow<FGsSchemaItemEffectAbnormalityAndReward>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemEffectAbnormalityAndRewardRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemEffectAbnormalityAndRewardRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemEffectAbnormalityAndReward final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Item");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaItemEffectAbnormalityAndReward*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaItemEffectAbnormalityAndReward>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->effectId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaItemEffectAbnormalityAndReward*& OutRow) const
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
		const FGsSchemaItemEffectAbnormalityAndReward* Temp;
		return !FindRowById(FGsSchemaItemEffectAbnormalityAndReward().effectId, Temp);
	}
#endif
};
