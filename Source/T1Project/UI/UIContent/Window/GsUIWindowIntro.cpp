// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowIntro.h"
#include "Components/PanelWidget.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeLobby/GsLobbyDataManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Message/GsMessageStage.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Data/GsLobbyConstant.h"
#include "Data/GsResourceManager.h"
#include "Sound/GsSoundPlayer.h"
#include "MediaPlayer.h"
#include "MediaAssets/Public/MediaSource.h"
#include "MediaUtils/Public/MediaPlayerOptions.h"
#include "MediaAssets/Public/MediaTexture.h"
#include "Misc/Paths.h"
#include "Misc/CoreDelegates.h"


void UGsUIWindowIntro::NativeOnInitialized()
{
	Super::NativeOnInitialized();	

	_btnSkip->OnClicked.AddDynamic(this, &UGsUIWindowIntro::OnClickSkip);
}

void UGsUIWindowIntro::NativeConstruct()
{
	Super::NativeConstruct();

	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddUObject(this, &UGsUIWindowIntro::OnWillEnterBackground);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(this, &UGsUIWindowIntro::OnHasEnteredForeground);

	bIsAnimationStop = false;

#if WITH_EDITOR
	_btnSkip->SetVisibility(ESlateVisibility::Visible);
#else
	_btnSkip->SetVisibility(ESlateVisibility::Collapsed);
#endif

	_panelRating->SetVisibility(ESlateVisibility::Collapsed);

	if (_mediaPlayer)
	{
		_mediaPlayer->OnEndReached.AddDynamic(this, &UGsUIWindowIntro::OnMovieEnd);
	}
	
	PlayMovie();
}

void UGsUIWindowIntro::NativeDestruct()
{
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.RemoveAll(this);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.RemoveAll(this);

	// 다른곳에서 재생한것을 끄지 않기 위함
	if (_mediaPlayer)
	{
		StopMovie();
		_mediaPlayer->OnEndReached.RemoveDynamic(this, &UGsUIWindowIntro::OnMovieEnd);
	}
	
	bIsAnimationStop = true;
	StopAllAnimationsEx();
	bIsAnimationStop = false;

	if (0 < _soundId)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->StopSound(_soundId);
		}
	}
	_soundId = 0;

	Super::NativeDestruct();
}

void UGsUIWindowIntro::PlayMovie()
{
	if (_mediaTexture)
	{
		_mediaTexture->ClearColor = FColor::Black;
	}
	
	bool bSuccess = false;
	if (_mediaPlayer)
	{
		_mediaPlayer->Close();

		if (const UGsLobbyConstant* lobbyConst = LSLobbyData()->GetLobbyConstantData())
		{
			const FString mediaSourcePath = lobbyConst->_introMovie.ToSoftObjectPath().ToString();
			_fileName = FPaths::GetBaseFilename(mediaSourcePath);

			if (UObject* mediaSource = GResource()->LoadSync<UObject>(FSoftObjectPath(mediaSourcePath)))
			{
				FMediaPlayerOptions option;
				option.PlayOnOpen = EMediaPlayerOptionBooleanOverride::Enabled;
				option.Loop = EMediaPlayerOptionBooleanOverride::Disabled;
				option.SeekTime = 0;
			
				_mediaPlayer->OpenSourceWithOptions(Cast<UMediaSource>(mediaSource), option);
				_mediaPlayer->SetNativeVolume(1.0f);
				_mediaPlayer->Rewind();

				bSuccess = true;
			
				// 사운드 플레이
				if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
				{
					_soundId = soundPlayer->PlaySoundBgmByPath(lobbyConst->_pathBgmIntro);
				}
			}
		}
		else
		{
			GSLOG(Error, TEXT("UGsUIWindowIntro lobbyConst is null"));
		}
	}

	// 무비 재생에 실패하면 바로 등급 애니로 넘어감
	if (false == bSuccess)
	{
		GSLOG(Error, TEXT("UGsUIWindowIntro PlayMovie Failed"));
		PlayRatingAnimation();
	}
}

void UGsUIWindowIntro::StopMovie()
{
	FText currFileName = _mediaPlayer->GetMediaName();
	if (_fileName == currFileName.ToString())
	{
		_mediaPlayer->Close();
	}

	if (0 < _soundId)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->StopSound(_soundId);
		}
	}

	_soundId = 0;
}

void UGsUIWindowIntro::PlayRatingAnimation()
{
	_panelRating->SetVisibility(ESlateVisibility::HitTestInvisible);
	
	// UI에 이벤트 전달
	StartRatingAnimation();
}

void UGsUIWindowIntro::FinishIntro()
{
	Close(true, false);

	// stop이 아니라 정상적으로 종료시에만 부르도록 함
	if (false == bIsAnimationStop)
	{
		if (FGsMessageHolder* msgMgr = GMessage())
		{
			FGsLobbyFlowMsgParam param(MessageStage::FNINSHED_INTRO_EFFECT);
			msgMgr->GetStageParam().SendMessage(MessageStage::LOBBY_STATE_UPDATE_NOTIFY, &param);
		}
	}
}

void UGsUIWindowIntro::OnClickSkip()
{
#if WITH_EDITOR
	StopMovie();
	FinishIntro();
#endif
}

void UGsUIWindowIntro::OnMovieEnd()
{
	StopMovie();
	PlayRatingAnimation();
}

void UGsUIWindowIntro::OnRatingAnimationFinished()
{
	FinishIntro();
}

void UGsUIWindowIntro::OnWillEnterBackground()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("GsUIWindowIntro OnWillEnterBackground"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, logString, true, textScale);

	GSLOG(Warning, TEXT("%s"), *logString);
#endif

	if (_mediaPlayer)
	{
		if (_mediaPlayer->IsPlaying())
		{
			_mediaPlayer->Pause();

			if (0 < _soundId)
			{
				if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
				{
					soundPlayer->PauseSound(true, _soundId);
				}
			}
		}
	}
}

void UGsUIWindowIntro::OnHasEnteredForeground()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("GsUIWindowIntro OnHasEnteredForeground"));

	FVector2D textScale{ 2.f, 2.f };
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Orange, logString, true, textScale);

	GSLOG(Warning, TEXT("%s"), *logString);
#endif

	if (_mediaPlayer)
	{
		if (_mediaPlayer->IsPaused())
		{
			_mediaPlayer->Play();

			if (0 < _soundId)
			{
				if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
				{
					soundPlayer->PauseSound(false, _soundId);
				}
			}
		}
	}
}

bool UGsUIWindowIntro::OnBack()
{
	// Back키 처리 무시
	return true;
}