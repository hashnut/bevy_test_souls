/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaGameMode.generated.h"

USTRUCT(BlueprintType, Meta=(Tooltip="커스터마이징 테스트 모드(false일 경우 기존)", AssetSchemaPackagePath="/Game/DataCenter/DataSchema/BSGsGameMode"))
struct DATACENTER_API FGsSchemaGameMode
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool CustomizeTestMode;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool CustomizeBoneScaleCalcMode;
	
public:
	FGsSchemaGameMode();
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaGameMode::StaticStruct());
	}
	
protected:
	virtual const UScriptStruct* GetThisScriptStruct() const override
	{
		return FGsSchemaGameMode::StaticStruct();
	}
	
	bool operator== (const FGsSchemaGameMode& __Other) const
	{
		return FGsSchemaGameMode::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaGameModeRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaGameModeRow() : Super()
	{
	}
	
	const UGsTable* GetTable() const
	{
		return FGsSchemaGameMode::GetStaticTable();
	}
	
	const FGsSchemaGameMode* GetRow() const
	{
		const FGsSchemaGameMode* Row = nullptr;
		return GetTable()->FindRow<FGsSchemaGameMode>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaGameModeRow>
	: public TStructOpsTypeTraitsBase2<FGsSchemaGameModeRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableGameMode : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const override
	{
		return TEXT("");
	}
	
};
