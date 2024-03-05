/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsSchemaGuildItemCollectionInnerSlotProperty.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildItemCollectionInnerSlotProperty"))
struct DATACENTER_API FGsSchemaGuildItemCollectionInnerSlotProperty
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="categorySubType"))
	ItemCategorySub categorySubType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	ItemGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantCount"))
	int32 enchantCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardId"))
	int32 rewardId;
public:
	FGsSchemaGuildItemCollectionInnerSlotProperty();
	
protected:
	bool operator== (const FGsSchemaGuildItemCollectionInnerSlotProperty& __Other) const
	{
		return FGsSchemaGuildItemCollectionInnerSlotProperty::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGuildItemCollectionInnerSlotProperty::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaGuildItemCollectionInnerSlotProperty::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGuildItemCollectionInnerSlotPropertyRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGuildItemCollectionInnerSlotPropertyRow() : Super()
	{
	}
	
	const FGsSchemaGuildItemCollectionInnerSlotProperty* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaGuildItemCollectionInnerSlotProperty* Row = nullptr;
		return Table->FindRow<FGsSchemaGuildItemCollectionInnerSlotProperty>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGuildItemCollectionInnerSlotPropertyRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaGuildItemCollectionInnerSlotPropertyRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGuildItemCollectionInnerSlotProperty final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild");
	}
	
};
