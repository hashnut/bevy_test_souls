/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"
#include "Runtime/DataCenter/Public/DataSchema/Net/GsSchemaNetErrorUI.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNetErrorProcess.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Net/BSGsNetErrorProcess"))
struct DATACENTER_API FGsSchemaNetErrorProcess
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="packetId"))
	FString packetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="useId"))
	FGsSchemaNetErrorUIRow useId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="process"))
	EGsNetErrorProcType process;
public:
	FGsSchemaNetErrorProcess();
	
protected:
	bool operator== (const FGsSchemaNetErrorProcess& __Other) const
	{
		return FGsSchemaNetErrorProcess::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNetErrorProcess::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNetErrorProcess::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNetErrorProcessRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNetErrorProcessRow() : Super()
	{
	}
	
	const FGsSchemaNetErrorProcess* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNetErrorProcess* Row = nullptr;
		return Table->FindRow<FGsSchemaNetErrorProcess>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNetErrorProcessRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNetErrorProcessRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNetErrorProcess final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Net");
	}
	
};
