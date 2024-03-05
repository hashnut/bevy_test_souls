#pragma once


/**
* file		GsDungeonStateHandler.h
* @brief	
* @author	PKT
* @date		2022/12/14
**/

#include "CoreMinimal.h"
#include "GsDungeonStateBase.h"
#include "../Message/GsMessageSystem.h"
#include "../Message/GsMessageGameObject.h"
#include "../Message/GsMessageContentDungeon.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"



class FGsDungeonStateHandler
{
private:
	TArray<TSharedPtr<FGsDungeonStateBase>> _stateSet;
	// 2022/12/12 PKT - Current In Dungeon
	TWeakPtr<FGsDungeonStateBase> _currentState;

	// 2022/12/14 PKT - Delegate
	MsgSystemHandleArray _listSystemDelegate;
	MsgGameObjHandleArray _gameObjectDelegate;
	MsgDungeonHandleArray _listDungeonDelegate;
	MsgGameObjHandleArray _listGameObjectDelegate;

public:
	void Initialize();
	void Finalize();
		
	const FGsDungeonStateBase* GetState(DungeonCategory InCategory) const;
	const FGsDungeonStateBase* CurrentState() const;

protected:
	FGsDungeonStateBase* Find(DungeonCategory InCategory) const;

	// 2022/12/14 PKT - Delegate
	void RegisterMessage();
	void UnRegisterMessage();

	void OnLoadTopoStart(const struct IGsMessageParam* InParam);
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam);
	// 2022/12/12 PKT - Enter Dungone
	void OnEnterDungeon(const struct IGsMessageParam* InParam);
	// 2022/12/12 PKT - Exit Dungeon
	void OnExitDungeon(const struct IGsMessageParam* InParam);
	// 2022/07/27 PKT - HUD Dungeon Exit Button Event
	void OnMessageTryExit(const struct IGsMessageParam* InParam);
	// 2022/07/27 PKT - On Dungeon Time
	void OnUpdateTime(const struct IGsMessageParam* InParam);
	// 2022/07/27 PKT - On Interaction NPC Message Event
	void OnInteractionNpcKeeperContents(const struct IGsMessageParam* InParam);
	// 2022/07/27 PKT - On Interaction Prop Message Event
	void OnInteractionPropPortalStart(const struct IGsMessageParam* InParam);	
	void OnInteractionPropPortalEnd(const struct IGsMessageParam* InParam);
	// 2022/07/28 PKT - Local Player Die
	void OnLocalPlayerDie(const struct IGsMessageParam* InParam);
	// 2023/2/15 PKT - Party 
	void OnPartyKick(const struct IGsMessageParam* InParam);
	void OnPartyLeave(const struct IGsMessageParam* InParam);	
	// 2023/3/6 PKT - PartyDungeon Event Start Count(파티 던전 플레이가 시작 되기 전 카운트)
	void OnPartyDungeonStartCount(const struct IGsMessageParam* InParam);
	// 2023/3/6 PKT - PartyDungeon Event Start(파티 던전 플레이가 시작 되었음. 파티 던전 입장과는 다름.)
	void OnPartyDungeonStartProgress(const struct IGsMessageParam* InParam);
	// 2023/3/23 PKT - 파티 던전 종료가 임박되었음을 알린다.
	void OnCloseImminentPartyDungeonStart(const struct IGsMessageParam* InParam);
	// 2023/3/23 PKT - 파티 던전 내 도움 요청
	void OnRequestHelpseeking(const struct IGsMessageParam* InParam);
	
	/**
	 * Server => Client
	 */
public:
	//
	void RecvJoinPartyDungeon(PD::GC::PKT_GC_ACK_COOP_PARTY_JOIN_READ& InPacket);
	// 2022/06/15 PKT - 파티 던전 매칭 중
	void RecvMatchingAck(PD::GC::PKT_GC_ACK_COOP_PARTY_AUTO_MATCHING_READ& InPacket);
	// 2022/06/15 PKT - 파티 던전을 위해 파티가 결성.
	void RecvInPartyNotify(PD::GC::PKT_GC_NOTIFY_COOP_PARTY_DUNGEON_INFO_READ& InPacket);
	// 2022/06/15 PKT - 던전 파티 매칭 취소에 대한 응답
	void RecvMatchingCancelAck(PD::GC::PKT_GC_ACK_COOP_PARTY_AUTO_MATCHING_CANCEL_READ& InPacket);
	// 2022/06/15 PKT - 던전 파티 매칭 실패에 대한 응답
	void RecvMatchingFailedNotify(PD::GC::PKT_GC_COOP_PARTY_AUTO_MATCHING_CANCELED_READ& InPacket);
	// 2022/06/29 PKT - 도움 요청에 대한 응답( 도움 요청한 사람만 받을 수 있음. - 파티장 )
	void RecvPartyDungeonAskHelpAck(PD::GC::PKT_GC_ACK_COOP_PARTY_SET_HELP_READ& InPacket);
	// 2022/06/29 PKT - 도움 요청에 대한 상태( 도움 요청에 대한 결과 - 파티원 )
	void RecvPartyDungeonHelpInfo(PD::GC::PKT_GC_NOTIFY_COOP_PARTY_HELP_INFO_READ& InPacket);
};