/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/Customize/GsSchemaCustomizeBoneScaleGroup.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsSchemaCustomizeBoneScaleSetting.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="Default Scale Vector 값은 1.0f, 1.0f, 1.0f 이다.", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Customize/BSGsCustomizeBoneScaleSetting"))
struct DATACENTER_API FGsSchemaCustomizeBoneScaleSetting
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="genderType", Tooltip))
	CreatureGenderType genderType;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGsSchemaCustomizeBoneScaleGroup> Height;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGsSchemaCustomizeBoneScaleGroup> Head;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGsSchemaCustomizeBoneScaleGroup> Chest;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGsSchemaCustomizeBoneScaleGroup> Waist;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGsSchemaCustomizeBoneScaleGroup> Pelvis;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FGsSchemaCustomizeBoneScaleGroup> LegThickness;
	
public:
	FGsSchemaCustomizeBoneScaleSetting();
	
protected:
	bool operator== (const FGsSchemaCustomizeBoneScaleSetting& __Other) const
	{
		return FGsSchemaCustomizeBoneScaleSetting::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCustomizeBoneScaleSetting::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCustomizeBoneScaleSetting::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCustomizeBoneScaleSettingRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCustomizeBoneScaleSettingRow() : Super()
	{
	}
	
	const FGsSchemaCustomizeBoneScaleSetting* GetRow() const
	{
		const FGsSchemaCustomizeBoneScaleSetting* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaCustomizeBoneScaleSetting>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCustomizeBoneScaleSettingRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCustomizeBoneScaleSettingRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCustomizeBoneScaleSetting final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CreatureGenderType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Customize");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCustomizeBoneScaleSetting*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCustomizeBoneScaleSetting>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->genderType, Row.Key);
		}
	}
	
public:
	bool FindRowById(const CreatureGenderType& InId, OUT const FGsSchemaCustomizeBoneScaleSetting*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const CreatureGenderType& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaCustomizeBoneScaleSetting* Temp;
		return !FindRowById(FGsSchemaCustomizeBoneScaleSetting().genderType, Temp);
	}
#endif
};
