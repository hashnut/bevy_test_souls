/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedBattlePassEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassDataExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/BattlePass/BSGsBattlePassDataExcel"))
struct DATACENTER_API FGsSchemaBattlePassDataExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passPerriodStart"))
	FString passPerriodStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passPerriodEnd"))
	FString passPerriodEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passExposureConditionType"))
	BattlePassExposureConditionType passExposureConditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passExposureConditionCountMin"))
	int32 passExposureConditionCountMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passExposureConditionCountMax"))
	int32 passExposureConditionCountMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="additionalRewardOpenCurrencyType"))
	CurrencyType additionalRewardOpenCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="additionalRewardOpenCurrency"))
	int32 additionalRewardOpenCurrency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="additionalRewardOpenItemId"))
	int64 additionalRewardOpenItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="additionalRewardOpenItemAmount"))
	int64 additionalRewardOpenItemAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passLevelGroupId"))
	int64 passLevelGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passMissionGroupId"))
	int64 passMissionGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="detail"))
	int32 detail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabDetailId"))
	int32 tabDetailId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passBanner"))
	FSoftObjectPath passBanner;
public:
	FGsSchemaBattlePassDataExcel();
	
protected:
	bool operator== (const FGsSchemaBattlePassDataExcel& __Other) const
	{
		return FGsSchemaBattlePassDataExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattlePassDataExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattlePassDataExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattlePassDataExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattlePassDataExcelRow() : Super()
	{
	}
	
	const FGsSchemaBattlePassDataExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattlePassDataExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaBattlePassDataExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattlePassDataExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattlePassDataExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattlePassDataExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/BattlePass");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaBattlePassDataExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBattlePassDataExcel>(RowsMap))
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
	bool FindRowById(const int64& InId, OUT const FGsSchemaBattlePassDataExcel*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int64& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaBattlePassDataExcel* Temp;
		return !FindRowById(FGsSchemaBattlePassDataExcel().id, Temp);
	}
#endif
};
