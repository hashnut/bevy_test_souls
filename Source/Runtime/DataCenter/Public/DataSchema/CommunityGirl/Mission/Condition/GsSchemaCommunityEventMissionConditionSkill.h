/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEventMissionConditionSkill.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/CommunityGirl/Mission/Condition/BSGsCommunityEventMissionConditionSkill"))
struct DATACENTER_API FGsSchemaCommunityEventMissionConditionSkill
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillId"))
	int32 skillId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillEnchantResult"))
	SkillEnchantResult skillEnchantResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillGradeMin"))
	SkillGrade skillGradeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillGradeMax"))
	SkillGrade skillGradeMax;
public:
	FGsSchemaCommunityEventMissionConditionSkill();
	
protected:
	bool operator== (const FGsSchemaCommunityEventMissionConditionSkill& __Other) const
	{
		return FGsSchemaCommunityEventMissionConditionSkill::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEventMissionConditionSkill::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEventMissionConditionSkill::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventMissionConditionSkillRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventMissionConditionSkillRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEventMissionConditionSkill* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEventMissionConditionSkill* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEventMissionConditionSkill>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventMissionConditionSkillRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventMissionConditionSkillRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEventMissionConditionSkill final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("CommunityGirl/Mission/Condition");
	}
	
};
