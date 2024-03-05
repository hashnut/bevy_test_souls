// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Tray/GsUITraySequence.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Sequence/GsSequenceManager.h"
#include "Sequence/GsSequencePlayer.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageSystem.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Sequence/GsUISequenceTextEffect.h"
#include "Sequence/GsSequenceEnum.h"
#include "Option/GsServerOption.h"

void UGsUITraySequence::NativeOnInitialized()
{
	_btnSkip->OnClicked.AddDynamic(this, &UGsUITraySequence::OnClickSkip);

	Super::NativeOnInitialized();
}

void UGsUITraySequence::OnClickSkip()
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

void UGsUITraySequence::PlaySequenceEffcet(EGsSequenceTextType In_TextType, const FText In_Text1, const FText In_Text2)
{	
	if (_textEffecter)
	{
		_textEffecter->PlayEffect(In_Text1, In_Text2);
	}
}

void UGsUITraySequence::SkipButtonOnOff(bool In_isSkip)
{
	if (_btnSkip)
	{
		_btnSkip->SetVisibility((In_isSkip) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

