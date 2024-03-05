// Fill out your copyright notice in the Description page of Project Settings.

#include "GsGameInstance.h"

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Classes/GsSpawn.h"

//#include "Data/GsDataContainManager.h"
//#include "Data/GsResourceManager.h"
//#include "Level/GsLevelManager.h"
//#include "GameObject/GsGlobalGameObjects.h"
//#include "GameObject/Item/GsItemManager.h"
//#include "Sequence/GsSequenceManager.h"
//#include "Management/ScopeGlobal/GsSoundManager.h"

#include "Management/ScopeGlobal/GsNetManager.h"
#include "Management/GsScopeHolder.h"
#include "Data/GsResourceManager.h"

#include "T1Project.h"
#include "Util/GsDir.h"

#include "GameMode/GsGameModeBase.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Loading/GsUILoadingScreen.h"
#include "Runtime/MoviePlayer/Public/MoviePlayer.h"

#include "GenericPlatform/GenericPlatformMisc.h"
#include "Management/GsMessageHolder.h"
#include "UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "UI/ScaleRule/GsUIScaleRule.h"
#include "Classes/Engine/GameViewportClient.h"
#include "Classes/Engine/UserInterfaceSettings.h"

#include "CoreDelegates.h"

#if PLATFORM_IOS
#include "IOS/IOSAppDelegate.h"
#endif
#include "Net/GsNetBase.h"
#include "Message/GsMessageSystem.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/ScaleRule/GsUIScaleRule.h"

#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UILib/Manager/GsLoadingScreenController.h"
#include "Engine/LocalPlayer.h"
#include "SWindow.h"

#define USE_TOUCH_EFFECT

#ifdef USE_TOUCH_EFFECT
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/IInputProcessor.h"
#include "Input/GsTouchInputProcessor.h"
#endif
#include "Data/GsGameClientVersion.h"
#include "Data/GsDataContainManager.h"


//-------------------------------------------------------------------------------
// 게임초기화 순서
//-------------------------------------------------------------------------------
// 1. 게임 앱의 초기화(PlayMode)	:	UGameInstance::Init
// 2. 레벨에 속한 액터의 초기화		:	AActor::PostInitailizeComponents
// 3. 플레이어의 로그인			:	AGameMode::PostLogin
// 4. 게임의 시작					:	AGameMode::StartPlay -> AActor::BeginPlay
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
// 플러그인 초기화, 플레이모드 실행시 호출됨 ------- 중복처리 주의
//-------------------------------------------------------------------------------
UGsGameInstance::UGsGameInstance()
{
	GSLOG_S(Warning);
//#if PLATFORM_WINDOWS && !WITH_EDITOR
//	gGameInstace = this;
//#endif
}

//-------------------------------------------------------------------------------
// 플레이모드(즉 실제게임)에서만 호출
//-------------------------------------------------------------------------------
void UGsGameInstance::Init()
{
	GSLOG_S(Warning);

	Super::Init();
	
#if PLATFORM_IOS
	UIApplication* dummyApplication = [UIApplication sharedApplication];

	if (dummyApplication != nil) NSLog(@"############################## dummyApplication exist................\n");
	else NSLog(@"############################## dummyApplication is nil................\n");

	Class clzHIVEAppDelegate = NSClassFromString(@"HIVEAppDelegate");
	SEL selApplicationDidFinishLaunchingWithOptions = NSSelectorFromString(@"application:didFinishLaunchingWithOptions:");
	if (clzHIVEAppDelegate != nil && [clzHIVEAppDelegate respondsToSelector : selApplicationDidFinishLaunchingWithOptions]) {
		NSMethodSignature* method = [clzHIVEAppDelegate methodSignatureForSelector : selApplicationDidFinishLaunchingWithOptions];
		if (method != nil) {

			NSInvocation* invocation = [NSInvocation invocationWithMethodSignature : method];
			[invocation setSelector : selApplicationDidFinishLaunchingWithOptions] ;
			[invocation setTarget : clzHIVEAppDelegate] ;
			[invocation setArgument : (void*)&dummyApplication atIndex : 2] ;
			NSDictionary* localLaunchOptions = [IOSAppDelegate GetDelegate].launchOptions;
			if (localLaunchOptions != nil) {
				[invocation setArgument : (void*)&localLaunchOptions atIndex : 3] ;
			}
			[invocation invoke];
		}
	}
#endif

	//매지지먼트 스코프핸들러 초기화
	_scopeHolder = NewObject<UGsScopeHolder>(this);
	check(_scopeHolder);
	_scopeHolder->Init();
	
	// NetManager 초기화 후에 추가 설정
	if (FGsNetManager* netManager = GNet())
	{
		//네트워크 메시지 등록
		netManager->RegisterMessage();
		// 시연빌드의 테스트를 위한 임시 코드들
		// 계정유저 설정
		netManager->SetServerUserName(_serverOwnerName);

// #if !PLATFORM_WINDOWS
// 		if (IsBranch(EGsGameClientBranchType::MOBILE_NET_REST_MODE))
// 		{
// 			FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddUObject(this, &UGsGameInstance::ApplicationWillEnterBackgroundDelegate);
// 			FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(this, &UGsGameInstance::ApplicationHasEnteredForegroundDelegate);
// 		}		
// #endif	// !PLATFORM_WINDOWS
	}

	// ScreenSaver Controler용 타이머 시작
	GetTimerManager().SetTimer(
		_controlScreenSaverTimerHandle, this, &UGsGameInstance::OnControlScreenSaver,
		_controlScreenSaverDelay, false, _controlScreenSaverDelay
	);

	// 로딩 전후 이벤트 등록
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UGsGameInstance::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGsGameInstance::OnPostLoadMap);

	_IOSRestartEnable = false;

#if PLATFORM_IOS
	//FIOSCoreDelegates::OnDidBecomeActive.AddDynamic(this, UGsGameInstance::OnIOSBecomeActive);
#endif

	_prevMapName.Empty();

#ifdef USE_TOUCH_EFFECT
	_touchInputProcessor = MakeShared<FGsTouchInputProcessor>();
	FSlateApplication::Get().RegisterInputPreProcessor(_touchInputProcessor);
#endif // USE_TOUCH_EFFECT

#if	!UE_BUILD_SHIPPING
	FConsoleCommandWithArgsDelegate Delegate;
	Delegate.BindStatic(&UGsGameInstance::ConsoleCommand);

	IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("zenonia"),
		TEXT("Commands dedicated to Zenonia profiling"),
		Delegate);
#endif
}

#if	!UE_BUILD_SHIPPING
void UGsGameInstance::ConsoleCommand(const TArray<FString>& Arguments)
{
	UE_LOG(LogTemp, Warning, TEXT("Commands dedicated to Zenonia profiling"));

	UWorld* world = GEngine->GameViewport->GetWorld();
	if (world != nullptr)
	{
		//UEngine::Exec(world, FString::Printf(TEXT("stat fps")));
		UKismetSystemLibrary::ExecuteConsoleCommand(world, FString::Printf(TEXT("stat FPS"), nullptr));
		UKismetSystemLibrary::ExecuteConsoleCommand(world, FString::Printf(TEXT("stat unitgraph"), nullptr));
		UKismetSystemLibrary::ExecuteConsoleCommand(world, FString::Printf(TEXT("stat SceneRendering"), nullptr));
	}
}
#endif

//-------------------------------------------------------------------------------
void UGsGameInstance::QuitGame()
{	
	if (UWorld* world = GEngine->GameViewport->GetWorld())
	{		
#if PLATFORM_IOS
		// 2021/08/25 PKT - IOS 종료..( Back Ground 숨김 )
		//UKismetSystemLibrary::QuitGame(world, nullptr, EQuitPreference::Background, false);
		// 2021/08/25 PKT - IOS 종료..
		int* forceCrashByExit = nullptr;
		(*forceCrashByExit) = 10;
		_IOSRestartEnable = true;
#else
		// 2021/08/25 PKT - etc..
		UKismetSystemLibrary::QuitGame(world, nullptr, EQuitPreference::Quit, false);
#endif		
	}
}

//-------------------------------------------------------------------------------
void UGsGameInstance::Shutdown()
{
// #if !PLATFORM_WINDOWS
// 	if (IsBranch(EGsGameClientBranchType::MOBILE_NET_REST_MODE))
// 	{
// 		FCoreDelegates::ApplicationWillEnterBackgroundDelegate.RemoveAll(this);
// 		FCoreDelegates::ApplicationHasEnteredForegroundDelegate.RemoveAll(this);
// 	}	
// #endif

#if WITH_EDITOR
	// 이전에 사용한 전역 변수가 남아서 에디터에서 시작부터 Scale먹는 문제 수정
	UGsUIScaleRule::SetUIScaleValue(1.f);
#endif

#ifdef USE_TOUCH_EFFECT
	if (_touchInputProcessor.IsValid())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(_touchInputProcessor);
		_touchInputProcessor.Reset();
	}
#endif // USE_TOUCH_EFFECT

	GetTimerManager().ClearTimer(_controlScreenSaverTimerHandle);

	if (nullptr != _scopeHolder)
	{
		_scopeHolder->Shutdown();
		_scopeHolder = nullptr;
	}

	_prevMapName.Empty();

	Super::Shutdown();

	FGsDir::CleanUp();
}

ULocalPlayer* UGsGameInstance::CreateInitialPlayer(FString& OutError)
{

	return Super::CreateInitialPlayer(OutError);
}

//-------------------------------------------------------------------------------
void UGsGameInstance::OnControlScreenSaver()
{
	// ScreenSaver 진입 금지 - Sleep 금지
	UKismetSystemLibrary::ControlScreensaver(false);	
}

void UGsGameInstance::OnPreLoadMap(const FString& MapName)
{
	// 로비쪽에서 로딩창을 보여주지 않기 위함
	if (TEXT("/Game/Maps/Begin") == MapName ||
		TEXT("/Game/Launcher/Launcher") == MapName)
	{
		_prevMapName.Empty();
		return;
	}
	else if (TEXT("/Game/Maps/Lobby/Lobby") == MapName)
	{
		// 인게임에서 넘어가는 상황엔 보여야 하므로 예외처리 
		if (_prevMapName.IsEmpty() ||
			TEXT("/Game/Maps/Begin") == MapName ||
			TEXT("/Game/Launcher/Launcher") == MapName)
		{
			return;
		}
	}

	_prevMapName = MapName;

	if (UGsUIManager* uiMgr = GUI())
	{
		if (UGsUILoadingScreen* loadingWidget = uiMgr->GetLoadingController()->GetLoadingWidgetForModule())
		{
			if (IsMoviePlayerEnabled())
			{
				GMessage()->GetUI().SendMessageAsync(MessageUI::PREV_LOAD_MAP, nullptr);

				// MoviePlayer 모듈에 로딩화면 전달
				if (IGameMoviePlayer* moviePlayer = GetMoviePlayer())
				{
					// Slate를 생성하지 않고, UUserWidget의 Slate 포인터를 가져와서 랜더링 하도록 처리
					FLoadingScreenAttributes LoadingScreen;
					LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
					LoadingScreen.MinimumLoadingScreenDisplayTime = 2; // 최소 2초는 출력되도록 보장
					LoadingScreen.WidgetLoadingScreen = loadingWidget->TakeWidget();

#ifdef RAON_ENGINE_DPI_SCALE_MOVIE_PLAYER_YJCHOUNG
					float viewportScale = 1.f;
					if (UGameViewportClient* gameViewport = GetGameViewportClient())
					{
						FVector2D viewportSize;
						gameViewport->GetViewportSize(viewportSize);

						viewportScale = GetDefault<UUserInterfaceSettings>(
							UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(
								FIntPoint(viewportSize.X, viewportSize.Y));
					}

					moviePlayer->SetViewportDPIScale(viewportScale);
#endif
					moviePlayer->SetupLoadingScreen(LoadingScreen);
				}
			}
		}
	}

	GResource()->CancelAll();	
}

void UGsGameInstance::OnPostLoadMap(UWorld*)
{	
	GMessage()->GetUI().SendMessageAsync(MessageUI::POST_LOAD_MAP, nullptr);
}

#if PLATFORM_IOS
void UGsGameInstance::OnIOSBecomeActive()
{
	if (false == _IOSRestartEnable)
	{
		return;
	}

	_IOSRestartEnable = false;

	/*FGsUIHelper::TrayDimmed(false);

	GMessage()->GetSystem().SendMessage(MessageSystem::BACKTO_LOGIN);*/
}
#endif


void UGsGameInstance::OnStart()
{
	Super::OnStart();
} 

void UGsGameInstance::ApplicationWillEnterBackgroundDelegate()
{
	if (FGsNetManager* netManager = GNet())
	{
		if (netManager)
		{
			netManager->SetRestMode(true);
		}
	}
}

void UGsGameInstance::ApplicationHasEnteredForegroundDelegate()
{
	if (FGsNetManager* netManager = GNet())
	{
		if (netManager)
		{
			netManager->SetRestMode(false);
		}
	}
}

bool UGsGameInstance::OnPrevCloseWindow()
{
	GSLOG_S(Warning);

	/**
	 * 일반적으로 GameThread에서 들어옴. Windows Message는 GameThread에서 처리 함.
	 */
	if (false == IsInGameThread())
	{
		return false;
	}

	/**
	 * 게임 쓰레드라도 로딩창이 떠 있는 경우가 있다. 로딩 시 종료 하기 이벤트 발생 시 UI 처리 순서보다 Windows Message 처리 순서가 높으므로...
	 */

	// 2023/5/29 PKT - 종료
	FGsUIHelper::PopupQuitGame();
	
	return true;
}