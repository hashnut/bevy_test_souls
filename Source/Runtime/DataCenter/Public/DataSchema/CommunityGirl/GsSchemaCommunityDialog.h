/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/GsSchemaEnums.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityDialog.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/CommunityGirl/BSGsCommunityDialog"))
struct DATACENTER_API FGsSchemaCommunityDialog
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id", Tooltip))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Condition", Tooltip))
	EGsGirlAnimState Condition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Dialog", Tooltip))
	FText Dialog;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="timeLimit", Tooltip))
	float timeLimit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Probability", Tooltip))
	float Probability;
public:
	FGsSchemaCommunityDialog();
	
protected:
	bool operator== (const FGsSchemaCommunityDialog& __Other) const
	{
		return FGsSchemaCommunityDialog::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityDialog::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityDialog::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityDialogRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityDialogRow() : Super()
	{
	}
	
	const FGsSchemaCommunityDialog* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityDialog* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityDialog>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityDialogRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityDialogRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityDialog final : public UGsTable
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
		TMap<FName, const FGsSchemaCommunityDialog*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCommunityDialog>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCommunityDialog*& OutRow) const
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
		const FGsSchemaCommunityDialog* Temp;
		return !FindRowById(FGsSchemaCommunityDialog().id, Temp);
	}
#endif
};
