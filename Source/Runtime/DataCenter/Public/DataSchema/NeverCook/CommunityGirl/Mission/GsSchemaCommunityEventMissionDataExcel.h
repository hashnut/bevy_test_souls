/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommunityEventEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEventMissionDataExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/CommunityGirl/Mission/BSGsCommunityEventMissionDataExcel"))
struct DATACENTER_API FGsSchemaCommunityEventMissionDataExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int64 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardId"))
	int32 rewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="missionType"))
	CommunityEventMissionType missionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="missionConditionTableName"))
	FString missionConditionTableName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="missionConditionRowName"))
	FName missionConditionRowName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventMissionCount"))
	int32 eventMissionCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventMissionText"))
	FText eventMissionText;
public:
	FGsSchemaCommunityEventMissionDataExcel();
	
protected:
	bool operator== (const FGsSchemaCommunityEventMissionDataExcel& __Other) const
	{
		return FGsSchemaCommunityEventMissionDataExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEventMissionDataExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEventMissionDataExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventMissionDataExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventMissionDataExcelRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEventMissionDataExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEventMissionDataExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEventMissionDataExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventMissionDataExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventMissionDataExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEventMissionDataExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/CommunityGirl/Mission");
	}
	
};
