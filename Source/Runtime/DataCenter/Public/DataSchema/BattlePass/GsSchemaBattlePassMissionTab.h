/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassDetail.h"
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassMissionCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/BattlePass/GsSchemaBattlePassTabDetail.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassMissionTab.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattlePass/BSGsBattlePassMissionTab"))
struct DATACENTER_API FGsSchemaBattlePassMissionTab
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabId"))
	int64 tabId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resetType"))
	ResetTimeTicket resetType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passMission"))
	TArray<FGsSchemaBattlePassMissionCommonRow> passMission;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tabDetail"))
	FGsSchemaBattlePassTabDetailRow tabDetail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="미션 탭 이름", DisplayName="detail"))
	FGsSchemaBattlePassDetailRow detail;
public:
	FGsSchemaBattlePassMissionTab();
	
protected:
	bool operator== (const FGsSchemaBattlePassMissionTab& __Other) const
	{
		return FGsSchemaBattlePassMissionTab::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattlePassMissionTab::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattlePassMissionTab::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattlePassMissionTabRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattlePassMissionTabRow() : Super()
	{
	}
	
	const FGsSchemaBattlePassMissionTab* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattlePassMissionTab* Row = nullptr;
		return Table->FindRow<FGsSchemaBattlePassMissionTab>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattlePassMissionTabRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattlePassMissionTabRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattlePassMissionTab final : public UGsTable
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
		TMap<FName, const FGsSchemaBattlePassMissionTab*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBattlePassMissionTab>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->tabId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int64& InId, OUT const FGsSchemaBattlePassMissionTab*& OutRow) const
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
		const FGsSchemaBattlePassMissionTab* Temp;
		return !FindRowById(FGsSchemaBattlePassMissionTab().tabId, Temp);
	}
#endif
};
