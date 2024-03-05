/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/Box/GsSchemaBox.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaMapBlockInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Map/Box/BSGsMapBlockInfo"))
struct DATACENTER_API FGsSchemaMapBlockInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="maplId"))
	int32 maplId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="blockList"))
	TArray<FGsSchemaBox> blockList;
public:
	FGsSchemaMapBlockInfo();
	
protected:
	bool operator== (const FGsSchemaMapBlockInfo& __Other) const
	{
		return FGsSchemaMapBlockInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaMapBlockInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaMapBlockInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaMapBlockInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaMapBlockInfoRow() : Super()
	{
	}
	
	const FGsSchemaMapBlockInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaMapBlockInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaMapBlockInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaMapBlockInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaMapBlockInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableMapBlockInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Map/Box");
	}
	
};
