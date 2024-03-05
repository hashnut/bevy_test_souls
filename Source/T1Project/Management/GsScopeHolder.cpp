#include "GsScopeHolder.h"
#include "GsMessageHolder.h"

#include "CoreMinimal.h"
#include "Engine.h"
#include "T1Project.h"

#include "UTIL/GsFPSAnalyzer.h"

#include "Message/MessageHandler/GsSystemMessageHandlerGlobal.h"
#include "Message/MessageHandler/GsSystemParamMessageHandlerGlobal.h"
#include "Message/MessageHandler/GsUIMessageHandlerGlobal.h"

#include "DataSchema/Camera/GsSchemaCameraModeData.h"

#include "Internationalization/CulturePointer.h"
#include "Internationalization/Internationalization.h"

#include "MOSUtil.h"
#include "Runtime/UMG/Public/Blueprint/WidgetBlueprintLibrary.h"
#include "Runtime/UMG/Public//Blueprint/SlateBlueprintLibrary.h"
#include "WidgetLayoutLibrary.h"

#include "Management/ScopeGlobal/GsHiveManager.h"

#include "Option/GsGameUserSettings.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

template<>
UGsScopeHolder* GSScopeHolderSingle::_instance = nullptr;

bool	UGsScopeHolder::_isGlobalCreated = false;
bool	UGsScopeHolder::_isGameCreated = false;
bool	UGsScopeHolder::_isLobbyCreated = false;

void UGsScopeHolder::Init()
{
	UE_LOG(LogTemp, Error, TEXT("UGsScopeHolder::Init()"));
	TGsSingleton::InitInstance(this);


	_fpsAnalyzer = new FGsFPSAnalyzer();

	_global = NewObject<UGsScopeGlobal>(this);
	check(_global);
	_lobby = NewObject<UGsScopeLobby>(this);
	check(_lobby);
	_game = NewObject<UGsScopeGame>(this);
	check(_game);

	// 0. 메시지 홀더 생성
	_message = new FGsMessageHolder();
	
	// 1. 전역매니저 생성 (메시지 매니저 포함)
	_global->Create();
	_isGlobalCreated = true;
	_isGlobalInitAfterResourceDownload = false;


	// 2. 메시지 ------------------------------------------------------------------------------------
	_messageHandlers.Add(new FGsSystemMessageHandlerGlobal());
	_messageHandlers.Add(new FGsSystemParamMessageHandlerGlobal());
	_messageHandlers.Add(new FGsUIMessageHandlerGlobal());

	for (IGsMessageHandler* handler : _messageHandlers)
	{
		handler->InitializeMessage();
	}
	//----------------------------------------------------------------------------------------------

	// 3. 전역 매니져들 메시지들이 여기서 등록됨
	_global->Init();

	FIOSKeyboardEvent event;
	event.BindUObject(this, &UGsScopeHolder::CallbackIOSKeyboardHeight);
	FMOSUtilModule::SetIOSKeyboardEvent(event);

	AHUD::OnShowDebugInfo.AddUObject(this, &UGsScopeHolder::OnDrawDebug);

	_testcontentScaleFactor = FMOSUtilModule::Get().GetContentScaleFactorIOS();

	GMessage()->GetSystem().SendMessage(MessageSystem::SYSTEM_INIT);
}



void UGsScopeHolder::CallbackIOSKeyboardHeight(float InHeight)
{
	FVector2D viewport = UWidgetLayoutLibrary::GetViewportSize(this);
	float scale = UWidgetLayoutLibrary::GetViewportScale(this);

	
	SCREEN_MESSAGE(1002, 200.0f, FColor::Green, "IOSKeyboardHeight : %.2f : ViewPort[%.2f, %.2f], ViewScale[%.2f] ContentScale", InHeight, viewport.X, viewport.Y, scale, _testcontentScaleFactor);
	_testKeyBoardHeight = InHeight * _testcontentScaleFactor;
}

void UGsScopeHolder::OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location)
{
	_testTouchPosition = Location;
	SCREEN_MESSAGE(1003, 200.0f, FColor::Cyan, "Press : [%.2f, %.2f]", _testTouchPosition.X, _testTouchPosition.Y);
}

void UGsScopeHolder::OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location)
{
	_testTouchPosition = Location;
	SCREEN_MESSAGE(1003, 200.0f, FColor::Cyan, "Release : [%.2f, %.2f]", _testTouchPosition.X, _testTouchPosition.Y);
}

void UGsScopeHolder::OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location)
{
	_testTouchPosition = Location;
	SCREEN_MESSAGE(1003, 200.0f, FColor::Cyan, "Move : [%.2f, %.2f]", _testTouchPosition.X, _testTouchPosition.Y);
}

void UGsScopeHolder::OnResourceDownloadComplete()
{
	if (false == _isGlobalInitAfterResourceDownload)
	{
		if (_global)
		{
			_global->InitAfterResourceDownload();
		}

		_isGlobalInitAfterResourceDownload = true;
	}

	if (false == _isLobbyInitAfterResourceDownload)
	{
		if (_lobby)
		{
			_lobby->InitAfterResourceDownload();
		}

		_isLobbyInitAfterResourceDownload = true;
	}

	// 주의: 리소스 다운로드 시점에는 ScopeGame 없음
	// ScopeGame은 리소스 다운이 끝난 시점에 생성되므로, Init() 시 바로 호출
}

void UGsScopeHolder::OnDrawDebug(AHUD* HUD, UCanvas* Canvas, const FDebugDisplayInfo& DisplayInfo, float& YL, float& YPos)
{
	//스케일된 뷰포트는 기기테스트후 재확인 : 1.0 인대 불필요한 로직계산 제거
	FVector2D ViewPort = UWidgetLayoutLibrary::GetViewportSize(this) /** UWidgetLayoutLibrary::GetViewportScale(this)*/;

	// bak1210 
	// 캔버스에는 DPI 스케일이 적용되어있음, 따라서 다운스케일된 스케일좌표에 찍어야 오리진 좌표가 표시된다.
	// 주의 : 표시할때에만 다운스케일 한다. 위젯을 스크린 좌표로 찍을 때는 (ViewPort.Y - _testKeyBoardHeight) 값을 활용해야함
	
	
	float scale = Canvas->GetDPIScale();

	static float SavePos = 0.0f;
	if (_testKeyBoardHeight != 0.0f && SavePos != _testKeyBoardHeight)
	{
		SavePos = _testKeyBoardHeight;
	}
	
	// Bottom -> Up(화면좌표가 하단에서 상단으로 올라갈수록 커질때)
	Canvas->DrawText(GEngine->GetLargeFont(), FString::Printf(TEXT("-@-")), (ViewPort.X / scale) * 0.5f, (ViewPort.Y - SavePos) / scale);
	// Up -> Down(화면좌표가 하단으로 내려갈수록 커질때 (IOS는 하단부터 상단으로 올라갈수록 높이가 커지는 좌표)
	Canvas->DrawText(GEngine->GetLargeFont(), FString::Printf(TEXT("-$-")), (ViewPort.X / scale) * 0.5, (ViewPort.Y - _testKeyBoardHeight) / scale);

	// 플렛폼 좌표(절대좌표)를 뷰포트 안쪽(로컬)으로 이동
#if PLATFORM_64BITS | PLATFORM_32BITS
	FVector2D MousePosition =  UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
#else
	FVector2D MousePosition(_testTouchPosition);
#endif

	FVector2D DPIPosition =  MousePosition / scale;
	// 출력은 현재좌표를 찍지만, 화면출력은 다운스케일링해서 찍는다.
	//Canvas->DrawText(GEngine->GetSmallFont(), FString::Printf(TEXT("* [%.2f, %.2f]"), MousePosition.X, MousePosition.Y), DPIPosition.X + 50.0f, DPIPosition.Y);
	Canvas->DrawText(GEngine->GetSmallFont(), FString::Printf(TEXT("* [Charge State [%d]]"), FPlatformMisc::IsBatteryChargeState()), DPIPosition.X + 50.0f, DPIPosition.Y);

	// 참고목록
	/*FVector2D widgetPos;
	USlateBlueprintLibrary::ScreenToWidgetAbsolute(this, ScaledScreen, widgetPos);

	Canvas->DrawText(GEngine->GetLargeFont(), FString::Printf(TEXT("# [%.2f, %.2f]"), widgetPos.X, widgetPos.Y), widgetPos.X, widgetPos.Y);
	FVector2D widgetLocalPos;
	
	USlateBlueprintLibrary::ScreenToWidgetLocal(this, geom, ScaledScreen, widgetLocalPos);
	Canvas->DrawText(GEngine->GetLargeFont(), FString::Printf(TEXT("WL [%.2f, %.2f]"), widgetLocalPos.X, widgetLocalPos.Y), widgetLocalPos.X, widgetLocalPos.Y);


	FVector2D viewPort;
	USlateBlueprintLibrary::ScreenToViewport(this, ScaledScreen, viewPort);
	Canvas->DrawText(GEngine->GetLargeFont(), FString::Printf(TEXT("@ [%.2f, %.2f]"), viewPort.X, viewPort.Y), viewPort.X, viewPort.Y);

	FVector2D AbviewPort, Pixel;
	USlateBlueprintLibrary::AbsoluteToViewport(this, ScaledScreen, Pixel,  AbviewPort);
	Canvas->DrawText(GEngine->GetLargeFont(), FString::Printf(TEXT("AB [%.2f, %.2f]"), AbviewPort.X, AbviewPort.Y), AbviewPort.X, AbviewPort.Y);

	Canvas->DrawText(GEngine->GetLargeFont(), FString::Printf(TEXT("PX [%.2f, %.2f]"), Pixel.X, Pixel.Y), Pixel.X, Pixel.Y);*/
	
}


void UGsScopeHolder::Update(float inTick)
{
	if (nullptr != _fpsAnalyzer && _fpsAnalyzer->GetActive())
	{
		_fpsAnalyzer->Update(GetWorld()->GetRealTimeSeconds(), inTick);
		SCREEN_MESSAGE(1001, 1.0f, FColor::Green, "Min FPS: %.2f \nMax FPS: %.2f \nAve FPS : %.2f", _fpsAnalyzer->GetMinFPS(), _fpsAnalyzer->GetMaxFPS(), _fpsAnalyzer->GetAveFPS());
	}

	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		gameUserSettings->Update(inTick);
	}

	_message->Update(inTick);

	_global->Update(inTick);
	_lobby->Update(inTick);
	_game->Update(inTick);

	if (_mapPostUpdateFunc.Num() != 0)
	{
		for (auto iter : _mapPostUpdateFunc)
		{
			if (iter.Value)
			{
				iter.Value();
			}
		}
		_mapPostUpdateFunc.Empty();
	}
}

void UGsScopeHolder::Shutdown()
{
	GMessage()->GetSystem().SendMessage(MessageSystem::SYSTEM_SHUTDOWN);
		//메시지 해지
	for (IGsMessageHandler* handler : _messageHandlers)
	{
		handler->FinalizeMessage();
		delete handler;
	}

	//스코프 해지
	if (nullptr != _game)
	{
		_game->Shutdown();
		_isGameCreated = false;
		_game = nullptr;
	}

	if (nullptr != _lobby)
	{
		_lobby->Shutdown();
		_isLobbyCreated = false;
		_lobby = nullptr;
	}
	
	if (nullptr != _global)
	{
		_global->Shutdown();
		_isGlobalCreated = false;
		_global = nullptr;
	}

	if (nullptr != _fpsAnalyzer)
	{
		delete _fpsAnalyzer;
		_fpsAnalyzer = nullptr;
	}

	if (_mapPostUpdateFunc.Num() != 0)
	{
		_mapPostUpdateFunc.Empty();
	}

	UE_LOG(LogTemp, Error, TEXT("UGsScopeHolder::Shutdown()"));

	TGsSingleton::RemoveInstance();
}

void UGsScopeHolder::StartPlayGameModeLobby()
{	
	if (false == _isLobbyCreated)
	{
		_lobby->Create();
		_isLobbyCreated = true;
		_isLobbyInitAfterResourceDownload = false;
		_lobby->Init();
	}
}

void UGsScopeHolder::BeginDestroyGameModeLobby()
{
	if (nullptr != _lobby)
	{
		_lobby->Finalize();
		_isLobbyCreated = false;
	}
}

void UGsScopeHolder::StartPlayGameModeWorld()
{
	if (false == _isGameCreated)
	{
		_game->Create();
		_isGameCreated = true;

		_game->Init();
	}
}

void UGsScopeHolder::BeginDestroyGameModeWorld()
{
	if (nullptr != _game)
	{
		_game->Finalize();
		_isGameCreated = false;
	}
}

void UGsScopeHolder::ShowFPS()
{
	if (nullptr != _fpsAnalyzer)
	{
		_fpsAnalyzer->SetActive(!_fpsAnalyzer->GetActive());
	}	
}

void UGsScopeHolder::ResetFPS()
{
	if (nullptr != _fpsAnalyzer)
	{
		_fpsAnalyzer->Reset();
	}
}

// 늦은 업데이트 처리 함수 추가
void UGsScopeHolder::AddPostUpdateFunc(EGsPostUpdateFuncType In_type, TFunction<void()> In_func)
{
	if (_mapPostUpdateFunc.Contains(In_type) == true)
	{
		return;
	}

	_mapPostUpdateFunc.Add(In_type, In_func);
}