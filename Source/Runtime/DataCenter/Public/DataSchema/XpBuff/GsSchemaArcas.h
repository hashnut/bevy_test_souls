/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/XpBuff/GsSchemaArcasBless.h"
#include "Runtime/DataCenter/Public/DataSchema/XpBuff/GsSchemaArcasConfig.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaArcas.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/XpBuff/BSGsArcas"))
struct DATACENTER_API FGsSchemaArcas
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="아르카스 축복 기본 설정", DisplayName="arcasConfig"))
	FGsSchemaArcasConfig arcasConfig;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="arcasBlessList"))
	TArray<FGsSchemaArcasBless> arcasBlessList;
public:
	FGsSchemaArcas();
	
protected:
	bool operator== (const FGsSchemaArcas& __Other) const
	{
		return FGsSchemaArcas::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaArcas::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaArcas::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaArcasRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaArcasRow() : Super()
	{
	}
	
	const FGsSchemaArcas* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaArcas* Row = nullptr;
		return Table->FindRow<FGsSchemaArcas>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaArcasRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaArcasRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableArcas final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("XpBuff");
	}
	
};
