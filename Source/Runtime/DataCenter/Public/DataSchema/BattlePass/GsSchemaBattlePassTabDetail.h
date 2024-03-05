/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassTabDetail.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattlePass/BSGsBattlePassTabDetail"))
struct DATACENTER_API FGsSchemaBattlePassTabDetail
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabId"))
	int32 tabId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabIndex"))
	uint8 tabIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabName"))
	FText tabName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabDesc"))
	FText tabDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="path가 필요한 경우 활용", DisplayName="tabImage"))
	FSoftObjectPath tabImage;
public:
	FGsSchemaBattlePassTabDetail();
	
protected:
	bool operator== (const FGsSchemaBattlePassTabDetail& __Other) const
	{
		return FGsSchemaBattlePassTabDetail::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattlePassTabDetail::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattlePassTabDetail::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattlePassTabDetailRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattlePassTabDetailRow() : Super()
	{
	}
	
	const FGsSchemaBattlePassTabDetail* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattlePassTabDetail* Row = nullptr;
		return Table->FindRow<FGsSchemaBattlePassTabDetail>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattlePassTabDetailRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattlePassTabDetailRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattlePassTabDetail final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BattlePass");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaBattlePassTabDetail*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaBattlePassTabDetail>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaBattlePassTabDetail*& OutRow) const
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
		const FGsSchemaBattlePassTabDetail* Temp;
		return !FindRowById(FGsSchemaBattlePassTabDetail().tabId, Temp);
	}
#endif
};
