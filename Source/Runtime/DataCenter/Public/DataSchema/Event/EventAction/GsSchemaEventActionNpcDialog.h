/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/NpcDialog/GsSchemaNpcDialogData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEventActionNpcDialog.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Event/EventAction/BSGsEventActionNpcDialog"))
struct DATACENTER_API FGsSchemaEventActionNpcDialog
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcDataId"))
	FGsSchemaNpcDataRow npcDataId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcDialogData"))
	FGsSchemaNpcDialogDataRow npcDialogData;
public:
	FGsSchemaEventActionNpcDialog();
	
protected:
	bool operator== (const FGsSchemaEventActionNpcDialog& __Other) const
	{
		return FGsSchemaEventActionNpcDialog::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEventActionNpcDialog::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEventActionNpcDialog::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEventActionNpcDialogRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEventActionNpcDialogRow() : Super()
	{
	}
	
	const FGsSchemaEventActionNpcDialog* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaEventActionNpcDialog* Row = nullptr;
		return Table->FindRow<FGsSchemaEventActionNpcDialog>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEventActionNpcDialogRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEventActionNpcDialogRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEventActionNpcDialog final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Event/EventAction");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaEventActionNpcDialog*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaEventActionNpcDialog>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaEventActionNpcDialog*& OutRow) const
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
		const FGsSchemaEventActionNpcDialog* Temp;
		return !FindRowById(FGsSchemaEventActionNpcDialog().id, Temp);
	}
#endif
};
