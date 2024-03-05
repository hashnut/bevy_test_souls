/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassCondition.h"
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassDetail.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassMissionCommon.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattlePass/BSGsBattlePassMissionCommon"))
struct DATACENTER_API FGsSchemaBattlePassMissionCommon
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passMissionId"))
	int64 passMissionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passMissionRewardExp"))
	int64 passMissionRewardExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passMissionCondition"))
	FGsSchemaBattlePassConditionRow passMissionCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passMissionCount"))
	int32 passMissionCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="미션 이름과 설명", DisplayName="detail"))
	FGsSchemaBattlePassDetailRow detail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardDetail"))
	FGsSchemaBattlePassDetailRow rewardDetail;
public:
	FGsSchemaBattlePassMissionCommon();
	
protected:
	bool operator== (const FGsSchemaBattlePassMissionCommon& __Other) const
	{
		return FGsSchemaBattlePassMissionCommon::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattlePassMissionCommon::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattlePassMissionCommon::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattlePassMissionCommonRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattlePassMissionCommonRow() : Super()
	{
	}
	
	const FGsSchemaBattlePassMissionCommon* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattlePassMissionCommon* Row = nullptr;
		return Table->FindRow<FGsSchemaBattlePassMissionCommon>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattlePassMissionCommonRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattlePassMissionCommonRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattlePassMissionCommon final : public UGsTable
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
		TMap<FName, const FGsSchemaBattlePassMissionCommon*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBattlePassMissionCommon>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->passMissionId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int64& InId, OUT const FGsSchemaBattlePassMissionCommon*& OutRow) const
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
		const FGsSchemaBattlePassMissionCommon* Temp;
		return !FindRowById(FGsSchemaBattlePassMissionCommon().passMissionId, Temp);
	}
#endif
};
