/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "GsSchemaAttendanceItem.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Attendance/Reward/BSGsAttendanceItem"))
struct DATACENTER_API FGsSchemaAttendanceItem
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FGsSchemaItemCommonRow itemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCount", Tooltip))
	int32 itemCount;
public:
	FGsSchemaAttendanceItem();
	
protected:
	bool operator== (const FGsSchemaAttendanceItem& __Other) const
	{
		return FGsSchemaAttendanceItem::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAttendanceItem::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAttendanceItem::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAttendanceItemRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAttendanceItemRow() : Super()
	{
	}
	
	const FGsSchemaAttendanceItem* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAttendanceItem* Row = nullptr;
		return Table->FindRow<FGsSchemaAttendanceItem>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAttendanceItemRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAttendanceItemRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAttendanceItem final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Attendance/Reward");
	}
	
};
