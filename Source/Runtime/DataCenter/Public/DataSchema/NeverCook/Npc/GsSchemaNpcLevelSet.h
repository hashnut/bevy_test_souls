/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/NeverCook/Npc/GsSchemaNpcLevelStatSet.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcLevelSet.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/Npc/BSGsNpcLevelSet"))
struct DATACENTER_API FGsSchemaNpcLevelSet
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Id"))
	int32 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="RewardId"))
	FGsSchemaRewardDataRow RewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="levelStatSetId"))
	FGsSchemaNpcLevelStatSetRow levelStatSetId;
public:
	FGsSchemaNpcLevelSet();
	
protected:
	bool operator== (const FGsSchemaNpcLevelSet& __Other) const
	{
		return FGsSchemaNpcLevelSet::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcLevelSet::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcLevelSet::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcLevelSetRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcLevelSetRow() : Super()
	{
	}
	
	const FGsSchemaNpcLevelSet* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcLevelSet* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcLevelSet>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcLevelSetRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcLevelSetRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcLevelSet final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/Npc");
	}
	
};
