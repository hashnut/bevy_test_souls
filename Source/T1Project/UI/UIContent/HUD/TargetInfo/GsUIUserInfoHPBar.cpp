// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIUserInfoHPBar.h"

#include "ContentsServer/GsContentsServerHelper.h"

#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/ObjectClass/GsGameObjectRemotePlayer.h"
#include "GameObject/Data/GsGameObjectDataRemotePlayer.h"

#include "HAL/PlatformApplicationMisc.h"

#include "Management/ScopeGame/GsPartyManager.h"
#include "Management/ScopeGame/GsArenaManager.h"
#include "Management/ScopeGame/GsPKBookManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsPersonalTradingManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Item/GsItemManager.h"

#include "Management/GsMessageHolder.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "Net/GsNetSendService.h"

#include "Party/GsPartyFunc.h"

#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupChat.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Net/GsNetSendServiceWorld.h"
#include "../UTIL/GsText.h"



void UGsUIUserInfoHPBar::BeginDestroy()
{
	if (_btnUserProfile)
	{
		_btnUserProfile->OnClicked.RemoveDynamic(this, &UGsUIUserInfoHPBar::OnClickUserProfile);
	}
	if (_btnInviteParty)
	{
		_btnInviteParty->OnClicked.RemoveDynamic(this, &UGsUIUserInfoHPBar::OnClickInviteParty);
	}
	if (_btnArena)
	{
		_btnArena->OnClicked.RemoveDynamic(this, &UGsUIUserInfoHPBar::OnClickArena);
	}
	if (_btnWhisper)
	{
		_btnWhisper->OnClicked.RemoveDynamic(this, &UGsUIUserInfoHPBar::OnClickWhisper);
	}
	if (_btnNameCopy)
	{
		_btnNameCopy->OnClicked.RemoveDynamic(this, &UGsUIUserInfoHPBar::OnClickCopy);
	}
	if (_btnTrade)
	{
		_btnTrade->OnClicked.RemoveDynamic(this, &UGsUIUserInfoHPBar::OnClickPersonalTrading);
	}

	if (_btnAddBoundaryTarget)
	{
		_btnAddBoundaryTarget->OnClicked.RemoveDynamic(this, &UGsUIUserInfoHPBar::OnClickBoundaryTarget);
	}

	if (_btnRemoveBoundaryTarget)
	{
		_btnRemoveBoundaryTarget->OnClicked.RemoveDynamic(this, &UGsUIUserInfoHPBar::OnClickBoundaryTarget);
	}
	
	Super::BeginDestroy();
}

void UGsUIUserInfoHPBar::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnUserProfile->OnClicked.AddDynamic(this, &UGsUIUserInfoHPBar::OnClickUserProfile);
	_btnInviteParty->OnClicked.AddDynamic(this, &UGsUIUserInfoHPBar::OnClickInviteParty);
	_btnArena->OnClicked.AddDynamic(this, &UGsUIUserInfoHPBar::OnClickArena);
	_btnWhisper->OnClicked.AddDynamic(this, &UGsUIUserInfoHPBar::OnClickWhisper);
	_btnNameCopy->OnClicked.AddDynamic(this, &UGsUIUserInfoHPBar::OnClickCopy);
	_btnAddBoundaryTarget->OnClicked.AddDynamic(this, &UGsUIUserInfoHPBar::OnClickBoundaryTarget);
	_btnRemoveBoundaryTarget->OnClicked.AddDynamic(this, &UGsUIUserInfoHPBar::OnClickBoundaryTarget);
	_btnTrade->OnClicked.AddDynamic(this, &UGsUIUserInfoHPBar::OnClickPersonalTrading);
}

void UGsUIUserInfoHPBar::NativeConstruct()
{
	Super::NativeConstruct();

	FGsMessageHolder* msgHolder = GMessage();
	_msgHandlerUserContextList.Emplace(msgHolder->GetUserContext().AddUObject(MessageContentUserContext::VALID_USER,
		this, &UGsUIUserInfoHPBar::OnReceiveUserInfo));

	// 2023/1/17 PKT - 경계 대상 추가에 대한 버튼 갱신
	_PKBookMessageHandler = GMessage()->GetPKBook().AddUObject(MessageContentPKBook::UI_INVALID_DETECTIONTARGET_LIST, this, &UGsUIUserInfoHPBar::InvalidateBoundaryTarget);
}

void UGsUIUserInfoHPBar::NativeDestruct()
{
	FGsMessageHolder* msgHolder = GMessage();
	for (TPair<MessageContentUserContext, FDelegateHandle>& handler : _msgHandlerUserContextList)
	{
		msgHolder->GetUserContext().Remove(handler);
	}
	_msgHandlerUserContextList.Empty();

	// 2023/1/17 PKT - 메세지 해제
	GMessage()->GetPKBook().Remove(_PKBookMessageHandler);

	Super::NativeDestruct();
}

void UGsUIUserInfoHPBar::InvalidateButton()
{
	const UGsGameObjectRemotePlayer* targetUser = FindTargetGameObject();
	const TMap<EGsContextContentsMenu, bool> activationList = GetActivationFeature(&_userContextInfo, targetUser);

	bool isArenaArea = activationList.Contains(EGsContextContentsMenu::ARENA);
	_btnArena->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::ARENA));
	_btnInviteParty->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::INVITE_PARTY));

	_btnArena->SetVisibility(isArenaArea ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	_btnInviteParty->SetVisibility(false == isArenaArea ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	_btnUserProfile->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::USER_PROFILE));
	_btnWhisper->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::WHISPER));
	_btnNameCopy->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::NAME_COPY));
	_btnTrade->SetIsEnabled(activationList.Contains(EGsContextContentsMenu::PERSONAL_TRADE));

	bool isActivationBoundaryTarget = activationList.Contains(EGsContextContentsMenu::USER_BOUNDARYTARGET);
	_btnAddBoundaryTarget->SetIsEnabled(isActivationBoundaryTarget);
	_btnRemoveBoundaryTarget->SetIsEnabled(isActivationBoundaryTarget);
	
	if (isActivationBoundaryTarget)
	{
		_switcherBoundaryTarget->SetActiveWidgetIndex(StaticCast<int32>(activationList[EGsContextContentsMenu::USER_BOUNDARYTARGET]));
	}
}

void UGsUIUserInfoHPBar::SetData(UGsGameObjectCreature* InTarget, const FString& InUserName)
{
	_isClickedArena = false;

	// 기존 정보 초기화
	FGsUserContextInfo defaultInfo;
	_userContextInfo = defaultInfo;

	FGsGameObjectDataRemotePlayer* remoteData = InTarget->GetCastData<FGsGameObjectDataRemotePlayer>();
	if (nullptr == remoteData)
		return;

	bool isSameWorldUser = false;
	if (FGsGameDataManager* gameDataMgr = GGameData())
	{
		WorldId myHomeWorldId = gameDataMgr->GetUserData()->mHomeWorldId;
		WorldId targetHomeWorldId = remoteData->GetHomeWorldId();

		isSameWorldUser = (myHomeWorldId == targetHomeWorldId);
	}

	_userContextInfo.SetUserDBId(remoteData->GetUserDBID());
	_userContextInfo.SetUserName(remoteData->GetName());
	_userContextInfo.SetAccountGrade(remoteData->GetAccountGrade());
	_userContextInfo.SetIsSameWorldUser(isSameWorldUser);
	_userContextInfo.SetHomeWorldId(remoteData->GetHomeWorldId());
	_userContextInfo.SetUserName(InUserName);

	_gameId = remoteData->GetGameId();

	InvalidateButton();
}

void UGsUIUserInfoHPBar::OnClickUserProfile()
{
	SendUserProfile(_userContextInfo.GetUserDBId());
}

void UGsUIUserInfoHPBar::OnClickInviteParty()
{
	SendInviteParty(_userContextInfo.GetUserName(), _userContextInfo.GetHomeWorldId());
}

void UGsUIUserInfoHPBar::OnClickArena()
{
	if (FGsArenaManager* arenaManager = GSArena())
	{
		if (FGsArenaHandler* arenaHandler = arenaManager->GetArenaHandler())
		{
			if (arenaHandler->CheckArenaAvailability(true))
			{
				_isClickedArena = SendUserContextRefresh(_userContextInfo.GetUserName(), _userContextInfo.GetHomeWorldId());
			}
		}
	}
}

void UGsUIUserInfoHPBar::OnClickWhisper()
{
	SendWhisper(_userContextInfo.GetUserDBId(), _userContextInfo.GetUserName(), _userContextInfo.GetHomeWorldId());
}

void UGsUIUserInfoHPBar::OnClickCopy()
{
	if (true == SendNameCopy(FText::FromString(_userContextInfo.GetUserName()), _userContextInfo.GetHomeWorldId()))
	{
		// 유저 아이디를 클립보드에 복사했습니다.
		ShowTicker(TEXT("ContextUIText"), TEXT("contextUIText1"));
	}
}

void UGsUIUserInfoHPBar::OnClickBoundaryTarget()
{
	bool isAdd = SWITCHER_BOUNDARYTARGET_INDEX::Add == StaticCast<SWITCHER_BOUNDARYTARGET_INDEX>(_switcherBoundaryTarget->GetActiveWidgetIndex());
	SendBoundaryTarget(isAdd, _userContextInfo.GetUserDBId(), _userContextInfo.GetHomeWorldId());
}

void UGsUIUserInfoHPBar::OnClickPersonalTrading()
{
	SendPersonalTrading(_gameId, _userContextInfo.GetUserName());
}

void UGsUIUserInfoHPBar::OnReceiveUserInfo(const IGsMessageParam* param)
{
	if (nullptr == param)
		return;

	if (GUI()->IsActiveWidget(TEXT("PopupChatUserInfo")))
		return;

	if (const FGsUIMsgParamContextUserInfo* info = param->Cast<const FGsUIMsgParamContextUserInfo>())
	{
		FString userName = _userContextInfo.GetUserName();
		if (_userContextInfo.GetUserDBId() != info->_userContextInfo.GetUserDBId())
			return;

		_userContextInfo = info->_userContextInfo;
		_userContextInfo.SetUserName(userName);
		// 2023/9/13 PKT - 갱신 결과에 따라 화면을 한번 갱신 한다.
		InvalidateButton();

		if (_isClickedArena)
		{
			OnClickedArena();
			_isClickedArena = false;
		}
	}
}

void UGsUIUserInfoHPBar::InvalidateBoundaryTarget(const struct IGsMessageParam* InParam)
{
	InvalidateButton();
}

void UGsUIUserInfoHPBar::OnClickedArena()
{
	if (false == SendDuelInvite(_userContextInfo.GetUserDBId(), _userContextInfo.GetUserLevel(), _userContextInfo.GetUserName(), _userContextInfo.IsVisible()))
	{
		ShowTicker(TEXT("ArenaText"), TEXT("NotInArenaReady1"));
	}
}

void UGsUIUserInfoHPBar::ShowTicker(const FTextKey& InNamespace, const FTextKey& InKey)
{
	FText findText;
	FText::FindText(InNamespace, InKey, findText);
	FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, findText);
}

const UGsGameObjectRemotePlayer* UGsUIUserInfoHPBar::FindTargetGameObject()
{
	if (UGsGameObjectManager* gameObjectManager = GSGameObject())
	{
		if (UGsGameObjectBase* findObject = gameObjectManager->FindObject(EGsGameObjectType::RemotePlayer, _gameId))
		{
			UGsGameObjectRemotePlayer* remoteObject = findObject->CastGameObject<UGsGameObjectRemotePlayer>();
			return remoteObject;
		}
	}
	return nullptr;
}
