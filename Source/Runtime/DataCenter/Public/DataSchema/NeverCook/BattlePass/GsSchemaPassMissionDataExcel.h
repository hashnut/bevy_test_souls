/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedBattlePassEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPassMissionDataExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/BattlePass/BSGsPassMissionDataExcel"))
struct DATACENTER_API FGsSchemaPassMissionDataExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passMissionId"))
	int64 passMissionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passMissionGroupId"))
	int64 passMissionGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passMissionTabId"))
	int32 passMissionTabId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="resetType"))
	ResetTimeTicket resetType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabDetailId"))
	int64 tabDetailId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passMissionRewardExp"))
	int64 passMissionRewardExp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passMissionType"))
	BattlePassMissionType passMissionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passConditionTableName"))
	FString passConditionTableName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passConditionRowName"))
	FName passConditionRowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="passMissionCount"))
	int32 passMissionCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="detail"))
	int64 detail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardDetail"))
	int64 rewardDetail;
public:
	FGsSchemaPassMissionDataExcel();
	
protected:
	bool operator== (const FGsSchemaPassMissionDataExcel& __Other) const
	{
		return FGsSchemaPassMissionDataExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPassMissionDataExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPassMissionDataExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPassMissionDataExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPassMissionDataExcelRow() : Super()
	{
	}
	
	const FGsSchemaPassMissionDataExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPassMissionDataExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaPassMissionDataExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPassMissionDataExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPassMissionDataExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePassMissionDataExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/BattlePass");
	}
	
};
