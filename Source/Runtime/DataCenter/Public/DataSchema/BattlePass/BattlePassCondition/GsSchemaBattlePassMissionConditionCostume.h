/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassMissionConditionCostume.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattlePass/BattlePassCondition/BSGsBattlePassMissionConditionCostume"))
struct DATACENTER_API FGsSchemaBattlePassMissionConditionCostume
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeId"))
	int32 costumeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeGradeMin"))
	CostumeGrade costumeGradeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeGradeMax"))
	CostumeGrade costumeGradeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="composeResult"))
	CostumeComposeResult composeResult;
public:
	FGsSchemaBattlePassMissionConditionCostume();
	
protected:
	bool operator== (const FGsSchemaBattlePassMissionConditionCostume& __Other) const
	{
		return FGsSchemaBattlePassMissionConditionCostume::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattlePassMissionConditionCostume::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattlePassMissionConditionCostume::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattlePassMissionConditionCostumeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattlePassMissionConditionCostumeRow() : Super()
	{
	}
	
	const FGsSchemaBattlePassMissionConditionCostume* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattlePassMissionConditionCostume* Row = nullptr;
		return Table->FindRow<FGsSchemaBattlePassMissionConditionCostume>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattlePassMissionConditionCostumeRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattlePassMissionConditionCostumeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattlePassMissionConditionCostume final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BattlePass/BattlePassCondition");
	}
	
};
