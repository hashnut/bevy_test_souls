/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/RootMotion/GsSchemaRootmotionInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaRootmotionSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/RootMotion/BSGsRootmotionSet"))
struct DATACENTER_API FGsSchemaRootmotionSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="구간 동안 특정 콜리전 타입을 기본 NPC타입으로 설정", DisplayName="collisionDisable"))
	bool collisionDisable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="timeLine"))
	TArray<FGsSchemaRootmotionInfo> timeLine;
public:
	FGsSchemaRootmotionSet();
	
protected:
	bool operator== (const FGsSchemaRootmotionSet& __Other) const
	{
		return FGsSchemaRootmotionSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaRootmotionSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaRootmotionSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaRootmotionSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaRootmotionSetRow() : Super()
	{
	}
	
	const FGsSchemaRootmotionSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaRootmotionSet* Row = nullptr;
		return Table->FindRow<FGsSchemaRootmotionSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaRootmotionSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaRootmotionSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableRootmotionSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/RootMotion");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaRootmotionSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaRootmotionSet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaRootmotionSet*& OutRow) const
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
		const FGsSchemaRootmotionSet* Temp;
		return !FindRowById(FGsSchemaRootmotionSet().id, Temp);
	}
#endif
};
