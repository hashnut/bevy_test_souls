/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/User/SkillSlot/GsSchemaUserSkillSlotInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUserComboSkillSlotSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/SkillSlot/BSGSUserComboSkillSlotSet"))
struct DATACENTER_API FGsSchemaUserComboSkillSlotSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="skill 데이터에서 sequenceInfo에 정보가 있을경우 currentStage정보 (기본 1)", DisplayName="stage"))
	int32 stage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="선판정 결과 기본 데미지 판정", DisplayName="normalAttack"))
	FGsSchemaUserSkillSlotInfo normalAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="선판정 결과 크리티컬 판정", DisplayName="criticalAttack"))
	FGsSchemaUserSkillSlotInfo criticalAttack;
public:
	FGsSchemaUserComboSkillSlotSet();
	
protected:
	bool operator== (const FGsSchemaUserComboSkillSlotSet& __Other) const
	{
		return FGsSchemaUserComboSkillSlotSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaUserComboSkillSlotSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaUserComboSkillSlotSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaUserComboSkillSlotSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaUserComboSkillSlotSetRow() : Super()
	{
	}
	
	const FGsSchemaUserComboSkillSlotSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaUserComboSkillSlotSet* Row = nullptr;
		return Table->FindRow<FGsSchemaUserComboSkillSlotSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaUserComboSkillSlotSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaUserComboSkillSlotSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableUserComboSkillSlotSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("User/SkillSlot");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaUserComboSkillSlotSet*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaUserComboSkillSlotSet>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->stage, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaUserComboSkillSlotSet*& OutRow) const
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
		const FGsSchemaUserComboSkillSlotSet* Temp;
		return !FindRowById(FGsSchemaUserComboSkillSlotSet().stage, Temp);
	}
#endif
};
