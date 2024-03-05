/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Item/GsStatTypeNameSelector.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaLevelStatTable.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/Level/BSGsLevelStatTable"))
struct DATACENTER_API FGsSchemaLevelStatTable
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="level"))
	int32 level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statPoint"))
	int32 statPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType1"))
	FGsStatTypeNameSelector statType1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue1"))
	int32 statValue1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType2"))
	FGsStatTypeNameSelector statType2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue2"))
	int32 statValue2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType3"))
	FGsStatTypeNameSelector statType3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue3"))
	int32 statValue3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType4"))
	FGsStatTypeNameSelector statType4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue4"))
	int32 statValue4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType5"))
	FGsStatTypeNameSelector statType5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue5"))
	int32 statValue5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType6"))
	FGsStatTypeNameSelector statType6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue6"))
	int32 statValue6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType7"))
	FGsStatTypeNameSelector statType7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue7"))
	int32 statValue7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType8"))
	FGsStatTypeNameSelector statType8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue8"))
	int32 statValue8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType9"))
	FGsStatTypeNameSelector statType9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue9"))
	int32 statValue9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statType10"))
	FGsStatTypeNameSelector statType10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="statValue10"))
	int32 statValue10;
public:
	FGsSchemaLevelStatTable();
	
protected:
	bool operator== (const FGsSchemaLevelStatTable& __Other) const
	{
		return FGsSchemaLevelStatTable::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaLevelStatTable::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaLevelStatTable::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaLevelStatTableRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaLevelStatTableRow() : Super()
	{
	}
	
	const FGsSchemaLevelStatTable* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaLevelStatTable* Row = nullptr;
		return Table->FindRow<FGsSchemaLevelStatTable>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaLevelStatTableRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaLevelStatTableRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableLevelStatTable final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("User/Level");
	}
	
};
