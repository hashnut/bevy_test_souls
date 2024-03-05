// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyLogin.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Net/GsNetSendService.h"
#include "Net/GsNetAutoLogin.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Option/GsGameUserSettings.h"

#include "UI/UIContent/Window/GsUIWindowLogin.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsLobbyConstant.h"

#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/GsMessageUI.h"
#include "Message/GsMessageStage.h"
#include "Message/MessageParam/GsStageMessageParam.h"


FGsStateLobbyLogin::FGsStateLobbyLogin() : FGsContentsLobbyBase(FGsContentsMode::InLobby::LOGIN)
{
}

FGsStateLobbyLogin::~FGsStateLobbyLogin()
{
}

void FGsStateLobbyLogin::Enter()
{
	FGsContentsLobbyBase::Enter();

	GSLOG(Log, TEXT("FGsStateLobbyLogin::Enter()"));

	// 메세지 등록
	RegisterMessages();

	// BSAM(PLATFORM_WINDOWS) - 아직 Hive인증, 로그인관련 정해진게 없어서 Editor처럼 자동 로그인으로 해놓는다.
#if WITH_EDITOR
	// 자동로그인 상태 체크
	if (FGsNetManager* netManager = GNet())
	{
		/*if (FGsNetAutoLogin* autoLogin = netManager->GetAutoLogin())
		{
			if (autoLogin->Enter(EGsAutoConnectType::CONNECT_LOGIN))
			{				
				return;
			}
		}*/
	}

	const FString& userLoginId = GNet()->GetLoginId();
	if (userLoginId.Len() > 0)
	{
		OpenUI(userLoginId);
	}
	else
	{
		if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
		{
			const FString& userInfoVid = gameUserSettings->GetUserInfoSetting(EGsOptionUserInfo::VID_NAME);
			OpenUI(userInfoVid);
		}
	}
#else
	StartLogIn();
#endif
}

void FGsStateLobbyLogin::OpenUI(const FString& inUserLoginId)
{
#if WITH_EDITOR
	if (UGsUIManager* uiManager = GUI())
	{
		FName key = TEXT("WindowLoginPCEditorOnly");
		FSoftObjectPath path("/Game/DeveloperTools/Window_LoginPCEditorOnly.Window_LoginPCEditorOnly");

		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidgetByPath(key, path);
		if (widget.IsValid())
		{
			_windowUI = Cast<UGsUIWindowLogin>(widget);
			_windowUI->OnClickButton.BindRaw(this, &FGsStateLobbyLogin::OnClickLogin);

			if (inUserLoginId.IsEmpty())
			{
				_windowUI->SetData(UKismetSystemLibrary::GetDeviceId());
			}
			else
			{
				_windowUI->SetData(inUserLoginId);
			}
		}
	}
#endif
}

void FGsStateLobbyLogin::Exit()
{
	UnBlocking();
	UnregisterMessages();
	
#if WITH_EDITOR
	if (_windowUI.IsValid())
	{
		_windowUI->Close();
	}
#endif

	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyLogin::RegisterMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
		return;

	MUI& mUI = messageMgr->GetUI();
	MSystemParam& mLobbySystem = messageMgr->GetSystemParam();

	_msgHandleUIList.Emplace(mUI.AddRaw(MessageUI::TITLE_CLICK_START, this, &FGsStateLobbyLogin::OnClick));

	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::NET_LOGIN_SUCCEEDED,
		this, &FGsStateLobbyLogin::NetLogInSucceeded));
	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::NET_LOGIN_FAILED,
		this, &FGsStateLobbyLogin::NetLogInFailed));
	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::NET_PRE_LOGIN_FAILED,
		this, &FGsStateLobbyLogin::NetLogInFailed));
	_msgHandleLobbySystemList.Emplace(mLobbySystem.AddRaw(MessageSystem::NET_CHECKVERSION_FAILED,
		this, &FGsStateLobbyLogin::NetLogInFailed));
}

void FGsStateLobbyLogin::UnregisterMessages()
{
	if (FGsMessageHolder* messageMgr = GMessage())
	{
		MUI& mUI = messageMgr->GetUI();
		for (MsgUIHandle& handleUI : _msgHandleUIList)
		{
			mUI.Remove(handleUI);
		}

		MSystemParam& mLobbySystem = messageMgr->GetSystemParam();
		for (MsgSystemHandle& handleLobbySystem : _msgHandleLobbySystemList)
		{
			mLobbySystem.Remove(handleLobbySystem);			
		}
	}

	_msgHandleLobbySystemList.Empty();
	_msgHandleUIList.Empty();
}

void FGsStateLobbyLogin::OnClickLogin(const FString& inVid)
{
	uint64 vid = FCString::Strtoui64(*inVid, NULL, 10);
	GNet()->SetVId(vid);

	StartLogIn();
}

void FGsStateLobbyLogin::OnClick(const IGsMessageParam* InParam)
{	
#if !WITH_EDITOR
	if (IsBlocking())
	{
		return;
	}

	StartLogIn();
#endif
}

void FGsStateLobbyLogin::StartLogIn()
{
	GSLOG(Log, TEXT("FGsStateLobbyLogin::StartLogIn()"));

	Blocking();
	
	// 서버, 클라이언트 버전 체크 후 로그인
	FGsNetSendService::SendAuthReqCheckVersion();
}

void FGsStateLobbyLogin::NetLogInSucceeded(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyLogin::NetLogInFailed() Succeeded"));

	UnBlocking();

	FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_LOGIN);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);	
}

void FGsStateLobbyLogin::NetLogInFailed(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStateLobbyLogin::NetLogInFailed() Failed"));

	UnBlocking();
}