#include "GsSystemParamMessageHandlerGlobal.h"

#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Runtime/Online/HTTP/Public/Interfaces/IHttpBase.h"
#include "Runtime/Online/HTTP/Public/Interfaces/IHttpRequest.h"
#include "Runtime/Online/HTTP/Public/Interfaces/IHttpResponse.h"
#include "Runtime/Online/HTTP/Public/PlatformHttp.h"

#include "../Plugins/HIVESDK/Source/HIVESDK/Public/HIVE_AuthV4.h"
#include "../Plugins/PlayGuard/Source/PlayGuard/Public/PlayGuardModule.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/MessageParam/GsSystemMessageParam.h"
#include "Hive/GsHiveEventMsg.h"

#include "T1Project.h"

FString FGsSystemParamMessageHandlerGlobal::_smokeSignalUrl;

FString FGsSystemParamMessageHandlerGlobal::_smokeSignalFirst;

FString FGsSystemParamMessageHandlerGlobal::_smokeSignalSecond;

FString FGsSystemParamMessageHandlerGlobal::_smokeSignalThird;

int32 FGsSystemParamMessageHandlerGlobal::_signalNum = 0;

FGsSystemParamMessageHandlerGlobal::~FGsSystemParamMessageHandlerGlobal()
{
}


void FGsSystemParamMessageHandlerGlobal::InitializeMessage()
{
	// 패킷 바인딩
	MSystemParam& systemParam = GMessage()->GetSystemParam();

	InitializeMessageDelegateList(&systemParam);

	AddMessageDelegate(systemParam.AddRaw(MessageSystem::HIVE_AUTH_TRUE,
		this, &FGsSystemParamMessageHandlerGlobal::OnHiveAuthTrue));

	AddMessageDelegate(systemParam.AddRaw(MessageSystem::SEQUENCE_PLAYER_END,
		this, &FGsSystemParamMessageHandlerGlobal::OnEndSequencePlayer));

	AddMessageDelegate(systemParam.AddRaw(MessageSystem::BEFORE_CHANGE_LEVEL,
		this, &FGsSystemParamMessageHandlerGlobal::OnBeforeChangeLevel));

	AddMessageDelegate(systemParam.AddRaw(MessageSystem::INIT_SMOKE_SIGNAL,
		this, &FGsSystemParamMessageHandlerGlobal::OnInitSmokeSignal));

	AddMessageDelegate(systemParam.AddRaw(MessageSystem::SEND_SMOKE_SIGNAL,
		this, &FGsSystemParamMessageHandlerGlobal::OnSendSmokeSignal));
}

void FGsSystemParamMessageHandlerGlobal::FinalizeMessage()
{
	FinalizeMessageDelegateList();
}

void FGsSystemParamMessageHandlerGlobal::OnHiveAuthTrue(const IGsMessageParam*)
{
	GSLOG(Log, TEXT("FGsSystemParamMessageHandlerGlobal::OnHiveAuthTrue()"));

	UGsHiveManager* HiveManager = GHive();
	if (nullptr == HiveManager)
	{
		GSLOG(Error, TEXT("nullptr == HiveManager"));
		return;
	}

	// 보안툴로 pid 전달
	const FString CurrentPid = TTypeToString<PlayerID>::ToString(HiveManager->GetPlayerID());

	FPlayGuardModule* PlayGuardModule = FPlayGuardModule::Get();
	if (nullptr != PlayGuardModule)
	{
		PlayGuardModule->SetUserId(CurrentPid);
	}

	CHerculesPlugin::SetUserId(CurrentPid);

	// 하이브 로그인 성공했을 때 Adjust 로 hive_login 메시지 보냄
	FGsHiveEventMsg specialMsg(EGsHiveActionType::SPECIAL, 1);    // 인자는 테이블에 정의된 id 값을 따릅니다
	GMessage()->GetHiveEvent().SendMessage(MessageContentHiveEvent::SEND_ANALYTICS_EVENT, &specialMsg);
}

void FGsSystemParamMessageHandlerGlobal::OnEndSequencePlayer(const IGsMessageParam*)
{
	// 소환 연출중이면 닫기
	FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
	if (summonHandler == nullptr)
	{
		return;
	}

	// 소환 연출중이면 지형 처리 안함(거기서도 쓰고 있어서)
	if (summonHandler->GetIsSummonPlay() == true)
	{
		return;
	}
	GLevel()->UnHoldStreamingLevel();

	// test: 현재 카메라 매니저 타겟이 local actor 가 아니면 바꾸자
#ifdef TEST_CHECK
	GLevel()->CheckViewTarget();
#endif
}

void FGsSystemParamMessageHandlerGlobal::OnBeforeChangeLevel(const IGsMessageParam* InParam)
{
	if (const FGsPrimitiveInt32* param = InParam->Cast<const FGsPrimitiveInt32>())
	{
		if (UGsUIManager* uiMgr = GUI())
		{
			uiMgr->OnChangeLevel();

			if (0 < param->_data)
			{
				bool bIsInvadeWorld = false;
				if (FGsGameFlow::Mode::GAME == GMode()->GetCurrentFlowType())
				{
					bIsInvadeWorld = GGameData()->IsInvadeWorld();
				}

				int32 playerLevel = 1;
				if (const FGsNetUserData* userData = GGameData()->GetUserData())
				{
					playerLevel = userData->mLevel;
				}

				uiMgr->ShowLoading(GLevel()->GetCurrentLevelId(), bIsInvadeWorld, playerLevel);
			}
		}
	}
}

void FGsSystemParamMessageHandlerGlobal::OnInitSmokeSignal(const IGsMessageParam* InParam)
{
#ifdef USE_SMOKE_SIGNAL
	if (FGsSystemParamMessageHandlerGlobal::_smokeSignalUrl.IsEmpty())
	{
		TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();

		HttpRequest->OnProcessRequestComplete().BindRaw(this, &FGsSystemParamMessageHandlerGlobal::OnSmokeUrlRequestComplete);
		HttpRequest->SetURL(TEXT("https://docs.google.com/forms/d/e/1FAIpQLScOdtR7hy-NZQrfBTb8bIpJYAonaCu53wW-Bt-0nzqbnhHaTg/viewform"));
		HttpRequest->SetVerb(TEXT("GET"));
		HttpRequest->ProcessRequest();
	}
#endif
}

void FGsSystemParamMessageHandlerGlobal::OnSendSmokeSignal(const IGsMessageParam* InParam)
{
#ifdef USE_SMOKE_SIGNAL
	if (FGsSystemParamMessageHandlerGlobal::_smokeSignalUrl.IsEmpty())
	{
		FGsInitSmokeSignal().Send();
		return;
	}

	const FGsSmokeSignal* Param = InParam->Cast<const FGsSmokeSignal>();
	if (nullptr == Param)
	{
		return;
	}

	FString MyVid(TEXT("PlayerUnknownBatlleGround"));
	FGsNetManager* netMgr = GNet();
	if (nullptr != netMgr)
	{
		MyVid = FString::Printf(TEXT("%llu"), netMgr->GetVId());
	}
	
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	//Request->SetURL(TEXT("https://docs.google.com/forms/d/e/1FAIpQLScOdtR7hy-NZQrfBTb8bIpJYAonaCu53wW-Bt-0nzqbnhHaTg/formResponse"));
	Request->SetURL(FGsSystemParamMessageHandlerGlobal::_smokeSignalUrl);
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	const FString ContentsStr = FString::Printf(TEXT("entry.%s=%s&entry.%s=%d&entry.%s=%s"), *FGsSystemParamMessageHandlerGlobal::_smokeSignalFirst, *FPlatformHttp::UrlEncode(MyVid), *FGsSystemParamMessageHandlerGlobal::_smokeSignalSecond, _signalNum++, *FGsSystemParamMessageHandlerGlobal::_smokeSignalThird, *FPlatformHttp::UrlEncode(Param->_data));
	Request->SetContentAsString(ContentsStr);
	Request->ProcessRequest();
#endif
}

void FGsSystemParamMessageHandlerGlobal::OnSmokeUrlRequestComplete(FHttpRequestPtr InHttpRequest, FHttpResponsePtr InHttpResponse, bool InbSucceeded)
{
#ifdef USE_SMOKE_SIGNAL
	if (!InbSucceeded || !InHttpResponse.IsValid())
	{
		return;
	}

	const FString Content = InHttpResponse->GetContentAsString();

	int32 StartIndex = 0;
	int32 EndIndex = 0;

	{
		const FString FormStr = TEXT("<form action=\"");
		StartIndex = Content.Find(FormStr) + FormStr.Len();
		if (0 >= StartIndex)
		{
			return;
		}

		EndIndex = Content.Find(TEXT("\""), ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex);
		if (0 > EndIndex || 200 < (EndIndex - StartIndex))
		{
			return;
		}

		FGsSystemParamMessageHandlerGlobal::_smokeSignalUrl = Content.Mid(StartIndex, EndIndex - StartIndex);
	}

	{
		const FString FirstStr = TEXT("&quot;,null,0,[[");
		StartIndex = Content.Find(FirstStr, ESearchCase::IgnoreCase, ESearchDir::FromStart, EndIndex) + FirstStr.Len();
		if (0 >= StartIndex)
		{
			return;
		}

		EndIndex = Content.Find(TEXT(","), ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex);
		if (0 > EndIndex || 200 < (EndIndex - StartIndex))
		{
			return;
		}

		FGsSystemParamMessageHandlerGlobal::_smokeSignalFirst = Content.Mid(StartIndex, EndIndex - StartIndex);
	}

	{
		const FString SecondStr = TEXT("&quot;,null,0,[[");
		StartIndex = Content.Find(SecondStr, ESearchCase::IgnoreCase, ESearchDir::FromStart, EndIndex) + SecondStr.Len();
		if (0 >= StartIndex)
		{
			return;
		}

		EndIndex = Content.Find(TEXT(","), ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex);
		if (0 > EndIndex || 200 < (EndIndex - StartIndex))
		{
			return;
		}

		FGsSystemParamMessageHandlerGlobal::_smokeSignalSecond = Content.Mid(StartIndex, EndIndex - StartIndex);
	}

	{
		const FString ThirdStr = TEXT("&quot;,null,0,[[");
		StartIndex = Content.Find(ThirdStr, ESearchCase::IgnoreCase, ESearchDir::FromStart, EndIndex) + ThirdStr.Len();
		if (0 >= StartIndex)
		{
			return;
		}

		EndIndex = Content.Find(TEXT(","), ESearchCase::IgnoreCase, ESearchDir::FromStart, StartIndex);
		if (0 > EndIndex || 200 < (EndIndex - StartIndex))
		{
			return;
		}

		FGsSystemParamMessageHandlerGlobal::_smokeSignalThird = Content.Mid(StartIndex, EndIndex - StartIndex);
	}
#endif
}
