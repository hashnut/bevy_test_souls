/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassLevelBonusExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/BattlePass/BSGsPassLevelBonusExcel"))
struct DATACENTER_API FGsSchemaPassLevelBonusExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passLevelGroupId"))
	int64 passLevelGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isPayBonus"))
	bool isPayBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exceedPerExp"))
	int64 exceedPerExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxExp"))
	int64 maxExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bonusRewardId"))
	int64 bonusRewardId;
public:
	FGsSchemaPassLevelBonusExcel();
	
protected:
	bool operator== (const FGsSchemaPassLevelBonusExcel& __Other) const
	{
		return FGsSchemaPassLevelBonusExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassLevelBonusExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassLevelBonusExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassLevelBonusExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassLevelBonusExcelRow() : Super()
	{
	}
	
	const FGsSchemaPassLevelBonusExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassLevelBonusExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaPassLevelBonusExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassLevelBonusExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassLevelBonusExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassLevelBonusExcel final : public UGsTable
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
		TMap<FName, const FGsSchemaPassLevelBonusExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPassLevelBonusExcel>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->passLevelGroupId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int64& InId, OUT const FGsSchemaPassLevelBonusExcel*& OutRow) const
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
		const FGsSchemaPassLevelBonusExcel* Temp;
		return !FindRowById(FGsSchemaPassLevelBonusExcel().passLevelGroupId, Temp);
	}
#endif
};
