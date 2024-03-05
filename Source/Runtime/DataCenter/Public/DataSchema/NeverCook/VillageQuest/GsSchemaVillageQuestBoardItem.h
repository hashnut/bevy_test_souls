/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Quest/Main/GsSchemaQuestMainStory.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedQuestEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaVillageQuestBoardItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/VillageQuest/BSGsVillageQuestBoardItem"))
struct DATACENTER_API FGsSchemaVillageQuestBoardItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ParentID"))
	int32 ParentID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="QuestMainStory"))
	FGsSchemaQuestMainStoryRow QuestMainStory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="QuestBoardType"))
	QuestBoardType QuestBoardType;
public:
	FGsSchemaVillageQuestBoardItem();
	
protected:
	bool operator== (const FGsSchemaVillageQuestBoardItem& __Other) const
	{
		return FGsSchemaVillageQuestBoardItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaVillageQuestBoardItem::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaVillageQuestBoardItem::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaVillageQuestBoardItemRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaVillageQuestBoardItemRow() : Super()
	{
	}
	
	const FGsSchemaVillageQuestBoardItem* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaVillageQuestBoardItem* Row = nullptr;
		return Table->FindRow<FGsSchemaVillageQuestBoardItem>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaVillageQuestBoardItemRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaVillageQuestBoardItemRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableVillageQuestBoardItem final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/VillageQuest");
	}
	
};
