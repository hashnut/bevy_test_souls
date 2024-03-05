#include "GsBuddyHandler.h"

#include "Net/GsNetSendService.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsBuddyManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsChatManager.h"
#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupPartyInvite.h"
#include "UI/UIContent/Popup/GsUIPopupBuddy.h"

#include "Option/GsGameUserSettings.h"
#include "Option/GsServerOption.h"

#include "Buddy/GsBuddyInfo.h"


void FGsBuddyHandler::Init()
{
	FGsMessageHolder* msg = GMessage();

	auto& contentsHandler = msg->GetContents();
	auto& gameObjectHandler = msg->GetGameObject();
	auto& uiHandler = msg->GetUI();

	_listGameObjectDelegate.Emplace(
		gameObjectHandler.AddRaw(MessageGameObject::LOCAL_SPAWN_ME_COMPLETE, 
			this, &FGsBuddyHandler::OnLocalPlayerSpawnComplete));

	_listGameObjectDelegate.Emplace(
		gameObjectHandler.AddRaw(MessageGameObject::LOCAL_REVIVE,
			this, &FGsBuddyHandler::OnLocalPlayerSpawnComplete));

	_listGameObjectDelegate.Emplace(
		gameObjectHandler.AddRaw(MessageGameObject::BUDDY_PACKET_RESULT_ERROR,
			this, &FGsBuddyHandler::BuddyErrorMsg));
	
	_listContentsDelegate.Emplace(contentsHandler.AddRaw(MessageContents::ACTIVE_POPUP_BUDDY,
		this, &FGsBuddyHandler::OnOpenPopupBuddy));

	_listUiDelegate.Emplace(uiHandler.AddRaw(MessageUI::BUDDY_ADD,
		this, &FGsBuddyHandler::OnAddBuddy));
	_listUiDelegate.Emplace(uiHandler.AddRaw(MessageUI::BUDDY_ADD_CANCEL,
		this, &FGsBuddyHandler::OnAddCancelBuddy));
	_listUiDelegate.Emplace(uiHandler.AddRaw(MessageUI::BUDDY_DELETE,
		this, &FGsBuddyHandler::OnDeleteBuddy));
	_listUiDelegate.Emplace(uiHandler.AddRaw(MessageUI::BUDDY_INVITE_ACCEPT,
		this, &FGsBuddyHandler::OnInviteAcceptBuddy));
	_listUiDelegate.Emplace(uiHandler.AddRaw(MessageUI::BUDDY_INVITE_REFUSE,
		this, &FGsBuddyHandler::OnInviteRefuseBuddy));
	_listUiDelegate.Emplace(uiHandler.AddRaw(MessageUI::BUDDY_WHISPER,
		this, &FGsBuddyHandler::OnWhisperBuddy));
	_listUiDelegate.Emplace(uiHandler.AddRaw(MessageUI::BUDDY_INVITE_PARTY,
		this, &FGsBuddyHandler::OnInvitePartyBuddy));

	//SetRedDot();
}

void FGsBuddyHandler::Close()
{
	FGsMessageHolder* msg = GMessage();

	for (auto iter : _listGameObjectDelegate)
	{
		msg->GetGameObject().Remove(iter);
	}
	_listGameObjectDelegate.Empty();

	for (auto iter : _listContentsDelegate)
	{
		msg->GetContents().Remove(iter);
	}
	_listContentsDelegate.Empty();

	for (auto iter : _listUiDelegate)
	{
		msg->GetUI().Remove(iter);
	}
	_listUiDelegate.Empty();
}

void FGsBuddyHandler::OnLocalPlayerSpawnComplete(const IGsMessageParam*)
{
	GSBuddy()->InitializeUserData();
	UpdateUIPopUpBuddy();
}

// 친구ui 오픈
void FGsBuddyHandler::OnOpenPopupBuddy()
{
	GSLOG(Error, TEXT("----OnOpenPopupBuddy"));
	GSLOG_S(Error);

	_uiManager = GetUIManager();
	if (_uiManager.IsValid())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = _uiManager->GetWidgetByKey(TEXT("PopupBuddy"));
		if (widget.IsValid())
		{
			_uiPopupBuddy = Cast<UGsUIPopupBuddy>(widget);
			_uiPopupBuddy->InvalidateAll();
		}
		else
		{
			_uiPopupBuddy = Cast<UGsUIPopupBuddy>(_uiManager->OpenAndGetWidget(TEXT("PopupBuddy")));
		}
	}

	// send
	FGsNetSendService::SendBuddyList_v2();
}

// 친구 추가 버튼을 눌렀을 때
void FGsBuddyHandler::OnAddBuddy(const IGsMessageParam* InParam)
{	
	const FGsUIMsgParamBuddy* param = InParam->Cast<const FGsUIMsgParamBuddy>();

	if (false == GSBuddy()->CheckIsAdded(param->_userName))
		return;

	auto result = FGsTextUtil::IsValidUserSearchNameOnly(param->_userName, false);
	if (FGsTextUtil::EUserNameValid::Valid != result.Get<0>())
		return;

	// send
	FGsNetSendService::SendBuddyInvite_v2(result.Get<1>(), result.Get<2>().ToString());
}

// 친구 초대 철회을 위해 X버튼을 눌렀을 때
void FGsBuddyHandler::OnAddCancelBuddy(const IGsMessageParam* InParam)
{
	// 신청 신청 철회
	const FGsUIMsgParamBuddy* param = InParam->Cast<const FGsUIMsgParamBuddy>();

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	// send
	FGsNetSendService::SendBuddyDeleteInvitation_v2(param->_userDBId, userData->mUserDBId);
}

// 내 친구 삭제를 위해 X버튼을 눌렀을 때
void FGsBuddyHandler::OnDeleteBuddy(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamBuddy* param = InParam->Cast<const FGsUIMsgParamBuddy>();

	// TEXT: 삭제할꺼야??
	FString strMgs;
	FText msg;
	FText::FindText(TEXT("UICommonText"), TEXT("DeleteUser"), msg);
	strMgs = FString::Format(*msg.ToString(), { param->_userName });

	uint64 userDBID = param->_userDBId;

	FGsUIHelper::PopupYesNo(FText::FromString(strMgs), [userDBID](bool bYes)
	{
		if (bYes)
		{
			// send
			FGsNetSendService::SendBuddyDelete_v2(userDBID);			
		}
	});
}

// 친구 요청을 위해 V버튼을 눌렀을 때 
void FGsBuddyHandler::OnInviteAcceptBuddy(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamBuddy* param = InParam->Cast<const FGsUIMsgParamBuddy>();

	if (false == GSBuddy()->CheckIsAccept(param->_userName))
		return;

#if WITH_EDITOR
	GSLOG(Log, TEXT("[Buddy] FGsBuddyHandler OnInviteAcceptBuddy- ID: %llu"), param->_userDBId);
#endif

	// send
	FGsNetSendService::SendBuddyAccept_v2(param->_userDBId);	
}

// 친구 거절을 위해 V버튼을 눌렀을 때 
void FGsBuddyHandler::OnInviteRefuseBuddy(const IGsMessageParam* InParam) 
{
	// 친구 신청 거절
	const FGsUIMsgParamBuddy* param = InParam->Cast<const FGsUIMsgParamBuddy>();

	// send
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	// send
	//내가 신청 받은 초대를 거절 : receiverUserDBId (자신 ID), senderUserDBId (상대 ID)
	FGsNetSendService::SendBuddyDeleteInvitation_v2(userData->mUserDBId, param->_userDBId);
}

// 친구에게 귓속말 할 때
void FGsBuddyHandler::OnWhisperBuddy(const IGsMessageParam* InParam)
{
	if (const FGsUIMsgParamBuddy* param = InParam->Cast<const FGsUIMsgParamBuddy>())
	{
		GSChat()->OpenWhisperInput(param->_userName);
	}
}

// 친구에서 파티 초대 할 때
void FGsBuddyHandler::OnInvitePartyBuddy(const IGsMessageParam* InParam)
{
	FGsPartyManager* partyManager = GSParty();
	if (nullptr == partyManager)
		return;
	
	if (!partyManager->GetIsInParty())
	{
		// 파티를 먼저 생성해야 합니다
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker7"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	}
	else
	{
		if (!partyManager->AmILeader())
		{
			// 친구를 초대 할 수 없다.
			FText findText;
			FText::FindText(TEXT("PartyText"), TEXT("PartyTicker10"), findText);
			FGsUIHelper::TrayMessageTicker(findText);
		}
		else
		{
			const FGsUIMsgParamBuddy* param = InParam->Cast<const FGsUIMsgParamBuddy>();

			if (UGsUIPopupPartyInvite* invitePopup = Cast<UGsUIPopupPartyInvite>(GUI()->OpenAndGetWidget(TEXT("PopupPartyInvite"))))
			{
				invitePopup->SetInitInvitee(param->_userName);
			}
		}
	}
}

// 현재 친구 리스트 v1
void FGsBuddyHandler::RecvBuddListAck(PD::GC::PKT_GC_ACK_BUDDY_LIST_READ & inPacket)
{
	FGsBuddyManager* buddyManager = GSBuddy();
	if (nullptr == buddyManager)
		return;

	buddyManager->ClearBuddyList();
	buddyManager->SetBuddyTabRedDot(inPacket.IsRedDot());

	for (
		PD::GC::PKT_GC_ACK_BUDDY_LIST_READ::BuddyListIterator iterator = inPacket.GetFirstBuddyListIterator();
		iterator != inPacket.GetLastBuddyListIterator();
		++iterator
		)
	{
		time_t lastLoginTim = iterator->LastLoginTime();
		buddyManager->AddBuddy(iterator->UserDBId(), iterator->UserName(),
			lastLoginTim);
	}
}

// 친구 초대 리스트 v1
void FGsBuddyHandler::RecvBuddyRecvInvitationListAck(PD::GC::PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_READ & inPacket)
{
	FGsBuddyManager* buddyManager = GSBuddy();
	if (nullptr == buddyManager)
		return;

	buddyManager->ClearAcceptBuddyList();
	buddyManager->SetBuddyAcceptTabRedDot(inPacket.IsRedDot());

	for (
		PD::GC::PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_READ::RecvInvitationListIterator iterator = inPacket.GetFirstRecvInvitationListIterator();
		iterator != inPacket.GetLastRecvInvitationListIterator();
		++iterator
		)
	{
		buddyManager->AddAcceptBuddy(iterator->UserDBId(), iterator->UserName());
	}
}

// 친구 요청 리스트 v1
void FGsBuddyHandler::RecvBuddySendInvitationListAck(PD::GC::PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_READ & inPacket)
{
	FGsBuddyManager* buddyManager = GSBuddy();
	if (nullptr == buddyManager)
		return;

	buddyManager->ClearApplyBuddyList();

	for (
		PD::GC::PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_READ::SendInvitationListIterator iterator = inPacket.GetFirstSendInvitationListIterator();
		iterator != inPacket.GetLastSendInvitationListIterator();
		++iterator
		)
	{
		buddyManager->AddApplyBuddy(iterator->UserDBId(), iterator->UserName());
	}

	UpdateUIPopUpBuddy();
}

///////// v2 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 현재 친구 리스트 v2
void FGsBuddyHandler::RecvBuddListAck_v2(PD::GC::PKT_GC_ACK_BUDDY_LIST_V2_READ& inPacket)
{
	FGsBuddyManager* buddyManager = GSBuddy();
	if (nullptr == buddyManager)
		return;

	buddyManager->ClearBuddyList();
	buddyManager->SetBuddyTabRedDot(inPacket.IsRedDot());

	for (
		PD::GC::PKT_GC_ACK_BUDDY_LIST_V2_READ::BuddyListIterator iterator = inPacket.GetFirstBuddyListIterator();
		iterator != inPacket.GetLastBuddyListIterator();
		++iterator
		)
	{
		time_t lastLoginTim = iterator->LastLoginTime();
		buddyManager->AddBuddy(iterator->UserDBId(), iterator->UserName(),
			lastLoginTim, iterator->WorldId());
	}

	UpdateUIPopUpBuddy();
}

// 친구 초대 리스트 v2
void FGsBuddyHandler::RecvBuddyRecvInvitationListAck_v2(PD::GC::PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_V2_READ& inPacket)
{
	FGsBuddyManager* buddyManager = GSBuddy();
	if (nullptr == buddyManager)
		return;

	buddyManager->ClearAcceptBuddyList();
	buddyManager->SetBuddyAcceptTabRedDot(inPacket.IsRedDot());

	for (
		PD::GC::PKT_GC_ACK_BUDDY_RECV_INVITATION_LIST_V2_READ::RecvInvitationListIterator iterator = inPacket.GetFirstRecvInvitationListIterator();
		iterator != inPacket.GetLastRecvInvitationListIterator();
		++iterator
		)
	{
		buddyManager->AddAcceptBuddy(iterator->UserDBId(), iterator->UserName(), iterator->WorldId());
	}

	UpdateUIPopUpBuddy();
}

// 친구 요청 리스트 v2
void FGsBuddyHandler::RecvBuddySendInvitationListAck_v2(PD::GC::PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_V2_READ& inPacket)
{
	FGsBuddyManager* buddyManager = GSBuddy();
	if (nullptr == buddyManager)
		return;

	buddyManager->ClearApplyBuddyList();

	for (
		PD::GC::PKT_GC_ACK_BUDDY_SEND_INVITATION_LIST_V2_READ::SendInvitationListIterator iterator = inPacket.GetFirstSendInvitationListIterator();
		iterator != inPacket.GetLastSendInvitationListIterator();
		++iterator
		)
	{
		buddyManager->AddApplyBuddy(iterator->UserDBId(), iterator->UserName(), iterator->WorldId());
	}

	UpdateUIPopUpBuddy();
}

// 친구가 내 초대에 응했을 때 (맞친구가 됐을때)
void FGsBuddyHandler::RecvBuddyAddNotify(uint64 inUserDBId, const FString& inUserName, time_t inElapsedTime, WorldId inWorldId)
{
	FGsBuddyManager* buddyManager = GSBuddy();
	if (nullptr == buddyManager)
		return;

	buddyManager->DeleteApplyBuddy(inUserDBId);
	buddyManager->AddBuddy(inUserDBId, inUserName, inElapsedTime, inWorldId);
	buddyManager->SetBuddyTabRedDot(true);

	UpdateUIPopUpBuddy();
}
	

// 내가친구 추가를 요청하고 응답을 받았다.
void FGsBuddyHandler::RecvBuddyInviteAck(IN UserDBId inUserDBId, IN FString inUserName, WorldId inWorldId)
{
	FText findText;
	// 친구 신청을 보냈습니다.
	FText::FindText(TEXT("FriendText"), TEXT("FriendTicker2"), findText);
	FGsUIHelper::TrayMessageTicker(findText);

	GSBuddy()->AddApplyBuddy(inUserDBId, inUserName, inWorldId);
	UpdateUIPopUpBuddy();
}

// 나에게 온 친구 요청을 수락 했을 때
void FGsBuddyHandler::RecvBuddyAcceptAck(IN UserDBId inUserDBId, const FString& inUserName, time_t inElapsedTime, WorldId inWorldId)
{
	FGsBuddyManager* buddyManager = GSBuddy();
	if (nullptr == buddyManager)
		return;

	buddyManager->DeleteAcceptBuddy(inUserDBId);
	buddyManager->AddBuddy(inUserDBId, inUserName, inElapsedTime, inWorldId);
	buddyManager->SetBuddyTabRedDot(true);
	UpdateUIPopUpBuddy();
}

// 내가 친구의 초대를 거절 또는 철회 했을 때
void FGsBuddyHandler::RecvBuddyDeleteInvitationdAck(IN UserDBId inReceiverUserDBId, UserDBId inSenderUserDBId)
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	FGsBuddyManager* buddyManager = GSBuddy();
	if (nullptr == buddyManager)
		return;

	UserDBId myDBId = userData->mUserDBId;

	if (inReceiverUserDBId == myDBId)
	{
		// 친구가 보낸 친구요청을 거절했을 때
		buddyManager->DeleteAcceptBuddy(inSenderUserDBId);
	}

	if (inSenderUserDBId == myDBId)
	{
		// 내가 신청한 초대를 철회했을 때
		buddyManager->DeleteApplyBuddy(inReceiverUserDBId);
	}

	UpdateUIPopUpBuddy();
}

// 내 맞친구 삭제하고 응답을 받았다.
void FGsBuddyHandler::RecvBuddyDeletAck(IN UserDBId inUserDBId)
{
	GSBuddy()->DeleteBuddy(inUserDBId);
	UpdateUIPopUpBuddy();
}

// 친구가 날 친구 추가요청을 했을때
void FGsBuddyHandler::RecvBuddyInvitedNotify(IN UserDBId inUserDBId, IN FString inUserName, WorldId inWorldId)
{	
	FGsBuddyManager* buddyManager = GSBuddy();
	if (nullptr == buddyManager)
		return;

	buddyManager->AddAcceptBuddy(inUserDBId, inUserName, inWorldId);
	buddyManager->SetBuddyAcceptTabRedDot(true);
	UpdateUIPopUpBuddy();	
}

// 친구가 로그인 했을 때
void FGsBuddyHandler::RecvBuddyLoginNotify(IN UserDBId inUserDBId)
{
	FString buddyName;
	if (GSBuddy()->GetBuddyName(inUserDBId, buddyName))
	{
		// 옵션이 켜져 있을때만 사용 
		if (FGsServerOption* serverOption = GServerOption())
		{
			const int bHideTicker = serverOption->GetOptionLoginAlert(EGsOptionLoginAlert::BUDDY_ALERT);
			if (0 != bHideTicker)
			{
				FText findText;
				// 친구 {0} 님이 접속하였습니다.
				FText::FindText(TEXT("FriendText"), TEXT("FriendTicker1"), findText);
				FText resultText = FText::Format(findText, FText::FromString(buddyName));

				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, resultText, TrayCondition::TC_Close, SectionPriority::None);
			}
		}

		GSBuddy()->SetBuddyLogin(inUserDBId);
		UpdateUIPopUpBuddy();
	}
}

// 친구가 로그아웃 했을 때
void FGsBuddyHandler::RecvBuddyLogoutNotify(IN UserDBId inUserDBId)
{
	GSBuddy()->SetBuddyLogout(inUserDBId);
	UpdateUIPopUpBuddy();
}

// 상대가 초대 거절 또는 초대 취소 알림
void FGsBuddyHandler::RecvBuddyInvitationDeletedNotify(IN UserDBId inReceiverUserDBId, IN UserDBId inSenderUserDBId)
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
		return;

	if (inSenderUserDBId == userData->mUserDBId)
	{
		//내가 신청한 초대를 상대가 거절한 경우(나의 신청 탭에서 상대를 삭제) : senderUserDBId(자신 ID), receiverUserDBId(상대 ID)
		GSBuddy()->DeleteApplyBuddy(inReceiverUserDBId);
	}

	if (inReceiverUserDBId == userData->mUserDBId)
	{
		//상대가 신청한 초대를 취소한 경우(나의 수락 탭에서 상대를 삭제) : senderUserDBId(상대 ID), receiverUserDBId(자신 ID)
		GSBuddy()->DeleteAcceptBuddy(inSenderUserDBId);
	}

	UpdateUIPopUpBuddy();
}

// 친구가 나를 친구 삭제 했을 때 
void FGsBuddyHandler::RecvBuddyDeletedNotify(IN UserDBId inUserDBId)
{
	GSBuddy()->DeleteBuddy(inUserDBId);
	UpdateUIPopUpBuddy();
}

// UIManager 선 셋팅
TWeakObjectPtr<class UGsUIManager> FGsBuddyHandler::GetUIManager()
{
	if (nullptr == _uiManager)
	{
		_uiManager = GUI();
	}

	return _uiManager;
}

void FGsBuddyHandler::UpdateUIPopUpBuddy()
{
	if (_uiPopupBuddy.IsValid())
	{
		_uiPopupBuddy->InvalidateAll();
	}
}

void FGsBuddyHandler::BuddyErrorMsg(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamBuddyError* param = InParam->Cast<const FGsUIMsgParamBuddyError>();

	FText findText;
	
	switch (param->_result)
	{
	case PD::Result::BUDDY_ERROR_INVALID_NAME_LENGTH:
		// 친구 - 유효하지 않는 이름 길이
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker3"), findText);
		break;
	case PD::Result::BUDDY_ERROR_NOT_FOUND_USER: // 친구 - 찾을 수 없는 유저	
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker3"), findText);
		break;
	case PD::Result::BUDDY_ERROR_SEND_INVITATION_FULL:
		// 친구 초대 실패 - 송신 자의 최대 친구 수를 초과함
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker5"), findText);
		break;
	case PD::Result::BUDDY_ERROR_BUDDY_REACHED_MAX_BUDDY:
		// 친구 초대/수락 실패 - 최대 친구 수를 초과함
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker7"), findText);
		break;
	case PD::Result::BUDDY_ERROR_TARGET_RECV_INVITATION_FULL:
		// 친구 초대 실패 - 수신자의 최대 초대 받기 수를 초과함
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker10"), findText);
		break;
	case PD::Result::BUDDY_ERROR_REACHED_MAX_BUDDY:
		// 친구 수락 실패 - 최대 친구 수를 초과함
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker6"), findText);
		break;	
	case PD::Result::BUDDY_ERROR_REQUEST_MY_SELF:
		// 친구 - 자신을 요청
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker1"), findText);
		break;
	case PD::Result::BUDDY_ERROR_ALREADY_BUDDY:
		// 친구 초대/수락 실패 - 이미 등록된 친구
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker2"), findText);
		break;
	case PD::Result::BUDDY_ERROR_ALREADY_INVITATION_RECV:
		// 친구 초대 실패 - 수신자가 송신자를 이미 초대함
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker9"), findText);
		break;
	case PD::Result::BUDDY_ERROR_ALREADY_INVITATION_SENT:
		// 친구 초대 실패 - 송신자가 수신자를 이미 초대함
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker8"), findText);
		break;
	case PD::Result::BUDDY_ERROR_INVALID_INVITATION_SAME_ACCOUNT:
		// 동일 계정 친구 초대 실패
		FText::FindText(TEXT("FriendText"), TEXT("FriendFailureTicker11"), findText);
		break;
	case PD::Result::BUDDY_ERROR_DELETE_RECV_INVITATION:
	case PD::Result::BUDDY_ERROR_DELETE_SEND_INVITATION:
	case PD::Result::BUDDY_ERROR_INVALID_INVITATION:
	case PD::Result::BUDDY_ERROR_INVALID_USER_ID:
	case PD::Result::BUDDY_ERROR_DB_OPERATION_ERROR:
	case PD::Result::BUDDY_ERROR_NOT_BUDDY:
		// 실패 시 다시 목록 요청
		//FGsNetSendService::SendBuddyList_v2();
		break;
	default:
		break;
	}

	if (findText.IsEmpty())
	{
		FText errorText;
		// NetText 테이블에서 사용할 티커값 조회
		FString key = FString::Format(TEXT("{0}"), { PD::ResultEnumToDisplayKey(param->_result) });
		FText::FindText(TEXT("NetText"), key, errorText);

		if (errorText.IsEmpty())
		{
#if WITH_EDITOR
			GSLOG(Log, TEXT("ACK_BUDDY_ACCEPT Failed, Result = %d"), (int)param->_result);
			return FGsUIHelper::PopupNetError(param->_result);
#endif
		}

		FGsUIHelper::TrayMessageTicker(errorText);
	}
	else
	{
		FGsUIHelper::TrayMessageTicker(findText);
	}
}