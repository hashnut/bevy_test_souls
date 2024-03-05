/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "GsSchemaAttendanceEvent.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Attendance/BSGsAttendanceEvent"))
struct DATACENTER_API FGsSchemaAttendanceEvent
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costType", Tooltip))
	CostType costType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="typeInfo", Tooltip))
	FGsSchemaRowPoly typeInfo;
public:
	FGsSchemaAttendanceEvent();
	
protected:
	bool operator== (const FGsSchemaAttendanceEvent& __Other) const
	{
		return FGsSchemaAttendanceEvent::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaAttendanceEvent::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaAttendanceEvent::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaAttendanceEventRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaAttendanceEventRow() : Super()
	{
	}
	
	const FGsSchemaAttendanceEvent* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaAttendanceEvent* Row = nullptr;
		return Table->FindRow<FGsSchemaAttendanceEvent>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaAttendanceEventRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaAttendanceEventRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableAttendanceEvent final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Attendance");
	}
	
};
