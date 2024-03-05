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
#include "GsSchemaSystemMessageConditionItemGetFieldBoss.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="FiledBoss Item System Chat Message Condition Table", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SystemMessage/Condition/BSGsSystemMessageConditionItemGetFieldBoss"))
struct DATACENTER_API FGsSchemaSystemMessageConditionItemGetFieldBoss
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ItemType"))
	ItemType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="minItemGrade"))
	ItemGrade minItemGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exceptGradeList"))
	TArray<ItemGrade> exceptGradeList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sendingOption"))
	FGsSchemaSendingOption sendingOption;
public:
	FGsSchemaSystemMessageConditionItemGetFieldBoss();
	
protected:
	bool operator== (const FGsSchemaSystemMessageConditionItemGetFieldBoss& __Other) const
	{
		return FGsSchemaSystemMessageConditionItemGetFieldBoss::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSystemMessageConditionItemGetFieldBoss::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSystemMessageConditionItemGetFieldBoss::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSystemMessageConditionItemGetFieldBossRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSystemMessageConditionItemGetFieldBossRow() : Super()
	{
	}
	
	const FGsSchemaSystemMessageConditionItemGetFieldBoss* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSystemMessageConditionItemGetFieldBoss* Row = nullptr;
		return Table->FindRow<FGsSchemaSystemMessageConditionItemGetFieldBoss>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSystemMessageConditionItemGetFieldBossRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSystemMessageConditionItemGetFieldBossRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSystemMessageConditionItemGetFieldBoss final : public UGsTable
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
		TMap<FName, const FGsSchemaSystemMessageConditionItemGetFieldBoss*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSystemMessageConditionItemGetFieldBoss>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSystemMessageConditionItemGetFieldBoss*& OutRow) const
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
		const FGsSchemaSystemMessageConditionItemGetFieldBoss* Temp;
		return !FindRowById(FGsSchemaSystemMessageConditionItemGetFieldBoss().id, Temp);
	}
#endif
};
