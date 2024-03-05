/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/ItemCollection/GsSchemaItemCollectionSubCategoryAttainmentUnit.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaItemCollection.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="아이템컬렉션 데이터 테이블", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/ItemCollection/BSGsItemCollection"))
struct DATACENTER_API FGsSchemaItemCollection
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="durationTime"))
	int32 durationTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="startDateTime"))
	FString startDateTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="endDateTime"))
	FString endDateTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passivityIds"))
	TArray<int32> passivityIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="AttainmentUnit"))
	FGsSchemaItemCollectionSubCategoryAttainmentUnitRow AttainmentUnit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventType"))
	ItemCollectionEventType eventType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(ClampMin="1", ClampMax="5", DisplayName="slotCount"))
	int32 slotCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemConditionId0"))
	int32 slotItemConditionId0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemIds0"))
	TArray<int32> slotItemIds0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemLevels0"))
	TArray<int32> slotItemLevels0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemCount0"))
	int32 slotItemCount0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemConditionId1"))
	int32 slotItemConditionId1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemIds1"))
	TArray<int32> slotItemIds1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemLevels1"))
	TArray<int32> slotItemLevels1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemCount1"))
	int32 slotItemCount1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemConditionId2"))
	int32 slotItemConditionId2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemIds2"))
	TArray<int32> slotItemIds2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemLevels2"))
	TArray<int32> slotItemLevels2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemCount2"))
	int32 slotItemCount2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemConditionId3"))
	int32 slotItemConditionId3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemIds3"))
	TArray<int32> slotItemIds3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemLevels3"))
	TArray<int32> slotItemLevels3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemCount3"))
	int32 slotItemCount3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemConditionId4"))
	int32 slotItemConditionId4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemIds4"))
	TArray<int32> slotItemIds4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemLevels4"))
	TArray<int32> slotItemLevels4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemCount4"))
	int32 slotItemCount4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemConditionId5"))
	int32 slotItemConditionId5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemIds5"))
	TArray<int32> slotItemIds5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemLevels5"))
	TArray<int32> slotItemLevels5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemCount5"))
	int32 slotItemCount5;
public:
	FGsSchemaItemCollection();
	
protected:
	bool operator== (const FGsSchemaItemCollection& __Other) const
	{
		return FGsSchemaItemCollection::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaItemCollection::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaItemCollection::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaItemCollectionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaItemCollectionRow() : Super()
	{
	}
	
	const FGsSchemaItemCollection* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaItemCollection* Row = nullptr;
		return Table->FindRow<FGsSchemaItemCollection>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaItemCollectionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaItemCollectionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableItemCollection final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("ItemCollection");
	}
	
};
