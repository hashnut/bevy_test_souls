#include "GsUIPopupChatUserInfo.h"

#include "Components/ListView.h"

#include "Guild/GsGuildHelper.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Popup/GsUIPopupBuddy.h"
#include "UI/UIContent/Window/Ranking/GsUIWindowRanking.h"
#include "UI/UIContent/HUD/GsUIHUDFrameMain.h"
#include "UI/UIContent/HUD/GsUIHUDParty.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "Net/GsNetSendService.h"
#include "Net/GsNetSendServiceWorld.h"

#include "Engine/Public/UnrealClient.h"


void UGsUIPopupChatUserInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_touchClose->OnClicked.AddDynamic(this, &UGsUIPopupChatUserInfo::OnTouchedExtraRange);
}

void UGsUIPopupChatUserInfo::NativeConstruct()
{
	FGsMessageHolder* msgHolder = GMessage();

	msgHolder->GetUI().SendMessage(MessageUI::OPEN_MIDDLE_SMALL_POPUP, nullptr);
	
	_msgHandlerUserContextList.Emplace(msgHolder->GetUserContext().AddUObject(MessageContentUserContext::VALID_USER,
		this, &UGsUIPopupChatUserInfo::OnReceiveUserInfo));
	_msgHandlerUserContextList.Emplace(msgHolder->GetUserContext().AddUObject(MessageContentUserContext::INVALID_USER,
		this, &UGsUIPopupChatUserInfo::OnReceiveInvalidUser));
	_msgHandlerUserContextList.Emplace(msgHolder->GetUserContext().AddUObject(MessageContentUserContext::INVALID_USER_OTHER_SERVER,
		this, &UGsUIPopupChatUserInfo::OnReceiveInvalidUserOtherServer));

	_msgHandlerGuild = msgHolder->GetGuild().AddUObject(MessageContentGuild::RECEIVE_OTHER_GUILD_DETAIL_INFO,
		this, &UGsUIPopupChatUserInfo::OnReceiveOtherGuildDetailInfo);

	_msgHandlerUIList.Emplace(msgHolder->GetUI().AddUObject(MessageUI::OPEN_MIDDLE_SMALL_POPUP,
		this, &UGsUIPopupChatUserInfo::OnOpenMiddleSmallPopup));

	_isHorizontal = true;

	_handleResize = FViewport::ViewportResizedEvent.AddUObject(this, &UGsUIPopupChatUserInfo::OnResizeViewport);

	Super::NativeConstruct();

	FGsUserContextInfo defaultInfo;
	_userContextInfo = defaultInfo;

	_centerUserInfo->SetVisibility(ESlateVisibility::Collapsed);
	_userInfoVertical->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIPopupChatUserInfo::NativeDestruct()
{
	GMessage()->GetUserContext().SendMessage(MessageContentUserContext::CLOSE, nullptr);

	FGsMessageHolder* msgHolder = GMessage();
	for (TPair<MessageContentUserContext, FDelegateHandle>& handler : _msgHandlerUserContextList)
	{
		msgHolder->GetUserContext().Remove(handler);
	}
	_msgHandlerUserContextList.Empty();

	msgHolder->GetGuild().Remove(_msgHandlerGuild);

	for (MsgUIHandle& handler : _msgHandlerUIList)
	{
		msgHolder->GetUI().Remove(handler);
	}
	_msgHandlerUIList.Empty();

	if (_handleResize.IsValid())
	{
		FViewport::ViewportResizedEvent.Remove(_handleResize);
	}

	Super::NativeDestruct();
}

void UGsUIPopupChatUserInfo::SetData(PopupInitData* initParam, bool InIsHorizontal)
{
	FGsGameDataManager* gameDataMgr = GGameData();
	if (nullptr == gameDataMgr)
		return;

	if (nullptr == initParam)
		return;

	_data = *initParam;
	_isHorizontal = InIsHorizontal;

	WorldId myHomeWorldId = gameDataMgr->GetUserData()->mHomeWorldId;

	if (_data._homeWorldId == INVALID_WORLD_ID)
	{
		_data._homeWorldId = myHomeWorldId;
	}
	
	bool isSameWorldUser = (myHomeWorldId == _data._homeWorldId);
	if (isSameWorldUser)
	{
		FGsNetSendService::SendReqUserContext(_data._nickName);
	}
	else
	{
		FGsNetSendServiceWorld::SendReqUserContextOtherServer(_data._nickName, _data._homeWorldId);
	}

	SetPosition(nullptr);
}

void UGsUIPopupChatUserInfo::SetPosition(UWidget* InTarget, UWidget* InArea /* = nullptr */)
{
	UWidget* target = InTarget;
	UWidget* area = InArea;
	UserInfoAnchor anchor = UserInfoAnchor::NONE;

	switch (_data._owner)
	{		
	case EGsUserContentsOwner::NONE:
	{
		target = nullptr;
		area = nullptr;
	}	break;
	case EGsUserContentsOwner::BUDDY_MEMBER:
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("PopupBuddy"));
		if (widget.IsValid())
		{
			TWeakObjectPtr<UGsUIPopupBuddy> popupBuddy = Cast<UGsUIPopupBuddy>(widget);
			if (popupBuddy.IsValid())
			{
				target = popupBuddy->GetUserInfoTargetWidget();
				anchor = UserInfoAnchor::TOP_RIGHT_SIDE;
			}
		}
	}	break;
	case EGsUserContentsOwner::PARTY_MEMBER:
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("HUDMain"));
		if (widget.IsValid())
		{
			if (UGsUIHUDFrameMain* hudMain = Cast<UGsUIHUDFrameMain>(widget.Get()))
			{
				if (UGsUIHUDParty* hudParty = hudMain->GetParty())
				{
					target = hudParty;
					anchor = UserInfoAnchor::TOP_RIGHT_SIDE;
				}
			}
		}
	}	break;
	case EGsUserContentsOwner::RANKING:
	{
		anchor = UserInfoAnchor::TOP_RIGHT_SIDE;
		_isDelayedOpen = true;
	}	break;
	case EGsUserContentsOwner::USER_CHEAT:
	case EGsUserContentsOwner::GUILD_MEMBER:
	{
		anchor = UserInfoAnchor::TOP_RIGHT_SIDE;
		_isDelayedOpen = true;
	}break;
	case EGsUserContentsOwner::USER_PROFILE:
	{
		anchor = UserInfoAnchor::TOP_RIGHT_UP;
	}break;
	default:
		break;
	}

	UGsUIUserInfo* userInfoUI = GetUserInfoUI();
	userInfoUI->SetTarget(target);
	userInfoUI->SetArea(area);
	userInfoUI->SetAnchor(anchor);
}

UGsUIUserInfo* UGsUIPopupChatUserInfo::GetUserInfoUI()
{
	return _isHorizontal ? _centerUserInfo : _userInfoVertical;
}

void UGsUIPopupChatUserInfo::OnReceiveUserInfo(const IGsMessageParam* param)
{
	if (param)
	{
		UGsUIUserInfo* userInfoUI = GetUserInfoUI();
		_centerUserInfo->SetVisibility(_isHorizontal ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		_userInfoVertical->SetVisibility(_isHorizontal ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

		if (userInfoUI->IsWaitingUserExist())
		{
			userInfoUI->ReceiveUserExist(true);
		}
		else
		{
			const FGsUIMsgParamContextUserInfo* info = param->Cast<const FGsUIMsgParamContextUserInfo>();

			_userContextInfo = info->_userContextInfo;
			_userContextInfo.SetUserName(_data._nickName);
			_data._uid = info->_userContextInfo.GetUserDBId();

			userInfoUI->SetData(_data._owner, _userContextInfo);
			userInfoUI->SetParent(this);
			userInfoUI->SetIsDelayedOpen(_isDelayedOpen);
			userInfoUI->SetReportContent(_data._chatText);
			_isDelayedOpen = false;
		}
	}
}

void UGsUIPopupChatUserInfo::OnReceiveInvalidUser(const IGsMessageParam* param)
{
	UGsUIUserInfo* userInfoUI = GetUserInfoUI();
	userInfoUI->ReceiveUserExist(false);

	if (_userContextInfo.GetUserDBId() == INVALID_USER_DB_ID)
	{
		Close();
	}
}

void UGsUIPopupChatUserInfo::OnReceiveInvalidUserOtherServer(const IGsMessageParam* param)
{
	// 찾을 수 없는 유저입니다.
	FText findText;
	FText::FindText(TEXT("ContextUIText"), TEXT("contextUIText6"), findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
	
	Close();
}

void UGsUIPopupChatUserInfo::OnReceiveOtherGuildDetailInfo(const IGsMessageParam* param)
{
	if (FGsGuildHelper::CanOpenGuildMainOther())
	{
		GUI()->Open(TEXT("PopupGuildMainOther"));
	}
}

void UGsUIPopupChatUserInfo::OnOpenMiddleSmallPopup(const IGsMessageParam* param)
{
	Close();
}

void UGsUIPopupChatUserInfo::OnResizeViewport(class FViewport* InViewport, uint32 InNum)
{
	Close();
}

void UGsUIPopupChatUserInfo::OnInputCancel()
{
	OnTouchedExtraRange();
}

void UGsUIPopupChatUserInfo::OnTouchedExtraRange()
{
	Close();
}