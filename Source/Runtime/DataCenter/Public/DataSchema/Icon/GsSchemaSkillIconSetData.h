/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaSkillIconSetData.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="Skill Icon Set Data", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Icon/BSGsSkillIconSetData"))
struct DATACENTER_API FGsSchemaSkillIconSetData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="iconPath"))
	FSoftObjectPath iconPath;
public:
	FGsSchemaSkillIconSetData();
	
protected:
	bool operator== (const FGsSchemaSkillIconSetData& __Other) const
	{
		return FGsSchemaSkillIconSetData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSkillIconSetData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSkillIconSetData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSkillIconSetDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSkillIconSetDataRow() : Super()
	{
	}
	
	const FGsSchemaSkillIconSetData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSkillIconSetData* Row = nullptr;
		return Table->FindRow<FGsSchemaSkillIconSetData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSkillIconSetDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSkillIconSetDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSkillIconSetData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Icon");
	}
	
};
