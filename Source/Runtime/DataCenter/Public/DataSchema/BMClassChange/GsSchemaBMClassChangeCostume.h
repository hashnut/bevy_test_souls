/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBMClassChangeCostume.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BMClassChange/BSGsBMClassChangeCostume"))
struct DATACENTER_API FGsSchemaBMClassChangeCostume
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="grade"))
	CostumeGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tier"))
	int32 tier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costValue"))
	int32 costValue;
public:
	FGsSchemaBMClassChangeCostume();
	
protected:
	bool operator== (const FGsSchemaBMClassChangeCostume& __Other) const
	{
		return FGsSchemaBMClassChangeCostume::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBMClassChangeCostume::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBMClassChangeCostume::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBMClassChangeCostumeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBMClassChangeCostumeRow() : Super()
	{
	}
	
	const FGsSchemaBMClassChangeCostume* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBMClassChangeCostume* Row = nullptr;
		return Table->FindRow<FGsSchemaBMClassChangeCostume>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBMClassChangeCostumeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBMClassChangeCostumeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBMClassChangeCostume final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BMClassChange");
	}
	
};
