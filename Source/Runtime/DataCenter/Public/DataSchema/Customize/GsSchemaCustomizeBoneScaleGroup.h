/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCustomizeBoneScaleGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Customize/BSGsCustomizeBoneScaleGroup"))
struct DATACENTER_API FGsSchemaCustomizeBoneScaleGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Desc;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName BoneName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="VirtualParentsBone", Tooltip))
	FName VirtualParentsBone;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector ScaleVector;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector MaxVector;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector MinVector;
	
public:
	FGsSchemaCustomizeBoneScaleGroup();
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCustomizeBoneScaleGroup::StaticStruct());
	}
	
protected:
	virtual const UScriptStruct* GetThisScriptStruct() const override
	{
		return FGsSchemaCustomizeBoneScaleGroup::StaticStruct();
	}
	
	bool operator== (const FGsSchemaCustomizeBoneScaleGroup& __Other) const
	{
		return FGsSchemaCustomizeBoneScaleGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCustomizeBoneScaleGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCustomizeBoneScaleGroupRow() : Super()
	{
	}
	
	const UGsTable* GetTable() const
	{
		return FGsSchemaCustomizeBoneScaleGroup::GetStaticTable();
	}
	
	const FGsSchemaCustomizeBoneScaleGroup* GetRow() const
	{
		const FGsSchemaCustomizeBoneScaleGroup* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaCustomizeBoneScaleGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCustomizeBoneScaleGroupRow>
	: public TStructOpsTypeTraitsBase2<FGsSchemaCustomizeBoneScaleGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCustomizeBoneScaleGroup : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const override
	{
		return TEXT("Customize");
	}
	
};
