/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaEliteDungeonConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Dungeon/BSGsEliteDungeonConfig"))
struct DATACENTER_API FGsSchemaEliteDungeonConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="randomHitRewardMin"))
	int32 randomHitRewardMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="randomHitRewardMax"))
	int32 randomHitRewardMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="randomHitRewardUserCount"))
	int32 randomHitRewardUserCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="randomHitRewardNSecond"))
	int32 randomHitRewardNSecond;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="afterClearKickTimeS"))
	int32 afterClearKickTimeS;
public:
	FGsSchemaEliteDungeonConfig();
	
protected:
	bool operator== (const FGsSchemaEliteDungeonConfig& __Other) const
	{
		return FGsSchemaEliteDungeonConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaEliteDungeonConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaEliteDungeonConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaEliteDungeonConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaEliteDungeonConfigRow() : Super()
	{
	}
	
	const FGsSchemaEliteDungeonConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaEliteDungeonConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaEliteDungeonConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaEliteDungeonConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaEliteDungeonConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableEliteDungeonConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Dungeon");
	}
	
};
