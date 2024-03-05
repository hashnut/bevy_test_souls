// Fill out your copyright notice in the Description page of Project Settings.


#include "GsNetAutoLogin.h"

#include "GsNetSendService.h"

#include "Option/GsGameUserSettings.h"
#include "Option/GsOptionEnumType.h"

#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Message/GsMessageStage.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "GsNetBase.h"
#include "UTIL/GsText.h"
#include "T1Project.h"
#include "Culture.h"


void FGsNetAutoLogin::OnInitialize()
{
	if (UGsOptionManager* opt = GOption())
	{
		gameUserSettings = opt->GetGameUserSettingsEx();
		if (nullptr != gameUserSettings)
		{
			isAutoLogin = gameUserSettings->IsAutoLogin();
		}
	}

	stageTypeFuncs.Reset();
	stageTypeFuncs.Emplace(EGsAutoConnectType::CONNECT_LOGIN, [this]() { return EnterLogin(); });
	stageTypeFuncs.Emplace(EGsAutoConnectType::CONNECT_SERVER, [this]() { return EnterServerSelect(); });
	stageTypeFuncs.Emplace(EGsAutoConnectType::CONNECT_CHARACTER, [this]() { return EnterCharacterSelect(); });
}

bool FGsNetAutoLogin::IsAutoLogin()
{
#if WITH_EDITOR
	return isAutoLogin;
#else
	return false;
#endif
}

const FString FGsNetAutoLogin::GetAppID()
{
	FString appID = TEXT("Editor AppID");
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{			
#ifdef WINDOWS_CROS_PLAY_PROCESS
#if !WITH_EDITOR
			if (UGsHiveManager* hiveMgr = GHive())
			{
				appID = hiveMgr->GetConfigurationGetAppId();				
			}
#endif
#else
			appID = FString(UTF8_TO_TCHAR(Configuration::getAppId().c_str()));
#endif				
		}
	}

	GSLOG(Log, TEXT("appID : %s"), *appID);
	return appID;
}

const Did FGsNetAutoLogin::GetDID()
{
	FString did = TEXT("did");
#if WITH_EDITOR
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			did = FString::Printf(TEXT("%lld"), netBase->GetDId());
		}
	}
#else	
#ifdef WINDOWS_CROS_PLAY_PROCESS
#if !WITH_EDITOR
	if (UGsHiveManager* hiveMgr = GHive())
	{
		did = hiveMgr->GetDid();	
	}
#endif
#else
#if PLATFORM_ANDROID || PLATFORM_IOS
	if (UGsHiveManager* hiveMgr = GHive())
	{	
		did = hiveMgr->GetDid(), // device identifier.	
	}
#endif
#endif
#endif
	GSLOG(Log, TEXT("DID : %s"), *did);
	return FCString::Strtoui64(*did, NULL, 10);
}

const FString FGsNetAutoLogin::GetDIDText()
{
	FString did = TEXT("did");
#ifdef WINDOWS_CROS_PLAY_PROCESS
#if !WITH_EDITOR
	if (UGsHiveManager* hiveMgr = GHive())
	{
		did = hiveMgr->GetDid();
	}
#endif
#else
#if PLATFORM_ANDROID || PLATFORM_IOS
	if (UGsHiveManager* hiveMgr = GHive())
	{
		did = hiveMgr->GetDid(), // device identifier.	
	}
#endif
#endif
	GSLOG(Log, TEXT("DID : %s"), *did);
	return did;
}

const FString FGsNetAutoLogin::GetDeviceName()
{
	FString deviceName = FGenericPlatformMisc::GetDeviceId();

	GSLOG(Log, TEXT("deviceName : %s"), *deviceName);
	return deviceName;
}

const FString FGsNetAutoLogin::GetHiveLanguage()
{
	//FString hiveLanguage = FInternationalization::Get().GetCurrentLanguage()->GetName();
	FString hiveLanguage = UGsLocalizationManager::GetCurrCultureName();

	GSLOG(Log, TEXT("hiveLanguage : %s"), *hiveLanguage);
	return hiveLanguage;
}

const FString FGsNetAutoLogin::GetHiveCountry()
{
	FString hiveCountry = TEXT("hive country");

	if (UGsHiveManager* hiveMgr = GHive())
	{
		hiveCountry = hiveMgr->GetHiveCountry();
	}

	GSLOG(Log, TEXT("hiveCountry : %s"), *hiveCountry);
	return hiveCountry;
}

bool FGsNetAutoLogin::Enter(EGsAutoConnectType in_StageType)
{
	if (true == isAutoLogin)
	{
		if (nullptr != gameUserSettings)
		{
			if (0 < stageTypeFuncs.Num() && stageTypeFuncs.Contains(in_StageType))
			{
				TFunction<bool()> func = stageTypeFuncs.FindRef(in_StageType);
				if (func())
				{
					return true;
				}
			}
		}
	}

	return isAutoLogin = false;
}

bool FGsNetAutoLogin::EnterLogin()
{
	const FString& userInfoVid = gameUserSettings->GetUserInfoSetting(EGsOptionUserInfo::VID_NAME);
	if (!userInfoVid.IsEmpty())
	{
		FGsUIHelper::ShowBlockUI();
		uint64 vid = FCString::Strtoui64(*userInfoVid, NULL, 10);		
		FGsNetSendService::SendAuthReqLogin();
		return true;
	}

	return false;
}

bool FGsNetAutoLogin::EnterServerSelect()
{
	const FString& serverName = gameUserSettings->GetUserInfoSetting(EGsOptionUserInfo::LAST_ENTER_WORLD_NAME);
	WorldId worldId = static_cast<WorldId>(FCString::Atoi(*serverName));
	if (!serverName.IsEmpty())
	{
		const TArray<ServerElem>& listServer = GNet()->GetSeverList();

		for (int i = 0; i < listServer.Num(); ++i)
		{
			if (listServer[i].mPlanetWorldId == worldId)
			{
				FGsNetSendService::SendAuthReqSelectServer(worldId);
				return true;
			}
		}
	}

	return false;
}

bool FGsNetAutoLogin::EnterCharacterSelect()
{
	const FString& characterName = gameUserSettings->GetUserInfoSetting(EGsOptionUserInfo::LAST_ENTER_CHARACTER);
	UserDBId characterID = static_cast<UserDBId>(FCString::Atoi64(*characterName));

	if (!characterName.IsEmpty())
	{
		TArray<LobbyDataType::FGsLobbyUserListData> listCharacter = LSLobbyData()->GetUserList();
		int8 listNum = listCharacter.Num();

		bool isFinds = false;
		if (0 < listNum)
		{
			for (int i = 0; i < listNum; ++i)
			{
				if (listCharacter[i]._id == characterID)
				{
					FGsNetSendService::SendGatewayReqSelectUser(static_cast<UserDBId>(FCString::Atoi64(*characterName)));

					// 캐릭터 선택했으면 오토로그인은 끝
					isAutoLogin = false;
					return true;
				}
			}
		}
	}

	return false;
}

void FGsNetAutoLogin::SetAutoLogin(bool in_value)
{
#if WITH_EDITOR
	isAutoLogin = in_value;
#else
	isAutoLogin = false;
#endif

	if (UGsOptionManager* opt = GOption())
	{
		opt->SetValue(EGsOptionItem::AutoLogin, isAutoLogin);
		if (nullptr != gameUserSettings)
			gameUserSettings->SetUserInfoSettingByType(EGsOptionUserInfo::AUTO_LOGIN, FString::FromInt(isAutoLogin));
	}
}

