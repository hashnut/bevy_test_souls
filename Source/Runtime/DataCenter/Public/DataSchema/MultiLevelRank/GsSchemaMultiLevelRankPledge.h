/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/MultiLevelRank/GsMultiLevelRankType.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMultiLevelRankPledge.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/MultiLevelRank/BSGsMultiLevelRankPledge"))
struct DATACENTER_API FGsSchemaMultiLevelRankPledge
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int32 groupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="multiLevelRankGroupId"))
	int32 multiLevelRankGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="position"))
	EGsMultiLevelRankPledgePosition position;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pledgeName"))
	FText pledgeName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pledgeDesc"))
	FText pledgeDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pledgeDesc2"))
	FText pledgeDesc2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="image"))
	FSoftObjectPath image;
public:
	FGsSchemaMultiLevelRankPledge();
	
protected:
	bool operator== (const FGsSchemaMultiLevelRankPledge& __Other) const
	{
		return FGsSchemaMultiLevelRankPledge::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMultiLevelRankPledge::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMultiLevelRankPledge::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMultiLevelRankPledgeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMultiLevelRankPledgeRow() : Super()
	{
	}
	
	const FGsSchemaMultiLevelRankPledge* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMultiLevelRankPledge* Row = nullptr;
		return Table->FindRow<FGsSchemaMultiLevelRankPledge>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMultiLevelRankPledgeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMultiLevelRankPledgeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMultiLevelRankPledge final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("MultiLevelRank");
	}
	
};
