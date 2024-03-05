/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOptionConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Option/BSGsOptionConfig"))
struct DATACENTER_API FGsSchemaOptionConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_windowResolutionMode1"))
	FIntPoint _windowResolutionMode1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_windowResolutionMode2"))
	FIntPoint _windowResolutionMode2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_windowResolutionMode3"))
	FIntPoint _windowResolutionMode3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_keyMappingParentTabIndex"))
	int32 _keyMappingParentTabIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_keyMappingChildTabIndex"))
	int32 _keyMappingChildTabIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="_keyMappingScrollIndex"))
	float _keyMappingScrollIndex;
public:
	FGsSchemaOptionConfig();
	
protected:
	bool operator== (const FGsSchemaOptionConfig& __Other) const
	{
		return FGsSchemaOptionConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOptionConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOptionConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOptionConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOptionConfigRow() : Super()
	{
	}
	
	const FGsSchemaOptionConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOptionConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaOptionConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOptionConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOptionConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOptionConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Option");
	}
	
};
