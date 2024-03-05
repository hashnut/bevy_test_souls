/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/EventProgress/GsEventProgressDefine.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedEventEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEventActionEventProgress.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/EventProgress/BSGsEventActionEventProgress"))
struct DATACENTER_API FGsSchemaEventActionEventProgress
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventActionType"))
	EventActionType eventActionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventProgressType"))
	EGsEventProgressType eventProgressType;
public:
	FGsSchemaEventActionEventProgress();
	
protected:
	bool operator== (const FGsSchemaEventActionEventProgress& __Other) const
	{
		return FGsSchemaEventActionEventProgress::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEventActionEventProgress::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEventActionEventProgress::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEventActionEventProgressRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEventActionEventProgressRow() : Super()
	{
	}
	
	const FGsSchemaEventActionEventProgress* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaEventActionEventProgress* Row = nullptr;
		return Table->FindRow<FGsSchemaEventActionEventProgress>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEventActionEventProgressRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEventActionEventProgressRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEventActionEventProgress final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<EventActionType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("EventProgress");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaEventActionEventProgress*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaEventActionEventProgress>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->eventActionType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const EventActionType& InId, OUT const FGsSchemaEventActionEventProgress*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const EventActionType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaEventActionEventProgress* Temp;
		return !FindRowById(FGsSchemaEventActionEventProgress().eventActionType, Temp);
	}
#endif
};
