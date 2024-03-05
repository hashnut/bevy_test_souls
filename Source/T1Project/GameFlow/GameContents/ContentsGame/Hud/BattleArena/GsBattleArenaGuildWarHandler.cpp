#include "GsBattleArenaGuildWarHandler.h"
#include "GsBattleArenaHelper.h"
#include "Unlock/GsUnlockDefine.h"
#include "../Chat/GsChatItem.h"
#include "../Level/GsLevelDefine.h"
#include "../Guild/GsGuildData.h"
#include "../Dungeon/GsDungeonData.h"
#include "../UTIL/GsTimeSyncUtil.h"
#include "../Item/GsItem.h"
#include "../Item/GsItemManager.h"
#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGame/GsChatManager.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGame/GsUnlockManager.h"
#include "../Management/ScopeGame/GsDungeonManager.h"
#include "../Management/ScopeGlobal/GsGuildManager.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"
#include "../Management/ScopeGame/GsGameObjectManager.h"
#include "../Management/ScopeGlobal/GsGameDataManager.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../Message/GsMessageSystem.h"
#include "../Message/GsMessageGameObject.h"
#include "../Message/GsMessageContentDungeon.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/MessageParam/GsStageMessageParam.h"
#include "../Management/ScopeGame/GsEventManager.h"
#include "../Message/MessageParam/GsBattleArenaMessageParam.h"
#include "../../GsContentsManagerGame.h"
#include "../../GsGameStateDungeon.h"
#include "../../../GsContentsMode.h"
#include "../../Dungeon/GsDungeonStateHandler.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "GameFramework/PlayerController.h"
#include "../GameObject/HUD/GsHUDLocalPlayer.h"
#include "../GameObject/Define/GsGameObjectDefine.h"
#include "../GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GsTable.h"
#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"
#include "DataSchema/BattleArena/GsSchemaGuildWarConfigData.h"
#include "DataSchema/Map/EventAction/GsSchemaMapEventActionImmediate.h"
#include "../UI/UILib/Base/GsUIWidgetBase.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIContent/HUD/GsUIHUDSideTab.h"
#include "../UI/UIContent/Helper/GsTimeStringHelper.h"
#include "../UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"
#include "../UI/UIContent/Popup/BattleArena/GsUIPopupBattleArenaGuildWarDetailScore.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Net/GsNetSendService.h"
#include "Net/GsNetSendServiceWorld.h"

#include "T1Project.h"


FGsBattleArenaGuildWarHandler::~FGsBattleArenaGuildWarHandler()
{
	Destory();
}

void FGsBattleArenaGuildWarHandler::Init()
{
	ResetData();

	BindMessage();

	// 2023/11/30 PKT - UI초기화를 위해 ..
	ContentsShotDown();
}

void FGsBattleArenaGuildWarHandler::Close()
{
	Destory();
}

void FGsBattleArenaGuildWarHandler::OnReconection()
{
	Destory();
}

void FGsBattleArenaGuildWarHandler::Update(float InDeltaTime)
{
	for (FGsSimpleTimer& item : _countdownTickerTimerSet)
	{
		item.Update(InDeltaTime);
	}
}

void FGsBattleArenaGuildWarHandler::Destory()
{
	ResetData();

	UnBindMessage();

	// 2023/11/30 PKT - UI초기화를 위해 ..
	ContentsShotDown();
}

void FGsBattleArenaGuildWarHandler::ResetData()
{
	_mapId = INVALID_MAP_ID;
	_state = EBattleArenaGuildWarState::ENone;
	_stateEndTime = 0;

	_entryMemberSet.Empty();

	_redTeamInfo.Reset();
	_blueTeamInfo.Reset();

	_endResultData.Reset();

	_countdownTickerTimerSet.Empty();

	// 2023/12/12 PKT - Arena 시작 시 카운트 꺼줌.
	FGsUIHelper::CloseTrayTickerCountEx();
}

void FGsBattleArenaGuildWarHandler::BindMessage()
{
	UnBindMessage();

	if (FGsMessageHolder* messageHolder = GMessage())
	{
		_systemMsg.Emplace(
			messageHolder->GetSystemParam().AddRaw(MessageSystem::LOAD_TOPO_START, this,
				&FGsBattleArenaGuildWarHandler::OnLoadTopoStart));

		/**
		 * GameObject
		 */
		_gameObjectMsg.Emplace(
			messageHolder->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
				&FGsBattleArenaGuildWarHandler::OnLocalPlayerSpawnComplete));

		/**
		 * Dungeon
		 */
		_listDungeonDelegate.Emplace(
			messageHolder->GetDungeon().AddRaw(
				MessageContentDungeon::CONTENTS_DUNGEON_STAGE_EXIT, this, &FGsBattleArenaGuildWarHandler::OnExitBattleArena));

		/**
		 * Item
		 */
		_listItemActionDelegates.Emplace(
			messageHolder->GetItem().AddRaw(
				MessageItem::ADDITEMLIST, this, &FGsBattleArenaGuildWarHandler::OnAddToInventory));
		// 2023/12/4 PKT - Update Item
		_listItemActionDelegates.Emplace(
			messageHolder->GetItem().AddRaw(
				MessageItem::UPDATEITEM, this, &FGsBattleArenaGuildWarHandler::OnChangeToInventory));
		// 2023/12/4 PKT - Delete Item
		_listItemActionDelegates.Emplace(
			messageHolder->GetItem().AddRaw(
				MessageItem::REMOVEITEM, this, &FGsBattleArenaGuildWarHandler::OnRemoveToInventory));

		/**
		 * Unlock Contents
		 */
		_unlockDelegateList.Emplace(
			messageHolder->GetUnlock().AddRaw(MessageContentUnlock::INVALIDATE_ALL_UNLOCK, this,
				&FGsBattleArenaGuildWarHandler::OnContetnsUnlock));

		/**
		 * Guild
		 */
		// 2023/12/22 PKT - 길드 탈퇴
		_guildMsgHandler.Emplace(
			messageHolder->GetGuild().AddRaw(MessageContentGuild::RECEIVE_LEAVE, this,
				&FGsBattleArenaGuildWarHandler::OnLeaveGuild));
		
		// 2023/12/22 PKT - 길드 해산
		_guildMsgHandler.Emplace(
			messageHolder->GetGuild().AddRaw(MessageContentGuild::RECEIVE_DISBAND, this,
				&FGsBattleArenaGuildWarHandler::OnLeaveGuild));
		
	}	
}

void FGsBattleArenaGuildWarHandler::UnBindMessage()
{
	if (FGsMessageHolder* messageHolder = GMessage())
	{
		for (auto& item : _systemMsg)
		{
			messageHolder->GetSystemParam().Remove(item);
		}
		_systemMsg.Empty();

		for (auto& item : _gameObjectMsg)
		{
			messageHolder->GetGameObject().Remove(item);
		}
		_gameObjectMsg.Empty();

		for (auto& item : _listDungeonDelegate)
		{
			messageHolder->GetDungeon().Remove(item);
		}
		_listDungeonDelegate.Empty();

		for (auto& item : _listItemActionDelegates)
		{
			messageHolder->GetItem().Remove(item);
		}
		_listItemActionDelegates.Empty();

		for (auto& item : _unlockDelegateList)
		{
			messageHolder->GetUnlock().Remove(item);
		}
		_unlockDelegateList.Empty();

		for (auto& item : _guildMsgHandler)
		{
			messageHolder->GetGuild().Remove(item);
		}
		_guildMsgHandler.Empty();
	}
}

void FGsBattleArenaGuildWarHandler::ChangedState(EBattleArenaGuildWarState InNextState)
{
	EBattleArenaGuildWarState prevState = _state;
	_state = InNextState;
	
	if (EBattleArenaGuildWarState::ENone == _state)
	{
		ResetData();
	}

	/**
	 * 2024/1/15 PKT - 타이머 알림 설정
	 */
	SetCountDownTicker(_state);
	
	SendWindowMessage(prevState, _state);
}

void FGsBattleArenaGuildWarHandler::SendWindowMessage(EBattleArenaGuildWarState InPrevState, EBattleArenaGuildWarState InCurrentState) const
{
	if (FGsMessageHolder* messageHolder = GMessage())
	{
		if (InPrevState != InCurrentState)
		{	
			if (EBattleArenaGuildWarState::ERegisterCompleted == InPrevState && EBattleArenaGuildWarState::ERegisterPeriod == InCurrentState)
			{
				// 2024/1/29 PKT - "그림자 전장 참여 신청이 취소되었습니다.	"			
				FText message;
				FGsBattleArenaHelper::FindText(TEXT("Ticker_GuildArena_ApplyJoin_Cancel"), message);
				// 2024/1/29 PKT - 티커
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
				// 2024/1/29 PKT - 채팅
				FGsUIHelper::AddChatMessageSystem(message);
			}
			else if (EBattleArenaGuildWarState::ERegisterPeriod == InPrevState && EBattleArenaGuildWarState::ERegisterCompleted == InCurrentState)
			{
				// 2024/1/29 PKT - "그림자 전장 참여 신청이 완료되었습니다."
				FText message;
				FGsBattleArenaHelper::FindText(TEXT("Ticker_GuildArena_ApplyJoin_Complete"), message);
				// 2024/1/29 PKT - 티커
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
				// 2024/1/29 PKT - 채팅
				FGsUIHelper::AddChatMessageSystem(message);
			}
			else if (EBattleArenaGuildWarState::ERegisterCompleted == InPrevState && EBattleArenaGuildWarState::EMatchingWait == InCurrentState)
			{
				// 2023/12/6 PKT - 입장 대기 시 사운드
				FGsBattleArenaHelper::PlayEffectSound(TEXT("UI_KnightArena_Open"));				
				// 2024/1/29 PKT - "그림자 전장 입장 시간이 되어 그림자 관문으로 자동 이동합니다."
				FText message;
				FGsBattleArenaHelper::FindText(TEXT("Ticker_GuildArena_LobbyOn"), message);
				// 2024/1/29 PKT - 티커
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
				// 2024/1/29 PKT - 채팅
				FGsUIHelper::AddChatMessageSystem(message);
			} 
			else if (EBattleArenaGuildWarState::EStandByStartGame == InCurrentState)
			{
				// 2023/12/22 PKT - Count Ticker UI 초기화 한번 해줌.(사실 안해줘도 되지만..)
				FGsUIHelper::CloseTrayTickerCountEx();

				ReservedShowTickerStartCount();
			}
			else if (EBattleArenaGuildWarState::EGamePlaying == InCurrentState)
			{	
				if (EBattleArenaGuildWarState::EStandByStartGame == InPrevState)
				{
					// 2023/12/6 PKT - 경기 시작 알리는 사운드
					FGsBattleArenaHelper::PlayEffectSound(TEXT("UI_KnightArena_Start"));
				}
				// 2023/11/22 PKT - 정보 갱신
				FGsBattleArenaStateInfo message(_mapId, EBattleArenaGuildWarState::EGameBegin, _stateEndTime, _redTeamInfo._score, _blueTeamInfo._score);
				messageHolder->GetBattleArena().SendMessage(MessageContentBattleArena::UPDATE_STATE_INFO, &message);
			}
		}
		// 2023/11/22 PKT - 정보 갱신
		FGsBattleArenaStateInfo message(_mapId, InCurrentState, _stateEndTime, _redTeamInfo._score, _blueTeamInfo._score);
		messageHolder->GetBattleArena().SendMessage(MessageContentBattleArena::UPDATE_STATE_INFO, &message);
	}
}

EBattleArenaGuildWarState FGsBattleArenaGuildWarHandler::ConvertServerStateToClientState(GuildWarState InGuildWarState, bool InIsRegister /*= false*/) const
{	// 2023/11/27 PKT - 서버 타입을 클라 타입으로 변환
	if (true == IsHiddenArena())
	{
		return EBattleArenaGuildWarState::ENone;
	}
	
	EBattleArenaGuildWarState outState = EBattleArenaGuildWarState::ENone;

	switch (InGuildWarState)
	{
	case GuildWarState::NONE:
		// 2023/11/27 PKT - 아무것도 아닌 상태( 신청 비활성화 )
		outState = EBattleArenaGuildWarState::ENone;
		break;
	case GuildWarState::REGISTER:
		// 2023/11/27 PKT - 신청 가능 및 신청 후 진행 중
		outState = (false == InIsRegister) ? EBattleArenaGuildWarState::ERegisterPeriod : EBattleArenaGuildWarState::ERegisterCompleted;
		break;
	case GuildWarState::READY:
		// 2023/11/27 PKT - 신청 완료(매칭 대기 상태)
		outState = EBattleArenaGuildWarState::EMatchingWait;
		break;
	case GuildWarState::PLAYWAIT:
	{	// 2023/12/5 PKT - 입장이 가능하나 입장은 하지 않은 상태
		outState = EBattleArenaGuildWarState::EAdmissionGameAvailable;

		if (const FGsSchemaMapData* currentMapData = GLevel()->GetCurrentMapInfo())
		{
			if (currentMapData->id == _mapId)
			{	// 2023/12/5 PKT - 로비로 입장하여 아직 Arena는 시작하지 않은 전단계
				outState = EBattleArenaGuildWarState::EStandByStartGame;
			}
		}
	}	break;
	case GuildWarState::PLAY:
		// 2023/11/27 PKT - Baltte Arena Guild War Contetns 진행
		outState = EBattleArenaGuildWarState::EGamePlaying;
		if (InIsRegister && MapType::GUILD_WAR != GLevel()->GetCurrentMapType())
		{	// 2023/12/12 PKT - 전장에 입장해 있는 상태가 아니라면 입장 할 수 있도록 한다.
			outState = EBattleArenaGuildWarState::EAdmissionGameAvailable;
		}
		break;
	case GuildWarState::END:
		// 2023/11/27 PKT - Baltte Arena Guild War Contetns 끝
		outState = EBattleArenaGuildWarState::EGameDone;
		break;
	case GuildWarState::DESTORY:
		// 2023/11/27 PKT - 서버에서 해당 Guild War Instance를 파괴 할때 보냄. 클라 처리 필요 없음.
		break;
	}
	return outState;
}

void FGsBattleArenaGuildWarHandler::ArenaStartAlarm(MapId InMapId)
{
	// 2023/12/14 PKT - Battle Arena Guild War 시작 알림.
	if (const FGsSchemaGuildWarConfigData* schemaConfigData = GetConfigData(InMapId))
	{
		if (UGsEventManager* eventManager = GSEvent())
		{	// 2023/12/15 PKT - 침공전 시작 알림과 유사한 티커 출력
			if (const FGsSchemaMapEventActionImmediate* ticker = schemaConfigData->openEventTickerId.GetRow())
			{
				eventManager->EventActionPlayImmediate(ticker->id);
			}

			// 2023/12/15 PKT - 침공전 시작 알림 채팅 출력
			if (const FGsSchemaMapEventActionImmediate* chat = schemaConfigData->openEventChatTickerId.GetRow())
			{
				eventManager->EventActionPlayImmediate(chat->id);
			}
		}
	}
}

void FGsBattleArenaGuildWarHandler::UpdateState(GuildWarState InState, MapId InMapId, const TArray<FGsBattleArenaEntryInfo>& InEntrySet, int32 maxCapacity, time_t InEndTime)
{
	_mapId = InMapId;
	_stateEndTime = (0 >= InEndTime) ? 0 : InEndTime;
	_entryMemberSet = InEntrySet;

	const UserDBId myUserDBId = GGameData()->GetUserData()->mUserDBId;
	int32 findIndex = InEntrySet.IndexOfByPredicate([myUserDBId](const FGsBattleArenaEntryInfo& InData)
		{
			return InData._userdbId == myUserDBId;
		}
	);

	EBattleArenaGuildWarState nextState = ConvertServerStateToClientState(InState, INDEX_NONE < findIndex);

	if (INDEX_NONE >= findIndex)
	{	// 2023/12/6 PKT - 신청하지 않은 상태에서 매칭 대기중이거나 아레나에 입장 할 수 있는 상태라면
		if (EBattleArenaGuildWarState::EMatchingWait == nextState || EBattleArenaGuildWarState::EAdmissionGameAvailable == nextState)
		{	// 2023/12/12 PKT - 잘못된 정보이므로 모든 Data 초기화
			nextState = EBattleArenaGuildWarState::ENone;
		}
	}

	ChangedState(nextState);

	if (EBattleArenaGuildWarState::ENone != nextState && GuildWarState::REGISTER == InState )
	{	
		if (0 < InEntrySet.Num())
		{	// 2023/12/4 PKT - "기사단 참여 신청 현황이 갱신되었습니다. ({0}/{1}) "
			if (const FGsGuildDataMine* guildInfo = GGuild()->GetMyGuildData())
			{
				FText textFormat;
				FGsBattleArenaHelper::FindText(TEXT("Ticker_GuildArena_ApplyJoin_Refresh"), textFormat);

				FText message = FText::Format(textFormat, { InEntrySet.Num(), maxCapacity });

				FGsUIHelper::AddChatMessageSystem(message);
			}			
		}
	}
	else if (EBattleArenaGuildWarState::EGameDone == _state && GuildWarState::END == InState)
	{	// 2023/12/22 PKT - 결과 정보와 상태 정보는 별도로 오는데 이게 순서를 보장할 수 없음. 그렇기 때문에 상태가 바뀌었고 결과 정보를 받았을 경우에 화면에 출력한다.
		if (_endResultData._isSaved)
		{
			OpenWindowPlayResult();
		}		
	}
}

void FGsBattleArenaGuildWarHandler::UpdateState(GuildWarState InState, const FGsBattleArenaGuildWarTeamInfo& InRedTeam, const FGsBattleArenaGuildWarTeamInfo& InBlueTeam, time_t InEndTime)
{
	if (MapType::GUILD_WAR != GLevel()->GetCurrentMapType())
	{	// 2023/12/12 PKT - 전장에 입장해 있는 상태가 아니라면 모든걸 무시..
		ContentsShotDown();
		return;
	}

	auto SetAsGuildInfoAndPoint = [](const FGsBattleArenaGuildWarTeamInfo& InSrc, FGsBattleArenaGuildWarTeamInfo& Indest)
	{		
		Indest._guildDBId		= InSrc._guildDBId;
		Indest._guildName		= InSrc._guildName;
		Indest._guildEmblemId	= InSrc._guildEmblemId;
		Indest._worldId			= InSrc._worldId;
		Indest._score			= InSrc._score;
	};

	SetAsGuildInfoAndPoint(InRedTeam, _redTeamInfo);
	SetAsGuildInfoAndPoint(InBlueTeam, _blueTeamInfo);
	// 2023/12/15 PKT - EndTime
	_stateEndTime = (0 >= InEndTime) ? 0 : InEndTime;

	EBattleArenaGuildWarState nextState = ConvertServerStateToClientState(InState);

	ChangedState(nextState);

	if (EBattleArenaGuildWarState::EGameDone == _state && GuildWarState::END == InState)
	{	// 2023/12/22 PKT - 결과 정보와 상태 정보는 별도로 오는데 이게 순서를 보장할 수 없음. 그렇기 때문에 상태가 바뀌었고 결과 정보를 받았을 경우에 화면에 출력한다.
		if (_endResultData._isSaved)
		{
			OpenWindowPlayResult();
		}
	}
}

void FGsBattleArenaGuildWarHandler::UpdateState(bool InRegister)
{
	EBattleArenaGuildWarState prevState = _state;
	EBattleArenaGuildWarState nextState = ConvertServerStateToClientState(GuildWarState::REGISTER, InRegister);
	ChangedState(nextState);
}

void FGsBattleArenaGuildWarHandler::ContentsShotDown()
{
	ChangedState(EBattleArenaGuildWarState::ENone);
}

void FGsBattleArenaGuildWarHandler::OnLoadTopoStart(const IGsMessageParam* inParam)
{
	if (nullptr == GLevel())
	{
		GSLOG(Error, TEXT("nullptr == GLevel()"));
		return;
	}

	if (MapType::GUILD_WAR != GLevel()->GetCurrentMapType())
	{
		return;
	}

	EGsLoadTopoEffectType loadTopoType = GLevel()->GetLoadTopoEffectType();

	if (EGsLoadTopoEffectType::DIFF_MAP != loadTopoType)
	{
		return;
	}	

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		GSLOG(Error, TEXT("nullptr == contents"));
		return;
	}

	FGsGameStateDungeon* stateDungeon = contents->GetContentsStateObject<FGsGameStateDungeon>(FGsContentsMode::InGame::ContentsDungeon);
	if (nullptr == stateDungeon)
	{
		GSLOG(Error, TEXT("nullptr == stateDungeon"));
		return;
	}

	const FGsDungeonStateHandler* dungeonStateHandler = stateDungeon->StateHandler();
	if (nullptr == dungeonStateHandler || nullptr == dungeonStateHandler->CurrentState())
	{
		return;
	}

	if (DungeonCategory::ARENA != dungeonStateHandler->CurrentState()->GetCategory())
	{
		return;
	}

	ResetData();

	ChangedState(EBattleArenaGuildWarState::ENone);

	/*if (GMessage())
	{
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::EXIT, nullptr);
	}*/
}

void FGsBattleArenaGuildWarHandler::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	SendWindowMessage(_state, _state);

	SetCountDownTicker(_state);
}

void FGsBattleArenaGuildWarHandler::OnContetnsUnlock(const IGsMessageParam* InParm)
{
	if (true == IsHiddenArena())
	{
		if (EBattleArenaGuildWarState::ENone != _state)
		{
			ContentsShotDown();
		}
	}
}

void FGsBattleArenaGuildWarHandler::OnExitBattleArena(const struct IGsMessageParam* InParam)
{	
	if (MapType::GUILD_WAR != GLevel()->GetCurrentMapType())
	{
		return;
	}

	ContentsShotDown();
}

void FGsBattleArenaGuildWarHandler::OnAddToInventory(struct FGsItemMessageParamBase& InParam)
{
	FGsAddItemList* param = static_cast<FGsAddItemList*>(&InParam);
	if (nullptr == param)
	{
		return;
	}

	//WINDOW_MESSAGE_LOCK	
	TWeakObjectPtr<UGsItemManager> itemManager = GItem();
	for (int32 idx = 0; idx < param->_itemDbidList.Num(); ++idx)
	{
		TWeakPtr<FGsItem> item = itemManager->FindItem(param->_itemDbidList[idx]);
		if (false == item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == findData, Param->_itemDBID : %lld"), param->_itemDbidList[idx]);
			return;
		}

		if (true == CheckedCoinItem(item.Pin().Get()->GetTID()))
		{
			break;
		}
	}
}

void FGsBattleArenaGuildWarHandler::OnChangeToInventory(struct FGsItemMessageParamBase& InParam)
{
	const FGsUpdateItem* Param = StaticCast<const FGsUpdateItem*>(&InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	TWeakObjectPtr<UGsItemManager> itemManager = GItem();
	if (itemManager.IsValid())
	{
		TWeakPtr<FGsItem> item = itemManager->FindItem(Param->_itemDBID);
		if (false == item.IsValid())
		{
			GSLOG(Error, TEXT("nullptr == findData, Param->_itemDBID : %lld"), Param->_itemDBID);
			return;
		}

		CheckedCoinItem(item.Pin().Get()->GetTID());
	}
}

void FGsBattleArenaGuildWarHandler::OnRemoveToInventory(struct FGsItemMessageParamBase& InParam)
{
	const FGsRemoveItem* Param = StaticCast<const FGsRemoveItem*>(&InParam);
	if (nullptr == Param)
	{
		GSLOG(Error, TEXT("nullptr == Param"));
		return;
	}

	CheckedCoinItem(Param->_itemTID);
}

void FGsBattleArenaGuildWarHandler::OnLeaveGuild(const IGsMessageParam* inMessage)
{
	// 2023/12/22 PKT - 길드를 떠났거나 추방당했거나 길드 해산 할 경우
	if (EBattleArenaGuildWarState::ENone != _state)
	{	
		ContentsShotDown();
	}
}

bool FGsBattleArenaGuildWarHandler::CheckedCoinItem(ItemId InItemId)
{
	bool outResult = false;
	do 	
	{
		const FGsSchemaMapData* currentMapData = GLevel()->GetCurrentMapInfo();
		if (nullptr == currentMapData)
		{
			break;
		}

		if (MapType::GUILD_WAR != currentMapData->mapType)
		{
			break;
		}

		if (currentMapData->id != _mapId)
		{
			break;
		}

		const FGsSchemaGuildWarConfigData* configData = GetConfigData(_mapId);
		if (nullptr == configData)
		{
			break;
		}

		const FGsSchemaItemCommon* itemCommonData = configData->coinId.GetRow();
		if (nullptr == itemCommonData)
		{
			break;
		}

		if (InItemId != itemCommonData->id)
		{
			break;
		}
		
		outResult = true;

	} while (0);
	
	if (outResult)
	{
		SendWindowMessage(_state, _state);
	}

	return outResult;
}


FGsBattleArenaGuildWarHandler::EResult FGsBattleArenaGuildWarHandler::CheckedRegisterCondition(bool InIsMessage /* = false */) const
{
	EResult outResult = EResult::Vaild;

	do
	{
		if (EBattleArenaGuildWarState::ERegisterCompleted == _state)
		{
			outResult = EResult::InvalidAlreadyResiger;
			break;
		}

		// 2023/11/23 PKT - Map & 던전 검사
		const FGsDungeonData* dungeonData = GSDungeon()->FindDungeonDataByMapId(_mapId);
		if (nullptr == dungeonData)
		{
			outResult = EResult::InvalidMapData;
			break;
		}

		// 2023/11/23 PKT - UnLock 검사
		if (false == dungeonData->IsUnLockState())
		{
			outResult = EResult::InvalidUnlockContents;
			break;
		}

		// 2023/11/23 PKT - 길드 데이터 검사
		if (false == GGuild()->IsGuild() || nullptr == GGuild()->GetMyGuildData())
		{
			outResult = EResult::InvalidNotFindGuildInfo;
			break;
		}

		const FGsSchemaGuildWarConfigData* schemaConfigData = GetConfigData(_mapId);
		if (nullptr == schemaConfigData)
		{
			outResult = EResult::InvalidUnKnown;
			break;
		}

		// 2023/11/23 PKT - 길드 레벨 검사
		if (GGuild()->GetGuildLevel() < schemaConfigData->limitGuildLevel)
		{
			outResult = EResult::InvalidGuildLevel;
			break;
		}

		// 2023/11/23 PKT - 본인의 길드 권환 검사
		if (StaticCast<int32>(schemaConfigData->limitMinApplyGrade) > StaticCast<int32>(GGuild()->GetMyGuildData()->_memberGrade))
		{
			outResult = EResult::InvalidGuildMemberGrade;
			break;
		}

		outResult = EResult::Vaild;

	} while (0);

	if (InIsMessage)
	{
		ShowTickerMessage(outResult);
	}

	return outResult;
}

void FGsBattleArenaGuildWarHandler::ShowTickerMessage(EResult InResult) const
{
	if (EResult::Vaild == InResult)
	{
		return;
	}

	FText tickerMessage;
	switch (InResult)
	{
	case EResult::InvalidGuildLevel:
		FGsBattleArenaHelper::FindText(TEXT("Ticker_ApplyJoin_NotGuildLevel"), tickerMessage);
		break;
	case EResult::InvalidGuildMemberGrade:
		FGsBattleArenaHelper::FindText(TEXT("Ticker_ApplyJoin_NotGuildGrade"), tickerMessage);
		break;
	case EResult::InvalidAlreadyResiger:
		FGsBattleArenaHelper::FindText(TEXT("Ticker_ApplyJoin_Already"), tickerMessage);
		break;
	case EResult::InvalidateEntranceWait:
		FGsBattleArenaHelper::FindText(TEXT("Ticker_GuildArena_Entrance_Wait"), tickerMessage);
		break;
	case EResult::InvalidUnlockContents:
		FGsBattleArenaHelper::FindText(TEXT("Ticker_ApplyJoin_NotLevel"), tickerMessage);
		break;
	default:
#if UE_BUILD_SHIPPING
		tickerMessage = FText::GetEmpty();
#else	//#if UE_BUILD_SHIPPING
		tickerMessage = FText::FromString(FString::Format(TEXT("Error Number {0}"), { StaticCast<int32>(InResult) }));		
#endif	//#if UE_BUILD_SHIPPING
		return;
	}

	if (false == tickerMessage.IsEmpty())
	{
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, tickerMessage);
	}
}

bool FGsBattleArenaGuildWarHandler::IsHiddenArena() const
{
	return (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::ARENA_DUNGEON_HIDDEN));
}

void FGsBattleArenaGuildWarHandler::SetCountDownTicker(EBattleArenaGuildWarState InState)
{
	_countdownTickerTimerSet.Empty();

	if (EBattleArenaGuildWarState::EMatchingWait != InState && EBattleArenaGuildWarState::EGamePlaying != InState)
	{
		return;
	}

	const time_t stateEndtime = _stateEndTime;
	auto SetTimer = [InState, stateEndtime](const TArray<int32>& InCountDownSet, TArray<FGsSimpleTimer>& OutTimerSet)
	{
		OutTimerSet.Empty();
		
		const time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();

		for (int32 item : InCountDownSet)
		{
			time_t countTime = (stateEndtime > item) ? stateEndtime - item : 0;
			if (currentTime >= countTime)
			{	// 2024/1/2 PKT - 이미 알림 시간을 지났다면 넣어 주지 않는다.
				continue;
			}

			FGsSimpleTimer timer;
			timer.SetInterval(countTime - currentTime);
			timer.Router.AddLambda([InState, item]()
				{
					FText format = FText::GetEmpty();
					if (EBattleArenaGuildWarState::EMatchingWait == InState)
					{
						FGsBattleArenaHelper::FindText(TEXT("Ticker_Entrance_RemainTime"), format);
					}
					else if (EBattleArenaGuildWarState::EGamePlaying == InState)
					{
						FGsBattleArenaHelper::FindText(TEXT("Ticker_Playend_RemainTime"), format);
					}					

					FString strTime;
					FGsTimeStringHelper::GetTimeStringFloor(FTimespan(item * ETimespan::TicksPerSecond), strTime);

					FText message = FText::Format(format, FText::FromString(strTime));
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);

					FGsUIHelper::AddChatMessageSystem(message);
				}
			);
			timer.Start();

			OutTimerSet.Emplace(timer);
		}
	};

	const FGsSchemaGuildWarConfigData* configData = GetConfigData(_mapId);
	if (nullptr == configData)
	{
		return;
	}

	// 2024/2/16 PKT - 기획 변경으로 현재 아래와 같은 알림은 그림자 전장 내에서만 사용
	if (EBattleArenaGuildWarState::EMatchingWait == _state && MapType::GUILD_WAR == GLevel()->GetCurrentMapType())
	{
		SetTimer(configData->entranceCountdownSet, _countdownTickerTimerSet);
	}
	else if (EBattleArenaGuildWarState::EGamePlaying == _state && MapType::GUILD_WAR == GLevel()->GetCurrentMapType())
	{
		SetTimer(configData->endPlayCountdownSet, _countdownTickerTimerSet);
	}
	else
	{
		_countdownTickerTimerSet.Empty();
	}
}

FGsBattleArenaGuildWarHandler::EResult FGsBattleArenaGuildWarHandler::CheckedEnterCondition(bool InIsMessage /* = false */) const
{
	EResult outResult = EResult::Vaild;

	do
	{
		if (EBattleArenaGuildWarState::EMatchingWait == _state)
		{
			outResult = EResult::InvalidateEntranceWait;
			break;
		}

		if (EBattleArenaGuildWarState::EAdmissionGameAvailable != _state)
		{
			outResult = EResult::InvalidEnterState;
			break;
		}

		const FGsDungeonData* dungeonData = GSDungeon()->FindDungeonDataByMapId(_mapId);
		if (nullptr == dungeonData)
		{
			outResult = EResult::InvalidMapData;
			break;
		}

		if (false == dungeonData->IsUnLockState())
		{
			outResult = EResult::InvalidUnlockContents;
			break;
		}

		outResult = EResult::Vaild;

	} while (0);

	if (InIsMessage)
	{
		ShowTickerMessage(outResult);
	}

	return outResult;
}

FText FGsBattleArenaGuildWarHandler::TextStateRemainingTime() const
{
	const time_t currentTime = FGsTimeSyncUtil::GetServerNowSecond();
	time_t remainingTime = (currentTime < _stateEndTime) ? (_stateEndTime - currentTime) : 0;

	FString strTime;
	FGsTimeStringHelper::GetTimeStringHMS(FTimespan(remainingTime * ETimespan::TicksPerSecond), strTime);

	return FText::FromString(strTime);
}

ItemAmount FGsBattleArenaGuildWarHandler::GetCoinAmount() const
{
	ItemAmount outAmount = 0;

	do
	{	// 2023/12/5 PKT - 내가 있는 곳이 Arena가 맞는가?
		const FGsSchemaMapData* schemaMapData = GLevel()->GetCurrentMapInfo();
		if (nullptr == schemaMapData || schemaMapData->id != _mapId)
		{
			break;
		}

		if (EBattleArenaGuildWarState::EStandByStartGame == _state)
		{
			break;
		}

		const FGsSchemaGuildWarConfigData* schemaConfigData = GetConfigData(_mapId);
		if (nullptr == schemaConfigData || nullptr == schemaConfigData->coinId.GetRow())
		{
			break;
		}

		TArray<TWeakPtr<FGsItem>> findItemSet = GItem()->FindByTID(schemaConfigData->coinId.GetRow()->id, ItemType::CURRENCY);
		for (auto item : findItemSet)
		{
			outAmount += item.Pin().Get()->GetAmount();
		}

	} while (0);

	return outAmount;
}

void FGsBattleArenaGuildWarHandler::DetailScore(const FGsBattleArenaGuildWarTeamInfo& InRedTeam, const FGsBattleArenaGuildWarTeamInfo& InBlueTeam)
{
	TWeakObjectPtr<UGsUIPopupBattleArenaGuildWarDetailScore> widget 
		= Cast<UGsUIPopupBattleArenaGuildWarDetailScore>(GUI()->GetWidgetByKey(TEXT("BattleArenaGuildWarDetailScore")));

	if (widget.IsValid())
	{	// 2023/11/24 PKT - 이미 열려 있는 상태라면 갱신을..
		widget->Invalidate(InRedTeam, InBlueTeam);
	}
	else
	{	// 2023/11/24 PKT - 열려 있지 않다면 새로 셋팅..
		widget.Reset();

		widget = Cast<UGsUIPopupBattleArenaGuildWarDetailScore>(GUI()->OpenAndGetWidget(TEXT("BattleArenaGuildWarDetailScore")));
		if (widget.IsValid())
		{
			widget->SetData(_state, InRedTeam, InBlueTeam);
		}
	}
}

void FGsBattleArenaGuildWarHandler::PlayEndResult(GuildWarEndReason InReason, GuildDBId InWinGuildId, int32 InMyPoint, const FGsBattleArenaGuildWarTeamInfo& InRedTeam, const FGsBattleArenaGuildWarTeamInfo& InBlueTeam)
{
	_endResultData.Reset();
	_redTeamInfo.Reset();
	_blueTeamInfo.Reset();

	_endResultData._isSaved = true;
	_endResultData._winGuildDBId = InWinGuildId;
	_endResultData._endReason = InReason;
	_endResultData._myPointScore = InMyPoint;

	_redTeamInfo = InRedTeam;
	_blueTeamInfo = InBlueTeam;

	if (EBattleArenaGuildWarState::EGameDone == _state)
	{	// 2023/12/22 PKT - 결과 정보와 상태 정보는 별도로 오는데 이게 순서를 보장할 수 없음. 그렇기 때문에 상태가 바뀌었고 결과 정보를 받았을 경우에 화면에 출력한다.
		OpenWindowPlayResult();
	}	

	if (GuildWarEndReason::TEAM_MEMBER_MINIMUM_CUT == _endResultData._endReason)
	{
		// 2024/2/20 PKT - 진행에 필요한 최소 인원이 부족하여 종료되었습니다.
		FText message;
		FGsBattleArenaHelper::FindText(TEXT("Popup_Opposing_NoMinimumMember"), message);
		// 2024/1/29 PKT - 채팅
		FGsUIHelper::AddChatMessageSystem(message);
	}
}

void FGsBattleArenaGuildWarHandler::MemberPositionInfo(const FGsBattleArenaMemberPositionInfos& InRedTeam, const FGsBattleArenaMemberPositionInfos& InBlueTeam)
{
	if (MapType::GUILD_WAR != GLevel()->GetCurrentMapType())
	{	// 2023/12/12 PKT - 전장에 입장해 있는 상태가 아니라면 모든걸 무시..
		return;
	}

	/*
	* 해당 내용은 개선 사항 반영때 넣기로 했음.
	*/

	if (UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		
	}
}

void FGsBattleArenaGuildWarHandler::TryToRegisterOrCancel()
{
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::BATTLE_ARENA_GUILD_WAR_REGISTER, true))
	{
		return;
	}

	if (EBattleArenaGuildWarState::ERegisterPeriod == _state)
	{
		if (EResult::Vaild != CheckedRegisterCondition(true))
		{	// 2023/11/23 PKT - 조건 미달
			return;
		}

		const FGsDungeonData* findDungeonData = GSDungeon()->FindDungeonDataByMapId(_mapId);
		const FGsSchemaDungeonData* schemaDungeonData = (findDungeonData) ? findDungeonData->GetSchema() : nullptr;
		const FGsSchemaMapData* schemaMapData = (schemaDungeonData) ? schemaDungeonData->mapId.GetRow() : nullptr;
		if (nullptr == schemaMapData)
		{
			return;
		}

		if (UGsUIPopupYesNoTwoMessage* popup = Cast<UGsUIPopupYesNoTwoMessage>(GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"))))
		{
			// 2023/12/7 PKT - "{0}참여 신청 하시겠습니까?"
			FText format;
			FGsBattleArenaHelper::FindText(TEXT("Popup_GuildArena_ApplyJoin"), format);

			FText message1 = FText::Format(format, schemaMapData->MapStringId);

			// 2023/12/7 PKT - "사전에 참여 신청을 완료한 유저만 이후 그림자 전장에 입장이 가능합니다."
			FText message2;
			FGsBattleArenaHelper::FindText(TEXT("Popup_GuildArena_ApplyJoin_Notice"), message2);

			MapId mapId = _mapId;
			popup->SetData(message1, message2, [mapId](bool InIsOk)
				{
					if (false == InIsOk)
					{
						return;
					}
					FGsNetSendService::SendReqBattleArenaApply(mapId, true);
				}
			);
		}
	}
	else if (EBattleArenaGuildWarState::ERegisterCompleted == _state)
	{
		MapId mapId = _mapId;
		FGsNetSendService::SendReqBattleArenaApply(mapId, false);
	}
}

void FGsBattleArenaGuildWarHandler::TryToEnterBattleArenaGuildWar()
{
	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::BATTLE_ARENA_GUILD_WAR_ENTER, true))
	{
		return;
	}

	if (EResult::Vaild != CheckedEnterCondition(true))
	{	// 2023/11/23 PKT - 조건 미달
		return;
	}

	// 2023/11/24 PKT - 던전 쪽으로 토스
	GSDungeon()->TryToEnterDungeon(_mapId);
}

void FGsBattleArenaGuildWarHandler::SendReqGuildWarScoreInfo()
{
	if (EBattleArenaGuildWarState::EGamePlaying == _state)
	{
		FGsNetSendServiceWorld::SendReqBattleArenaGuildWarDetailScore();
	}	
}

void FGsBattleArenaGuildWarHandler::TryToOpenPlayDetailResult() 
{
	TWeakObjectPtr<UGsUIPopupBattleArenaGuildWarDetailScore> widget
		= Cast<UGsUIPopupBattleArenaGuildWarDetailScore>(GUI()->GetWidgetByKey(TEXT("BattleArenaGuildWarDetailScore")));

	widget = Cast<UGsUIPopupBattleArenaGuildWarDetailScore>(GUI()->OpenAndGetWidget(TEXT("BattleArenaGuildWarDetailScore")));
	if (widget.IsValid())
	{
		widget->SetData(_state, _redTeamInfo, _blueTeamInfo);
	}
}

const FGsSchemaGuildWarConfigData* FGsBattleArenaGuildWarHandler::GetConfigData(const MapId InMapId) const
{
	const FGsSchemaGuildWarConfigData* outData = nullptr;
	do 
	{
		const UGsTable* table = FGsSchemaGuildWarConfigData::GetStaticTable();
		const TMap<FName, uint8*>* OutRowMap;
		if (nullptr == table || false == table->GetRowMap(OutRowMap))
		{
			GSLOG(Error, TEXT("nullptr == table || false == table->GetRowMap(OutRowMap)"));
			break;
		}

		for (const auto& pair : (*OutRowMap))
		{	// 2022/06/09 PKT - 하나만 적용 시키고 빠져 나간다.
			const FGsSchemaGuildWarConfigData* findData = reinterpret_cast<const FGsSchemaGuildWarConfigData*>(pair.Value);
			if (findData && findData->mapDataId.GetRow() && findData->mapDataId.GetRow()->id == InMapId)
			{
				outData = findData;
				break;
			}
		}

		break;

	} while (0);

	if (nullptr == outData)
	{
		GSLOG(Error, TEXT("nullptr == outData"));
	}	
	return outData;
}

void FGsBattleArenaGuildWarHandler::ReservedShowTickerStartCount() const
{
	// 2023/12/22 PKT - 요 함수에 중복으로 호출 되는 경우가 있다. 그러면 화면에 숫자가 여러번 표기 되는데 이를 막고자 한번만 호출 한다.
	if (false == FGsUIHelper::IsActiveTrayTickerCountEx())
	{	
		static int32 life_time = 5;			
		FGsUIHelper::TrayTickerCountEx(life_time, _stateEndTime);
	}	
}

void FGsBattleArenaGuildWarHandler::OpenWindowPlayResult() const
{
	if (false == _endResultData._isSaved)
	{
		return;
	}

	TWeakObjectPtr<UGsUIPopupBattleArenaGuildWarDetailScore> popup
		= Cast<UGsUIPopupBattleArenaGuildWarDetailScore>(GUI()->GetWidgetByKey(TEXT("BattleArenaGuildWarDetailScore")));
	if (popup.IsValid())
	{	// 2023/11/28 PKT - 열려 있다면 닫아 준다.
		popup->Close();
	}

	if (false == GUI()->IsActiveWidget(TEXT("TrayBattleArenaPlayResult")))
	{
		GUI()->Open(TEXT("TrayBattleArenaPlayResult"));
	}
}