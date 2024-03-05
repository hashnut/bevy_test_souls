/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Resurrect/GsSchemaResurrectPaneltyGroup.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"
#include "GsSchemaResurrectInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Resurrect/BSGsResurrectInfo"))
struct DATACENTER_API FGsSchemaResurrectInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="ResurrectType", Tooltip))
	ResurrectType ResurrectType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="description", Tooltip))
	FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="currencyType", Tooltip))
	CurrencyType currencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cost", Tooltip))
	int32 cost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hpRate", Tooltip))
	float hpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mpRate", Tooltip))
	float mpRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invncibleTime", Tooltip))
	int32 invncibleTime;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaAbnormalitySetRow abnormalityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="canRecovery", Tooltip))
	bool canRecovery;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="recoveryTime", Tooltip))
	int32 recoveryTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="panelty", Tooltip))
	TArray<FGsSchemaResurrectPaneltyGroup> panelty;
public:
	FGsSchemaResurrectInfo();
	
protected:
	bool operator== (const FGsSchemaResurrectInfo& __Other) const
	{
		return FGsSchemaResurrectInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaResurrectInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaResurrectInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaResurrectInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaResurrectInfoRow() : Super()
	{
	}
	
	const FGsSchemaResurrectInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaResurrectInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaResurrectInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaResurrectInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaResurrectInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableResurrectInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Resurrect");
	}
	
};
