/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Management/ScopeGlobal/EGsOptionPopupType.h"
#include "Runtime/DataCenter/Public/DataSchema/Option/GsSchemaOptionDivision.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionCategory.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionCategory"))
struct DATACENTER_API FGsSchemaOptionCategory
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="IsDivision"))
	bool IsDivision;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="DivisionList"))
	TArray<FGsSchemaOptionDivisionRow> DivisionList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isPopup"))
	bool isPopup;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="popupType"))
	EGsOptionPopupType popupType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isDevelopment"))
	bool isDevelopment;
public:
	FGsSchemaOptionCategory();
	
protected:
	bool operator== (const FGsSchemaOptionCategory& __Other) const
	{
		return FGsSchemaOptionCategory::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOptionCategory::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOptionCategory::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOptionCategoryRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOptionCategoryRow() : Super()
	{
	}
	
	const FGsSchemaOptionCategory* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOptionCategory* Row = nullptr;
		return Table->FindRow<FGsSchemaOptionCategory>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOptionCategoryRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOptionCategoryRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOptionCategory final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
};
