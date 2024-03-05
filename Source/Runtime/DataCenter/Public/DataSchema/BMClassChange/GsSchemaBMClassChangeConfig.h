/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/BMClassChange/GsSchemaBMClassChangeEquipCategory.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/User/GsSchemaUserSkillSet.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBMClassChangeConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BMClassChange/BSGsBMClassChangeConfig"))
struct DATACENTER_API FGsSchemaBMClassChangeConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeMinGrade"))
	CostumeGrade costumeMinGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="equipMinGrade"))
	ItemGrade equipMinGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="equipCategoryList"))
	TArray<FGsSchemaBMClassChangeEquipCategory> equipCategoryList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="excludeCostumeIdList"))
	TArray<FGsSchemaCostumeCommonRow> excludeCostumeIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="excludeItemIdList"))
	TArray<FGsSchemaItemCommonRow> excludeItemIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="excludeUserSkillSetIdList"))
	TArray<FGsSchemaUserSkillSetRow> excludeUserSkillSetIdList;
public:
	FGsSchemaBMClassChangeConfig();
	
protected:
	bool operator== (const FGsSchemaBMClassChangeConfig& __Other) const
	{
		return FGsSchemaBMClassChangeConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBMClassChangeConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBMClassChangeConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBMClassChangeConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBMClassChangeConfigRow() : Super()
	{
	}
	
	const FGsSchemaBMClassChangeConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBMClassChangeConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaBMClassChangeConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBMClassChangeConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBMClassChangeConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBMClassChangeConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BMClassChange");
	}
	
};
