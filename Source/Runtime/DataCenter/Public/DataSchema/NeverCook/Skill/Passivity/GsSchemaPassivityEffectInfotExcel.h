/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityEffectInfotExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Skill/Passivity/BSGsPassivityEffectInfotExcel"))
struct DATACENTER_API FGsSchemaPassivityEffectInfotExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectId"))
	int32 effectId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="effectInfoGroupId"))
	int32 effectInfoGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionInfoType"))
	PassivityConditionType conditionInfoType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionInfoMethod"))
	PassivityConditionMethod conditionInfoMethod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param1"))
	int32 param1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param2"))
	int32 param2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param3"))
	AbnormalityEffectType param3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="executeConditionId"))
	int32 executeConditionId;
public:
	FGsSchemaPassivityEffectInfotExcel();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectInfotExcel& __Other) const
	{
		return FGsSchemaPassivityEffectInfotExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassivityEffectInfotExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassivityEffectInfotExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassivityEffectInfotExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassivityEffectInfotExcelRow() : Super()
	{
	}
	
	const FGsSchemaPassivityEffectInfotExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassivityEffectInfotExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaPassivityEffectInfotExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassivityEffectInfotExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassivityEffectInfotExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassivityEffectInfotExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Skill/Passivity");
	}
	
};
