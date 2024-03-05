/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaDeviceRegisterConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Config/BSGsDeviceRegisterConfig"))
struct DATACENTER_API FGsSchemaDeviceRegisterConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="DeviceRegisterRewardPassivityList"))
	TArray<int32> DeviceRegisterRewardPassivityList;
public:
	FGsSchemaDeviceRegisterConfig();
	
protected:
	bool operator== (const FGsSchemaDeviceRegisterConfig& __Other) const
	{
		return FGsSchemaDeviceRegisterConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaDeviceRegisterConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaDeviceRegisterConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaDeviceRegisterConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaDeviceRegisterConfigRow() : Super()
	{
	}
	
	const FGsSchemaDeviceRegisterConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaDeviceRegisterConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaDeviceRegisterConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaDeviceRegisterConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaDeviceRegisterConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableDeviceRegisterConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Config");
	}
	
};
