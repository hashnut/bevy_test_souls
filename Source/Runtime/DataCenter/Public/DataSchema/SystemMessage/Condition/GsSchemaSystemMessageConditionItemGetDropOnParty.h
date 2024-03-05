/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/SystemMessage/Condition/GsSchemaSendingOption.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSystemMessageConditionItemGetDropOnParty.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="GetItem Kill Boss by Party", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SystemMessage/Condition/BSGsSystemMessageConditionItemGetDropOnParty"))
struct DATACENTER_API FGsSchemaSystemMessageConditionItemGetDropOnParty
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemType"))
	ItemType itemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="minItemGrade"))
	ItemGrade minItemGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exceptGradeList"))
	TArray<ItemGrade> exceptGradeList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sendingOption"))
	FGsSchemaSendingOption sendingOption;
public:
	FGsSchemaSystemMessageConditionItemGetDropOnParty();
	
protected:
	bool operator== (const FGsSchemaSystemMessageConditionItemGetDropOnParty& __Other) const
	{
		return FGsSchemaSystemMessageConditionItemGetDropOnParty::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSystemMessageConditionItemGetDropOnParty::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSystemMessageConditionItemGetDropOnParty::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSystemMessageConditionItemGetDropOnPartyRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSystemMessageConditionItemGetDropOnPartyRow() : Super()
	{
	}
	
	const FGsSchemaSystemMessageConditionItemGetDropOnParty* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSystemMessageConditionItemGetDropOnParty* Row = nullptr;
		return Table->FindRow<FGsSchemaSystemMessageConditionItemGetDropOnParty>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSystemMessageConditionItemGetDropOnPartyRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSystemMessageConditionItemGetDropOnPartyRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSystemMessageConditionItemGetDropOnParty final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SystemMessage/Condition");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSystemMessageConditionItemGetDropOnParty*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSystemMessageConditionItemGetDropOnParty>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSystemMessageConditionItemGetDropOnParty*& OutRow) const
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
		const FGsSchemaSystemMessageConditionItemGetDropOnParty* Temp;
		return !FindRowById(FGsSchemaSystemMessageConditionItemGetDropOnParty().id, Temp);
	}
#endif
};
