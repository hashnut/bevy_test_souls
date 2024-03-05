#include "GsSequenceManager.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequence/Public/LevelSequenceActor.h"
#include "ActorSequence/Public/ActorSequenceComponent.h"
#include "ActorSequence/Public/ActorSequencePlayer.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ActorEx/GsCharacterBase.h"
#include "Object.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "UI/UIContent/Popup/GsUIPopupWatchMovieReward.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "Reward/GsRewardHelper.h"
#include "Currency/GsCurrencyHelper.h"
#include "Management/GsMessageHolder.h"


void UGsSequenceManager::BeginDestroy()
{
	Super::BeginDestroy();

	_sequencePlayer = nullptr;
}

void UGsSequenceManager::Initialize()
{
	FGsMessageHolder* message = GMessage();

	_listSystemParamDelegate.Emplace(
		message->GetSystemParam().AddUObject(MessageSystem::SEQUENCE_PLAYER_START, this, &UGsSequenceManager::PlaySequence));

	if (nullptr == _sequencePlayer)
	{
		_sequencePlayer = NewObject<UGsSequencePlayer>(this);
		_sequencePlayer->Initialize();
	}
}

void UGsSequenceManager::Finalize()
{
	FGsMessageHolder* message = GMessage();

	if (_listSystemParamDelegate.Num() != 0)
	{
		for (auto iter : _listSystemParamDelegate)
		{
			message->GetSystemParam().Remove(iter);
		}
		_listSystemParamDelegate.Empty();
	}

	if (nullptr != _sequencePlayer)
	{
		_sequencePlayer->ConditionalBeginDestroy();
	}
}

UGsSequencePlayer* UGsSequenceManager::GetSequencePlayer()
{
	return _sequencePlayer;
}

void UGsSequenceManager::PlaySequence(const IGsMessageParam* inParam)
{
	ClearWatchMovieReward();
}

void UGsSequenceManager::ClearWatchMovieReward()
{
	_playingSequenceResId = 0;
	_isWatchMovieRewarded = false;
}

bool UGsSequenceManager::GetIsWatchMovieReward()
{
	return _isWatchMovieRewarded;
}

void UGsSequenceManager::SetCinematicStart(int32 inSequenceResId, bool inIsAalreadyRewarded)
{
	_playingSequenceResId = inSequenceResId;
	_isWatchMovieRewarded = inIsAalreadyRewarded;
}

void UGsSequenceManager::SetCinematicEnd(int32 inSequenceResId, bool inIsGetReward)
{
	if (_playingSequenceResId == inSequenceResId)
	{
		if (true == inIsGetReward)
		{			
			ShowRewardTicker(inSequenceResId);
		}
	}

	_playingSequenceResId = 0;
	_isWatchMovieRewarded = false;

	if (GUI()->IsActiveWidget(TEXT("PopupWatchMovieReward")))
	{
		GUI()->CloseByKeyName(TEXT("PopupWatchMovieReward"));
	}
}

void UGsSequenceManager::OpenPopupWatchMovieReward(TFunction<void(bool)> inCallback)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupWatchMovieReward"));
	if (widget.IsValid())
	{
		if (UGsUIPopupWatchMovieReward* popup = Cast<UGsUIPopupWatchMovieReward>(widget.Get()))
		{
			popup->SetData(_playingSequenceResId, inCallback);
		}
	}
}

void UGsSequenceManager::ShowRewardTicker(int32 inSequenceResId)
{
	FText findText;
	FText::FindText(TEXT("SystemChatText"), TEXT("SystemChat_CinematicViewReward"), findText);
	if (findText.IsEmpty())
		return;

	const FGsSchemaSequenceResData* resData = GSequencePlayer()->GetSequenceResData(inSequenceResId);
	if (nullptr == resData)
	{
		return;
	}

	const FGsSchemaRewardData* rewardData = resData->viewRewardId.GetRow();
	if (nullptr == rewardData)
	{		
		return;
	}

	FGsRewardUnboxingData data;
	FGsRewardHelper::UnboxingRewardData(rewardData->id, data);

	if (0 < data.GetCommonExp())
	{
		// °æÇèÄ¡
		FText findTextExp;
		FText::FindText(TEXT("ResurrectionText"), TEXT("Recovery_EXP"), findTextExp);
		if (!findTextExp.IsEmpty())
		{
			FText txt;
			FGsUIStringHelper::GetAmountText(data.GetCommonExp(), MAX_uint64, txt);
			if (!txt.IsEmpty())
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FText::Format(findText, findTextExp, txt));
			}
		}		
	}
	else
	{
		FText textAmount;
		FGsUIStringHelper::GetAmountText(data.GetCommonCurrencyAmount(), 0, textAmount);
		if (!textAmount.IsEmpty())
		{
			FText textCurrencyName = FGsCurrencyHelper::GetCurrencyLocalizedText(data._currencyType);
			if (!textCurrencyName.IsEmpty())
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, FText::Format(findText, textAmount, textCurrencyName));
			}
		}
	}
}
void UGsSequenceManager::Update(float In_delta)
{
	if (_sequencePlayer != nullptr)
	{
		_sequencePlayer->Update(In_delta);
	}
}