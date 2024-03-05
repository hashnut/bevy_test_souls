/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Option/GsSchemaOptionToggle.h"
#include "Runtime/DataCenter/Public/Option/GsOptionEnumType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionSlider.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionSlider"))
struct DATACENTER_API FGsSchemaOptionSlider
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="MinValue"))
	float MinValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="MaxValue"))
	float MaxValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="textType"))
	EGsSliderTextType textType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="PercentShow"))
	bool PercentShow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="toggleList"))
	TArray<FGsSchemaOptionToggle> toggleList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="OnOffItemType"))
	EGsOptionItem OnOffItemType;
public:
	FGsSchemaOptionSlider();
	
protected:
	bool operator== (const FGsSchemaOptionSlider& __Other) const
	{
		return FGsSchemaOptionSlider::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOptionSlider::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOptionSlider::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOptionSliderRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOptionSliderRow() : Super()
	{
	}
	
	const FGsSchemaOptionSlider* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOptionSlider* Row = nullptr;
		return Table->FindRow<FGsSchemaOptionSlider>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOptionSliderRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOptionSliderRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOptionSlider final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
};
