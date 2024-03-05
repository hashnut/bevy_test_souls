/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSealNodeLevel.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="봉인노드 레벨업 정보", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Seal/BSGsSealNodeLevel"))
struct DATACENTER_API FGsSchemaSealNodeLevel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="봉인노드 레벨", DisplayName="sealNodeLevelId"))
	int32 sealNodeLevelId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="필요 경험치", DisplayName="sealNodeLevel"))
	int32 sealNodeLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="랜덤 획득 경험치 최소", DisplayName="needExp"))
	int32 needExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="랜덤 획득 경험치 최대", DisplayName="gainExpRandomMin"))
	int32 gainExpRandomMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="획득 스탯 타입", DisplayName="gainExpRandomMax"))
	int32 gainExpRandomMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="레벨획득스탯ID", DisplayName="levelGainStatId"))
	int32 levelGainStatId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="비용: 아이템ID", DisplayName="costItemId"))
	TArray<int32> costItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="비용: 아이템갯수", DisplayName="costItemAmount"))
	TArray<int32> costItemAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="비용: 재화 타입", DisplayName="costCurrencyType"))
	CurrencyType costCurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="비용: 재화 갯수", DisplayName="costCurrencyAmount"))
	int32 costCurrencyAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="리롤 비용 ID", DisplayName="rerollCostId"))
	int32 rerollCostId;
public:
	FGsSchemaSealNodeLevel();
	
protected:
	bool operator== (const FGsSchemaSealNodeLevel& __Other) const
	{
		return FGsSchemaSealNodeLevel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSealNodeLevel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSealNodeLevel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSealNodeLevelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSealNodeLevelRow() : Super()
	{
	}
	
	const FGsSchemaSealNodeLevel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSealNodeLevel* Row = nullptr;
		return Table->FindRow<FGsSchemaSealNodeLevel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSealNodeLevelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSealNodeLevelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSealNodeLevel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Seal");
	}
	
};
