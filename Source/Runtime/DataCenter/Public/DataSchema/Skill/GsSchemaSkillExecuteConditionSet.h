/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillExecuteConditionSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillExecuteConditionSet"))
struct DATACENTER_API FGsSchemaSkillExecuteConditionSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionMethod"))
	SkillExecuteConditionMethod conditionMethod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="\"conditionMethod\" 의 타입에 따라 가변 구조체 사용", AllowedClasses="GsSchemaSkillExecuteConditionMethodStat, GsSchemaSkillExecuteConditionMethodPassivity, GsSchemaSkillExecuteConditionMethodAbnormality", DisplayName="conditionMethodParam"))
	FGsSchemaRowPoly conditionMethodParam;
public:
	FGsSchemaSkillExecuteConditionSet();
	
protected:
	bool operator== (const FGsSchemaSkillExecuteConditionSet& __Other) const
	{
		return FGsSchemaSkillExecuteConditionSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillExecuteConditionSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillExecuteConditionSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillExecuteConditionSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillExecuteConditionSetRow() : Super()
	{
	}
	
	const FGsSchemaSkillExecuteConditionSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillExecuteConditionSet* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillExecuteConditionSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillExecuteConditionSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillExecuteConditionSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillExecuteConditionSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSkillExecuteConditionSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSkillExecuteConditionSet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSkillExecuteConditionSet*& OutRow) const
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
		const FGsSchemaSkillExecuteConditionSet* Temp;
		return !FindRowById(FGsSchemaSkillExecuteConditionSet().id, Temp);
	}
#endif
};
