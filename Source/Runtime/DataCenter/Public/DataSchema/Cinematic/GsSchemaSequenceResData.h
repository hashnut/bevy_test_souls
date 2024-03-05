/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Sequence/GsSequenceEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSequenceResData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Cinematic/BSGsSequenceResData"))
struct DATACENTER_API FGsSchemaSequenceResData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sequenceResId"))
	int32 sequenceResId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="resObjectPath"))
	FSoftObjectPath resObjectPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="hideTagList"))
	TArray<FName> hideTagList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isSkip"))
	bool isSkip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="sequenceTextType"))
	EGsSequenceTextType sequenceTextType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="titleText"))
	FText titleText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="descText"))
	FText descText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="durationSec"))
	int32 durationSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="viewRewardId"))
	FGsSchemaRewardDataRow viewRewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dimmedWaitTime"))
	float dimmedWaitTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isCallDimmedOff"))
	bool isCallDimmedOff;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="soundMix"))
	EGsSoundMixType soundMix;
public:
	FGsSchemaSequenceResData();
	
protected:
	bool operator== (const FGsSchemaSequenceResData& __Other) const
	{
		return FGsSchemaSequenceResData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSequenceResData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSequenceResData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSequenceResDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSequenceResDataRow() : Super()
	{
	}
	
	const FGsSchemaSequenceResData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSequenceResData* Row = nullptr;
		return Table->FindRow<FGsSchemaSequenceResData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSequenceResDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSequenceResDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSequenceResData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Cinematic");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSequenceResData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSequenceResData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->sequenceResId, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaSequenceResData*& OutRow) const
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
		const FGsSchemaSequenceResData* Temp;
		return !FindRowById(FGsSchemaSequenceResData().sequenceResId, Temp);
	}
#endif
};
