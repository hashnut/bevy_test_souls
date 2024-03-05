/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsSchemaResetTimeTicketConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BSGsResetTimeTicketConfig"))
struct DATACENTER_API FGsSchemaResetTimeTicketConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resetType", Tooltip))
	ResetTimeTicket resetType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="makeCron", Tooltip))
	FString makeCron;
	
public:
	FGsSchemaResetTimeTicketConfig();
	
protected:
	bool operator== (const FGsSchemaResetTimeTicketConfig& __Other) const
	{
		return FGsSchemaResetTimeTicketConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaResetTimeTicketConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaResetTimeTicketConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaResetTimeTicketConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaResetTimeTicketConfigRow() : Super()
	{
	}
	
	const FGsSchemaResetTimeTicketConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaResetTimeTicketConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaResetTimeTicketConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaResetTimeTicketConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaResetTimeTicketConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableResetTimeTicketConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<ResetTimeTicket, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaResetTimeTicketConfig*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaResetTimeTicketConfig>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->resetType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const ResetTimeTicket& InId, OUT const FGsSchemaResetTimeTicketConfig*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const ResetTimeTicket& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaResetTimeTicketConfig* Temp;
		return !FindRowById(FGsSchemaResetTimeTicketConfig().resetType, Temp);
	}
#endif
};
