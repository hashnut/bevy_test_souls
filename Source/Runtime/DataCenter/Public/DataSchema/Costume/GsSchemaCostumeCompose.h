/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeComposeRate.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeProbability.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeCompose.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeCompose"))
struct DATACENTER_API FGsSchemaCostumeCompose
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	CostumeGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costGold"))
	int32 costGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="upgradeRateList"))
	TArray<FGsSchemaCostumeComposeRate> upgradeRateList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="successProbabilityId"))
	FGsSchemaCostumeProbabilityRow successProbabilityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="failureProbabilityId"))
	FGsSchemaCostumeProbabilityRow failureProbabilityId;
public:
	FGsSchemaCostumeCompose();
	
protected:
	bool operator== (const FGsSchemaCostumeCompose& __Other) const
	{
		return FGsSchemaCostumeCompose::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCostumeCompose::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCostumeCompose::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCostumeComposeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCostumeComposeRow() : Super()
	{
	}
	
	const FGsSchemaCostumeCompose* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCostumeCompose* Row = nullptr;
		return Table->FindRow<FGsSchemaCostumeCompose>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCostumeComposeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCostumeComposeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCostumeCompose final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Costume");
	}
	
};
