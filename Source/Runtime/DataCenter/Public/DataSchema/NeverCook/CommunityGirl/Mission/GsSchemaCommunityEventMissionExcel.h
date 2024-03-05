/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardBoxData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEventMissionExcel.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NeverCook/CommunityGirl/Mission/BSGsCommunityEventMissionExcel"))
struct DATACENTER_API FGsSchemaCommunityEventMissionExcel
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isBonus"))
	bool isBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bonusRewardId"))
	FGsSchemaRewardBoxDataRow bonusRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventMissionTabList"))
	TArray<int32> eventMissionTabList;
public:
	FGsSchemaCommunityEventMissionExcel();
	
protected:
	bool operator== (const FGsSchemaCommunityEventMissionExcel& __Other) const
	{
		return FGsSchemaCommunityEventMissionExcel::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEventMissionExcel::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEventMissionExcel::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventMissionExcelRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventMissionExcelRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEventMissionExcel* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEventMissionExcel* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEventMissionExcel>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventMissionExcelRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventMissionExcelRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEventMissionExcel final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NeverCook/CommunityGirl/Mission");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCommunityEventMissionExcel*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCommunityEventMissionExcel>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCommunityEventMissionExcel*& OutRow) const
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
		const FGsSchemaCommunityEventMissionExcel* Temp;
		return !FindRowById(FGsSchemaCommunityEventMissionExcel().id, Temp);
	}
#endif
};
