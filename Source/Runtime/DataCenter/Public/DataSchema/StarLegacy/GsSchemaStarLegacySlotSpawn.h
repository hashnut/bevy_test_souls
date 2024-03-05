/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaStarLegacySlotSpawn.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/StarLegacy/BSGsStarLegacySlotSpawn"))
struct DATACENTER_API FGsSchemaStarLegacySlotSpawn
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="legacySlotId"))
	int32 legacySlotId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardDataId"))
	int32 rewardDataId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rewardSpawnRate"))
	int32 rewardSpawnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="itemGrade"))
	ItemGrade itemGrade;
public:
	FGsSchemaStarLegacySlotSpawn();
	
protected:
	bool operator== (const FGsSchemaStarLegacySlotSpawn& __Other) const
	{
		return FGsSchemaStarLegacySlotSpawn::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaStarLegacySlotSpawn::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaStarLegacySlotSpawn::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaStarLegacySlotSpawnRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaStarLegacySlotSpawnRow() : Super()
	{
	}
	
	const FGsSchemaStarLegacySlotSpawn* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaStarLegacySlotSpawn* Row = nullptr;
		return Table->FindRow<FGsSchemaStarLegacySlotSpawn>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaStarLegacySlotSpawnRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaStarLegacySlotSpawnRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableStarLegacySlotSpawn final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("StarLegacy");
	}
	
};
