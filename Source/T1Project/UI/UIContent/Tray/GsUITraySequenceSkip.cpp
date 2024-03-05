// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITraySequenceSkip.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Sequence/GsSequenceManager.h"
#include "Sequence/GsSequencePlayer.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageSystem.h"
#include "Option/GsServerOption.h"

void UGsUITraySequenceSkip::NativeOnInitialized()
{
	_btnSkip->OnClicked.AddDynamic(this, &UGsUITraySequenceSkip::OnClickSkip);

	Super::NativeOnInitialized();
}

void UGsUITraySequenceSkip::OnClickSkip()
{
	bool isWatchMovieRewarded = GSequence()->GetIsWatchMovieReward();
	if (isWatchMovieRewarded)
	{
		GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_STOP, nullptr);
		Close();
	}
	else
	{
		if (false == GServerOption()->GetIsIgnoreCinematicRewardPopup())
		{
			GSequence()->OpenPopupWatchMovieReward([this](bool bInIsOk)
				{
					if (bInIsOk)
					{
						GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_STOP, nullptr);
						Close();
					}
				});
		}
		else
		{
			GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_STOP, nullptr);
			Close();
		}
	}
}
