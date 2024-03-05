/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedBattlePassEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassDetail.h"
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassLevel.h"
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassMissionGroup.h"
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassTabDetail.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassCommon.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattlePass/BSGsBattlePassCommon"))
struct DATACENTER_API FGsSchemaBattlePassCommon
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passPeriodStart"))
	FString passPeriodStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passPeriodEnd"))
	FString passPeriodEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passExposureConditionType"))
	BattlePassExposureConditionType passExposureConditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passExposureConditionCountMin"))
	int32 passExposureConditionCountMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passExposureConditionCountMax"))
	int32 passExposureConditionCountMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="additionalRewardOpenCurrencyType"))
	CurrencyType additionalRewardOpenCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="additionalRewardOpenCurrency"))
	int64 additionalRewardOpenCurrency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="additionalRewardOpenItemId"))
	FGsSchemaItemCommonRow additionalRewardOpenItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="additionalRewardOpenItemAmount"))
	int64 additionalRewardOpenItemAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passLevelGroupId"))
	FGsSchemaBattlePassLevelRow passLevelGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passMissionGroupId"))
	FGsSchemaBattlePassMissionGroupRow passMissionGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="배틀 패스 이름", DisplayName="detail"))
	FGsSchemaBattlePassDetailRow detail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tabDetailId"))
	FGsSchemaBattlePassTabDetailRow tabDetailId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passBanner"))
	FSoftObjectPath passBanner;
public:
	FGsSchemaBattlePassCommon();
	
protected:
	bool operator== (const FGsSchemaBattlePassCommon& __Other) const
	{
		return FGsSchemaBattlePassCommon::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattlePassCommon::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattlePassCommon::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattlePassCommonRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattlePassCommonRow() : Super()
	{
	}
	
	const FGsSchemaBattlePassCommon* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattlePassCommon* Row = nullptr;
		return Table->FindRow<FGsSchemaBattlePassCommon>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattlePassCommonRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattlePassCommonRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattlePassCommon final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int64, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BattlePass");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaBattlePassCommon*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBattlePassCommon>(RowsMap))
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
	bool FindRowById(const int64& InId, OUT const FGsSchemaBattlePassCommon*& OutRow) const
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
		const FGsSchemaBattlePassCommon* Temp;
		return !FindRowById(FGsSchemaBattlePassCommon().id, Temp);
	}
#endif
};
