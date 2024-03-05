/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Attendance/Reward/GsSchemaAttendanceReward.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommunityEventEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEventAttendance.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Attendance/BSGsCommunityEventAttendance"))
struct DATACENTER_API FGsSchemaCommunityEventAttendance
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="attendanceType"))
	AttendanceType attendanceType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="typeInfo"))
	FGsSchemaRowPoly typeInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="reuse"))
	bool reuse;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardList"))
	TArray<FGsSchemaAttendanceReward> rewardList;
public:
	FGsSchemaCommunityEventAttendance();
	
protected:
	bool operator== (const FGsSchemaCommunityEventAttendance& __Other) const
	{
		return FGsSchemaCommunityEventAttendance::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEventAttendance::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEventAttendance::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventAttendanceRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventAttendanceRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEventAttendance* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEventAttendance* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEventAttendance>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventAttendanceRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventAttendanceRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEventAttendance final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Attendance");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCommunityEventAttendance*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCommunityEventAttendance>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaCommunityEventAttendance*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaCommunityEventAttendance* Temp;
		return !FindRowById(FGsSchemaCommunityEventAttendance().id, Temp);
	}
#endif
};
