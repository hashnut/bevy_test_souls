/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCustomizeColorGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Customize/BSGsCustomizeColorGroup"))
struct DATACENTER_API FGsSchemaCustomizeColorGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString desc;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int32> colorIndexList;
	
public:
	FGsSchemaCustomizeColorGroup();
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCustomizeColorGroup::StaticStruct());
	}
	
protected:
	virtual const UScriptStruct* GetThisScriptStruct() const override
	{
		return FGsSchemaCustomizeColorGroup::StaticStruct();
	}
	
	bool operator== (const FGsSchemaCustomizeColorGroup& __Other) const
	{
		return FGsSchemaCustomizeColorGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCustomizeColorGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCustomizeColorGroupRow() : Super()
	{
	}
	
	const UGsTable* GetTable() const
	{
		return FGsSchemaCustomizeColorGroup::GetStaticTable();
	}
	
	const FGsSchemaCustomizeColorGroup* GetRow() const
	{
		const FGsSchemaCustomizeColorGroup* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaCustomizeColorGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCustomizeColorGroupRow>
	: public TStructOpsTypeTraitsBase2<FGsSchemaCustomizeColorGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCustomizeColorGroup : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const override
	{
		return TEXT("Customize");
	}
	
};
