/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Option/GsOptionEnumType.h"
#include "Runtime/DataCenter/Public/Management/ScopeGlobal/EGsOptionPopupType.h"
#include "Runtime/DataCenter/Public/DataSchema/Option/GsSchemaOptionItemGroup.h"
#include "Runtime/DataCenter/Public/DataSchema/Option/GsSchemaOptionItem.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionDivision.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionDivision"))
struct DATACENTER_API FGsSchemaOptionDivision
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="NameText"))
	FText NameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="OptionGroup"))
	FGsSchemaOptionItemGroupRow OptionGroup;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="OptionList"))
	TArray<FGsSchemaOptionItemRow> OptionList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isPopup"))
	bool isPopup;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="popupType"))
	EGsOptionPopupType popupType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="OptionExceptionPlatformTypeList"))
	TArray<EGsOptionPlatformType> OptionExceptionPlatformTypeList;
public:
	FGsSchemaOptionDivision();
	
protected:
	bool operator== (const FGsSchemaOptionDivision& __Other) const
	{
		return FGsSchemaOptionDivision::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOptionDivision::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOptionDivision::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOptionDivisionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOptionDivisionRow() : Super()
	{
	}
	
	const FGsSchemaOptionDivision* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOptionDivision* Row = nullptr;
		return Table->FindRow<FGsSchemaOptionDivision>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOptionDivisionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOptionDivisionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOptionDivision final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
};
