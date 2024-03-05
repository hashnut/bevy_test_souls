// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameModeBase.h"
#include "T1Project.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "UTIL/GsLevelUtil.h"
#include "Significance/GsGameViewPortClient.h"
#include "Public/GsTableManager.h"
#include "GSGameInstance.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/LevelStreaming.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Option/GsGameUserSettings.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "UTIL/GsText.h"

//--------------------------------------------------------------------------------------------------
AGsGameModeBase::AGsGameModeBase()
{	
	PrimaryActorTick.bCanEverTick = true;
}
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
AGsGameModeBase::~AGsGameModeBase()
{	
}

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void AGsGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	//GSLOG(Warning, TEXT("InitGame Call"));
}

//--------------------------------------------------------------------------------------------------
// 플레이어가 게임에 입장하는 순간(로그인), Editor에서 Play
//--------------------------------------------------------------------------------------------------
void AGsGameModeBase::PostLogin(APlayerController * newPlayer)
{
	//GSLOG(Warning, TEXT("%s"), *newPlayer->GetFName().ToString());

	Super::PostLogin(newPlayer);

	//GSLOG(Warning, TEXT("PostLogin End"));
}

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void AGsGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (nullptr != _scopeHolder)
	{
		_scopeHolder->Update(DeltaSeconds);
	}
	
}

void AGsGameModeBase::EndPlay(const EEndPlayReason::Type inEndPlayReason)
{
	Super::EndPlay(inEndPlayReason);
}

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void AGsGameModeBase::BeginDestroy()
{
	/*if (nullptr != _gameObjectManager)
	{
		_gameObjectManager->Uninitialize();
		_gameObjectManager = nullptr;
	}*/

	Super::BeginDestroy();
}

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void AGsGameModeBase::StartPlay()
{
	Super::StartPlay();
	
	if (UGsGameInstance* instance = Cast<UGsGameInstance>(GetGameInstance()))
	{
		_scopeHolder = instance->GetScopeHolder();
	}

	if (UGameViewportClient* view = GetWorld()->GetGameViewport())
	{
		if (UGsGameViewPortClient* gsView = CastChecked<UGsGameViewPortClient>(view))
		{
			gsView->UpdateDragTriggerDistance();
		}
	}
}


//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void AGsGameModeBase::InitSignificanceManager()
{
	if (GetWorld())
	{
		if (UGameViewportClient * view = GetWorld()->GetGameViewport())
		{
			if (UGsGameViewPortClient * gsView = CastChecked<UGsGameViewPortClient>(view))
			{
				gsView->InitSignificanceManager();
			}
		}
	}
}

const DevicePlatformType AGsGameModeBase::GetPlatformType()
{
	DevicePlatformType platfromType = DevicePlatformType::EDITOR;

#if WITH_EDITOR
#else // WITH_EDITOR
#if PLATFORM_WINDOWS
	platfromType = DevicePlatformType::WINDOWS;
#elif PLATFORM_ANDROID
	platfromType = DevicePlatformType::GOOLE_ANDROID;
#elif PLATFORM_IOS
	platfromType = DevicePlatformType::APPLE_IOS;
#endif
#endif	// WITH_EDITOR

	GSLOG(Log, TEXT("platform : %s"), *FGsTextUtil::GetStringFromEnum(TEXT("DevicePlatformType"), platfromType));

	return platfromType;
}