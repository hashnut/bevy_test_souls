/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/GameObject/Define/EGsDisPlayFormatType.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaStatFormatingData.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="Stat formating 및 ui order 순서 테이블", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Stat/BSGsStatFormatingData"))
struct DATACENTER_API FGsSchemaStatFormatingData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(ShowAllItems, DisplayName="statType"))
	FGsStatTypeNameSelector statType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statName"))
	FText statName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="markFormat"))
	FText markFormat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="숫자가 높을수록 제일 먼저 출력", DisplayName="uiOrderNum"))
	int32 uiOrderNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statIconImagePath"))
	FSoftObjectPath statIconImagePath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="displayFormatType"))
	EGsDisPlayFormatType displayFormatType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sector"))
	int32 sector;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="correspondingWeapon"))
	TArray<CreatureWeaponType> correspondingWeapon;
public:
	FGsSchemaStatFormatingData();
	
protected:
	bool operator== (const FGsSchemaStatFormatingData& __Other) const
	{
		return FGsSchemaStatFormatingData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaStatFormatingData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaStatFormatingData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaStatFormatingDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaStatFormatingDataRow() : Super()
	{
	}
	
	const FGsSchemaStatFormatingData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaStatFormatingData* Row = nullptr;
		return Table->FindRow<FGsSchemaStatFormatingData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaStatFormatingDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaStatFormatingDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableStatFormatingData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<FGsStatTypeNameSelector, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Stat");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaStatFormatingData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaStatFormatingData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->statType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const FGsStatTypeNameSelector& InId, OUT const FGsSchemaStatFormatingData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const FGsStatTypeNameSelector& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaStatFormatingData* Temp;
		return !FindRowById(FGsSchemaStatFormatingData().statType, Temp);
	}
#endif
};
