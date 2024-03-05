#include "GsPartyFunc.h"

#include "UTIL/GsText.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"

#include "Management/ScopeGame/GsPartyManager.h"

#include "ContentsServer/GsContentsServerHelper.h"

#include "DataSchema/GameObject/GsSchemaWeaponUIInfo.h"

#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedDefine.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendService.h"


// 초대할 사람 이름 체크
bool FGsPartyFunc::CheckPartyInviteeName(const FString& In_name)
{
	// 이름 체크
	if (CheckPartyInviteeNameLength(In_name) == false)
	{
		return false;
	}

	// 파티원인가
	if (GSParty()->IsInPartyName(In_name) == true)
	{
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker3"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return false;
	}
	// 초대한 사람 이름인가
	if (GSParty()->IsPartyInviteeName(In_name) == true)
	{
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker1"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return false;
	}

	return true;
}
// 초대할 사람 밴 대상인가
bool FGsPartyFunc::CheckPartyInviteBan(const FString& In_name)
{
	if (GSParty()->IsInviteBanTarget(In_name) == true)
	{
		FText findText;
		// 같은 대상을 계속 초대할 수 없습니다.
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker6"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return false;
	}

	return true;
}

// 초대할 사람 이름 글자 크기 체크
bool FGsPartyFunc::CheckPartyInviteeNameLength(const FString& In_name)
{
	int convertLen = FGsTextUtil::GetStringLengthNameType(In_name);

	// 최대 자수 넘음
	if (convertLen > MAX_CHAR_NAME_LEN)
	{
		// 올바른 이름을 입력해주세요
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker16"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return false;
	}
	// 최소 자수 보다 적거나 같음
	else if (convertLen < MIN_CHAR_NAME_LEN)
	{
		// 올바른 이름을 입력해주세요
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker16"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return false;
	}

	return true;
}

// 초대 응답 패킷 result 티커 출력
bool FGsPartyFunc::ShowTickerPartyInviteError(PD::Result In_result)
{
	switch (In_result)
	{
		// 파티 초대 대기중인 유저입니다
	case PD::Result::PARTY_ERROR_ALREADY_PARTY_INVITED:
	{
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker1"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	//https://jira.com2us.com/jira/browse/C2URWQ-40
	// 존재하지 않는 유저입니다.
	case PD::Result::PARTY_ERROR_NOT_FOUND_USER:
	{
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker2"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	//https://jira.com2us.com/jira/browse/C2URWQ-40
	// 파티에 초대할 수 없는 유저입니다. 
	// https://jira.com2us.com/jira/browse/C2URWQ-4479
	case PD::Result::PARTY_ERROR_NOT_LOGIN_USER:
	case PD::Result::PARTY_ERROR_NOT_INVITABLE_USER:
	{
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker4"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	// 파티에 속해 있는 유저입니다.
	case PD::Result::PARTY_ERROR_ALREADY_PARTY_MEMBER:
	{
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker3"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	// 파티가 가득참
	case PD::Result::PARTY_ERROR_MEMBER_FULL:
	{
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("Party_Invite_Fail_01"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	// 접속중이지 않거나 존재하지 않는 캐릭터입니다.
	case PD::Result::PARTY_ERROR_INVALID_HOME_WORLD_ID:
	{
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PARTY_ERROR_INVALID_HOME_WORLD_ID"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	}

	return false;
}


// weapon ui data 접근
const FGsSchemaWeaponUIInfo* FGsPartyFunc::GetWeaponUIInfo(CreatureWeaponType In_weaponType)
{
	const UGsTableWeaponUIInfo* tbl = Cast<UGsTableWeaponUIInfo>(FGsSchemaWeaponUIInfo::GetStaticTable());
	if (nullptr == tbl)
	{
		return nullptr;
	}

	const FGsSchemaWeaponUIInfo* findTbl = nullptr;
	if (false == tbl->FindRowById(In_weaponType, findTbl))
	{
		return nullptr;
	}
	return findTbl;
}

// 파티 나감 팝업 열기
void FGsPartyFunc::OpenLeavePopup()
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupYesNo"));
	if (false == widget.IsValid())
	{
		return;
	}

	UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get());
	if (popup)
	{
		// TEXT: 정말로 탈퇴하시겠습니까
		FText textDesc;
		FText::FindText(TEXT("PartyText"), TEXT("PartyQuit_desc"), textDesc);

		popup->SetData(textDesc, [](bool bYes)
		{
			if (bYes)
			{
				// 파티 나가기 패킷 전송					
				FGsNetSendService::SendPartyLeave();
			}
		});
	}
}

void FGsPartyFunc::OpenLeavePopup(TFunction<void(void)> Infunc)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupYesNo"));
	if (false == widget.IsValid())
	{
		return;
	}

	UGsUIPopupYesNo* popup = Cast<UGsUIPopupYesNo>(widget.Get());
	if (popup)
	{
		// TEXT: 정말로 탈퇴하시겠습니까
		FText textDesc;
		FText::FindText(TEXT("PartyText"), TEXT("PartyQuit_desc"), textDesc);

		popup->SetData(textDesc, [Infunc](bool bYes)
			{
				if (bYes)
				{
					// 파티 나가기 패킷 전송					
					FGsNetSendService::SendPartyLeave();
					
					if (Infunc)
					{
						Infunc();
					}
				}
			});
	}
}

// 추방 티커 출력
void FGsPartyFunc::ShowTickerKicked(const FString& In_name)
{
	// {0}님이 파티에서 추방 되었습니다.
	FText findText;
	FText::FindText(TEXT("PartyText"), TEXT("PartyTicker11"), findText);

	FText resultText = FText::Format(findText, FText::FromString(In_name));

	FGsUIHelper::AddChatMessageSystem(resultText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, resultText);
}

void FGsPartyFunc::ShowTickerInviteFailed()
{
	FText findText;
	// 파티초대를 거절한 유저입니다.
	FText::FindText(TEXT("PartyText"), TEXT("PartyTicker5"), findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}

// drop ownership change ticker
void FGsPartyFunc::ShowTickerChangeDropOwnershipType(PartyDropOwnershipType In_type)
{
	FText findText;
	switch (In_type)
	{
	case PartyDropOwnershipType::RANDOM:
	{
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker13"), findText);
	}
	break;
	case PartyDropOwnershipType::SEQUENCE:
	{
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker14"), findText);
	}
	break;
	case PartyDropOwnershipType::FREE:
	{
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker15"), findText);
	}
	break;
	default:
		return;
	}
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}

void FGsPartyFunc::ShowChatLeaderChange(FString In_newLeader)
{
	FText findText;
	FText::FindText(TEXT("PartyText"), TEXT("PartyTicker12"), findText);

	FText resultText = FText::Format(findText, FText::FromString(In_newLeader));

	FGsUIHelper::AddChatMessageSystem(resultText);
}

void FGsPartyFunc::ShowTickerPartyMemberTarget()
{
	FText findText;
	// 아군은 타겟 공유 대상으로 지정할 수 없습니다.
	FText::FindText(TEXT("PartyText"), TEXT("PartyTarget_Cannot_Ally"), findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}
void FGsPartyFunc::ShowTickerTryShareByPartyMember()
{
	FText findText;
	// 파티장만 타겟 공유 대상을 지정할 수 있습니다.
	FText::FindText(TEXT("PartyText"), TEXT("PartyTarget_Only_Leader"), findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}

void FGsPartyFunc::InviteParty(const FString& In_inviteeName, WorldId In_homeWorldId)
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
		return;
	}

	if (!partyManager->AmILeader())
	{
		// 파티에 초대할 수 없는 상태 입니다
		FText findText;
		FText::FindText(TEXT("PartyText"), TEXT("PartyTicker10"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return;
	}

	if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::PARTY_NO_PARTY_INVITES, true))
	{
		return;
	}

	// 1. 이름 길이 체크
	// 2. 파티원인가
	// 3. 초대한 사람 이름인가
	if (false == CheckPartyInviteeName(In_inviteeName))
	{
		return;
	}
	// 1. 밴 대상인가
	if (false == CheckPartyInviteBan(In_inviteeName))
	{
		return;
	}

	FGsNetSendService::SendPartyInvite(In_inviteeName, In_homeWorldId);
}
// show error ticker: ack party leader change
bool FGsPartyFunc::ShowTickerPartyChangeLeader(PD::Result In_result)
{
	switch (In_result)
	{
		// 이미 탈퇴한 파티원입니다.
	case PD::Result::PARTY_ERROR_NOT_FOUND_USER:
	{
		FText findText;
		FText::FindText(TEXT("NetText"), TEXT("PARTY_ERROR_NOT_FOUND_USER"), findText);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
		return true;
	}
	}

	return false;
}

// 파티원 이름 구하기(prefix 붙일지 말지 현재 맵에따라 처리)
FString FGsPartyFunc::GetPartyMemberName(const FString& In_name, WorldId In_homeWorldId)
{
	// 인터서버 필드면 prefix 붙인다	
	FString newName = In_name;
	if (true == FGsContentsServerHelper::IsLocatedOnInterServer())
	{
		FText textName = FText::FromString(In_name);
		newName = FGsTextUtil::CombineUserAndPrefix(textName, In_homeWorldId, true);
	}

	return newName;
}