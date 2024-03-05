/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Title/GsSchemaTitleSubCategory.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedTitleEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowPoly.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTitle.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Title/BSGsTitle"))
struct DATACENTER_API FGsSchemaTitle
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="subCategoryRowId"))
	FGsSchemaTitleSubCategoryRow subCategoryRowId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	TitleGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="equipSlotType"))
	TitleEquipSlot equipSlotType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passivitySetId"))
	FGsSchemaPassivitySetRow passivitySetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FText desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="passivityDesc"))
	FText passivityDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="isProgressDisplayFlag"))
	bool isProgressDisplayFlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="retainType"))
	TitleRetainType retainType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="retainCheckType"))
	TitleConditionCheckType retainCheckType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionType"))
	TitleConditionType conditionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionDesc"))
	FString conditionDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="conditionalCount"))
	int32 conditionalCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="titleConditionData"))
	FGsSchemaRowPoly titleConditionData;
public:
	FGsSchemaTitle();
	
protected:
	bool operator== (const FGsSchemaTitle& __Other) const
	{
		return FGsSchemaTitle::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTitle::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTitle::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTitleRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTitleRow() : Super()
	{
	}
	
	const FGsSchemaTitle* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTitle* Row = nullptr;
		return Table->FindRow<FGsSchemaTitle>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTitleRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTitleRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTitle final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Title");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaTitle*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaTitle>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaTitle*& OutRow) const
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
		const FGsSchemaTitle* Temp;
		return !FindRowById(FGsSchemaTitle().id, Temp);
	}
#endif
};
