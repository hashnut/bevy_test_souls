// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayMovie.h"

#include "UTIL/GsSoundUtil.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Tray/GsUITrayFadeEffect.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "Management/ScopeGlobal/GsSoundManager.h"

#include "DataSchema/GsSchemaEnums.h"
#include "DataSchema/Movie/GsSchemaMovieData.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "MediaAssets/Public/MediaTexture.h"
#include "MediaAssets/Public/MediaPlaylist.h"
#include "MediaSource.h"
#include "MediaPlayer.h"
#include "MediaPlayerFacade.h"

#include "Components/AudioComponent.h"
#include "Sound/SoundWave.h"

#include "Kismet/GameplayStatics.h"

#include "Engine/Classes/Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "Sound/GsSoundPlayer.h"

#include "Management/GsMessageHolder.h"
#include "Message/GsMessageSystem.h"
#include "Management/ScopeGlobal/GsNetManager.h"
#include "Net/GsNetBase.h"

#include "T1Project.h"
#include "UTIL/GsText.h"


void UGsUITrayMovie::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (_mediaTexture != nullptr)
	{
		_mediaTexture->ClearColor = FColor::White;
	}
}

void UGsUITrayMovie::NativeConstruct()
{
	Super::NativeConstruct();

	_isBtnAnimPlaying = true;

	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddUObject(this, &UGsUITrayMovie::OnWillEnterBackground);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(this, &UGsUITrayMovie::OnHasEnteredForeground);

	FGsMessageHolder* message = GMessage();

	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::NET_DISCONECTED, this, &UGsUITrayMovie::NetDisconected));

	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::GATEWAY_SERVER_LOGIN_RETRY, this, &UGsUITrayMovie::ReconnectStart));

	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::GAME_SERVER_RECONNECTION_SUCCESS, this, &UGsUITrayMovie::ReconnectEnd));

	_listSystemParamDelegate.Emplace(
		message->GetSystemParam().AddUObject(MessageSystem::VIEWPORT_LOST_FOCUS, this, &UGsUITrayMovie::LostFocus));

	_listSystemParamDelegate.Emplace(
		message->GetSystemParam().AddUObject(MessageSystem::VIEWPORT_RECEIVE_FOCUS, this, &UGsUITrayMovie::ReveiveFocus));


	if (UGsSoundManager* soundManager = GSound())
	{
		if (soundManager->GetOrCreateSoundMixerController())
		{
			soundManager->GetOrCreateSoundMixerController()->OnMixChanged(EGsSoundMixType::Cinemas);
		}
	}
}

void UGsUITrayMovie::NativeDestruct()
{
	FGsMessageHolder* message = GMessage();

	MSystem& msgSystem = message->GetSystem();
	for (MsgSystemHandle& handle : _listSystemDelegate)
	{
		msgSystem.Remove(handle);
	}
	_listSystemDelegate.Empty();

	MSystemParam& msgSystemParam = message->GetSystemParam();
	for (MsgSystemHandle& handle : _listSystemParamDelegate)
	{
		msgSystemParam.Remove(handle);
	}
	_listSystemParamDelegate.Empty();
	_isSkipEffect = false;

	// 주의: 로비로 가기 등으로 Close()가 명시적으로 못불렸을 경우를 위해 이 곳에서 해제.
	Stop();

	if (_mediaPlayer)
	{
		_mediaPlayer->OnEndReached.RemoveDynamic(this, &UGsUITrayMovie::OnEnd);
	}

	GUI()->ShowTicker();

	if (UGsSoundManager* soundManager = GSound())
	{
		if (soundManager->GetOrCreateSoundMixerController())
		{
			UGsSoundUtil::ResetMusicSoundVolumeBySettingValue();

			soundManager->GetOrCreateSoundMixerController()->OffMixMode(EGsSoundMixType::Cinemas);
		}
	}

	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.RemoveAll(this);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.RemoveAll(this);

	Super::NativeDestruct();
}

// 터치 이벤트 무조건 막기(카메라 회전 막기)
FReply UGsUITrayMovie::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUITrayMovie::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUITrayMovie::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (_isSkipEffect)
	{
		ShowSkipMovieButton();
		PlayAnimation(_animClickSkipEffect);
	}

	return FReply::Unhandled();
}

void UGsUITrayMovie::CloseInternal()
{
	Super::CloseInternal();

	// 무비 종료가 되면 FadeIn을 재생하여 부드럽게 빠지도록 처리
	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
	{
		return;
	}
	// 로딩이 아닐때만 페이드 사용
	if (false == IsLoadingMovie())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("TrayFadeEffect"));
		if (widget.IsValid())
		{
			if (UGsUITrayFadeEffect* fadeEffectWidget = Cast<UGsUITrayFadeEffect>(widget.Get()))
			{
				fadeEffectWidget->SetFadeAnimation(UGsUITrayFadeEffect::EGsFadeAnimationType::FadeIn);
				fadeEffectWidget->SetAnimationFinishedCallback([fadeEffectWidget]()
					{
						// Fade UI 닫기
						fadeEffectWidget->Close();
					});
			}
		}
	}
}

bool UGsUITrayMovie::Play(const FString& inName, bool inIsShowSkipMovieButton,
	TFunction<void()> In_callbackCloseFunc)
{
	_callbackCloseMovie = In_callbackCloseFunc;
	if (_mediaTexture != nullptr)
	{
		_mediaTexture->ClearColor = GetClearColor();
	}

	const UGsTableManager& mgr = UGsTableManager::GetInstance();

	const UGsTable* table = mgr.GetTable(FGsSchemaMovieData::StaticStruct());

	if (nullptr == table)
	{
		return false;
	}

	if (false == table->FindRow<FGsSchemaMovieData>(FName(*inName), _playMovieData))
	{
		return false;
	}

	_isSkipEffect = false;
	if (inIsShowSkipMovieButton)
	{
		if (_playMovieData->isSkip)
		{
			if (_playMovieData->isSkipEffect)
			{
				HideSkipMovieButton();
				_isSkipEffect = true;
			}
			else
			{
				ShowSkipMovieButton();
			}
		}
		else
		{
			HideSkipMovieButton();
		}
	}
	else
	{
		HideSkipMovieButton();
	}

	_isLoop = _playMovieData->isLoop;
	bool Result = PlayMovieByTblData(_isLoop);

#ifdef TEST_DELAY_MOVIE
	float delay =
		GData()->GetGlobalData()->_loadingCallbackDelay;

	if (UWorld* world = GUI()->GetWorld())
	{
		world->GetTimerManager().SetTimer(_timerHandle,
			FTimerDelegate::CreateUObject(this, &UGsUITrayMovie::CallbackTimeEnd),
			delay, false);
	}
#endif
	GUI()->HideTicker();

	return Result;
}

// 테이블 데이터로 무비 플레이
bool UGsUITrayMovie::PlayMovieByTblData(bool In_isLoop)
{
	const FString soundPath = _playMovieData->sound.ToSoftObjectPath().ToString();
	const FString mediaSourcePath = _playMovieData->mediaSource.ToSoftObjectPath().ToString();

	if (soundPath.IsEmpty() == false)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			_soundId = soundPlayer->PlaySoundByPath(soundPath);
		}
	}
	PlayMediaSource(mediaSourcePath, In_isLoop);

	return true;
}

void UGsUITrayMovie::Stop()
{
	if (_mediaPlayer)
	{
		_mediaPlayer->Close();
	}

	if (nullptr == GScope())
	{
		return;
	}

	if (0 < _soundId)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			soundPlayer->StopSound(_soundId);
		}
	}

	if (nullptr != _callbackCloseMovie)
	{
		_callbackCloseMovie();
		_callbackCloseMovie = nullptr;
	}

	// B2: 씬 전환으로 지우는 도중에 STOP_MOVIE 메시지에 의해 UI가 추가되며 크래시 나는 문제 수정
	if (UGsUIManager* uiMgr = GUI())
	{
		if (false == uiMgr->IsRemovingWidgets())
		{
			if (FGsMessageHolder* msg = GMessage())
			{
				msg->GetUI().SendMessage(MessageUI::STOP_MOVIE, nullptr);
			}
		}
	}
}

void UGsUITrayMovie::ClearCloseMovieCallBack()
{
	_callbackCloseMovie = nullptr;
}

void UGsUITrayMovie::Pause(bool In_isPause)
{
	if (_mediaPlayer)
	{
		if (In_isPause)
		{
			if (_mediaPlayer->IsPlaying())
			{
				_mediaPlayer->Pause();
			}
		}
		else
		{
			if (_mediaPlayer->IsPaused())
			{
				_mediaPlayer->Play();
			}
		}
	}

	if (UGsSoundManager* soundManager = GSound())
	{
		if (UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController())
		{
			soundMixerController->OffMixMode(EGsSoundMixType::Cinemas);
		}

		if (UGsSoundPlayer* soundPlayer = soundManager->GetOrCreateSoundPlayer())
		{
			if (0 < _soundId)
			{
				soundPlayer->PauseSound(In_isPause, _soundId);
			}
		}
	}
}

bool UGsUITrayMovie::PlayMediaSource(const FString& inMediaSource,
	bool In_isLoop)
{
	bool Result = false;

	UObject* mediaSource = GetMediaSource(inMediaSource);
	if (mediaSource)
	{
		FMediaPlayerOptions option;
		option.PlayOnOpen = EMediaPlayerOptionBooleanOverride::Enabled;
		option.Loop =
			(In_isLoop == true) ? EMediaPlayerOptionBooleanOverride::Enabled :
			EMediaPlayerOptionBooleanOverride::Disabled;
		option.SeekTime = 0;

		Result = _mediaPlayer->OpenSourceWithOptions(Cast<UMediaSource>(mediaSource),
			option);
		_mediaPlayer->SetNativeVolume(1.0f);

		if (false == IsLoadingMovie())
		{
			GMessage()->GetUI().SendMessage(MessageUI::PLAY_MOVIE, nullptr);
		}

		_mediaPlayer->OnEndReached.AddUniqueDynamic(this, &UGsUITrayMovie::OnEnd);
	}
	return Result;
}

void UGsUITrayMovie::HideSkipMovieButton()
{
	if (_skipButton)
	{
		_skipButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUITrayMovie::ShowSkipMovieButton()
{
	if (_skipButton)
	{
		if (false == _skipButton->IsVisible())
		{
			_skipButton->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UGsUITrayMovie::OnEnd()
{
	if (_mediaPlayer)
	{
		UMediaPlaylist* playList = _mediaPlayer->GetPlaylist();
		if (nullptr != playList)
		{
			int listNum = playList->Num();
			for (int i = 0; i < listNum; ++i)
			{
				playList->RemoveAt(i);
			}
		}
	}

	if (_isLoop == false)
	{
		Close();
	}
}

void UGsUITrayMovie::OnSkip()
{
	if (_isBtnAnimPlaying)
		return;

	OnEnd();
}

void UGsUITrayMovie::PlayMovie(FString inName)
{
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("TrayMovie"));
		if (widget.IsValid())
		{
			if (UGsUITrayMovie* movieWidget = Cast<UGsUITrayMovie>(widget.Get()))
			{
				movieWidget->Stop();
				movieWidget->Play(inName);
			}
		}
	}
}

void UGsUITrayMovie::StopMovie()
{
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("TrayMovie"));
		if (widget.IsValid())
		{
			if (UGsUITrayMovie* movieWidget = Cast<UGsUITrayMovie>(widget.Get()))
			{
				movieWidget->Stop();
			}
		}
	}
}

bool UGsUITrayMovie::PlayMovieCallbackFunc(FString inName, TFunction<void()> In_callbackCloseFunc)
{
	bool Result = false;

	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->OpenAndGetWidget(TEXT("TrayMovie"));
		if (widget.IsValid())
		{
			if (UGsUITrayMovie* movieWidget = Cast<UGsUITrayMovie>(widget.Get()))
			{
				Result = movieWidget->Play(inName, true, In_callbackCloseFunc);
			}
		}
	}

	return Result;
}

void UGsUITrayMovie::StopMovieWithoutCallbackFunc()
{
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("TrayMovie"));
		if (widget.IsValid())
		{
			if (UGsUITrayMovie* movieWidget = Cast<UGsUITrayMovie>(widget.Get()))
			{
				movieWidget->ClearCloseMovieCallBack();
				movieWidget->Stop();
			}
		}
	}
}

void UGsUITrayMovie::MoviePause(bool In_isPause)
{
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("TrayMovie"));
		if (widget.IsValid())
		{
			if (UGsUITrayMovie* movieWidget = Cast<UGsUITrayMovie>(widget.Get()))
			{
				movieWidget->Pause(In_isPause);
			}
		}
	}
}

void UGsUITrayMovie::PlaySound(const FString& inSound)
{
	if (_audioComp)
	{
		_audioComp->Stop();
	}

	UObject* source = StaticLoadObject(USoundWave::StaticClass(), NULL, *inSound);

	if (source)
	{
		if (USoundWave* sound = Cast<USoundWave>(source))
		{
			if (nullptr == _audioComp)
			{
				_audioComp = UGameplayStatics::SpawnSound2D(GetWorld(), sound);
			}
			else
			{
				_audioComp->SetSound(sound);
			}

			if (_audioComp)
			{
				UGsSoundUtil::ResetMusicSoundVolumeBySettingValue();
				_audioComp->bStopWhenOwnerDestroyed = false;
				_audioComp->Play();
			}
		}
	}
}

UObject* UGsUITrayMovie::GetMediaSource(const FString& inMediaSource)
{
	return StaticLoadObject(UMediaSource::StaticClass(), NULL, *inMediaSource);
}


void UGsUITrayMovie::OnBtnAnimFinished()
{
	_isBtnAnimPlaying = false;
}

void UGsUITrayMovie::OnWillEnterBackground()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("GsUITrayMovie OnWillEnterBackground"));

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

void UGsUITrayMovie::OnHasEnteredForeground()
{
#if !UE_BUILD_SHIPPING
	const FString logString = FString::Printf(TEXT("GsUITrayMovie OnHasEnteredForeground"));

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


void UGsUITrayMovie::NetDisconected()
{
	GSLOG(Log, TEXT("UGsUITrayMovie::NetDisconected()"));

	if (_mediaPlayer)
	{
		if (_mediaPlayer->IsPlaying())
		{
			if (false == _mediaPlayer->IsPaused())
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
}

void UGsUITrayMovie::ReconnectStart()
{
	GSLOG(Log, TEXT("UGsUITrayMovie::ReconnectStart()"));

	if (_mediaPlayer)
	{
		if (_mediaPlayer->IsPlaying())
		{
			if (false == _mediaPlayer->IsPaused())
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
}

void UGsUITrayMovie::ReconnectEnd()
{
	GSLOG(Log, TEXT("UGsUITrayMovie::ReconnectEnd()"));

	if (_mediaPlayer)
	{
		if (true == _mediaPlayer->IsPaused())
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

void UGsUITrayMovie::LostFocus(const IGsMessageParam*)
{
	if (_mediaPlayer)
	{
		if (_mediaPlayer->IsPlaying())
		{
			if (false == _mediaPlayer->IsPaused())
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
}

void UGsUITrayMovie::ReveiveFocus(const IGsMessageParam*)
{
	GSLOG(Log, TEXT("UGsUITrayMovie::ReveiveFocus()"));

	if (_mediaPlayer)
	{
		if (true == _mediaPlayer->IsPaused())
		{
			if (FGsNetManager* netMgr = GNet())
			{
				TSharedPtr<FGsNetBase> netBase = netMgr->GetActive();
				if (netBase.IsValid())
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
	}
}
