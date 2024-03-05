/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Costume/GsSchemaCostumeData.h"
#include "Runtime/DataCenter/Public/DataSchema/Fairy/GsSchemaFairyData.h"
#include "Runtime/DataCenter/Public/DataSchema/Item/GsSchemaItemData.h"
#include "Runtime/DataCenter/Public/DataSchema/Location/GsSchemaLocationData.h"
#include "Runtime/DataCenter/Public/DataSchema/Skill/GsSchemaSkillData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUserInitialData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/BSGsUserInitialData"))
struct DATACENTER_API FGsSchemaUserInitialData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="gold"))
	int32 gold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="locationData"))
	TArray<FGsSchemaLocationData> locationData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemDataList"))
	TArray<FGsSchemaItemData> itemDataList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="skillDataList"))
	TArray<FGsSchemaSkillData> skillDataList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="fairyDataList"))
	TArray<FGsSchemaFairyData> fairyDataList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="costumeDataList"))
	TArray<FGsSchemaCostumeData> costumeDataList;
public:
	FGsSchemaUserInitialData();
	
protected:
	bool operator== (const FGsSchemaUserInitialData& __Other) const
	{
		return FGsSchemaUserInitialData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaUserInitialData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaUserInitialData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaUserInitialDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaUserInitialDataRow() : Super()
	{
	}
	
	const FGsSchemaUserInitialData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaUserInitialData* Row = nullptr;
		return Table->FindRow<FGsSchemaUserInitialData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaUserInitialDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaUserInitialDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableUserInitialData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("User");
	}
	
};
