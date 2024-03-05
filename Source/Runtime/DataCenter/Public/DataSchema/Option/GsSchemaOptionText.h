/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Management/ScopeGlobal/EGsOptionTextColorType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionText.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionText"))
struct DATACENTER_API FGsSchemaOptionText
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isMain"))
	bool isMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mainTextContents"))
	FText mainTextContents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mainTextColorType"))
	EGsOptionTextColorType mainTextColorType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="subTextContents"))
	FText subTextContents;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isTooltip"))
	bool isTooltip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tooltipDesc"))
	FText tooltipDesc;
public:
	FGsSchemaOptionText();
	
protected:
	bool operator== (const FGsSchemaOptionText& __Other) const
	{
		return FGsSchemaOptionText::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOptionText::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOptionText::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOptionTextRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOptionTextRow() : Super()
	{
	}
	
	const FGsSchemaOptionText* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOptionText* Row = nullptr;
		return Table->FindRow<FGsSchemaOptionText>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOptionTextRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOptionTextRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOptionText final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
};
