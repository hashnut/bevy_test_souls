// Fill out your copyright notice in the Description page of Project Settings.


#include "GsFieldBossHandler.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsItemUtil.h"
#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsTableUtil.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Tray/GsUITrayTickerFieldBoss.h"
#include "UI/UIContent/Popup/GsUIPopupWarp.h"

#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "DataSchema/Map/Spawn/GsSchemaSpawnData.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapTerritoryData.h"
#include "DataSchema/Map/GsSchemaMapSpotInfo.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"

#include "Net/GsNetSendServiceWorld.h"


const FTextKey BOSS_DESPAWN_TEXT_KEY = TEXT("BossDespawn");
const FTextKey BOSS_EXISTENCE_TEXT_KEY = TEXT("BossExistence");
const FTextKey BOSS_KILL_TEXT_KEY = TEXT("BossKill");
const FTextKey BOSS_KILL_GET_REWARD_TEXT_KEY = TEXT("BossKill_GetReward");
const FTextKey BOSS_SPAWN_TEXT_KEY = TEXT("BossSpawn");
const FTextKey TICKER_BOSS_KILL = TEXT("Ticker_BossKill");
const FTextKey TICKER_BOSS_SPAWN = TEXT("Ticker_BossSpawn");

const int32 FIELDBOSS_SPAWN_GROUP_COUNT = 1;
const int32 FIELDBOSS_SPAWN_PROPOBILITY = 10000;

int FGsFieldBossHandler::_fieldBossReadyMinutes = 30;

FGsBossRewardInfo::FGsBossRewardInfo(void* ptr)
{
	PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ::ChannelDefeatlist::RankingList::RewardItemList* cast_ptr =
		reinterpret_cast<PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ::ChannelDefeatlist::RankingList::RewardItemList*>(ptr);

	_itemId = cast_ptr->RewardItem().mItemId;
	_count = cast_ptr->RewardItem().mItemAmount;
}

//-------------------------------------------------------------------------------------------------
FGsBossRankInfo::FGsBossRankInfo(void* ptr)
{
	PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ::ChannelDefeatlist::RankingList* cast_ptr =
		reinterpret_cast<PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ::ChannelDefeatlist::RankingList*>(ptr);

	_usereName = cast_ptr->Charname();
	_guildName = cast_ptr->GuildName();
	_worldId = cast_ptr->HomeWorldId();
	_worldPrefix = cast_ptr->HomeWorldPrefix();
	_isParty = cast_ptr->Party();
	_level = cast_ptr->Level();
	_contributeRate = cast_ptr->ContributeRate();
	_socialMediaInfo = cast_ptr->SocialMediaInfo();

	_rewardList.Empty();
	for (PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ::ChannelDefeatlist::RankingList::RewardItemListIterator rewardIter = cast_ptr->GetFirstRewardItemListIterator()
		; rewardIter != cast_ptr->GetLastRewardItemListIterator()
		; ++rewardIter)
	{
		TSharedPtr<FGsBossRewardInfo> temp = MakeShareable(new FGsBossRewardInfo(&(*rewardIter)));
		_rewardList.Add(temp);
	}
}

//-------------------------------------------------------------------------------------------------
FGsChannelDefeatList::FGsChannelDefeatList(void* Ptr)
{
	PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ::ChannelDefeatlist* cast_ptr = reinterpret_cast<PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ::ChannelDefeatlist*>(Ptr);
	_channelId = cast_ptr->ChannelId();	
	_logDateTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(cast_ptr->LogDateTime());

	rankInfoArray.Empty();
	for (PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ::ChannelDefeatlist::RankingListIterator channelIter = cast_ptr->GetFirstRankingListIterator()
		; channelIter != cast_ptr->GetLastRankingListIterator()
		; ++channelIter)
	{
		TSharedPtr<FGsBossRankInfo> temp = MakeShareable(new FGsBossRankInfo(&(*channelIter)));
		rankInfoArray.Add(temp);
	}
}

//-------------------------------------------------------------------------------------------------
FGsResultDepeatFieldBoss::FGsResultDepeatFieldBoss(PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ* inRankInfo)
{
	for (PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ::ChannelDefeatlistIterator channelIter = inRankInfo->GetFirstChannelDefeatlistIterator()
		; channelIter != inRankInfo->GetLastChannelDefeatlistIterator()
		; ++channelIter)
	{
		TSharedPtr<FGsChannelDefeatList> fieldBossInfo = MakeShareable(new FGsChannelDefeatList(&(*channelIter)));
		_channelDefeatArray.Add(fieldBossInfo);
	}
}
//-------------------------------------------------------------------------------------------------

void FGsFieldBossHandler::Init()
{
	FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, BOSS_DESPAWN_TEXT_KEY, _fieldBossDespawnText);
	FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, BOSS_EXISTENCE_TEXT_KEY, _fieldBossExistenceText);
	FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, BOSS_KILL_TEXT_KEY, _fieldBossKillText);
	FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, BOSS_KILL_GET_REWARD_TEXT_KEY, _fieldBossKillGetItemText);
	FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, BOSS_SPAWN_TEXT_KEY, _fieldBossSpawnText);
	FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, TICKER_BOSS_KILL, _tickerFieldBossKillText);
	FText::FindText(FIELD_BOSS_NAMESPACE_TEXT_KEY, TICKER_BOSS_SPAWN, _tickerFieldBossSpawnText);
	
	MSystem& msgSystem = GMessage()->GetSystem();
	_systemMsgList.Add(msgSystem.AddRaw(MessageSystem::WARP_WITH_DESPAWN_READY, this, &FGsFieldBossHandler::OnWarpReady));

	MGameObject& gameObject = GMessage()->GetGameObject();
	_gameObjectHandleArray.Add(gameObject.AddRaw(MessageGameObject::COMMON_DESPAWN, this, &FGsFieldBossHandler::OnRemoveAggroTarget));
	_gameObjectHandleArray.Add(gameObject.AddRaw(MessageGameObject::COMMON_DIE, this, &FGsFieldBossHandler::OnRemoveAggroTarget));

	_fieldBossReadyMinutes = GData()->GetGlobalData()->_fieldBossReadyMinutes;

	ClearAllAggroTarget();
	ClearContribution();
}

void FGsFieldBossHandler::Close()
{
	MSystem& msgSys = GMessage()->GetSystem();
	for (MsgSystemHandle& msgSystemHandle : _systemMsgList)
	{
		msgSys.Remove(msgSystemHandle);
	}
	_systemMsgList.Empty();


	MGameObject& msgGameObject = GMessage()->GetGameObject();
	for (MsgGameObjHandle& msgGameObjectHandle : _gameObjectHandleArray)
	{
		msgGameObject.Remove(msgGameObjectHandle);
	}
	_gameObjectHandleArray.Empty();

	ClearFieldBossInfo();
	ClearAllAggroTarget();
	ClearContribution();

	ClearBossInfoCooperation();
}

void FGsFieldBossHandler::AddFieldBossInfo(FieldBossId inFieldBossId, const FVector& inPos, time_t inEndTime)
{
	FGsFieldBossInfo info;
	info._spawnTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(inEndTime);
	info._spawnPos = inPos;

	_fieldBossMap.Emplace(inFieldBossId, info);
}

void FGsFieldBossHandler::RemoveFieldBossInfo(FieldBossId inFieldBossId)
{
	int32 levelId = GLevel()->GetCurrentLevelId();
	if (_fieldBossMap.Contains(inFieldBossId))
	{
		_fieldBossMap.Remove(inFieldBossId);
	}
}

void FGsFieldBossHandler::ClearFieldBossInfo()
{
	_fieldBossMap.Empty();
}

void FGsFieldBossHandler::OnWarpReady()
{
	ClearFieldBossInfo();
	ClearAllAggroTarget();
	ClearContribution();
}

bool FGsFieldBossHandler::IsBossInfoEmpty()
{
	return (_fieldBossMap.Num() == 0);
}

TArray<const FGsSchemaNpcData*> FGsFieldBossHandler::GetFieldBossNpcDataArray(int32 inLevelId)
{
	TArray<const FGsSchemaFieldBossData*> bossDataArray = GetFieldBossDataByLevelId(inLevelId);
	TArray<const FGsSchemaNpcData*> npcDataArray;
	for (const FGsSchemaFieldBossData* bossData : bossDataArray)
	{
		npcDataArray.Add(bossData->bossNpcId.GetRow());
	}

	return npcDataArray;
}

const struct FGsSchemaFieldBossData* FGsFieldBossHandler::GetFieldBossData(int32 inMapId, CreatureId inNpcId)
{
	const TArray<const FGsSchemaFieldBossData*>& bossDataArray = GetFieldBossDataByLevelId(inMapId);	
	const FGsSchemaFieldBossData* const* find = bossDataArray.FindByPredicate([&](const FGsSchemaFieldBossData* bossData)->bool{
		if (nullptr == bossData
			|| nullptr == bossData->bossNpcId.GetRow())
		{
			return false;
		}
		return bossData->bossNpcId.GetRow()->id == inNpcId;
	});

	if (find)
	{
		return (*find);
	}

	return nullptr;
}

bool FGsFieldBossHandler::TryGetRemainFieldBossTime(FieldBossId inFieldBossCreatureId, FTimespan& outRemainTime)
{
	if (_fieldBossMap.Contains(inFieldBossCreatureId))
	{
		const FDateTime& spawnTime = _fieldBossMap[inFieldBossCreatureId]._spawnTime;
		outRemainTime = spawnTime - FGsTimeSyncUtil::GetClientNowDateTime();
		return true;
	}

	return false;
}

bool FGsFieldBossHandler::TryGetFieldBossState(FieldBossId inFieldBossCreatureId, EGsFieldBossState& outFieldBossState)
{
	// 스폰 시간 지났으면 스폰
	// 안지났으면
	// 30분 이내면 레디
	// 나머진 디스폰
	outFieldBossState = EGsFieldBossState::Despawn;

	if (_fieldBossMap.Contains(inFieldBossCreatureId))
	{
		const FGsFieldBossInfo& info = _fieldBossMap[inFieldBossCreatureId];
		outFieldBossState = GetBossState(true, info._spawnTime);

		return true;
	}

	return false;
}

bool FGsFieldBossHandler::TryGetFieldBossSpawnTime(FieldBossId inFieldBossId, FDateTime& outFieldBossSpawnTime)
{
	if (_fieldBossMap.Contains(inFieldBossId))
	{
		const FGsFieldBossInfo& info = _fieldBossMap[inFieldBossId];
		outFieldBossSpawnTime = info._spawnTime;
		return true;
	}
	return false;
}

void FGsFieldBossHandler::TryGetFieldBossInfos(FieldBossId inFieldBossCreatureId,
	TArray<FVector>& outSpawnPosArray,
	int& outMapId, int& outWarpSpotId)
{
	outSpawnPosArray.Empty();
	const FGsSchemaFieldBossData* fieldBossData = FindFieldBossData(inFieldBossCreatureId);
	if (nullptr == fieldBossData)
	{
		return;
	}

	TArray<const FGsSchemaSpawnData*> spawnDataArray;
	int mapId = fieldBossData->mapId.GetRow()->id;
	outMapId = mapId;
	outWarpSpotId = fieldBossData->WarpSpotId;

	UGsLevelUtil::TryGetSpawnData(mapId, spawnDataArray);
	const FGsSchemaSpawnData** find = nullptr;
	for (int32 groupId : fieldBossData->spawnGroupIdList)
	{
		find = spawnDataArray.FindByPredicate([&](const FGsSchemaSpawnData* spawnData)->bool {
			return spawnData->SpawnGroupId == groupId;
			});
		if (find && (*find))
		{
			outSpawnPosArray.Add((*find)->Region.Center);
		}
	}
}

bool FGsFieldBossHandler::IsNeedToBeShownWorldMap(FieldBossId inFieldBossId)
{
	const FGsSchemaFieldBossData* data = FindFieldBossData(inFieldBossId);
	if (nullptr == data)
	{
		return false;
	}

	if (data->worldMapMark
		&& data->spawnProbability >= FIELDBOSS_SPAWN_PROPOBILITY)
	{
		return true;
	}

	return false;
}

const struct FGsSchemaFieldBossData* FGsFieldBossHandler::FindFieldBossData(FieldBossId inFieldBoss)
{
	const UGsTable* table = FGsSchemaFieldBossData::GetStaticTable();
	const UGsTableFieldBossData* bossTable = Cast<const UGsTableFieldBossData>(table);
	if (nullptr == bossTable)
	{
		GSLOG(Error, TEXT("boss table is missing"));
		return nullptr;
	}

	TArray<const FGsSchemaFieldBossData*> fieldBossArray;
	bossTable->GetAllRows(fieldBossArray);

	const FGsSchemaFieldBossData** find = fieldBossArray.FindByPredicate([&](const FGsSchemaFieldBossData* inData)->bool {
		return inFieldBoss == inData->bossId;
		});

	return ((find) ? (*find) : nullptr);
}

const struct FGsSchemaFieldBossData* FGsFieldBossHandler::FindFieldBossDataByMutantBossId(FieldBossId inMutantBossldBoss)
{
	const UGsTable* table = FGsSchemaFieldBossData::GetStaticTable();
	const UGsTableFieldBossData* bossTable = Cast<const UGsTableFieldBossData>(table);

	TArray<const FGsSchemaFieldBossData*> fieldBossArray;
	bossTable->GetAllRows(fieldBossArray);

	const FGsSchemaFieldBossData** find = fieldBossArray.FindByPredicate([&](const FGsSchemaFieldBossData* inData)->bool {
		return inMutantBossldBoss == inData->mutantApear.fieldbossDataId;
		});

	if (find)
	{
		return (*find);
	}


	return nullptr;
}

TArray<const FGsSchemaFieldBossData*> FGsFieldBossHandler::GetFieldBossDataByLevelId(int32 inLevelId)
{
	TArray<const FGsSchemaFieldBossData*> bossArray;
	const UGsTable* table = FGsSchemaFieldBossData::GetStaticTable();
	const UGsTableFieldBossData* bossTable = Cast<const UGsTableFieldBossData>(table);
	if (nullptr == bossTable)
	{
		GSLOG(Error, TEXT("boss table is missing"));
		return bossArray;
	}

	TArray<const FGsSchemaFieldBossData*> fieldBossArray;
	bossTable->GetAllRows(fieldBossArray);
	const FGsSchemaMapData* mapData = nullptr;

	for (const FGsSchemaFieldBossData* fieldBoss : fieldBossArray)
	{
		if (nullptr == fieldBoss)
		{
			continue;
		}

		mapData = fieldBoss->mapId.GetRow();		
		if (mapData
		&& mapData->id == inLevelId)
		{
			bossArray.Add(fieldBoss);
		}
	}

	return bossArray;
}

void FGsFieldBossHandler::OnFieldBossSpawn(const IGsMessageParam* InParam)
{
	const FGsUIMsgFieldBoss* param = InParam->Cast<const FGsUIMsgFieldBoss>();
	if (nullptr == param)
	{
		return;
	}

	const FGsSchemaFieldBossData* fieldBossData = FindFieldBossData(param->_fieldBossId);
	if (nullptr == fieldBossData
	|| nullptr == fieldBossData->bossNpcId.GetRow())
	{
		return;
	}

	CreatureId bossId = fieldBossData->bossNpcId.GetRow()->id;	
	FGsUIHelper::TrayTickerBoss(EGsBossTickerStateType::Spawn, bossId);	
	FGsUIHelper::AddChatMessageSystem(FText::Format(_fieldBossSpawnText, UGsGameObjectUtil::GetNpcName(bossId)));
}

void FGsFieldBossHandler::OnFieldBossDespawn(const IGsMessageParam* InParam)
{
	const FGsUIMsgFieldBoss* param = InParam->Cast<const FGsUIMsgFieldBoss>();
	if (nullptr == param)
	{
		return;
	}

	const FGsSchemaFieldBossData* fieldBossData = FindFieldBossData(param->_fieldBossId);
	if (nullptr == fieldBossData
	|| nullptr == fieldBossData->bossNpcId.GetRow())
	{
		return;
	}

	const FText& tickerMsg = FText::Format(_fieldBossDespawnText, UGsGameObjectUtil::GetNpcName(fieldBossData->bossNpcId.GetRow()->id));
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, tickerMsg, TrayCondition::TC_Close);
	FGsUIHelper::AddChatMessageSystem(tickerMsg);
}

void FGsFieldBossHandler::OnFieldBossDie(const IGsMessageParam* InParam)
{
	const FGsUIMsgFieldBoss* param = InParam->Cast<const FGsUIMsgFieldBoss>();
	if (nullptr == param)
	{
		return;
	}

	const FGsSchemaFieldBossData* fieldBossData = FindFieldBossData(param->_fieldBossId);
	if (nullptr == fieldBossData
	|| nullptr == fieldBossData->bossNpcId.GetRow())
	{
		return;
	}

	//show hud		
	CreatureId npcId = fieldBossData->bossNpcId.GetRow()->id;
	FGsUIHelper::TrayTickerBoss(EGsBossTickerStateType::Die, npcId);	
	FGsUIHelper::AddChatMessageSystem(FText::Format(_fieldBossKillText, UGsGameObjectUtil::GetNpcName(npcId)));
}

void FGsFieldBossHandler::OnFieldBossItemGet(const IGsMessageParam* InParam)
{
	//상황에 상관없이 1.5초 딜레이 후에 Ticker를 띄우는 것으로 바뀌어 받은 곳에서 해준다.
	const FGsUIMsgParamSystemMessage* param = InParam->Cast<const FGsUIMsgParamSystemMessage>();
	if (nullptr == param)
	{
		return;
	}

	//상황에 상관없이 1.5초 딜레이 후에 Ticker를 띄움
	//FGsUIHelper::TraySectionMessageDelayTicker(param->_sectionType, param->_systemMessage, 1.5f, SectionPriority::Insert);
}

void FGsFieldBossHandler::SetDefeatResult(PD::SC::PKT_SC_ACK_RESULT_DEFEAT_FILEDBOSS_READ* inDefeatResult)
{
	FieldBossId bossId = inDefeatResult->FieldBossId();
	TSharedPtr<FGsResultDepeatFieldBoss> fieldBoss = MakeShareable(new FGsResultDepeatFieldBoss(inDefeatResult));
	depeatResultFieldBossMap.Emplace(bossId, fieldBoss);
}

// 스폰 시간 지났으면 스폰
// 안지났으면
// 30분 이내면 레디
// 나머진 디스폰
EGsFieldBossState FGsFieldBossHandler::GetBossState(bool In_isHaveSpawnTime, FDateTime In_spawnTime)
{
	if (In_isHaveSpawnTime == false)
	{
		return EGsFieldBossState::Despawn;
	}

	FDateTime nowTime = FGsTimeSyncUtil::GetServerNowDateTimeUTC();
	bool isSpawn = In_spawnTime < nowTime;
	if (isSpawn == false)
	{
		FTimespan remainDateTime = In_spawnTime - nowTime;

		int minutes = remainDateTime.GetMinutes();

		// 대기 시간 이상이면
		if (remainDateTime.GetTotalMinutes() > _fieldBossReadyMinutes)
		{
			return EGsFieldBossState::Despawn;
		}
		else
		{
			return EGsFieldBossState::Ready;
		}
	}
	return EGsFieldBossState::Spawn;
}

int64 FGsFieldBossHandler::GetAggroTarget(int64 inNpcGameId)
{
	if (_aggroTargetMap.Contains(inNpcGameId))
	{
		return _aggroTargetMap[inNpcGameId];
	}

	return 0;
}

void FGsFieldBossHandler::AddAggroTarget(int64 inNpcGameId, int64 inTargetGameId)
{
	_aggroTargetMap.Emplace(inNpcGameId, inTargetGameId);

#if AGGRO_DEBUG
	FString userName;
	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		UGsGameObjectBase* findObject = mgr->FindObject(EGsGameObjectType::Creature, inTargetGameId);
		if (findObject)
		{			
			UGsGameObjectPlayer* playerGameObject = Cast<UGsGameObjectPlayer>(findObject);
			if (playerGameObject)
			{
				userName = playerGameObject->GetData()->GetName();
			}
		}		
	}

	GSLOG(Log, TEXT("FGsFieldBossHandler::AddAggroTarget - npc game id : %lld\ttarget game id : %lld\tname : %s")
		, inNpcGameId
		, inTargetGameId
		, *userName);
#endif
}

void FGsFieldBossHandler::OnRemoveAggroTarget(const IGsMessageParam* InParam)
{
	if (nullptr == InParam)
	{
		return;
	}

	const FGsGameObjectMessageParam* param = InParam->Cast<const FGsGameObjectMessageParam>();	
	if (nullptr == param)
	{
		return;
	}

	UGsGameObjectCreature* creature = Cast<UGsGameObjectCreature>(param->_paramOwner);
	if (nullptr == creature)
	{
		return;
	}

	ClearAggroTarget(creature->GetGameId());
}

bool FGsFieldBossHandler::ClearAggroTarget(int64 inNpcGameId)
{
	if (_aggroTargetMap.Contains(inNpcGameId))
	{
		_aggroTargetMap.Remove(inNpcGameId);
		
#if AGGRO_DEBUG
		GSLOG(Log, TEXT("AGGRO_DEBUG - ClearAggroTarget - npc game id : %lld"), inNpcGameId);
#endif
		return true;
	}	

	return false;
}

void FGsFieldBossHandler::ClearAllAggroTarget()
{
	_aggroTargetMap.Empty();
}

FGsFieldBossInfoCooperation* FGsFieldBossHandler::FindBossInfoCooperation(FieldBossId In_fieldBossId)
{
	FGsFieldBossInfoCooperation* findInfo =
		_mapCooperationBossInfo.FindRef(In_fieldBossId);

	if (findInfo == nullptr)
	{
		findInfo = new FGsFieldBossInfoCooperation();
		
		int mapId = 0;
		int warpSpotId = 0;
		TArray<FVector> spawnArray;
		TryGetFieldBossInfos(In_fieldBossId, spawnArray, mapId, warpSpotId);
		if (spawnArray.Num() != 0)
		{
			findInfo->_spawnPos = spawnArray[0];
		}

		
		int64 findSpotId = -1;
		FText findSpotName;
		FVector findSpotPos = FVector::ZeroVector;

		FGsClientSpawnManager* spawnManager = GSClientSpawn();
		if (spawnManager != nullptr)
		{
			FGsSchemaSpotInfo findSpotInfo;
			if (spawnManager->TryGetSpot(
				mapId, warpSpotId, findSpotInfo) == true)
			{
				findSpotId = findSpotInfo.id;
				findSpotName = findSpotInfo.name;
				findSpotPos = findSpotInfo.pos;
			}			
		}		

		findInfo->_nearTeleportSpotId = findSpotId;
		findInfo->_spotName = findSpotName;
		findInfo->_autoMoveTargetPos = findSpotPos;

		_mapCooperationBossInfo.Add(In_fieldBossId, findInfo);
	}

	return findInfo;
}

void FGsFieldBossHandler::ClearBossInfoCooperation()
{
	if (_mapCooperationBossInfo.Num() != 0)
	{
		for (auto iter : _mapCooperationBossInfo)
		{
			if (iter.Value == nullptr)
			{
				continue;
			}
			delete iter.Value;
		}
		_mapCooperationBossInfo.Empty();
	}
}

void FGsFieldBossHandler::MoveToFieldBoss(FieldBossId In_bossId)
{
	FGsFieldBossInfoCooperation* findInfo = FindBossInfoCooperation(In_bossId);
	if (nullptr == findInfo)
	{
		return;
	}

	const FGsSchemaFieldBossData* fieldBossData = FindFieldBossData(In_bossId);
	if (nullptr == fieldBossData
	|| nullptr == fieldBossData->mapId.GetRow())
	{
		return;
	}
	
	GSAI()->MoveToPosAuto(EGsMovementAutoContentsType::FieldBossMove, fieldBossData->mapId.GetRow()->id,
		findInfo->_autoMoveTargetPos);
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	GUI()->CloseAllStack();
}
void FGsFieldBossHandler::WarpToFieldBoss(FieldBossId In_bossId)
{
	FGsFieldBossInfoCooperation* findInfo =	FindBossInfoCooperation(In_bossId);
	if (findInfo == nullptr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupWarp"));
	if (widget.IsValid() == false)
	{
		return;
	}

	UGsUIPopupWarp* popupWarp = Cast<UGsUIPopupWarp>(widget.Get());
	if (popupWarp == nullptr)
	{
		return;
	}

	const FGsSchemaFieldBossData* fieldBossData = FindFieldBossData(In_bossId);
	if (nullptr == fieldBossData
	|| nullptr == fieldBossData->mapId.GetRow())
	{
		return;
	}

	int32 mapId = fieldBossData->mapId.GetRow()->id;
	CurrencyType currencyType = CurrencyType::GOLD;
	int useCost = 0;

	const FGsSchemaMapData* mapTbl = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(mapId);
	if (mapTbl)
	{
		const FGsSchemaMapBalance* mapBalanceData = mapTbl->balanceId.GetRow();
		if (mapBalanceData)
		{
			currencyType = mapBalanceData->cost.costCurrencyType;
			useCost = mapBalanceData->cost.costCurrencyAmount;
		}
	}

	popupWarp->SetData(mapTbl->MapStringId.ToString(),
		FGsCostPackage::CreateSimple(currencyType, useCost),
		[mapId, findInfo]()
		{
			int32 level = GGameData()->GetUserData()->mLevel;
			if (UGsLevelManager::IsCanSpotWarp(mapId, findInfo->_nearTeleportSpotId, level))
			{
				// 서버 전송
				FGsNetSendServiceWorld::SendPortalWarp(mapId, findInfo->_nearTeleportSpotId);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, nullptr);
			}

			// main hud로 이동
			GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);

			GUI()->CloseAllStack();
		});
}

// 필드 보스 디스폰
// https://jira.com2us.com/jira/browse/CHR-23655
void FGsFieldBossHandler::FieldBossDespawn(FieldBossId In_fieldBossId)
{
	const FGsSchemaFieldBossData* bossData =  FindFieldBossData(In_fieldBossId);
	if (bossData == nullptr)
	{
		return;
	}

	if (nullptr == bossData->bossNpcId.GetRow())
	{
		return;
	}

	FGsUIHelper::TrayTickerBossDespawn(bossData->bossNpcId.GetRow()->id);
}