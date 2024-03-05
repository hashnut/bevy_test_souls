/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEventMissionTabExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/CommunityGirl/Mission/BSGsCommunityEventMissionTabExcel"))
struct DATACENTER_API FGsSchemaCommunityEventMissionTabExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabId"))
	int32 tabId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabNameText"))
	FText tabNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventMissionList"))
	TArray<int32> eventMissionList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabOpenTime"))
	int32 tabOpenTime;
public:
	FGsSchemaCommunityEventMissionTabExcel();
	
protected:
	bool operator== (const FGsSchemaCommunityEventMissionTabExcel& __Other) const
	{
		return FGsSchemaCommunityEventMissionTabExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEventMissionTabExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEventMissionTabExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventMissionTabExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventMissionTabExcelRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEventMissionTabExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEventMissionTabExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEventMissionTabExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventMissionTabExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventMissionTabExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEventMissionTabExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/CommunityGirl/Mission");
	}
	
};
