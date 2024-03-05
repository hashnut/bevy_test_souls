/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedBattlePassEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaBattlePassCondition.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BattlePass/BSGsBattlePassCondition"))
struct DATACENTER_API FGsSchemaBattlePassCondition
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passMissionType"))
	BattlePassMissionType passMissionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passMissonConditionData"))
	FGsSchemaRowPoly passMissonConditionData;
public:
	FGsSchemaBattlePassCondition();
	
protected:
	bool operator== (const FGsSchemaBattlePassCondition& __Other) const
	{
		return FGsSchemaBattlePassCondition::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaBattlePassCondition::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaBattlePassCondition::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaBattlePassConditionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaBattlePassConditionRow() : Super()
	{
	}
	
	const FGsSchemaBattlePassCondition* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaBattlePassCondition* Row = nullptr;
		return Table->FindRow<FGsSchemaBattlePassCondition>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaBattlePassConditionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaBattlePassConditionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableBattlePassCondition final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("BattlePass");
	}
	
};
