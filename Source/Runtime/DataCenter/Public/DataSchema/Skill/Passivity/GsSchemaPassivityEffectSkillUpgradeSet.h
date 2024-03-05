/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectSkillUpgradeParam.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityEffectSkillUpgradeSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Passivity/BSGsPassivityEffectSkillUpgradeSet"))
struct DATACENTER_API FGsSchemaPassivityEffectSkillUpgradeSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param"))
	FGsSchemaPassivityEffectSkillUpgradeParam param;
public:
	FGsSchemaPassivityEffectSkillUpgradeSet();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectSkillUpgradeSet& __Other) const
	{
		return FGsSchemaPassivityEffectSkillUpgradeSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassivityEffectSkillUpgradeSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassivityEffectSkillUpgradeSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassivityEffectSkillUpgradeSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassivityEffectSkillUpgradeSetRow() : Super()
	{
	}
	
	const FGsSchemaPassivityEffectSkillUpgradeSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassivityEffectSkillUpgradeSet* Row = nullptr;
		return Table->FindRow<FGsSchemaPassivityEffectSkillUpgradeSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassivityEffectSkillUpgradeSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassivityEffectSkillUpgradeSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassivityEffectSkillUpgradeSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/Passivity");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaPassivityEffectSkillUpgradeSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPassivityEffectSkillUpgradeSet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaPassivityEffectSkillUpgradeSet*& OutRow) const
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
		const FGsSchemaPassivityEffectSkillUpgradeSet* Temp;
		return !FindRowById(FGsSchemaPassivityEffectSkillUpgradeSet().id, Temp);
	}
#endif
};
