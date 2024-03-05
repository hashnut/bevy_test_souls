/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Repeat/GsSchemaQuestRepeatScroll.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaVillageQuestRepeatItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/VillageQuest/BSGsVillageQuestRepeatItem"))
struct DATACENTER_API FGsSchemaVillageQuestRepeatItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ParentID"))
	int32 ParentID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="QuestRepetScroll"))
	FGsSchemaQuestRepeatScrollRow QuestRepetScroll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(ClampMin="0", ClampMax="10000", DisplayName="QuestProbablity"))
	int32 QuestProbablity;
public:
	FGsSchemaVillageQuestRepeatItem();
	
protected:
	bool operator== (const FGsSchemaVillageQuestRepeatItem& __Other) const
	{
		return FGsSchemaVillageQuestRepeatItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaVillageQuestRepeatItem::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaVillageQuestRepeatItem::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaVillageQuestRepeatItemRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaVillageQuestRepeatItemRow() : Super()
	{
	}
	
	const FGsSchemaVillageQuestRepeatItem* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaVillageQuestRepeatItem* Row = nullptr;
		return Table->FindRow<FGsSchemaVillageQuestRepeatItem>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaVillageQuestRepeatItemRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaVillageQuestRepeatItemRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableVillageQuestRepeatItem final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/VillageQuest");
	}
	
};
