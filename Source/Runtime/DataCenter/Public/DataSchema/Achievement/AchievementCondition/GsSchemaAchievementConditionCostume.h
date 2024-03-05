/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAchievementConditionCostume.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Achievement/AchievementCondition/BSGsAchievementConditionCostume"))
struct DATACENTER_API FGsSchemaAchievementConditionCostume
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeGrade"))
	CostumeGrade costumeGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeComposeResult"))
	CostumeComposeResult costumeComposeResult;
public:
	FGsSchemaAchievementConditionCostume();
	
protected:
	bool operator== (const FGsSchemaAchievementConditionCostume& __Other) const
	{
		return FGsSchemaAchievementConditionCostume::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAchievementConditionCostume::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAchievementConditionCostume::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAchievementConditionCostumeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAchievementConditionCostumeRow() : Super()
	{
	}
	
	const FGsSchemaAchievementConditionCostume* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAchievementConditionCostume* Row = nullptr;
		return Table->FindRow<FGsSchemaAchievementConditionCostume>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAchievementConditionCostumeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAchievementConditionCostumeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAchievementConditionCostume final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Achievement/AchievementCondition");
	}
	
};
