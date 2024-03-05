/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Title/GsSchemaTitleSubCategory.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTitleExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Title/BSGsTitleExcel"))
struct DATACENTER_API FGsSchemaTitleExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="subCategoryRowId"))
	FGsSchemaTitleSubCategoryRow subCategoryRowId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	TitleGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="equipSlotType"))
	TitleEquipSlot equipSlotType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passivitySetId"))
	FGsSchemaPassivitySetRow passivitySetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passivityDesc"))
	FText passivityDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FText desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isProgressDisplayFlag"))
	bool isProgressDisplayFlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="retainType"))
	TitleRetainType retainType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="retainCheckType"))
	TitleConditionCheckType retainCheckType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionType"))
	TitleConditionType conditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionDesc"))
	FString conditionDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionalCount"))
	int32 conditionalCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionTableType"))
	FString conditionTableType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionTableRowName"))
	FString conditionTableRowName;
public:
	FGsSchemaTitleExcel();
	
protected:
	bool operator== (const FGsSchemaTitleExcel& __Other) const
	{
		return FGsSchemaTitleExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTitleExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTitleExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTitleExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTitleExcelRow() : Super()
	{
	}
	
	const FGsSchemaTitleExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTitleExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaTitleExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTitleExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTitleExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTitleExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Title");
	}
	
};
