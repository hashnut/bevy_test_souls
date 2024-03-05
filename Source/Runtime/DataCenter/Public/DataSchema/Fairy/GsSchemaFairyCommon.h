/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyDetail.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyLevel.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaFairyCommon.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Fairy/BSGsFairyCommon"))
struct DATACENTER_API FGsSchemaFairyCommon
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="grade", Tooltip="일반,고급,희귀,영웅,전설"))
	FairyGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tier"))
	int32 tier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="levelId", Tooltip="FairyLevel Table 참조"))
	FGsSchemaFairyLevelRow levelId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="detailID"))
	FGsSchemaFairyDetailRow detailID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="summonHighGradeComment"))
	FText summonHighGradeComment;
public:
	FGsSchemaFairyCommon();
	
protected:
	bool operator== (const FGsSchemaFairyCommon& __Other) const
	{
		return FGsSchemaFairyCommon::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaFairyCommon::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaFairyCommon::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaFairyCommonRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaFairyCommonRow() : Super()
	{
	}
	
	const FGsSchemaFairyCommon* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaFairyCommon* Row = nullptr;
		return Table->FindRow<FGsSchemaFairyCommon>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaFairyCommonRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaFairyCommonRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableFairyCommon final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Fairy");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaFairyCommon*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaFairyCommon>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaFairyCommon*& OutRow) const
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
		const FGsSchemaFairyCommon* Temp;
		return !FindRowById(FGsSchemaFairyCommon().id, Temp);
	}
#endif
};
