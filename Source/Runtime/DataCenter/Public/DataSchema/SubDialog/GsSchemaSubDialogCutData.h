/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "DataSchema/SubDialog/GsSchemaSubDialogActorData.h"
#include "GsSchemaSubDialogCutData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/SubDialog/BSGsSubDialogCutData"))
struct DATACENTER_API FGsSchemaSubDialogCutData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Id", Tooltip))
	int32 Id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="remainTime", Tooltip))
	float remainTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="actor", Tooltip))
	FGsSchemaSubDialogActorDataRow actor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="facetype", Tooltip))
	EGsSubDialogActorFaceType facetype;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name", Tooltip))
	FText name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MultiLine="true", DisplayName="script", Tooltip))
	FText script;
	
public:
	FGsSchemaSubDialogCutData();
	
protected:
	bool operator== (const FGsSchemaSubDialogCutData& __Other) const
	{
		return FGsSchemaSubDialogCutData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSubDialogCutData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSubDialogCutData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSubDialogCutDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSubDialogCutDataRow() : Super()
	{
	}
	
	const FGsSchemaSubDialogCutData* GetRow() const
	{
		const FGsSchemaSubDialogCutData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaSubDialogCutData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSubDialogCutDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSubDialogCutDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSubDialogCutData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("SubDialog");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaSubDialogCutData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaSubDialogCutData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaSubDialogCutData*& OutRow) const
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
		const FGsSchemaSubDialogCutData* Temp;
		return !FindRowById(FGsSchemaSubDialogCutData().Id, Temp);
	}
#endif
};
