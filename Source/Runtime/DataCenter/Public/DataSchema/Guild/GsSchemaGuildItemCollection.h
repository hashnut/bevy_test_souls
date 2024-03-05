/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Guild/GsSchemaGuildItemCollectionInnerSlotProperty.h"
#include "Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "GsSchemaGuildItemCollection.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="길드 아이템 컬렉션 테이블", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildItemCollection"))
struct DATACENTER_API FGsSchemaGuildItemCollection
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="type"))
	GuildItemCollectionType type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enableTimeSetStart"))
	FString enableTimeSetStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enableTimeSetEnd"))
	FString enableTimeSetEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="completeRewardPassivityList"))
	TArray<int32> completeRewardPassivityList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemConditionId0"))
	int32 slotItemConditionId0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="slotProperty0"))
	FGsSchemaGuildItemCollectionInnerSlotPropertyRow slotProperty0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemConditionId1"))
	int32 slotItemConditionId1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="slotProperty1"))
	FGsSchemaGuildItemCollectionInnerSlotPropertyRow slotProperty1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemConditionId2"))
	int32 slotItemConditionId2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="slotProperty2"))
	FGsSchemaGuildItemCollectionInnerSlotPropertyRow slotProperty2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemConditionId3"))
	int32 slotItemConditionId3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="slotProperty3"))
	FGsSchemaGuildItemCollectionInnerSlotPropertyRow slotProperty3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemConditionId4"))
	int32 slotItemConditionId4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="slotProperty4"))
	FGsSchemaGuildItemCollectionInnerSlotPropertyRow slotProperty4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="slotItemConditionId5"))
	int32 slotItemConditionId5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="slotProperty5"))
	FGsSchemaGuildItemCollectionInnerSlotPropertyRow slotProperty5;
public:
	FGsSchemaGuildItemCollection();
	
protected:
	bool operator== (const FGsSchemaGuildItemCollection& __Other) const
	{
		return FGsSchemaGuildItemCollection::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGuildItemCollection::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaGuildItemCollection::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGuildItemCollectionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGuildItemCollectionRow() : Super()
	{
	}
	
	const FGsSchemaGuildItemCollection* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaGuildItemCollection* Row = nullptr;
		return Table->FindRow<FGsSchemaGuildItemCollection>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGuildItemCollectionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaGuildItemCollectionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGuildItemCollection final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild");
	}
	
};
