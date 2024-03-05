/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Management/ScopeGlobal/EGsOptionTextColorType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionButtonGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionButtonGroup"))
struct DATACENTER_API FGsSchemaOptionButtonGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameTextColorType"))
	EGsOptionTextColorType nameTextColorType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="subText"))
	FText subText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="buttonList"))
	TArray<FString> buttonList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="buttonTextList"))
	TArray<FText> buttonTextList;
public:
	FGsSchemaOptionButtonGroup();
	
protected:
	bool operator== (const FGsSchemaOptionButtonGroup& __Other) const
	{
		return FGsSchemaOptionButtonGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOptionButtonGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOptionButtonGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOptionButtonGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOptionButtonGroupRow() : Super()
	{
	}
	
	const FGsSchemaOptionButtonGroup* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOptionButtonGroup* Row = nullptr;
		return Table->FindRow<FGsSchemaOptionButtonGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOptionButtonGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOptionButtonGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOptionButtonGroup final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
};
