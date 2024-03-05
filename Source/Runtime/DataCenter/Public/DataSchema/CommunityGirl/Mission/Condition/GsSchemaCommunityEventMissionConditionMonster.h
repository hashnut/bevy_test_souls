/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaCommunityEventMissionConditionMonster.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/CommunityGirl/Mission/Condition/BSGsCommunityEventMissionConditionMonster"))
struct DATACENTER_API FGsSchemaCommunityEventMissionConditionMonster
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="monsterId"))
	int32 monsterId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="mapIdList"))
	TArray<int32> mapIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="monsterRace"))
	CreatureRaceType monsterRace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcFunction"))
	NpcFunctionType npcFunction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="monsterGradeMin"))
	CreatureRankType monsterGradeMin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="monsterGradeMax"))
	CreatureRankType monsterGradeMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contributeRank"))
	int32 contributeRank;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="enterWorldServerType"))
	EnterWorldServerType enterWorldServerType;
public:
	FGsSchemaCommunityEventMissionConditionMonster();
	
protected:
	bool operator== (const FGsSchemaCommunityEventMissionConditionMonster& __Other) const
	{
		return FGsSchemaCommunityEventMissionConditionMonster::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaCommunityEventMissionConditionMonster::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaCommunityEventMissionConditionMonster::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaCommunityEventMissionConditionMonsterRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaCommunityEventMissionConditionMonsterRow() : Super()
	{
	}
	
	const FGsSchemaCommunityEventMissionConditionMonster* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaCommunityEventMissionConditionMonster* Row = nullptr;
		return Table->FindRow<FGsSchemaCommunityEventMissionConditionMonster>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaCommunityEventMissionConditionMonsterRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaCommunityEventMissionConditionMonsterRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableCommunityEventMissionConditionMonster final : public UGsTable
{
	GENERATED_BODY()

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("CommunityGirl/Mission/Condition");
	}
	
};
