/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEventMissionConditionItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/CommunityGirl/Mission/Condition/BSGsCommunityEventMissionConditionItem"))
struct DATACENTER_API FGsSchemaCommunityEventMissionConditionItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemId"))
	int32 itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemGradeMin"))
	ItemGrade itemGradeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemGradeMax"))
	ItemGrade itemGradeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemType"))
	ItemType itemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCategoryMain"))
	ItemCategoryMain itemCategoryMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCategorySub"))
	ItemCategorySub itemCategorySub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCraftResult"))
	CraftResult itemCraftResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemEnchantMinLevel"))
	int32 itemEnchantMinLevel;
public:
	FGsSchemaCommunityEventMissionConditionItem();
	
protected:
	bool operator== (const FGsSchemaCommunityEventMissionConditionItem& __Other) const
	{
		return FGsSchemaCommunityEventMissionConditionItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEventMissionConditionItem::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEventMissionConditionItem::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventMissionConditionItemRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventMissionConditionItemRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEventMissionConditionItem* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEventMissionConditionItem* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEventMissionConditionItem>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventMissionConditionItemRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventMissionConditionItemRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEventMissionConditionItem final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("CommunityGirl/Mission/Condition");
	}
	
};
