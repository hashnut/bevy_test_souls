/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGuildAcquisition.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Guild/BSGsGuildAcquisition"))
struct DATACENTER_API FGsSchemaGuildAcquisition
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCategorySubList"))
	TArray<ItemCategorySub> itemCategorySubList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemCategoryMain"))
	ItemCategoryMain itemCategoryMain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gradeAll"))
	bool gradeAll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gradeMin"))
	ItemGrade gradeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gradeMax"))
	ItemGrade gradeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gradeDefault"))
	ItemGrade gradeDefault;
public:
	FGsSchemaGuildAcquisition();
	
protected:
	bool operator== (const FGsSchemaGuildAcquisition& __Other) const
	{
		return FGsSchemaGuildAcquisition::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGuildAcquisition::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaGuildAcquisition::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGuildAcquisitionRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGuildAcquisitionRow() : Super()
	{
	}
	
	const FGsSchemaGuildAcquisition* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaGuildAcquisition* Row = nullptr;
		return Table->FindRow<FGsSchemaGuildAcquisition>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGuildAcquisitionRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaGuildAcquisitionRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGuildAcquisition final : public UGsTable
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
		TMap<FName, const FGsSchemaGuildAcquisition*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaGuildAcquisition>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaGuildAcquisition*& OutRow) const
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
		const FGsSchemaGuildAcquisition* Temp;
		return !FindRowById(FGsSchemaGuildAcquisition().id, Temp);
	}
#endif
};
