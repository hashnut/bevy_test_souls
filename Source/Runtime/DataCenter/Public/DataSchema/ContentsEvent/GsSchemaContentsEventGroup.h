/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/ContentsEvent/GsSchemaContentsEventListItem.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedEventEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaContentsEventGroup.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="컨텐츠이벤트 그룹", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/ContentsEvent/BSGSContentsEventGroup"))
struct DATACENTER_API FGsSchemaContentsEventGroup
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="devDesc"))
	FString devDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="startTime"))
	FDateTime startTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="endTime"))
	FDateTime endTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="낮은 수가 먼저 출력됨. 오름 차순", DisplayName="displayPriority"))
	int32 displayPriority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="icon"))
	FSoftObjectPath icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="title"))
	FText title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FText desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="visible"))
	bool visible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionType"))
	ContentsEventCondition conditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionValue"))
	int32 conditionValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventList"))
	TArray<FGsSchemaContentsEventListItem> eventList;
public:
	FGsSchemaContentsEventGroup();
	
protected:
	bool operator== (const FGsSchemaContentsEventGroup& __Other) const
	{
		return FGsSchemaContentsEventGroup::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaContentsEventGroup::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaContentsEventGroup::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaContentsEventGroupRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaContentsEventGroupRow() : Super()
	{
	}
	
	const FGsSchemaContentsEventGroup* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaContentsEventGroup* Row = nullptr;
		return Table->FindRow<FGsSchemaContentsEventGroup>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaContentsEventGroupRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaContentsEventGroupRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableContentsEventGroup final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("ContentsEvent");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaContentsEventGroup*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaContentsEventGroup>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaContentsEventGroup*& OutRow) const
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
		const FGsSchemaContentsEventGroup* Temp;
		return !FindRowById(FGsSchemaContentsEventGroup().id, Temp);
	}
#endif
};
