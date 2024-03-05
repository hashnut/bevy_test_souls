/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyComposeRate.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyProbability.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyCompose.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyCompose"))
struct DATACENTER_API FGsSchemaFairyCompose
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	FairyGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costGold"))
	int32 costGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="upgradeRateList"))
	TArray<FGsSchemaFairyComposeRate> upgradeRateList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="successProbabilityId"))
	FGsSchemaFairyProbabilityRow successProbabilityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="failureProbabilityId"))
	FGsSchemaFairyProbabilityRow failureProbabilityId;
public:
	FGsSchemaFairyCompose();
	
protected:
	bool operator== (const FGsSchemaFairyCompose& __Other) const
	{
		return FGsSchemaFairyCompose::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFairyCompose::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFairyCompose::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFairyComposeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFairyComposeRow() : Super()
	{
	}
	
	const FGsSchemaFairyCompose* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFairyCompose* Row = nullptr;
		return Table->FindRow<FGsSchemaFairyCompose>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFairyComposeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFairyComposeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFairyCompose final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Fairy");
	}
	
};
