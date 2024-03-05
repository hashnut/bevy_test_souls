
#include "GsPartyDungeonState.h"

#include "../Party/GsPartyMemberInfo.h"
#include "../Dungeon/GsDungeonHelper.h"
#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGame/GsPartyManager.h"
#include "../Management/ScopeGame/GsGameObjectManager.h"

#include "../Message/MessageParam/GsMessageParam.h"
#include "../Message/GsMessageContents.h"
#include "../Message/GsMessageGameObject.h"
#include "../Message/GsMessageContentDungeon.h"

#include "GameFramework/PlayerController.h"
#include "../ActorEx/GsCharacterBase.h"
#include "../GameObject/HUD/GsHUDLocalPlayer.h"
#include "../GameObject/Define/GsGameObjectDefine.h"
#include "../GameObject/ObjectClass/GsGameObjectBase.h"

#include "../UI/UILib/Base/GsUIWidgetBase.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIContent/HUD/GsUIHUDSideTab.h"
#include "../UI/UIContent/Popup/GsUIPopupOK.h"
#include "../UI/UIContent/Popup/GsUIPopupYesNoTwoMessage.h"

#include "../Net/GsNetSendService.h"



void FGsPartyDungeonState::Initialize()
{
	FGsDungeonStateBase::Initialize();

	_helpseekingCount = 0;
	_helpseekingState = EDungeonHelpseekingState::ENone;

	_applyGroupId = INVALID_DUNGEON_GROUP_ID;
	_applyDifficulty = DungeonDifficulty::NONE;
}

void FGsPartyDungeonState::Finalize()
{
	_helpseekingCount = 0;
	_helpseekingState = EDungeonHelpseekingState::ENone;

	_applyGroupId = INVALID_DUNGEON_GROUP_ID;
	_applyDifficulty = DungeonDifficulty::NONE;

	FGsDungeonStateBase::Finalize();
}

bool FGsPartyDungeonState::Enter(MapId InMapId)
{
	if (false == FGsDungeonStateBase::Enter(InMapId))
	{
		return false;
	}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_INFO_UPDATE, nullptr);

	return true;
}

void FGsPartyDungeonState::Exit()
{
	_helpseekingCount = 0;
	_helpseekingState = EDungeonHelpseekingState::ENone;

	_applyGroupId = INVALID_DUNGEON_GROUP_ID;
	_applyDifficulty = DungeonDifficulty::NONE;

	// 2023/3/16 PKT - 파티 던전에서 나갈 때 이벤트로 받은 메세지를 모두 닫는다.
	IGsMessageParam param;
	GMessage()->GetEvent().SendMessage(MessageContentEvent::ALL_CLEAR_EVENT_ACTION_IMMEDIATE, param);

	FGsDungeonStateBase::Exit();
}

void FGsPartyDungeonState::LocalPlayerDie()
{
	// 2022/12/13 PKT - 던전 입장 전 준비 완료 상태에서 죽으면 준비 완료 상태를 해제 한다.
	if (EDungeonProgressState::EReady == _progressState)
	{
		// 2022/07/28 PKT - 던전 입장 전 준비 상태를 해지 한다.
		FGsNetSendService::SendPartyDungeonReady(false);
	}
}

FGsDungeonStateBase::HUDTimeMode FGsPartyDungeonState::GetHUDTimeMode() const
{	// 2023/3/3 PKT - 파티 던전에서는 HUD Time(Dungeon에서 사용하는)을 사용하지 않는다.
	return HUDTimeMode::Hidden;
}

bool FGsPartyDungeonState::IsExitNotifyMessage(FText& InMessage1, FText& InMessage2)
{
	// 2022/07/27 PKT - "협동 파티에서 탈퇴합니다."
	FText::FindText(TEXT("PartyDungeonText"), TEXT("PopUp_DungeonOut"), InMessage1);
	// 2022/08/03 PKT - "던전에서 나가시겠습니까?"
	FText::FindText(TEXT("PartyDungeonText"), TEXT("PopUp_DungeonOut2"), InMessage2);

	return true;
}

void FGsPartyDungeonState::RequestHelpseeking()
{
	const static int32 FULL_PARTY_COUNT = 4;
	if (FULL_PARTY_COUNT <= GSParty()->GetPartyMemberCount())
	{
		FText message;
		FGsDungeonHelper::FindText(TEXT("UI_Notice_Party_Full_Message"), message);
		FGsUIHelper::TrayMessageTicker(message);

		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupYesNoTwoMessage"));
	if (!widget.IsValid())
	{
		GSLOG(Error, TEXT("!widget.IsValid()"));
		return;
	}

	UGsUIPopupYesNoTwoMessage* popup = (widget.IsValid()) ? Cast<UGsUIPopupYesNoTwoMessage>(widget.Get()) : nullptr;
	if (nullptr == popup)
	{
		GSLOG(Error, TEXT("Invalid popup"));
		return;
	}

	FText message1;
	FGsDungeonHelper::FindText(TEXT("PartyDungeon_PopUp_CallHelp"), message1);

	FText message2 = FText::GetEmpty();

	TFunction<void(bool)> InCallback = [](bool InIsOk)
	{
		if (false == InIsOk)
		{
			return;
		}
		FGsNetSendService::SendPartyDungeonAskHelp();
	};

	popup->SetData(message1, message2, InCallback);
}

void FGsPartyDungeonState::ClearData()
{
	_progressState = EDungeonProgressState::ENone;
	_applyGroupId = INVALID_DUNGEON_GROUP_ID;
	_applyDifficulty = DungeonDifficulty::NONE;

	// 2022/06/14 PKT - Party Info Update
	//GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_INFO_UPDATE, nullptr);
	
	// 2022/06/14 PKT - 매칭 취소에 대한 Window 갱신
	GMessage()->GetDungeon().SendMessage(MessageContentDungeon::INVALIDATE_DATA, nullptr);
}

void FGsPartyDungeonState::SetStartPlay(const FDateTime& InStartTime, const FDateTime& InEndTime)
{

}

void FGsPartyDungeonState::SetJoin(EDungeonProgressState InProgressState, DungeonGroupId InDungeonGroupId, DungeonDifficulty InDifficulty)
{
	_progressState = InProgressState;
	_applyGroupId = InDungeonGroupId;
	_applyDifficulty = InDifficulty;
	
	/**
	 * UI Event
	 */
	 // 2022/11/02 PKT - 1. 열려 있는 던전 Window close
	GMessage()->GetContents().SendMessage(MessageContents::CLOSE_WINDOW_DUNGEON);

	// 2022/11/02 PKT - 2. Close Quick Menu
	GMessage()->GetContents().SendMessage(MessageContents::CLOSE_QUICK_MENU);

	// 2022/11/02 PKT - 3. HUD Reset
	GUI()->SetHUDMode(EGsUIHUDMode::NORMAL);	

	// 2022/06/14 PKT - Party Info Update
	GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_INFO_UPDATE, nullptr);
}

void FGsPartyDungeonState::SetHelpseekingInfo(int32 InCount, bool InIsEnable /* = true */)
{
	if (EDungeonHelpseekingState::EInActive != _helpseekingState)
	{	
		_helpseekingState = InIsEnable ? EDungeonHelpseekingState::EActive : EDungeonHelpseekingState::EWait;
	}

	_helpseekingCount = FMath::Max(InCount, 0);

	if (0 >= _helpseekingCount)
	{
		_helpseekingState = EDungeonHelpseekingState::EInActive;
	}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_INFO_UPDATE, nullptr);
}

void FGsPartyDungeonState::SetHelpseekingEnable(bool InIsEnable)
{
	//_helpseekingEnable = InIsEnable;

	////if (GSParty()->AmILeader())
	//{	// 2022/06/29 PKT - 리더일때만 UI 갱신
	//	GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_INFO_UPDATE, nullptr);
	//}
}

void FGsPartyDungeonState::SetHelpseekingLock(bool InIsLock)
{
	_helpseekingState = EDungeonHelpseekingState::EInActive;

	// 2022/06/19 PKT - 파티 HUD 업데이트
	//if (GSParty()->AmILeader())
	{	// 2022/06/29 PKT - 리더일때만 UI 갱신
		GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_INFO_UPDATE, nullptr);
	}
}

void FGsPartyDungeonState::NotifyPartyKickReason()
{
	if (GSParty()->AmILeader())
	{
		// 2022/06/07 PKT - 안내 팝업창을 띄운다.
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupOK"));
		if (!widget.IsValid())
		{
			GSLOG(Error, TEXT("!widget.IsValid()"));
			return;
		}

		if (UGsUIPopupOK* popup = Cast<UGsUIPopupOK>(widget.Get()))
		{	// 2022/06/07 PKT - "모든 파티원이 준비 완료 상태 중 던전 입장을 지연 시켜 파티에서 자동 추방되었습니다."
			FText message;
			FText::FindText(TEXT("PartyDungeonText"), TEXT("PopUp_DelayKickOut"), message);

			popup->SetData(message);
		}
	}
	
	ClearData();

	//GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_INFO_UPDATE, nullptr);
}

void FGsPartyDungeonState::PartyMathcingBroken()
{
	ClearData();

	GMessage()->GetGameObject().SendMessage(MessageGameObject::PARTY_HUD_INFO_UPDATE, nullptr);
}	