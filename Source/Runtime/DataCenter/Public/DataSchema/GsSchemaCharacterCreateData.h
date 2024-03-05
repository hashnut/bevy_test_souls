/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "GsSchemaCharacterCreateData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BSGsCharacterCreateData"))
struct DATACENTER_API FGsSchemaCharacterCreateData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="genderType", Tooltip))
	CreatureGenderType genderType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="desc", Tooltip))
	FString desc;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="characterPath", Tooltip))
	FSoftObjectPath characterPath;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tatooId", Tooltip))
	int32 tatooId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="HairPartsId", Tooltip))
	int32 HairPartsId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="facePartsId", Tooltip))
	int32 facePartsId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="bodyPartsId", Tooltip))
	int32 bodyPartsId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="cloakPartsId", Tooltip))
	int32 cloakPartsId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weaponPartsId", Tooltip))
	int32 weaponPartsId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="hairColorId", Tooltip))
	int32 hairColorId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skinColorId", Tooltip))
	int32 skinColorId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pupilColorId", Tooltip))
	int32 pupilColorId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="browColorId", Tooltip))
	int32 browColorId;
	
public:
	FGsSchemaCharacterCreateData();
	
protected:
	bool operator== (const FGsSchemaCharacterCreateData& __Other) const
	{
		return FGsSchemaCharacterCreateData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCharacterCreateData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCharacterCreateData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCharacterCreateDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCharacterCreateDataRow() : Super()
	{
	}
	
	const FGsSchemaCharacterCreateData* GetRow() const
	{
		const FGsSchemaCharacterCreateData* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaCharacterCreateData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCharacterCreateDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCharacterCreateDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCharacterCreateData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<CreatureGenderType, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCharacterCreateData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCharacterCreateData>(RowsMap))
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
	bool FindRowById(const CreatureGenderType& InId, OUT const FGsSchemaCharacterCreateData*& OutRow) const
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
		const FGsSchemaCharacterCreateData* Temp;
		return !FindRowById(FGsSchemaCharacterCreateData().genderType, Temp);
	}
#endif
};
