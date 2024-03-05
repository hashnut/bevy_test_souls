/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/InterServer/World/GsSchemaWorldData.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaInterServerConnData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/InterServer/BSGsInterServerConnData"))
struct DATACENTER_API FGsSchemaInterServerConnData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="planetId"))
	int64 planetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="worldId"))
	FGsSchemaWorldDataRow worldId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="interServerWorldIdList"))
	TArray<FGsSchemaWorldDataRow> interServerWorldIdList;
public:
	FGsSchemaInterServerConnData();
	
protected:
	bool operator== (const FGsSchemaInterServerConnData& __Other) const
	{
		return FGsSchemaInterServerConnData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaInterServerConnData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaInterServerConnData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaInterServerConnDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaInterServerConnDataRow() : Super()
	{
	}
	
	const FGsSchemaInterServerConnData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaInterServerConnData* Row = nullptr;
		return Table->FindRow<FGsSchemaInterServerConnData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaInterServerConnDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaInterServerConnDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableInterServerConnData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("InterServer");
	}
	
};
