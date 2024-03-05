/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Item/GsStatTypeNameSelector.h"
#include "GsSchemaSkillLevelSetInfoExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Skill/Level/BSGsSkillLevelSetInfoExcel"))
struct DATACENTER_API FGsSchemaSkillLevelSetInfoExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="skillId", Tooltip="기획용 설명"))
	int32 skillId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="cooldownTime", Tooltip="기획 확인용"))
	float cooldownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enchantSkillUpCostId"))
	int32 enchantSkillUpCostId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="notifyInfoList"))
	TArray<int32> notifyInfoList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="EffectText"))
	FText EffectText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireItemId1"))
	int32 requireItemId1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireAmount1"))
	int32 requireAmount1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireItemId2"))
	int32 requireItemId2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="requireAmount2"))
	int32 requireAmount2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType1"))
	FGsStatTypeNameSelector statType1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue1"))
	int32 statValue1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType2"))
	FGsStatTypeNameSelector statType2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue2"))
	int32 statValue2;
public:
	FGsSchemaSkillLevelSetInfoExcel();
	
protected:
	bool operator== (const FGsSchemaSkillLevelSetInfoExcel& __Other) const
	{
		return FGsSchemaSkillLevelSetInfoExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillLevelSetInfoExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillLevelSetInfoExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillLevelSetInfoExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillLevelSetInfoExcelRow() : Super()
	{
	}
	
	const FGsSchemaSkillLevelSetInfoExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillLevelSetInfoExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillLevelSetInfoExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillLevelSetInfoExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillLevelSetInfoExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillLevelSetInfoExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Skill/Level");
	}
	
};
