/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Shared/Client/SharedEnums/SharedIffEnum.h"
#include "GsSchemaPVPAlignmentData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/PVP/BSGsPVPAlignmentData"))
struct DATACENTER_API FGsSchemaPVPAlignmentData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Id", Tooltip))
	int32 Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name", Tooltip))
	FString name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pvpAlignmentPointMax", Tooltip))
	int32 pvpAlignmentPointMax;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGsSchemaPassivitySetRow> passivityIdList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="threatDuration", Tooltip))
	int32 threatDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="userKillPoint", Tooltip))
	int32 userKillPoint;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade", Tooltip))
	PVPAlignmentGrade grade;
	
public:
	FGsSchemaPVPAlignmentData();
	
protected:
	bool operator== (const FGsSchemaPVPAlignmentData& __Other) const
	{
		return FGsSchemaPVPAlignmentData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPVPAlignmentData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPVPAlignmentData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPVPAlignmentDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPVPAlignmentDataRow() : Super()
	{
	}
	
	const FGsSchemaPVPAlignmentData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPVPAlignmentData* Row = nullptr;
		return Table->FindRow<FGsSchemaPVPAlignmentData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPVPAlignmentDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPVPAlignmentDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePVPAlignmentData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("PVP");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaPVPAlignmentData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPVPAlignmentData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->Id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaPVPAlignmentData*& OutRow) const
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
		const FGsSchemaPVPAlignmentData* Temp;
		return !FindRowById(FGsSchemaPVPAlignmentData().Id, Temp);
	}
#endif
};
