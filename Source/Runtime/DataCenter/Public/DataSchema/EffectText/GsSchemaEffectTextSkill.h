/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/EffectText/GsSchemaEffectTextSkillLevel.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEffectTextSkill.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/EffectText/BSGsEffectTextSkill"))
struct DATACENTER_API FGsSchemaEffectTextSkill
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillId"))
	int32 skillId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelList"))
	TArray<FGsSchemaEffectTextSkillLevel> levelList;
public:
	FGsSchemaEffectTextSkill();
	
protected:
	bool operator== (const FGsSchemaEffectTextSkill& __Other) const
	{
		return FGsSchemaEffectTextSkill::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEffectTextSkill::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEffectTextSkill::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEffectTextSkillRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEffectTextSkillRow() : Super()
	{
	}
	
	const FGsSchemaEffectTextSkill* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaEffectTextSkill* Row = nullptr;
		return Table->FindRow<FGsSchemaEffectTextSkill>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEffectTextSkillRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEffectTextSkillRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEffectTextSkill final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("EffectText");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaEffectTextSkill*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaEffectTextSkill>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->skillId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaEffectTextSkill*& OutRow) const
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
		const FGsSchemaEffectTextSkill* Temp;
		return !FindRowById(FGsSchemaEffectTextSkill().skillId, Temp);
	}
#endif
};
