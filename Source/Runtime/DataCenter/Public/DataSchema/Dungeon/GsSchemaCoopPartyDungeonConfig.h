/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/DataSchema/Map/EventAction/GsSchemaMapEventActionImmediate.h"
#include "Runtime/DataCenter/Public/DataSchema/GsSchemaResetTimeTicketConfig.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCoopPartyDungeonConfig.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/Dungeon/BSGsCoopPartyDungeonConfig"))
struct DATACENTER_API FGsSchemaCoopPartyDungeonConfig
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="weeklyRewardCount"))
	int32 weeklyRewardCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="autoMatchingTimeS"))
	int32 autoMatchingTimeS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="afterReadyKickTimeS"))
	int32 afterReadyKickTimeS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="callHelpCount"))
	int32 callHelpCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="resetRTT"))
	FGsSchemaResetTimeTicketConfigRow resetRTT;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="entranceBanTimeS"))
	int32 entranceBanTimeS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="afterClearKickTimeS"))
	int32 afterClearKickTimeS;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="timerEventId"))
	FGsSchemaMapEventActionImmediateRow timerEventId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="startDelayCount"))
	int32 startDelayCount;
public:
	FGsSchemaCoopPartyDungeonConfig();
	
protected:
	bool operator== (const FGsSchemaCoopPartyDungeonConfig& __Other) const
	{
		return FGsSchemaCoopPartyDungeonConfig::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCoopPartyDungeonConfig::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCoopPartyDungeonConfig::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCoopPartyDungeonConfigRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCoopPartyDungeonConfigRow() : Super()
	{
	}
	
	const FGsSchemaCoopPartyDungeonConfig* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCoopPartyDungeonConfig* Row = nullptr;
		return Table->FindRow<FGsSchemaCoopPartyDungeonConfig>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCoopPartyDungeonConfigRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCoopPartyDungeonConfigRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCoopPartyDungeonConfig final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("Dungeon");
	}
	
};
