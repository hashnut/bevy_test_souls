/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedAchievementEnum.h"
#include "GsSchemaAchievementCondition.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Achievement/BSGsAchievementCondition"))
struct DATACENTER_API FGsSchemaAchievementCondition
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionType"))
	AchievementType conditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionValue"))
	FGsSchemaRowPoly conditionValue;
public:
	FGsSchemaAchievementCondition();
	
protected:
	bool operator== (const FGsSchemaAchievementCondition& __Other) const
	{
		return FGsSchemaAchievementCondition::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAchievementCondition::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAchievementCondition::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAchievementConditionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAchievementConditionRow() : Super()
	{
	}
	
	const FGsSchemaAchievementCondition* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAchievementCondition* Row = nullptr;
		return Table->FindRow<FGsSchemaAchievementCondition>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAchievementConditionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAchievementConditionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAchievementCondition final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Achievement");
	}
	
};
