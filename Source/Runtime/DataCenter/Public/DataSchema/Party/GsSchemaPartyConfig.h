/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaPartyConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Party/BSGsPartyConfig"))
struct DATACENTER_API FGsSchemaPartyConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="partyNumber"))
	int32 partyNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="partyBonusGold"))
	float partyBonusGold;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="partyBonusExp"))
	float partyBonusExp;
public:
	FGsSchemaPartyConfig();
	
protected:
	bool operator== (const FGsSchemaPartyConfig& __Other) const
	{
		return FGsSchemaPartyConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaPartyConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaPartyConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaPartyConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaPartyConfigRow() : Super()
	{
	}
	
	const FGsSchemaPartyConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaPartyConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaPartyConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaPartyConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaPartyConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTablePartyConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Party");
	}
	
};
