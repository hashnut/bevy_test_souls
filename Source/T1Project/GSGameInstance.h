// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/EngineTypes.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#if WITH_EDITOR
#include "SWindow.h"
#endif
#include "GsGameInstance.generated.h"
class UWorld;
class UGsScopeHolder;
class UWorld;
class FGsTouchInputProcessor;
class SWindow;
//-------------------------------------------------------------------------------
// 게임초기화 순서
//-------------------------------------------------------------------------------
// 1. 게임 앱의 초기화(PlayMode)	:	UGameInstance::Init
// 2. 레벨에 속한 액터의 초기화		:	AActor::PostInitailizeComponents
// 3. 플레이어의 로그인			:	AGameMode::PostLogin
// 4. 게임의 시작					:	AGameMode::StartPlay -> AActor::BeginPlay
//-------------------------------------------------------------------------------

UCLASS()
class T1PROJECT_API UGsGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	

protected:
	UPROPERTY(Transient)
	UGsScopeHolder*			_scopeHolder = nullptr;

protected:
	static constexpr float	_controlScreenSaverDelay = 3.f;
	FTimerHandle			_controlScreenSaverTimerHandle;

protected:
	// 개발모드
	UPROPERTY(EditDefaultsOnly, Category = "Dev")
	bool _bIsDevMode = false;

	// View Table/Net/AddresssTable
	UPROPERTY(EditDefaultsOnly, Category = "Dev")
	FString _serverOwnerName;

	// VID
	UPROPERTY()
	FString _vidName;

	bool _IOSRestartEnable = false;

	TSharedPtr<FGsTouchInputProcessor> _touchInputProcessor;

protected:
	FString _prevMapName;

public:
	UGsGameInstance();
	virtual ~UGsGameInstance() = default;

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	virtual class ULocalPlayer* CreateInitialPlayer(FString& OutError);

	// 2021/06/29 PKT - App 강제 종료
	void QuitGame();

	// Control ScreenSaver
	void OnControlScreenSaver();

public:
	// 개발모드인가
	UFUNCTION(BlueprintPure, Category = "Dev")
	bool IsDevMode() const { return _bIsDevMode; }

public:
	UGsScopeHolder* GetScopeHolder() { return _scopeHolder; }

private:
	void OnPreLoadMap(const FString& MapName);
	void OnPostLoadMap(UWorld*);
	
#if PLATFORM_IOS
	void OnIOSBecomeActive();
#endif		

protected:
	virtual void OnStart();

	UFUNCTION()
	void ApplicationWillEnterBackgroundDelegate();

	UFUNCTION()
	void ApplicationHasEnteredForegroundDelegate();
		
public:
	bool OnPrevCloseWindow();

#if	!UE_BUILD_SHIPPING
//private:
	static void ConsoleCommand(const TArray<FString>& Arguments);
#endif
};
