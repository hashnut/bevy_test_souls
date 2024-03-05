/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Quest/Dialog/GsDialogType.h"
#include "Runtime/DataCenter/Public/DataSchema/Camera/GsSchemaCameraDialogViewData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/NpcDialog/GsSchemaNpcDialogCutGroupData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcDialogData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NpcDialog/BSGsNpcDialogData"))
struct DATACENTER_API FGsSchemaNpcDialogData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Id"))
	int32 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cutGroupId"))
	TArray<FGsSchemaNpcDialogCutGroupDataRow> cutGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dialogMultiNpc"))
	TArray<FGsSchemaNpcDataRow> dialogMultiNpc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="playerPosSpotId"))
	int64 playerPosSpotId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dialogViewType"))
	FGsSchemaCameraDialogViewDataRow dialogViewType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyId"))
	FGsSchemaNpcDataRow fairyId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyPosType"))
	EGsNpcDialogFairyPosType fairyPosType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="IsZoomFirstCameraMove"))
	bool IsZoomFirstCameraMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="closeFadeInTime"))
	float closeFadeInTime;
public:
	FGsSchemaNpcDialogData();
	
protected:
	bool operator== (const FGsSchemaNpcDialogData& __Other) const
	{
		return FGsSchemaNpcDialogData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcDialogData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcDialogData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcDialogDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcDialogDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcDialogData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcDialogData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcDialogData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcDialogDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcDialogDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcDialogData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NpcDialog");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaNpcDialogData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaNpcDialogData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaNpcDialogData*& OutRow) const
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
		const FGsSchemaNpcDialogData* Temp;
		return !FindRowById(FGsSchemaNpcDialogData().Id, Temp);
	}
#endif
};
