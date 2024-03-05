// Fill out your copyright notice in the Description page of Project Settings.


#include "GsStateLobbyTitle.h"

#include "T1Project.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Net/GsNetSendService.h"

#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsDivergenceManager.h"
#include "Management/ScopeGlobal/GsPatchManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/GsMessageHolder.h"

#include "Message/GsMessageStage.h"
#include "Message/GsMessageUI.h"
#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/MessageParam/GsStageMessageParam.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsLobbyConstant.h"
#include "Data/GsLocalizationConstant.h"

#include "UI/UIContent/Popup/GsUIPopupDownload.h"
#include "UI/UIContent/Window/GsUIWindowTitle.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"

#include "Sound/GsSoundPlayer.h"

// BSAM - 타이틀 동영상등 출력하는곳
/*
	1. Patch 가 필요하면 State Patch로 간뒤 처리후 Begin 로드
	2. 아니면 State SeverSelector로 Begin 로드
*/

FGsStateLobbyTitle::FGsStateLobbyTitle() : FGsContentsLobbyBase(FGsContentsMode::InLobby::TITLE)
{
}

FGsStateLobbyTitle::~FGsStateLobbyTitle()
{
}

void FGsStateLobbyTitle::Enter()
{
	FGsContentsLobbyBase::Enter();

	FGsUIHelper::HideBlockUI();

	GSLOG(Log, TEXT("FGsStateLobbyTitle::Enter()"));
	
	// 메세지 등록
	RegisterMessages();

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	const UGsLobbyConstant* lobbyConstant = LSLobbyData()->GetLobbyConstantData();
	if (nullptr == lobbyConstant)
		return;

#if WITH_EDITOR
	if (false == GNet()->IsLobbyHandshakeCompleted())
	{
		GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
		return;
	}
	
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidgetByPath(
		lobbyConstant->_pathWIndowTitle.Id, lobbyConstant->_pathWIndowTitle.Path);
	if (widget.IsValid())
	{
		if (UGsUIWindowTitle* windowTitle = Cast<UGsUIWindowTitle>(widget.Get()))
		{
			windowTitle->UpdateServerName();
		}
	}	
#else	
	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidgetByPath(
		lobbyConstant->_pathWIndowTitle.Id, lobbyConstant->_pathWIndowTitle.Path);
	if (widget.IsValid())
	{
		if (UGsUIWindowTitle* windowTitle = Cast<UGsUIWindowTitle>(widget.Get()))
		{
			windowTitle->UpdateServerName();
			windowTitle->PlayMovie();
			// 옵션 관련 리소스를 모두 런처로 옴겼으니 이 체크는 이제 빼도 되지않을까?
			// 하이브 로그아웃과 관련이 있을까?
			// 일단 주석 (2022-10-17 최미란)
			// windowTitle->SetVisibleOptionBtn(GPatch()->IsMounted());
		}
	}	
#endif

	// bgm play
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	GSLOG(Log, TEXT("FGsStateLobbyTitle: UGsSoundPlayer* soundPlayer = GSoundPlayer()"));
	soundPlayer->PlaySoundBgmByPath(lobbyConstant->_pathBgmTitle);

#if WITH_EDITOR
	if (GHive()->GetIsDeleteAccountSuccessed())
	{
		FText findText;
		FText::FindText(TEXT("LauncherText"), TEXT("Popup_Delete_Result"), findText);
		FGsUIHelper::PopupSystemMsg(findText, [this]()
			{
				GHive()->SetIsDeleteAccountSuccess(false);
			});
	}
#endif
}

void FGsStateLobbyTitle::Exit()
{
	GSLOG(Log, TEXT("FGsStateLobbyTitle::Exit()"));

	// 메세지 삭제
	UnregisterMessages();
	FGsContentsLobbyBase::Exit();
}

void FGsStateLobbyTitle::RegisterMessages()
{
	FGsMessageHolder* messageMgr = GMessage();
	if (nullptr == messageMgr)
		return;

	MUI& mUI = messageMgr->GetUI();
	MSystemParam& mLobbySystem = messageMgr->GetSystemParam();

	_msgHandleList.Emplace(mUI.AddRaw(MessageUI::TITLE_CLICK_START, this, &FGsStateLobbyTitle::OnClick));
}

void FGsStateLobbyTitle::UnregisterMessages()
{
	if (FGsMessageHolder* messageMgr = GMessage())
	{
		MUI& mUI = messageMgr->GetUI();
		for (MsgUIHandle& handleUI : _msgHandleList)
		{
			mUI.Remove(handleUI);
		}

		MSystemParam& mLobbySystem = messageMgr->GetSystemParam();
		for (MsgSystemHandle& handleLobbySystem : _msgHandleLobbySystemList)
		{
			mLobbySystem.Remove(handleLobbySystem);
		}
	}

	_msgHandleList.Empty();
	_msgHandleLobbySystemList.Empty();
}

void FGsStateLobbyTitle::OnClick(const IGsMessageParam* InParam)
{
	FGsUIHelper::ShowBlockUI();

	FGsLobbyFlowMsgParam param(MessageStage::REQUEST_TITLE_CLICK_EVENT);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
}


void FGsStateLobbyTitle::Update(float In_deltaTime)
{
	FGsContentsLobbyBase::Update(In_deltaTime);
}

void FGsStateLobbyTitle::Init()
{
	FGsContentsLobbyBase::Init();
}

void FGsStateLobbyTitle::Close()
{	
	FGsContentsLobbyBase::Close();
}