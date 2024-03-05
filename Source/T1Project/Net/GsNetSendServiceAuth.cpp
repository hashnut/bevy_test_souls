#include "GsNetSendService.h"

#include "../Plugins/PlayGuard/Source/PlayGuard/Public/PlayGuardModule.h"
#include "Shared/Shared/SharedPackets/PD_Client_Auth.h"
#include "Shared/Shared/SharedPackets/PdlVersion.h"
#include "Shared/Client/SharedEnums/SharedAppGuardEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Internationalization.h"
#include "Culture.h"
#include "GenericPlatformMisc.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/GsMessageSystem.h"
#include "Management/GsMessageHolder.h"
#include "GameMode/GsGameModeBase.h"
#include "Engine/Engine.h"

void FGsNetSendService::SendAuthReqPassword(const FString& inId, const FString& inPassword)
{
	if (FGsNetManager* netMgr = GNet())
	{
		netMgr->SetLoginId(inId);
		
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CA::PKT_CA_REQ_CHECK_PASSWORD_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inId, inPassword);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendAuthReqCheckVersion()
{
	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsUIHelper::ShowBlockUI();

			uint64 pdlVersion = PD::GetPdlVersion();
			
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CA::PKT_CA_REQ_CHECK_VERSION_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), pdlVersion);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

void FGsNetSendService::SendAuthReqPreLogin(const uint64 inVid)
{
	GSLOG(Log, TEXT("SendAuthReqPreLogin Vid : llu"), inVid);

	if (FGsNetManager* netMgr = GNet())
	{
		FGsUIHelper::ShowBlockUI();

		FString deviceName = FGenericPlatformMisc::GetDeviceMakeAndModel();
		//FString deviceName = FGenericPlatformMisc::GetDeviceId();
		//FString hiveLanguageCode = FInternationalization::Get().GetCurrentLanguage()->GetName();
		FString hiveLanguageCode = UGsLocalizationManager::GetCurrCultureName();
		FString hiveCountryCode = TEXT("Hive Country");
		TArray<FString> idpList;
		FTimespan UTCOffset = FDateTime::Now() - FDateTime::UtcNow();
		int timeZoneValue = UTCOffset.GetHours();

		// #todo-hive channelType, marketType 하드코딩 정식 하이브 함수 테스트후 변경해야됨.
		FString channelType = FString::Printf(TEXT("HIVE"));
		// FString marketType = FString::Printf(TEXT("marketType"));
		bool isUseSyncPay = false;
		bool isSandbox = false;

#if PLATFORM_ANDROID
		const FString marketType(TEXT("GO"));
#elif PLATFORM_IOS
		const FString marketType(TEXT("AP"));
#elif PLATFORM_WINDOWS
		const FString marketType(TEXT("PC"));
#else
		const FString marketType(TEXT("PC"));
#endif

		DevicePlatformType platformType = AGsGameModeBase::GetPlatformType();

		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			AppGuardActiveMode CurrentAppGuardActiveMode = AppGuardActiveMode::IN_ACTIVATE;
			switch (FPlayGuardModule::GetGuardActivationMode())
			{
			case GUARD_ACTIVATION_MODE_DISABLE:
				CurrentAppGuardActiveMode = AppGuardActiveMode::IN_ACTIVATE;
				break;

			case GUARD_ACTIVATION_MODE_ENABLE_TEST:
				CurrentAppGuardActiveMode = AppGuardActiveMode::ACTIVATE_TEST;
				break;

			default:
				CurrentAppGuardActiveMode = AppGuardActiveMode::ACTIVATE;
				break;
			}

#if WITH_EDITOR
			FString appID = TEXT("Editor AppID");
			GSLOG(Error, TEXT("[FGsNetSendService::SendAuthReqLogin] - appID : %s"), *appID);

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			FString did = FString::Printf(TEXT("%llu"), netBase->GetDId());

			PD::CA::PKT_CA_REQ_PRE_LOGIN_WRITE builder(
				sendBuffer->GetBuffer()
				, sendBuffer->GetSize()
				, inVid, PD::GetPdlVersion()
				, TEXT("authorization")
				, (uint8)platformType
				, appID
				, did
				, deviceName
				, hiveLanguageCode
				, hiveCountryCode
				, timeZoneValue
				, channelType
				, marketType
				, isSandbox
				, CurrentAppGuardActiveMode);

			netBase->Send(builder.mPktWriter.GetPktSize());
			netMgr->SetVId(inVid);
#else
			if (UGsHiveManager* hiveMgr = GHive())
			{
				FString appID = hiveMgr->GetConfigurationGetAppId();
				GSLOG(Error, TEXT("[FGsNetSendService::SendAuthReqLogin] - appID : %s"), *appID);

				FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
				deviceName = FGenericPlatformMisc::GetDeviceMakeAndModel();
				//deviceName = FGenericPlatformMisc::GetDeviceId();
				hiveCountryCode = hiveMgr->GetHiveCountry();
				hiveMgr->HelperGetIDPList(idpList);
				isSandbox = hiveMgr->IsSandBox();
				//channelType = hiveMgr->GetChannel();
				//marketType = hiveMgr->GetMarket();

				// BSAM - 제가 알기로 애초에 로그인 안되면 해당 값을 받을수 없음, 전제조건은 로그인(사인-인)됬다는 말인듯함.
				PD::CA::PKT_CA_REQ_PRE_LOGIN_WRITE builder(
					sendBuffer->GetBuffer()
					, sendBuffer->GetSize()
					, hiveMgr->GetPlayerID()
					, PD::GetPdlVersion()
					, hiveMgr->GetPlayerToken()	// playerToken : 사인-인 검증에 필요한 playerId 와 연결된 토큰
					, (uint8)platformType		// 플랫폼 타입
					, appID						// appid
					, hiveMgr->GetDid()			// device identifier.
					, deviceName				// device name
					, hiveLanguageCode			// hive language
					, hiveCountryCode			// hive conttry
					, timeZoneValue
					, channelType
					, marketType
					, isSandbox
					, CurrentAppGuardActiveMode);


				hiveMgr->HelperGetIDPList(idpList);
				for (FString IDP : idpList)
				{
					GSLOG(Error, TEXT("[Login] - IDP : %s"), *IDP);
					builder.CreateIdpList(IDP);
				}

				netBase->Send(builder.mPktWriter.GetPktSize());
				netMgr->SetVId(hiveMgr->GetPlayerID());

				GSLOG(Log, TEXT("SendAuthReqRreLogin Vid : %llu"), hiveMgr->GetPlayerID());
			}
#endif
			GSLOG(Error, TEXT("[RreLogin] - CurrentCultureName : %s"), *hiveLanguageCode);
			GSLOG(Error, TEXT("[RreLogin] - DeviceName : %s"), *deviceName);
			GSLOG(Error, TEXT("[RreLogin] - TimezoneValue : %d"), timeZoneValue);
			GSLOG(Error, TEXT("[RreLogin] - HiveCountry : %s"), *hiveCountryCode);
			GSLOG(Error, TEXT("[RreLogin] - Channel : %s"), *channelType);
			GSLOG(Error, TEXT("[RreLogin] - Market : %s"), *marketType);
		}
	}
}

void FGsNetSendService::SendAuthReqLogin(const bool InUseZpay)
{
	if (FGsNetManager* netMgr = GNet())
	{
		FGsUIHelper::ShowBlockUI();

		GSLOG(Error, TEXT("[Login] - useZPay : %d"), InUseZpay);

		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CA::PKT_CA_REQ_LOGIN_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), InUseZpay);
			netBase->Send(builder.mPktWriter.GetPktSize());
		}
	}
}

bool FGsNetSendService::SendAuthReqSelectServer(uint16 inPlanetWorldId)
{
	GSLOG(Log, TEXT("SendAuthReqSelectServer PlanetWorldId : %d"), inPlanetWorldId);

	if (FGsNetManager* netMgr = GNet())
	{
#if	!UE_BUILD_SHIPPING
		FString serverName;
		netMgr->GetPlanetWorldIdName(inPlanetWorldId, serverName);

		const FString logString = FString::Printf(
			TEXT("[NetSend] SendAuthReqSelectServer PlanetWorldId : %d, PlanetWorldName : %s"), inPlanetWorldId, *serverName);
		//FVector2D textScale{ 1.5f, 1.5f };
		//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Green, logString, true, textScale);

		GSLOG(Log, TEXT("%s"), *logString);
#endif

		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			////bak1210 
			//// 안명달 구두협약 : 선택하고 클라먼저 하이브쪽에 서버선택
			////https://jira.com2us.com/wiki/pages/viewpage.action?pageId=304440737
			//FString hiveAnalyticsServerId;
			//if (netMgr->TryGetHiveAnalyticsServerId(inPlanetWorldId, hiveAnalyticsServerId))
			//{
			//	GHive()->SetServerIdBySelectServer(hiveAnalyticsServerId);
			//}
			////-------------------------------------------------------------------------

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			//GSLOG(Log, TEXT("SendAuthReqSelectServer [%d]"), inWorldId);
			PD::CA::PKT_CA_REQ_SELECT_SERVER_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), inPlanetWorldId);
			netBase->Send(builder.mPktWriter.GetPktSize());
						
			//netBase->SetPacketSendBlock(true);			
			FGsUIHelper::ShowBlockUI();
			FGsUIHelper::TrayDimmed(true);

			GSLOG(Log, TEXT("SendAuthReqSelectServer Send->Success"));

			return true;
		}		
	}

	return false;
}

bool FGsNetSendService::SendAuthReqAppGuardAuth()
{
#if PLATFORM_ANDROID || PLATFORM_IOS
	GSLOG(Verbose, TEXT("Try to send SendAuthReqAppGuardAuth()"));

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FPlayGuardModule* PlayGuardModule = FPlayGuardModule::Get();

			// ReceivedFromAppGuardCallback 는 앱가드로부터 UniqueClientID 응답을 받았는지 여부. 응답이 성공이었든, 실패였든 결과에 무관하게 응답을 받았다면 true.
			const bool ReceivedFromAppGuardCallback = (nullptr != PlayGuardModule && PlayGuardModule->GetUniqueClientIdState() == EUniqueClientIdState::Recieved);

			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			PD::CA::PKT_CA_REQ_APP_GUARD_AUTH_WRITE builder(sendBuffer->GetBuffer(), sendBuffer->GetSize(), ReceivedFromAppGuardCallback);
			netBase->Send(builder.mPktWriter.GetPktSize());
			GSLOG(Verbose, TEXT("Sended SendAuthReqAppGuardAuth(%d)"), ReceivedFromAppGuardCallback ? 1 : 0);
			return true;
		}
	}

	GSLOG(Error, TEXT("Failed to send SendAuthReqAppGuardAuth()"));
	return false; // 송신 실패이므로, 이 후 진행을 caller 에게 맡기기 위해 false 반환
#else
	GSLOG(Verbose, TEXT("Skip SendAuthReqAppGuardAuth()"));
	return false; // 아무것도 송신하지 않았으므로, 이 후 진행을 caller 에게 맡기기 위해 false 반환
#endif
}

bool FGsNetSendService::SendAuthReqDeleteAccount()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("Send, AuthServer DELETE_ACCOUNT"));

	FVector2D textScale{ 1.f, 1.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

#if !WITH_EDITOR
	GSLOG(Log, TEXT("Try to send SendAuthReqDeleteAccount()"));

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
						
			UGsHiveManager* hiveMgr = GHive();
			if (nullptr == hiveMgr)
			{
				GSLOG(Log, TEXT("Failed to send SendAuthReqDeleteAccount()"));
				return false;
			}		

			PD::CA::PKT_CA_REQ_DELETE_ACCOUNT_WRITE builder(
				sendBuffer->GetBuffer(), 
				sendBuffer->GetSize(), 
				hiveMgr->GetPlayerToken(), 
				hiveMgr->GetConfigurationGetAppId(), 
				FCString::Strtoui64(*hiveMgr->GetDid(), NULL, 10));
			netBase->Send(builder.mPktWriter.GetPktSize());
			GSLOG(Log, TEXT("Sended SendAuthReqDeleteAccount()"));
			return true;
		}
	}

	GSLOG(Log, TEXT("Failed to send SendAuthReqDeleteAccount()"));
	return false;
#else
	GSLOG(Log, TEXT("Skip SendAuthReqDeleteAccount()"));

#if FLOW_DUBUG_TEST
	// UI 다 닫고
	GUI()->CloseAllStack();

	FText text;
	FText::FindText(TEXT("LauncherText"), TEXT("Popup_Delete_Result"), text);
	FGsUIHelper::PopupSystemMsg(text, []()
		{
			GMessage()->GetSystem().SendMessage(MessageSystem::EXIT_GAME);

		});
#endif
	return true;
#endif
}

void FGsNetSendService::SendAuthReqIdpConflictPlayer()
{
	GSLOG(Log, TEXT("Try to send SendAuthReqIdpConflictPlayer()"));

	if (FGsNetManager* netMgr = GNet())
	{
		TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
		if (netBase.IsValid())
		{
			FGsNetBuffer* sendBuffer = netBase->GetLocalSendBufferPtr();
			UGsHiveManager* hiveMgr = GHive();
			if (nullptr == hiveMgr)
			{
				GSLOG(Log, TEXT("Failed to send SendAuthReqIdpConflictPlayer(), HiveManager is null"));
			}

			GSLOG(Log, TEXT("SendAuthReqIdpConflictPlayer() hiveMgr->GetConflictPlayer().playerId : %llu"), hiveMgr->GetConflictPlayer().playerId);

			PD::CA::PKT_CA_REQ_IDP_CONFLICT_PLYER_WRITE builder(
				sendBuffer->GetBuffer(),
				sendBuffer->GetSize(),
				hiveMgr->GetConflictPlayer().playerId);

			netBase->Send(builder.mPktWriter.GetPktSize());

			GSLOG(Log, TEXT("Sended SendAuthReqIdpConflictPlayer()"));

			return;
		}
	}

	GSLOG(Log, TEXT("Failed to send SendAuthReqIdpConflictPlayer()"));
}