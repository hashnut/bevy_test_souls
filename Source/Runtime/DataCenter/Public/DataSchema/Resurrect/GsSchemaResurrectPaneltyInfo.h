/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaResurrectPaneltyInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Resurrect/BSGsResurrectPaneltyInfo"))
struct DATACENTER_API FGsSchemaResurrectPaneltyInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Meta=(Tooltip="exp 시 읽는 경험치 퍼센트, 골드시 잃는골드량, 아이템시 슬롯 인덱스"))
	int64 param;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="percentage", Tooltip))
	float percentage;
	
public:
	FGsSchemaResurrectPaneltyInfo();
	
protected:
	bool operator== (const FGsSchemaResurrectPaneltyInfo& __Other) const
	{
		return FGsSchemaResurrectPaneltyInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaResurrectPaneltyInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaResurrectPaneltyInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaResurrectPaneltyInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaResurrectPaneltyInfoRow() : Super()
	{
	}
	
	const FGsSchemaResurrectPaneltyInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaResurrectPaneltyInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaResurrectPaneltyInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaResurrectPaneltyInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaResurrectPaneltyInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableResurrectPaneltyInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Resurrect");
	}
	
};
