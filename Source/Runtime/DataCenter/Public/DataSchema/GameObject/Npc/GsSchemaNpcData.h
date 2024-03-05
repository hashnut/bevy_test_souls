/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#pragma once
#include "Runtime/Engine/Public/GeneratedCodeHelpers.h"
#include "Runtime/CoreUObject/Public/Blueprint/BlueprintSupport.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCombatEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "Runtime/DataCenter/Public/DataSchema/MonsterKnowledgeCollection/GsSchemaMonsterKnowledgeLevelSet.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcCommonActionData.h"
#include "Runtime/DataCenter/Public/DataSchema/Stat/GsSchemaNpcCorrectionStatData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/Npc/GsSchemaNpcReactionData.h"
#include "Runtime/DataCenter/Public/DataSchema/GameObject/GsSchemaShapeData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedIffEnum.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsSchemaRowBase.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/GsTableManager.h"
#include "GsSchemaNpcData.generated.h"

USTRUCT(BlueprintType, Meta=(AssetSchemaPackagePath="/Game/DataCenter/DataSchema/GameObject/Npc/BSGsNpcData"))
struct DATACENTER_API FGsSchemaNpcData
	: public FGsSchemaBase
{
	GENERATED_BODY()
	SCHEMA_BASE_GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="id"))
	int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="nameText"))
	FText nameText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="balanceId"))
	FGsSchemaNpcBalanceRow balanceId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="fixedLevel"))
	int32 fixedLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="고정형 Npc 구분을 위한 이동 타입", DisplayName="moveType"))
	CreatureMoveType moveType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="iffFactionType"))
	IffFactionType iffFactionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="npcFunctionType"))
	NpcFunctionType npcFunctionType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="NPC에 세력을 구분하는 값으로 사용", DisplayName="npcFactionId"))
	int32 npcFactionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="commonInfoId"))
	FGsSchemaCreatureCommonInfoRow commonInfoId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="shapeId"))
	FGsSchemaShapeDataRow shapeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="scale"))
	float scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="balloonMessageId"))
	int32 balloonMessageId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="npcAIId"))
	int32 npcAIId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="interactionRange"))
	int32 interactionRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="race"))
	CreatureRaceType race;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="attackType"))
	AttackType attackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="defenseMaterial"))
	ItemMaterial defenseMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="element"))
	MonsterElement element;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="commonActionListId"))
	FGsSchemaNpcCommonActionDataRow commonActionListId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="reactionListId"))
	FGsSchemaNpcReactionDataRow reactionListId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="shopGroupIdList"))
	TArray<int32> shopGroupIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="스폰 액션 출력 시간", DisplayName="spawnActionTime"))
	float spawnActionTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contributionDistance"))
	int32 contributionDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="contributionDelete"))
	bool contributionDelete;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="canItemDropOverride"))
	CanItemDrop canItemDropOverride;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="rotateSpeed"))
	int32 rotateSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isOffensive"))
	bool isOffensive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="sightRadius"))
	float sightRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="obsoleted"))
	bool obsoleted;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="questBoardGroupIdList"))
	int32 questBoardGroupIdList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="NpcFunctionDesc"))
	FText NpcFunctionDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="isPalyBalloon"))
	bool isPalyBalloon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="balloonData"))
	int32 balloonData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="grade"))
	CreatureRankType grade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="increaseArcasPoint"))
	int32 increaseArcasPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="decreaseArcasPoint"))
	int32 decreaseArcasPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(DisplayName="pvpAlignmentPoint"))
	int32 pvpAlignmentPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="monsterKnowledgeLevelSetId"))
	FGsSchemaMonsterKnowledgeLevelSetRow monsterKnowledgeLevelSetId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="npcCorrectionList"))
	TArray<FGsSchemaNpcCorrectionStatDataRow> npcCorrectionList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(DisplayName="polymorpShapeId"))
	FGsSchemaShapeDataRow polymorpShapeId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="해당 Npc 스폰 후 Move 하기 전까지의 시간", DisplayName="spawnMove"))
	float spawnMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta=(Tooltip="해당 Npc가 선공 몬스터일 경우, 대상 인식 후 전투 대상으로 설정하기 까지 걸리는 시간", DisplayName="attackCognition"))
	float attackCognition;
public:
	FGsSchemaNpcData();
	
protected:
	bool operator== (const FGsSchemaNpcData& __Other) const
	{
		return FGsSchemaNpcData::StaticStruct()->CompareScriptStruct(this, &__Other, 0);
	};
	
public:
	static const UGsTable* GetStaticTable()
	{
		return UGsTableManager::GetInstance().GetTable(FGsSchemaNpcData::StaticStruct());
	}
	
protected:
	const UScriptStruct* GetThisScriptStruct() const final
	{
		return FGsSchemaNpcData::StaticStruct();
	}
	
};

USTRUCT(BlueprintType)
struct DATACENTER_API FGsSchemaNpcDataRow : public FGsSchemaRowBase
{
	GENERATED_BODY()
	
	FGsSchemaNpcDataRow() : Super()
	{
	}
	
	const FGsSchemaNpcData* GetRow() const
	{
		const UGsTable* Table = GetTable();
		if (nullptr == Table || !Table->IsValidLowLevel())
		{
			return nullptr;
		}
		const FGsSchemaNpcData* Row = nullptr;
		return Table->FindRow<FGsSchemaNpcData>(RowName, Row) ? Row : nullptr;
	}
};

template<>
struct TStructOpsTypeTraits<FGsSchemaNpcDataRow> final
	: public TStructOpsTypeTraitsBase2<FGsSchemaNpcDataRow>
{
	enum
	{
		WithPostSerialize = true,
	};
};

UCLASS(BlueprintType)
class DATACENTER_API UGsTableNpcData final : public UGsTable
{
	GENERATED_BODY()

protected:
	TMap<int32, FName> _indexById;

protected:
	FString GetInnerPackagePath() const final
	{
		return TEXT("GameObject/Npc");
	}
	
	void OnCachedDataTableChanged() final
	{
		TMap<FName, const FGsSchemaNpcData*> RowsMap;
		if (!CreateAllRowsMap<const FGsSchemaNpcData>(RowsMap))
		{
			return;
		}
		_indexById.Empty(RowsMap.Num());
		for (auto Row : RowsMap)
		{
			_indexById.Emplace(Row.Value->id, Row.Key);
		}
	}
	
public:
	bool FindRowById(const int32& InId, OUT const FGsSchemaNpcData*& OutRow) const
	{
		return FindInIndexMap(_indexById, InId, OutRow);
	}
	
#if WITH_EDITOR
	void RemoveRowById(const int32& InId)
	{
		RemoveInIndexMap(_indexById, InId);
	}
	
	bool IsValidAllPrimaryKeys() const final
	{
		const FGsSchemaNpcData* Temp;
		return !FindRowById(FGsSchemaNpcData().id, Temp);
	}
#endif
};
