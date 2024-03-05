/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/CommunityGirl/Mission/GsSchemaCommunityEventMissionCommon.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEventMissionTab.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/CommunityGirl/Mission/BSGsCommunityEventMissionTab"))
struct DATACENTER_API FGsSchemaCommunityEventMissionTab
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tabOpenTime"))
	int32 tabOpenTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="eventMissionList"))
	TArray<FGsSchemaCommunityEventMissionCommonRow> eventMissionList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tabNameText"))
	FText tabNameText;
public:
	FGsSchemaCommunityEventMissionTab();
	
protected:
	bool operator== (const FGsSchemaCommunityEventMissionTab& __Other) const
	{
		return FGsSchemaCommunityEventMissionTab::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEventMissionTab::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEventMissionTab::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventMissionTabRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventMissionTabRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEventMissionTab* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEventMissionTab* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEventMissionTab>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventMissionTabRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventMissionTabRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEventMissionTab final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("CommunityGirl/Mission");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCommunityEventMissionTab*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCommunityEventMissionTab>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCommunityEventMissionTab*& OutRow) const
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
		const FGsSchemaCommunityEventMissionTab* Temp;
		return !FindRowById(FGsSchemaCommunityEventMissionTab().id, Temp);
	}
#endif
};
