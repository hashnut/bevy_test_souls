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
#include "GsSchemaPassivityEffectInvokeCooldownSetExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Skill/Passivity/BsGsPassivityEffectInvokeCooldownSetExcel"))
struct DATACENTER_API FGsSchemaPassivityEffectInvokeCooldownSetExcel
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="targetAllUserSkillSetId"))
	bool targetAllUserSkillSetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="targetUserSkillSetIdList"))
	TArray<int32> targetUserSkillSetIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="targetCooldownRate"))
	int32 targetCooldownRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeParamId"))
	int32 causeParamId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeParam1"))
	AbnormalityEffectType causeParam1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeParam2"))
	ItemCategorySub causeParam2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="causeParam3"))
	ItemGrade causeParam3;
public:
	FGsSchemaPassivityEffectInvokeCooldownSetExcel();
	
protected:
	bool operator== (const FGsSchemaPassivityEffectInvokeCooldownSetExcel& __Other) const
	{
		return FGsSchemaPassivityEffectInvokeCooldownSetExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassivityEffectInvokeCooldownSetExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassivityEffectInvokeCooldownSetExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassivityEffectInvokeCooldownSetExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassivityEffectInvokeCooldownSetExcelRow() : Super()
	{
	}
	
	const FGsSchemaPassivityEffectInvokeCooldownSetExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassivityEffectInvokeCooldownSetExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaPassivityEffectInvokeCooldownSetExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassivityEffectInvokeCooldownSetExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassivityEffectInvokeCooldownSetExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassivityEffectInvokeCooldownSetExcel final : public UGsTable
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
		TMap<FName, const FGsSchemaPassivityEffectInvokeCooldownSetExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPassivityEffectInvokeCooldownSetExcel>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaPassivityEffectInvokeCooldownSetExcel*& OutRow) const
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
		const FGsSchemaPassivityEffectInvokeCooldownSetExcel* Temp;
		return !FindRowById(FGsSchemaPassivityEffectInvokeCooldownSetExcel().id, Temp);
	}
#endif
};
