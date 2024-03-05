/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaOfflinePlay.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/OfflinePlay/BSGsOfflinePlay"))
struct DATACENTER_API FGsSchemaOfflinePlay
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="defaultUseMinute", Tooltip))
	int32 defaultUseMinute;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 terminateDelaySeconds;
public:
	FGsSchemaOfflinePlay();
	
protected:
	bool operator== (const FGsSchemaOfflinePlay& __Other) const
	{
		return FGsSchemaOfflinePlay::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaOfflinePlay::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaOfflinePlay::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaOfflinePlayRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaOfflinePlayRow() : Super()
	{
	}
	
	const FGsSchemaOfflinePlay* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaOfflinePlay* Row = nullptr;
		return Table->FindRow<FGsSchemaOfflinePlay>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaOfflinePlayRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaOfflinePlayRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableOfflinePlay final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("OfflinePlay");
	}
	
};
