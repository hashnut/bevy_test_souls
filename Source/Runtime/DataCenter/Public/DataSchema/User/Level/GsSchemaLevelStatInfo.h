/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "Item/GsStatTypeNameSelector.h"
#include "GsSchemaLevelStatInfo.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/Level/BSGsLevelStatInfo"))
struct DATACENTER_API FGsSchemaLevelStatInfo
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="Type"))
	FGsStatTypeNameSelector Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="Value"))
	int32 Value;
public:
	FGsSchemaLevelStatInfo();
	
protected:
	bool operator== (const FGsSchemaLevelStatInfo& __Other) const
	{
		return FGsSchemaLevelStatInfo::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaLevelStatInfo::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaLevelStatInfo::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaLevelStatInfoRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaLevelStatInfoRow() : Super()
	{
	}
	
	const FGsSchemaLevelStatInfo* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaLevelStatInfo* Row = nullptr;
		return Table->FindRow<FGsSchemaLevelStatInfo>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaLevelStatInfoRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaLevelStatInfoRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableLevelStatInfo final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("User/Level");
	}
	
};
