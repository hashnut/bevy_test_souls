/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEventActionTeleport.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Event/EventAction/BSGsEventActionTeleport"))
struct DATACENTER_API FGsSchemaEventActionTeleport
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ActionTeleportId"))
	int32 ActionTeleportId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SpotId"))
	int32 SpotId;
public:
	FGsSchemaEventActionTeleport();
	
protected:
	bool operator== (const FGsSchemaEventActionTeleport& __Other) const
	{
		return FGsSchemaEventActionTeleport::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEventActionTeleport::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEventActionTeleport::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEventActionTeleportRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEventActionTeleportRow() : Super()
	{
	}
	
	const FGsSchemaEventActionTeleport* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaEventActionTeleport* Row = nullptr;
		return Table->FindRow<FGsSchemaEventActionTeleport>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEventActionTeleportRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEventActionTeleportRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEventActionTeleport final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Event/EventAction");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaEventActionTeleport*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaEventActionTeleport>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->ActionTeleportId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaEventActionTeleport*& OutRow) const
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
		const FGsSchemaEventActionTeleport* Temp;
		return !FindRowById(FGsSchemaEventActionTeleport().ActionTeleportId, Temp);
	}
#endif
};
