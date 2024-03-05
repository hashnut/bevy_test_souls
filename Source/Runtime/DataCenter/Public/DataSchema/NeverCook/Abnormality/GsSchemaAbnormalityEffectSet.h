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
#include "GsSchemaAbnormalityEffectSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Abnormality/BSGsAbnormalityEffectSet"))
struct DATACENTER_API FGsSchemaAbnormalityEffectSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="abnormalityEffectType"))
	AbnormalityEffectType abnormalityEffectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="animationTypeInfo"))
	EGsAbnormalityAnimProcessType animationTypeInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Desc"))
	FString Desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param1"))
	FString param1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param2"))
	FString param2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param3"))
	FString param3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param4"))
	FString param4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param5"))
	FString param5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param6"))
	FString param6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="param7"))
	FString param7;
public:
	FGsSchemaAbnormalityEffectSet();
	
protected:
	bool operator== (const FGsSchemaAbnormalityEffectSet& __Other) const
	{
		return FGsSchemaAbnormalityEffectSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAbnormalityEffectSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAbnormalityEffectSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAbnormalityEffectSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAbnormalityEffectSetRow() : Super()
	{
	}
	
	const FGsSchemaAbnormalityEffectSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAbnormalityEffectSet* Row = nullptr;
		return Table->FindRow<FGsSchemaAbnormalityEffectSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAbnormalityEffectSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAbnormalityEffectSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAbnormalityEffectSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Abnormality");
	}
	
};
