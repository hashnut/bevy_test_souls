/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaAttendanceReward.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Attendance/Reward/BSGsAttendanceReward"))
struct DATACENTER_API FGsSchemaAttendanceReward
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="day"))
	int32 day;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="itemId"))
	FGsSchemaItemCommonRow itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCount"))
	int32 itemCount;
public:
	FGsSchemaAttendanceReward();
	
protected:
	bool operator== (const FGsSchemaAttendanceReward& __Other) const
	{
		return FGsSchemaAttendanceReward::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAttendanceReward::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAttendanceReward::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAttendanceRewardRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAttendanceRewardRow() : Super()
	{
	}
	
	const FGsSchemaAttendanceReward* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAttendanceReward* Row = nullptr;
		return Table->FindRow<FGsSchemaAttendanceReward>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAttendanceRewardRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAttendanceRewardRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAttendanceReward final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Attendance/Reward");
	}
	
};
