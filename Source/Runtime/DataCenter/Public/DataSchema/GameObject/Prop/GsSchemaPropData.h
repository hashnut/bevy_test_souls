/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardBoxData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaShapeData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedInteractionEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedPropEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPropData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Prop/BSGsPropData"))
struct DATACENTER_API FGsSchemaPropData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="propType"))
	PropType propType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="shapeId"))
	FGsSchemaShapeDataRow shapeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="nameString"))
	FText nameString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="interactionType"))
	PropInteractionType interactionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="1sceond = 1000ms", DisplayName="touchInteractionTime"))
	float touchInteractionTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="touchInteractionRange"))
	float touchInteractionRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="multipleInteractionUserCount"))
	int32 multipleInteractionUserCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="https://jira.com2us.com/jira/browse/CHR-21365", DisplayName="spawnMaintainTimeS"))
	int32 spawnMaintainTimeS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hitCountMax"))
	int32 hitCountMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="rewardId"))
	FGsSchemaRewardBoxDataRow rewardId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="multipleInteractionCountMax"))
	int32 multipleInteractionCountMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="despawnInterval"))
	float despawnInterval;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="paramValue"))
	int32 paramValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="paramValue2"))
	int32 paramValue2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="actionFinishPlayEffectObject"))
	FSoftObjectPath actionFinishPlayEffectObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="playEffectObject"))
	FSoftObjectPath playEffectObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="playEffectOffset"))
	FVector playEffectOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="playEffectScale"))
	float playEffectScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="abnormalityId"))
	FGsSchemaAbnormalitySetRow abnormalityId;
public:
	FGsSchemaPropData();
	
protected:
	bool operator== (const FGsSchemaPropData& __Other) const
	{
		return FGsSchemaPropData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPropData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPropData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPropDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPropDataRow() : Super()
	{
	}
	
	const FGsSchemaPropData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPropData* Row = nullptr;
		return Table->FindRow<FGsSchemaPropData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPropDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPropDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePropData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/Prop");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaPropData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaPropData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaPropData*& OutRow) const
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
		const FGsSchemaPropData* Temp;
		return !FindRowById(FGsSchemaPropData().id, Temp);
	}
#endif
};
