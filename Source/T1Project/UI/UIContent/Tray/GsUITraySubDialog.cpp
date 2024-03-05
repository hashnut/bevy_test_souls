// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITraySubDialog.h"

#include "SubDialog/GsSchemaSubDialogCutData.h"
#include "SubDialog/GsSchemaSubDialogActorData.h"

#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"

#include "Sound/GsSoundPlayer.h"

#include "RichTextBlock.h"
#include "UTIL/GsGameObjectUtil.h"

#include "Classes/GsSpawn.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"


void UGsUITraySubDialog::NativeDestruct()
{
	if (UGsSoundPlayer* SoundPlayer = GSound()->GetOrCreateSoundPlayer())
	{
		if (SoundPlayer->IsPlaying(_voiceAudioId))
		{
			SoundPlayer->StopSound(_voiceAudioId);
		}
	}

	// 사운드 믹서 변경
	if (UGsSoundManager* soundManager = GSound())
	{
		if (UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController())
		{
			soundMixerController->OffMixMode(EGsSoundMixType::SubDialog);
		}
	}

	_remainTime = 0.0f;
	_voiceAudioId = 0;
	DestroyActor();

	Super::NativeDestruct();
}

void UGsUITraySubDialog::NativeConstruct()
{
	Super::NativeConstruct();

	// 사운드 믹서 변경
	if (UGsSoundManager* soundManager = GSound())
	{
		if (UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController())
		{
			soundMixerController->OnMixChanged(EGsSoundMixType::SubDialog);
		}
	}
}

void UGsUITraySubDialog::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (0 < _remainTime)
	{
		_remainTime -= InDeltaTime;
		if (0 >= _remainTime)
		{
			_remainTime = 0.0f;
			OnEndPlayTime();
		}
	}

	Super::NativeTick(MyGeometry, InDeltaTime);
}

FReply UGsUITraySubDialog::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUITraySubDialog::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUITraySubDialog::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

void UGsUITraySubDialog::OnEndPlayTime()
{
	OnStopVoice();
	// 끝날 때 꼭 액터 삭제
	DestroyActor();
	// 콜백 호출
	if (_delegateRemainTimeOver.IsBound())
	{
		_delegateRemainTimeOver.ExecuteIfBound();
	}
}

bool UGsUITraySubDialog::IsPlaying() const
{
	return (0 < _remainTime) ? true : false;
}

void UGsUITraySubDialog::StartAction(const FGsSchemaSubDialogCutData* inCutData, bool inIsVoicePlayable)
{
	// 시작 할 때 앞전 액터가 남아 있으면 삭제
	DestroyActor();

	if (nullptr != inCutData)
	{
		_nameText = inCutData->name;

		const FGsNetUserData* userData = GGameData()->GetUserData();
		if (nullptr == userData)
		{
			CloseInternal();
			return;
		}

		FText textScrip = inCutData->script;
		FString strScript = inCutData->script.ToString();
		if (strScript.Contains(TEXT("{0}")))
		{
			textScrip = FText::Format(inCutData->script, FText::FromString(userData->mUserName));
		}

		_richTextScript->SetText(textScrip);
		
		FGsSchemaSubDialogActorDataRow actorDataRow = inCutData->actor;	
		if (const FGsSchemaSubDialogActorData* actorData = actorDataRow.GetRow())
		{			
			if (UClass* uClass = GResource()->LoadSync<UClass>(actorData->actorPath, true))
			{
				if (const UGsGlobalConstant* data = GData()->GetGlobalData())
				{
					_actor = GsSpawn::BPClass(GetWorld(), uClass, data->_subActorSpawnPosOffset, FRotator::ZeroRotator);
					
				}
			}						
		}
		_remainTime = inCutData->remainTime;

		if (inIsVoicePlayable)
		{
			if (UGsSoundPlayer* SoundPlayer = GSound()->GetOrCreateSoundPlayer())
			{
				FString strFileName = LexToString(inCutData->Id);
				if (USoundBase* sound = GLocalization()->GetSubDialogueSound(strFileName, false))
				{
					_voiceAudioId = SoundPlayer->PlaySoundObject(sound);
#if	WITH_EDITOR
					const FString voiceAudioString = FString::Printf(
						TEXT("[SoundMixer] PlaySound voiceAudioId : %llu, strFileName : %s"), _voiceAudioId, *strFileName);
					//FVector2D textScale{ 1.5f, 1.5f };
					//GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Purple, voiceAudioString, true, textScale);

					GSLOG(Log, TEXT("%s"), *voiceAudioString);
#endif
				}
			}
		}
	}
}

void UGsUITraySubDialog::DestroyActor()
{
	if (nullptr != _actor)
	{
		GetWorld()->DestroyActor(_actor);
		_actor = nullptr;
	}
}

void UGsUITraySubDialog::OnStopVoice()
{
	if (0 < _voiceAudioId)
	{
		if (UGsSoundPlayer* SoundPlayer = GSound()->GetOrCreateSoundPlayer())
		{
			if (SoundPlayer->IsPlaying(_voiceAudioId))
			{	
				SoundPlayer->StopSound(_voiceAudioId);
			}			
		}
	}	

	_voiceAudioId = 0;
}