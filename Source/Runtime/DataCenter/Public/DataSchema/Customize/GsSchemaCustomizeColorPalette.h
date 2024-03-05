/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCustomizeColorPalette.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Customize/BSGsCustomizeColorPalette"))
struct DATACENTER_API FGsSchemaCustomizeColorPalette
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FColor> colorList;
	
public:
	FGsSchemaCustomizeColorPalette();
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCustomizeColorPalette::StaticStruct());
	}
	
protected:
	virtual const UScriptStruct* GetThisScriptStruct() const override
	{
		return FGsSchemaCustomizeColorPalette::StaticStruct();
	}
	
	bool operator== (const FGsSchemaCustomizeColorPalette& __Other) const
	{
		return FGsSchemaCustomizeColorPalette::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCustomizeColorPaletteRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCustomizeColorPaletteRow() : Super()
	{
	}
	
	const UGsTable* GetTable() const
	{
		return FGsSchemaCustomizeColorPalette::GetStaticTable();
	}
	
	const FGsSchemaCustomizeColorPalette* GetRow() const
	{
		const FGsSchemaCustomizeColorPalette* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaCustomizeColorPalette>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCustomizeColorPaletteRow>
	: public TStructOpsTypeTraitsBase2<FGsSchemaCustomizeColorPaletteRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCustomizeColorPalette : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const override
	{
		return TEXT("Customize");
	}
	
};
