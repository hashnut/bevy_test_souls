/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUserLevelRecoveryExpInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/Level/BSGsUserLevelRecoveryExpInfo"))
struct DATACENTER_API FGsSchemaUserLevelRecoveryExpInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expRecoveryGoldCost"))
	int32 expRecoveryGoldCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expRecoveryDiaCost"))
	int32 expRecoveryDiaCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="expRevoceryItemCost"))
	int32 expRevoceryItemCost;
public:
	FGsSchemaUserLevelRecoveryExpInfo();
	
protected:
	bool operator== (const FGsSchemaUserLevelRecoveryExpInfo& __Other) const
	{
		return FGsSchemaUserLevelRecoveryExpInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaUserLevelRecoveryExpInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaUserLevelRecoveryExpInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaUserLevelRecoveryExpInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaUserLevelRecoveryExpInfoRow() : Super()
	{
	}
	
	const FGsSchemaUserLevelRecoveryExpInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaUserLevelRecoveryExpInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaUserLevelRecoveryExpInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaUserLevelRecoveryExpInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaUserLevelRecoveryExpInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableUserLevelRecoveryExpInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("User/Level");
	}
	
};
