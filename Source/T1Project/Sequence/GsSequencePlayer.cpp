// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSequencePlayer.h"
#include "T1Project.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "ActorSequencePlayer.h"
#include "ActorSequenceComponent.h"
#include "ActorEx/GsCharacterBase.h"
#include "Kismet/GameplayStatics.h"
#include "NoExportTypes.h"
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "ActorSequencePlayer.h"

#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLocalizationManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "GsSequenceManager.h"

#include "Input/GsInputEventMsgBase.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "EngineUtils.h"
#include "GsSequenceInfo.h"

#include "GameFramework/Actor.h"
#include "GameFlow/GsGameFlowBase.h"

#include "Cinematic/GsSchemaSequenceResData.h"
#include "Sound/GsSoundMixerController.h"
#include "Sound/GsSoundPlayer.h"

#include "Runtime/LevelSequence/Public/DefaultLevelSequenceInstanceData.h"
#include "UI/UIContent/Tray/GsUITrayFadeEffect.h"
#include "UI/UIContent/Tray/GsUITraySequence.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Sequence/GsSequenceEnum.h"

#include "Engine/Public/TimerManager.h"

#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Net/GsNetSendServiceWorld.h"


#define TEST_SOUND_SYNC

void UGsSequencePlayer::Initialize()
{
	_sequencePlayer	= nullptr;
	_sequenceActor	= nullptr;
	_levelSequence	= nullptr;

	FGsMessageHolder* message = GMessage();
		
	_listSystemParamDelegate.Emplace(
		message->GetSystemParam().AddUObject(MessageSystem::SEQUENCE_PLAYER_START, this, &UGsSequencePlayer::PlaySequence));

	_listSystemParamDelegate.Emplace(
		message->GetSystemParam().AddUObject(MessageSystem::SEQUENCE_PLAYER_STOP, this, &UGsSequencePlayer::StopSequence));

	_listSystemParamDelegate.Emplace(
		message->GetSystemParam().AddUObject(MessageSystem::SEQUENCE_PLAYER_STOP_ONLY_PLAYER, this, &UGsSequencePlayer::StopSequenceOnlyPlayer));

	_listSystemParamDelegate.Emplace(
		message->GetSystemParam().AddUObject(MessageSystem::SEQUENCE_PLAYER_ERROR_STOP, this, &UGsSequencePlayer::ErrorStopSequence));

	_listSystemParamDelegate.Emplace(
		message->GetSystemParam().AddUObject(MessageSystem::SEQUENCE_PLAYER_PAUSE, this, &UGsSequencePlayer::PauseSequence));

	_listSystemParamDelegate.Emplace(
		message->GetSystemParam().AddUObject(MessageSystem::SEQUENCE_PLAYER_PLAY, this, &UGsSequencePlayer::PauseNextPlaySequence));

	_listSystemParamDelegate.Emplace(
		message->GetSystemParam().AddUObject(MessageSystem::SEQUENCE_PLAYER_FORCE_STOP, this, &UGsSequencePlayer::ForceStopSequence));

	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::NET_DISCONECTED, this, &UGsSequencePlayer::NetDisconected));
	
	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::GATEWAY_SERVER_LOGIN_RETRY, this, &UGsSequencePlayer::ReconnectStart));

	_listSystemDelegate.Emplace(
		message->GetSystem().AddUObject(MessageSystem::GAME_SERVER_RECONNECTION_SUCCESS, this, &UGsSequencePlayer::ReconnectEnd));

	ClearShowActorBeforeSequencePlay();
	ClearHideActorBeforeSequencePlay();

	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddUObject(this, &UGsSequencePlayer::OnWillEnterBackground);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddUObject(this, &UGsSequencePlayer::OnHasEnteredForeground);}

void UGsSequencePlayer::Finalize()
{
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.RemoveAll(this);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.RemoveAll(this);

	StopSequence();
	StopActorSequence();	

	ClearShowActorBeforeSequencePlay();
	ClearHideActorBeforeSequencePlay();

	FGsMessageHolder* message = GMessage();

	if (_listSystemParamDelegate.Num() != 0)
	{
		for (auto iter : _listSystemParamDelegate)
		{
			message->GetSystemParam().Remove(iter);
		}
		_listSystemParamDelegate.Empty();
	}

	if (_listSystemDelegate.Num() != 0)
	{
		for (auto iter : _listSystemDelegate)
		{
			message->GetSystem().Remove(iter);
		}
		_listSystemDelegate.Empty();
	}

	if (_arrayStoredLevelSequence.Num() != 0)
	{
		_arrayStoredLevelSequence.Empty();
		_mapStoredLevelSequnce.Empty();
	}
}

void UGsSequencePlayer::StopSound()
{
	if (UGsSoundManager* soundManager = GSound())
	{
		if (UGsSoundPlayer* soundPlayer = soundManager->GetOrCreateSoundPlayer())
		{
			if (0 < _audioId)
			{
				soundPlayer->StopSound(_audioId);
				_audioId = 0;
			}
		}

		if (UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController())
		{
			if (_sequenceResData && _isSoundMixChanged)
			{				
				soundMixerController->OffMixMode(_sequenceResData->soundMix);
				_isSoundMixChanged = false;				
			}
		}
	}
}

void UGsSequencePlayer::PlaySequence(const IGsMessageParam* inParam)
{
	const FGsSequenceMessageParam* sequenceParam = inParam->Cast<const FGsSequenceMessageParam>();
	if (nullptr == sequenceParam)
		return;

	StopSound();
	
	_messageParam.SetData(sequenceParam);

	// ID 저장, 끝날때 다시 돌려준다.
	_sequenceId = sequenceParam->_sequenceId;

	GSLOG(Log, TEXT("GsSequencePlayer::PlaySequence() _sequenceResId : %d"), _sequenceId);

	_sequenceResData = GetSequenceResData(_sequenceId);
	if (nullptr == _sequenceResData)
		return;

	_isUIHide = sequenceParam->_isUIHide;

	// clear
	_isClickSkip = false;

	ULevelSequence* resObject = nullptr;
		
	if (sequenceParam->_isStore == true)
	{
		// 기존에 로드 되었는지 체크
		resObject =
			GetStoredRes(_sequenceId, _sequenceResData->resObjectPath);
	}
	else
	{
		// 그냥 로드
		resObject =
			GetSequenceRes(_sequenceResData->resObjectPath);
	}
	if(nullptr == resObject)
		return;

	// 입력 막는 메시지 보낸다
	//FGsInputEventMsgBase msg;
	//msg.Id = 1;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_CINEMATIC, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	PlaySequence(resObject);

	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}

	// 스킵 버튼 출력할지
	if (_sequenceResData->isSkip == true ||
		_sequenceResData->sequenceTextType != EGsSequenceTextType::SEQUENCE_TEXT_TYPE_NONE)
	{
		_isDirtySkipOn = true;
	}

	if (_isUIHide)
	{
		uiMgr->SetHideFlags(EGsUIHideFlags::STATE_CINEMATIC);
	}
}

// 스퀸서 애셋을 플레이 시킨다.
// 애셋 타입
void UGsSequencePlayer::PlaySequence(ULevelSequence* inLevelSequence)
{
	if (nullptr == inLevelSequence)
		return;

	// 플레이어 생성
	if (nullptr == _sequencePlayer)
	{
		FMovieSceneSequencePlaybackSettings playbackSettings;
		playbackSettings.bHideHud = true;
		playbackSettings.bDisableMovementInput = true;

		_sequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(this,
			inLevelSequence, playbackSettings, _sequenceActor);

		if (nullptr == _sequencePlayer) return;

		_sequencePlayer->OnPlay.AddDynamic(this, &UGsSequencePlayer::OnPlayLevelSequence);
		_sequencePlayer->OnFinished.AddDynamic(this, &UGsSequencePlayer::OnFinishLevelSequence);
	}	
	
	// 다른 Aasset이 들어온 경우
	if (_levelSequence != inLevelSequence)
	{
		if (true == IsPlaying()) { StopSequence(); }

		_levelSequence = inLevelSequence;
		if (nullptr != _sequenceActor)
		{
			_sequenceActor->SetSequence(_levelSequence);
			_sequenceActor->InitializePlayer();

		}
	}

	// 기존 시퀀스랑 같아도 위치 값은 최신으로 바꿔야 함
	if (nullptr != _sequenceActor)
	{
		if (_messageParam._posType != EGsSequnceParamPosType::None)
		{
			_sequenceActor->bOverrideInstanceData = true;
		}
		else
		{
			// 기본은 꺼져 있다
			_sequenceActor->bOverrideInstanceData = false;
		}
		if (_sequenceActor->DefaultInstanceData != nullptr)
		{
			UDefaultLevelSequenceInstanceData* data =
				Cast<UDefaultLevelSequenceInstanceData>(_sequenceActor->DefaultInstanceData);
			if (data != nullptr)
			{
				if (_messageParam._posType != EGsSequnceParamPosType::None)
				{
					if (_messageParam._posType == EGsSequnceParamPosType::Actor)
					{
						data->TransformOrigin = _messageParam._transformOriginActor->GetTransform();
					}
					else
					{
						data->TransformOrigin = _messageParam._transformOrigin;
					}
				}
				// tr도 기본으로 초기화
				else
				{
					data->TransformOrigin = FTransform::Identity;
				}
			}
		}
	}


	if (nullptr != _sequencePlayer)
	{
		// 연출 시작
		UpdateVisible(GetWorld());

		_sequencePlayer->SetPlaybackPosition(FMovieSceneSequencePlaybackParams(0, EUpdatePositionMethod::Jump));
		_sequencePlayer->Play();
	}
}

// 스퀸서 애셋을 플레이를 끝낸다.
void UGsSequencePlayer::StopSequence()
{
	if (nullptr == _sequencePlayer)
		return;

	// 연출 종료
	_sequencePlayer->GoToEndAndStop();
}

void UGsSequencePlayer::StopSequence(const IGsMessageParam* inParam)
{
	_isClickSkip = true;
	StopSequence();
	OnFinishLevelSequence();
}

void UGsSequencePlayer::PauseSequence(const IGsMessageParam* inParam)
{
	if (nullptr != _sequencePlayer)
	{
		if (_sequencePlayer->IsPlaying())
		{
			_sequencePlayer->Pause();

			if (UGsSoundManager* soundManager = GSound())
			{
				if (UGsSoundPlayer* soundPlayer = soundManager->GetOrCreateSoundPlayer())
				{
					if (0 < _audioId)
					{
						soundPlayer->PauseSound(true, _audioId);
					}
				}
			}
		}
	}
}

void UGsSequencePlayer::PauseNextPlaySequence(const IGsMessageParam* inParam)
{
	if (nullptr != _sequencePlayer)
	{
		if (_sequencePlayer->IsPaused())
		{
			_sequencePlayer->Play();

			if (UGsSoundManager* soundManager = GSound())
			{
				if (UGsSoundPlayer* soundPlayer = soundManager->GetOrCreateSoundPlayer())
				{
					if (0 < _audioId)
					{
						soundPlayer->PauseSound(false, _audioId);
					}
				}
			}
		}
	}
}

// 즉시 강제 종료
void UGsSequencePlayer::ForceStopSequence(const IGsMessageParam* inParam)
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}

	if (nullptr == _sequenceResData)
		return;

	StopSequence();
	StopSound();

	UGsUIManager* uiMgr = GUI();
	if (uiMgr != nullptr)
	{
		if (_isUIHide)
			uiMgr->ClearHideFlags(EGsUIHideFlags::STATE_CINEMATIC);

		// 스킵 버튼 출력했으면 닫기
		if (_sequenceResData->isSkip == true ||
			_sequenceResData->sequenceTextType != EGsSequenceTextType::SEQUENCE_TEXT_TYPE_NONE)
		{
			// 시퀀스 스킵 닫기
			uiMgr->CloseByKeyName(TEXT("TraySequence"));
		}		
	}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_SEPARATE_LIGHT_SOURCE_END, nullptr);

	//Sequence play 종료
	UpdateVisible(GetWorld(), true);

	FGsMessageHolder* message = GMessage();
	// 인풋 막기 해제(줌, 회전)
	//FGsInputEventMsgBase msg;
	//msg.Id = 0;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_CINEMATIC, false);
	message->GetInput().SendMessage(MessageInput::BlockInput, msg);	
}

// 액터에 포함 된 시퀀스를 플레이할때 사용한다. (애셋이 존재하지 않는다.)
void UGsSequencePlayer::PlayActorSequence(AActor * inOwnerActor, const FString& inActorSequenceName)
{
	if (nullptr == inOwnerActor)
		return;

	if (inActorSequenceName.IsEmpty())
		return;

	// 액터에서 시퀀서를 찾는다.
	TArray<UActorSequenceComponent*> children;
	inOwnerActor->GetComponents<UActorSequenceComponent>(children);

	for (UActorSequenceComponent* iter : children)
	{
		// 컨포넌트 이름 비교로 찾는다.
		UActorSequenceComponent* child = iter;
		if (nullptr == child)
			continue;
		
		if (child->GetName() != inActorSequenceName)
			continue;
		
		_actorSequencePlayer = child->GetSequencePlayer();
		if (nullptr == _actorSequencePlayer)
			continue;

		_actorSequencePlayer->Play();
	}
}

void UGsSequencePlayer::StopActorSequence()
{
	if (nullptr == _actorSequencePlayer)
		return;

	_actorSequencePlayer->Stop();
}

void UGsSequencePlayer::UpdateVisible(UWorld* inWorld, bool inFinish)
{
	if (nullptr == inWorld)
	{
		return;
	}		

	//시퀀스 플레이어 스타트
	if (false == inFinish)
	{	
		if (nullptr == _sequenceResData)
		{
			GSLOG(Error, TEXT("%s"), TEXT("sequence res data is missing"));
			return;
		}

		const UGsGlobalConstant* globalData = GData()->GetGlobalData();
		if (nullptr == globalData)
		{
			GSLOG(Log, TEXT("%s"), TEXT("Global data is missing"));
			return;
		}

		ClearShowActorBeforeSequencePlay();
		ClearHideActorBeforeSequencePlay();

		const TArray<FName>& tagList = globalData->_hideTagList;
		AActor* actor = nullptr;
		bool isHidden;
		bool isHide = false;
		for (FActorIterator It(inWorld); It; ++It)
		{
			actor = *It;
			if (nullptr == actor)
			{
				continue;
			}				

			if (actor->IsPendingKill())
			{
				continue;
			}		
			
			isHide = false;

			for (const FName& tagName : tagList)
			{
				if (actor->ActorHasTag(tagName))
				{
					isHidden = actor->IsHidden();
					if (isHidden)
					{
						_hideActorBeforeSequencePlayArray.Add(actor);
					}
					else
					{
						_showActorBeforeSequencePlayArray.Add(actor);
					}

					actor->SetActorHiddenInGame(true); // 끈다				
					isHide = true;
					break;
				}
			}

			//GlobalConstant 데이터에 체크가 안되면 ResData에서 두번째로 확인한다
			if (false == isHide)
			{
				for (const FName& tagName : _sequenceResData->hideTagList)
				{
					if (actor->ActorHasTag(tagName))
					{
						isHidden = actor->IsHidden();
						if (isHidden)
						{
							_hideActorBeforeSequencePlayArray.Add(actor);
						}
						else
						{
							_showActorBeforeSequencePlayArray.Add(actor);
						}

						actor->SetActorHiddenInGame(true); // 끈다				

						break;
					}
				}
			}			
		}
	}
	else
	{			
		//시네마틱 이전에 숨김 처리된 액터라면 다시 안보이게
		for (AActor* actor : _showActorBeforeSequencePlayArray)
		{
			if (nullptr == actor)
				continue;

			actor->SetActorHiddenInGame(false);
		}

		//시네마틱 이전에 보이게 처리된 액터라면 다시 보이게
		for (AActor* actor : _hideActorBeforeSequencePlayArray)
		{
			if (nullptr == actor)
				continue;

			actor->SetActorHiddenInGame(true);
		}		
	}
}
	
void UGsSequencePlayer::ClearShowActorBeforeSequencePlay()
{
	_showActorBeforeSequencePlayArray.Empty();
}

void UGsSequencePlayer::ClearHideActorBeforeSequencePlay()
{
	_hideActorBeforeSequencePlayArray.Empty();
}

void UGsSequencePlayer::OnPlayLevelSequence()
{
	if (0 == _sequenceId)
	{
		return;
	}

	// 믹서를 시네마틱 상태로 변경(실제 사운드 재생은 키 이벤트를 받아서 진행)
	if (UGsSoundManager* soundManager = GSound())
	{
#ifndef TEST_SOUND_SYNC
		//if (UGsSoundPlayer* SoundPlayer = soundManager->GetOrCreateSoundPlayer())
		//{
		//	// 내 캐릭터가 여자인지 검사 
		//	bool bIsFemail = false;
		//	if (FGsGameDataManager* gameDataMgr = GGameData())
		//	{
		//		if (const FGsNetUserData* userData = gameDataMgr->GetUserData())
		//		{
		//			bIsFemail = (userData->mGender == CreatureGenderType::FEMALE) ? true : false;
		//		}
		//	}

		//	FString strFileName = LexToString(_sequenceId);
		//	if (USoundBase* sound = GLocalization()->GetCinematicSound(strFileName, bIsFemail))
		//	{
		//		_audioId = SoundPlayer->PlaySoundObject(sound);				
		//	}
		//}

		//if (UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController())
		//{
		//	if (_sequenceResData)
		//	{
		//		soundMixerController->OnMixChanged(_sequenceResData->soundMix);
		//	}			
		//}
#endif
	}
}

void UGsSequencePlayer::OnFinishLevelSequence()
{
	StopSound();

	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}

	UpdateVisible(world, true);


	if (nullptr == _sequenceResData)
	{
		return;
	}

	UGsUIManager* uiMgr = GUI();
	if (uiMgr != nullptr)
	{		
		// 스킵 버튼 출력했으면 닫기
		if (_sequenceResData->isSkip == true ||
			_sequenceResData->sequenceTextType != EGsSequenceTextType::SEQUENCE_TEXT_TYPE_NONE)
		{
			// 시퀀스 스킵 닫기
			uiMgr->CloseByKeyName(TEXT("TraySequence"));
		}	
	}
	if (_sequenceResData->dimmedWaitTime == 0.0f)
	{
		PostSequenceEnd();
	}
	else
	{
		FGsUIHelper::TrayDimmed(true);

		world->GetTimerManager().SetTimer(_timeHandleDimmedOn, 
			FTimerDelegate::CreateLambda([this]() {
				DimmedOff();
			}), _sequenceResData->dimmedWaitTime, false);
	}
}

// 시퀀스가 플레이 중인가?
bool UGsSequencePlayer::IsPlaying() const
{
	return (nullptr != _sequencePlayer) ? _sequencePlayer->IsPlaying() : false;
}

// 시퀀스가 중지 상태인가?
bool UGsSequencePlayer::IsPaused() const
{
	return (nullptr != _sequencePlayer) ? _sequencePlayer->IsPaused() : false;
}

bool UGsSequencePlayer::IsGameFlowTypeGame()  const
{
	if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
	{
		return true;
	}
	return false;
}

// 사운드 애셋을 얻어온다.
ULevelSequence* UGsSequencePlayer::GetSequenceRes(const FSoftObjectPath inSequenceResPath) const
{
	ULevelSequence* load = GResource()->LoadSync<ULevelSequence>(inSequenceResPath);
	if (nullptr == load)
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("[SequencePlayer] GetSequenceRes : %s - load == nullptr"), *inSequenceResPath.ToString());
#endif // WITH_EDITOR
		return nullptr;
	}

	return load;
}

const FGsSchemaSequenceResData* UGsSequencePlayer::GetSequenceResData(int32 inSequenceId) const
{
	const UGsTableSequenceResData* table = Cast<UGsTableSequenceResData>(FGsSchemaSequenceResData::GetStaticTable());
	if (nullptr == table)
		return nullptr;

	const FGsSchemaSequenceResData* schemaSequenceResData = nullptr;
	if (false == table->FindRowById(inSequenceId, schemaSequenceResData))
		return nullptr;

	return schemaSequenceResData;
}
// 저장된거 가져오기
// 없으면 로드
ULevelSequence* UGsSequencePlayer::GetStoredRes(int32 In_sequenceId, const FSoftObjectPath inSequenceResPath)
{
	ULevelSequence* storedLevel =
		_mapStoredLevelSequnce.FindRef(In_sequenceId);

	if (storedLevel == nullptr)
	{
		storedLevel =
			GetSequenceRes(inSequenceResPath);

		if (storedLevel == nullptr)
		{
			return nullptr;
		}

		_mapStoredLevelSequnce.Add(In_sequenceId, storedLevel);
		_arrayStoredLevelSequence.Add(storedLevel);
	}

	return storedLevel;
}

void UGsSequencePlayer::StartSound()
{
	if (0 == _sequenceId)
	{
		return;
	}

#ifdef TEST_SOUND_SYNC
	if (UGsSoundManager* soundManager = GSound())
	{
		if (UGsSoundPlayer* SoundPlayer = soundManager->GetOrCreateSoundPlayer())
		{
			// 내 캐릭터가 여자인지 검사 
			bool bIsFemail = false;
			if (FGsGameDataManager* gameDataMgr = GGameData())
			{
				if (const FGsNetUserData* userData = gameDataMgr->GetUserData())
				{
					bIsFemail = (userData->mGender == CreatureGenderType::FEMALE) ? true : false;
				}
			}

			FString strFileName = LexToString(_sequenceId);
			if (USoundBase* sound = GLocalization()->GetCinematicSound(strFileName, bIsFemail))
			{
				_audioId = SoundPlayer->PlaySoundObject(sound);
			}
		}

		if (UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController())
		{
			if (_sequenceResData)
			{
				soundMixerController->OnMixChanged(_sequenceResData->soundMix);
				_isSoundMixChanged = true;
			}
		}
	}
#endif
}


void UGsSequencePlayer::PostSequenceEnd()
{
	UGsUIManager* uiMgr = GUI();
	if (uiMgr != nullptr)
	{
		if (_isUIHide)
			uiMgr->ClearHideFlags(EGsUIHideFlags::STATE_CINEMATIC);

	}

	FGsMessageHolder* message = GMessage();

	FGsSequenceMessageParam param;
	param._sequenceId = _sequenceId;
	param._isSkip = _isClickSkip;
	message->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_END, &param);

	GMessage()->GetGameObject().SendMessage(MessageGameObject::ENV_SEPARATE_LIGHT_SOURCE_END, nullptr);

	// 인풋 막기 해제(줌, 회전)
	//FGsInputEventMsgBase msg;
	//msg.Id = 0;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_CINEMATIC, false);
	message->GetInput().SendMessage(MessageInput::BlockInput, msg);
	
}

void UGsSequencePlayer::DimmedOff()
{
	PostSequenceEnd();

	if (_timeHandleDimmedOn.IsValid() == true)
	{
		UWorld* world = GetWorld();
		if (world == nullptr)
		{
			return;
		}

		// 타이머 클리어
		world->GetTimerManager().ClearTimer(_timeHandleDimmedOn);
		_timeHandleDimmedOn.Invalidate();
	}

	if (nullptr == _sequenceResData)
	{
		return;
	}

	if (_sequenceResData->isCallDimmedOff == true)
	{
		FGsUIHelper::TrayDimmed(false);
	}
}

void UGsSequencePlayer::OnWillEnterBackground()
{
	GSLOG(Log, TEXT("UGsSequencePlayer::OnWillEnterBackground()"));
	if (IsPlaying())
	{
		PauseSequence(nullptr);
	}
}

void UGsSequencePlayer::OnHasEnteredForeground()
{
	GSLOG(Log, TEXT("UGsSequencePlayer::OnHasEnteredForeground()"));
	if (IsPaused())
	{
		PauseNextPlaySequence(nullptr);
	}
}

void UGsSequencePlayer::NetDisconected()
{
	GSLOG(Log, TEXT("UGsSequencePlayer::NetDisconected()"));
	/*if (IsPlaying() && !IsPaused())
	{
		PauseSequence(nullptr);
	}*/
}

void UGsSequencePlayer::ReconnectStart()
{
	GSLOG(Log, TEXT("UGsSequencePlayer::ReconnectStart()"));
	/*if (IsPlaying() && !IsPaused())
	{
		PauseSequence(nullptr);
	}*/
}

void UGsSequencePlayer::ReconnectEnd()
{
	GSLOG(Log, TEXT("UGsSequencePlayer::ReconnectEnd()"));
	/*if (IsPaused())
	{
		PauseNextPlaySequence(nullptr);
	}*/
}
// 시퀀스 플레이어만 종료(메시지 send, 나머지 정리 빼고)
void UGsSequencePlayer::StopSequenceOnlyPlayer(const IGsMessageParam* inParam)
{
	GSLOG(Error, TEXT("%s"), TEXT("UGsSequencePlayer::StopSequenceOnlyPlayer call"));

	ForceStopSequence(nullptr);

	// 서버로 시네마틱을 멈춘다고 보낸다.
	FGsNetSendServiceWorld::SendCinematicEndByEventAction(_sequenceId);
}

void UGsSequencePlayer::ErrorStopSequence(const IGsMessageParam* inParam)
{
	GSLOG(Error, TEXT("%s"), TEXT("UGsSequencePlayer::ErrorStopSequence call"));

	ForceStopSequence(nullptr);
}

bool UGsSequencePlayer::IsSequencePlaying()
{
	bool isPlaying = false;
	if (UGsSequencePlayer* sequencePlayer = GSequencePlayer())
	{
		isPlaying = sequencePlayer->IsPlaying();
	}

	return isPlaying;
}

void UGsSequencePlayer::Update(float In_delta)
{
	if (_isDirtySkipOn == false)
	{
		return;
	}

	_isDirtySkipOn = false;

	UGsUIManager* uiMgr = GUI();
	if (uiMgr != nullptr)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiMgr->OpenAndGetWidget(TEXT("TraySequence"));
		if (widget.IsValid())
		{
			TWeakObjectPtr<UGsUITraySequence> traySequence = Cast<UGsUITraySequence>(widget);
			if (traySequence.IsValid())
			{
				traySequence->SkipButtonOnOff(_sequenceResData->isSkip);
				traySequence->PlaySequenceEffcet(_sequenceResData->sequenceTextType, _sequenceResData->titleText, _sequenceResData->descText);
			}
		}
	}
}