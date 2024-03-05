/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "Runtime/DataCenter/Public/DataSchema/Sound/GsSchemaSoundResData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaUserLookInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaInvadeConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/BSGsInvadeConfig"))
struct DATACENTER_API FGsSchemaInvadeConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="currenctItemId"))
	FGsSchemaItemCommonRow currenctItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="invadeAttackerTransformCostumeId"))
	FGsSchemaUserLookInfoRow invadeAttackerTransformCostumeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="invasionBGM"))
	FGsSchemaSoundResDataRow invasionBGM;
public:
	FGsSchemaInvadeConfig();
	
protected:
	bool operator== (const FGsSchemaInvadeConfig& __Other) const
	{
		return FGsSchemaInvadeConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaInvadeConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaInvadeConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaInvadeConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaInvadeConfigRow() : Super()
	{
	}
	
	const FGsSchemaInvadeConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaInvadeConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaInvadeConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaInvadeConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaInvadeConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableInvadeConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("InterServer");
	}
	
};
