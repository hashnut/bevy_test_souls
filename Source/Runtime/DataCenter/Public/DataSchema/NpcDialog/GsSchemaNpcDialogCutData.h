/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Quest/Dialog/GsDialogType.h"
#include "Runtime/DataCenter/Public/DataSchema/Camera/GsSchemaCameraDialogViewData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Runtime/DataCenter/Public/DataSchema/NpcDialog/GsSchemaNpcDialogActorData.h"
#include "Runtime/DataCenter/Public/DataSchema/NpcDialog/GsSchemaNpcDialogImgData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcDialogCutData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NpcDialog/BSGsNpcDialogCutData"))
struct DATACENTER_API FGsSchemaNpcDialogCutData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Id"))
	int32 Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dialogType"))
	EGsNpcDialogType dialogType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isMy"))
	bool isMy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="textColorType"))
	EGsDialogTextColorType textColorType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="actorName"))
	FText actorName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MultiLine="true", DisplayName="scriptText"))
	FText scriptText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="voiceId"))
	int32 voiceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="playerPosSpotId"))
	int64 playerPosSpotId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="dialogViewType"))
	FGsSchemaCameraDialogViewDataRow dialogViewType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="actortList"))
	TArray<FGsSchemaNpcDialogActorDataRow> actortList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cameraViewList"))
	TArray<FGsSchemaNpcDataRow> cameraViewList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="imgInfo"))
	FGsSchemaNpcDialogImgDataRow imgInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="playEffectObject"))
	FSoftObjectPath playEffectObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="playEffectOffset"))
	FVector playEffectOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="playEffectScale"))
	float playEffectScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="playEffectDelayTime"))
	float playEffectDelayTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isHideAllUI"))
	bool isHideAllUI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isFadeInEffect"))
	bool isFadeInEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="narrationLoopBP"))
	FSoftObjectPath narrationLoopBP;
public:
	FGsSchemaNpcDialogCutData();
	
protected:
	bool operator== (const FGsSchemaNpcDialogCutData& __Other) const
	{
		return FGsSchemaNpcDialogCutData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcDialogCutData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcDialogCutData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcDialogCutDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcDialogCutDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcDialogCutData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcDialogCutData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcDialogCutData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcDialogCutDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcDialogCutDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcDialogCutData final : public UGsTable
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
		TMap<FName, const FGsSchemaNpcDialogCutData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaNpcDialogCutData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaNpcDialogCutData*& OutRow) const
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
		const FGsSchemaNpcDialogCutData* Temp;
		return !FindRowById(FGsSchemaNpcDialogCutData().Id, Temp);
	}
#endif
};
