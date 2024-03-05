/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillLevelSetExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Skill/Level/BSGsSkillLevelSetExcel"))
struct DATACENTER_API FGsSchemaSkillLevelSetExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Desc"))
	FString Desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="SkillId"))
	int32 SkillId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxEnchantLevel"))
	int32 maxEnchantLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxUpgradeStep"))
	int32 maxUpgradeStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spiritShotConsumption"))
	int32 spiritShotConsumption;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currentStep"))
	int32 currentStep;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillUpgradeId"))
	FName skillUpgradeId;
public:
	FGsSchemaSkillLevelSetExcel();
	
protected:
	bool operator== (const FGsSchemaSkillLevelSetExcel& __Other) const
	{
		return FGsSchemaSkillLevelSetExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillLevelSetExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillLevelSetExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillLevelSetExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillLevelSetExcelRow() : Super()
	{
	}
	
	const FGsSchemaSkillLevelSetExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillLevelSetExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillLevelSetExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillLevelSetExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillLevelSetExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillLevelSetExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Skill/Level");
	}
	
};
