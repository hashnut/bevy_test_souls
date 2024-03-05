/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaTitleConditionDataEtc.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Title/BSGsTitleConditionDataEtc"))
struct DATACENTER_API FGsSchemaTitleConditionDataEtc
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mParam1"))
	FString mParam1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mParam2"))
	FString mParam2;
public:
	FGsSchemaTitleConditionDataEtc();
	
protected:
	bool operator== (const FGsSchemaTitleConditionDataEtc& __Other) const
	{
		return FGsSchemaTitleConditionDataEtc::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaTitleConditionDataEtc::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaTitleConditionDataEtc::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaTitleConditionDataEtcRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaTitleConditionDataEtcRow() : Super()
	{
	}
	
	const FGsSchemaTitleConditionDataEtc* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaTitleConditionDataEtc* Row = nullptr;
		return Table->FindRow<FGsSchemaTitleConditionDataEtc>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaTitleConditionDataEtcRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaTitleConditionDataEtcRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableTitleConditionDataEtc final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Title");
	}
	
};
