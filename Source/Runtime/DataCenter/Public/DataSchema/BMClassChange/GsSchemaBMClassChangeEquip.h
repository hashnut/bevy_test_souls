/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBMClassChangeEquip.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BMClassChange/BSGsBMClassChangeEquip"))
struct DATACENTER_API FGsSchemaBMClassChangeEquip
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	ItemGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tier"))
	int32 tier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costValue"))
	int32 costValue;
public:
	FGsSchemaBMClassChangeEquip();
	
protected:
	bool operator== (const FGsSchemaBMClassChangeEquip& __Other) const
	{
		return FGsSchemaBMClassChangeEquip::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBMClassChangeEquip::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBMClassChangeEquip::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBMClassChangeEquipRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBMClassChangeEquipRow() : Super()
	{
	}
	
	const FGsSchemaBMClassChangeEquip* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBMClassChangeEquip* Row = nullptr;
		return Table->FindRow<FGsSchemaBMClassChangeEquip>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBMClassChangeEquipRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBMClassChangeEquipRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBMClassChangeEquip final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BMClassChange");
	}
	
};
