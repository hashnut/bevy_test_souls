/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Customize/GsSchemaCustomizeColorGroup.h"
#include "DataSchema/Customize/GsSchemaCustomizePartsGroup.h"
#include "DataSchema/Customize/GsSchemaCustomizePreset.h"
#include "DataSchema/Customize/GsSchemaCustomizeTattooGroup.h"
#include "GsSchemaCustomizeMenu.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Customize/BSGsCustomizeMenu"))
struct DATACENTER_API FGsSchemaCustomizeMenu
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString desc;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGsSchemaCustomizePresetRow> presetList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="facePartsGroup", Tooltip))
	FGsSchemaCustomizePartsGroupRow facePartsGroup;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hairPartsGroup", Tooltip))
	FGsSchemaCustomizePartsGroupRow hairPartsGroup;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="복장변경"))
	FGsSchemaCustomizePartsGroupRow bodyPartsGroup;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaCustomizeTattooGroupRow tattooGroup;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaCustomizeColorGroupRow skinColorGroup;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaCustomizeColorGroupRow hairColorGroup;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaCustomizeColorGroupRow pupilColorGroup;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaCustomizeColorGroupRow browColorGroup;
	
public:
	FGsSchemaCustomizeMenu();
	
protected:
	bool operator== (const FGsSchemaCustomizeMenu& __Other) const
	{
		return FGsSchemaCustomizeMenu::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCustomizeMenu::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCustomizeMenu::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCustomizeMenuRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCustomizeMenuRow() : Super()
	{
	}
	
	const FGsSchemaCustomizeMenu* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCustomizeMenu* Row = nullptr;
		return Table->FindRow<FGsSchemaCustomizeMenu>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCustomizeMenuRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCustomizeMenuRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCustomizeMenu final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Customize");
	}
	
};
