/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/MultiLevelRank/GsMultiLevelRankType.h"
#include "Runtime/DataCenter/Public/MultiLevelRank/ERankLinkType.h"
#include "Runtime/DataCenter/Public/DataSchema/MultiLevelRank/GsSchemaMultiLevelRankSlot.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMultiLevelRank.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MultiLevelRank/BSGsMultiLevelRank"))
struct DATACENTER_API FGsSchemaMultiLevelRank
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rank"))
	int32 rank;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exp"))
	int64 exp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costType"))
	CostType costType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costItemId"))
	int32 costItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costItemAmount"))
	int32 costItemAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType"))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyCost"))
	int32 currencyCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectType"))
	ItemGrade effectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardId"))
	FGsSchemaRewardDataRow rewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardSlotDesc"))
	FText rewardSlotDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardPopupDesc"))
	FText rewardPopupDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardType"))
	EGsMultiLevelRewardType rewardType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="manRewardShortImage"))
	FSoftObjectPath manRewardShortImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="womanRewardShortImage"))
	FSoftObjectPath womanRewardShortImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="manRewardLongImage"))
	FSoftObjectPath manRewardLongImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="womanRewardLongImage"))
	FSoftObjectPath womanRewardLongImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="multiLevelRankInfoList"))
	TArray<FGsSchemaMultiLevelRankSlotRow> multiLevelRankInfoList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="분기별 link 타입", DisplayName="linkType"))
	ERankLinkType linkType;
public:
	FGsSchemaMultiLevelRank();
	
protected:
	bool operator== (const FGsSchemaMultiLevelRank& __Other) const
	{
		return FGsSchemaMultiLevelRank::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMultiLevelRank::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMultiLevelRank::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMultiLevelRankRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMultiLevelRankRow() : Super()
	{
	}
	
	const FGsSchemaMultiLevelRank* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMultiLevelRank* Row = nullptr;
		return Table->FindRow<FGsSchemaMultiLevelRank>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMultiLevelRankRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMultiLevelRankRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMultiLevelRank final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("MultiLevelRank");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaMultiLevelRank*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaMultiLevelRank>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->rank, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaMultiLevelRank*& OutRow) const
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
		const FGsSchemaMultiLevelRank* Temp;
		return !FindRowById(FGsSchemaMultiLevelRank().rank, Temp);
	}
#endif
};
