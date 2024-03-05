/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsSchemaAchievementConditionCreature.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Achievement/AchievementCondition/BSGsAchievementConditionCreature"))
struct DATACENTER_API FGsSchemaAchievementConditionCreature
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureId"))
	int64 creatureId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="creatureRankType"))
	CreatureRankType creatureRankType;
public:
	FGsSchemaAchievementConditionCreature();
	
protected:
	bool operator== (const FGsSchemaAchievementConditionCreature& __Other) const
	{
		return FGsSchemaAchievementConditionCreature::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAchievementConditionCreature::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAchievementConditionCreature::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAchievementConditionCreatureRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAchievementConditionCreatureRow() : Super()
	{
	}
	
	const FGsSchemaAchievementConditionCreature* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAchievementConditionCreature* Row = nullptr;
		return Table->FindRow<FGsSchemaAchievementConditionCreature>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAchievementConditionCreatureRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAchievementConditionCreatureRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAchievementConditionCreature final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Achievement/AchievementCondition");
	}
	
};
