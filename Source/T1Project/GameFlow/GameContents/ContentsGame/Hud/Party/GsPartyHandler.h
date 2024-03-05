#pragma once

#include "CoreMinimal.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"

#include "Message/GsMessageGameObject.h"

/*
	파티 핸들러:
	party 매니저에서 다른 게임 매니저 참조 하는것은 여기서 처리
*/
class FGsPartyMemberInfo;
class UGsGameObjectBase;

struct IGsMessageParam;

class FGsPartyHandler
{
private:
	// game object 메세지 해제용
	MsgGameObjHandleArray _gameObjectMsg;

public:
	void Init();
	void Close();

	// 메시지
	// 로컬 스폰 끝
	void OnLocalPlayerSpawnComplete(const IGsMessageParam*);
	// common despawn message
	void OnCommonDespawn(const IGsMessageParam* inParam);
	// common spawn message
	void OnCommonSpawn(const IGsMessageParam* inParam);
	void OnCommonDie(const IGsMessageParam* inParam);

	// 생성 응답
	void RecvPartyCreate(PartyDropOwnershipType In_type);
	// 내정보 만들기
	FGsPartyMemberInfo* MakeMyInfo(int In_slotId, bool In_isLeader);
	// 초대 수락
	void RecvPartyJoined(PD::GC::PKT_GC_PARTY_JOINED_READ& In_packet, TMap<int64, bool> In_mapSpawnedInfo);	
	// 2023/7/17 PKT - 초대 수락 실패에 대한 초대자 정보 초기화
	void RecvClearInviterInfo();
	// 초대 수락 실패
	void RecvPartyJoinedFailed();

	// 네임플레이트 갱신
	void UpdateNameplate(int64 InGameId);
	// 멤버 추가(초대 수락)
	void RecvPartyMemberJoined(PD::GC::PKT_GC_PARTY_MEMBER_JOINED_READ& In_packet, bool In_isSpawned);
	// 멤버 갱신
	void RecvPartyMemberUpdate(PD::GC::PKT_GC_PARTY_MEMBER_UPDATED_READ& In_packet, bool In_isSpawned);
	// 파티 나감(내가)
	void RecvPartyLeave();
	// 파티원 나감
	void RecvPartyMemberLeft(int In_leftSlotId, int In_leaderSlotId);
	// 멤버중 하나 추방당함
	void RecvPartyKick(int In_kickSlotId);
	// 추방 당함
	void RecvPartyKicked();
	// 초대 거절 처리
	void CancelInvite();

	// 2022/06/13 PKT - 멤버의 준비 상태
	void RecvPartyMemberReadyState(PD::GC::PKT_GC_NOTIFY_COOP_PARTY_READY_STATUS_READ& InPacket);
	// 2022/06/13 PKT - 자신의 준비 상태
	void RecvPartyReadyState(PD::GC::PKT_GC_ACK_COOP_PARTY_READY_READ& InPacket);

	// test party target
	void RecvPartyTargetClick(bool In_isActive, int64 In_targetServerId = -1);

	// ui event
	void OnClickPartyTargetButton(bool In_isActive);

	void UpdatePartyTargetButtonHUDMode();

	void UpdateSharedIdState(UGsGameObjectBase* In_findObj = nullptr);

	void ChangedSharePartyTargetId(int64 In_id);
};    