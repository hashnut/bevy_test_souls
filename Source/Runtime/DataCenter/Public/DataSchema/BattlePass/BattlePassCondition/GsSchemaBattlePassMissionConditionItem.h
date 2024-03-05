/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassMissionConditionItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattlePass/BattlePassCondition/BSGsBattlePassMissionConditionItem"))
struct DATACENTER_API FGsSchemaBattlePassMissionConditionItem
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
public:
	FGsSchemaBattlePassMissionConditionItem();
	
protected:
	bool operator== (const FGsSchemaBattlePassMissionConditionItem& __Other) const
	{
		return FGsSchemaBattlePassMissionConditionItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattlePassMissionConditionItem::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattlePassMissionConditionItem::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattlePassMissionConditionItemRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattlePassMissionConditionItemRow() : Super()
	{
	}
	
	const FGsSchemaBattlePassMissionConditionItem* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattlePassMissionConditionItem* Row = nullptr;
		return Table->FindRow<FGsSchemaBattlePassMissionConditionItem>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattlePassMissionConditionItemRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattlePassMissionConditionItemRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattlePassMissionConditionItem final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BattlePass/BattlePassCondition");
	}
	
};
