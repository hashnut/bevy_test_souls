/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class UGsHitEffectData;
#include "GsSchemaHealByTypeEffectSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Skill/HitEffect/BSGsHealByTypeEffectSet"))
struct DATACENTER_API FGsSchemaHealByTypeEffectSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	HealByType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectPath"))
	UGsHitEffectData* effectPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="SoundResData 테이블에 있는 ID 값을 입력하여, HealByTypeEffectSet에 등록된 이펙트가 출력될 때 사운드가 같이 출력되도록 설정.", DisplayName="hitSound"))
	FGsSchemaSoundResDataRow hitSound;
public:
	FGsSchemaHealByTypeEffectSet();
	
protected:
	bool operator== (const FGsSchemaHealByTypeEffectSet& __Other) const
	{
		return FGsSchemaHealByTypeEffectSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaHealByTypeEffectSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaHealByTypeEffectSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaHealByTypeEffectSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaHealByTypeEffectSetRow() : Super()
	{
	}
	
	const FGsSchemaHealByTypeEffectSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaHealByTypeEffectSet* Row = nullptr;
		return Table->FindRow<FGsSchemaHealByTypeEffectSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaHealByTypeEffectSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaHealByTypeEffectSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableHealByTypeEffectSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<HealByType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Skill/HitEffect");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaHealByTypeEffectSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaHealByTypeEffectSet>(RowsMap))
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
	bool FindRowById(const HealByType& InId, OUT const FGsSchemaHealByTypeEffectSet*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const HealByType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaHealByTypeEffectSet* Temp;
		return !FindRowById(FGsSchemaHealByTypeEffectSet().type, Temp);
	}
#endif
};
