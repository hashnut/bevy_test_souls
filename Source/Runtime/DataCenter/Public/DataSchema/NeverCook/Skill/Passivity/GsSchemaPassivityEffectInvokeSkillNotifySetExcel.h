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
#include "GsSchemaPassivityEffectInvokeSkillNotifySetExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Skill/Passivity/BSGsPassivityEffectInvokeSkillNotifySetExcel"))
struct DATACENTER_API FGsSchemaPassivityEffectInvokeSkillNotifySetExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ratePct"))
	int32 ratePct;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rateCondition"))
	int32 rateCondition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cause"))
	PassivityCauseType cause;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cooldownTime"))
	float cooldownTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="target"))
	PassivityInvokeTargetType target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillNotifyId"))
	int32 skillNotifyId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeParamId"))
	int32 causeParamId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeParam1"))
	AbnormalityEffectType causeParam1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeParam2"))
	ItemCategorySub causeParam2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeParam3"))
	ItemGrade causeParam3;
public:
	FGsSchemaPassivityEffectInvokeSkillNotifySetExcel();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectInvokeSkillNotifySetExcel& __Other) const
	{
		return FGsSchemaPassivityEffectInvokeSkillNotifySetExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassivityEffectInvokeSkillNotifySetExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassivityEffectInvokeSkillNotifySetExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassivityEffectInvokeSkillNotifySetExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassivityEffectInvokeSkillNotifySetExcelRow() : Super()
	{
	}
	
	const FGsSchemaPassivityEffectInvokeSkillNotifySetExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassivityEffectInvokeSkillNotifySetExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaPassivityEffectInvokeSkillNotifySetExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassivityEffectInvokeSkillNotifySetExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassivityEffectInvokeSkillNotifySetExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassivityEffectInvokeSkillNotifySetExcel final : public UGsTable
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
		TMap<FName, const FGsSchemaPassivityEffectInvokeSkillNotifySetExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPassivityEffectInvokeSkillNotifySetExcel>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaPassivityEffectInvokeSkillNotifySetExcel*& OutRow) const
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
		const FGsSchemaPassivityEffectInvokeSkillNotifySetExcel* Temp;
		return !FindRowById(FGsSchemaPassivityEffectInvokeSkillNotifySetExcel().id, Temp);
	}
#endif
};
