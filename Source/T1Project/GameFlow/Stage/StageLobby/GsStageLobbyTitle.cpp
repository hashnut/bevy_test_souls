// Fill out your copyright notice in the Description page of Project Settings.

#include "GsStageLobbyTitle.h"
#include "GameFlow/Stage/GsStageMode.h"

#include "Net/GsNetSendService.h"

#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"

#include "Sound/GsSoundPlayer.h"

#include "GameFlow/GsGameFlowLobby.h"
#include "GameFlow/GameContents/ContentsLobby/GsContentsManagerLobby.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UIContent/Window/GsUIWindowTitle.h"
#include "UI/UIContent/Popup/GsUIPopupServerSelect.h"
#include "UI/UIContent/Popup/GsUIPopupDownload.h"

#include "Message/GsMessageStage.h"
#include "Message/GsMessageUI.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageSystem.h"

#include "Data/GsLobbyConstant.h"
#include "Data/GsDataContainManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Message/MessageParam/GsStageMessageParam.h"



FGsStageLobbyTitle::FGsStageLobbyTitle() : FGsStageLobbyBase(FGsStageMode::Lobby::TITLE)
{
}

FGsStageLobbyTitle::~FGsStageLobbyTitle()
{
}

void FGsStageLobbyTitle::Enter()
{
	FGsStageLobbyBase::Enter();

	BindMessages();

	if (nullptr == GScope())
		return;

	// ui 오픈
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		ErrorExit();
		return;
	}

	UGsLobbyDataManager* lobbyDataMgr = LSLobbyData();
	if (nullptr == lobbyDataMgr)
	{
		ErrorExit();
		return;
	}

	const UGsLobbyConstant* lobbyConstant = lobbyDataMgr->GetLobbyConstantData();
	if (nullptr == lobbyConstant)
	{
		ErrorExit();
		return;
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidgetByPath(
		lobbyConstant->_pathWIndowTitle.Id, lobbyConstant->_pathWIndowTitle.Path);
	if (widget.IsValid() == false)
	{
		ErrorExit();
		return;
	}

	_windowUI = Cast<UGsUIWindowTitle>(widget.Get());
	_windowUI->OnClickServerSelect.BindRaw(this, &FGsStageLobbyTitle::OnClickServerSelect);
	_windowUI->OnClickOption.BindRaw(this, &FGsStageLobbyTitle::OnClickOption);
	_windowUI->OnClickStart.BindRaw(this, &FGsStageLobbyTitle::OnClickStart);
	_windowUI->OnClickAccountChange.BindRaw(this, &FGsStageLobbyTitle::OnClickAccountChange);
	_windowUI->OnClickDeviceRegister.BindRaw(this, &FGsStageLobbyTitle::OnClickDeviceRegister);

	// BSAM_PATCH_DATA_REPAIR
	_windowUI->OnClickPatchCheck.BindRaw(this, &FGsStageLobbyTitle::OnClickPatchCheck);

	_windowUI->PlayMovie();

	FGsUIHelper::TrayLobbyFadeIn(1.0f);
}

void FGsStageLobbyTitle::BindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MSystemParam& mSystem = msgMgr->GetSystemParam();

	_listSystemDelegate.Emplace(mSystem.AddRaw(MessageSystem::HIVE_AUTH_FALSE,
		this, &FGsStageLobbyTitle::HiveAuthResultFailed));
	_listSystemDelegate.Emplace(mSystem.AddRaw(MessageSystem::HIVE_AUTH_TRUE,
		this, &FGsStageLobbyTitle::HiveAuthResultSucceeded));

	_listSystemDelegate.Emplace(mSystem.AddRaw(MessageSystem::HIVE_AUTH_LOGOUT_TRUE,
		this, &FGsStageLobbyTitle::HiveAuthLogoutResultSucceeded));
	_listSystemDelegate.Emplace(mSystem.AddRaw(MessageSystem::HIVE_AUTH_LOGOUT_FALSE,
		this, &FGsStageLobbyTitle::HiveAuthLogoutResultFailed));
	_listSystemDelegate.Emplace(mSystem.AddRaw(MessageSystem::REGISTER_DEVIECE_POPUP_SUCCEEDED,
		this, &FGsStageLobbyTitle::HiveDeviceRegisterPopupSuccessded));
	_listSystemDelegate.Emplace(mSystem.AddRaw(MessageSystem::REGISTER_DEVIECE_POPUP_FAILED,
		this, &FGsStageLobbyTitle::HiveDeviceRegisterPopupFailed));
	_listSystemDelegate.Emplace(mSystem.AddRaw(MessageSystem::REGISTER_DEVIECE_RESULT_RECEVIED_SERVER,
		this, &FGsStageLobbyTitle::HiveDeviceRegisterResult));

	_listSystemDelegate.Emplace(mSystem.AddRaw(MessageSystem::NET_SERVER_SELECT_SUCCEEDED,
		this, &FGsStageLobbyTitle::UpdateServerName));
	_listSystemDelegate.Emplace(mSystem.AddRaw(MessageSystem::NET_SERVER_SELECT_FAILED,
		this, &FGsStageLobbyTitle::UpdateServerName));
	_listSystemDelegate.Emplace(mSystem.AddRaw(MessageSystem::NET_GATEWAY_LOGIN_FAILED,
		this, &FGsStageLobbyTitle::UpdateServerName));
	_listSystemDelegate.Emplace(mSystem.AddRaw(MessageSystem::HIVE_Show_SignIn,
		this, &FGsStageLobbyTitle::HIVEShowSignIn));
}

void FGsStageLobbyTitle::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MSystemParam& mSystem = msgMgr->GetSystemParam();
	for (MsgSystemHandle& handle : _listSystemDelegate)
	{
		mSystem.Remove(handle);
	}
	_listSystemDelegate.Empty();
}

void FGsStageLobbyTitle::Exit()
{
	UnbindMessages();

	if (_windowUI.IsValid())
	{
		_windowUI->Close();
	}

	if (_popupLobbyOptionUI.IsValid())
	{
		_popupLobbyOptionUI->Close();
	}

	FGsStageLobbyBase::Exit();
}

void FGsStageLobbyTitle::ErrorExit()
{
#if WITH_EDITOR
	GMessage()->GetSystem().SendMessage(MessageSystem::EXIT_GAME);
#else
	GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);
#endif

	const FString errorString = FString::Printf(TEXT("[GsStageLobbyTitle] ErrorExit"));
	GSLOG(Log, TEXT("%s"), *errorString);
}

void FGsStageLobbyTitle::Update(float In_deltaTime)
{
	FGsStageLobbyBase::Update(In_deltaTime);
}

void FGsStageLobbyTitle::OnClickServerSelect()
{
	class FGsGameFlowLobby* flowLobby = GMode()->GetLobbyFlow();
	if (nullptr == flowLobby)
		return;

	FGsContentsManagerLobby* contents = flowLobby->GetContentsManagerLobby();
	if (nullptr == contents)
		return;

	if (FGsContentsMode::InLobby::TITLE != contents->GetCurrentContentsMode())
		return;	

#if !WITH_EDITOR
	UGsHiveManager* hiveManager = GHive();
	if (nullptr == hiveManager)
		return;

	if (false == hiveManager->IsSignIn())
		return;
#endif
			
	// TODO: 서버선택창 팝업
	UGsDataContainManager* dataContainManager = GData();
	if (nullptr == dataContainManager)
		return;

	const UGsLobbyConstant* lobbyConstant = LSLobbyData()->GetLobbyConstantData();
	if (nullptr == lobbyConstant)
		return;
	
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidgetByPath(
		lobbyConstant->_pathPopupServerSelect.Id, lobbyConstant->_pathPopupServerSelect.Path);
	if (widget.IsValid() == false)
		return;		

	UGsUIPopupServerSelect* popupServerSelect = Cast<UGsUIPopupServerSelect>(widget.Get());
	if (popupServerSelect == nullptr)
		return;

	popupServerSelect->OnClickServer.BindRaw(this, &FGsStageLobbyTitle::OnPopupClickServerSelect);
}

void FGsStageLobbyTitle::OnPopupClickServerSelect(uint16 In_PlanetWorldId)
{
	if (_windowUI.IsValid())
	{
		_windowUI->SetServerName(In_PlanetWorldId);
	}
}

void FGsStageLobbyTitle::OnClickOption()
{
	class FGsGameFlowLobby* flowLobby = GMode()->GetLobbyFlow();
	if (nullptr == flowLobby)
		return;

	FGsContentsManagerLobby* contents = flowLobby->GetContentsManagerLobby();
	if (nullptr == contents)
		return;

	if (FGsContentsMode::InLobby::TITLE != contents->GetCurrentContentsMode())
		return;

#if !WITH_EDITOR
	UGsHiveManager* hiveManager = GHive();
	if (nullptr == hiveManager)
		return;

	if (false == hiveManager->IsSignIn())
		return;
#endif

	const UGsLobbyConstant* lobbyConstant = LSLobbyData()->GetLobbyConstantData();
	if (nullptr == lobbyConstant)
		return;

	// TODO: 로비옵션 팝업
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidgetByPath(
		lobbyConstant->_pathPopupLobbyOption.Id, lobbyConstant->_pathPopupLobbyOption.Path);
	if (widget.IsValid() == false)
		return;

	_popupLobbyOptionUI = Cast<UGsUIPopupLobbyOption>(widget.Get());
}

void FGsStageLobbyTitle::OnClickStart(uint16 In_planetWorldId)
{
	FGsUIMsgParamInt param(static_cast<int32>(In_planetWorldId));
	GMessage()->GetUI().SendMessage(MessageUI::TITLE_CLICK_START, &param);
}

void FGsStageLobbyTitle::OnClickAccountChange()
{
#if !WITH_EDITOR
	UGsHiveManager* hiveManager = GHive();
	if (nullptr == hiveManager)
		return;

	if (false == hiveManager->IsInit())
		return;

	if (false == hiveManager->IsSignIn())
	{
		HIVEShowSignIn(nullptr);
		return;
	}
#endif

	// 계정 변경 시작
	SetIsAccountChanging(true);

	//FGsLobbyFlowMsgParam param(MessageStage::CHANGE_ACCOUNT);
	//GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);

	// 변경할꺼면 무조건 서버 로그아웃
	TSharedPtr<FGsNetBase> netBase = GNet()->GetActive();
	if (netBase.IsValid())
	{
		GNet()->OnExitGame();
	}

	GHive()->OnAccountChange();
}

// BSAM_PATCH_DATA_REPAIR
void FGsStageLobbyTitle::OnClickPatchCheck()
{
	FText msg;
	FText::FindText(TEXT("TitleSceneText"), TEXT("PatchDataRepairDesc"), msg);
	FGsUIHelper::PopupSystemYesNo(msg, [this](bool bYes)
	{
		if (bYes)
		{
			FGsLobbyFlowMsgParam param(MessageStage::REQUEST_PATCH_REPAIR);
			GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
		}
	});
}

void FGsStageLobbyTitle::OnClickDeviceRegister()
{
	class FGsGameFlowLobby* flowLobby = GMode()->GetLobbyFlow();
	if (nullptr == flowLobby)
		return;

	FGsContentsManagerLobby* contents = flowLobby->GetContentsManagerLobby();
	if (nullptr == contents)
		return;

	if (FGsContentsMode::InLobby::TITLE != contents->GetCurrentContentsMode())
		return;

#if !WITH_EDITOR
	UGsHiveManager* hiveManager = GHive();
	if (nullptr == hiveManager)
		return;	

	if (false == hiveManager->IsSignIn())
		return;

	FGsUIHelper::ShowBlockUI();	
	hiveManager->ShowDeviceManagement();	
#endif
}

// 하이브 SignIn 요청 실패
void FGsStageLobbyTitle::HiveAuthResultFailed(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStageLobbyTitle::HiveAuthResultFailed() false"));

	if (false == GetIsAccountChanging())
		return;

#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("[StageLobbyTitle] HiveAuthResultFailed"));

	FVector2D textScale{ 1.f, 1.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	AccountChagedResult(false);
}

// 하이브 SignIn 요청 성공
void FGsStageLobbyTitle::HiveAuthResultSucceeded(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStageLobbyTitle::HiveAuthResultSucceeded() true"));

	if (false == GetIsAccountChanging())
		return;

#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("[StageLobbyTitle] HiveAuthResultSucceeded"));

	FVector2D textScale{ 1.f, 1.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	AccountChagedResult(true);
}

// 하이브 LogOut 실패
void FGsStageLobbyTitle::HiveAuthLogoutResultFailed(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStageLobbyTitle::HiveAuthLogoutResultFailed() false"));

#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("[StageLobbyTitle] HiveAuthLogoutResultFailed"));

	FVector2D textScale{ 1.f, 1.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	FGsUIHelper::HideBlockUI();
}

// 하이브 LogOut 성공
void FGsStageLobbyTitle::HiveAuthLogoutResultSucceeded(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStageLobbyTitle::HiveAuthLogoutResultSucceeded()"));

#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("[StageLobbyTitle] HiveAuthLogoutResultSucceeded"));

	FVector2D textScale{ 1.f, 1.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_HIVE_LOGOUT);
	GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
}

void FGsStageLobbyTitle::SetIsAccountChanging(bool In_Value)
{
	_isAccountChanging = In_Value;

	if (_isAccountChanging)
	{
		// 계정 변경 중엔 다른거 못하도록 롹걸기
		FGsUIHelper::ShowBlockUI();
		FGsUIHelper::SetBlockUISetting(FText::GetEmpty());
	}
	else
	{
		// 계정 변경 중엔 다른거 못하도록 롹풀기
		FGsUIHelper::HideBlockUI();
	}
}

void FGsStageLobbyTitle::AccountChagedResult(bool inValue)
{
	// 계정변경 후 로그인이면
	if (GetIsAccountChanging())
	{
		// 실패든 성공이든
		// 계정변경 상태 끔
		SetIsAccountChanging(false);

		if (true == inValue)
		{
			// 성공
			/*FText text;
			FText::FindText(TEXT("LobbyText"), TEXT("Popup_Account_Change_Result"), text);
			FGsUIHelper::PopupSystemMsg(text);*/
#if !UE_BUILD_SHIPPING
			const FString logString = FString::Printf(TEXT("AccountChagedResult : Success"));

			FVector2D textScale{ 1.f, 1.f };
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif
			//OnClickStart(GNet()->GetPlanetWorldId());
		}
		else
		{
			// 실패
			/*FText text;
			FText::FindText(TEXT("LobbyText"), TEXT("Popup_Account_Change_Fail"), text);
			FGsUIHelper::PopupSystemMsg(text);*/
#if !UE_BUILD_SHIPPING
			const FString logString = FString::Printf(TEXT("AccountChagedResult : failure"));

			FVector2D textScale{ 1.f, 1.f };
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

#if WITH_EDITOR
			GMessage()->GetSystem().SendMessage(MessageSystem::EXIT_GAME);
#else
			//GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);

#endif
		}
	}
}

// 기기등록 hive Popup 성공
void FGsStageLobbyTitle::HiveDeviceRegisterPopupSuccessded(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStageLobbyTitle::HiveDeviceRegisterPopupSuccessded()"));

	FGsUIHelper::HideBlockUI();

	// 기기등록 박스를 누를 경우 무조건 서버로그인을 새로한다
	GMessage()->GetSystem().SendMessage(MessageSystem::BACKTO_TITLE);
}

// 기기등록  hive Popup 실패
void FGsStageLobbyTitle::HiveDeviceRegisterPopupFailed(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStageLobbyTitle::HiveDeviceRegisterPopupFailed()"));

	FGsUIHelper::HideBlockUI();

	// 기기등록 박스를 누를 경우 무조건 서버로그인을 새로한다
	GMessage()->GetSystem().SendMessage(MessageSystem::BACKTO_TITLE);
}

// 기기등록 auth서버 로그인 패킷 결과
void FGsStageLobbyTitle::HiveDeviceRegisterResult(const IGsMessageParam* InParam)
{
	if (const FGsPrimitiveUInt8* param = InParam->Cast<const FGsPrimitiveUInt8>())
	{
		bool isRegister = static_cast<bool>(param->_data);

		if (_windowUI.IsValid())
		{
			_windowUI->SetRegisterIcon(isRegister);
		}
	}
}

void FGsStageLobbyTitle::UpdateServerName(const IGsMessageParam* InParam)
{
	if (_windowUI.IsValid())
	{
		_windowUI->UpdateServerName();
	}
}

void FGsStageLobbyTitle::HIVEShowSignIn(const IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("FGsStageLobbyTitle::HIVEShowSignIn()"));

	if (UGsHiveManager* hiveMgr = GHive())
	{
		hiveMgr->ShowSignIn();
	}
}