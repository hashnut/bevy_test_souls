#include "GsUITicker.h"
#include "UMG/Public/Animation/UMGSequencePlayer.h"
#include "MovieScene/Public/Evaluation/MovieScenePlayback.h"
#include "MovieScene/Public/MovieSceneTimeHelpers.h"

void UGsUITicker::PauseAllAnimations()
{
	_animationPause = true;
	TArray<UUMGSequencePlayer*, TInlineAllocator<8>> CurrentActivePlayers(ActiveSequencePlayers);
	for (UUMGSequencePlayer* FoundPlayer : CurrentActivePlayers)
	{
		if (FoundPlayer->GetPlaybackStatus() == EMovieScenePlayerStatus::Playing)
		{
			if (auto anim = FoundPlayer->GetAnimation())
			{
				_timeSet.Add(FoundPlayer, FoundPlayer->GetCurrentTime().AsSeconds());
				FoundPlayer->SetCurrentTime(anim->GetEndTime() - 0.1f);
			}

			FoundPlayer->Tick(0.1f);

			//Pause() Stop으로 바꿔놓아 제거에 체크될 수 있음 Pause로 상태를 바꿔둔다
			FoundPlayer->Pause();
			FoundPlayer->SetPlaybackStatus(EMovieScenePlayerStatus::Paused); 
		}
	}
}

void UGsUITicker::RestartAllAnimations()
{
	if (IsAnimationPause())
	{
		_animationPause = false;
		TArray<UUMGSequencePlayer*, TInlineAllocator<8>> CurrentActivePlayers(ActiveSequencePlayers);
		for (UUMGSequencePlayer* FoundPlayer : CurrentActivePlayers)
		{
			if (FoundPlayer->GetPlaybackStatus() == EMovieScenePlayerStatus::Paused)
			{
				if (auto iter = _timeSet.Find(FoundPlayer))
				{
					FoundPlayer->SetCurrentTime(*iter);
				}
				else FoundPlayer->SetCurrentTime(0);
				FoundPlayer->SetPlaybackStatus(EMovieScenePlayerStatus::Playing);
			}
		}

		_timeSet.Reset();
	}
}

void UGsUITicker::ClearAllPauseAnimations()
{
	if (IsAnimationPause())
	{
		_animationPause = false;
		_timeSet.Reset();

		TArray<UUMGSequencePlayer*, TInlineAllocator<8>> CurrentActivePlayers(ActiveSequencePlayers);
		for (UUMGSequencePlayer* FoundPlayer : CurrentActivePlayers)
		{
			if (FoundPlayer->GetPlaybackStatus() == EMovieScenePlayerStatus::Paused)
			{
				if (auto anim = FoundPlayer->GetAnimation())
				{
					FoundPlayer->SetCurrentTime(anim->GetEndTime() - 0.1f);
				}
				FoundPlayer->Tick(0.1f);
				FoundPlayer->SetPlaybackStatus(EMovieScenePlayerStatus::Playing);
			}
		}
	}
}

void UGsUITicker::OnAnimationStartedPlaying(UUMGSequencePlayer& Player)
{
	if (IsAnimationPause())
	{
		if (auto anim = Player.GetAnimation())
		{
			Player.SetCurrentTime(anim->GetEndTime() - 0.1f);
		}

		Player.Tick(0.1f);

		Player.Pause(); //현재 포지션의 애니메이션 사운드는 계속 재생되므로, 애니메이션 끝으로 보내준다.
		Player.SetPlaybackStatus(EMovieScenePlayerStatus::Paused);
	}

	Super::OnAnimationStartedPlaying(Player);
}
