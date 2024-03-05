// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIOptionAccountPage.h"

#include "T1Project.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupAccountDelete.h"

#include "Message/GsMessageSystem.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsDivergenceManager.h"

#include "Object.h"
#include "UserWidget.h"
#include "Text.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include "GameFlow/GsGameFlow.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/GsMessageStage.h"
#include "Message/GsMessageUI.h"
#include "Hive/GsHiveEventMsg.h"

#include "Net/GSNet.h"
#include "Net/GsNetSendService.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UTIL/GsText.h"
#include "Divergence/GsDivergenceFunc.h"
#include "HIVE_AuthV4.h"

#include "HAL/PlatformApplicationMisc.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "CanvasPanel.h"


void UGsUIOptionAccountPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//_btnGotoTitle->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickGotoTitle);
	//_btnGotoLobby->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickGotoLobby);	
	_btnGameExit->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickGameExit);

	// 계정 삭제
	_btnDeleteAccount->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickDeleteAccount);
	// 계정 로그아웃
	_btnLogoutAccount->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickLogoutAccount);

	// idf 등록
	_btnHiveConnect->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickHiveConnect);
	_btnFacebookConnect->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickFacebookConnect);
	_btnGoogleConnect->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickGoogleConnect);
	_btnAppleConnect->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickAppleConnect);
	// idf 해제
	_btnHiveDisConnect->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickHiveDisConnect);
	_btnFacebookDisConnect->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickFacebookDisConnect);
	_btnGoogleDisConnect->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickGoogleDisConnect);
	_btnAppleDisConnect->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickAppleDisConnect);

	// CsCode 복사
	_btnCsCodeCopy->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickCsCodeCopy);

	// 문의하기
	_btnGameInquiry->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickInquiry);
	// 이용약관
	_btnGameTerms->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickTerms);
	// 유저정보
	_btnUserProfile->OnClicked.AddDynamic(this, &UGsUIOptionAccountPage::OnClickUserProfile);

}

void UGsUIOptionAccountPage::NativeConstruct()
{
	Super::NativeConstruct();

	if (FGsMessageHolder* message = GMessage())
	{
		_listSystemIntDelegate.Emplace(
			message->GetSystemInt().AddUObject(MessageSystem::HIVE_CONNECT_SUCCESS, this, &UGsUIOptionAccountPage::HiveConnectSucceeded));

		_listSystemIntDelegate.Emplace(
			message->GetSystemInt().AddUObject(MessageSystem::HIVE_CONNECT_FAIL, this, &UGsUIOptionAccountPage::HiveConnectFailed));

		_listSystemIntDelegate.Emplace(
			message->GetSystemInt().AddUObject(MessageSystem::HIVE_DISCONNECT_SUCCESS, this, &UGsUIOptionAccountPage::HiveDisConnectSucceeded));

		_listSystemIntDelegate.Emplace(
			message->GetSystemInt().AddUObject(MessageSystem::HIVE_DISCONNECT_FAIL, this, &UGsUIOptionAccountPage::HiveDisConnectFailed));

		_listSystemDelegate.Emplace(
			message->GetSystemParam().AddUObject(MessageSystem::HIVE_AUTH_LOGOUT_TRUE, this, &UGsUIOptionAccountPage::HiveLogOutSucceeded));

		_listSystemDelegate.Emplace(
			message->GetSystemParam().AddUObject(MessageSystem::HIVE_AUTH_LOGOUT_FALSE, this, &UGsUIOptionAccountPage::HiveLogOutFailed));	

		_listSystemDelegate.Emplace(
			message->GetSystemParam().AddUObject(MessageSystem::HIVE_USER_PROFILE_UPDATE, this, &UGsUIOptionAccountPage::HiveUserProfileUpdateRequest));

		_listSystemDelegate.Emplace(
			message->GetSystemParam().AddUObject(MessageSystem::HIVE_IDP_CONFLICT_PLAYER, this, &UGsUIOptionAccountPage::HiveIdpConflictPlayer));

		_listSystemDelegate.Emplace(
			message->GetSystemParam().AddUObject(MessageSystem::HIVE_SignOut, this, &UGsUIOptionAccountPage::HIVESignOut));

		_listSystemDelegate.Emplace(
			message->GetSystemParam().AddUObject(MessageSystem::HIVE_IdpConnectCancle, this, &UGsUIOptionAccountPage::HIVEIdepConnectCancle));
	}

	// 타입 초기화
	_connecctType = EGsProviderType::NONE;
	// 세팅
	SetData();

#if PLATFORM_IOS
	_btnGameExit->SetVisibility(ESlateVisibility::Collapsed);
#else
	_btnGameExit->SetVisibility(ESlateVisibility::Visible);
#endif

#if FLOW_DUBUG_TEST
	SetVisibleDeleteAccountBtn(true);
	SetVisibleLogoutAccountBtn(true);
#else
	SetVisibleIdpBtn();
#if !WITH_EDITOR
	SetVisibleDeleteAccountBtn(true);
	SetVisibleLogoutAccountBtn(true);	
#else
	SetVisibleDeleteAccountBtn(false);
	SetVisibleLogoutAccountBtn(false);
#endif
#endif
}

void UGsUIOptionAccountPage::NativeDestruct()
{
	if (FGsMessageHolder* message = GMessage())
	{
		if (_listSystemDelegate.Num() != 0)
		{
			for (auto iter : _listSystemDelegate)
			{
				message->GetSystemParam().Remove(iter);
			}
			_listSystemDelegate.Empty();
		}

		if (_listSystemIntDelegate.Num() != 0)
		{
			for (auto iter : _listSystemIntDelegate)
			{
				message->GetSystemInt().Remove(iter);
			}
			_listSystemIntDelegate.Empty();
		}
	}

	Super::NativeDestruct();
}

void UGsUIOptionAccountPage::OnClickGotoTitle()
{
	// 게임 나가기 메시지 보내기. GameFlowManager에서 받아 각 State에 따라 처리된다.
	GMessage()->GetSystem().SendMessage(MessageSystem::BACKTO_TITLE);
}

void UGsUIOptionAccountPage::OnClickGotoLobby()
{
	// 게임 나가기 메시지 보내기. GameFlowManager에서 받아 각 State에 따라 처리된다.
	GMessage()->GetSystem().SendMessage(MessageSystem::EXIT_GAME);
}

void UGsUIOptionAccountPage::OnClickHiveConnect()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click HiveConnect"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	OnIdpDisConnect(EGsProviderType::HIVE);
}

void UGsUIOptionAccountPage::OnClickFacebookConnect()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click FacebookConnect"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	OnIdpDisConnect(EGsProviderType::FACEBOOK);
}

void UGsUIOptionAccountPage::OnClickGoogleConnect()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click GoogleConnect"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	OnIdpDisConnect(EGsProviderType::GOOGLE);
}

void UGsUIOptionAccountPage::OnClickAppleConnect()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click AppleConnect"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	OnIdpDisConnect(EGsProviderType::APPLE);
}

void UGsUIOptionAccountPage::OnIdpConnect(EGsProviderType In_Type)
{
#if FLOW_DUBUG_TEST	
	_testIndex = static_cast<int>(In_Type);	
	HiveInfoUpdate();
#else
#if !WITH_EDITOR
	UGsHiveManager* hiveMgr = GHive();
	if (nullptr == hiveMgr)
		return;

	//EGsProviderType outCurConnectProviderType;
	//GetConnectedType(outCurConnectProviderType);
	//if (EGsProviderType::NONE != outCurConnectProviderType &&
	//	In_Type != outCurConnectProviderType)
	//{
	//	// 로그아웃 할것 backup
	//	//_disConnecctType = outCurConnectProviderType;
	//}

	if (false == IsConnectedInType(In_Type))
	{
		GSLOG(Log, TEXT("OptionAccountPage::Hive Connect call, providerType : %s"), *FGsTextUtil::GetStringFromEnum(TEXT("EGsProviderType"), In_Type));
		hiveMgr->Connect(In_Type);
		FGsUIHelper::ShowBlockUI();
	}
#endif
#endif
}

void UGsUIOptionAccountPage::OnIdpDisConnect(EGsProviderType In_Type)
{	
	// 연동 안된 타입을 눌렀을때 
#if FLOW_DUBUG_TEST	
	_testIndex = 0;
	HiveInfoUpdate();
#else
#if !WITH_EDITOR
	UGsHiveManager* hiveMgr = GHive();
	if (nullptr == hiveMgr)
		return;

//#if PLATFORM_WINDOWS
	if (IsConnectedInType(In_Type))
	{
		if (1 >= GetConnectedCount())
		{
			// pc는 게스트가 없어 무고건 1개는 있어야해
			// 막을거야
			FText findText;
			FText::FindText(TEXT("LauncherText"), TEXT("DisConnect_Error"), findText);
			FGsUIHelper::PopupSystemMsg(findText);
		}
		else
		{
			GSLOG(Log, TEXT("OptionAccountPage::Hive DisConnect call, providerType : %s"), *FGsTextUtil::GetStringFromEnum(TEXT("EGsProviderType"), In_Type));
			hiveMgr->DisConnect(In_Type);
		}		
	}
	else
	{	
		// 연동 후에 올수있는 이벤트 
		GSLOG(Log, TEXT("OptionAccountPage::Hive DisConnect Faile"));		
	}
//#else
//	if (IsConnectedInType(In_Type))
//	{
//		GSLOG(Log, TEXT("OptionAccountPage::Hive DisConnect call, providerType : %s"), *FGsTextUtil::GetStringFromEnum(TEXT("EGsProviderType"), In_Type));
//		hiveMgr->DisConnect(In_Type);
//	}
//#endif
#endif
#endif
}

void UGsUIOptionAccountPage::OnClickHiveDisConnect()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click HiveDisConnect"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	OnIdpConnect(EGsProviderType::HIVE);
}

void UGsUIOptionAccountPage::OnClickFacebookDisConnect()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click FacebookDisConnect"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	OnIdpConnect(EGsProviderType::FACEBOOK);
}

void UGsUIOptionAccountPage::OnClickGoogleDisConnect()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click GoogleDisConnect"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	OnIdpConnect(EGsProviderType::GOOGLE);
}

void UGsUIOptionAccountPage::OnClickAppleDisConnect()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click AppleDisConnect"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	OnIdpConnect(EGsProviderType::APPLE);
}

void UGsUIOptionAccountPage::OnClickCsCodeCopy()
{
	FText textVid = _textVID->GetText();
	if (false == textVid.ToString().IsEmpty())
	{
		FPlatformApplicationMisc::ClipboardCopy(*textVid.ToString());

		FText findText;
		FText::FindText(TEXT("OptionText"), TEXT("Notice_CScodeCopy"), findText);
		FGsUIHelper::TrayTickerLauncher(findText);
	}
}

void UGsUIOptionAccountPage::OnClickGameExit()
{
	FGsUIHelper::PopupQuitGame();
}

void UGsUIOptionAccountPage::OnClickDeleteAccount()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click DeleteAccount"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

#if FLOW_DUBUG_TEST
#else
	UGsHiveManager* hiveMgr = GHive();
	if (nullptr != hiveMgr)
	{
		if (false == hiveMgr->IsGuest())
		{
			if (false == hiveMgr->IsSignIn())
			{
				// 로그인 된게 없어 계정삭제 할게 없어
				FText errorText;
				FText::FindText(TEXT("LauncherText"), TEXT("NetDisconected"), errorText);

				FGsUIHelper::PopupNetError(errorText);
				return;
			}
		}
		else
		{
			// 게스트 로그인 상태이면?
			// 삭제할게 없어
			FText errorText;
			FText::FindText(TEXT("LauncherText"), TEXT("NetDisconected"), errorText);

			FGsUIHelper::PopupNetError(errorText);
			return;
		}
	}
#endif

	FText msg;
	FText::FindText(TEXT("LauncherText"), TEXT("Popup_Delete_Desc"), msg);

	FGsUIHelper::PopupSystemYesNo(msg, [](bool bYes)
	{
		if (bYes)
		{
			TPair<FName, FSoftObjectPath> pathList = GUI()->GetPathIPopupAccountDelete();
			TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidgetByPath(pathList.Key, pathList.Value);
			if (widget.IsValid())
			{
				UGsUIPopupAccountDelete* popup = Cast<UGsUIPopupAccountDelete>(widget.Get());
				if (popup)
				{
					popup->SetData([](bool bPopupCharacterDeleteYes)
					{
						if (bPopupCharacterDeleteYes)
						{
							if (FGsNet::Mode::GAME == GNet()->GetCurrentNetType())
							{
								FGsNetSendService::SendReqDeleteAccount();
							}
							else
							{
								FGsNetSendService::SendAuthReqDeleteAccount();
							}
						}
					});
				}
			}
		}		
	});	
}

void UGsUIOptionAccountPage::SetVisibleDeleteAccountBtn(bool In_Visible)
{
	if (nullptr == _btnDeleteAccount)
		return;
		
	if (In_Visible)
	{
#if FLOW_DUBUG_TEST
		_btnDeleteAccount->SetVisibility(ESlateVisibility::Visible);
#else
		if (UGsHiveManager* hiveMgr = GHive())
		{
			if (false == hiveMgr->IsGuest() && true == hiveMgr->IsSignIn() && true == IsConnected())
			{
				In_Visible = true;
			}
			else
			{			
				In_Visible = false;
			}
		}
		else
		{
			In_Visible = false;
		}
#endif
	}

	_btnDeleteAccount->SetVisibility(In_Visible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UGsUIOptionAccountPage::SetVisibleLogoutAccountBtn(bool In_Visible)
{
	if (nullptr == _btnLogoutAccount)
		return;

	if (In_Visible)
	{
#if FLOW_DUBUG_TEST
		_btnLogoutAccount->SetVisibility(ESlateVisibility::Visible);
#else
		if (UGsHiveManager* hiveMgr = GHive())
		{
			if (false == hiveMgr->IsGuest() && true == hiveMgr->IsSignIn())
			{
				In_Visible = true;
			}			
			else
			{
				In_Visible = false;
			}
		}
		else
		{
			In_Visible = false;
		}
#endif
	}
	
	_btnLogoutAccount->SetVisibility(In_Visible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

class UWidgetSwitcher* UGsUIOptionAccountPage::GetIdpButtonInProviderType(EGsProviderType In_ProviderType)
{
	class UWidgetSwitcher* idpButton = nullptr;

	switch (In_ProviderType)
	{
	case EGsProviderType::GUEST:
		break;
	case EGsProviderType::HIVE:
		idpButton = _swicherBtnHive;
		break;
	case EGsProviderType::FACEBOOK:
		idpButton = _swicherBtnFacebook;
		break;
	case EGsProviderType::GOOGLE:
		idpButton = _swicherBtnGoogle;
		break;
	case EGsProviderType::APPLE:
		idpButton = _swicherBtnApple;
		break;
	case EGsProviderType::NONE:
		break;
	default:
		break;
	}

	return idpButton;
}

void UGsUIOptionAccountPage::SetVisibleIdpBtnInProviderType(EGsProviderType In_ProviderType, bool In_isVisible)
{
	class UWidgetSwitcher* idpButton = GetIdpButtonInProviderType(In_ProviderType);
	if (nullptr != idpButton)
	{
		idpButton->SetVisibility(In_isVisible? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UGsUIOptionAccountPage::SetVisibleIdpBtn()
{
	for (size_t i = 0; i < static_cast<int>(EGsProviderType::NONE); i++)
	{
		SetVisibleIdpBtnInProviderType(static_cast<EGsProviderType>(i), false);
	}
	
#if FLOW_DUBUG_TEST	
#else
	if (UGsHiveManager* hiveMgr = GHive())
	{
#if !WITH_EDITOR
		TArray<FString> idpList;
		hiveMgr->HelperGetIDPList(idpList);
		for (FString IDP : idpList)
		{
			GSLOG(Error, TEXT("[UIOptionAccountPage] - IDP : %s"), *IDP);

			hive::ProviderType type = hive::ProviderInfo::providerTypeFromString(TCHAR_TO_UTF8(*IDP));
			SetVisibleIdpBtnInProviderType(GetClientProviderType(type), true);
		}
#else
		for (size_t i = 0; i < static_cast<int>(EGsProviderType::NONE); i++)
		{
			SetVisibleIdpBtnInProviderType(static_cast<EGsProviderType>(i), true);
		}
#endif

		InvalidateConnectButtons();
	}
#endif
}

void UGsUIOptionAccountPage::OnClickLogoutAccount()
{
	GSLOG(Log, TEXT("[OptionAccountPage] Click Logout"));

#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Click LogoutAccount"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

#if FLOW_DUBUG_TEST
	HiveLogOutSucceeded(nullptr);
#else
#if !WITH_EDITOR
		if (UGsHiveManager* hiveMgr = GHive())
		{
			if (false == hiveMgr->IsGuest())
			{
				if (true == hiveMgr->IsSignIn())
				{
					GSLOG(Log, TEXT("[OptionAccountPage] hive SignOut Call"));
					hiveMgr->SignOut();
				}
			}
		}

		FGsUIHelper::ShowBlockUI();
#endif
#endif
}

void UGsUIOptionAccountPage::SetData()
{
	FGsNetManager* netMgr = GNet();
	if (nullptr == netMgr)
	{
		return;
	}

	TSharedPtr<FGsNetBase> netBase = GNet()->GetActive();
	if (netBase.IsValid() == false)
	{
		return;
	}

	if (netBase->GetType() == FGsNet::Mode::LOBBY)
	{
		_canvasPanelName->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_canvasPanelName->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		const FGsNetUserData* userData = GGameData()->GetUserData();
		if (nullptr != userData)
		{
			_textName->SetText(FText::FromString(userData->mUserName));

			// IOS 에서는 나가기 버튼 숨김
			if (_btnGameExit)
			{
#if PLATFORM_IOS
				_btnGameExit->SetVisibility(ESlateVisibility::Collapsed);
#else
				_btnGameExit->SetVisibility(ESlateVisibility::Visible);
#endif
			}
		}
	}

	const FString& selectPlanetWorldName = netMgr->GetSelectPlanetWorldName();
	if (!selectPlanetWorldName.IsEmpty())
	{
		_textServerName->SetText(FText::FromString(selectPlanetWorldName));
	}

	FString vid = FString::Printf(TEXT("%llu"), netMgr->GetVId());
	_textVID->SetText(FText::FromString(vid));

	FString version;
	GetVersion(version);
	_textGameVersion->SetText(FText::FromString(version));

#if FLOW_DUBUG_TEST	
	_testIndex = 0;
#endif
	HiveInfoUpdate();

	// 유저프로필 가져오기
	// 2023/08/38 현 시점 기준, SDK 버전 업데이트로 PC 에서도 프로필 기능 활성화
#if !WITH_EDITOR
	if (UGsHiveManager* hiveMgr = GHive())
	{
		hiveMgr->GetProfile();
	}
#endif
}

void UGsUIOptionAccountPage::GetVersion(OUT FString& outVersion)
{
	outVersion = GDivergence()->GetProjectVersion();
	GSLOG(Log, TEXT("UGsUIOptionAccountPage outVersion: %s"), *outVersion);
}

void UGsUIOptionAccountPage::HiveConnectFailed(int32 InParam)
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Hive Connect Failed"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	// 연동 실패
	EGsProviderType providerType = GetClientProviderType(static_cast<hive::ProviderType>(InParam));

	GSLOG(Log, TEXT("OptionAccountPage::Hive Connect result : Faile, providerType : %s, param: %d"), 
		*FGsTextUtil::GetStringFromEnum(TEXT("EGsProviderType"), providerType), InParam);

	FGsUIHelper::HideBlockUI();
}

EGsProviderType UGsUIOptionAccountPage::GetClientProviderType(IN hive::ProviderType inHiveProviderType)
{
	switch (inHiveProviderType)
	{
	case hive::ProviderType::HIVE:
		 return EGsProviderType::HIVE;
		break;
	case hive::ProviderType::FACEBOOK:
		return  EGsProviderType::FACEBOOK;
		break;
	case hive::ProviderType::GOOGLE:
		return  EGsProviderType::GOOGLE;
		break;
	case hive::ProviderType::SIGNIN_APPLE:
		return  EGsProviderType::APPLE;
		break;
	default:
		break;
	}

	return EGsProviderType::NONE;
}

void UGsUIOptionAccountPage::HiveConnectSucceeded(int32 InParam)
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Hive Connect Succeeded"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	UGsHiveManager* hiveMgr = GHive();
	if (nullptr == hiveMgr)
		return;

	EGsProviderType providerType = GetClientProviderType(static_cast<hive::ProviderType>(InParam));

	GSLOG(Log, TEXT("OptionAccountPage::Hive Connect result : Success, providerType : %s, param: %d"), 
		*FGsTextUtil::GetStringFromEnum(TEXT("EGsProviderType"), providerType), InParam);

	bool bIsTryDisconnect = false;

	/*if (EGsProviderType::NONE != _disConnecctType)
	{
		if (IsConnectedInType(_disConnecctType))
		{
			GSLOG(Log, TEXT("OptionAccountPage::Hive DisConnect Call -> ProviderType : %s"), *FGsTextUtil::GetStringFromEnum(TEXT("EGsProviderType"), _disConnecctType));
			hiveMgr->DisConnect(_disConnecctType);
			bIsTryDisconnect = true;
		}
	}*/

	//SetIdpButtonSetting(true, providerType);
	InvalidateConnectButtons();

	if (false == bIsTryDisconnect)
	{
		FGsUIHelper::HideBlockUI();
	}
}

void UGsUIOptionAccountPage::HiveDisConnectSucceeded(int32 InParam)
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("HiveDisConnect Succeeded"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	// 연동해지 성공	
	UGsHiveManager* hiveMgr = GHive();
	if (nullptr == hiveMgr)
		return;

	EGsProviderType providerType = GetClientProviderType(static_cast<hive::ProviderType>(InParam));
	GSLOG(Log, TEXT("OptionAccountPage::Hive DisConnect result : Success, providerType : %s, param: %d"), 
		*FGsTextUtil::GetStringFromEnum(TEXT("EGsProviderType"), providerType), InParam);

	//SetIdpButtonSetting(false, providerType);
	InvalidateConnectButtons();
	FGsUIHelper::HideBlockUI();
}

void UGsUIOptionAccountPage::HiveDisConnectFailed(int32 InParam)
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("HiveDisConnect Failed"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	// 연동해지 실패
	EGsProviderType providerType = GetClientProviderType(static_cast<hive::ProviderType>(InParam));

	GSLOG(Log, TEXT("OptionAccountPage::Hive DisConnect result : Failed, providerType : %s, param: %d"), 
		*FGsTextUtil::GetStringFromEnum(TEXT("EGsProviderType"), providerType), InParam);

	FGsUIHelper::HideBlockUI();
}

void UGsUIOptionAccountPage::HiveLogOutSucceeded(const struct IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("UGsUIOptionAccountPage::HiveLogOutSucceeded() Call"));

	FGsUIHelper::HideBlockUI();

	FText text;
	FText::FindText(TEXT("LauncherText"), TEXT("AccountLogoutSucceeded"), text);

	// 로그아웃 성공
	FGsUIHelper::PopupSystemMsg(text, []()
		{
			if (FGsGameFlowManager* flowManager = GMode())
			{
				if (FGsGameFlow::Mode::LOBBY == flowManager->GetCurrentFlowType())
				{
					TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("PopupLobbyOption"));
					if (widget.IsValid())
					{
						widget->Close();
					}
				}
				GMessage()->GetSystem().SendMessage(MessageSystem::RESTART_GAME);				
			}
		});
}

void UGsUIOptionAccountPage::HiveLogOutFailed(const struct IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("UGsUIOptionAccountPage::HiveLogOutFailed() Call"));

	FGsUIHelper::HideBlockUI();

	FText errorText;
	FText::FindText(TEXT("LauncherText"), TEXT("AccountLogoutFailed"), errorText);

	// 로그아웃 실패
	FGsUIHelper::PopupNetError(errorText);
}

void UGsUIOptionAccountPage::HiveInfoUpdate()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("HiveInfoUpdate -> button settings"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

#if FLOW_DUBUG_TEST	
	if (0 == _testIndex)	// 게스트
	{
		_swicherBtnHive->SetActiveWidgetIndex(0);
		_swicherBtnFacebook->SetActiveWidgetIndex(0);
		_swicherBtnApple->SetActiveWidgetIndex(0);
		_swicherBtnGoogle->SetActiveWidgetIndex(0);
	}
	else if (1 == _testIndex)	// 하이브
	{
		_swicherBtnHive->SetActiveWidgetIndex(1);
		_swicherBtnFacebook->SetActiveWidgetIndex(0);
		_swicherBtnApple->SetActiveWidgetIndex(0);
		_swicherBtnGoogle->SetActiveWidgetIndex(0);
	}
	else if (2 == _testIndex) // FACEBOOK
	{
		_swicherBtnHive->SetActiveWidgetIndex(0);
		_swicherBtnFacebook->SetActiveWidgetIndex(1);
		_swicherBtnApple->SetActiveWidgetIndex(0);
		_swicherBtnGoogle->SetActiveWidgetIndex(0);
	}
	else if (3 == _testIndex) //GOOGLE
	{
		_swicherBtnHive->SetActiveWidgetIndex(0);
		_swicherBtnFacebook->SetActiveWidgetIndex(0);
		_swicherBtnApple->SetActiveWidgetIndex(0);
		_swicherBtnGoogle->SetActiveWidgetIndex(1);
	}
	else // APPLE
	{
		_swicherBtnHive->SetActiveWidgetIndex(0);
		_swicherBtnFacebook->SetActiveWidgetIndex(0);
		_swicherBtnApple->SetActiveWidgetIndex(1);
		_swicherBtnGoogle->SetActiveWidgetIndex(0);
	}
#else
	InvalidateConnectButtons();
#endif
}

void UGsUIOptionAccountPage::SetIdpButtonSetting(bool In_IsOn, EGsProviderType In_ProviderTypeType)
{
	switch (In_ProviderTypeType)
	{
	case EGsProviderType::GUEST:
		break;
	case EGsProviderType::HIVE:
		_swicherBtnHive->SetActiveWidgetIndex(In_IsOn ? 1 : 0);
		break;
	case EGsProviderType::FACEBOOK:
		_swicherBtnFacebook->SetActiveWidgetIndex(In_IsOn ? 1 : 0);
		break;
	case EGsProviderType::GOOGLE:
		_swicherBtnGoogle->SetActiveWidgetIndex(In_IsOn ? 1 : 0);
		break;
	case EGsProviderType::APPLE:
		_swicherBtnApple->SetActiveWidgetIndex(In_IsOn ? 1 : 0);
		break;
	case EGsProviderType::NONE:
		break;
	default:
		break;
	}
}

void UGsUIOptionAccountPage::GetConnectedType(OUT EGsProviderType& Out_ProviderType)
{
	Out_ProviderType = EGsProviderType::NONE;

	if (UGsHiveManager* hiveMgr = GHive())
	{
		for (int i = static_cast<int>(EGsProviderType::HIVE); i < static_cast<int>(EGsProviderType::APPLE) + 1; ++i)
		{
			bool isConnected = false;
			isConnected = hiveMgr->IsIdpConnected(static_cast<EGsProviderType>(i));

			if (isConnected)
			{
				Out_ProviderType = static_cast<EGsProviderType>(i);
				return;
			}
		}
	}
}

bool UGsUIOptionAccountPage::IsConnectedInType(EGsProviderType In_Type)
{
	UGsHiveManager* hiveMgr = GHive();
	if (nullptr == hiveMgr)
		return false;

	return hiveMgr->IsIdpConnected(In_Type);
}

bool UGsUIOptionAccountPage::IsConnected()
{
	if (UGsHiveManager* hiveMgr = GHive())
	{
		for (int i = static_cast<int>(EGsProviderType::HIVE); i < static_cast<int>(EGsProviderType::APPLE) + 1; ++i)
		{
			bool isConnected = false;
			EGsProviderType providerType = static_cast<EGsProviderType>(i);
			isConnected = hiveMgr->IsIdpConnected(providerType);

			if (isConnected)
			{				
				return true;
			}
		}
	}

	return false;
}

int UGsUIOptionAccountPage::GetConnectedCount()
{
	int count = 0;
	if (UGsHiveManager* hiveMgr = GHive())
	{
		for (int i = static_cast<int>(EGsProviderType::HIVE); i < static_cast<int>(EGsProviderType::APPLE) + 1; ++i)
		{
			bool isConnected = false;
			EGsProviderType providerType = static_cast<EGsProviderType>(i);
			isConnected = hiveMgr->IsIdpConnected(providerType);

			if (isConnected)
			{
				++count;
			}
		}
	}

	return count;
}

void UGsUIOptionAccountPage::OnDisConnect()
{
	if (UGsHiveManager* hiveMgr = GHive())
	{
		for (int i = static_cast<int>(EGsProviderType::HIVE); i < static_cast<int>(EGsProviderType::APPLE) + 1; ++i)
		{
			bool isConnected = false;
			EGsProviderType providerType = static_cast<EGsProviderType>(i);
			isConnected = hiveMgr->IsIdpConnected(providerType);

			if (isConnected)
			{
				GSLOG(Log, TEXT("OptionAccountPage::Hive DisConnect Call -> ProviderType : %s"), *FGsTextUtil::GetStringFromEnum(TEXT("EGsProviderType"), providerType));
				hiveMgr->DisConnect(providerType);
				return;
			}
		}
	}
}

void UGsUIOptionAccountPage::InvalidateConnectButtons()
{
#if WITH_EDITOR
	_swicherBtnHive->SetActiveWidgetIndex(0);
	_swicherBtnFacebook->SetActiveWidgetIndex(0);
	_swicherBtnApple->SetActiveWidgetIndex(0);
	_swicherBtnGoogle->SetActiveWidgetIndex(0);
#else
	_swicherBtnHive->SetActiveWidgetIndex(IsConnectedInType(EGsProviderType::HIVE) ? 1 : 0);
	_swicherBtnFacebook->SetActiveWidgetIndex(IsConnectedInType(EGsProviderType::FACEBOOK) ? 1 : 0);
	_swicherBtnApple->SetActiveWidgetIndex(IsConnectedInType(EGsProviderType::APPLE) ? 1 : 0);
	_swicherBtnGoogle->SetActiveWidgetIndex(IsConnectedInType(EGsProviderType::GOOGLE) ? 1 : 0);
#endif
}

void UGsUIOptionAccountPage::OnHttpResponseProfileUpdate(FHttpRequestPtr httpRequest, FHttpResponsePtr httpResponse, bool isSuccess)
{
	GSLOG(Log, TEXT("Profile Update Response Success State : %d"), isSuccess);

	if (isSuccess)
	{
		UTexture2D* outputTexture = UKismetRenderingLibrary::ImportBufferAsTexture2D(GetWorld(), httpResponse->GetContent());

		_imgUserProfile->SetBrushFromTexture(outputTexture);
	}
}

void UGsUIOptionAccountPage::SetImageProfileWithUrl(const FString& InImageUrl /*= TEXT("https://sandbox-hive-fn.qpyou.cn/hubweb/avatar_img/public/noimage.png")*/)
{
	// HTTP 관련 작업 -> 수령한 url 로 실제 이미지를 세팅해 준다
	_requestProfileImgDownload = FHttpModule::Get().CreateRequest();

	GSLOG(Log, TEXT("Url link : %s"), *InImageUrl);
	_requestProfileImgDownload->SetURL(InImageUrl);

	_requestProfileImgDownload->SetVerb(TEXT("GET"));

	_requestProfileImgDownload->OnProcessRequestComplete().BindUObject(this, &UGsUIOptionAccountPage::OnHttpResponseProfileUpdate);

	_requestProfileImgDownload->ProcessRequest();
}

void UGsUIOptionAccountPage::OnClickInquiry()
{
#if !WITH_EDITOR
	if (UGsHiveManager* hiveMgr = GHive())
	{
		hiveMgr->ShowInquiry();
	}
#endif
}

void UGsUIOptionAccountPage::OnClickTerms()
{
#if !WITH_EDITOR
	if (UGsHiveManager* hiveMgr = GHive())
	{
		hiveMgr->ShowTerms();
	}
#endif
}

void UGsUIOptionAccountPage::OnClickUserProfile()
{
	// 윈도우에서는 클릭하면 티커를 출력하고 리턴한다
//#if PLATFORM_WINDOWS
//	FText text;
//	FText::FindText(TEXT("OptionText"), TEXT("Alert_CannotEnterHiveProfile_PC"), text);
//	FGsUIHelper::TrayTickerLauncher(text);
//	return;
//#endif

#if !WITH_EDITOR
	if (UGsHiveManager* hiveMgr = GHive())
	{
		hiveMgr->ShowProfile();
	}
#endif
}

void UGsUIOptionAccountPage::HiveUserProfileUpdateRequest(const struct IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("UGsUIOptionAccountPage::HiveUserProfilepdate() Call"));

	FGsUIHelper::HideBlockUI();

	UGsHiveManager* hiveMgr = GHive();
	if (nullptr == hiveMgr)
	{
		GSLOG(Error, TEXT("nullptr == hiveMgr"));
		return;
	}

	GSLOG(Log, TEXT("\nAuthV4::GetProfile playerImageUrl : %s"), *(hiveMgr->GetPlayerImageUrl()));

	SetImageProfileWithUrl(hiveMgr->GetPlayerImageUrl());
}

// idp 연동 충돌
void UGsUIOptionAccountPage::HiveIdpConflictPlayer(const struct IGsMessageParam* InParam)
{
	if (UGsHiveManager* hiveMgr = GHive())
	{
		//hiveMgr->GetDid() ?
		// 유저 id 얻어서 서버로 전달
		if (FGsNetManager* netMgr = GNet())
		{
			if (netMgr->GetActive().IsValid())
			{
				if (FGsNet::Mode::GAME == GNet()->GetCurrentNetType())
				{
					// PKT_CG_REQ
					FGsNetSendService::SendReqIdpConflictPlayer();
				}
				else if (FGsNet::Mode::LOBBY == GNet()->GetCurrentNetType())
				{
					// PKT_CA_REQ
					FGsNetSendService::SendAuthReqIdpConflictPlayer();
				}
			}
		}
	}
}

void UGsUIOptionAccountPage::HIVESignOut(const struct IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("UGsUIOptionAccountPage::HIVESignOut() Call"));

	if (UGsHiveManager* hiveMgr = GHive())
	{
		hiveMgr->SignOut();
	}
}

void UGsUIOptionAccountPage::HIVEIdepConnectCancle(const struct IGsMessageParam* InParam)
{
	GSLOG(Log, TEXT("UGsUIOptionAccountPage::HIVEIdepConnectCancle() Call"));

	FGsUIHelper::HideBlockUI();
}
