/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommunityEventEnum.h"
#include "Runtime/DataCenter/Public/Community/GsCommunityDefine.h"
#include "Runtime/DataCenter/Public/DataSchema/CommunityGirl/GsSchemaCommunityEventDetail.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEvent.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/CommunityGirl/BSGsCommunityEvent"))
struct DATACENTER_API FGsSchemaCommunityEvent
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isTest"))
	bool isTest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tabNameText"))
	FText tabNameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="tabIndex"))
	int32 tabIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventTimeStart"))
	FString eventTimeStart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventTimeEnd"))
	FString eventTimeEnd;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventType"))
	CommunityEventType eventType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="durationHoursReceive"))
	int32 durationHoursReceive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="durationHoursComplete"))
	int32 durationHoursComplete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="communityEventConditionType"))
	CommunityEventConditionType communityEventConditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionTypeInfoId"))
	FGsSchemaRowPoly conditionTypeInfoId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventTimeTextType"))
	ECommunityEventTimeType eventTimeTextType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="eventTimeText"))
	FText eventTimeText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="typeInfoEventId"))
	FGsSchemaRowPoly typeInfoEventId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="bannerImgPath"))
	FSoftObjectPath bannerImgPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="detail"))
	FGsSchemaCommunityEventDetailRow detail;
public:
	FGsSchemaCommunityEvent();
	
protected:
	bool operator== (const FGsSchemaCommunityEvent& __Other) const
	{
		return FGsSchemaCommunityEvent::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEvent::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEvent::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEvent* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEvent* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEvent>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEvent final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("CommunityGirl");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCommunityEvent*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCommunityEvent>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCommunityEvent*& OutRow) const
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
		const FGsSchemaCommunityEvent* Temp;
		return !FindRowById(FGsSchemaCommunityEvent().id, Temp);
	}
#endif
};
