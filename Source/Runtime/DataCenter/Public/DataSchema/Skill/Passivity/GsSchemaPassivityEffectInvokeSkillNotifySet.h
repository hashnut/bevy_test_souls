/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/ParamInfo/GsSchemaPassivityEffectInvokeSkillNotifyParam.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityEffectInvokeSkillNotifySet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/Passivity/BSGsPassivityEffectInvokeSkillNotifySet"))
struct DATACENTER_API FGsSchemaPassivityEffectInvokeSkillNotifySet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param"))
	FGsSchemaPassivityEffectInvokeSkillNotifyParam param;
public:
	FGsSchemaPassivityEffectInvokeSkillNotifySet();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectInvokeSkillNotifySet& __Other) const
	{
		return FGsSchemaPassivityEffectInvokeSkillNotifySet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassivityEffectInvokeSkillNotifySet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassivityEffectInvokeSkillNotifySet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassivityEffectInvokeSkillNotifySetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassivityEffectInvokeSkillNotifySetRow() : Super()
	{
	}
	
	const FGsSchemaPassivityEffectInvokeSkillNotifySet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassivityEffectInvokeSkillNotifySet* Row = nullptr;
		return Table->FindRow<FGsSchemaPassivityEffectInvokeSkillNotifySet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassivityEffectInvokeSkillNotifySetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassivityEffectInvokeSkillNotifySetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassivityEffectInvokeSkillNotifySet final : public UGsTable
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
		TMap<FName, const FGsSchemaPassivityEffectInvokeSkillNotifySet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPassivityEffectInvokeSkillNotifySet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaPassivityEffectInvokeSkillNotifySet*& OutRow) const
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
		const FGsSchemaPassivityEffectInvokeSkillNotifySet* Temp;
		return !FindRowById(FGsSchemaPassivityEffectInvokeSkillNotifySet().id, Temp);
	}
#endif
};
