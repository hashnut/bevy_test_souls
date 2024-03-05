// Fill out your copyright notice in the Description page of Project Settings.


#include "GsWorldBossHandler.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Message/MessageParam/GsBossMessageParam.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageBoss.h"
#include "DataSchema/WorldBoss/GsSchemaWorldBossData.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "UTIL/GsTableUtil.h"
#include "Net/GsNetSendServiceBoss.h"
#include "Currency/GsCostPackage.h"
#include "UI/UIContent/Popup/GsUIPopupWarp.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoCurrency.h"
#include "UI/UIContent/Popup/GsUIPopupCommonPay.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/Stage/StageGame/GsStageManagerGame.h"
#include "GameFlow/Stage/GsStageMode.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "UI/UIContent/Common/Entry/GsUIItemIconListViewEntry.h"

const FName WORLD_BOSS_RESULT = TEXT("WindowWorldBossResult");
const FTextKey POPUP_COMMON_ENTER1 = TEXT("Popup_Common_Enter1");
const FTextKey POPUP_COMMON_ENTER2 = TEXT("Popup_Common_Enter2");
const FTextKey NOTIC_SPAWN_READY = TEXT("Notice_SpawnReady");
const FTextKey NOTIC_SPAWN = TEXT("Notice_Spawn");

void FGsWorldBossRankData::Init(PktIterator<PD::SC::PKT_SC_WORLD_BOSS_CLEAR_READ::Rankinglist> inData)
{
	_rank = inData->Rank();
	_userName = inData->CharName();
	_pvpGrade = inData->PvpAlignmentGrade();
	_contributePoint = inData->ContributePoint();
	_guildName = inData->GuildName();
	_guildDBId = inData->GuildDBId();
	_guildEmblemId = inData->GuildEmblemId();
	_basicRewardGold = inData->BasicRewardGold();
	_lastHitRewardIdList.Empty();

	_basicRewardIdList.Empty();
	for (PktIterator<PD::SC::PKT_SC_WORLD_BOSS_CLEAR_READ::Rankinglist::BasicRewardItemList> rewardIter = inData->GetFirstBasicRewardItemListIterator()
		; rewardIter != inData->GetLastBasicRewardItemListIterator()
		; ++rewardIter)
	{
		int32 count = 1;
		_basicRewardIdList.Add(FGsItemIconData(rewardIter->Item().mItemId, rewardIter->Item().mItemAmount));
	}

	_rewardIdList.Empty();
	for (PktIterator<PD::SC::PKT_SC_WORLD_BOSS_CLEAR_READ::Rankinglist::RewardItemList> rewardIter = inData->GetFirstRewardItemListIterator()
		; rewardIter != inData->GetLastRewardItemListIterator()
		; ++rewardIter)
	{
		_rewardIdList.Add(FGsItemIconData(rewardIter->Item().mItemId, rewardIter->Item().mItemAmount));
	}
}

void FGsWorldBossRankData::Init(const int32 inRank
	, const GuildDBId inDBId
	, const ContributePoint inContributionPoint
	, const PVPAlignmentGrade inPvpGrade
	, const FString& inUserName
	, const FString& inGuildName
	, const GuildEmblemId inEmblemId
	, const TArray<FGsItemIconData>& inItemArray)
{
	_rank = inRank;
	_guildDBId = inDBId;
	_pvpGrade = inPvpGrade;
	_userName = inUserName;
	_guildName = inGuildName;
	_guildEmblemId = inEmblemId;	
	_rewardIdList = inItemArray;
	_lastHitRewardIdList.Empty();
}

void FGsWorldBossRankData::Sort()
{
	_basicRewardIdList.Sort([](const FGsItemIconData& source, const FGsItemIconData& dest)->bool {
		const FGsSchemaItemCommon* sourceItem = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(source._itemId);
		const FGsSchemaItemCommon* destItem = UGsTableUtil::FindRowById<UGsTableItemCommon, FGsSchemaItemCommon>(dest._itemId);

		if (nullptr == sourceItem)
		{
			return false;
		}

		if (nullptr == destItem)
		{
			return false;
		}

		if (sourceItem->grade > destItem->grade)
		{
			return true;
		}
		else if (sourceItem->grade == destItem->grade)
		{
			return false;
		}

		return false;});
}

void FGsWorldBossRankData::Clear()
{
	_rank = 0;
	_contributePoint = 0;
	_pvpGrade = PVPAlignmentGrade::NETURAL;
	_userName = "";
	_guildName = "";
	_guildEmblemId = 0;
	_rewardIdList.Empty();
	_basicRewardIdList.Empty();
	_basicRewardGold = 0;
}

void FGsWorldBossHandler::Init()
{

}

void FGsWorldBossHandler::Close()
{

}

void FGsWorldBossHandler::WorldBossInfo(PD::SC::PKT_SC_ACK_WORLD_BOSS_INFO_READ* inPkt)
{
	WorldBossId worldBossId = inPkt->WorldBossId();	
	FDateTime bossRoomOpenTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(inPkt->MapOpenime());
	FDateTime bossSpawnTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(inPkt->BossSpawnTime());
	

	const FGsSchemaWorldBossData* worldBossData = UGsTableUtil::FindRowById<UGsTableWorldBossData, FGsSchemaWorldBossData>(worldBossId);
	if (nullptr == worldBossData)
	{
		return;
	}

	FGsUIMsgParamSelectBoss param(worldBossData, bossRoomOpenTime, bossSpawnTime, inPkt->DailyMaxRewardEarned());
	GMessage()->GetUI().SendMessage(MessageUI::INVALID_BOSS, &param);
}

void FGsWorldBossHandler::InformWorldMapCreate(PD::SC::PKT_SC_INFORM_WORLD_BOSS_MAP_CREATE_READ* inPkt)
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsStageManagerGame* stageMgr = gameFlow->GetStageManagerGame();
	if (nullptr == stageMgr)
	{
		return;
	}

	if (stageMgr->GetCurrentStageMode() == FGsStageMode::Game::WORLD_BOSS)
	{
		return;
	}

	WorldBossId bossId = inPkt->BossId();
	FDateTime bossSpawnTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(inPkt->ExpireTime());

	//월드 보스 룸 준비 완료 
	if (false == inPkt->EnteringWorld())
	{	// 2024/1/2 PKT - https://jira.com2us.com/jira/browse/C2URWQ-7888 최초 의도와 잘못 만들어진것으로 조건을 true에서 false로 수정
		//				  (feat.서버 차준규 차식님)
		FText spawnReadyText;
		FText::FindText(WORLD_BOSS_TEXT_KEY, NOTIC_SPAWN_READY, spawnReadyText);

		const FGsSchemaWorldBossData* worldBoss = FindWorldBossData(bossId);
		if (worldBoss)
		{
			FText message = FText::Format(spawnReadyText, worldBoss->nickName);
			FGsUIHelper::TrayMessageTicker(message);
			FGsUIHelper::AddChatMessageSystem(message);
		}
	}

	InvalidBossInfo(bossId, FGsTimeSyncUtil::GetClientNowDateTime(), bossSpawnTime);

	_worldBossRoomReadyArray.Emplace(bossId);	

	FGsWorldBossRoomReadyParam param = FGsWorldBossRoomReadyParam(bossId, bossSpawnTime);
	GMessage()->GetBoss().SendMessage(MessageBoss::INFORM_WORLD_BOSS_OPEN, &param);
}

void FGsWorldBossHandler::WorldBossSpawnReady(PD::SC::PKT_SC_WORLD_BOSS_SPAWN_READY_STATE_READ* inPkt)
{
	WorldBossId bossId = inPkt->WorldBossId();
	FDateTime endTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(inPkt->TimeInfo());

	FGsWorldBossRoomReadyParam param = FGsWorldBossRoomReadyParam(bossId, endTime);
	GMessage()->GetBoss().SendMessage(MessageBoss::WORLD_BOSS_SPAWN_READY, &param);
}

void FGsWorldBossHandler::AlarmWorldBossSpawn(PD::SC::PKT_SC_ALARM_WORLD_BOSS_SPAWN_READ* inPkt)
{
	WorldBossId bossId = inPkt->BossId();
	FDateTime endTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(inPkt->ExpireTime());

	FText spawnFormatText;
	FText::FindText(WORLD_BOSS_TEXT_KEY, NOTIC_SPAWN, spawnFormatText);

	//EnteringWorld 가 false면 정상 진입 true면 재접속이다
	//재접속 시에는 알림 처리 안하기
	if (false == inPkt->EnteringWorld())
	{
		//티커를 띄운다.
		const FGsSchemaWorldBossData* worldBoss = FindWorldBossData(bossId);
		if (worldBoss)
		{
			FText message = FText::Format(spawnFormatText, worldBoss->nickName);
			FGsUIHelper::TrayTickerBoss(EGsBossTickerStateType::Spawn, worldBoss->bossNpcId.GetRow()->id);
		}
	}

	if (_worldBossRoomReadyArray.Contains(bossId))
	{
		_worldBossRoomReadyArray.Remove(bossId);
	}

	FGsWorldBossSpawnParam param = FGsWorldBossSpawnParam(bossId, endTime);
	GMessage()->GetBoss().SendMessage(MessageBoss::ALARM_WORLD_BOSS_SPAWN, &param);

	//미니맵 옆 월드 보스 아이콘 숨기고 미니맵 하단 시간 띄움

#if WORLD_BOSS_DEBUG
	FDateTime nowDateTime = FGsTimeSyncUtil::GetClientNowDateTime();	
	UE_LOG(LogTemp, Log, TEXT("WorldBoss - end server utc time : %s\tend client tiem : %s\tnow client time : %s")
		, *endTime.ToString()
		, *FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(inPkt->ExpireTime()).ToString()
		, *nowDateTime.ToString());
#endif
	
	FGsUIMsgParamDungeonTime timeParam = FGsUIMsgParamDungeonTime(endTime, true, true);
	GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::HUD_TIME, &timeParam);
	
	GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::ON_HUD_TIME, nullptr);

	GUI()->SetHUDMode(EGsUIHUDMode::NORMAL);
}

void FGsWorldBossHandler::WorldBossClear(PD::SC::PKT_SC_WORLD_BOSS_CLEAR_READ* inPkt)
{
	WorldBossId bossId = inPkt->BossId();

	_worldBossRankDataArray.Empty();
	for (PktIterator<PD::SC::PKT_SC_WORLD_BOSS_CLEAR_READ::Rankinglist> rankIter = inPkt->GetFirstRankinglistIterator()
		; rankIter != inPkt->GetLastRankinglistIterator(); ++rankIter)
	{
		if (rankIter->IsAlive())
		{
			FGsWorldBossRankData data;
			data.Init(rankIter);
			_worldBossRankDataArray.Add(data);			
		}		
	}

	_lastHitRankData.Clear();
	FGsWorldBossRankData* lastHit = _worldBossRankDataArray.FindByPredicate([&](FGsWorldBossRankData& rankData)->bool {
		return rankData._userName == inPkt->LastHituserName();
		});
	if (lastHit)
	{
		for (PktIterator<PD::SC::PKT_SC_WORLD_BOSS_CLEAR_READ::LastHitRewardlist> iter = inPkt->GetFirstLastHitRewardlistIterator()
			; iter != inPkt->GetLastLastHitRewardlistIterator(); ++iter)
		{
			(*lastHit)._lastHitRewardIdList.Add(FGsItemIconData(iter->Item().mItemId, iter->Item().mItemAmount));
		}		

		_lastHitRankData = (*lastHit);
	}	
	else
	{
		_lastHitRankData.Clear();
	}

	FString userName = GGameData()->GetUserData()->mUserName;
	const FGsWorldBossRankData* userData = _worldBossRankDataArray.FindByPredicate([&](FGsWorldBossRankData& rankData)->bool {
		return rankData._userName == userName;
		});
	if (userData)
	{
		_playerRankData = (*userData);
		_playerRankData.Sort();
	}	
	else
	{	
		TArray<FGsItemIconData> itemArray;
		FGsNetUserData* playerUserData = const_cast<FGsNetUserData*>(GGameData()->GetUserData());
		_playerRankData.Init(0
			, playerUserData->mGuildData._guildDBId
			, 0
			, playerUserData->mLookInfo.mPVPAlignmentGrade
			, userName
			, playerUserData->mGuildData.GetGuildName()
			, playerUserData->mGuildData._emblemId
			, itemArray);
	}

	GUI()->OpenAndGetWidget(WORLD_BOSS_RESULT);

	FGsWorldBossParam param = FGsWorldBossParam(bossId);
	GMessage()->GetBoss().SendMessage(MessageBoss::WORLD_BOSS_CLEAR, &param);

	GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::OFF_HUD_TIME, nullptr);	
}

void FGsWorldBossHandler::AlarmWorldBossTimeOver(PD::SC::PKT_SC_ALARM_WORLD_BOSS_TIME_OVER_READ* inPkt)
{
	WorldBossId bossId = inPkt->BossId();

	GUI()->OpenAndGetWidget(WORLD_BOSS_RESULT);

	FGsWorldBossTimeOverParam param = FGsWorldBossTimeOverParam(bossId, inPkt->IsConsolationReward());
	GMessage()->GetBoss().SendMessage(MessageBoss::ALARM_WORLD_BOSS_TIME_OVER, &param);

	GMessage()->GetDungeonHUDTime().SendMessage(MessageContentDungeonHUDTime::OFF_HUD_TIME, nullptr);
}

void FGsWorldBossHandler::WorldBossNotRecommed(PD::SC::PKT_SC_WORLD_BOSS_NOT_RECOMMEND_READ* inPkt)
{
	WorldBossId bossId = inPkt->BossId();	

	FGsWorldBossParam param = FGsWorldBossParam(bossId);
	GMessage()->GetBoss().SendMessage(MessageBoss::WORLD_BOSS_NOT_RECOMMEND, &param);
}

void FGsWorldBossHandler::InvalidBossInfo(WorldBossId inBossId, const FDateTime& inBossRoomReadyTime, const FDateTime& inBossSpawnTime)
{
	if (_worldBossInfoMap.Contains(inBossId))
	{
		_worldBossInfoMap[inBossId]._mapOpenTime = inBossRoomReadyTime;
		_worldBossInfoMap[inBossId]._bossSpawnTime = inBossSpawnTime;
	}
	else
	{
		FGsWorldBossInfo worldBossInfo = FGsWorldBossInfo(inBossId, inBossRoomReadyTime, inBossSpawnTime);		
		_worldBossInfoMap.Emplace(inBossId, worldBossInfo);
	}

#if WORLD_BOSS_DEBUG
	if (inBossRoomReadyTime < inBossSpawnTime)
	{
		GSLOG(Log, TEXT("Invalid world boss - boss id : %lld\tboss room ready time : %s\tboss spawn time : %s")
			, (int64)inBossId
			, *inBossRoomReadyTime.ToString()
			, *inBossSpawnTime.ToString());
	}
	else
	{
		GSLOG(Log, TEXT("Invalid world boss - boss id : %lld\tboss spawn time : %s\tboss room ready : %s")
			, (int64)inBossId
			, *inBossSpawnTime.ToString()
			, *inBossRoomReadyTime.ToString());
	}
#endif
}

const FGsSchemaWorldBossData* FGsWorldBossHandler::FindWorldBossData(WorldBossId inId)
{
	const UGsTable* table = FGsSchemaWorldBossData::GetStaticTable();
	const UGsTableWorldBossData* bossTable = Cast<const UGsTableWorldBossData>(table);
	if (nullptr == bossTable)
	{
		GSLOG(Error, TEXT("boss table is missing"));
		return nullptr;
	}

	TArray<const FGsSchemaWorldBossData*> fieldBossArray;
	bossTable->GetAllRows(fieldBossArray);

	const FGsSchemaWorldBossData** find = fieldBossArray.FindByPredicate([&](const FGsSchemaWorldBossData* inData)->bool {
		return inId == inData->id;
		});

	return ((find) ? (*find) : nullptr);
}

bool FGsWorldBossHandler::TryGetWorldAllBossInfo(OUT TMap<WorldBossId, FGsWorldBossInfo>& outWorldBossMap)
{
	outWorldBossMap = _worldBossInfoMap;
	return true;
}

bool FGsWorldBossHandler::TryGetWorldBossInfo(WorldBossId inBossId, OUT FGsWorldBossInfo& outWorldBossInfo)
{
	if (_worldBossInfoMap.Contains(inBossId))
	{
		outWorldBossInfo = _worldBossInfoMap[inBossId];
		return true;
	}

	return false;
}

void FGsWorldBossHandler::WarpToWorldBoss(WorldBossId In_bossId)
{
	const FGsSchemaWorldBossData* bossInfo = FindWorldBossData(In_bossId);
	if (bossInfo == nullptr)
	{
		return;
	}
	
	if (nullptr == bossInfo
		|| nullptr == bossInfo->mapId.GetRow())
	{
		return;
	}	

	int32 mapId = bossInfo->mapId.GetRow()->id;
	CurrencyType currencyType = CurrencyType::GOLD;
	int useCost = 0;

	const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(mapId);
	if (mapData)
	{
		const FGsSchemaMapBalance* mapBalanceData = mapData->balanceId.GetRow();
		if (mapBalanceData)
		{
			currencyType = mapBalanceData->cost.costCurrencyType;
			useCost = mapBalanceData->cost.costCurrencyAmount;
		}
	}

	FText mapFormat;
	FText popupCoommonEnter2;
	FText::FindText(WORLD_BOSS_TEXT_KEY, POPUP_COMMON_ENTER1, mapFormat);
	FText::FindText(WORLD_BOSS_TEXT_KEY, POPUP_COMMON_ENTER2, popupCoommonEnter2);

	FText mapText = FText::Format(mapFormat, bossInfo->mapId.GetRow()->MapStringId);

	if (CurrencyType::NONE == currencyType
		|| 0 == useCost)
	{				
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"));
		if (widget.IsValid())
		{
			if (UGsUIPopupYesNoTwoMessage* popup = Cast<UGsUIPopupYesNoTwoMessage>(widget.Get()))
			{
				popup->SetData(mapText, popupCoommonEnter2
					, [mapId](bool inClick) {
						if (inClick)
						{
							GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, nullptr);
							FGsNetSendServiceBoss::SendReqWorldBossMapWarp(mapId);							
							GUI()->CloseAllStack();
						}
					});
			}
		}
	}
	else
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupCommonPay"));
		if (widget.IsValid())
		{
			if (UGsUIPopupCommonPay* popup = Cast<UGsUIPopupCommonPay>(widget.Get()))
			{
				TSharedRef<FGsCostPackage> CostPackage(FGsCostPackage::CreateSimple(currencyType, useCost));
				popup->SetData(mapText
					, MoveTemp(CostPackage)
					, [mapId](bool inClick) {
						if (inClick)
						{
							GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CONTENTS_WARP_REQUEST, nullptr);
							FGsNetSendServiceBoss::SendReqWorldBossMapWarp(mapId);
							GUI()->CloseAllStack();
						}
					});
				popup->SetUnderText(popupCoommonEnter2);
			}
		}
	}	
}

bool FGsWorldBossHandler::IsActiveWorldBoss(WorldBossId In_bossId)
{
	if (false == _worldBossInfoMap.Contains(In_bossId))
	{
		return false;
	}

	const FDateTime& currentTime = FGsTimeSyncUtil::GetClientNowDateTime();
	const FGsWorldBossInfo& bossInfo = _worldBossInfoMap[In_bossId];
	if (bossInfo._mapOpenTime <= currentTime
		&& bossInfo._bossSpawnTime > currentTime)
	{
#if WORLD_BOSS_DEBUG
		GSLOG(Log, TEXT("IsActiveWorldBoss - active - boss id : %lld\tboss room ready time : %s\tboss spawn time : %s\tcurrent time : %s")
			, (int64)In_bossId
			, *bossInfo._mapOpenTime.ToString()
			, *bossInfo._bossSpawnTime.ToString()
			, *currentTime.ToString());
#endif
		return true;
	}

#if WORLD_BOSS_DEBUG
	GSLOG(Log, TEXT("IsActiveWorldBoss - deactive - boss id : %lld\tboss room ready time : %s\tboss spawn time : %s\tcurrent time : %s")
		, (int64)In_bossId
		, *bossInfo._mapOpenTime.ToString()
		, *bossInfo._bossSpawnTime.ToString()
		, *currentTime.ToString());
#endif

	return false;
}
