/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
class USoundMix;
#include "GsSchemaSoundMixData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Sound/BSGsSoundMixData"))
struct DATACENTER_API FGsSchemaSoundMixData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="mixTypeID"))
	EGsSoundMixType mixTypeID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="soundMix"))
	USoundMix* soundMix;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="soundMixPath"))
	FSoftObjectPath soundMixPath;
public:
	FGsSchemaSoundMixData();
	
protected:
	bool operator== (const FGsSchemaSoundMixData& __Other) const
	{
		return FGsSchemaSoundMixData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaSoundMixData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaSoundMixData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaSoundMixDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaSoundMixDataRow() : Super()
	{
	}
	
	const FGsSchemaSoundMixData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaSoundMixData* Row = nullptr;
		return Table->FindRow<FGsSchemaSoundMixData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaSoundMixDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaSoundMixDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableSoundMixData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Sound");
	}
	
};
