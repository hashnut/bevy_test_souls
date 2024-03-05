/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedGsOptionItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionItemGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionItemGroup"))
struct DATACENTER_API FGsSchemaOptionItemGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="textMain"))
	FText textMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="textSub"))
	FText textSub;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="optionItemType"))
	EGsOptionItem optionItemType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isToggleOn"))
	bool isToggleOn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="textOn"))
	FText textOn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="textOff"))
	FText textOff;
public:
	FGsSchemaOptionItemGroup();
	
protected:
	bool operator== (const FGsSchemaOptionItemGroup& __Other) const
	{
		return FGsSchemaOptionItemGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOptionItemGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOptionItemGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOptionItemGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOptionItemGroupRow() : Super()
	{
	}
	
	const FGsSchemaOptionItemGroup* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOptionItemGroup* Row = nullptr;
		return Table->FindRow<FGsSchemaOptionItemGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOptionItemGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOptionItemGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOptionItemGroup final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
};
