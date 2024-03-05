/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCustomizeTattooData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Customize/BsGsCustomizeTattooData"))
struct DATACENTER_API FGsSchemaCustomizeTattooData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="texturePath", Tooltip))
	FSoftObjectPath texturePath;
	
public:
	FGsSchemaCustomizeTattooData();
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCustomizeTattooData::StaticStruct());
	}
	
protected:
	virtual const UScriptStruct* GetThisScriptStruct() const override
	{
		return FGsSchemaCustomizeTattooData::StaticStruct();
	}
	
	bool operator== (const FGsSchemaCustomizeTattooData& __Other) const
	{
		return FGsSchemaCustomizeTattooData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCustomizeTattooDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCustomizeTattooDataRow() : Super()
	{
	}
	
	const UGsTable* GetTable() const
	{
		return FGsSchemaCustomizeTattooData::GetStaticTable();
	}
	
	const FGsSchemaCustomizeTattooData* GetRow() const
	{
		const FGsSchemaCustomizeTattooData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaCustomizeTattooData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCustomizeTattooDataRow>
	: public TStructOpsTypeTraitsBase2<FGsSchemaCustomizeTattooDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCustomizeTattooData : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const override
	{
		return TEXT("Customize");
	}
	
};
