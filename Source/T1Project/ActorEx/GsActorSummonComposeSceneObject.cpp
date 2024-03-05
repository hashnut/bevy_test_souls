#include "GsActorSummonComposeSceneObject.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsSummonManager.h"

#include "Message/GsMessageSystem.h"

#include "ActorEx/GsActorSummonComposeCeline.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsGameClientVersion.h"

#include "DataCenter/Public/DataSchema/GsSchemaEnums.h"

#include "Sound/GsSoundPlayer.h"
#include "Sound/GsSoundMixerController.h"

#include "UI/UIContent/Tray/GsUITraySummonSkip.h"

#include "GameFramework/PlayerController.h"

#include "Runtime/Engine/Classes/Camera/CameraComponent.h"

AGsActorSummonComposeSceneObject::AGsActorSummonComposeSceneObject() :Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AGsActorSummonComposeSceneObject::Initialize()
{
	// 드래그 인풋 등록
	InputComponent = NewObject<UInputComponent>(this);

	if (InputComponent)
	{
		if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
		{
			if (nullptr == _sceneCamera)
			{
				_sceneCamera = FindComponentByClass<UCameraComponent>();
			}

			// 카메라 초기화
			GLevel()->PushViewTarget(_sceneCamera->GetOwner());

			InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AGsActorSummonComposeSceneObject::OnTouchMove);
			InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGsActorSummonComposeSceneObject::OnTouchPress);
			InputComponent->BindTouch(EInputEvent::IE_Released, this, &AGsActorSummonComposeSceneObject::OnTouchRelease);

			EnableInput(controller);
		}
	}

	SpawnCeline();

	ChangeProgress(0.0f, 0.0f);

	_isDragInteractionFinish = false;

	// bgm 틀기
	ChangeSoundSummonMixer(true);
}

void AGsActorSummonComposeSceneObject::OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location)
{
	//https://jira.com2us.com/jira/browse/C2URWQ-4085
	if (_isDragInteractionFinish == true)
	{
		return;
	}

	// 1번 아니면 안함
	if (ETouchIndex::Touch1 != FingerIndex)
	{
		return;
	}

	_isTouchPress = true;
	_prevTouchLoc = Location;
	_pressTouchLoc = Location;
}
void AGsActorSummonComposeSceneObject::OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location)
{
	// https://jira.com2us.com/jira/browse/C2URWQ-4085
	if (_isDragInteractionFinish == true)
	{
		return;
	}

	// 1번 아니면 안함
	if (ETouchIndex::Touch1 != FingerIndex)
	{
		return;
	}

	_isTouchPress = false;
	_prevTouchLoc = FVector::ZeroVector;
	_pressTouchLoc = FVector::ZeroVector;

	float dragReleaseCheckVal =
		GData()->GetGlobalData()->_summonStoneDragReleaseSuccessCheckVal;

	// 릴리즈 시점에 0.7보다 크면 성공으로 간주
	if (_dragProgressVal >= dragReleaseCheckVal)
	{
		_isDragComplete = true;
	}

	EndDragSound();
}
void AGsActorSummonComposeSceneObject::OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location)
{
	// https://jira.com2us.com/jira/browse/C2URWQ-4085
	if (_isDragInteractionFinish == true)
	{
		return;
	}

	// 1번 아니면 안함
	if (ETouchIndex::Touch1 != FingerIndex)
	{
		return;
	}


	//// 처음 터치한 위치 기준(드래그 중간에 좌우 다되게)
	//float diffY = Location.Y - _pressTouchLoc.Y;

	//// 0 보다 작으면 위쪽 방향이 순방향
	//if (diffY < 0.0f)
	//{
	//	_isUpDrag = true;
	//}
	//else
	//{
	//	_isUpDrag = false;
	//}

	// 이전과 차이값
	FVector dir = Location - _prevTouchLoc;

	_prevTouchLoc = Location;

	float yDiffVal = dir.Y * GData()->GetGlobalData()->_summonDragMulVal;

	// 0보다 크면 위쪽 방향
	bool isUpDragNow = (yDiffVal < 0.0f) ? true : false;
	// 절대값
	float absDiffVal = FMath::Abs(yDiffVal);
	// 이전값과 비교해서 위로 들어올리면 양수, 아니면 음수
	float  addVal = (isUpDragNow == true) ? absDiffVal : -absDiffVal;

	// 0에서 1사이만 저장
	float newDragVal = FMath::Clamp(_dragProgressVal + addVal, 0.0f, 1.0f);

	if (_dragProgressVal == newDragVal)
	{
		return;
	}

	float oldVal = _dragProgressVal;
	_dragProgressVal = newDragVal;

	ChangeProgress(_dragProgressVal, oldVal);

	// 끝가지 도달하면
	if (_dragProgressVal == 1.0f)
	{
		_isDragComplete = true;
	}
}


void AGsActorSummonComposeSceneObject::EndPlay(EEndPlayReason::Type Reaseon)
{
	if (nullptr != _sceneCamera)
	{
		GLevel()->PopViewTarget(_sceneCamera->GetOwner());
	}

	Super::EndPlay(Reaseon);
}
void AGsActorSummonComposeSceneObject::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	UpdateReleseStone(DeltaTime);

	// 드래그 끝났다면
	if (_isDragComplete == true)
	{
		_isDragComplete = false;
		// 드래그 값 초기화
		_dragProgressVal = 0.0f;

		EndDragSound();

		CompleteDrag();
	}
}

// 터치 땠을때 소환석 돌아가는 처리
void AGsActorSummonComposeSceneObject::UpdateReleseStone(float In_deltaTime)
{
	// 터치 누르고 있거나
	// 진행도가 0이면 처리 안함
	// not drag complete
	if (_isTouchPress == true ||
		_dragProgressVal == 0.0f ||
		_isDragComplete == true)
	{
		return;
	}

	float changeVal = In_deltaTime *
		GData()->GetGlobalData()->_summonStoneTouchReleaseMulVal;

	float oldVal = _dragProgressVal;
	_dragProgressVal =
		FMath::Max(_dragProgressVal - changeVal, 0.0f);

	
	ChangeProgress(_dragProgressVal, oldVal);
}

// 드래그 사운드 끝
void AGsActorSummonComposeSceneObject::EndDragSound()
{
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		if (_setDragSoundId.Num() != 0)
		{
			for (auto iter : _setDragSoundId)
			{
				soundPlayer->StopSound(iter);
			}

			_setDragSoundId.Empty();
		}
	}
}
// 드래그 사운드 시작
void AGsActorSummonComposeSceneObject::StartDragSound()
{
	if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
	{
		uint64 dragSoundId = soundPlayer->PlaySound2D("Snd_Cue_UI_Fairy_Drag");
		if (_setDragSoundId.Contains(dragSoundId) == false)
		{
			_setDragSoundId.Add(dragSoundId);
		}
	}
}

void AGsActorSummonComposeSceneObject::CompleteDrag()
{
	// summon mixer 빼고 다음 시네마 처리
	// 안그럼 소리가 안나올지도
	// bgm 끄기
	ChangeSoundSummonMixer(false);

	int sequenceId = GSSummon()->GetComposeResultSequenceId();
	// 저사양 ios 메모리 부족 크래쉬 이슈
	// https://jira.com2us.com/jira/browse/C2URWQ-6794
	bool isStore = IsBranch(EGsGameClientBranchType::SUMMON_CINEMATIC_IOS_MEMORY_ISSUE) ? false : true;
	FGsSequenceMessageParam param(sequenceId, this, isStore);
	GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_START, &param);

	GSSummon()->SetIsPlaySequence(true);

	DespawnCeline();

	// hide summon skip
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}
	uiMgr->CloseByKeyName("TraySummonSkip", true);

	_isDragInteractionFinish = true;

}

void AGsActorSummonComposeSceneObject::Finalize()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	// 인풋 해제
	if (APlayerController* controller = world->GetFirstPlayerController())
	{
		if (InputComponent)
		{
			DisableInput(controller);
		}
	}
	// https://jira.com2us.com/jira/browse/C2URWQ-5387
	// 씬 삭제시 말고
	// 드래그 완료시와
	// 스킵시에서 돌린다(순서 꼬이는 문제)
	// 믹서 원래대로 돌린다
	//ChangeSoundSummonMixer(false);

	_summonComposeBGMSoundId = 0;
	if (_setDragSoundId.Num() != 0)
	{
		_setDragSoundId.Empty();
	}
	_dragProgressVal = 0.0f;

	DespawnCeline();
}

// 소환 사운드 믹서 교체후 기본 음악(bgm) 출력
void AGsActorSummonComposeSceneObject::ChangeSoundSummonMixer(bool In_isSummonMode)
{
	UGsSoundManager* soundManager = GSound();

	if (soundManager == nullptr)
	{
		return;
	}

	UGsSoundMixerController* soundMixerController = soundManager->GetOrCreateSoundMixerController();

	if (soundMixerController == nullptr)
	{
		return;
	}

	if (In_isSummonMode == true)
	{
		soundMixerController->OnMixChanged(EGsSoundMixType::Summon);
	}
	else
	{
		soundMixerController->OffMixMode(EGsSoundMixType::Summon);
	}

	UGsSoundPlayer* SoundPlayer = soundManager->GetOrCreateSoundPlayer();

	if (SoundPlayer == nullptr)
	{
		return;
	}

	if (In_isSummonMode == true)
	{
		if (_summonComposeBGMSoundId == 0)
		{
			FString composeSoundName =
				GData()->GetGlobalData()->_summonComposeBGMSound;
			_summonComposeBGMSoundId = SoundPlayer->PlaySound2D(composeSoundName);
		}
	}
	else
	{
		SoundPlayer->StopSound(_summonComposeBGMSoundId);
		// 초기화 추가
		_summonComposeBGMSoundId = 0;
	}
}

void AGsActorSummonComposeSceneObject::SpawnCeline()
{
	if (_spawnPos == nullptr)
	{
		return;
	}

	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}

	FString sceneObjectPath = GData()->GetGlobalData()->_summonComposeCeline.ToString();

	FString sceneObjectLoadPath = sceneObjectPath + TEXT("_C");

	if (UClass* sceneObjectC = LoadObject<UClass>(nullptr, *sceneObjectLoadPath))
	{
		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* makeActor =
			world->SpawnActor(sceneObjectC, &FTransform::Identity, spawnParam);

		makeActor->AttachToComponent(_spawnPos, FAttachmentTransformRules::KeepRelativeTransform);

		_composeCeline = Cast<AGsActorSummonComposeCeline>(makeActor);

		if (_composeCeline != nullptr)
		{
			_composeCeline->SetAnimationPause();
		}

	}
}

void AGsActorSummonComposeSceneObject::DespawnCeline()
{
	if (_composeCeline != nullptr)
	{
		_composeCeline->Destroy();
		_composeCeline = nullptr;
	}
}

TWeakObjectPtr<UGsUITraySummonSkip> AGsActorSummonComposeSceneObject::GetUITraySummonSkip()
{
	if (_summonSkipUI.IsValid() == true)
	{
		return _summonSkipUI;
	}

	
	TWeakObjectPtr<UGsUIWidgetBase> wigdet = GUI()->GetWidgetByKey(TEXT("TraySummonSkip"));
	_summonSkipUI = Cast<UGsUITraySummonSkip>(wigdet);

	return _summonSkipUI;
}

void AGsActorSummonComposeSceneObject::ChangeProgressUI(float In_progressRate)
{
	TWeakObjectPtr<UGsUITraySummonSkip> summonSkip = GetUITraySummonSkip();

	if (summonSkip.IsValid() == false)
	{
		return;
	}

	summonSkip->OnChangedProgressRate(In_progressRate);
}

void AGsActorSummonComposeSceneObject::ChangeProgress(float In_nowProgressRate, float In_oldProgressRate)
{
	ChangeProgressUI(In_nowProgressRate);

	if (_composeCeline != nullptr)
	{
		_composeCeline->OnDragProgress(In_nowProgressRate, In_oldProgressRate);
	}

	ChangeProgressCamPos(In_nowProgressRate);
}

void AGsActorSummonComposeSceneObject::ChangeProgressCamPos(float In_progressRate)
{
	if (_camStartPos == nullptr || _camEndPos == nullptr ||
	_sceneCamera == nullptr)
	{
		return;
	}

	FVector startPos = _camStartPos->GetComponentLocation();
	FVector endPos = _camEndPos->GetComponentLocation();

	FVector newPos = FMath::Lerp(startPos, endPos, In_progressRate);

	_sceneCamera->SetWorldLocation(newPos);
}