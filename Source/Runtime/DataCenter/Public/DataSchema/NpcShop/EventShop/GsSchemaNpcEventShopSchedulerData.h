/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcEventShopSchedulerData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/NpcShop/EventShop/BSGsNpcEventShopSchedulerData"))
struct DATACENTER_API FGsSchemaNpcEventShopSchedulerData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="groupId"))
	int32 groupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapId"))
	int32 mapId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnGroupId"))
	int32 spawnGroupId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="startDateTime"))
	FString startDateTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="endDateTime"))
	FString endDateTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="spawnMakeCron"))
	FString spawnMakeCron;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="유지 시간", DisplayName="durationMinutes"))
	int32 durationMinutes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="desc"))
	FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="infomTerritoryId"))
	int32 infomTerritoryId;
public:
	FGsSchemaNpcEventShopSchedulerData();
	
protected:
	bool operator== (const FGsSchemaNpcEventShopSchedulerData& __Other) const
	{
		return FGsSchemaNpcEventShopSchedulerData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcEventShopSchedulerData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcEventShopSchedulerData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcEventShopSchedulerDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcEventShopSchedulerDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcEventShopSchedulerData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcEventShopSchedulerData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcEventShopSchedulerData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcEventShopSchedulerDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcEventShopSchedulerDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcEventShopSchedulerData final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("NpcShop/EventShop");
	}
	
};
