/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeAppearace.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeEffect.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumePassivity.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeSkill.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeDetail.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCostumeCommon.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Costume/BSGsCostumeCommon"))
struct DATACENTER_API FGsSchemaCostumeCommon
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="name"))
	FText name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	CostumeGrade grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="genderType"))
	CreatureGenderType genderType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="statPassivityId"))
	FGsSchemaPassivitySetRow statPassivityId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="skillPassivityList"))
	TArray<FGsSchemaCostumePassivity> skillPassivityList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillActiveInfo"))
	FGsSchemaCostumeSkill skillActiveInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="tier"))
	int32 tier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="detail"))
	FGsSchemaCostumeDetail detail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="summonHighGradeComment"))
	FText summonHighGradeComment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="effectInfo"))
	FGsSchemaCostumeEffect effectInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="useAppearance", Tooltip="외형으로 사용되는가"))
	bool useAppearance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(EditCondition="useAppearance", DisplayName="useWearParts"))
	FGsSchemaCostumeAppearace useWearParts;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="weaponType"))
	TArray<CreatureWeaponType> weaponType;
public:
	FGsSchemaCostumeCommon();
	
protected:
	bool operator== (const FGsSchemaCostumeCommon& __Other) const
	{
		return FGsSchemaCostumeCommon::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCostumeCommon::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCostumeCommon::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCostumeCommonRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCostumeCommonRow() : Super()
	{
	}
	
	const FGsSchemaCostumeCommon* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCostumeCommon* Row = nullptr;
		return Table->FindRow<FGsSchemaCostumeCommon>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCostumeCommonRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCostumeCommonRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCostumeCommon final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Costume");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaCostumeCommon*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaCostumeCommon>(RowsMap))
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
	bool FindRowById(const int32& InId, OUT const FGsSchemaCostumeCommon*& OutRow) const
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
		const FGsSchemaCostumeCommon* Temp;
		return !FindRowById(FGsSchemaCostumeCommon().id, Temp);
	}
#endif
};
