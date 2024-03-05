/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassivityInvokeAbnormalitySetExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Skill/Passivity/BSGsPassivityInvokeAbnormalitySetExcel"))
struct DATACENTER_API FGsSchemaPassivityInvokeAbnormalitySetExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ratePct"))
	int32 ratePct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cause"))
	PassivityCauseType cause;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="CauseInfoDataId"))
	FName CauseInfoDataId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cooldownTime"))
	float cooldownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="target"))
	PassivityInvokeTargetType target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="abnormalityId"))
	int32 abnormalityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeParamId"))
	int32 causeParamId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeParam1"))
	AbnormalityEffectType causeParam1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeParam2"))
	ItemCategorySub causeParam2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeParam3"))
	ItemGrade causeParam3;
public:
	FGsSchemaPassivityInvokeAbnormalitySetExcel();
	
protected:
	bool operator== (const FGsSchemaPassivityInvokeAbnormalitySetExcel& __Other) const
	{
		return FGsSchemaPassivityInvokeAbnormalitySetExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassivityInvokeAbnormalitySetExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassivityInvokeAbnormalitySetExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassivityInvokeAbnormalitySetExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassivityInvokeAbnormalitySetExcelRow() : Super()
	{
	}
	
	const FGsSchemaPassivityInvokeAbnormalitySetExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassivityInvokeAbnormalitySetExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaPassivityInvokeAbnormalitySetExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassivityInvokeAbnormalitySetExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassivityInvokeAbnormalitySetExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassivityInvokeAbnormalitySetExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Skill/Passivity");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaPassivityInvokeAbnormalitySetExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPassivityInvokeAbnormalitySetExcel>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaPassivityInvokeAbnormalitySetExcel*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaPassivityInvokeAbnormalitySetExcel* Temp;
		return !FindRowById(FGsSchemaPassivityInvokeAbnormalitySetExcel().id, Temp);
	}
#endif
};
