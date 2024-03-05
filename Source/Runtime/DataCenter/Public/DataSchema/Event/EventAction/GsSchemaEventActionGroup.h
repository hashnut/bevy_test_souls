/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Event/EventAction/GsSchemaEventAction.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEventActionGroup.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="이벤트액션그룹", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Event/EventAction/BSGsEventActionGroup"))
struct DATACENTER_API FGsSchemaEventActionGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="이벤트액션그룹 ID"))
	int32 id;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(DisplayName="이벤트액션 목록"))
	TArray<FGsSchemaEventAction> eventActionList;
	
public:
	FGsSchemaEventActionGroup();
	
protected:
	bool operator== (const FGsSchemaEventActionGroup& __Other) const
	{
		return FGsSchemaEventActionGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEventActionGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEventActionGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEventActionGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEventActionGroupRow() : Super()
	{
	}
	
	const FGsSchemaEventActionGroup* GetRow() const
	{
		const FGsSchemaEventActionGroup* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaEventActionGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEventActionGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEventActionGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEventActionGroup final : public UGsTable
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
		TMap<FName, const FGsSchemaEventActionGroup*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaEventActionGroup>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaEventActionGroup*& OutRow) const
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
		const FGsSchemaEventActionGroup* Temp;
		return !FindRowById(FGsSchemaEventActionGroup().id, Temp);
	}
#endif
};
