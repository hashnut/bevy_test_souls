/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/EffectText/GsSchemaEffectTextData.h"
#include "GsSchemaEffectTextSkillNotify.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/EffectText/BSGsEffectTextSkillNotify"))
struct DATACENTER_API FGsSchemaEffectTextSkillNotify
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillNotifyId", Tooltip))
	int32 skillNotifyId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dataList", Tooltip))
	TArray<FGsSchemaEffectTextData> dataList;
	
public:
	FGsSchemaEffectTextSkillNotify();
	
protected:
	bool operator== (const FGsSchemaEffectTextSkillNotify& __Other) const
	{
		return FGsSchemaEffectTextSkillNotify::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEffectTextSkillNotify::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEffectTextSkillNotify::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEffectTextSkillNotifyRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEffectTextSkillNotifyRow() : Super()
	{
	}
	
	const FGsSchemaEffectTextSkillNotify* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaEffectTextSkillNotify* Row = nullptr;
		return Table->FindRow<FGsSchemaEffectTextSkillNotify>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEffectTextSkillNotifyRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEffectTextSkillNotifyRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEffectTextSkillNotify final : public UGsTable
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
		TMap<FName, const FGsSchemaEffectTextSkillNotify*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaEffectTextSkillNotify>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->skillNotifyId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaEffectTextSkillNotify*& OutRow) const
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
		const FGsSchemaEffectTextSkillNotify* Temp;
		return !FindRowById(FGsSchemaEffectTextSkillNotify().skillNotifyId, Temp);
	}
#endif
};
