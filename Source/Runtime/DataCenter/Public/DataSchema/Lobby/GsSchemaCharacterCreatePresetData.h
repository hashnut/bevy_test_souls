/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Customize/GsSchemaCustomizePreset.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCharacterCreatePresetData.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="캐릭터 생성 프리셋 정보", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Lobby/BSGsCharacterCreatePresetData"))
struct DATACENTER_API FGsSchemaCharacterCreatePresetData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="genderType"))
	CreatureGenderType genderType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="characterPath"))
	FSoftObjectPath characterPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="customizePreset"))
	FGsSchemaCustomizePresetRow customizePreset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bodyPartsShapeId"))
	int32 bodyPartsShapeId;
public:
	FGsSchemaCharacterCreatePresetData();
	
protected:
	bool operator== (const FGsSchemaCharacterCreatePresetData& __Other) const
	{
		return FGsSchemaCharacterCreatePresetData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCharacterCreatePresetData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCharacterCreatePresetData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCharacterCreatePresetDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCharacterCreatePresetDataRow() : Super()
	{
	}
	
	const FGsSchemaCharacterCreatePresetData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCharacterCreatePresetData* Row = nullptr;
		return Table->FindRow<FGsSchemaCharacterCreatePresetData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCharacterCreatePresetDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCharacterCreatePresetDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCharacterCreatePresetData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Lobby");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCharacterCreatePresetData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCharacterCreatePresetData>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCharacterCreatePresetData*& OutRow) const
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
		const FGsSchemaCharacterCreatePresetData* Temp;
		return !FindRowById(FGsSchemaCharacterCreatePresetData().id, Temp);
	}
#endif
};
