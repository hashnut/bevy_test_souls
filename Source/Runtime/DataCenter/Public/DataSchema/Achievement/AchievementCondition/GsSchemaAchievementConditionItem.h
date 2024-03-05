/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsSchemaAchievementConditionItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Achievement/AchievementCondition/BSGsAchievementConditionItem"))
struct DATACENTER_API FGsSchemaAchievementConditionItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ItemId"))
	int64 ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCategoryMain"))
	ItemCategoryMain itemCategoryMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCategorySub"))
	ItemCategorySub itemCategorySub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemGrade"))
	ItemGrade itemGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemType"))
	ItemType itemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="craftResult"))
	CraftResult craftResult;
public:
	FGsSchemaAchievementConditionItem();
	
protected:
	bool operator== (const FGsSchemaAchievementConditionItem& __Other) const
	{
		return FGsSchemaAchievementConditionItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAchievementConditionItem::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAchievementConditionItem::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAchievementConditionItemRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAchievementConditionItemRow() : Super()
	{
	}
	
	const FGsSchemaAchievementConditionItem* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAchievementConditionItem* Row = nullptr;
		return Table->FindRow<FGsSchemaAchievementConditionItem>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAchievementConditionItemRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAchievementConditionItemRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAchievementConditionItem final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Achievement/AchievementCondition");
	}
	
};
