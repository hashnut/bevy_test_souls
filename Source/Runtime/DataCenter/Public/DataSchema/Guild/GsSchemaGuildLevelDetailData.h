/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGuildLevelDetailData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildLevelDetailData"))
struct DATACENTER_API FGsSchemaGuildLevelDetailData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="기획참고용", DisplayName="note"))
	FString note;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxSubCaptain"))
	int32 maxSubCaptain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxFirstKnight"))
	int32 maxFirstKnight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxSecondKnight"))
	int32 maxSecondKnight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="수호탑레벨", DisplayName="campLevel"))
	int32 campLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maxZealPointLevel"))
	int32 maxZealPointLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="다음 레벨까지 필요 공헌도", DisplayName="contributionNextLevel"))
	int64 contributionNextLevel;
public:
	FGsSchemaGuildLevelDetailData();
	
protected:
	bool operator== (const FGsSchemaGuildLevelDetailData& __Other) const
	{
		return FGsSchemaGuildLevelDetailData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGuildLevelDetailData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaGuildLevelDetailData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGuildLevelDetailDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGuildLevelDetailDataRow() : Super()
	{
	}
	
	const FGsSchemaGuildLevelDetailData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaGuildLevelDetailData* Row = nullptr;
		return Table->FindRow<FGsSchemaGuildLevelDetailData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGuildLevelDetailDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaGuildLevelDetailDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGuildLevelDetailData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Guild");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaGuildLevelDetailData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaGuildLevelDetailData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->level, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaGuildLevelDetailData*& OutRow) const
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
		const FGsSchemaGuildLevelDetailData* Temp;
		return !FindRowById(FGsSchemaGuildLevelDetailData().level, Temp);
	}
#endif
};
