/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillCollision.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillNotifyElement.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillTargetFilter.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UGsHitEffectData;
#include "GsSchemaSkillNotifySet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/BSGsSkillNotifySet"))
struct DATACENTER_API FGsSchemaSkillNotifySet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="SkillNotifyType 열거형 참조", DisplayName="type"))
	SkillNotifyType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="localPlayerOnly"))
	bool localPlayerOnly;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="effectText"))
	FText effectText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="hiteffect"))
	UGsHitEffectData* hiteffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hitsound"))
	FGsSchemaSoundResDataRow hitsound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="targetFilterInfo"))
	FGsSchemaSkillTargetFilterRow targetFilterInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="collisionInfoList"))
	TArray<FGsSchemaSkillCollisionRow> collisionInfoList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="notifyElementList"))
	TArray<FGsSchemaSkillNotifyElementRow> notifyElementList;
public:
	FGsSchemaSkillNotifySet();
	
protected:
	bool operator== (const FGsSchemaSkillNotifySet& __Other) const
	{
		return FGsSchemaSkillNotifySet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillNotifySet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillNotifySet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillNotifySetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillNotifySetRow() : Super()
	{
	}
	
	const FGsSchemaSkillNotifySet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillNotifySet* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillNotifySet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillNotifySetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillNotifySetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillNotifySet final : public UGsTable
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
		TMap<FName, const FGsSchemaSkillNotifySet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSkillNotifySet>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSkillNotifySet*& OutRow) const
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
		const FGsSchemaSkillNotifySet* Temp;
		return !FindRowById(FGsSchemaSkillNotifySet().id, Temp);
	}
#endif
};
