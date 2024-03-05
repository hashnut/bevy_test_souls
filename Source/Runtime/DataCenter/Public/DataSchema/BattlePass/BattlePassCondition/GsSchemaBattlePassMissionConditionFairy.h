/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassMissionConditionFairy.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattlePass/BattlePassCondition/BSGsBattlePassMissionConditionFairy"))
struct DATACENTER_API FGsSchemaBattlePassMissionConditionFairy
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyId"))
	int32 fairyId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyGradeMin"))
	FairyGrade fairyGradeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyGradeMax"))
	FairyGrade fairyGradeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyComposeResult"))
	FairyComposeResult fairyComposeResult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyLevel"))
	int32 fairyLevel;
public:
	FGsSchemaBattlePassMissionConditionFairy();
	
protected:
	bool operator== (const FGsSchemaBattlePassMissionConditionFairy& __Other) const
	{
		return FGsSchemaBattlePassMissionConditionFairy::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattlePassMissionConditionFairy::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattlePassMissionConditionFairy::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattlePassMissionConditionFairyRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattlePassMissionConditionFairyRow() : Super()
	{
	}
	
	const FGsSchemaBattlePassMissionConditionFairy* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattlePassMissionConditionFairy* Row = nullptr;
		return Table->FindRow<FGsSchemaBattlePassMissionConditionFairy>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattlePassMissionConditionFairyRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattlePassMissionConditionFairyRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattlePassMissionConditionFairy final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BattlePass/BattlePassCondition");
	}
	
};
