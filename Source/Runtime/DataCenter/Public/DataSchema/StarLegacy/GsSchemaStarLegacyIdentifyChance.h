/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaStarLegacyIdentifyChance.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/StarLegacy/BSGsStarLegacyIdentifyChance"))
struct DATACENTER_API FGsSchemaStarLegacyIdentifyChance
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="FairyGrade"))
	FairyGrade FairyGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Count"))
	int32 Count;
public:
	FGsSchemaStarLegacyIdentifyChance();
	
protected:
	bool operator== (const FGsSchemaStarLegacyIdentifyChance& __Other) const
	{
		return FGsSchemaStarLegacyIdentifyChance::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaStarLegacyIdentifyChance::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaStarLegacyIdentifyChance::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaStarLegacyIdentifyChanceRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaStarLegacyIdentifyChanceRow() : Super()
	{
	}
	
	const FGsSchemaStarLegacyIdentifyChance* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaStarLegacyIdentifyChance* Row = nullptr;
		return Table->FindRow<FGsSchemaStarLegacyIdentifyChance>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaStarLegacyIdentifyChanceRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaStarLegacyIdentifyChanceRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableStarLegacyIdentifyChance final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("StarLegacy");
	}
	
};
