/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UGsHitEffectData;
#include "GsSchemaExtraTypeEffectSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/HitEffect/BSGsExtraTypeEffectSet"))
struct DATACENTER_API FGsSchemaExtraTypeEffectSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	HitResultType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectPath"))
	UGsHitEffectData* effectPath;
public:
	FGsSchemaExtraTypeEffectSet();
	
protected:
	bool operator== (const FGsSchemaExtraTypeEffectSet& __Other) const
	{
		return FGsSchemaExtraTypeEffectSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaExtraTypeEffectSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaExtraTypeEffectSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaExtraTypeEffectSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaExtraTypeEffectSetRow() : Super()
	{
	}
	
	const FGsSchemaExtraTypeEffectSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaExtraTypeEffectSet* Row = nullptr;
		return Table->FindRow<FGsSchemaExtraTypeEffectSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaExtraTypeEffectSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaExtraTypeEffectSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableExtraTypeEffectSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<HitResultType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/HitEffect");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaExtraTypeEffectSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaExtraTypeEffectSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->type, Row.Key);
		}
	}
	
public:
	bool FindRowById(const HitResultType& InId, OUT const FGsSchemaExtraTypeEffectSet*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const HitResultType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaExtraTypeEffectSet* Temp;
		return !FindRowById(FGsSchemaExtraTypeEffectSet().type, Temp);
	}
#endif
};
