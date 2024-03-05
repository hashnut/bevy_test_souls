#pragma once

#include "CoreMinimal.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageContents.h"
#include "Message/GsMessageUI.h"
#include "Shared/Shared/SharedPackets/PD_Gateway_Client.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

/*
	
*/
class UGsUIWidgetBase;
class UGsUIManager;
class UGsUIPopupBuddy;
struct IGsMessageParam;
class FGsBuddyHandler
{
private:
	MsgGameObjHandleArray	_listGameObjectDelegate;
	TArray<TPair<MessageContents, FDelegateHandle>>		_listContentsDelegate;
	MsgUIHandleArray			_listUiDelegate;

protected:
	TWeakObjectPtr<class UGsUIManager>		_uiManager;
	TWeakObjectPtr<class UGsUIPopupBuddy>	_uiPopupBuddy;

protected:
	TWeakObjectPtr<class UGsUIManager> GetUIManager();	// UIManager 선 셋팅

public:
	void Init();
	void Close();

	// 메시지
	// 로컬 스폰 끝
	void OnLocalPlayerSpawnComplete(const IGsMessageParam*);
	// 친구ui 오픈
	void OnOpenPopupBuddy();	
	// 친구 추가 버튼을 눌렀을 때
	void OnAddBuddy(const IGsMessageParam* InParam);
	// 친구 초대 철회을 위해 X버튼을 눌렀을 때
	void OnAddCancelBuddy(const IGsMessageParam* InParam);
	// 내 친구 삭제를 위해 X버튼을 눌렀을 때
	void OnDeleteBuddy(const IGsMessageParam* InParam);
	// 친구 요청을 위해 V버튼을 눌렀을 때 
	void OnInviteAcceptBuddy(const IGsMessageParam* InParam);
	// 친구 거절을 위해 V버튼을 눌렀을 때 
	void OnInviteRefuseBuddy(const IGsMessageParam* InParam);
	// 친구에게 귓속말 할 때
	void OnWhisperBuddy(const IGsMessageParam* InParam);
	// 친구에서 파티 초대 할 때
	void OnInvitePartyBuddy(const IGsMessageParam* InParam);

protected:
	// 친구창 갱신
	void UpdateUIPopUpBuddy();	
	// 패킷 결과 에러 출력
	void BuddyErrorMsg(const IGsMessageParam* InParam);

public:
	// 현재 친구 리스트 v1
	void RecvBuddListAck(PD::GC::PKT_GC_ACK_BUDDY_LIST_READ& inPacket);
	// 친구 초대 리스트 v1
	void RecvBuddyRecvInvitationListAck(PD::GC::PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_READ& inPacket);
	// 친구 요청 리스트 v1
	void RecvBuddySendInvitationListAck(PD::GC::PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_READ& inPacket);

	// 친구가 내 초대에 응했을 때 (맞친구가 됐을때)
	void RecvBuddyAddNotify(uint64 inUserDBId, const FString& inUserName, time_t inElapsedTime, WorldId inWorldId = INVALID_WORLD_ID);
	// 내가친구 추가를 요청하고 응답을 받았다.
	void RecvBuddyInviteAck(IN UserDBId inUserDBId, IN FString inUserName, WorldId inWorldId = INVALID_WORLD_ID);
	// 나에게 온 친구 요청을 수락 했을 때
	void RecvBuddyAcceptAck(IN UserDBId inUserDBId, const FString& inUserName, time_t inElapsedTime, WorldId inWorldId = INVALID_WORLD_ID);
	// 내가 친구의 초대를 거절 또는 철회 했을 때
	void RecvBuddyDeleteInvitationdAck(IN UserDBId inReceiverUserDBId, UserDBId inSenderUserDBId);
	// 내 맞친구 삭제하고 응답을 받았다.
	void RecvBuddyDeletAck(IN UserDBId inUserDBId);
	// 친구가 날 친구 추가요청을 했을때
	void RecvBuddyInvitedNotify(IN UserDBId inUserDBId, IN FString inUserName, WorldId inWorldId = INVALID_WORLD_ID);
	// 친구가 로그인 했을 때
	void RecvBuddyLoginNotify(IN UserDBId inUserDBId);
	// 친구가 로그아웃 했을 때
	void RecvBuddyLogoutNotify(IN UserDBId inUserDBId);
	// 상대가 초대 거절 또는 초대 취소 알림
	void RecvBuddyInvitationDeletedNotify(IN UserDBId inReceiverUserDBId, IN UserDBId inSenderUserDBId);
	// 친구가 나를 친구 삭제 했을 때 
	void RecvBuddyDeletedNotify(IN UserDBId inUserDBId);



	// 현재 친구 리스트 v2
	void RecvBuddListAck_v2(PD::GC::PKT_GC_ACK_BUDDY_LIST_V2_READ& inPacket);
	// 친구 초대 리스트 v2
	void RecvBuddyRecvInvitationListAck_v2(PD::GC::PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_V2_READ& inPacket);
	// 친구 요청 리스트 v2
	void RecvBuddySendInvitationListAck_v2(PD::GC::PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_V2_READ& inPacket);
};
