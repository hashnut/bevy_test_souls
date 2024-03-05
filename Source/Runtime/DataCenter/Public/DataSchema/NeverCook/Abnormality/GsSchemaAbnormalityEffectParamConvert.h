/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAbnormalityEffectParamConvert.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Abnormality/BSGsAbnormalityEffectParamConvert"))
struct DATACENTER_API FGsSchemaAbnormalityEffectParamConvert
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="abnormalityEffectType"))
	AbnormalityEffectType abnormalityEffectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="convertType"))
	EGsAbnormalityEffectConvertParamType convertType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="convertTableName"))
	FString convertTableName;
public:
	FGsSchemaAbnormalityEffectParamConvert();
	
protected:
	bool operator== (const FGsSchemaAbnormalityEffectParamConvert& __Other) const
	{
		return FGsSchemaAbnormalityEffectParamConvert::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAbnormalityEffectParamConvert::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAbnormalityEffectParamConvert::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAbnormalityEffectParamConvertRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAbnormalityEffectParamConvertRow() : Super()
	{
	}
	
	const FGsSchemaAbnormalityEffectParamConvert* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAbnormalityEffectParamConvert* Row = nullptr;
		return Table->FindRow<FGsSchemaAbnormalityEffectParamConvert>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAbnormalityEffectParamConvertRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAbnormalityEffectParamConvertRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAbnormalityEffectParamConvert final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<AbnormalityEffectType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Abnormality");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaAbnormalityEffectParamConvert*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaAbnormalityEffectParamConvert>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->abnormalityEffectType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const AbnormalityEffectType& InId, OUT const FGsSchemaAbnormalityEffectParamConvert*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const AbnormalityEffectType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaAbnormalityEffectParamConvert* Temp;
		return !FindRowById(FGsSchemaAbnormalityEffectParamConvert().abnormalityEffectType, Temp);
	}
#endif
};
