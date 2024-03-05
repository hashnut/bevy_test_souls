/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedBossEnum.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "GsSchemaFieldBossRewardData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/FieldBoss/BSGsFieldBossRewardData"))
struct DATACENTER_API FGsSchemaFieldBossRewardData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bossRewardId", Tooltip))
	int32 bossRewardId;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaRewardDataRow rewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mailTypeId", Tooltip))
	int32 mailTypeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="giveType", Tooltip))
	BossRewardGiveType giveType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contributionRankMin", Tooltip))
	int32 contributionRankMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contributionRankMax", Tooltip))
	int32 contributionRankMax;
public:
	FGsSchemaFieldBossRewardData();
	
protected:
	bool operator== (const FGsSchemaFieldBossRewardData& __Other) const
	{
		return FGsSchemaFieldBossRewardData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFieldBossRewardData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFieldBossRewardData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFieldBossRewardDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFieldBossRewardDataRow() : Super()
	{
	}
	
	const FGsSchemaFieldBossRewardData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFieldBossRewardData* Row = nullptr;
		return Table->FindRow<FGsSchemaFieldBossRewardData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFieldBossRewardDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFieldBossRewardDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFieldBossRewardData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/FieldBoss");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaFieldBossRewardData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaFieldBossRewardData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->bossRewardId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaFieldBossRewardData*& OutRow) const
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
		const FGsSchemaFieldBossRewardData* Temp;
		return !FindRowById(FGsSchemaFieldBossRewardData().bossRewardId, Temp);
	}
#endif
};
