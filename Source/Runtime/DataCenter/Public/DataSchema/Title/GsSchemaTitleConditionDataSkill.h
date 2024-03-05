/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/User/GsSchemaUserSkillSet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTitleConditionDataSkill.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Title/BSGsTitleConditionDataSkill"))
struct DATACENTER_API FGsSchemaTitleConditionDataSkill
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mUserSkillId"))
	FGsSchemaUserSkillSetRow mUserSkillId;
public:
	FGsSchemaTitleConditionDataSkill();
	
protected:
	bool operator== (const FGsSchemaTitleConditionDataSkill& __Other) const
	{
		return FGsSchemaTitleConditionDataSkill::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTitleConditionDataSkill::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTitleConditionDataSkill::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTitleConditionDataSkillRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTitleConditionDataSkillRow() : Super()
	{
	}
	
	const FGsSchemaTitleConditionDataSkill* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTitleConditionDataSkill* Row = nullptr;
		return Table->FindRow<FGsSchemaTitleConditionDataSkill>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTitleConditionDataSkillRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTitleConditionDataSkillRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTitleConditionDataSkill final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Title");
	}
	
};
