/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/DataCenter/Public/Option/GsOptionEnumType.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionItem"))
struct DATACENTER_API FGsSchemaOptionItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="OptionSelectType"))
	EGsOptionSelectType OptionSelectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="OptionItemType"))
	EGsOptionItem OptionItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="OptionExceptionPlatformTypeList"))
	TArray<EGsOptionPlatformType> OptionExceptionPlatformTypeList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="OptionDisableList"))
	TArray<EGsOptionPlatformType> OptionDisableList;
public:
	FGsSchemaOptionItem();
	
protected:
	bool operator== (const FGsSchemaOptionItem& __Other) const
	{
		return FGsSchemaOptionItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOptionItem::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOptionItem::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOptionItemRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOptionItemRow() : Super()
	{
	}
	
	const FGsSchemaOptionItem* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOptionItem* Row = nullptr;
		return Table->FindRow<FGsSchemaOptionItem>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOptionItemRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOptionItemRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOptionItem final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
};
