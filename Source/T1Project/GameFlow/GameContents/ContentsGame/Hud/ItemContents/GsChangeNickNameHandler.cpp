// Fill out your copyright notice in the Description page of Project Settings.


#include "GsChangeNickNameHandler.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "UI/UIContent/Popup/GsUIPopupChangeNickName.h"
#include "UI/UIContent/Popup/GsUIPopupChangeNickNameConfirm.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Net/GsNetSendService.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "Input/GsInputEventMsgBase.h"
#include "DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


//#define TEST_NICKNAME

void FGsChangeNickNameHandler::Init()
{
	BindMessages();
}

void FGsChangeNickNameHandler::Close()
{
	UnbindMessages();

	_ticketItemDBId = INVALID_ITEM_DB_ID;
	_strTempNickName.Empty();
}

void FGsChangeNickNameHandler::BindMessages()
{
	FGsMessageHolder* msgHolder = GMessage();
	if (nullptr == msgHolder)
	{
		return;
	}
		
	MItemContents& msgItemContents = msgHolder->GetItemContents();
	_msgHandleItemContentsList.Emplace(msgItemContents.AddRaw(MessageContentItem::CHANGE_NICKNAME_USE_TICKET,
		this, &FGsChangeNickNameHandler::OnUseTicket));
	_msgHandleItemContentsList.Emplace(msgItemContents.AddRaw(MessageContentItem::CHANGE_NICKNAME_CHECK_REQUEST,
		this, &FGsChangeNickNameHandler::OnRequestCheckNickName));
	_msgHandleItemContentsList.Emplace(msgItemContents.AddRaw(MessageContentItem::CHANGE_NICKNAME_CHECK_VALID,
		this, &FGsChangeNickNameHandler::OnCheckValidNickName));
	_msgHandleItemContentsList.Emplace(msgItemContents.AddRaw(MessageContentItem::CHANGE_NICKNAME_CONFIRM_REQUEST,
		this, &FGsChangeNickNameHandler::OnRequestConfirmNickName));
	_msgHandleItemContentsList.Emplace(msgItemContents.AddRaw(MessageContentItem::CHANGE_NICKNAME_CONFIRM,
		this, &FGsChangeNickNameHandler::OnConfirmNickName));
}

void FGsChangeNickNameHandler::UnbindMessages()
{
	FGsMessageHolder* msgHolder = GMessage();
	if (nullptr == msgHolder)
	{
		return;
	}

	MItemContents& msgItemContents = msgHolder->GetItemContents();
	for (MsgHandleItemContents& msgUIHandle : _msgHandleItemContentsList)
	{
		msgItemContents.Remove(msgUIHandle);
	}
	_msgHandleItemContentsList.Empty();
}

void FGsChangeNickNameHandler::OnUseTicket(const IGsMessageParam* InParam)
{
	const FGsPrimitiveUInt64* param = InParam->Cast<const FGsPrimitiveUInt64>();
	if (nullptr == param)
	{
		return;
	}

	if (INVALID_ITEM_DB_ID == param->_data)
	{
		return;
	}

	if (false == IsValidPlayerState())
	{
		return;
	}

	_ticketItemDBId = param->_data;	

	GUI()->Open(TEXT("PopupChangeNickName"));
}

void FGsChangeNickNameHandler::OnRequestCheckNickName(const IGsMessageParam* InParam)
{
	const FGsPrimitiveString* param = InParam->Cast<const FGsPrimitiveString>();
	if (nullptr == param)
	{
		return;
	}

	if (param->_data.IsEmpty())
	{
		return;
	}

	// 아이템 사용 후 자동이동 같은걸로 마을을 벗어나는 상황 체크
	if (false == IsValidPlayerState())
	{
		return;
	}

	_strTempNickName = param->_data;	

#ifdef TEST_NICKNAME
	GMessage()->GetItemContents().SendMessage(MessageContentItem::CHANGE_NICKNAME_CHECK_VALID, nullptr);
#else
	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("ChangeNickNameCheck")));
	FGsNetSendService::SendReqCheckNickNameChange(_strTempNickName);	
#endif
}

void FGsChangeNickNameHandler::OnCheckValidNickName(const IGsMessageParam*)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return;
	}

	// 이전 팝업 닫기
	uiMgr->CloseByKeyName(TEXT("PopupChangeNickName"));

	// 저장된 아이템, 텍스트 없으면 멈춤
	if (INVALID_ITEM_DB_ID == _ticketItemDBId ||
		_strTempNickName.IsEmpty())
	{
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("PopupChangeNickNameConfirm"));
	if (widget.IsValid())
	{
		if (UGsUIPopupChangeNickNameConfirm* popup = Cast<UGsUIPopupChangeNickNameConfirm>(widget.Get()))
		{
			FString currNickName;
			if (const FGsNetUserData* userData = GGameData()->GetUserData())
			{
				currNickName = userData->mUserName;
			}

			popup->SetData(currNickName, _strTempNickName);
		}
	}
}

void FGsChangeNickNameHandler::OnRequestConfirmNickName(const IGsMessageParam*)
{
	// 저장된 아이템, 텍스트 없으면 멈춤
	if (INVALID_ITEM_DB_ID == _ticketItemDBId ||
		_strTempNickName.IsEmpty())
	{
		return;
	}

	// 아이템 사용 후 자동이동 같은걸로 마을을 벗어나는 상황 체크
	if (false == IsValidPlayerState())
	{
		return;
	}

	if (FGsMessageHolder* msgMgr = GMessage())
	{
		FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_COMMON_POPUP, true);
		msgMgr->GetInput().SendMessage(MessageInput::BlockInput, msg);
	}

#ifdef TEST_NICKNAME
	GMessage()->GetItemContents().SendMessage(MessageContentItem::CHANGE_NICKNAME_CONFIRM, nullptr);	
#else
	FGsUIHelper::ShowBlockUIExplicit(FName(TEXT("ChangeNickNameConfirm")));
	FGsNetSendService::SendReqDecideNickNameChange(_strTempNickName, _ticketItemDBId);
#endif
}

void FGsChangeNickNameHandler::OnConfirmNickName(const IGsMessageParam* InParam)
{
	// TEXT: 캐릭터명 변경이 완료되었습니다. 캐릭터 명 변경을 적용하기 위해 타이틀 화면으로 이동합니다.
	FText textMsg;
	FText::FindText(TEXT("ItemUIText"), TEXT("ItemUI_NickNameChange_CompletePopup"), textMsg);

	FGsUIHelper::PopupSystemMsg(textMsg, []()
		{
			if (FGsMessageHolder* msgMgr = GMessage())
			{
				FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_COMMON_POPUP, false);
				msgMgr->GetInput().SendMessage(MessageInput::BlockInput, msg);
			}

			// 주의: MessageSystem::BACKTO_TITLE 은 게임을 나가겠냐는 팝업이 뜨고, 
			// BACKTO_LOGIN은 로그아웃이 제대로 안되는지 이상함
			GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
		});
}

bool FGsChangeNickNameHandler::IsValidPlayerState()
{
	// 마을 안인지 확인(2023/12/28 PKT - 그림자 전장 내 로비도 마을로 취급 사용 못하도록 수정)
	if (false == GFence()->IsPlayerInTownFence() || MapType::GUILD_WAR == GLevel()->GetCurrentMapType())
	{
		// TEXT: 해당 아이템은 마을 이외의 지역에서는 사용이 불가능합니다
		FText textMsg;
		FText::FindText(TEXT("ItemUIText"), TEXT("ItemUI_Notice_OutOfTown"), textMsg);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);

		return false;
	}

	return true;
}
