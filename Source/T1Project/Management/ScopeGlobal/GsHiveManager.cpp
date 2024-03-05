#include "GsHiveManager.h"

#include "T1Project.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsBMShopManager.h"

#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsHiveMessageParam.h"

#include "GsDivergenceManager.h"

#include "Internationalization/Culture.h"
#include "Internationalization/CulturePointer.h"
#include "Internationalization/Internationalization.h"

#include <sstream>
#include <HIVESDK.h>

#include "Dom/JsonObject.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Misc/FileHelper.h"
#include "JsonObjectConverter.h"

#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/GsMessageStage.h"
#if USE_HERCULES
#include "Hercules_App.h"
#endif
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Hive/GsHiveEventHandler.h"
#include "Engine/Engine.h"
#include "Async.h"

// test
//#include "UI/UIContent/Helper/GsUIHelper.h"

template<>
UGsHiveManager* GsHiveManagerSingle::_instance = nullptr;

void UGsHiveManager::Initialize()
{
	UGsHiveManager::InitInstance(this);

	_isAutoSignIn = false;
	_isInit = false;
	_isSignIn = false;

	_hiveEventHandler = MakeShared<FGsHiveEventHandler>();
}

void UGsHiveManager::InitializeAfterResourceDownload()
{
	if (_hiveEventHandler.IsValid())
	{
		_hiveEventHandler->Initialize(this);
	}
}

void UGsHiveManager::Finalize()
{
#if !WITH_EDITOR
	Hercules_Uninitialize();
#endif

	if (_hiveEventHandler.IsValid())
	{
		_hiveEventHandler->Finalize();
	}
	UGsHiveManager::RemoveInstance();
}

void UGsHiveManager::Update(float inTick)
{
}

void UGsHiveManager::SetConfig()
{
	//초기화 전에 처리하는 것이 맞아보인다. 
	//Setup에 실패하거나 반복해도 크게 상관없는 부분이라 성공 체크까지는 필요없을 듯
	auto locale = FInternationalization::Get().GetCurrentLocale();
	
	FString strLocalName = locale.Get().GetTwoLetterISOLanguageName();
	GSLOG(Log, TEXT("GsHiveManager::SetConfig() localeName : %s"), *strLocalName);

	if (!strLocalName.IsEmpty())
	{
		std::string localName(TCHAR_TO_UTF8(*strLocalName));
#if PLATFORM_IOS || PLATFORM_ANDROID	
		hive::Configuration::setGameLanguage(localName);
#else
		ConfigurationSetGameLanguage(localName.c_str());
#endif
	}

	//Server로 부터 받거나, 테이블을 만들어야 되지 않을까? 아직은 하나뿐임
	//FString region = locale.Get().GetRegion(); region.ToLower(); 를 key로 해도 된다.	
	
	if (GDivergence() && GDivergence()->GetService() == TEXT("live"))
	{
		Configuration::setZone(ZoneType::REAL);
	}
	else
	{
		Configuration::setZone(ZoneType::SANDBOX); //sandbox를 지정해야 sandbox인 것 같다
	}
	// 해당 기능은 사용되지 않는 함수로 플로우 단계로 옮겨져 있다고 전달받음, 그래서 해당 코드를 주석처리(미란님, 지현님한테 내용 전달받음)
//#if PLATFORM_IOS || PLATFORM_ANDROID
//	FString strServerId = hive::Configuration::getServerId().c_str();
//#else	
//	auto curId = ConfigurationGetServerId();
//	FString strServerId = FString(curId);
//#endif
//
//	if (strServerId.IsEmpty())
//	{
//		// Divergence참조하게 변경(https://jira.com2us.com/wiki/pages/viewpage.action?pageId=334155031)
//		SetServerIdBySelectServer(); //나중에 다시 등록이 되기 때문에 GLOBAL만 등록
//	}
	//if (curId.empty())
	//{
	//	GSLOG(Log, TEXT("hivelog(Configuration::setServerId) result : %s\n"), *promoServer);
	//	Configuration::setServerId(std::string(TCHAR_TO_UTF8(*promoServer)));
	//}
	//else
	//{
	//	GSLOG(Log, TEXT("hivelog(Configuration::updateServerId) result : %s\n"), UTF8_TO_TCHAR(curId.c_str()));
	//	Configuration::updateServerId(std::string(TCHAR_TO_UTF8(*promoServer)));
	//}
	
	// AuthV1, AuthV4 초기화 이전 호출
	hive::Configuration::setUseHercules(false);

#if PLATFORM_IOS || PLATFORM_ANDROID
	// Hive 인증키
	FString HiveCertKeyString = TEXT("eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJIaXZlIiwiaWF0IjoxNjc3MTM4ODE3LCJqdGkiOiItMTQyNDA0NjkyMCJ9.tLYarfeO_LlphT_tYDIKQSs1wPFUuE4DpxxWKmemHG4");
	std::string hiveCertKey(TCHAR_TO_UTF8(*HiveCertKeyString));

	// Hive 인증키 등록, 초기화 이전 호출
	hive::Configuration::setHiveCertificationKey(hiveCertKey);
#endif
}

void UGsHiveManager::CheckAuthenticationState(ResultAPI const& In_Result)
{
	switch (In_Result.code)
	{
		case ResultAPI::Code::AuthV4InvalidServerResponse:
		{
			// 전달받은 메시지와 함께 HIVE에 문의
		}break;

		case ResultAPI::Code::AuthV4ConflictPlayer:
		{
			// 계정 충돌에 대한 로직을 수행하세요.

		}break;

		case ResultAPI::Code::AuthV4UserInBlacklist:
		{
			// 해당 계정으로 게임 진행을 하지 못하도록 하세요.
		}break;

		case ResultAPI::Code::AuthV4InvalidSession:
		{
			// 일시적인 현상으로 해당 에러가 발생 할 수 있습니다. 해당 문제가 지속되는 경우 전달받은 에러 메시지와 함께 HIVE에 문의
		}break;

		case ResultAPI::Code::AuthV4ServerResponseError:
		{
			// 전달받은 메시지와 함께 HIVE에 문의
		}break;

		case ResultAPI::Code::AuthV4InvalidResponseData:
		{
			// HIVE 서버 통신 프로토콜 규약에 맞지 않은 데이터가 전달되었습니다.
			// 전달받은 메시지와 함께 HIVE에 문의
		}break;
	}
}

bool UGsHiveManager::IsInit()
{
	FString msg = FString::Format(TEXT("UGsHiveManager::IsInit() {0}"), { _isInit });
	//FGsUIHelper::PopupSystemMsg(FText::FromString(msg));

	GSLOG(Log, TEXT("GsHiveManager::IsInit() call : %s"), *msg);
	
	return _isInit;
}

bool UGsHiveManager::IsSignIn()
{
	FString msg = FString::Format(TEXT("UGsHiveManager::IsSignIn() {0}"), { _isSignIn });
	//FGsUIHelper::PopupSystemMsg(FText::FromString(msg));

	return _isSignIn;
}

bool UGsHiveManager::IsAutoSignIn()
{
	FString msg = FString::Format(TEXT("UGsHiveManager::IsAutoSignIn() {0}"), { _isAutoSignIn });
	//FGsUIHelper::PopupSystemMsg(FText::FromString(msg));

	return _isAutoSignIn;
}

// Hive 초기화가 실패하면 윈도우(Editor)가 아니면 종료해야되는거 아닌가? 일단은 종료는 안시킨다. 대신 다음플로우로 진행은 못하게 해놓음
void UGsHiveManager::Init()
{
	GSLOG(Log, TEXT("hivelog : AuthV4::setup call"));
	//FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("hivelog : AuthV4::setup call")));

	AuthV4::setup([=](ResultAPI const& In_Result, bool In_IsAutoSignIn, std::string In_Did, std::vector<ProviderType> const& In_ProviderTypeList)
	{
		FString providerListString;

		if (In_Result.isSuccess())
		{
			GSLOG(Log, TEXT("hivelog : AuthV4::setup In_Result = success"));
			_isInit = true;
			_isAutoSignIn = In_IsAutoSignIn;

			AsyncTask(ENamedThreads::GameThread, [this]() {
					Hercules_Initialize();
				});
			
			for (auto i = In_ProviderTypeList.begin(); i != In_ProviderTypeList.end(); i++)
			{
				ProviderType providerType = (ProviderType)*i;

				FString typeString = FString(ProviderInfo::stringProviderType(providerType).c_str());

				providerListString.Append(typeString);
				providerListString.Append(TEXT("\n"));
			}

			// 명시적 푸시 권한 요청 추가
			Push::requestPushPermission();
			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_INIT_TRUE, nullptr);
		}
		else if (In_Result.needExit())
		{
			AsyncTask(ENamedThreads::GameThread, [this]() {
					ExitGame();
				});
			return;
		}
		else
		{
		
			GSLOG(Log, TEXT("hivelog : AuthV4::setup In_Result = fail"));
			switch (In_Result.code)
			{
				case ResultAPI::Code::AuthV4InvalidParamDid:
				{
					// 설정된 app ID 및 HIVE 설정값이 정상인지 확인하세요.
					// 해당문제가 지속되는 경우 전달받은 에러 메시지와 함께 HIVE에 문의.
					//FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("hivelog : AuthV4::setup AuthV4InvalidParamDid")));
				}break;

				case ResultAPI::Code::AuthV4NetworkError:
				{
					// 기기의 네트워크 연결 상태를 확인하세요.
					// 기기의 네트워크 상태가 정상임에도 해당 에러가 발생하는 경우 에러 메시지와 함께 HIVE에 문의
					//FGsUIHelper::PopupSystemMsg(FText::FromString(TEXT("hivelog : AuthV4::setup AuthV4NetworkError")));
				}break;
			}

			// 공통 AuthV4 HIVE 유저 인증 상태 코드는 아래 함수를 확인해주세요.
			CheckAuthenticationState(In_Result);

			GSLOG(Log, TEXT("hivelog : AuthV4::setup In_Result = fail -> MessageSystem::HIVE_INIT_FALSE send"));
			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_INIT_FALSE, nullptr);
		}

		GSLOG(Log, TEXT("hivelog(AuthV4::setup) result : %s\nisAutoSignIn : %s\nDID:%s\ntypeList : %s"),
			*(convertResultAPIToFString(In_Result)),
			In_IsAutoSignIn ? TEXT("TRUE") : TEXT("FALSE"),
			*FString(In_Did.c_str()),
			*providerListString);
	});
}

void UGsHiveManager::AutoSignIn()
{
	//TODO: Enable to SignIn as Auto(ProviderType::AUTO)
	// 따로 PlayerInfo를 저장하지 않는건 AuthV4::getPlayerInfo()로 받을수 있기 때문이다. 해당정보에 문제가 있으면 추가하기로 하자.
	AuthV4::signIn(ProviderType::AUTO, [=](ResultAPI const & In_Result,PlayerInfo const & In_PlayerInfo)
	{
		if (In_Result.isSuccess())
		{
			_isSignIn = true;
			CurrentPlayer = In_PlayerInfo;

			//FGsLobbyFlowMsgParam param(MessageStage::SUCCEEDED_HIVE_AUTH);
			//GMessage()->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);		

			GSLOG(Log, TEXT("hivelog :AutoSignIn() Success -> MessageSystem::HIVE_AUTH_TRUE send"));

			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_TRUE, nullptr);
		}
		else if (In_Result.needExit())
		{
			AsyncTask(ENamedThreads::GameThread, [this]() {
				ExitGame();
				});
			return;
		}

		GSLOG(Log, TEXT("hivelog(AuthV4::signIn) result : %s\n"), *(convertResultAPIToFString(In_Result)));
	});
}

void UGsHiveManager::ShowConflictHelper()
{
	GSLOG(Log, TEXT("hivelog : AuthV4::Helper::showConflict call"));

	AuthV4::Helper::showConflict([=](ResultAPI const& In_Result, std::shared_ptr<PlayerInfo> In_PlayerInfo)
	{
		if (In_Result.needExit())
		{
			AsyncTask(ENamedThreads::GameThread, [this]() {
					ExitGame();
				});
			return;
		}

		switch (In_Result.code)
		{
			// 계정 전환: 게임 재시작 필요
		case ResultAPI::AuthV4PlayerChange:
		{
			GMessage()->GetSystem().SendMessageAsync(MessageSystem::RESTART_GAME);
			return;
		}
		break;
		// 현재 사용자 유지
		case ResultAPI::AuthV4PlayerResolved:
		{
			_isSignIn = true;

			GSLOG(Log, TEXT("hivelog : AuthV4::SignInHelper Success"));

			if (In_PlayerInfo != nullptr)
			{
				GSLOG(Log, TEXT("hivelog : AuthV4::SignInHelper Success, In_PlayerInfo != nullptr"));
				CurrentPlayer = *In_PlayerInfo;
			}
			else
			{
				GSLOG(Log, TEXT("hivelog : AuthV4::Helper::signIn Success, In_PlayerInfo == nullptr, SetCurPlayerInfo() call"));
				SetCurPlayerInfo();

				GSLOG(Log, TEXT("hivelog : CurrentPlayer Player ID :%lld\n"), CurrentPlayer.playerId);
				GSLOG(Log, TEXT("hivelog : CurrentPlayer PlayerName:%s\nDID :%s\ntoken :%s"),
					FUTF8ToTCHAR(CurrentPlayer.playerName.c_str()).Get(),
					*FString(CurrentPlayer.did.c_str()),
					*FString(CurrentPlayer.playerToken.c_str()));
			}

			GSLOG(Log, TEXT("hivelog : AuthV4::SignInHelper Success, MessageSystem::HIVE_AUTH_TRUE send"));

			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_TRUE, nullptr);
			return;
		}
		break;
		// 기기 인증에 실패하여 로그인이 해제된 경우입니다. AUTO로 재로그인을 시도하거나 로그아웃하세요.
		// showconflict 상황에서 기기 등록 취소 시 AuthV4NotRegisteredDevice가 내려가게 되며 이런 경우 게임에서 재접속 처리 후 다시 signin 처리해주시면 됩니다.
		case ResultAPI::Code::AuthV4NotRegisteredDevice:
		{
			// 테스트 결과 AuthV4NotRegisteredDevice가 온시점에 하이브에 연동된 계정들이 모두 없어짐(로그아웃되어짐, getPlayerInfo정보가 없음)
			//GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_SignOut, nullptr);
			// 하이브 팀에서 로그아웃은 하지 말라고 함
			_isSignIn = false;

#if PLATFORM_WINDOWS
			AsyncTask(ENamedThreads::GameThread, [this]() {
				SignOut();
				});
#else
			GMessage()->GetSystem().SendMessageAsync(MessageSystem::RESTART_GAME);
#endif
			
			return;
		}
		break;
		default:
		{
			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_FALSE, nullptr);
		}
		break;
		}

		GSLOG(Log, TEXT("hivelog(AuthV4::Helper::showConflict) result : %s\n"), *(convertResultAPIToFString(In_Result)));
	});
}

void UGsHiveManager::ShowConflictHelperLobbyAndInGame(TArray<TMap<FString, FString>> ConflictPlayerGameData, TArray<FString> ServerNameArray, TArray<FString> UserNameArray)
{
	GSLOG(Log, TEXT("hivelog : AuthV4::Helper::showConflict call"));

	ConflictSingleViewInfo conflictInfo = ConflictSingleViewInfo(ConflictPlayer.playerId);

	for (int32 i = 0; i < ConflictPlayerGameData.Num(); ++i)
	{
		FString ServerTitle = TEXT("1. Server");
		std::string StrServerTitle(TCHAR_TO_UTF8(*ServerTitle));
		FString serverName = ServerNameArray[i];
		std::string StrServerName(TCHAR_TO_UTF8(*serverName));

#if PLATFORM_WINDOWS
		conflictInfo.SetCharValue(StrServerTitle.c_str(), StrServerName.c_str());
#else
		conflictInfo.SetValue(StrServerTitle.c_str(), StrServerName.c_str());
#endif

		FString NameTitle = TEXT("2. UserName");
		std::string StrNameTitle(TCHAR_TO_UTF8(*NameTitle));
		FString UserName = UserNameArray[i];
		std::string StrUserName(TCHAR_TO_UTF8(*UserName));

#if PLATFORM_WINDOWS
		conflictInfo.SetCharValue(StrNameTitle.c_str(), StrUserName.c_str());
#else
		conflictInfo.SetValue(StrNameTitle.c_str(), StrUserName.c_str());
#endif

		for (auto& itr : ConflictPlayerGameData[i])
		{
			conflictInfo.SetValue(TCHAR_TO_UTF8(*itr.Key), TCHAR_TO_UTF8(*itr.Value));
			GSLOG(Log, TEXT("(*itr.Key) : %s, (*itr.Value) : %s"), *(itr.Key), *(itr.Value));
		}
	}

	AuthV4::Helper::showConflict(conflictInfo, [=](ResultAPI const& In_Result, std::shared_ptr<PlayerInfo> In_PlayerInfo)
	{
		if (In_Result.needExit())
		{
			AsyncTask(ENamedThreads::GameThread, [this]() {
					ExitGame();
				});
			return;
		}

		switch (In_Result.code)
		{
			// 계정 전환: 게임 재시작 필요
		case ResultAPI::AuthV4PlayerChange:
		{
			GMessage()->GetSystem().SendMessageAsync(MessageSystem::RESTART_GAME);
			return;
		}
		break;
		// 현재 사용자 유지
		case ResultAPI::AuthV4PlayerResolved:
		{
			_isSignIn = true;

			GSLOG(Log, TEXT("hivelog : AuthV4::SignInHelper Success"));

			if (In_PlayerInfo != nullptr)
			{
				GSLOG(Log, TEXT("hivelog : AuthV4::SignInHelper Success, In_PlayerInfo != nullptr"));
				CurrentPlayer = *In_PlayerInfo;
			}
			else
			{
				GSLOG(Log, TEXT("hivelog : AuthV4::Helper::signIn Success, In_PlayerInfo == nullptr, SetCurPlayerInfo() call"));
				SetCurPlayerInfo();

				GSLOG(Log, TEXT("hivelog : CurrentPlayer Player ID :%lld\n"), CurrentPlayer.playerId);
				GSLOG(Log, TEXT("hivelog : CurrentPlayer PlayerName:%s\nDID :%s\ntoken :%s"),
					FUTF8ToTCHAR(CurrentPlayer.playerName.c_str()).Get(),
					*FString(CurrentPlayer.did.c_str()),
					*FString(CurrentPlayer.playerToken.c_str()));
			}

			GSLOG(Log, TEXT("hivelog : AuthV4::SignInHelper Success, MessageSystem::HIVE_AUTH_TRUE send"));
		}
		break;
		// 기기 인증에 실패하여 로그인이 해제된 경우입니다. AUTO로 재로그인을 시도하거나 로그아웃하세요.
		// showconflict 상황에서 기기 등록 취소 시 AuthV4NotRegisteredDevice가 내려가게 되며 이런 경우 게임에서 재접속 처리 후 다시 signin 처리해주시면 됩니다.
		case ResultAPI::Code::AuthV4NotRegisteredDevice:
		{
			// 테스트 결과 AuthV4NotRegisteredDevice가 온시점에 하이브에 연동된 계정들이 모두 없어짐(로그아웃되어짐, getPlayerInfo정보가 없음)
			//GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_SignOut, nullptr);
			_isSignIn = false;

#if PLATFORM_WINDOWS
			AsyncTask(ENamedThreads::GameThread, [this]() {
				SignOut();
				});
#else
			GMessage()->GetSystem().SendMessageAsync(MessageSystem::RESTART_GAME);
#endif
			return;
		}
		break;
		default:
		{
			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_IdpConnectCancle, nullptr);			
		}
		break;
		}

		GSLOG(Log, TEXT("hivelog(AuthV4::Helper::showConflict) result : %s\n"), *(convertResultAPIToFString(In_Result)));
	});
}

void UGsHiveManager::SignInHelper()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("[Hive SignInHelper] AuthV4::Helper::signIn call"));

	FVector2D textScale{ 1.f, 1.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

#if FLOW_DUBUG_TEST
	GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_TRUE, nullptr);
#else
	GSLOG(Log, TEXT("hivelog : AuthV4::Helper::signIn call"));

#if PLATFORM_WINDOWS
	HIVESDKCEFImeEnable();
#endif
	AuthV4::Helper::signIn([=](ResultAPI const& In_Result, std::shared_ptr<PlayerInfo> In_PlayerInfo)
		{
#if PLATFORM_WINDOWS
			HIVESDKCEFImeDisable();
#endif
			if (In_Result.isSuccess())
			{
				_isSignIn = true;

				GSLOG(Log, TEXT("hivelog : AuthV4::SignInHelper Success"));

				if (In_PlayerInfo != nullptr)
				{
					GSLOG(Log, TEXT("hivelog : AuthV4::SignInHelper Success, In_PlayerInfo != nullptr"));
					CurrentPlayer = *In_PlayerInfo;
				}
				else
				{
					GSLOG(Log, TEXT("hivelog : AuthV4::Helper::signIn Success, In_PlayerInfo == nullptr, SetCurPlayerInfo() call"));
					SetCurPlayerInfo();

					GSLOG(Log, TEXT("hivelog : CurrentPlayer Player ID :%lld\n"), CurrentPlayer.playerId);
					GSLOG(Log, TEXT("hivelog : CurrentPlayer PlayerName:%s\nDID :%s\ntoken :%s"),
						FUTF8ToTCHAR(CurrentPlayer.playerName.c_str()).Get(),
						*FString(CurrentPlayer.did.c_str()),
						*FString(CurrentPlayer.playerToken.c_str()));
				}

				GSLOG(Log, TEXT("hivelog : AuthV4::SignInHelper Success, MessageSystem::HIVE_AUTH_TRUE send"));

				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_TRUE, nullptr);
				return;
			}
			else  if (In_Result.needExit())
			{
				AsyncTask(ENamedThreads::GameThread, [this]() {
						ExitGame();
					});
				return;
			}
			else
			{
				GSLOG(Log, TEXT("hivelog : AuthV4::Helper::signIn fail"));

				switch (In_Result.code)
				{
				case ResultAPI::AuthV4ConflictPlayer:
					{
						// 계정 충돌
						// 계정 충돌 UI를 노출하여 충돌 상황을 해결 할 수 있도록 하세요.
						if (In_PlayerInfo != nullptr)
							ConflictPlayer = *In_PlayerInfo;

						//GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_IDP_CONFLICT_PLAYER, nullptr);			

						//thread 안에서 호출하지 않도록 게임 thread queue 로 빼준다
						AsyncTask(ENamedThreads::GameThread_Local, [this]() {
								ShowConflictHelper();
							});
						return;
					}
					break;
				case ResultAPI::AuthV4HelperImplifiedLoginFail:
					// 묵시적 로그인에 실패
					// 명시적 로그인 UI를 노출하여 로그인을 진행하세요.
					// ShowSignIn();
					GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_Show_SignIn, nullptr);
					return;

					break;
				// 기기 인증에 실패하여 로그인이 해제된 경우입니다. AUTO로 재로그인을 시도하거나 로그아웃하세요.
				// showconflict 상황에서 기기 등록 취소 시 AuthV4NotRegisteredDevice가 내려가게 되며 이런 경우 게임에서 재접속 처리 후 다시 signin 처리해주시면 됩니다.
				case ResultAPI::Code::AuthV4NotRegisteredDevice:				
					// 테스트 결과 AuthV4NotRegisteredDevice가 온시점에 하이브에 연동된 계정들이 모두 없어짐(로그아웃되어짐, getPlayerInfo정보가 없음)
					_isSignIn = false;
					GMessage()->GetSystem().SendMessageAsync(MessageSystem::RESTART_GAME);
					return;

					break;
				}

				GSLOG(Log, TEXT("hivelog : AuthV4::SignInHelper fail -> MessageSystem::HIVE_AUTH_FALSE send"));

				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_FALSE, nullptr);
			}

			GSLOG(Log, TEXT("hivelog(AuthV4::Helper::signIn) result : %s\n"), *(convertResultAPIToFString(In_Result)));
		});

#endif
}

void UGsHiveManager::ShowSignIn()
{
	GSLOG(Log, TEXT("hivelog : AuthV4::ShowSignIn call"));

#if PLATFORM_WINDOWS
	HIVESDKCEFImeEnable();
#endif
//#if PLATFORM_IOS
//	dispatch_async(dispatch_get_main_queue(), ^{
//#endif
	AuthV4::showSignIn([=](ResultAPI const& In_Result, PlayerInfo const& In_PlayerInfo)
	{
#if PLATFORM_WINDOWS
		HIVESDKCEFImeDisable();
#endif

		if (In_Result.isSuccess())
		{
			_isSignIn = true;
			CurrentPlayer = In_PlayerInfo;

			GSLOG(Log, TEXT("hivelog : AuthV4::ShowSignIn Success -> MessageSystem::HIVE_AUTH_TRUE call"));

			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_TRUE, nullptr);
			return;
		}
		else  if (In_Result.needExit())
		{
			AsyncTask(ENamedThreads::GameThread, [this]() {
					ExitGame();
				});
			return;
		}
		else
		{
			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_FALSE, nullptr);
			
			//case ResultAPI::Code::AuthV4JsonException:
			//	// 전달받은 에러 메시지와 함께 HIVE에 문의주세요
			//	break;
			//case ResultAPI::Code::AuthV4CancelDialog:
			//	// 로그인 취소에 대한 안내 문구를 노출 후 ShowSignIn API를 재 호출 하여 로그인을 수행하도록 하세요.
			//	break;
			//case ResultAPI::Code::AuthV4NetworkError:
			//	// 기기의 네트워크 연결 상태를 확인하세요.
			//	// 기기의 네트워크 연결 상태가 정상임에도 해당 에러가 발생하는 경우 에러 메시지와 함께 HIVE 에 문의주세요.
			//	break;
			//	//추가로 AuthV4.signIn와 동일한 ResultAPI 코드가 전달됩니다.
			//}
			//// 공통 AuthV4 HIVE 유저 인증 상태 코드는 아래 함수를 확인해주세요.
			//CheckAuthenticationState(In_Result);

			//GSLOG(Log, TEXT("hivelog : AuthV4::ShowSignIn fail"));
		}		

		GSLOG(Log, TEXT("hivelog(AuthV4::showSignIn) result : %s\n"), *(convertResultAPIToFString(In_Result)));
	});
//#if PLATFORM_IOS
//	});
//#endif
}

void UGsHiveManager::SignIn()
{
	GSLOG(Log, TEXT("hivelog : AuthV4::signIn call"));
	
	ProviderType type = ProviderType::HIVE;
#if PLATFORM_WINDOWS
	HIVESDKCEFImeEnable();
#endif
	AuthV4::signIn(type, [=](ResultAPI const& In_Result, PlayerInfo const& In_PlayerInfo)
	{
#if PLATFORM_WINDOWS
		HIVESDKCEFImeDisable();
#endif
		if (In_Result.isSuccess())
		{
			_isSignIn = true;
			CurrentPlayer = In_PlayerInfo;

			GSLOG(Log, TEXT("hivelog : AuthV4::signIn Success -> MessageSystem::HIVE_AUTH_TRUE send"));
		
			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_TRUE, nullptr);
		}
		else  if (In_Result.needExit())
		{
			AsyncTask(ENamedThreads::GameThread, [this]() {
					ExitGame();
				});
			return;
		}
		else
		{
			switch (In_Result.code)
			{
				case ResultAPI::Code::AuthV4NetworkError:
					// 기기의 네트워크 연결 상태를 확인하세요.
					// 기기의 네트워크 연결 상태가 정상임에도 해당 에러가 발생하는 경우 에러 메시지와 함께 HIVE 에 문의주세요.
					break;
				case ResultAPI::Code::AuthV4ResponseFailProviderUserID:
					// 일시적인 현상으로 해당 문제가 발생할 수 있습니다.
					// 해당 에러가 지속적으로 발생하는 경우 에러 메시지와 함께 HIVE 에 문의주세요.
					break;
					/***** ProviderType = HIVE 인 경우 *****/
				case ResultAPI::Code::AuthV4HIVECancel:
					// 유저가 HIVE 로그인을 재시도 혹은 다른 Provider를 통해 로그인을 할 수 있는 로그인 창을 노출해주세요.
					break;
				case ResultAPI::Code::AuthV4HIVEInvalidParam:
					// 전달받은 에러 메시지와 함께 HIVE에 문의주세요
					break;
				case ResultAPI::Code::AuthV4HIVEResponseError:
					// 네트워크 연결에 문제가 있는 경우 발생하기도 하므로 기기의 네트워크 연결 상태를 확인하세요.
					// 기기의 네트워크 연결 상태가 정상임에도 해당 에러가 발생하는 경우 에러 메시지와 함께 HIVE 에 문의주세요.
					break;
					/***** ProviderType = Facebook 인 경우 *****/
				case ResultAPI::Code::AuthV4FacebookResponseError:
					// Facebook 프레임워크 및 설정이 정상적으로 적용되어 있는지 확인해주세요.
					break;
				case ResultAPI::Code::AuthV4FacebookCancel:
					// 유저가 Facebook 로그인을 재시도 혹은 다른 Provider를 통해 로그인 할 수 있는 로그인 창을 노출해주세요.
					break;
				case ResultAPI::Code::AuthV4FacebookInvalidResponseData:
					// 전달받은 에러 메시지와 함께 HIVE에 문의주세요
					break;
					/***** ProviderType = Game Center 인 경우 *****/
				case ResultAPI::Code::AuthV4AppleCancel:
					// 유저가 다른 Provider를 통해 로그인을 할 수 있는 로그인 창을 노출해주세요.
					// AuthV4.helper.showGameCenterLoginCancelDialog API를 호출하여 Game Center 취소 안내 문구를 노출하세요.
					break;
				case ResultAPI::Code::AuthV4AppleLoginCancel:
					// AuthV4.helper.showGameCenterLoginCancelDialog API를 호출하여 Game Center 취소 안내 문구를 노출하세요.
					break;
				case ResultAPI::Code::AuthV4AppleResponseError:
					// 전달 받은 에러 메시지 내의 GameCenter 에러 값을 확인하세요.
					break;
				case ResultAPI::Code::AuthV4AppleResponseFailLogin:
					// 네트워크 상태가 불안정 하거나 GameCenter 정보가 등록된 정보와 불일치하여 발생합니다.
					// 위의 정보들이 정상적으로 되어있음에도 지속적으로 에러가 발생하는 경우 전달받은 에러 메시지와 함께 HIVE에 문의주세요
					break;
					/***** ProviderType = Google 인 경우 *****/
				case ResultAPI::Code::AuthV4GoogleResponseFailLogin:
					// Google 로그인을 재 시도하도록 안내해주세요.
					// 지속적으로 발생하는 경우 전달받은 에러 메시지와 함께 HIVE에 문의주세요.
					break;
				case ResultAPI::Code::AuthV4ProviderLoginCancel:
					// 유저가 Google 로그인을 재시도 혹은 다른 Provider를 통해 로그인 할 수 있는 로그인 창을 노출해주세요.
					break;
					/***** ProviderType = QQ 인 경우 *****/
				case ResultAPI::Code::AuthV4QQResponseFailLogin:
					// 네트워크 연결에 문제가 있는 경우 발생하기도 하므로 기기의 네트워크 연결 상태를 확인하세요.
					// 기기의 네트워크 연결 상태가 정상임에도 해당 에러가 발생하는 경우 에러 메시지와 함께 HIVE 에 문의주세요.
					break;
				case ResultAPI::Code::AuthV4QQResponseError:
					// QQ로그인을 재 시도하도록 안내해주세요.
					// 지속적으로 발생하는 경우 전달받은 에러 메시지와 함께 HIVE에 문의주세요
					break;
				case ResultAPI::Code::AuthV4QQCancel:
					// 유저가 QQ 로그인을 재시도 혹은 다른 Provider를 통해 로그인 할 수 있는 로그인 창을 노출해주세요,
					break;
				case ResultAPI::Code::AuthV4QQNetworkError:
					// 기기의 네트워크 연결 상태를 확인하세요.
					// 기기의 네트워크 연결 상태가 정상임에도 해당 에러가 발생하는 경우 에러 메시지와 함께 HIVE 에 문의주세요.
					break;
				case ResultAPI::Code::AuthV4QQInvalidResponseData:
					// QQ로그인을 재 시도하도록 안내해주세요.
					// 지속적으로 발생하는 경우 전달받은 에러 메시지와 함께 HIVE에 문의주세요
					break;
					/***** ProviderType = WeChat 인 경우 *****/
				case ResultAPI::Code::AuthV4WechatInvalidResponseData:
					// 일시적인 현상으로 해당 문제가 발생할 수 있습니다.
					// 해당 에러가 지속적으로 발생하는 경우 에러 메시지와 함께 HIVE 에 문의주세요.
					break;
				case ResultAPI::Code::AuthV4WechatResponseError:
					// WeChat 로그인을 재 시도하도록 안내해주세요.
					// 지속적으로 발생하는 경우 전달받은 에러 메시지와 함께 HIVE에 문의주세요
					break;
				case ResultAPI::Code::AuthV4WechatResponseFailLogin:
					// 일시적인 현상으로 해당 문제가 발생할 수 있습니다.
					// 해당 에러가 지속적으로 발생하는 경우 에러 메시지와 함께 HIVE 에 문의주세요.
					break;
				case ResultAPI::Code::AuthV4WechatNetworkError:
					// 기기의 네트워크 연결 상태를 확인하세요.
					// 기기의 네트워크 연결 상태가 정상임에도 해당 에러가 발생하는 경우 에러 메시지와 함께 HIVE 에 문의주세요.
					break;
				case ResultAPI::Code::AuthV4WechatNotSupportedRequest:
					// WeChat App 버전을 업그레이드 하거나 설치하도록 안내하세요.
					break;
					/***** ProviderType = VK 인 경우 *****/
				case ResultAPI::Code::AuthV4VKResponseError:
					// VK 로그인을 재 시도하도록 안내해주세요.
					// 지속적으로 발생하는 경우 전달받은 에러 메시지와 함께 HIVE에 문의주세요.
					break;
				case ResultAPI::Code::AuthV4VKCancel:
					// 유저가 VK 로그인을 재시도 혹은 다른 Provider를 통해 로그인 할 수 있는 로그인 창을 노출해주세요,
					break;
				case ResultAPI::Code::AuthV4VKTokenResponseError:
					// 네트워크 상태가 불안정 하거나 VK 정보가 등록된 정보와 불일치하여 발생합니다.
					// 위의 정보들이 정상적으로 되어있음에도 지속적으로 에러가 발생하는 경우 전달받은 에러 메시지와 함께 HIVE에 문의주세요.
					break;
					/***** ProviderType = Apple 인 경우 *****/
				case ResultAPI::Code::AuthV4SignInAppleCanceled:
					// 유저가 Apple 로그인을 재시도 혹은 다른 Provider를 통해 로그인 할 수 있는 로그인 창을 노출해주세요.
					break;
				case ResultAPI::Code::AuthV4SignInAppleUnknown:
					// 유저가 Apple 로그인을 재시도 혹은 다른 Provider를 통해 로그인 할 수 있는 로그인 창을 노출해주세요.
					break;
				case ResultAPI::Code::AuthV4SignInAppleFailed:
					// 일시적인 현상으로 해당 문제가 발생할 수 있습니다.
					// 해당 에러가 지속적으로 발생하는 경우 에러 메시지와 함께 HIVE 에 문의주세요.
					break;
				case ResultAPI::Code::AuthV4SignInAppleInvalidResponse:
					// 일시적인 현상으로 해당 문제가 발생할 수 있습니다.
					// 해당 에러가 지속적으로 발생하는 경우 에러 메시지와 함께 HIVE 에 문의주세요.
					break;
				case ResultAPI::Code::AuthV4SignInAppleNotHandled:
					// 일시적인 현상으로 해당 문제가 발생할 수 있습니다.
					// 해당 에러가 지속적으로 발생하는 경우 에러 메시지와 함께 HIVE 에 문의주세요.
					break;
			}

			GSLOG(Log, TEXT("hivelog : AuthV4::SignIn fail -> MessageSystem::HIVE_AUTH_FALSE send"));
			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_FALSE, nullptr);
		}
		// 공통 AuthV4 HIVE 유저 인증 상태 코드는 아래 함수를 확인해주세요.
		CheckAuthenticationState(In_Result);
	});
}

void UGsHiveManager::SignOut()
{
	GSLOG(Log, TEXT("hivelog : AuthV4::signOut call"));

#if FLOW_DUBUG_TEST
	//// 계정선택
	//SignInHelper();
	// 로그아웃 성공
	GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_LOGOUT_TRUE, nullptr);
#else
	AuthV4::Helper::signOut([=](ResultAPI const& In_Result, std::shared_ptr<PlayerInfo> playerInfo)
		{
			GSLOG(Log, TEXT("\nSign Out result : %s"), *(convertResultAPIToFString(In_Result)));
			if (In_Result.isSuccess())
			{
				_isSignIn = false;
				
				// 로그아웃 성공
				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_LOGOUT_TRUE, nullptr);
				return;
			}
			else  if (In_Result.needExit())
			{
				AsyncTask(ENamedThreads::GameThread, [this]() {
						ExitGame();
					});
				return;
			}
			else
			{
				GSLOG(Log, TEXT("\nSign Out result : fail"));

				if (playerInfo != nullptr)
				{
					GSLOG(Log, TEXT("\nSign Out result : fail -> playerInfo != nullptr"));

					GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_AUTH_LOGOUT_FALSE, nullptr);

					GSLOG(Log, TEXT("player ID : %lld"), playerInfo->playerId);
					GSLOG(Log, TEXT("DID :%s\n token :%s\n"), *FString(playerInfo->did.c_str()), *FString(playerInfo->playerToken.c_str()));
				}
				else
				{
					GSLOG(Log, TEXT("\nSign Out result : fail -> playerInfo == nullptr"));
					GMessage()->GetSystem().SendMessageAsync(MessageSystem::RESTART_GAME);
				}
			}
		});

#endif
}

void UGsHiveManager::CheckMaintenance(bool isRetry)
{
	/*if (isCheckAuthMaintenance)
	{
		SetServerIdByUserSelectServer(GDivergence()->GetRegionServerId());
	}*/

	// isShow 값이 true 이면 HIVE SDK 내부에서 제재 유저 팝업을 띄운다.
	// isShow 값이 false 이면 게임에서 제재 유저 팝업을 띄우기 위한 데이터를 반환한다.
	AuthV4::checkMaintenance(true, [=](ResultAPI const& result, std::vector<AuthV4MaintenanceInfo> const& maintenanceInfolist) 
		{
			//GSLOG(Log, TEXT("\nSign Out result : %s"), *FString(result.toString().c_str()));

			if (nullptr == GScope())
			{
				GSLOG(Error, TEXT("checkMaintenance nullptr == GScope"));
				return;
			}

			FGsMessageHolder* Message = GMessage();
			if (nullptr == Message)
			{
				GSLOG(Error, TEXT("checkMaintenance nullptr == Message"));
				return;
			}

			if (result.isSuccess())
			{
				// 결과가 SUCCESS 이지만 점검 공지 내용이 없다면 handler 의 maintenanceInfo 는 비어있게 된다.
				// 성공은 했지만 서버점검 내용이 없으니 정상 진행
				if (maintenanceInfolist.empty() == true)
				{					
					GSLOG(Log, TEXT("maintenanceInfolist.empty() == true, MessageSystem::HIVE_CHECK_MAINTENANCE_TRUE"));
					Message->GetSystem().SendMessageAsync(MessageSystem::HIVE_CHECK_MAINTENANCE_TRUE);

					return;
				}
			}
			else if (result.needExit())
			{
				AsyncTask(ENamedThreads::GameThread, [this]() {
						ExitGame();
					});
				return;
			}
			else
			{

				switch (result.code)
				{
				case ResultAPI::Code::AuthV4NetworkError:
				case ResultAPI::Code::AuthV4InvalidResponseData:
				case ResultAPI::Code::AuthV4ServerResponseError:
				{
					if (isRetry)
					{
						GSLOG(Log, TEXT("MessageSystem::HIVE_CHECK_MAINTENANCE_RETRY_FAILED"));
						Message->GetSystem().SendMessageAsync(MessageSystem::HIVE_CHECK_MAINTENANCE_RETRY_FAILED);
						return;
					}
					else
					{
						GSLOG(Log, TEXT("MessageSystem::HIVE_CHECK_MAINTENANCE_RETRY"));
						Message->GetSystem().SendMessageAsync(MessageSystem::HIVE_CHECK_MAINTENANCE_RETRY);
						return;
					}
				}
				break;
				}

				Message->GetSystem().SendMessageAsync(MessageSystem::HIVE_CHECK_MAINTENANCE_FALSE);
			}
			
		});
}

void UGsHiveManager::SetCurPlayerInfo()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	CurrentPlayer = AuthV4::getPlayerInfo();
#else
	auto infoString = AuthV4GetPlayerInfo();
	std::string strInfo(infoString);

	//Char to JSon	
	picojson::value playerInfoJson;
	std::string err = picojson::parse(playerInfoJson, strInfo);
	CurrentPlayer.playerId = playerInfoJson.get("playerId").get<PlayerID>();
	CurrentPlayer.did = playerInfoJson.get("did").get<std::string>();
	CurrentPlayer.playerImageUrl = playerInfoJson.get("playerImageUrl").get<std::string>();
	CurrentPlayer.playerName = playerInfoJson.get("playerName").get<std::string>();
	CurrentPlayer.playerToken = playerInfoJson.get("playerToken").get<std::string>();
	picojson::object providerInfoData = playerInfoJson.get("providerInfoData").get<picojson::object>();
	std::map<hive::ProviderType, hive::ProviderInfo> mapProviderInfoData;
	for (picojson::value::object::const_iterator i = providerInfoData.begin();
		i != providerInfoData.end();
		++i) {
		auto value = i->second;
		hive::ProviderInfo __proInfo;
		__proInfo.providerType = (hive::ProviderType)value.get("providerType").get<int64_t>();
		__proInfo.providerUserId = value.get("providerUserId").get<std::string>();
		__proInfo.providerEmail = value.get("providerEmail").get<std::string>();
		mapProviderInfoData[(hive::ProviderType)(stoi(i->first))] = __proInfo;
		//std::cout << i->first << ': ' << i->second.to_str() << std::endl;
	}
	CurrentPlayer.providerInfoData = mapProviderInfoData;
#endif
}

PlayerID UGsHiveManager::GetPlayerID()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	PlayerInfo playerInfo = AuthV4::getPlayerInfo();
#else
	auto infoString = AuthV4GetPlayerInfo();
	std::string strInfo(infoString);

	//Char to JSon
	PlayerInfo playerInfo;
	picojson::value playerInfoJson;
	std::string err = picojson::parse(playerInfoJson, strInfo);

	playerInfo.playerId = playerInfoJson.get("playerId").get<PlayerID>();
	playerInfo.did = playerInfoJson.get("did").get<std::string>();
	playerInfo.playerImageUrl = playerInfoJson.get("playerImageUrl").get<std::string>();
	playerInfo.playerName = playerInfoJson.get("playerName").get<std::string>();
	playerInfo.playerToken = playerInfoJson.get("playerToken").get<std::string>();
	picojson::object providerInfoData = playerInfoJson.get("providerInfoData").get<picojson::object>();
	//for(picojson::object::iterator itr = providerInfoData)
	std::map<hive::ProviderType, hive::ProviderInfo> mapProviderInfoData;
	for (picojson::value::object::const_iterator i = providerInfoData.begin();
		i != providerInfoData.end();
		++i) {
		auto value = i->second;
		hive::ProviderInfo __proInfo;
		__proInfo.providerType = (hive::ProviderType)value.get("providerType").get<int64_t>();
		__proInfo.providerUserId = value.get("providerUserId").get<std::string>();
		__proInfo.providerEmail = value.get("providerEmail").get<std::string>();
		mapProviderInfoData[(hive::ProviderType)(stoi(i->first))] = __proInfo;
		//std::cout << i->first << ': ' << i->second.to_str() << std::endl;
	}
	playerInfo.providerInfoData = mapProviderInfoData;
#endif

	return playerInfo.playerId;
}

FString UGsHiveManager::GetPlayerToken()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	PlayerInfo playerInfo = AuthV4::getPlayerInfo();
#else
	auto infoString = AuthV4GetPlayerInfo();
	std::string strInfo(infoString);

	//Char to JSon
	PlayerInfo playerInfo;
	picojson::value playerInfoJson;
	std::string err = picojson::parse(playerInfoJson, strInfo);
	playerInfo.playerId = playerInfoJson.get("playerId").get<PlayerID>();
	playerInfo.did = playerInfoJson.get("did").get<std::string>();
	playerInfo.playerImageUrl = playerInfoJson.get("playerImageUrl").get<std::string>();
	playerInfo.playerName = playerInfoJson.get("playerName").get<std::string>();
	playerInfo.playerToken = playerInfoJson.get("playerToken").get<std::string>();
	picojson::object providerInfoData = playerInfoJson.get("providerInfoData").get<picojson::object>();
	std::map<hive::ProviderType, hive::ProviderInfo> mapProviderInfoData;
	for (picojson::value::object::const_iterator i = providerInfoData.begin();
		i != providerInfoData.end();
		++i) {
		auto value = i->second;
		hive::ProviderInfo __proInfo;
		__proInfo.providerType = (hive::ProviderType)value.get("providerType").get<int64_t>();
		__proInfo.providerUserId = value.get("providerUserId").get<std::string>();
		__proInfo.providerEmail = value.get("providerEmail").get<std::string>();
		mapProviderInfoData[(hive::ProviderType)(stoi(i->first))] = __proInfo;
		//std::cout << i->first << ': ' << i->second.to_str() << std::endl;
	}
	playerInfo.providerInfoData = mapProviderInfoData;
#endif

	return playerInfo.playerToken.c_str();
}

FString UGsHiveManager::GetDid()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	PlayerInfo playerInfo = AuthV4::getPlayerInfo();
#else
	auto infoString = AuthV4GetPlayerInfo();
	std::string strInfo(infoString);

	//Char to JSon
	PlayerInfo playerInfo;
	picojson::value playerInfoJson;
	std::string err = picojson::parse(playerInfoJson, strInfo);
	playerInfo.playerId = playerInfoJson.get("playerId").get<PlayerID>();
	playerInfo.did = playerInfoJson.get("did").get<std::string>();
	playerInfo.playerImageUrl = playerInfoJson.get("playerImageUrl").get<std::string>();
	playerInfo.playerName = playerInfoJson.get("playerName").get<std::string>();
	playerInfo.playerToken = playerInfoJson.get("playerToken").get<std::string>();
	picojson::object providerInfoData = playerInfoJson.get("providerInfoData").get<picojson::object>();
	std::map<hive::ProviderType, hive::ProviderInfo> mapProviderInfoData;
	for (picojson::value::object::const_iterator i = providerInfoData.begin();
		i != providerInfoData.end();
		++i) {
		auto value = i->second;
		hive::ProviderInfo __proInfo;
		__proInfo.providerType = (hive::ProviderType)value.get("providerType").get<int64_t>();
		__proInfo.providerUserId = value.get("providerUserId").get<std::string>();
		__proInfo.providerEmail = value.get("providerEmail").get<std::string>();
		mapProviderInfoData[(hive::ProviderType)(stoi(i->first))] = __proInfo;
		//std::cout << i->first << ': ' << i->second.to_str() << std::endl;
	}
	playerInfo.providerInfoData = mapProviderInfoData;
#endif

	return playerInfo.did.c_str();
}

bool UGsHiveManager::IsGuest()
{
	GSLOG(Log, TEXT("IsGuest call"));

	for (auto providerInfo : AuthV4::getPlayerInfo().providerInfoData)
	{
		GSLOG(Log, TEXT("\ngetPlayerInfo().providerType : %s"), *FString(ProviderInfo::stringProviderType(providerInfo.second.providerType).c_str()));
		GSLOG(Log, TEXT("\ngetPlayerInfo().providerUserId : %s"), *FString(providerInfo.second.providerUserId.c_str()));
	}

	for (auto providerInfo : AuthV4::getPlayerInfo().providerInfoData) 
	{
		if ((providerInfo.second.providerType == ProviderType::GUEST)
			&& (providerInfo.second.providerUserId != ""))
		{
			return true;
		}
	}

	return false;
}

bool UGsHiveManager::IsIdpConnected(EGsProviderType In_TypeValue)
{
	ProviderType providerType = ProviderType::AUTO;
	switch (In_TypeValue)
	{
	case EGsProviderType::HIVE:
		providerType = ProviderType::HIVE;
		break;
	case EGsProviderType::FACEBOOK:
		providerType = ProviderType::FACEBOOK;
		break;
	case EGsProviderType::GOOGLE:
		providerType = ProviderType::GOOGLE;
		break;
	case EGsProviderType::APPLE:
		providerType = ProviderType::SIGNIN_APPLE;
		break;
	}

	FString typeString = FString(ProviderInfo::stringProviderType(providerType).c_str());
#if PLATFORM_IOS || PLATFORM_ANDROID
	for (auto providerInfo : AuthV4::getPlayerInfo().providerInfoData)
	{
		FString providerUserIdString = FString(providerInfo.second.providerUserId.c_str());
		FString providerNameString = FString(providerInfo.second.providerName.c_str());

		if ((providerInfo.second.providerType == providerType)
			&& (providerInfo.second.providerUserId != ""))
		{
			GSLOG(Log, TEXT("[HiveManager] ProviderType : %s ProviderUserId : %s, ProviderName : %s, connected"), *typeString, *providerUserIdString, *providerNameString);
			
			return true;
		}
		else
		{
			GSLOG(Log, TEXT("[HiveManager] ProviderType : %s ProviderUserId : %s, ProviderName : %s, not connected"), *typeString, *providerUserIdString, *providerNameString);
		}
	}

	GSLOG(Log, TEXT("[HiveManager] ProviderType : %s, not connected"), *typeString);
#else
	auto infoString = AuthV4GetPlayerInfo();
	std::string strInfo(infoString);

	//Char to JSon
	PlayerInfo _playerInfo;
	picojson::value playerInfoJson;
	std::string err = picojson::parse(playerInfoJson, strInfo);
	picojson::object providerInfoData = playerInfoJson.get("providerInfoData").get<picojson::object>();

	for (picojson::value::object::const_iterator i = providerInfoData.begin(); i != providerInfoData.end(); ++i)
	{
		auto value = i->second;
		hive::ProviderInfo proInfo;
		proInfo.providerType = (hive::ProviderType)value.get("providerType").get<int64_t>();
		proInfo.providerUserId = value.get("providerUserId").get<std::string>();

		if ((proInfo.providerType == providerType)
			&& (proInfo.providerUserId != ""))
		{
			GSLOG(Log, TEXT("[HiveManager] ProviderType : %s, connected"), *typeString);
			return true;
		}
	}

	GSLOG(Log, TEXT("[HiveManager] ProviderType : %s, not connected"), *typeString);
#endif
	
	return false;
}

bool UGsHiveManager::IsHive()
{
	for (auto providerInfo : AuthV4::getPlayerInfo().providerInfoData)
	{
		if ((providerInfo.second.providerType == ProviderType::HIVE)
			&& (providerInfo.second.providerUserId != ""))
		{
			return true;
		}
	}

	return false;
}

bool UGsHiveManager::IsGoogle()
{
	for (auto providerInfo : AuthV4::getPlayerInfo().providerInfoData)
	{
		if ((providerInfo.second.providerType == ProviderType::GOOGLE)
			&& (providerInfo.second.providerUserId != ""))
		{
			return true;
		}
	}

	return false;
}

bool UGsHiveManager::IsApple()
{
	for (auto providerInfo : AuthV4::getPlayerInfo().providerInfoData)
	{
		if ((providerInfo.second.providerType == ProviderType::SIGNIN_APPLE)
			&& (providerInfo.second.providerUserId != ""))
		{
			return true;
		}
	}

	return false;
}

bool UGsHiveManager::IsFacebook()
{
	for (auto providerInfo : AuthV4::getPlayerInfo().providerInfoData)
	{
		if ((providerInfo.second.providerType == ProviderType::FACEBOOK)
			&& (providerInfo.second.providerUserId != ""))
		{
			return true;
		}
	}

	return false;
}

void UGsHiveManager::Connect(EGsProviderType In_ProviderType)
{
	ProviderType providerType = ProviderType::AUTO;
	switch (In_ProviderType)
	{
	case EGsProviderType::HIVE:
		providerType = ProviderType::HIVE;
		break;
	case EGsProviderType::FACEBOOK:
		providerType = ProviderType::FACEBOOK;
		break;
	case EGsProviderType::GOOGLE:
		providerType = ProviderType::GOOGLE;
		break;
	case EGsProviderType::APPLE:
		providerType = ProviderType::SIGNIN_APPLE;
		break;
	}
	
	// Hive SDK AuthV4 커넥트(연동) 요청
	AuthV4::Helper::connect(providerType, [=](ResultAPI const& result, std::shared_ptr<PlayerInfo> playerInfo)
		{
			GSLOG(Log, TEXT("\nConnect Out result : %s"), *(convertResultAPIToFString(result)));

			if (result.isSuccess())
			{
				// 연동 성공					
				GMessage()->GetSystemInt().SendMessageAsync(MessageSystem::HIVE_CONNECT_SUCCESS, static_cast<int32>(providerType));
				return;
			}
			else if (result.needExit())
			{
				AsyncTask(ENamedThreads::GameThread, [this]() {
						ExitGame();
					});
				return;
			}
			else
			{
				switch (result.code)
				{
				case ResultAPI::Code::AuthV4ConflictPlayer:
				{
					// 계정 충돌
					if (playerInfo != nullptr)
						ConflictPlayer = *playerInfo;

					GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_IDP_CONFLICT_PLAYER, nullptr);
					return;
				}
				break;
				case ResultAPI::Code::AuthV4ProviderLoginCancel:
					break;
				default:
					// 기타 예외 상황
					break;
				}
				GMessage()->GetSystemInt().SendMessageAsync(MessageSystem::HIVE_CONNECT_FAIL, static_cast<int32>(providerType));
			}
		});
}

void UGsHiveManager::DisConnect(EGsProviderType In_ProviderType)
{
	ProviderType providerType = ProviderType::AUTO;
	switch (In_ProviderType)
	{
	case EGsProviderType::HIVE:
		providerType = ProviderType::HIVE;
		break;
	case EGsProviderType::FACEBOOK:
		providerType = ProviderType::FACEBOOK;
		break;
	case EGsProviderType::GOOGLE:
		providerType = ProviderType::GOOGLE;
		break;
	case EGsProviderType::APPLE:
		providerType = ProviderType::SIGNIN_APPLE;
		break;
	}

	// Hive SDK AuthV4 디스-커넥트(연동 해제) 요청
	AuthV4::Helper::disconnect(providerType, [=](ResultAPI const& In_Result, std::shared_ptr<PlayerInfo> playerInfo)
		{
			GSLOG(Log, TEXT("\nAuthV4::Helper::disconnect result : %s"), *(convertResultAPIToFString(In_Result)));

			if (In_Result.isSuccess())
			{
				// Provider 연결 해제 성공
				// 연동 성공					
				GMessage()->GetSystemInt().SendMessageAsync(MessageSystem::HIVE_DISCONNECT_SUCCESS, static_cast<int32>(providerType));
				return;
			}
			else if(In_Result.needExit())
			{
				AsyncTask(ENamedThreads::GameThread, [this]() {
						ExitGame();
					});
				return;
			}
			else
			{
				GMessage()->GetSystemInt().SendMessageAsync(MessageSystem::HIVE_DISCONNECT_FAIL, static_cast<int32>(providerType));
			}
		});
}

void UGsHiveManager::PushGetRemote()
{
	GSLOG(Log, TEXT("\n excute PushGetRemote"));

	Push::getRemotePush([=](ResultAPI const& In_Result, RemotePush const& In_RemotePushSetting)
		{
			GSLOG(Log, TEXT("\n PushGetRemote\n result = %s\n Remote Push Setting = %s\n"), *(convertResultAPIToFString(In_Result)), *FString(In_RemotePushSetting.toString().c_str()));

			_Night = In_RemotePushSetting.isAgreeNight;
			_Notice = In_RemotePushSetting.isAgreeNotice;

			FGsHivePushMessageParam param(_Night, _Notice);
			GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_PUSH, &param);
		});
}

void UGsHiveManager::PushSetRemote(bool bNotice, bool bNight)
{
	GSLOG(Log, TEXT("\n excute PushSetRemote bNotice : %d, bNight : %d"), static_cast<int>(bNotice), static_cast<int>(bNight));

	RemotePush remotePush;
	remotePush.isAgreeNight = bNight;
	remotePush.isAgreeNotice = bNotice;
	Push::setRemotePush(remotePush, [=](ResultAPI const& In_Result, RemotePush const& In_RemotePushSetting)
		{
			if (In_Result.isSuccess())
			{
				GSLOG(Log, TEXT("\n PushSetRemote\n result = %s\n Remote Push Setting = %s\n"), *(convertResultAPIToFString(In_Result)), *FString(In_RemotePushSetting.toString().c_str()));

				_Night = In_RemotePushSetting.isAgreeNight;
				_Notice = In_RemotePushSetting.isAgreeNotice;
			}
		});
}

void UGsHiveManager::PushSetForeground(bool bLocalPush, bool bRemotePush)
{
	GSLOG(Log, TEXT("\n excute setForegroundPush"));
	PushSetting localPushSetting(bRemotePush, bLocalPush);

	Push::setForegroundPush(localPushSetting, [=](ResultAPI const& In_Result, PushSetting const& In_PushSetting)
		{
			if (In_Result.isSuccess())
			{
				GSLOG(Log, TEXT("\n setForegroundPush\n result = %s\n Foreground Push Setting = %s\n"), *(convertResultAPIToFString(In_Result)), *FString(In_PushSetting.toString().c_str()));

				_LocalPush = In_PushSetting.useForegroundLocalPush;
				_RemotePush = In_PushSetting.useForegroundRemotePush;
			}
		});
}

void UGsHiveManager::PushGetForeground()
{
	GSLOG(Log, TEXT("\n excute getForegroundPush"));
	Push::getForegroundPush([=](ResultAPI const& In_Result, PushSetting const& In_PushSetting)
		{	
			GSLOG(Log, TEXT("getForegroundPush\n result = %s\n Foreground Push Setting = %s\n"), *(convertResultAPIToFString(In_Result)), *FString(In_PushSetting.toString().c_str()));

			_LocalPush = In_PushSetting.useForegroundLocalPush;
			_RemotePush = In_PushSetting.useForegroundRemotePush;
		});
}


bool UGsHiveManager::IsPushNight()
{
	FString msg = FString::Format(TEXT("UGsHiveManager::IsPushNight() {0}"), { _Night });
	//FGsUIHelper::PopupSystemMsg(FText::FromString(msg));

	return _Night;
}

bool UGsHiveManager::IsPushNotice()
{
	FString msg = FString::Format(TEXT("UGsHiveManager::IsPushNotice() {0}"), { _Notice });
	//FGsUIHelper::PopupSystemMsg(FText::FromString(msg));

	return _Notice;
}


// 대배너 출력
void UGsHiveManager::PromotionBANNER()
{
	GSLOG(Log, TEXT("\n excute PromotionBANNER"));

	// 프로모션 뷰를 대배너 타입으로 설정하기
	PromotionType promotionType = PromotionType::BANNER;

	// true라면 ‘오늘 하루 다시보지 않기’ 버튼을 표시하지 않음. 혹 오늘 이미 유저가 다시 보지 않도록 설정했더라도 무시하고 대배너를 띄움
	bool isForced = false;

	// 대배너 띄우기
	// 윈도우에서는 HIVESDKCEFImeEnable와 HIVESDKCEFImeDisable을 호출해줘야 한글이 입력된다.
	// 후에 hive 쪽에 한글입력이 필요한 부분이 있을 시 
	// 꼭 같이 넣어주자.
#if PLATFORM_WINDOWS
	HIVESDKCEFImeEnable();
#endif
	Promotion::showPromotion(promotionType, isForced, [=](ResultAPI In_Result, PromotionEventType promotionEventType)
		{
#if PLATFORM_WINDOWS
			HIVESDKCEFImeDisable();
#endif
			GSLOG(Log, TEXT("showPromotion\n result = %s\n PromotionEventType = BANNER\n"), *(convertResultAPIToFString(In_Result)));

			// 대배너 띄우기 결과 콜백 함수
			if (In_Result.isSuccess())
			{
				// API 호출 성공
				switch (promotionEventType)
				{
					// 프로모션 뷰가 뜸
				case PromotionEventType::OPEN:
					break;

					//프로모션 뷰가 닫힘
				case PromotionEventType::CLOSE:
					break;

					// 게임 종료 팝업(더 많은 게임 보기)에서 종료하기가 선택됨
				case PromotionEventType::EXIT:
					break;
				}

			}
			else if (In_Result.needExit())
			{
				AsyncTask(ENamedThreads::GameThread, [this]() {
					ExitGame();
					});
			}
			else
			{
			}
		});
}

void UGsHiveManager::PromotionNEWS()
{
	GSLOG(Log, TEXT("\n excute PromotionNEWS"));

	// 프로모션 뷰를 새소식 페이지로 설정하기
	PromotionType promotionType = PromotionType::NEWS;

	// true라면 ‘오늘 하루 다시보지 않기’ 버튼을 표시하지 않음. 혹 오늘 이미 유저가 다시 보지 않도록 설정했더라도 무시하고 새소식 페이지를 띄움
	bool isForced = false;

	// 새소식 페이지 띄우기
	// 윈도우에서는 HIVESDKCEFImeEnable와 HIVESDKCEFImeDisable을 호출해줘야 한글이 입력된다.
	// 후에 hive 쪽에 한글입력이 필요한 부분이 있을 시 
	// 꼭 같이 넣어주자.
#if PLATFORM_WINDOWS
	HIVESDKCEFImeEnable();
#endif
	Promotion::showPromotion(promotionType, isForced, [=](ResultAPI In_Result, PromotionEventType promotionEventType)
		{
#if PLATFORM_WINDOWS
			HIVESDKCEFImeDisable();
#endif
			GSLOG(Log, TEXT("showPromotion\n result = %s\n PromotionEventType = NEWS\n"), *(convertResultAPIToFString(In_Result)));

			// 새소식 페이지 띄우기 결과 콜백 함수
			if (In_Result.isSuccess())
			{
				// API 호출 성공
				switch (promotionEventType)
				{
					// 프로모션 뷰가 뜸
				case PromotionEventType::OPEN:
					break;

					//프로모션 뷰가 닫힘
				case PromotionEventType::CLOSE:
					break;

					// 게임 종료 팝업(더 많은 게임 보기)에서 종료하기가 선택됨
				case PromotionEventType::EXIT:
					break;
				}

			}
			else if (In_Result.needExit())
			{
				AsyncTask(ENamedThreads::GameThread, [this]() {
					ExitGame();
					});
			}
			else
			{
			}
		});
}

void UGsHiveManager::PromotionNOTICE()
{
	GSLOG(Log, TEXT("\n excute PromotionNOTICE"));

	// 프로모션 뷰를 공지사항 목록으로 설정하기
	PromotionType promotionType = PromotionType::NOTICE;

	// true라면 ‘오늘 하루 다시보지 않기’ 버튼을 표시하지 않음. 혹 오늘 이미 유저가 다시 보지 않도록 설정했더라도 무시하고 공지사항 페이지를 띄움
	bool isForced = false;

	// 공지사항을 활성화한 새소식 페이지 띄우기
	// 윈도우에서는 HIVESDKCEFImeEnable와 HIVESDKCEFImeDisable을 호출해줘야 한글이 입력된다.
	// 후에 hive 쪽에 한글입력이 필요한 부분이 있을 시 
	// 꼭 같이 넣어주자.
#if PLATFORM_WINDOWS
	HIVESDKCEFImeEnable();
#endif
	Promotion::showPromotion(promotionType, isForced, [=](ResultAPI In_Result, PromotionEventType promotionEventType)
		{
#if PLATFORM_WINDOWS
			HIVESDKCEFImeDisable();
#endif
			GSLOG(Log, TEXT("showPromotion\n result = %s\n PromotionEventType = NOTICE\n"), *(convertResultAPIToFString(In_Result)));

			// 공지사항 페이지 띄우기 결과 콜백 함수
			if (In_Result.isSuccess())
			{
				// API 호출 성공
			}

			if (In_Result.needExit())
			{
				ExitGame();
				return;
			}

			switch (promotionEventType)
			{
				// 프로모션 뷰가 뜸
			case PromotionEventType::OPEN:
				break;

				//프로모션 뷰가 닫힘
			case PromotionEventType::CLOSE:
				break;

				// 게임 종료 팝업(더 많은 게임 보기)에서 종료하기가 선택됨
			case PromotionEventType::EXIT:
				break;
			}
		});
}

void UGsHiveManager::ShowNews()
{
	GSLOG(Log, TEXT("\n excute ShowNews"));

	// 특정 메뉴 활성화를 위한 파라미터 설정
	std::string menu = "custom promotion type";
	// 특정 메뉴 활성화한 새소식 페이지 띄우기	
}

/*
새소식 페이지 정보는 로그인 직후 서버와 1회 통신하여 전달받습니다. 게임에서 설정한 게임 언어, 
게임 서버 등의 정보를 유저가 변경하는 경우 갱신이 필요합니다. 유저가 게임을 장시간 켜두고 사용하는 
경우에도 최신 정보 갱신이 이루어지지 않을 수 있습니다. 유저가 게임 로비에 접속할 때 게임에서는 새소식 정보 
갱신을 수동으로 진행해야 합니다. Promotion 클래스의 updatePromotionData() 메서드를 호출하여 정보를 갱신하세요.
*/
void UGsHiveManager::UpdatePromotionData()
{
	GSLOG(Log, TEXT("\n excute UpdatePromotionData"));

	Promotion::updatePromotionData();
}

void UGsHiveManager::ShowNativeReview()
{
	GSLOG(Log, TEXT("\n excute ShowNativeReview"));

	Promotion::showNativeReview([=](ResultAPI In_Result, PromotionEventType promotionEventType)
		{
			GSLOG(Log, TEXT("showNativeReview\n result = %s\n Review\n"), *(convertResultAPIToFString(In_Result)));

			// 리뷰 페이지 띄우기 결과 콜백 핸들러
			if (In_Result.isSuccess())
			{
				// API 호출 성공
			}

			if (In_Result.needExit())
			{
				ExitGame();
				return;
			}

			switch (promotionEventType)
			{
				// 프로모션 뷰가 뜸
			case PromotionEventType::OPEN:
				break;

				//프로모션 뷰가 닫힘
			case PromotionEventType::CLOSE:
				break;

				// 게임 종료 팝업(더 많은 게임 보기)에서 종료하기가 선택됨
			case PromotionEventType::EXIT:
				break;
			}

		});
}

void UGsHiveManager::ShowExit()
{
	GSLOG(Log, TEXT("\n excute ShowExit"));

	Promotion::showExit([=](ResultAPI In_Result, PromotionEventType promotionEventType)
		{
			GSLOG(Log, TEXT("showExit\n result = %s\n ShowExit\n"), *(convertResultAPIToFString(In_Result)));

			// 종료팝업 띄우기 결과 콜백 핸들러
			if (In_Result.isSuccess())
			{
				switch (promotionEventType)
				{
					// 프로모션 뷰가 뜸
				case PromotionEventType::OPEN:
					break;

					//프로모션 뷰가 닫힘
				case PromotionEventType::CLOSE:
					break;

					// 게임 종료 팝업 종료버튼 클릭 시
				case PromotionEventType::EXIT:
					AsyncTask(ENamedThreads::GameThread, [this]() {
							ExitGame();
						});
					break;
				}
			}
		});
}

void UGsHiveManager::GetBadgeInfo()
{
	GSLOG(Log, TEXT("\n excute GetBadgeInfo"));

	Promotion::getBadgeInfo([=](ResultAPI In_Result, std::vector<PromotionBadgeInfo> const& badgeInfoList)
		{
			GSLOG(Log, TEXT("getBadgeInfo\n result = %s\n ShowExit\n"), *(convertResultAPIToFString(In_Result)));

			// 배지 정보를 조회 결과 콜백 핸들러
			if (In_Result.isSuccess())
			{
				// API 호출 성공
			}

			if (In_Result.needExit())
			{
				ExitGame();
				return;
			}
		});
}

bool UGsHiveManager::GetOfferwallState()
{
	GSLOG(Log, TEXT("\n excute GetOfferwallState"));

	// 오퍼월 노출 가능 여부 확인
	OfferwallState offerwallState = Promotion::getOfferwallState();

	if (offerwallState == OfferwallState::ENABLED) 
	{
		// 오퍼월 버튼 노출 가능
		return true;
	}
	else 
	{
		// 오퍼월 버튼 노출 불가능
		return false;
	}
}


void UGsHiveManager::ShowOfferwall()
{
	GSLOG(Log, TEXT("\n excute ShowOfferwall"));

	// 오퍼월 웹뷰 호출하기
	Promotion::showOfferwall([=](ResultAPI In_Result, PromotionEventType promotionEventType)
		{
			GSLOG(Log, TEXT("showOfferwall\n result = %s\n Offerwall\n"), *(convertResultAPIToFString(In_Result)));

			// 오퍼월 웹뷰 호출하기 결과 콜백 함수
			if (In_Result.isSuccess())
			{
				// API 호출 성공
			}

			if (In_Result.needExit())
			{
				ExitGame();
				return;
			}

			switch (promotionEventType)
			{
				// 프로모션 뷰가 뜸
			case PromotionEventType::OPEN:
				break;

				//프로모션 뷰가 닫힘
			case PromotionEventType::CLOSE:
				break;

				// 게임 종료 팝업(더 많은 게임 보기)에서 종료하기가 선택됨
			case PromotionEventType::EXIT:
				break;
			}
		});
}

void UGsHiveManager::GetAppInvitationData()
{
	GSLOG(Log, TEXT("\n excute GetAppInvitationData"));

	// 앱 설치 유도 데이터 조회하기
	Promotion::getAppInvitationData([=](ResultAPI In_Result, AppInvitationData appInvitationData)
		{
			GSLOG(Log, TEXT("getAppInvitationData\n result = %s\n tAppInvitationData\n"), *(convertResultAPIToFString(In_Result)));

			// 앱 설치 유도 데이터 조회 결과 콜백 함수
			if (In_Result.isSuccess())
			{
			// API 호출 성공
			}

			if (In_Result.needExit())
			{
				ExitGame();
				return;
			}
		});
}

/*
	*@brief 요청한 Provider 의 상태를 체크 한다.
	* 요청한 Provider 로 Login 이 되어있지 않으면 Login 까지 시도한다.<br / >
	*Login 에 성공하면 providerUserId 까지 얻어 온다.<br / >
	*<br / >
	*현재 playerId 와 connect 를 요청하지는 않는다.<br / >
	*<br / >
	*SUCCESS 는 조회에 성공했다는 뜻이며 providerInfo 에 providerUserId 여부로 로그인된 유저를 판단해야 한다.<br / >
	*실제 provider 에 로그인 되어있다면 providerUserId 가 존재한다.그 외 providerType 만 존재.<br / >
	*<br / >
	*Google Play Games, Apple Game Center 등 묵시적 사인 - 인 방식을 사용할 경우
	*
	*@param providerType 상태를 체크할 ProviderType
*/
// 현재 프로세스 담당자가 어떻게 사용할지 몰라 string으로 출력한다(string, enum type으로 변경이 가능하다)
//enum class ProviderType {
//	GUEST = 0
//	, HIVE
//	, FACEBOOK
//	, GOOGLE
//	, QQ
//	, WEIBO
//	, VK
//	, WECHAT
//	, APPLE
//	, SIGNIN_APPLE
//	, LINE
//	, TWITTER
//	, WEVERSE
//
//	, AUTO = 99
//};
void UGsHiveManager::CheckProvider(FString providerType)
{
	std::string targetProviderType(TCHAR_TO_UTF8(*providerType));
	AuthV4::checkProvider(ProviderInfo::providerTypeFromString(targetProviderType), [=](ResultAPI const& In_Result, ProviderInfo const& providerInfo)
		{
			GSLOG(Log, TEXT("\nAuthV4::checkProvider result : %s"), *(convertResultAPIToFString(In_Result)));
			if (In_Result.isSuccess())
			{
				FString provType = FString(ProviderInfo::stringProviderType(providerInfo.providerType).c_str());
				FString provId = FString(providerInfo.providerUserId.c_str());
				GSLOG(Log, TEXT("\nType : %s\nID :%s\n"), *provType, *provId);
			}

			if (In_Result.needExit())
			{
				ExitGame();
				return;
			}
		});
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static std::string PromotionBadgeTargetToString(hive::PromotionBadgeTarget promotionBadgeTarget)
{
	if (promotionBadgeTarget == hive::PromotionBadgeTarget::NEWS)
		return "NEWS";
	else if (promotionBadgeTarget == hive::PromotionBadgeTarget::NOTICE)
		return "NOTICE";
	else if (promotionBadgeTarget == hive::PromotionBadgeTarget::CUSTOMVIEW)
		return "CUSTOMVIEW";
	else
		return "CUSTOMBOARD";
}

FString UGsHiveManager::convertResultAPIToFString(const hive::ResultAPI& reulstAPI)
{
	TSharedPtr<FJsonObject> resultAPIJson = MakeShareable(new FJsonObject);
	resultAPIJson->SetNumberField(TEXT("errorCode"), reulstAPI.errorCode);
	resultAPIJson->SetStringField(TEXT("errorMessage"), *FString(reulstAPI.errorMessage.c_str()));
	resultAPIJson->SetStringField(TEXT("message"), *FString(reulstAPI.message.c_str()));
	resultAPIJson->SetNumberField(TEXT("latencyMs"), reulstAPI.latencyMs);
	resultAPIJson->SetNumberField(TEXT("code"), reulstAPI.code);

	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TCondensedJsonPrintPolicy< TCHAR >> > Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy< TCHAR >>::Create(&OutputString);
	FJsonSerializer::Serialize(resultAPIJson.ToSharedRef(), Writer);
	return OutputString;
}

FString UGsHiveManager::convertPromotionViewInfoToFString(const hive::PromotionViewInfo& info)
{
	TSharedPtr<FJsonObject> resultAPIJson = MakeShareable(new FJsonObject);
	resultAPIJson->SetStringField(TEXT("url"), *FString(info.url.c_str()));
	resultAPIJson->SetStringField(TEXT("postString"), *FString(info.postString.c_str()));

	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TCondensedJsonPrintPolicy< TCHAR >> > Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy< TCHAR >>::Create(&OutputString);
	FJsonSerializer::Serialize(resultAPIJson.ToSharedRef(), Writer);
	return OutputString;
}

FString UGsHiveManager::convertPromotionBadgeInfoToFString(const hive::PromotionBadgeInfo& info)
{
	TSharedPtr<FJsonObject> resultAPIJson = MakeShareable(new FJsonObject);
	resultAPIJson->SetStringField(TEXT("badgeType"), *FString(info.badgeType.c_str()));
	resultAPIJson->SetStringField(TEXT("contentsKey"), *FString(info.contentsKey.c_str()));
	resultAPIJson->SetStringField(TEXT("target"), (PromotionBadgeTargetToString(info.target).c_str()));

	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TCondensedJsonPrintPolicy< TCHAR >> > Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy< TCHAR >>::Create(&OutputString);
	FJsonSerializer::Serialize(resultAPIJson.ToSharedRef(), Writer);
	return OutputString;
}

FString UGsHiveManager::convertPromotionBannerInfoToFString(const hive::PromotionBannerInfo& info)
{
	TSharedPtr<FJsonObject> resultAPIJson = MakeShareable(new FJsonObject);
	resultAPIJson->SetStringField(TEXT("displayEndDate"), *FString(info.displayEndDate.c_str()));
	resultAPIJson->SetStringField(TEXT("displayStartDate"), *FString(info.displayStartDate.c_str()));
	resultAPIJson->SetStringField(TEXT("imageUrl"), *FString(info.imageUrl.c_str()));
	resultAPIJson->SetStringField(TEXT("interworkData"), *FString(info.interworkData.c_str()));
	resultAPIJson->SetStringField(TEXT("linkUrl"), *FString(info.linkUrl.c_str()));
	resultAPIJson->SetNumberField(TEXT("pid"), info.pid);
	resultAPIJson->SetStringField(TEXT("typeBanner"), *FString(info.typeBanner.c_str()));
	resultAPIJson->SetStringField(TEXT("typeLink"), *FString(info.typeLink.c_str()));
	resultAPIJson->SetNumberField(TEXT("utcEndDate"), info.utcEndDate);
	resultAPIJson->SetNumberField(TEXT("utcStartDate"), info.utcStartDate);

	FString OutputString;
	TSharedRef< TJsonWriter<TCHAR, TCondensedJsonPrintPolicy< TCHAR >> > Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy< TCHAR >>::Create(&OutputString);
	FJsonSerializer::Serialize(resultAPIJson.ToSharedRef(), Writer);
	return OutputString;

}

void UGsHiveManager::ShowChatbotInquiry()
{
	HIVESDKIMEEnable();
	picojson::object additionalInfoJson;
	additionalInfoJson["evt_code"] = picojson::value("1000");
	additionalInfoJson["p1"] = picojson::value("random");

	std::string strAdditionalInfo = picojson::value(additionalInfoJson).serialize();
#if PLATFORM_IOS || PLATFORM_ANDROID
	AuthV4::showChatbotInquiry(strAdditionalInfo, [=](ResultAPI const& result) 
		{
			HIVESDKIMEDisable();
			GSLOG(Log, TEXT("\nAuthV4::showChatbotInquiry result : %s"), *(convertResultAPIToFString(result)));
		});
#else
	AuthV4ShowChatbotInquiry(strAdditionalInfo.c_str(), [=](ResultAPI const& result) 
		{
			HIVESDKIMEDisable();
			GSLOG(Log, TEXT("\nAuthV4ShowChatbotInquiry result : %s"), *(convertResultAPIToFString(result)));
		});
#endif
}

void UGsHiveManager::HelperGetIDPList(OUT TArray<FString>& outIDPList)
{
	GSLOG(Log, TEXT("\nGet IDP List"));

#if PLATFORM_IOS || PLATFORM_ANDROID
	std::vector<ProviderType> idpList = AuthV4::Helper::getIDPList();
#else
	std::vector<ProviderType> idpList;
	auto IDPListString = AuthV4HelperGetIDPList();
	std::string strIDPList(IDPListString);
	picojson::value IDPListJson;
	std::string err = picojson::parse(IDPListJson, strIDPList);
	picojson::array IDPListJsonArr = IDPListJson.get<picojson::array>();
	for (picojson::array::iterator i = IDPListJsonArr.begin();
		i != IDPListJsonArr.end();
		++i) {
		auto value = (*i);
		int providerTypeRtn = value.get<int64_t>();
		idpList.push_back((hive::ProviderType)providerTypeRtn);
	}
#endif

	for (auto i = idpList.begin(); i != idpList.end(); i++)
	{
		ProviderType providerType = (ProviderType)*i;
		FString typeString = FString(ProviderInfo::stringProviderType(providerType).c_str());
		
		GSLOG(Log, TEXT("%s"), *typeString);
		outIDPList.Emplace(typeString);		
	}
	GSLOG(Log, TEXT("\n"));
}

void UGsHiveManager::ResetConfig()
{
	picojson::value jsonParam;
#if PLATFORM_WINDOWS || PLATFORM_MAC
	mStrZone = TEXT("TEST");
#else
	mStrZone = TEXT("SANDBOX");
#endif
#if !WITH_EDITOR
#if PLATFORM_IOS || PLATFORM_ANDROID
	std::string configJsonString = Configuration::getConfiguration();
#else
	std::string configJsonString(ConfigurationGetConfigurationJsonString());
#endif
	std::string err = picojson::parse(jsonParam, configJsonString);
	if (!err.empty())
	{
		//        return;
	}
	mStrAppId = jsonParam.get("appId").to_str().c_str();
	mStrZone = jsonParam.get("zone").to_str().c_str();
	mBuseLog = jsonParam.get("useLog").get<bool>();
#endif
	//setup Default value
#if !PLATFORM_WINDOWS
	mStrLanguage = TEXT("한국어,ko");
#else
	FString defaultLanguage = FGenericPlatformMisc::GetDefaultLanguage();
	UE_LOG(LogTemp, Warning, TEXT("######### FGenericPlatformMisc::GetDefaultLanguage. : %s"), *defaultLanguage);
	if (defaultLanguage.Len() > 0) {
		UE_LOG(LogTemp, Warning, TEXT("######### FGenericPlatformMisc::GetDefaultLanguage. : %s"), *defaultLanguage.Left(2));
		if (defaultLanguage.Contains("zh")) {
			;
			if (defaultLanguage.Contains("CN")) {
				defaultLanguage = TEXT("zh-Hans");
			}
			else {
				defaultLanguage = TEXT("zh-Hant");
			}
		}
		else {
			defaultLanguage = defaultLanguage.Left(2);
		}
	}
	TArray<FString> StrLanguageArr;
	FString LanuguageListArr[] = {
		TEXT("English,en"),
		TEXT("Deutsch,de"),
		TEXT("español,es"),
		TEXT("français,fr"),
		TEXT("Indonesia,id"),
		TEXT("italiano,it"),
		TEXT("日本語,ja"),
		TEXT("한국어,ko"),
		TEXT("português,pt"),
		TEXT("русский,ru"),
		TEXT("ไทย,th"),
		TEXT("Türkçe,tr"),
		TEXT("Tiếng Việt,vi"),
		TEXT("简体中文,zh-Hans"),
		TEXT("繁體中文,zh-Hant"),
		TEXT("아랍어,ar"),
		TEXT("Not set,null")
	};
	mStrLanguage = TEXT("English,en");
	StrLanguageArr.Append(LanuguageListArr, UE_ARRAY_COUNT(LanuguageListArr));
	for (auto It = StrLanguageArr.CreateConstIterator(); It; ++It) {
		if ((*It).Contains(defaultLanguage)) {
			mStrLanguage = *It;
		}
	}
#endif
	mStrServerID = TEXT("KR");
	mUseAgeGate = false;
	mStrHiveOrientation = TEXT("all");
}

FString UGsHiveManager::ViewConfig()
{
	FString strConfig;
#if !WITH_EDITOR
#if PLATFORM_IOS || PLATFORM_ANDROID
	strConfig = Configuration::getConfiguration().c_str();
#else
	std::string configJsonString(ConfigurationGetConfigurationJsonString());
	strConfig = configJsonString.c_str();
#endif
#endif
	return strConfig;
}

void UGsHiveManager::SaveConfig()
{
	//Set Configuration

	// 설정 대화 상자가 닫힐 때 설정한 내용을 설정 파일에 기록한다.

	UE_LOG(LogTemp, Warning, TEXT("######### UConfigurationDialog Before Saveing Json String. : %s"), *mStrServerID);

	FString SavedConfigFile = FPaths::Combine(*FPaths::ProjectDir(), *FString("HIVESDKV4ConfigSaved.txt"));
	TSharedPtr<FJsonObject> arg = MakeShareable(new FJsonObject);
	arg->SetStringField(TEXT("appId"), mStrAppId);
	arg->SetStringField(TEXT("zone"), mStrZone);
	arg->SetStringField(TEXT("gameLanguage"), mStrLanguage);
	arg->SetStringField(TEXT("serverId"), mStrServerID);
	arg->SetBoolField(TEXT("useLog"), mBuseLog);
	arg->SetBoolField(TEXT("useAgeGate"), mUseAgeGate);
	arg->SetStringField(TEXT("hiveTheme"), mStrHiveTheme);
	arg->SetStringField(TEXT("hiveOrientation"), mStrHiveOrientation);
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(arg.ToSharedRef(), Writer);

	UE_LOG(LogTemp, Warning, TEXT("######### UConfigurationDialog Before Saveing Json String. : %s"), *OutputString);
	FFileHelper::SaveStringToFile(OutputString, *SavedConfigFile);

	std::string saveAppId(TCHAR_TO_UTF8(*mStrAppId));
#if !WITH_EDITOR
#if PLATFORM_IOS || PLATFORM_ANDROID
	Configuration::setAppId(saveAppId);
#else
	ConfigurationSetAppId(saveAppId.c_str());
#endif
	ZoneType saveZone;
	if (mStrZone.Equals(TEXT("SANDBOX"), ESearchCase::IgnoreCase))
	{
		saveZone = ZoneType::SANDBOX;
	}
	else if (mStrZone.Equals(TEXT("TEST"), ESearchCase::IgnoreCase))
	{
		saveZone = ZoneType::TEST;
	}
	else if (mStrZone.Equals(TEXT("DEV"), ESearchCase::IgnoreCase))
	{
		saveZone = ZoneType::DEV;
	}
	else
	{
		//default Real.
		saveZone = ZoneType::REAL;
	}
	UE_LOG(LogTemp, Warning, TEXT("######### UConfigurationDialog Before Saveing ZoneType:: %s"), *mStrZone);
	UE_LOG(LogTemp, Warning, TEXT("######### Enum : %d"), (int)saveZone);
	Configuration::setZone(saveZone);
	FString langCode;
	mStrLanguage.Split(TEXT(","), nullptr, &langCode);
	std::string saveLanguage(TCHAR_TO_UTF8(*langCode));
	if (saveLanguage.compare("null") != 0)
	{
#if PLATFORM_IOS || PLATFORM_ANDROID
		Configuration::setGameLanguage(saveLanguage);
#else
		ConfigurationSetGameLanguage(saveLanguage.c_str());
#endif

	}
	Configuration::setUseLog(mBuseLog);
	std::string saveServerID(TCHAR_TO_UTF8(*mStrServerID));

#if PLATFORM_IOS || PLATFORM_ANDROID
	Configuration::setServerId(saveServerID);
#else
	ConfigurationSetServerId(saveServerID.c_str());
#endif

	Configuration::setAgeGateU13(mUseAgeGate);
	SetHiveTheme(mStrHiveTheme);
	SetHiveOrientation(mStrHiveOrientation);
	//	PASS GAMEMODE CONFIG
	modeStrLanguage = langCode;
	modeStrServerID = mStrServerID;

	UE_LOG(LogTemp, Warning, TEXT("this lang code : %s"), *langCode);
#endif
}

void UGsHiveManager::SetHiveTheme(FString hiveThemeType)
{
	HiveThemeType type = HiveThemeType::hiveLight;
	if (hiveThemeType.Equals(TEXT("hiveDark")))
	{
		type = HiveThemeType::hiveDark;
	}

	Configuration::setHiveTheme(type);
}

void UGsHiveManager::SetHiveOrientation(FString hiveOrientation)
{
	std::string strHiveOrientation(TCHAR_TO_UTF8(*hiveOrientation));
	Configuration::setHiveOrientation(strHiveOrientation);
}

void UGsHiveManager::ConfigurationUpdateGameLanguage(FString language)
{
	std::string strLanguage(TCHAR_TO_UTF8(*language));
	Configuration::updateGameLanguage(strLanguage);

	UE_LOG(LogTemp, Warning, TEXT("Configuration::UpdateGameLanguage(%s) \n"), *language);

	FString savedLanguage = GetSavedLanguage(language);

	FString SavedConfigFile = FPaths::Combine(*FPaths::ProjectDir(), *FString("HIVESDKV4ConfigSaved.txt"));
	TSharedPtr<FJsonObject> arg = MakeShareable(new FJsonObject);
	arg->SetStringField(TEXT("gameLanguage"), savedLanguage);
	arg->SetStringField(TEXT("serverId"), modeStrServerID);
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(arg.ToSharedRef(), Writer);
	FFileHelper::SaveStringToFile(OutputString, *SavedConfigFile);

	modeStrLanguage = language;
}

FString UGsHiveManager::GetSavedLanguage(FString langCode)
{
	FString LanuguageListArr[] = {
		TEXT("English,en"),
		TEXT("Deutsch,de"),
		TEXT("español,es"),
		TEXT("français,fr"),
		TEXT("Indonesia,id"),
		TEXT("italiano,it"),
		TEXT("日本語,ja"),
		TEXT("한국어,ko"),
		TEXT("português,pt"),
		TEXT("русский,ru"),
		TEXT("ไทย,th"),
		TEXT("Türkçe,tr"),
		TEXT("Tiếng Việt,vi"),
		TEXT("简体中文,zh-Hans"),
		TEXT("繁體中文,zh-Hant"),
		TEXT("아랍어,ar"),
		TEXT("Not set,null")
	};

	FString savedLanguage = TEXT("한국어,ko");
	for (FString lang : LanuguageListArr)
	{
		if (lang.Contains(*langCode, ESearchCase::CaseSensitive, ESearchDir::FromEnd)) {
			savedLanguage = lang;
			break;
		}
	}
	return savedLanguage;
}

FString UGsHiveManager::GetConfigurationGetAppId()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UE_LOG(LogTemp, Warning, TEXT("appid : %s\n"), *FString(hive::Configuration::getAppId().c_str()));
	return hive::Configuration::getAppId().c_str();
#else	
	auto localAppId = ConfigurationGetAppId();
	UE_LOG(LogTemp, Warning, TEXT("appid : %s\n"), *FString(localAppId));
	return FString(localAppId);
#endif
}

hive::Account UGsHiveManager::GetCurrentAccountFromHive()
{
	hive::Account _account = hive::Auth::getAccount();	//	get From AuthV1 First.
	if (_account.accessToken.empty())	//	if find fail, try to get AuthV4 PlayerInfo
	{
#if PLATFORM_IOS || PLATFORM_ANDROID
		auto _playerInfo = hive::AuthV4::getPlayerInfo();
#else
		auto infoString = AuthV4GetPlayerInfo();
		std::string strInfo(infoString);

		//Char to JSon
		PlayerInfo _playerInfo;
		picojson::value playerInfoJson;
		std::string err = picojson::parse(playerInfoJson, strInfo);

		_playerInfo.playerId = playerInfoJson.get("playerId").get<PlayerID>();
		_playerInfo.did = playerInfoJson.get("did").get<std::string>();
		_playerInfo.playerImageUrl = playerInfoJson.get("playerImageUrl").get<std::string>();
		_playerInfo.playerName = playerInfoJson.get("playerName").get<std::string>();
		_playerInfo.playerToken = playerInfoJson.get("playerToken").get<std::string>();
		picojson::object providerInfoData = playerInfoJson.get("providerInfoData").get<picojson::object>();
		std::map<hive::ProviderType, hive::ProviderInfo> mapProviderInfoData;
		for (picojson::value::object::const_iterator i = providerInfoData.begin(); i != providerInfoData.end(); ++i)
		{
			auto value = i->second;
			hive::ProviderInfo __proInfo;
			__proInfo.providerType = (hive::ProviderType)value.get("providerType").get<int64_t>();
			__proInfo.providerUserId = value.get("providerUserId").get<std::string>();
			__proInfo.providerEmail = value.get("providerEmail").get<std::string>();
			mapProviderInfoData[(hive::ProviderType)(stoi(i->first))] = __proInfo;
		}
		_playerInfo.providerInfoData = mapProviderInfoData;
#endif
		_account.did = _playerInfo.did;
		_account.vid = std::to_string(_playerInfo.playerId);	//	long long -> std::string
		_account.accessToken = _playerInfo.playerToken;
	}

	return _account;
}

void UGsHiveManager::ConfigurationUpdateServerID(FString serverId)
{
	std::string strServerId(TCHAR_TO_UTF8(*serverId));
	Configuration::updateServerId(strServerId);

	UE_LOG(LogTemp, Warning, TEXT("Configuration::UpdateServerID(%s) \n"), *serverId);

	FString SavedConfigFile = FPaths::Combine(*FPaths::ProjectDir(), *FString("HIVESDKV4ConfigSaved.txt"));
	TSharedPtr<FJsonObject> arg = MakeShareable(new FJsonObject);
	arg->SetStringField(TEXT("serverId"), serverId);

	FString langCode = modeStrLanguage;
	FString savedLanguage = GetSavedLanguage(langCode);
	arg->SetStringField(TEXT("gameLanguage"), savedLanguage);

	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(arg.ToSharedRef(), Writer);
	FFileHelper::SaveStringToFile(OutputString, *SavedConfigFile);

	modeStrServerID = serverId;
}

void UGsHiveManager::SetServerIdByUserSelectServer(const FString& inServerid)
{
#if !WITH_EDITOR
	auto curId = Configuration::getServerId();
	if (curId.empty())
	{
		GSLOG(Log, TEXT("hivelog SetServerIdBySelectServer (Configuration::setServerId) result : %s\n"), *inServerid);
		Configuration::setServerId(std::string(TCHAR_TO_UTF8(*inServerid)));
	}
	else
	{
		GSLOG(Log, TEXT("hivelog SetServerIdBySelectServer (Configuration::updateServerId) result : %s\n"), *inServerid);
		Configuration::updateServerId(std::string(TCHAR_TO_UTF8(*inServerid)));
	}
#endif
}

FString UGsHiveManager::IAPV4TypeStringFromType(IAPV4Type argIapType)
{
	FString strType;
	/*switch (argIapType) {
	case IAPV4Type::NotSelected:
		strType = TEXT("NotSelected");
		break;
	case IAPV4Type::AppleAppStore:
		strType = TEXT("AppleAppStore");
		break;
	case IAPV4Type::GooglePlayStore:
		strType = TEXT("GooglePlayStore");
		break;
	case IAPV4Type::HIVELebi:
		strType = TEXT("HIVELebi");
		break;
	case IAPV4Type::OneStore:
		strType = TEXT("OneStore");
		break;
	case IAPV4Type::AmazonAppStore:
		strType = TEXT("AmazonAppStore");
		break;
	case IAPV4Type::SamsungGalaxyStore:
		strType = TEXT("SamsungGalaxyStore");
		break;
	case IAPV4Type::HuaweiAppGallery:
		strType = TEXT("HuaweiAppGallery");
		break;
	case IAPV4Type::HiveStore:
		strType = TEXT("HiveStore");
		break;
	case IAPV4Type::GooglePlayStorePC:
		strType = TEXT("GooglePlayStorePC");
		break;
	default:
		strType = TEXT("NONE");
		break;
	}*/
	return strType;
}

// LSJ
// 이 함수 사용 안함
// FGsIAPPurchaseBase::TryHivePurchase() 함수에서 호출함
/*
void UGsHiveManager::IAPV4purchase(FString strMarketPid, FString strAdditionalInfo)
{
	std::string marketPid(TCHAR_TO_UTF8(*strMarketPid));
	std::string additionalInfo(TCHAR_TO_UTF8(*strAdditionalInfo));
#if PLATFORM_IOS || PLATFORM_ANDROID
	IAPV4::purchase(marketPid, additionalInfo, [=](ResultAPI const& result, IAPV4Receipt const& receipt) 
	{
#else
	IAPV4Purchase(marketPid.c_str(), additionalInfo.c_str(), [=](ResultAPI const& result, IAPV4Receipt const& receipt) 
	{
#endif
		UE_LOG(LogTemp, Warning, TEXT("IAPV4purchase result = %s\n"), *(convertResultAPIToFString(result)));
		if (result.isSuccess()) 
		{

			switch (receipt.type)
			{
			case IAPV4Type::AppleAppStore:
				break;
			case IAPV4Type::GooglePlayStore:
				break;
			case IAPV4Type::HIVELebi:
				break;
			case IAPV4Type::OneStore:
				break;
			default:
				break;
			}

			//UE_LOG(LogTemp, Warning, TEXT("type : %s"), *IAPV4TypeStringFromType(receipt.type));
			UE_LOG(LogTemp, Warning, TEXT("product info"));
			UE_LOG(LogTemp, Warning, TEXT("marketpid : %s"), FUTF8ToTCHAR(receipt.product.marketPid.c_str()).Get());
			UE_LOG(LogTemp, Warning, TEXT("currency : %s"), FUTF8ToTCHAR(receipt.product.currency.c_str()).Get());
			UE_LOG(LogTemp, Warning, TEXT("price : %f"), receipt.product.price);
			UE_LOG(LogTemp, Warning, TEXT("displayPrice : %s"), FUTF8ToTCHAR(receipt.product.displayPrice.c_str()).Get());
			UE_LOG(LogTemp, Warning, TEXT("title : %s"), FUTF8ToTCHAR(receipt.product.title.c_str()).Get());
			UE_LOG(LogTemp, Warning, TEXT("productDescription : %s"), FUTF8ToTCHAR(receipt.product.productDescription.c_str()).Get());
			UE_LOG(LogTemp, Warning, TEXT("hiveiapReceipt : %s ..."), FUTF8ToTCHAR((receipt.hiveiapReceipt.substr(0, 80)).c_str()).Get());
			UE_LOG(LogTemp, Warning, TEXT("bypassInfo : %s ..."), FUTF8ToTCHAR(receipt.bypassInfo.c_str()).Get());
			UE_LOG(LogTemp, Warning, TEXT("\n"));
		}

		if (result.needExit())
		{
			ExitGame();
			return;
		}
	});

}
*/

// isReady : true - 스택에 쌓인 hive 이벤트 콜백을 순차적으로 클라에게 전송 , false 클라 전송 중지 (스텍에 쌓아놓음)
// 인게임 상황에 따라 컨트롤 필요
void UGsHiveManager::SetEngagementReady(bool isReady)
{
	UE_LOG(LogTemp, Warning, TEXT("SetEngagementReady(%s)"), isReady ? TEXT("TRUE") : TEXT("FALSE"));

#if PLATFORM_IOS || PLATFORM_ANDROID
	ResultAPI EngagementReadyResult = Promotion::setEngagementReady(isReady);
#else
	auto readyResult = PromotionSetEngagementReady(isReady);
	//  JSON -> hive::ResultAPI
	picojson::value resultApiJson;
	std::string err = picojson::parse(resultApiJson, readyResult);

	ResultAPI EngagementReadyResult;
	EngagementReadyResult.code = (hive::ResultAPI::Code)resultApiJson.get("code").get<int64_t>();
	EngagementReadyResult.errorCode = (hive::ResultAPI::ErrorCode)resultApiJson.get("errorCode").get<int64_t>();
	EngagementReadyResult.message = resultApiJson.get("message").get<std::string>();
	EngagementReadyResult.errorMessage = resultApiJson.get("errorMessage").get<std::string>();
	EngagementReadyResult.latencyMs = resultApiJson.get("latencyMs").get<int64_t>();
#endif

	UE_LOG(LogTemp, Warning, TEXT("AuthV4 setEngagementReady result = %s"), *(convertResultAPIToFString(EngagementReadyResult)));
}

bool UGsHiveManager::PromotionProcessURI(FString uri) 
{
	std::string strUri(TCHAR_TO_UTF8(*uri));
#if PLATFORM_IOS || PLATFORM_ANDROID
	bool result = Promotion::processURI(strUri);
#else
	bool result = PromotionHIVEProcessURI(strUri.c_str());
#endif
	UE_LOG(LogTemp, Warning, TEXT("\n\n=== 인터워크 스키마 실행 ===\n result= %s \n"), *(LexToString(result)));

	return result;
}

FString UGsHiveManager::GetHiveCountry()
{
#if WITH_EDITOR
	return TEXT("Hive Country");
#else
#if PLATFORM_IOS || PLATFORM_ANDROID
	UE_LOG(LogTemp, Warning, TEXT("HiveCountry : %s\n"), *FString(hive::Configuration::getHiveCountry().c_str()));
	return hive::Configuration::getHiveCountry().c_str();
#else
	auto HIVECountry = ConfigurationGetHIVECountry();
	UE_LOG(LogTemp, Warning, TEXT("HIVECountry : %s\n"), *FString(HIVECountry));
	return FString(HIVECountry);
#endif
#endif
}

// 윈도우는 채널 함수가 없음.
 FString UGsHiveManager::GetChannel()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UE_LOG(LogTemp, Warning, TEXT("Channel : %s\n"), *FString(hive::Configuration::getChannel().c_str()));
	return hive::Configuration::getChannel().c_str();
#else
	return TEXT("Hive Channel");
#endif
}

FString UGsHiveManager::GetMarket()
{
#if PLATFORM_IOS || PLATFORM_ANDROID
	UE_LOG(LogTemp, Warning, TEXT("Market : %s\n"), *FString(hive::Configuration::getMarket().c_str()));
	return hive::Configuration::getMarket().c_str();
#else
	auto HIVEMarket = ConfigurationGetMarket();
	UE_LOG(LogTemp, Warning, TEXT("HIVEMarket : %s\n"), *FString(HIVEMarket));
	return FString(HIVEMarket);
#endif
}

void UGsHiveManager::SetWindowsEngagementHandlerCB()
{
#if PLATFORM_WINDOWS
	GSLOG(Log, TEXT("[Bind SetWindowsEngagementHandlerCB]"));
	Promotion::setEngagementHandler([&, this](ResultAPI const& result, EngagementEventType engagementEventType, EngagementEventState engagementEventState, std::string const& param)
		{
			GSLOG(Log, TEXT("[Call setEngagementHandler]"));
			if (result.isSuccess())
			{
				// API 호출 성공
				GSLOG(Log, TEXT("[setEngagementHandler - isSuccess ]"));

				switch (engagementEventType)
				{
					// 프로모션 뷰가 뜸
				case EngagementEventType::EVENT_TYPE:
					GSLOG(Log, TEXT("EVENT_TYPE"));
					break;
				case EngagementEventType::PROMOTION_VIEW:
					GSLOG(Log, TEXT("PROMOTION_VIEW"));
					break;
				case EngagementEventType::OFFERWALL_VIEW:
					GSLOG(Log, TEXT("OFFERWALL_VIEW"));
					break;
				case EngagementEventType::USER_ACQUISITION:
					GSLOG(Log, TEXT("USER_ACQUISITION"));
					break;
				case EngagementEventType::COUPON:
					GSLOG(Log, TEXT("COUPON"));
					break;
				case EngagementEventType::AUTH_LOGIN_VIEW:
					GSLOG(Log, TEXT("AUTH_LOGIN_VIEW"));
					break;
				case EngagementEventType::SOCIAL_INQUIRY_VIEW:
					GSLOG(Log, TEXT("SOCIAL_INQUIRY_VIEW"));
					break;
				case EngagementEventType::EVENT: //HIVEEngagementEventTypeEvent Engagement에 의해 처리될 수 없는 이벤트(host가 game인 경우)를 전달해주는 콜백.
				{
					GSLOG(Log, TEXT("EVENT"));

					if (param.empty())
					{
						GSLOG(Error, TEXT("[SetWindowsEngagementHandlerCB] param is empty"));
					}
					else
					{
						FString jsonObjectStr = FString(UTF8_TO_TCHAR(param.c_str()));
						if (jsonObjectStr.IsEmpty())
						{
							GSLOG(Error, TEXT("[SetWindowsEngagementHandlerCB] jsonObjectStr is empty"));
							return;
						}
						// Log Code
						GSLOG(Log, TEXT("[SetWindowsEngagementHandlerCB] jsonObjectStr : %s"), *jsonObjectStr);
						// {"api":"/eventpopupshop","param":"cid=1&product_id=223061302&tid=_2306"}

						FUserEngagementInfoWindows engagementInfo;
						if (FJsonObjectConverter::JsonObjectStringToUStruct(jsonObjectStr, &engagementInfo, 0, 0))
						{
							FString engagementParam = engagementInfo.param;
							if (false == engagementParam.IsEmpty())
							{
								//GSLOG(Error, TEXT("[SetWindowsEngagementHandlerCB] engagementParam : %s"), *engagementParam);
								if (IsMatchedCallbackTypeByKey(engagementParam, "product_id"))
								{
									// BM 상점
									OnEngagementCallback(EGsEngagementCallbackType::BMSHOP, engagementParam);
								}
							}
							else
							{
								GSLOG(Error, TEXT("[SetWindowsEngagementHandlerCB] engagementInfo.param is empty"));
							}
						}
						else
						{
							GSLOG(Error, TEXT("[SetWindowsEngagementHandlerCB] false == FJsonObjectConverter::JsonObjectStringToUStruct"));
						}
					}
					break;
				}
				case EngagementEventType::IAP_UPDATED:
					GSLOG(Log, TEXT("IAP_UPDATED"));
					break;
				case EngagementEventType::IAP_PURCHASE:
					GSLOG(Log, TEXT("IAP_PURCHASE"));
					break;
				case EngagementEventType::IAP_PROMOTE:
					GSLOG(Log, TEXT("IAP_PROMOTE"));
					break;
				case EngagementEventType::COMPANION:
					GSLOG(Log, TEXT("COMPANION"));
					break;
				case EngagementEventType::SOCIAL_MYINQUIRY_VIEW:
					GSLOG(Log, TEXT("SOCIAL_MYINQUIRY_VIEW"));
					break;
				case EngagementEventType::SOCIAL_PROFILE_VIEW:
					GSLOG(Log, TEXT("SOCIAL_PROFILE_VIEW"));
					break;
				}
			}
			else if (result.needExit())
			{
				AsyncTask(ENamedThreads::GameThread, [this]() {
						ExitGame();
					});
				
				return;
			}
			else
			{
				GSLOG(Warning, TEXT("[setEngagementHandler - result is not Success ]"));
			}
		});
#endif
}

void UGsHiveManager::SetEngagementHandlerCB()
{
	GSLOG(Log, TEXT("[Bind SetEngagementHandlerCB]"));
	Promotion::setEngagementHandler([&, this](ResultAPI const& result, EngagementEventType engagementEventType, EngagementEventState engagementEventState, picojson::value const& param)
		{
			GSLOG(Log, TEXT("[Call setEngagementHandler]"));
			if (result.isSuccess())
			{
				// API 호출 성공
				GSLOG(Log, TEXT("[setEngagementHandler - isSuccess ]"));

				switch (engagementEventType)
				{
					// 프로모션 뷰가 뜸
				case EngagementEventType::EVENT_TYPE:
					GSLOG(Log, TEXT("EVENT_TYPE"));
					break;
				case EngagementEventType::PROMOTION_VIEW:
					GSLOG(Log, TEXT("PROMOTION_VIEW"));
					break;
				case EngagementEventType::OFFERWALL_VIEW:
					GSLOG(Log, TEXT("OFFERWALL_VIEW"));
					break;
				case EngagementEventType::USER_ACQUISITION:
					GSLOG(Log, TEXT("USER_ACQUISITION"));
					break;
				case EngagementEventType::COUPON:
					GSLOG(Log, TEXT("COUPON"));
					break;
				case EngagementEventType::AUTH_LOGIN_VIEW:
					GSLOG(Log, TEXT("AUTH_LOGIN_VIEW"));
					break;
				case EngagementEventType::SOCIAL_INQUIRY_VIEW:
					GSLOG(Log, TEXT("SOCIAL_INQUIRY_VIEW"));
					break;
				case EngagementEventType::EVENT: //HIVEEngagementEventTypeEvent Engagement에 의해 처리될 수 없는 이벤트(host가 game인 경우)를 전달해주는 콜백.
				{
					GSLOG(Log, TEXT("EVENT"));

					// 혹시나 object 타입으로 사용하지 않는 컨텐츠 에서는 별도 예외처리 해주세요
					// BM 팝업상점에서는 무조건 object 타입
					if (param.is<picojson::object>())
					{
						if (param.contains("param"))
						{
							picojson::value getParam = param.get("param");
							std::string productInfo = getParam.to_str();
							FString convert = FString(UTF8_TO_TCHAR(productInfo.c_str()));
							if (convert.Contains("product_id"))
							{
								OnEngagementCallback(EGsEngagementCallbackType::BMSHOP, convert);
							}
						}
						else
						{
							GSLOG(Warning, TEXT("[setEngagementHandler - param.contains(param) is empty ]"));
						}
					}
					else
					{
						GSLOG(Warning, TEXT("[setEngagementHandler - is not object]"));
					}
					break;
				}
				case EngagementEventType::IAP_UPDATED:
					GSLOG(Log, TEXT("IAP_UPDATED"));
					break;
				case EngagementEventType::IAP_PURCHASE:
					GSLOG(Log, TEXT("IAP_PURCHASE"));
					break;
				case EngagementEventType::IAP_PROMOTE:
					GSLOG(Log, TEXT("IAP_PROMOTE"));
					break;
				case EngagementEventType::COMPANION:
					GSLOG(Log, TEXT("COMPANION"));
					break;
				case EngagementEventType::SOCIAL_MYINQUIRY_VIEW:
					GSLOG(Log, TEXT("SOCIAL_MYINQUIRY_VIEW"));
					break;
				case EngagementEventType::SOCIAL_PROFILE_VIEW:
					GSLOG(Log, TEXT("SOCIAL_PROFILE_VIEW"));
					break;
				}
			}
			else
			{
				if (result.needExit())
				{
					ExitGame();
					return;
				}

				GSLOG(Warning, TEXT("[setEngagementHandler - result is not Success ]"));
			}
		});
}

void UGsHiveManager::OnEngagementCallback(const EGsEngagementCallbackType InType, FString InJsonValue)
{
	if (InJsonValue.IsEmpty())
	{
		GSLOG(Warning, TEXT("[OnEngagementCallback] - InJsonValue is Empty"));
		return;
	}

	TArray<FString> splitList;
	TArray<FString> childSplitList;
	switch (InType)
	{
	case EGsEngagementCallbackType::BMSHOP:
	{
		int32 resultID = 0;
		InJsonValue.ParseIntoArray(splitList, TEXT("&"));
		for (FString iter : splitList)
		{
			if (iter.Contains("product_id"))
			{
				iter.ParseIntoArray(childSplitList, TEXT("="));
				for (FString childIter : childSplitList)
				{
					//GSLOG(Warning, TEXT("childIter : %s"), *childIter);
					if (false == childIter.IsNumeric())
						continue;

					resultID = FCString::Atoi(*childIter);
					break;
				}
			}
		}
		if (FGsBMShopManager* bmShop = GBMShop())
		{
			bmShop->SetHivePromotionCallbackData(resultID);
		}

		break;
	}
	}
}

const bool UGsHiveManager::IsMatchedCallbackTypeByKey(FString InJsonValue, FString InFindKey)
{
	if (InJsonValue.IsEmpty() || InFindKey.IsEmpty())
	{
		return false;
	}

	TArray<FString> splitList;
	TArray<FString> splitChildList;
	InJsonValue.ParseIntoArray(splitList, TEXT("&"));
	for (FString& iter : splitList)
	{
		iter.ParseIntoArray(splitChildList, TEXT("="));
		for (FString& subiter : splitChildList)
		{
			if (subiter.Contains(InFindKey))
			{
				return true;
			}
		}
	}
	return false;
}

bool UGsHiveManager::IsDisplayFreeDiaCountry()
{
	FString hiveCountry = GetHiveCountry();
	if (hiveCountry.IsEmpty())
	{
		return false;
	}
	FString lower = hiveCountry.ToLower();
	if (lower.Contains("jp"))
	{
		return true;
	}
	return false;
}


// 계정 변경
void UGsHiveManager::OnAccountChange()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("HiveManager : OnAccountChange"));

	FVector2D textScale{ 1.f, 1.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, logString, true, textScale);
#endif

	SignOut();
}

void UGsHiveManager::Hercules_Initialize()
{
	if (_InitializedHercules) return;

	const FString appName = TEXT("worldofzenonia");

	GSLOG(Log, TEXT("Hercules Module Loaded"));
#if USE_HERCULES

#if PLATFORM_ANDROID
	if (CHerculesPlugin::Initialize(appName, HERCULES_NONE) == 0)
	{
		_InitializedHercules = true;
		GSLOG(Log, TEXT("Hercules Module Success Initialize"));
	}

#elif PLATFORM_WINDOWS
	const FString appId = TEXT("com.com2usholdings.zenonia.windows.microsoftstore.kr.normal");
	const FString appVersion = GDivergence()->GetClientVersion();// TEXT("20230602");
	if (CHerculesPlugin::Initialize(appName, HERCULES_NONE, appId, appVersion) == 0)
	{
		_InitializedHercules = true;
		GSLOG(Log, TEXT("Hercules Module Success Initialize"));
	}

#else
	if (CHerculesPlugin::Initialize(appName, HERCULES_NONE) == 0)
	{
		_InitializedHercules = true;
		GSLOG(Log, TEXT("Hercules Module Success Initialize"));
	}
#endif

#endif

}

void UGsHiveManager::Hercules_Uninitialize()
{
	if (_InitializedHercules == false)
		return;

#if USE_HERCULES
	CHerculesPlugin::Uninitialize();
#endif

	_InitializedHercules = false;
}

bool UGsHiveManager::IsSandBox()
{
	ZoneType type = Configuration::getZone();
	if (type == ZoneType::SANDBOX)
	{
		return true;
	}

	return false;
}

void UGsHiveManager::ShowDeviceManagement()
{
	GSLOG(Log, TEXT("ShowDeviceManagement call"));

	// 윈도우에서는 HIVESDKCEFImeEnable와 HIVESDKCEFImeDisable을 호출해줘야 한글이 입력된다.
	// 기기등록 후 기기등록 이름 변경 때 한글로도 입력하여 변경하기 위함이다.
	// 후에 hive 쪽에 한글입력이 필요한 부분이 있을 시 
	// 꼭 같이 넣어주자.
#if PLATFORM_WINDOWS
	HIVESDKCEFImeEnable();
#endif
	// Hive SDK AuthV4 기기관리창 요청
	AuthV4::showDeviceManagement([=](ResultAPI const& In_Result)
		{
#if PLATFORM_WINDOWS
			HIVESDKCEFImeDisable();
#endif
			GSLOG(Log, TEXT("ShowDeviceManagement result : %s\n"), *(convertResultAPIToFString(In_Result)));
			// 결과 콜백
			if (In_Result.isSuccess())
			{
				GSLOG(Log, TEXT("hivelog : showDeviceManagement Success -> MessageSystem::REGISTER_DEVIECE_SUCCEEDED call"));

				// 기기관리창 닫힘		
				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::REGISTER_DEVIECE_POPUP_SUCCEEDED, nullptr);
			}
			else if (In_Result.needExit())
			{			
				AsyncTask(ENamedThreads::GameThread, [this]() { 
					ExitGame(); 
				});
				return;
			}
			else
			{
				GSLOG(Log, TEXT("hivelog : showDeviceManagement Failed -> MessageSystem::REGISTER_DEVIECE_FAILED call"));

				// TODO : Error Handling
				GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::REGISTER_DEVIECE_POPUP_FAILED, nullptr);
			}
		});
}

void UGsHiveManager::ShowCommunity()
{
	GSLOG(Log, TEXT("ShowCommunity call"));

	// 윈도우에서는 HIVESDKCEFImeEnable와 HIVESDKCEFImeDisable을 호출해줘야 한글이 입력된다.
	// 후에 hive 쪽에 한글입력이 필요한 부분이 있을 시 
	// 꼭 같이 넣어주자.
#if PLATFORM_WINDOWS
	HIVESDKCEFImeEnable();
#endif
	SocialV4::ViewType viewType = SocialV4::ViewType::Frame;
	SocialV4::showCommunity(SocialV4::ProviderType::HIVE, viewType, [=](ResultAPI const& result)
		{
#if PLATFORM_WINDOWS
			HIVESDKCEFImeDisable();
#endif
			GSLOG(Log, TEXT("ShowDeviceManagement result : %s\n"), *(convertResultAPIToFString(result)));
			// 결과 콜백
			if (result.isSuccess())
			{
				GSLOG(Log, TEXT("hivelog : ShowCommunity Success"));
			}
			else
			{
				if (result.needExit())
				{
					ExitGame();
					return;
				}

				GSLOG(Log, TEXT("hivelog : ShowCommunity Failed"));
			}
		});
}

void UGsHiveManager::AnalyticsSendEvent(const FString& inEventName)
{
#if WITH_EDITOR
	GSLOG(Log, TEXT("hivelog : AnalyticsSendEvent PlatformType == Editor, EventName : %s"), *inEventName);
	return;
#endif
	if (inEventName.IsEmpty())
	{	
		GSLOG(Log, TEXT("hivelog : AnalyticsSendEvent inEventName Empty"));
		return;
	}

	if (false == IsSignIn())
	{
		GSLOG(Log, TEXT("hivelog : AnalyticsSendEvent SignIn false"));
		return;
	}

	std::string eventName(TCHAR_TO_UTF8(*inEventName));
	Analytics::sendEvent(eventName);

	GSLOG(Log, TEXT("hivelog : AnalyticsSendEvent [EventName : %s]"), *inEventName);
}

void UGsHiveManager::ExitGame()
{
	GSLOG(Log, TEXT("hivelog : ExitGame()"));

#if PLATFORM_IOS
	// 2021/08/25 PKT - IOS 종료..( Back Ground 숨김 )
	//UKismetSystemLibrary::QuitGame(world, nullptr, EQuitPreference::Background, false);
	// 2021/08/25 PKT - IOS 종료..
	int* forceCrashByExit = nullptr;
	(*forceCrashByExit) = 10;
#else
	if (UWorld* world = GEngine->GameViewport->GetWorld())
	{
		UKismetSystemLibrary::QuitGame(world, nullptr, EQuitPreference::Quit, false);
	}
#endif
}

// 문의하기
void UGsHiveManager::ShowInquiry()
{
	GSLOG(Log, TEXT("ShowInquiry call"));

	// 윈도우에서는 HIVESDKCEFImeEnable와 HIVESDKCEFImeDisable을 호출해줘야 한글이 입력된다.
	// 후에 hive 쪽에 한글입력이 필요한 부분이 있을 시 
	// 꼭 같이 넣어주자.
#if PLATFORM_WINDOWS
	HIVESDKCEFImeEnable();
#endif
	AuthV4::showInquiry([=](ResultAPI const& result)
		{		
#if PLATFORM_WINDOWS
			HIVESDKCEFImeDisable();
#endif
			GSLOG(Log, TEXT("\nAuthV4::ShowInquiry result : %s"), *(convertResultAPIToFString(result)));
		});
}

// 이용 약관
void UGsHiveManager::ShowTerms()
{
	GSLOG(Log, TEXT("ShowTerms call"));

	AuthV4::showTerms([=](ResultAPI const& result)
		{
			GSLOG(Log, TEXT("\nAuthV4::ShowTerms result : %s"), *(convertResultAPIToFString(result)));
		});
}

void UGsHiveManager::ShowProfile()
{
	GSLOG(Log, TEXT("ShowProfile call"));

	GSLOG(Log, TEXT("hivelog : CurrentPlayer Player ID :%lld\n"), CurrentPlayer.playerId);
	GSLOG(Log, TEXT("hivelog : CurrentPlayer PlayerName:%s\nDID :%s\ntoken :%s"),
		FUTF8ToTCHAR(CurrentPlayer.playerName.c_str()).Get(),
		*FString(CurrentPlayer.did.c_str()),
		*FString(CurrentPlayer.playerToken.c_str()));

	// 윈도우에서는 HIVESDKCEFImeEnable와 HIVESDKCEFImeDisable을 호출해줘야 한글이 입력된다.
	// 후에 hive 쪽에 한글입력이 필요한 부분이 있을 시 
	// 꼭 같이 넣어주자.
#if PLATFORM_WINDOWS
	HIVESDKCEFImeEnable();
#endif
	AuthV4::showProfile(CurrentPlayer.playerId, [=](ResultAPI const& result)
		{
#if PLATFORM_WINDOWS
			HIVESDKCEFImeDisable();
#endif
			GSLOG(Log, TEXT("\nAuthV4::ShowProfile result : %s"), *(convertResultAPIToFString(result)));

			AsyncTask(ENamedThreads::GameThread, [this]() {
				GetProfile();
			});
		});
}

void UGsHiveManager::GetProfile()
{
	GSLOG(Log, TEXT("GetProfile call"));

	vector<PlayerID> playerIdList;
	playerIdList.push_back(CurrentPlayer.playerId);

	GSLOG(Log, TEXT("CurrentPlayer.playerId : %d"), CurrentPlayer.playerId);

	AuthV4::getProfile(playerIdList, [=](ResultAPI const& result, std::vector<ProfileInfo> const& profileInfoList)
		{
			if (result.isSuccess())
			{
				if (profileInfoList.size() > 0) {
					std::string playerImageUrl;

					// 제일 마지막에 접속한 녀석의 계정 정보를 이용하여 프로필 이미지를 업데이트
					// (이재천님, 정윤일님과 논의)
					_playerImageUrl = Forward<FString>(FString(profileInfoList.back().playerImageUrl.c_str()));

					GSLOG(Log, TEXT("\nAuthV4::GetProfile playerImageUrl : %s"), *_playerImageUrl);

					GMessage()->GetSystemParam().SendMessageAsync(MessageSystem::HIVE_USER_PROFILE_UPDATE, nullptr);
				}
			}

			GSLOG(Log, TEXT("\nAuthV4::GetProfile result : %s"), *(convertResultAPIToFString(result)));
		});
}

const PlayerInfo& UGsHiveManager::GetConflictPlayer()
{
	return ConflictPlayer;
}
