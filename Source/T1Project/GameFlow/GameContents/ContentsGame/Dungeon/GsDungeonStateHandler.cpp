
#include "GsDungeonStateHandler.h"

#include "GsNormalDungeonState.h"
#include "GsSpecialDungeonState.h"
#include "GsPartyDungeonState.h"
#include "GsEliteDungeonState.h"
#include "GsBattleArenaDungeonState.h"

#include "../Level/GsLevelDefine.h"

#include "../Dungeon/GsDungeonHelper.h"
#include "../Dungeon/GsDungeonCommonEnum.h"
#include "../Dungeon/GsDungeonGroupBase.h"

#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGame/GsEventManager.h"
#include "../Management/ScopeGame/GsPartyManager.h"
#include "../Management/ScopeGame/GsDungeonManager.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"

#include "../Message/GsMessageSystem.h"
#include "../Message/GsMessageContentDungeon.h"
#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/MessageParam/GsUIMessageParam.h"
#include "../Message/MessageParam/GsInteractionMessageParam.h"

#include "../UTIL/GsTimeSyncUtil.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"

#include "DataSchema/Dungeon/GsSchemaCoopPartyDungeonConfig.h"
#include "DataSchema/Map/EventAction/GsSchemaMapEventActionImmediate.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "T1Project.h"


void FGsDungeonStateHandler::Initialize()
{
	// 2022/12/07 PKT - Category 별 던전
	_stateSet.Emplace(MakeShared<FGsNormalDungeonState>());
	_stateSet.Emplace(MakeShared<FGsSpecialDungeonState>());
	_stateSet.Emplace(MakeShared<FGsPartyDungeonState>());
	_stateSet.Emplace(MakeShared<FGsEliteDungeonState>());
	_stateSet.Emplace(MakeShared<FGsBattleArenaDungeonState>());

	// 2022/12/07 PKT - 입성한 던전 정보
	_currentState.Reset();

	RegisterMessage();
}

void FGsDungeonStateHandler::Finalize()
{
	UnRegisterMessage();

	_stateSet.Empty();
	_currentState.Reset();
}

const FGsDungeonStateBase* FGsDungeonStateHandler::GetState(DungeonCategory InCategory) const
{
	return Find(InCategory);
}

const FGsDungeonStateBase* FGsDungeonStateHandler::CurrentState() const
{
	return _currentState.IsValid() ? _currentState.Pin().Get() : nullptr;
}

FGsDungeonStateBase* FGsDungeonStateHandler::Find(DungeonCategory InCategory) const
{
	for (auto item : _stateSet)
	{
		if (item.IsValid() && item->GetCategory() == InCategory)
		{
			return item.Get();
		}
	}

	return nullptr;
}

void FGsDungeonStateHandler::RegisterMessage()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}

	_listSystemDelegate.Emplace(
		messageHolder->GetSystemParam().AddRaw(MessageSystem::LOAD_TOPO_START, this,
			&FGsDungeonStateHandler::OnLoadTopoStart));

	_gameObjectDelegate.Emplace(
		messageHolder->GetGameObject().AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, this,
			&FGsDungeonStateHandler::OnLocalPlayerSpawnComplete));

	_listDungeonDelegate.Emplace(
		messageHolder->GetDungeon().AddRaw(
			MessageContentDungeon::ENTER, this, &FGsDungeonStateHandler::OnEnterDungeon));

	_listDungeonDelegate.Emplace(
		messageHolder->GetDungeon().AddRaw(
			MessageContentDungeon::EXIT, this, &FGsDungeonStateHandler::OnExitDungeon));

	// 2022/07/27 PKT - 나가기 이벤트 메세지
	_listDungeonDelegate.Emplace(
		messageHolder->GetDungeon().AddRaw(
			MessageContentDungeon::TRY_EXIT_DUNGEON, this, &FGsDungeonStateHandler::OnMessageTryExit)
	);

	// 2022/07/27 PKT - Dungeon Time
	_listDungeonDelegate.Emplace(
		messageHolder->GetDungeon().AddRaw(
			MessageContentDungeon::UPDATE_TIME, this, &FGsDungeonStateHandler::OnUpdateTime)
	);

	_listDungeonDelegate.Emplace(
		messageHolder->GetDungeon().AddRaw(
			MessageContentDungeon::INTERACTION_PROP_PORTAL, this, &FGsDungeonStateHandler::OnInteractionPropPortalStart)
	);

	_listDungeonDelegate.Emplace(
		messageHolder->GetDungeon().AddRaw(
			MessageContentDungeon::START_PARTY_DUNGEON_COUNT, this, &FGsDungeonStateHandler::OnPartyDungeonStartCount)
	);

	_listDungeonDelegate.Emplace(
		messageHolder->GetDungeon().AddRaw(
			MessageContentDungeon::START_PARTY_DUNGEON_PLAY, this, &FGsDungeonStateHandler::OnPartyDungeonStartProgress)
	);

	_listDungeonDelegate.Emplace(
		messageHolder->GetDungeon().AddRaw(
			MessageContentDungeon::CLOSE_IMMINENT_PARTY_DUNGEON_PLAY, this, &FGsDungeonStateHandler::OnCloseImminentPartyDungeonStart)
	);

	_listDungeonDelegate.Emplace(
		messageHolder->GetDungeon().AddRaw(
			MessageContentDungeon::PARTY_DUNGEON_REQUEST_HELPSEEKING, this, &FGsDungeonStateHandler::OnRequestHelpseeking)
	);

	// 2022/07/27 PKT - interaction 이벤트 메세지
	_listGameObjectDelegate.Emplace(
		messageHolder->GetGameObject().AddRaw(
			MessageGameObject::INTERACTION_CONTENTS_BY_NPC_START, this, &FGsDungeonStateHandler::OnInteractionNpcKeeperContents)
	);

	_listGameObjectDelegate.Emplace(
		messageHolder->GetGameObject().AddRaw(MessageGameObject::INTERACTION_CONTENTS_BY_PROP_END, this, &FGsDungeonStateHandler::OnInteractionPropPortalEnd));

	_listGameObjectDelegate.Emplace(
		messageHolder->GetGameObject().AddRaw(MessageGameObject::PARTY_KICK, this, &FGsDungeonStateHandler::OnPartyKick));

	_listGameObjectDelegate.Emplace(
		messageHolder->GetGameObject().AddRaw(MessageGameObject::PARTY_BREAKS_UP, this, &FGsDungeonStateHandler::OnPartyLeave));
		
	// 2022/07/28 PKT - Local Player Die 이벤트 메세지
	_listGameObjectDelegate.Emplace(
		messageHolder->GetGameObject().AddRaw(MessageGameObject::LOCAL_DIE, this, &FGsDungeonStateHandler::OnLocalPlayerDie));
}

void FGsDungeonStateHandler::UnRegisterMessage()
{
	FGsMessageHolder* messageHolder = GMessage();
	if (nullptr == messageHolder)
	{
		GSLOG(Error, TEXT("nullptr == messageHolder"));
		return;
	}

	for (auto iter : _listSystemDelegate)
	{
		messageHolder->GetSystemParam().Remove(iter);
	}
	_listSystemDelegate.Empty();

	for (auto iter : _gameObjectDelegate)
	{
		messageHolder->GetGameObject().Remove(iter);
	}
	_gameObjectDelegate.Empty();

	for (auto iter : _listDungeonDelegate)
	{
		messageHolder->GetDungeon().Remove(iter);
	}
	_listDungeonDelegate.Empty();

	for (auto iter : _listGameObjectDelegate)
	{
		messageHolder->GetGameObject().Remove(iter);
	}
	_listGameObjectDelegate.Empty();
}

void FGsDungeonStateHandler::OnLoadTopoStart(const IGsMessageParam* InParam)
{
	if (false == _currentState.IsValid() || nullptr == GLevel())
	{
		return;
	}

	if (EGsLoadTopoEffectType::DIFF_MAP != GLevel()->GetLoadTopoEffectType())
	{
		return;
	}

	if (GMessage())
	{
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::EXIT, nullptr);
	}
}

void FGsDungeonStateHandler::OnLocalPlayerSpawnComplete(const IGsMessageParam* InParam)
{
	if (false == _currentState.IsValid())
	{
		if (const FGsSchemaMapData* currentMapData = GLevel()->GetCurrentMapInfo())
		{
			if (GSDungeon()->FindGroupDataById(currentMapData->id))
			{
				FGsPrimitiveInt32 message(currentMapData->id);
				GMessage()->GetDungeon().SendMessage(MessageContentDungeon::ENTER, &message);
			}
		}
	}
}

void FGsDungeonStateHandler::OnEnterDungeon(const struct IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* param = (nullptr != InParam) ? InParam->Cast<const FGsPrimitiveInt32>() : nullptr;
	if (nullptr == param)
	{
		GSLOG(Error, TEXT("nullptr == param"));
		return;
	}

	const FGsDungeonGroupBase* dungeonGroupData = GSDungeon()->FindGroupDataByMapId(param->_data);
	if (nullptr == dungeonGroupData)
	{
		GSLOG(Error, TEXT("nullptr == dungeonGroupData : Map Id : [%d]"), param->_data);
		return;
	}

	_currentState.Reset();

	for (auto item : _stateSet)
	{
		if (item.IsValid() && dungeonGroupData->GetCategory() == item->GetCategory())
		{
			bool result = item->Enter(param->_data);

			// 2022/11/01 PKT - Shared
			if (result)
			{
				_currentState = item;

				// 2022/11/01 PKT - 활성화 되어 있는 던전 시간 업데이트
				_currentState.Pin()->HUDTimeUpdate();
			}

			break;
		}
	}

	if (_currentState.IsValid())
	{
		MapId mapId = _currentState.Pin()->GetMapId();
		FGsPrimitiveInt32 message(mapId);
		GMessage()->GetDungeon().SendMessage(MessageContentDungeon::CONTENTS_DUNGEON_STAGE_ENTER, &message);
	}
}

void FGsDungeonStateHandler::OnExitDungeon(const IGsMessageParam* InParam)
{
	if (false == _currentState.IsValid())
	{
		return;
	}

	MapId prevMapId = _currentState.Pin()->GetMapId();

	if (DungeonCategory::PARTY != _currentState.Pin().Get()->GetCategory())
	{
		// 2022/12/14 PKT - Exit..
		_currentState.Pin()->Exit();
	}
	else
	{
		if (TSharedPtr<FGsPartyDungeonState> party = StaticCastSharedPtr<FGsPartyDungeonState>(_currentState.Pin()))
		{
			DungeonGroupId prevGroupId = party->GetGroupId();
			DungeonDifficulty difficulty = party->GetApplyDifficulty();

			// 2022/12/14 PKT - Exit..
			party->Exit();

			if (party && GSParty()->FindMyPartyMemberInfo())
			{	// 2022/12/14 PKT - 파티 던전에서 나갈때 파티 상태로 나가면 상태를 유지 시켜야 함.
				party->SetJoin(EDungeonProgressState::EJoinParty, prevGroupId, difficulty);
			}
		}
	}

	// 2022/12/14 PKT - Reset
	_currentState.Reset();

	FGsPrimitiveInt32 message(StaticCast<int32>(prevMapId));
	GMessage()->GetDungeon().SendMessage(MessageContentDungeon::CONTENTS_DUNGEON_STAGE_EXIT, &message);
}


// 2022/07/27 PKT - HUD Dungeon Exit Button Event
void FGsDungeonStateHandler::OnMessageTryExit(const IGsMessageParam* InParam)
{
	/**
	 * 아이템을 사용해서 나가는 경우와 사용하지 않고 '나가기'등을 통해 나가는 경우가 있음. 아이템 사용이 아닌 경우 'InParam == nullptr'
	 */
	if (false == _currentState.IsValid())
	{
		return;
	}

	ItemDBId useItem = INVALID_ITEM_DB_ID;
	if (InParam)
	{	// 2022/08/05 PKT - Param정보가 있으면 나가기 버튼이 아닌 귀환 주문서를 사용한 것!
		const FGsPrimitiveUInt64* param = InParam->Cast<const FGsPrimitiveUInt64>();
		if (nullptr == param || INVALID_ITEM_DB_ID == param->_data)
		{
			GSLOG(Error, TEXT("nullptr == param || INVALID_ITEM_DB_ID == param->_data"));
			return;
		}

		useItem = param->_data;
	}

	_currentState.Pin()->TryExit(useItem);
}

// 2022/07/27 PKT - On Dungeon Time
void FGsDungeonStateHandler::OnUpdateTime(const IGsMessageParam* InParam)
{
	if (false == _currentState.IsValid())
	{
		return;
	}

	DungeonGroupId targetGroupId = INVALID_DUNGEON_GROUP_ID;
	const FGsPrimitiveInt32* param = StaticCast<const FGsPrimitiveInt32*>(InParam);
	if (param)
	{
		targetGroupId = param->_data;
	}
	
	// 2022/11/01 PKT - 활성화 되어 있는 던전 시간 업데이트
	_currentState.Pin().Get()->HUDTimeUpdate(targetGroupId);
}

// 2022/07/27 PKT - On Interaction NPC Message Event
void FGsDungeonStateHandler::OnInteractionNpcKeeperContents(const IGsMessageParam* InParam)
{
	const FGsInteractionMessageParamContents* interactionParam = InParam->Cast<const FGsInteractionMessageParamContents>();
	if (nullptr == interactionParam)
	{
		GSLOG(Error, TEXT("nullptr == interactionParam"));
		return;
	}

	if (false == _currentState.IsValid())
	{
		return;
	}

	_currentState.Pin()->InteractionNpcKeeperContents(interactionParam->_npcTblId);
}

// 2022/07/27 PKT - On Interaction Prop Message Event
void FGsDungeonStateHandler::OnInteractionPropPortalStart(const IGsMessageParam* InParam)
{
	if (nullptr == InParam || false == _currentState.IsValid())
	{
		return;
	}

	const FGsPropInteractionMessageParamContents* interactionParam = InParam->Cast<const FGsPropInteractionMessageParamContents>();
	if (interactionParam)
	{
		_currentState.Pin().Get()->InteractionPropPortalStart(interactionParam->_tableId, interactionParam->_gameId);
	}
}

void FGsDungeonStateHandler::OnInteractionPropPortalEnd(const IGsMessageParam* InParam)
{
	if (nullptr == InParam || false == _currentState.IsValid())
	{
		return;
	}

	const FGsPropInteractionMessageParamContents* interactionParam = InParam->Cast<const FGsPropInteractionMessageParamContents>();
	if (interactionParam)
	{
		_currentState.Pin()->InteractionPropPortalEnd(interactionParam->_gameId);
	}
}

void FGsDungeonStateHandler::OnPartyKick(const struct IGsMessageParam* InParam)
{
	for (auto item : _stateSet)
	{
		if (item.IsValid() && DungeonCategory::PARTY == item->GetCategory())
		{
			if (EDungeonProgressState::ENone != item->GetProgressState())
			{
				StaticCastSharedPtr<FGsPartyDungeonState>(item)->NotifyPartyKickReason();
			}
		}
	}
}

void FGsDungeonStateHandler::OnPartyLeave(const struct IGsMessageParam* InParam)
{
	for (auto item : _stateSet)
	{
		if (item.IsValid() && DungeonCategory::PARTY == item->GetCategory())
		{			 
			if (EDungeonProgressState::ENone != item->GetProgressState())
			{
				StaticCastSharedPtr<FGsPartyDungeonState>(item)->ClearData();
			}			
		}
	}
}

void FGsDungeonStateHandler::OnPartyDungeonStartCount(const struct IGsMessageParam* InParam)
{
	if (nullptr == InParam || false == _currentState.IsValid())
	{
		return;
	}

	if (DungeonCategory::PARTY != _currentState.Pin()->GetCategory())
	{
		return;
	}

	const FGsPrimitiveUInt64* param = StaticCast<const FGsPrimitiveUInt64*>(InParam);
	
	FDateTime StartTime = FGsTimeSyncUtil::ConvertServerTimeToUTCDateTime(param->_data);
	FDateTime currentTime = FGsTimeSyncUtil::GetServerNowDateTime();

	FTimespan countTimeSpawn = (StartTime.GetTicks() > currentTime.GetTicks()) ? StartTime.GetTicks() - currentTime.GetTicks() : 0;
	int32 count = countTimeSpawn.GetTotalSeconds();
	FGsUIHelper::TrayTickerCount(0, count, nullptr);
}

void FGsDungeonStateHandler::OnPartyDungeonStartProgress(const struct IGsMessageParam* InParam)
{
	if (nullptr == InParam || false == _currentState.IsValid())
	{
		return;
	}

	if (DungeonCategory::PARTY != _currentState.Pin()->GetCategory())
	{
		return;
	}

	const FGsUIMsgParamDungeonTimeEX* param = StaticCast<const FGsUIMsgParamDungeonTimeEX*>(InParam);
	
	const FGsSchemaCoopPartyDungeonConfig* configData = GSDungeon()->GetPartyDungeonConfig();
	const FGsSchemaMapEventActionImmediate* timerEventData = (configData) ? configData->timerEventId.GetRow() : nullptr;
	if (nullptr == timerEventData)
	{
		return;
	}
		
	TArray<EvenParameterValue> timeDataSet;
	
	// 2023/3/6 PKT - 시작시간
	FDateTime BeginTime = FGsTimeSyncUtil::ConvertServerTimeToClientDateTime(param->_begineTime);

	// 2023/3/23 PKT - 흐른 시간 / 끝날 시간 Tick
	int64 elapsedTimeTick = FMath::Max<int64>(FGsTimeSyncUtil::GetClientNowDateTime().GetTicks() - BeginTime.GetTicks(), 0);
	int64 endTimeTick = FMath::Max<int64>((param->_endTime - param->_begineTime) * ETimespan::TicksPerSecond, 0);

	// 2023/3/23 PKT - Tick -> Second
	timeDataSet.Emplace(FTimespan(elapsedTimeTick).GetTotalSeconds());
	timeDataSet.Emplace(FTimespan(endTimeTick).GetTotalSeconds());

	GSEvent()->EventActionPlayImmediate(timerEventData->id, true, timeDataSet);
}

void FGsDungeonStateHandler::OnCloseImminentPartyDungeonStart(const struct IGsMessageParam* InParam)
{
	FGsDungeonStateBase* findData = Find(DungeonCategory::PARTY);
	if (nullptr == findData)
	{
		GSLOG(Error, TEXT("nullptr == findData"));
		return;
	}

	StaticCast<FGsPartyDungeonState*>(findData)->SetHelpseekingLock(true);
}

void FGsDungeonStateHandler::OnRequestHelpseeking(const struct IGsMessageParam* InParam)
{
	FGsDungeonStateBase* findData = Find(DungeonCategory::PARTY);
	if (nullptr == findData)
	{
		GSLOG(Error, TEXT("nullptr == findData"));
		return;
	}

	StaticCast<FGsPartyDungeonState*>(findData)->RequestHelpseeking();
}

// 2022/07/28 PKT - Local Player Die
void FGsDungeonStateHandler::OnLocalPlayerDie(const IGsMessageParam* InParam)
{
	for (auto item : _stateSet)
	{
		if (item.IsValid() && EDungeonProgressState::ENone != item->GetProgressState())
		{
			item->LocalPlayerDie();
		}
	}
}


/**
* Server => Client
*/
void FGsDungeonStateHandler::RecvJoinPartyDungeon(PD::GC::PKT_GC_ACK_COOP_PARTY_JOIN_READ& InPacket)
{
	/**
	 * 
	 */
}

// 2022/06/15 PKT - 파티 던전 매칭 중
void FGsDungeonStateHandler::RecvMatchingAck(PD::GC::PKT_GC_ACK_COOP_PARTY_AUTO_MATCHING_READ& InPacket)
{
	FGsDungeonStateBase* findData = Find(DungeonCategory::PARTY);
	if (nullptr == findData)
	{
		GSLOG(Error, TEXT("nullptr == findData"));
		return;
	}

	/**
	 * 2022/11/02 PKT - 파티 던전을 위해 매칭이 시작.
	 */
	StaticCast<FGsPartyDungeonState*>(findData)->SetJoin(
		EDungeonProgressState::EMatching, InPacket.DungeonGroupId(), InPacket.DungeonDifficulty());
}

// 2022/06/15 PKT - 파티 던전을 위해 파티가 결성.
void FGsDungeonStateHandler::RecvInPartyNotify(PD::GC::PKT_GC_NOTIFY_COOP_PARTY_DUNGEON_INFO_READ& InPacket)
{
	FGsDungeonStateBase* findData = Find(DungeonCategory::PARTY);
	if (nullptr == findData)
	{
		GSLOG(Error, TEXT("nullptr == findData"));
		return;
	}

	/**
	 * 2022/11/02 PKT - 파티 던전을 위해 매칭이 시작.
	 */
	StaticCast<FGsPartyDungeonState*>(findData)->SetJoin(
		EDungeonProgressState::EJoinParty, InPacket.DungeonGroupId(), InPacket.DungeonDifficulty());
}

// 2022/06/15 PKT - 던전 파티 매칭 취소에 대한 응답
void FGsDungeonStateHandler::RecvMatchingCancelAck(PD::GC::PKT_GC_ACK_COOP_PARTY_AUTO_MATCHING_CANCEL_READ& InPacket)
{
	FGsDungeonStateBase* findData = Find(DungeonCategory::PARTY);
	if (nullptr == findData)
	{
		GSLOG(Error, TEXT("nullptr == findData"));
		return;
	}

	FText message;
	FGsDungeonHelper::FindText(TEXT("PartyDungeon_Notice_AutoMatching_Cancel"), message);
	FGsUIHelper::TrayMessageTicker(message);
	
	/**
	 * 2022/11/02 PKT - 던전 파티 매칭 취소
	 */
	StaticCast<FGsPartyDungeonState*>(findData)->PartyMathcingBroken();
}
//	// 2022/06/15 PKT - 던전 파티 매칭 실패에 대한 응답
void FGsDungeonStateHandler::RecvMatchingFailedNotify(PD::GC::PKT_GC_COOP_PARTY_AUTO_MATCHING_CANCELED_READ& InPacket)
{
	FGsDungeonStateBase* findData = Find(DungeonCategory::PARTY);
	if (nullptr == findData)
	{
		GSLOG(Error, TEXT("nullptr == findData"));
		return;
	}

	/**
	 * 2022/11/02 PKT - 던전 파티 매칭 실패
	 */
	StaticCast<FGsPartyDungeonState*>(findData)->PartyMathcingBroken();
}

// 2022/06/29 PKT - 도움 요청에 대한 응답( 도움 요청한 사람만 받을 수 있음. - 파티장 )
void FGsDungeonStateHandler::RecvPartyDungeonAskHelpAck(PD::GC::PKT_GC_ACK_COOP_PARTY_SET_HELP_READ& InPacket)
{
	FGsDungeonStateBase* findData = Find(DungeonCategory::PARTY);
	if (nullptr == findData)
	{
		GSLOG(Error, TEXT("nullptr == findData"));
		return;
	}

	/**
	 * 
	 */
	StaticCast<FGsPartyDungeonState*>(findData)->SetHelpseekingEnable(false);
}

// 2022/06/29 PKT - 도움 요청에 대한 상태( 도움 요청에 대한 결과 - 파티원 )
void FGsDungeonStateHandler::RecvPartyDungeonHelpInfo(PD::GC::PKT_GC_NOTIFY_COOP_PARTY_HELP_INFO_READ& InPacket)
{
	/**
	* 2022/06/29 PKT - 도움 요청에 대한 상태 정보는 모든 파티원들이 다 받는다.
	*                  이유는 파티장이 나갔을 경우 도움 요청에 대한 상태를 그대로 다른 파티장이 승계받아야 하기 때문에
	*                  다른 파티원들도 모두 도움 요청 정보를 갖고 있는다.
	*/
	FGsDungeonStateBase* findData = Find(DungeonCategory::PARTY);
	if (nullptr == findData)
	{
		GSLOG(Error, TEXT("nullptr == findData"));
		return;
	}

	// 2022/11/02 PKT - 도움 요청에 대한 난입이 들어오기 전까지 중복해서 도움 요청을 할 수 없음.
	StaticCast<FGsPartyDungeonState*>(findData)->SetHelpseekingInfo(InPacket.HelpCount(), false == InPacket.CanForcedEnter());
}