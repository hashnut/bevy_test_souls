/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Resurrect/GsSchemaResurrectPaneltyInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaResurrectPaneltyGroup.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Resurrect/BSGsResurrectPaneltyGroup"))
struct DATACENTER_API FGsSchemaResurrectPaneltyGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	int32 id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="exp", Tooltip))
	FGsSchemaResurrectPaneltyInfo exp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gold", Tooltip))
	FGsSchemaResurrectPaneltyInfo gold;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Item", Tooltip))
	TArray<FGsSchemaResurrectPaneltyInfo> Item;
	
public:
	FGsSchemaResurrectPaneltyGroup();
	
protected:
	bool operator== (const FGsSchemaResurrectPaneltyGroup& __Other) const
	{
		return FGsSchemaResurrectPaneltyGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaResurrectPaneltyGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaResurrectPaneltyGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaResurrectPaneltyGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaResurrectPaneltyGroupRow() : Super()
	{
	}
	
	const FGsSchemaResurrectPaneltyGroup* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaResurrectPaneltyGroup* Row = nullptr;
		return Table->FindRow<FGsSchemaResurrectPaneltyGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaResurrectPaneltyGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaResurrectPaneltyGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableResurrectPaneltyGroup final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Resurrect");
	}
	
};
