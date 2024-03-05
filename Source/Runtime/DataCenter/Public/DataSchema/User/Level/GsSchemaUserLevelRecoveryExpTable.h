/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "DataSchema/User/Level/GsSchemaUserLevelRecoveryExpInfo.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaUserLevelRecoveryExpTable.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/User/Level/BSGsUserLevelRecoveryExpTable"))
struct DATACENTER_API FGsSchemaUserLevelRecoveryExpTable
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="LevelExpRecovery", Tooltip))
	TArray<FGsSchemaUserLevelRecoveryExpInfo> LevelExpRecovery;
	
public:
	FGsSchemaUserLevelRecoveryExpTable();
	
protected:
	bool operator== (const FGsSchemaUserLevelRecoveryExpTable& __Other) const
	{
		return FGsSchemaUserLevelRecoveryExpTable::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaUserLevelRecoveryExpTable::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaUserLevelRecoveryExpTable::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaUserLevelRecoveryExpTableRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaUserLevelRecoveryExpTableRow() : Super()
	{
	}
	
	const FGsSchemaUserLevelRecoveryExpTable* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaUserLevelRecoveryExpTable* Row = nullptr;
		return Table->FindRow<FGsSchemaUserLevelRecoveryExpTable>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaUserLevelRecoveryExpTableRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaUserLevelRecoveryExpTableRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableUserLevelRecoveryExpTable final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("User/Level");
	}
	
};
