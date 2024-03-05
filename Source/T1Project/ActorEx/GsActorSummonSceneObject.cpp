#include "GsActorSummonSceneObject.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"

#include "Management/ScopeGame/GsSummonManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"

#include "Input/GsInputBindingLocalPlayer.h"

#include "ActorEx/GsActorSummonFairyStone.h"
#include "ActorEx/GsActorSummonFairy.h"
#include "ActorEx/GsActorSummonVeilObject.h"
#include "ActorEx/GsActorSummonCostume.h"
#include "ActorEx/GsActorSummonVeilCostume.h"

#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

#include "Fairy/GsFairyData.h"

#include "Costume/GsCostumeData.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataSchema/Summon/GsSchemaSummonPosIndex.h"
#include "DataSchema/Summon/GsSchemaSummonCostumeResPath.h"

#include "DataSchema/GsSchemaEnums.h"

#include "Message/GsMessageSystem.h"
#include "Message/MessageParam/GsMessageParam.h"

#include "Summon/GsSummonDefine.h"
#include "Summon/GsSummonCardData.h"

#include "Option/GsGameUserSettings.h"

#include "UI/UIContent/Tray/GsUITraySummon.h"
#include "UI/UIContent/Tray/GsUITrayFadeEffect.h"
#include "UI/UIContent/Billboard/GsUIBillboardSummonStone.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsClassUtil.h"
#include "UTIL/GsDrawDebugHelpers.h"

#include "Camera/Summon/Interface/GsSummonCameraWork.h"
#include "Camera/Summon/GsSummonCameraRotaion.h"
#include "Camera/Summon/GsSummonCameraLocation.h"
#include "Camera/Summon/GsSummonCameraBlur.h"

#include "GameMode/GsGameModeBase.h"
#include "GameMode/GsGameModeDefine.h"
#include "GameMode/GsGameModeTestClient.h"

#include "GameFlow/GameContents/ContentsGame/Hud/Summon/GsSummonHandler.h"

#include "Sound/GsSoundMixerController.h"
#include "Sound/GsSoundPlayer.h"

#include "Engine/Engine.h"
#include "Engine/Classes/Camera/PlayerCameraManager.h"

#include "GameFramework/PlayerController.h"

#include "Camera/CameraComponent.h"
#include "CinematicCamera/Public/CineCameraComponent.h"

#include "Core/Public/Misc/DateTime.h"

#include "Classes/Components/SceneComponent.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Engine/Scene.h"
#include "Engine/Classes/Curves/CurveFloat.h"

#include "Kismet/GameplayStatics.h"

#include "Classes/Particles/ParticleSystem.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"

#include "Algo/Reverse.h"

#include "T1Project.h"

const FName SceneRootComponentName = TEXT("SummonRootComponent");
const FName CameraRotComponentName = TEXT("CameraRotComponent");
const FName SceneCameraComponentName = TEXT("SummonSceneCamera");
const FName LeafEffectPosName = TEXT("LeafEffectPos");

AGsActorSummonSceneObject::AGsActorSummonSceneObject() :Super()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(SceneRootComponentName);

	_cameraRotator = CreateDefaultSubobject<USceneComponent>(CameraRotComponentName);
	_cameraRotator->SetupAttachment(RootComponent);

	//_sceneCamera = CreateDefaultSubobject<UCineCameraComponent>(SceneCameraComponentName);
	_sceneCamera = CreateDefaultSubobject<UCameraComponent>(SceneCameraComponentName);
	_sceneCamera->SetupAttachment(_cameraRotator);


	_leafFxPos = CreateDefaultSubobject<USceneComponent>(LeafEffectPosName);
	_leafFxPos->SetupAttachment(_sceneCamera);
}

void AGsActorSummonSceneObject::Initialize()
{
	// 드래그 인풋 등록
	InputComponent = NewObject<UInputComponent>(this);

	if (InputComponent)
	{
		if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
		{
			// 카메라 초기화
			GLevel()->PushViewTarget(_sceneCamera->GetOwner());

			InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AGsActorSummonSceneObject::OnTouchMove);
			InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AGsActorSummonSceneObject::OnTouchPress);
			InputComponent->BindTouch(EInputEvent::IE_Released, this, &AGsActorSummonSceneObject::OnTouchRelease);

			EnableInput(controller);
		}
	}


	IGsSummonCameraWork* blurWork = new FGsSummonCameraBlur();
	blurWork->Initialize();
	blurWork->SetOwner(this);

	if (FGsSummonCameraBlur* blurWorkClass = static_cast<FGsSummonCameraBlur*>(blurWork))
	{
		blurWorkClass->_callbackFinishLerp = [this](EGsCameraWorkType In_type)
		{
			OnZoomLerpFinish(In_type);
		};
	}

	_mapCamWorks.Add(EGsCameraWorkType::BlurWork, blurWork);

	// 카메라 work 등록
	IGsSummonCameraWork* rotationWork = new FGsSummonCameraRotation();
	rotationWork->Initialize();
	rotationWork->SetOwner(this);

	if (FGsSummonCameraRotation* rotationWorkClass = static_cast<FGsSummonCameraRotation*>(rotationWork))
	{
		rotationWorkClass->_callbackFinishLerp = [this](EGsCameraWorkType In_type)
		{
			OnZoomLerpFinish(In_type);
		};
	}

	_mapCamWorks.Add(EGsCameraWorkType::RotationWork, rotationWork);

	IGsSummonCameraWork* locationWork = new FGsSummonCameraLocation();
	locationWork->Initialize();
	locationWork->SetOwner(this);

	if (FGsSummonCameraLocation* locationWorkClass = static_cast<FGsSummonCameraLocation*>(locationWork))
	{
		locationWorkClass->_callbackFinishLerp = [this](EGsCameraWorkType In_type)
		{
			OnZoomLerpFinish(In_type);
		};
	}

	_mapCamWorks.Add(EGsCameraWorkType::LocationWork, locationWork);


	_backupCameraExposureVal = _sceneCamera->PostProcessSettings.AutoExposureBias;

	_dragCameraWhiteCurve = GData()->GetSummonDragCameraWhiteCurve();

	_dragCameraExposureMax = GData()->GetGlobalData()->_summonDragCameraExposureMax;

	FGsMessageHolder* message = GMessage();

	_listUiDelegate.Emplace(
		message->GetUI().AddUObject(MessageUI::TRAY_FADE_EFFECT_ANI_FINISHED, this, &AGsActorSummonSceneObject::OnFadeEffectAniFinished));
}
void AGsActorSummonSceneObject::OnTouchPress(ETouchIndex::Type FingerIndex, FVector Location)
{

	// 1번 아니면 안함
	if (ETouchIndex::Touch1 != FingerIndex)
	{
		return;
	}


	// high grade zoom 	state
	if(_summonStep == EGsSummonProgressStepType::HighGradeZoom)
	{
		_isTouchPress = true;
		_prevTouchLoc = Location;
		_pressTouchLoc = Location;


		StartDragSound();
	}
	else if (_summonStep == EGsSummonProgressStepType::ShowAllStone)
	{
		UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
		if (player == nullptr)
		{
			return;
		}
		UGsGameObjectLocalPlayer* local = player->CastGameObject<UGsGameObjectLocalPlayer>();

		if (local == nullptr)
		{
			return;
		}
		UGsInputBindingLocalPlayer* inputBinder = local->GetInputBinder();

		if (nullptr == inputBinder)
		{
			return;
		}
		if (true == inputBinder->GetIsDrawDebugArrow())
		{
			FGsDrawDebugHelpers::DrawDebugArrow((FVector2D)Location);
		}

	}
}
void AGsActorSummonSceneObject::OnTouchRelease(ETouchIndex::Type FingerIndex, FVector Location)
{
	// 1번 아니면 안함
	if (ETouchIndex::Touch1 != FingerIndex)
	{
		return;
	}
	// high grade zoom 	state
	if(_summonStep == EGsSummonProgressStepType::HighGradeZoom)
	{
		if (_stonePress == true)
		{
			// not use player Controller 2368  CurrentComponent == null when focus out release
			if (_highGradeDragStone != nullptr)
			{
				_highGradeDragStone->OnStoneTouchEnd();
			}

			OnTouchEndHighGradeSummonStone();
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
	else if (_summonStep == EGsSummonProgressStepType::ShowAllStone)
	{

	}

}
void AGsActorSummonSceneObject::OnTouchMove(ETouchIndex::Type FingerIndex, FVector Location)
{
	// 1번 아니면 안함
	// 줌상태 아니어도 안함
	if (ETouchIndex::Touch1 != FingerIndex||
		_summonStep != EGsSummonProgressStepType::HighGradeZoom)
	{
		return;
	}

	if (_highGradeDragStone == nullptr ||
		_stonePress == false)
	{
		return;
	}

	// 처음 터치한 위치 기준(드래그 중간에 좌우 다되게)
	float diffX = Location.X - _pressTouchLoc.X;

	// 0 보다 크면 오른쪽 방향이 순방향
	if (diffX > 0.0f)
	{
		_isRightDrag = true;
	}
	else
	{
		_isRightDrag = false;
	}
	

	// 이전과 차이값
	FVector dir = Location - _prevTouchLoc;

	_prevTouchLoc = Location;

	float xDiffVal = dir.X * GData()->GetGlobalData()->_summonDragMulVal;

	// 0보다 크면 오른쪽 방향
	bool isRightDragNow = (xDiffVal > 0.0f) ? true : false;
	// 절대값
	float absDiffVal = FMath::Abs(xDiffVal);
	// 방향이 같으면 양수, 아니면 음수
	float  addVal = (isRightDragNow == _isRightDrag) ? absDiffVal : -absDiffVal;
	
	// 0에서 1사이만 저장
	float newDragVal = FMath::Clamp(_dragProgressVal + addVal, 0.0f, 1.0f );

	if (_dragProgressVal == newDragVal)
	{
		return;
	}

	float oldVal = _dragProgressVal;
	_dragProgressVal = newDragVal;


	_highGradeDragStone->OnDragProgress(_dragProgressVal, oldVal);

	// 끝가지 도달하면
	// 고등급 줌 상태
	if (_dragProgressVal == 1.0f)
	{
		_isDragComplete = true;
	}
}

// 줌 끝났을때
void AGsActorSummonSceneObject::OnZoomLerpFinish(EGsCameraWorkType In_type)
{
	if (In_type == EGsCameraWorkType::BlurWork)
	{
		SetHideObjects(true, _selectVeilObjectIndex);
	}
	else
	{
		_arrayZoomWork.Remove(In_type);

		if (_arrayZoomWork.Num() != 0)
		{
			return;
		}

		// 포스트 프로세스 값 0으로 초기화
		if (_sceneCamera == nullptr)
		{
			return;
		}

		if (_sceneCamera->PostProcessSettings.WeightedBlendables.Array.Num() == 0)
		{
			return;
		}

		_sceneCamera->PostProcessSettings.WeightedBlendables.Array[0].Weight = 0.0f;

		// 줌 상태로 전환
		_summonStep = EGsSummonProgressStepType::HighGradeZoom;
	}
}

// 스톤 스폰
void AGsActorSummonSceneObject::SummonFairyStones()
{
	TArray<TSharedPtr<FGsFairyData>> arrayData = GSSummon()->GetSummonFairyData();

	if (_arraySummonStoneGrade.Num() != 0)
	{
		_arraySummonStoneGrade.Empty();
	}
	if (arrayData.Num() == 0)
	{
		return;
	}
	for (auto iter : arrayData)
	{
		if (iter.IsValid() == false)
		{
			continue;
		}

		_arraySummonStoneGrade.Add(iter->GetFairyGrade());
	}

	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}
	bool isSkip = false;
	if (UGsSummonManager* summonManager = GSSummon())
	{
		isSkip = summonManager->IsSummonSkip();
	}

	// 스킵인가
	if (isSkip == true)
	{
		if (_arraySummonStoneGrade.Num() != 0)
		{
			int lastIndex = _arraySummonStoneGrade.Num() - 1;
			for (int i= lastIndex; 0<= i; --i)
			{
				FairyGrade grade = _arraySummonStoneGrade[i];
				SpawnFairyStone(i, grade);
			}
		}
		
		_arraySummonStoneGrade.Empty();

		SetBtnTypeOpenAll();
	}
	else
	{		
		// 처음 하나 스폰
		SummonFairyStoneSingle();
	}
}

// 페어리 소환석 스폰 처리
AGsActorSummonVeilObject* AGsActorSummonSceneObject::SpawnFairyStone(int In_dataIndex, FairyGrade In_grade)
{
	FString summonStoneNormalPath =
		GData()->GetGlobalData()->_summonStoneNormalPath.ToString();

	FString summonStoneHighPath =
		GData()->GetGlobalData()->_summonStoneHighPath.ToString();

	FString summonStonePath = IsHighGradeFairy(In_grade) ? summonStoneHighPath : summonStoneNormalPath;

	AGsActorSummonVeilObject* veilObject = SpawnVeilObject(summonStonePath, In_dataIndex);;
	
	if ( nullptr != veilObject)
	{
		if (AGsActorSummonFairyStone* fairyStone = Cast<AGsActorSummonFairyStone>(veilObject))
		{
			fairyStone->SetGrade(In_grade);
			fairyStone->AttachGradeParticle();
		}
	}

	return veilObject;
}

// veil 코스튬 스폰 처리
AGsActorSummonVeilObject* AGsActorSummonSceneObject::SpawnVeilCostume(int In_dataIndex, CostumeGrade In_grade)
{
	FString summonVeilCostumeNormalPath =
		GData()->GetGlobalData()->_summonVeilCostumeNormalPath.ToString();
	FString summonVeilCostumeHighPath =
		GData()->GetGlobalData()->_summonVeilCostumeHighPath.ToString();

	FString summonVeilCostumePath = IsHighGradeCostume(In_grade) ? summonVeilCostumeHighPath : summonVeilCostumeNormalPath;

	AGsActorSummonVeilObject* veilObject = SpawnVeilObject(summonVeilCostumePath, In_dataIndex);
	if (nullptr != veilObject)
	{
		if (AGsActorSummonVeilCostume* veilCostume = Cast<AGsActorSummonVeilCostume>(veilObject))
		{
			veilCostume->SetGrade(In_grade);
			veilCostume->AttachGradeParticle();
		}
	}

	return veilObject;
}
void AGsActorSummonSceneObject::SummonVeilCostumes()
{
	TArray<TSharedPtr<FGsCostumeData>> arrayData = GSSummon()->GetSummonCostumeData();

	if (_arraySummonVeilCostumeGrade.Num() != 0)
	{
		_arraySummonVeilCostumeGrade.Empty();
	}
	if (arrayData.Num() == 0)
	{
		return;
	}
	for (auto iter : arrayData)
	{
		if (iter.IsValid() == false)
		{
			continue;
		}

		_arraySummonVeilCostumeGrade.Add(iter->GetCostumeGrade());
	}

	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}
	bool isSkip = false;
	if (UGsSummonManager* summonManager = GSSummon())
	{
		isSkip = summonManager->IsSummonSkip();
	}

	// 스킵인가
	if (isSkip == true)
	{

		if (_arraySummonVeilCostumeGrade.Num() != 0)
		{
			int lastIndex = _arraySummonVeilCostumeGrade.Num() - 1;
			for (int i = lastIndex; 0 <= i; --i)
			{
				CostumeGrade grade = _arraySummonVeilCostumeGrade[i];
				SpawnVeilCostume(i, grade);
			}
		}

		_arraySummonVeilCostumeGrade.Empty();

		SetBtnTypeOpenAll();
	}
	else
	{
		// 처음 하나 스폰
		SummonVeilCostumeSingle();
	 }

}

void AGsActorSummonSceneObject::EndPlay(EEndPlayReason::Type Reaseon)
{
	GLevel()->PopViewTarget(_sceneCamera->GetOwner());




	Super::EndPlay(Reaseon);
}

// Called every frame
void AGsActorSummonSceneObject::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if (_mapCamWorks.Num() >0)
	{
		for (const auto& iter: _mapCamWorks )
		{
			if (iter.Value == nullptr)
			{
				continue;
			}

			iter.Value->Update(DeltaTime);
		}
	}
	UpdateReleseStone(DeltaTime);

	// 드래그 끝났다면
	if (_isDragComplete == true)
	{
		_isDragComplete = false;
		// 드래그 값 초기화
		_dragProgressVal = 0.0f;

		_summonStep = EGsSummonProgressStepType::SuccessDragVeilObject;

		if (_highGradeDragStone != nullptr)
		{
			_highGradeDragStone->StartSuccessDragMorphTarget();
			// 위젯 끈다
			_highGradeDragStone->SetHideWidget(true);
		}
		// bp에 연출 노티 처리
		SuccessDragHighVeilObject();

		EndDragSound();

		// cam shake finish
		UWorld* world = GetWorld();
		if (APlayerController* controller = UGameplayStatics::GetPlayerController(world, 0))
		{
			if (APlayerCameraManager* camManager = controller->PlayerCameraManager)
			{
				camManager->StopAllCameraShakes(true);
			}
		}
	}

	if (_summonStep == EGsSummonProgressStepType::SuccessDragVeilObject)
	{
		if (_highGradeDragStone != nullptr)
		{
			_highGradeDragStone->UpdateSuccessDragMorphTarget(DeltaTime);

			// 진행된 만큼 카메라 white 처리
			UpdateCameraExposure(_highGradeDragStone->GetSuccessDragProgressVal());
		
			if (true == _highGradeDragStone->GetIsSuccessDragMorpthTargetFinish())
			{
				_summonStep = EGsSummonProgressStepType::HighGradeAppearCutscene;

				// 등장 연출 처리
				StartHighGradeOpen();
				// 현재 열린 unveil object 디스폰 처리(veil show는 안함)
				// 연출 안에서 안보이게 할려고
				OpenUnveilObject(_selectVeilObjectIndex, false);
			}
		}
	}

	// 소환석 순차 소환 처리
	UpdateNextSummonVeilObject();
	// 소환 unveil object show 처리
	UpdateNextSummonUnveilObject();

	UpdateDragEffectColor();
}

// 시간에 따른 다음 소환석 스폰
void AGsActorSummonSceneObject::UpdateNextSummonVeilObject()
{
	if (_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		if (_arraySummonStoneGrade.Num() == 0)
		{
			return;
		}
	}
	else
	{
		if (_arraySummonVeilCostumeGrade.Num() == 0)
		{
			return;
		}
	}

	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _startTimeSummonStone;

	static float maxTimeTick = 
		GData()->GetGlobalData()->_spawnSummonStoneWaitSecond * ETimespan::TicksPerSecond;

	// 다음 스폰 시간
	if (diffTick >= maxTimeTick)
	{
		if (_summonType == EGsSummonType::SUMMON_FAIRY)
		{
			SummonFairyStoneSingle();
		}
		else
		{
			SummonVeilCostumeSingle();
		}
	}

	bool isAllVeilSpawn = false;
	// 마지막 스폰까지 했으면 콜리전 다시 킨다
	if (_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		if (_arraySummonStoneGrade.Num() == 0)
		{
			isAllVeilSpawn = true;
		}
	}
	else
	{
		if (_arraySummonVeilCostumeGrade.Num() == 0)
		{
			isAllVeilSpawn = true;
		}
	}

	if (true == isAllVeilSpawn)
	{
		// 충돌 복구
		SetStoneTouchCollisionActive(true);
	}

}

// 시간에 따른 다음 unveil object
void AGsActorSummonSceneObject::UpdateNextSummonUnveilObject()
{
	if (_arraySummonOrderUnveilObjectShow.Num() == 0)
	{
		return;
	}

	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _startTimeSummonUnveilObject;

	static float maxTimeTick = 
		GData()->GetGlobalData()->_spawnSummonUnveilObjectWaitSecond * ETimespan::TicksPerSecond;

	if (diffTick >= maxTimeTick)
	{
		ShowUnveilObjectSingle();
	}

	// 마지막까지 show 했다면
	if (_arraySummonOrderUnveilObjectShow.Num() == 0)
	{
		// 충돌체 복구
		SetStoneTouchCollisionActive(true);
	}
}

// 시간에 따른 drag 이펙트 색상 처리
void AGsActorSummonSceneObject::UpdateDragEffectColor()
{
	// 터치 때거나
	// 줌상태 아니어도 안함
	if (_isTouchPress == false ||
		_stonePress == false ||
		_highGradeDragStone == nullptr ||
		_summonStep != EGsSummonProgressStepType::HighGradeZoom)
	{
		return;
	}

	int64 nowTick = FDateTime::UtcNow().GetTicks();
	int64 diffTick = nowTick - _startTimeEffectColorChange;

	static float maxTimeTick = 
		GData()->GetGlobalData()->_summonEffectColorChangeSecond * ETimespan::TicksPerSecond;

	if (diffTick >= maxTimeTick)
	{
		_startTimeEffectColorChange = nowTick;
		_highGradeDragStone->ChangeEffectColor();
	}
}

// 소환석 스폰
AGsActorSummonVeilObject* AGsActorSummonSceneObject::SpawnVeilObject(
	FString In_path, int In_index)
{
	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return nullptr;
	}

	AGsActorSummonVeilObject* stoneActor = nullptr;

	FString summonStonePath = In_path;
	summonStonePath += TEXT("_C");
	if (UClass* loadFairyStoneC = LoadObject<UClass>(nullptr, *summonStonePath))
	{
		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* makeActor =
			world->SpawnActor(loadFairyStoneC, &FTransform::Identity, spawnParam);

		if (makeActor == nullptr)
		{
			return nullptr;
		}

		stoneActor = Cast<AGsActorSummonVeilObject>(makeActor);

		if (stoneActor == nullptr)
		{
			return nullptr;
		}

		_arraySummonVeilObject.Add(stoneActor);
		_mapSummonVeilObject.Add(In_index, stoneActor);

		USceneComponent* posComponent = GetPosComponent(In_index);
		if (posComponent == nullptr)
		{
			return nullptr;
		}
		stoneActor->AttachToComponent(posComponent, FAttachmentTransformRules::KeepRelativeTransform);

		FVector offset = stoneActor->GetSpawnPosOffset();
		stoneActor->SetActorRelativeLocation(offset);

		// 인덱스 세팅
		stoneActor->SetDataIndex(In_index);	
		stoneActor->_callbackClick = [this](int In_dataIndex)
		{
			OnClickSummonVeilObject(In_dataIndex);
		};

		stoneActor->_callbackDragTouchStart = [this](AGsActorSummonVeilObject* In_stone)
		{
			OnTouchStartHighGradeSummonStone(In_stone);
		};

		//stoneActor->_callbackDragTouchEnd = [this]()
		//{
		//	OnTouchEndHighGradeSummonStone();
		//};
	}

	return stoneActor;
}

// 소환석 디스폰
void AGsActorSummonSceneObject::DespawnVeilObject(int In_index)
{
	// 소환석 디스폰
	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}
	AGsActorSummonVeilObject* stone =
		_mapSummonVeilObject.FindRef(In_index);

	if (stone != nullptr)
	{
		if (_highGradeDragStone == stone)
		{
			_highGradeDragStone = nullptr;
		}

		stone->Finalize();
		_arraySummonVeilObject.Remove(stone);
		_mapSummonVeilObject.Remove(In_index);
		world->DestroyActor(stone);
	}
}
// 모든 소환석 디스폰
void AGsActorSummonSceneObject::DespawnVeilObjects(bool In_isAllGrade)
{
	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}

	if (_arraySummonVeilObject.Num() != 0)
	{
		if (In_isAllGrade == true)
		{
			for (auto iter : _arraySummonVeilObject)
			{
				iter->Finalize();
				world->DestroyActor(iter);
			}

			_arraySummonVeilObject.Empty();
			_mapSummonVeilObject.Empty();
		}
		// 저등급만
		else
		{
			TArray<int> despawnIds;
			for (auto iter : _arraySummonVeilObject)
			{				
				if (iter->IsHighGrade() == false)
				{
					despawnIds.Add(iter->GetDataIndex());
				}
			}
			for (int id: despawnIds)
			{
				DespawnVeilObject(id);
			}
		}
	}
}
// 페어리 스폰
IGsSummonUnveilObjectInterface* AGsActorSummonSceneObject::SpawnUnveilObject(
	FString In_path, int In_index)
{
	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return nullptr;
	}

	IGsSummonUnveilObjectInterface* summonUnveilObject = nullptr;

	FString summonUnveilObjectPath = In_path += TEXT("_C");
	if (UClass* unveilObjectC = LoadObject<UClass>(nullptr, *summonUnveilObjectPath))
	{
		FActorSpawnParameters spawnParam = FActorSpawnParameters();
		spawnParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AActor* makeActor =
			world->SpawnActor(unveilObjectC, &FTransform::Identity, spawnParam);

		if (makeActor == nullptr)
		{
			return nullptr;
		}

		if (_summonType == EGsSummonType::SUMMON_FAIRY)
		{
			summonUnveilObject =
				Cast<AGsActorSummonFairy>(makeActor);
		}
		else
		{
			summonUnveilObject =
				Cast<AGsActorSummonCostume>(makeActor);
		}

		if (summonUnveilObject == nullptr)
		{
			return nullptr;
		}

		summonUnveilObject->SetDataIndex(In_index);

		_arraySummonUnveilObject.Add(makeActor);
		_mapSummonUnveilObject.Add(In_index, summonUnveilObject);

		USceneComponent* posComponent = GetPosComponent(In_index);
		if (posComponent == nullptr)
		{
			return nullptr;
		}
		makeActor->AttachToComponent(posComponent, FAttachmentTransformRules::KeepRelativeTransform);

		FVector offset = summonUnveilObject->GetSpawnPosOffset();
		makeActor->SetActorRelativeLocation(offset);

		// 처음 스폰하자마자 hide
		summonUnveilObject->SetHide(true);
	}

	return summonUnveilObject;
}

// 모든 페어리 디스폰
void AGsActorSummonSceneObject::DespawnUnveilObjects()
{
	if (_mapSummonUnveilObject.Num() != 0)
	{
		for (auto iterUnveilObject : _mapSummonUnveilObject)
		{
			if (iterUnveilObject.Value == nullptr)
			{
				continue;
			}

			iterUnveilObject.Value->Finalize();
		}
	}

	if (_arraySummonUnveilObject.Num() != 0)
	{
		for (auto iterUnveilActor : _arraySummonUnveilObject)
		{
			iterUnveilActor->Destroy();
		}
		_arraySummonUnveilObject.Empty();
		_mapSummonUnveilObject.Empty();
	}
}
// 스톤 스폰 하나(no skip)
void AGsActorSummonSceneObject::SummonFairyStoneSingle()
{
	if (_arraySummonStoneGrade.Num() == 0)
	{
		return;
	}

	// 시간 저장
	_startTimeSummonStone = FDateTime::UtcNow().GetTicks();

	int index = _arraySummonStoneGrade.Num() - 1;


	// 등급가져오기
	FairyGrade grade = _arraySummonStoneGrade[index];
	// 스폰 대기 배열에서 삭제
	_arraySummonStoneGrade.RemoveAt(index);
	
	if (AGsActorSummonVeilObject* makeStone = SpawnFairyStone(index, grade))
	{	
		// 마지막꺼나 하나짜리는 콜리전 끌 필요 없다
		if (index != 0)
		{
			// 스폰 연출중엔 콜리전 끄기
			makeStone->SetTouchCollisionEnable(false);
		}
	}

	// 1. spawn stone
	// 2. check btn state
	if (index == 0)
	{
		SetBtnTypeOpenAll();
	}
}

// veil 코스튬 스폰 하나(no skip)
void AGsActorSummonSceneObject::SummonVeilCostumeSingle()
{
	if (_arraySummonVeilCostumeGrade.Num() == 0)
	{
		return;
	}

	// 시간 저장
	_startTimeSummonStone = FDateTime::UtcNow().GetTicks();

	int index = _arraySummonVeilCostumeGrade.Num() - 1;

	// 등급가져오기
	CostumeGrade grade = _arraySummonVeilCostumeGrade[index];
	// 스폰 대기 배열에서 삭제
	_arraySummonVeilCostumeGrade.RemoveAt(index);

	if (AGsActorSummonVeilObject* makeCube = SpawnVeilCostume(index, grade))
	{
		// 마지막꺼나 하나짜리는 콜리전 끌 필요 없다
		if (index != 0)
		{
			makeCube->SetTouchCollisionEnable(false);
		}
	}

	// 1. spawn stone
	// 2. check btn state
	if (index == 0)
	{
		SetBtnTypeOpenAll();
	}
}

// 인덱스에 의한 위치 구하기
USceneComponent* AGsActorSummonSceneObject::GetPosComponent(int In_index)
{
	// 나중에 테이블로 서치 추가
	if (_arraySpawnPos.Num() <= In_index)
	{
		return nullptr;
	}

	int count = GSSummon()->GetCurrentSummonTypeDataCount();
	
	const FGsSchemaSummonPosIndex* findTbl =
		UGsTableUtil::FindRowById<UGsTableSummonPosIndex, FGsSchemaSummonPosIndex>(count);

	if (findTbl == nullptr)
	{
		return nullptr;
	}
	
	if (findTbl->arrayPosIndex.Num() <= In_index)
	{
		return nullptr;
	}

	int posIndex = findTbl->arrayPosIndex[In_index];

	if (_arraySpawnPos.Num() <= posIndex)
	{
		return nullptr;
	}

	return _arraySpawnPos[posIndex];
}

// 소멸 처리
void AGsActorSummonSceneObject::Finalize()
{
	UWorld* world = GetWorld();

	if (world == nullptr)
	{
		return;
	}

	DespawnVeilObjects(true);
	
	DespawnUnveilObjects();

	_traySummon = nullptr;

	if (_mapCamWorks.Num() > 0)
	{
		for (auto& iter : _mapCamWorks)
		{
			if (iter.Value == nullptr)
			{
				continue;
			}
			iter.Value->Finalize();
			delete iter.Value;
			iter.Value = nullptr;
		}
		_mapCamWorks.Empty();
	}

	// 인풋 해제
	if (APlayerController* controller = world->GetFirstPlayerController())
	{
		if (InputComponent)
		{
			DisableInput(controller);
		}
	}

	_highGradeDragStone = nullptr;

	_dragProgressVal = 0.0f;

	_localData = nullptr;

	// 먼가 있으면 비운다
	if (_arraySummonStoneGrade.Num() != 0)
	{
		_arraySummonStoneGrade.Empty();
	}

	if (_arraySummonVeilCostumeGrade.Num() != 0)
	{
		_arraySummonVeilCostumeGrade.Empty();
	}

	if (_arraySummonOrderUnveilObjectShow.Num() != 0)
	{
		_arraySummonOrderUnveilObjectShow.Empty();
	}

	_isLockAllVeilShow = false;

	// 믹서 원래대로 돌린다
	ChangeSoundSummonMixer(false);

	_summonBaseSoundId = 0;
	if (_setDragSoundId.Num() != 0)
	{
		_setDragSoundId.Empty();
	}
	FGsMessageHolder* msg = GMessage();

	if (_listUiDelegate.Num() != 0)
	{
		for (auto iter : _listUiDelegate)
		{
			msg->GetUI().Remove(iter);
		}
		_listUiDelegate.Empty();
	}
}

// 페어리 스폰
void AGsActorSummonSceneObject::SummonFairy(int In_dataIndex)
{
	TArray<TSharedPtr<FGsFairyData>> arrayData = GSSummon()->GetSummonFairyData();

	if (arrayData.Num() == 0||
		arrayData.Num() <= In_dataIndex) 
	{
		return;
	}

	TSharedPtr<FGsFairyData> fairyData = arrayData[In_dataIndex];

	if (fairyData.IsValid() == false)
	{
		return;
	}
	FairyGrade fairyGrade = fairyData->GetFairyGrade();
	int fairyId = fairyData->_fairyCommon->id;

	FString path =
		fairyData->GetSummonFairyPath().ToString();

	if (IGsSummonUnveilObjectInterface* unveilObject = SpawnUnveilObject(path, In_dataIndex))
	{
		if (AGsActorSummonFairy* fairy = static_cast<AGsActorSummonFairy*>(unveilObject))
		{
			fairy->SetGrade(fairyGrade);
			fairy->AttachGradeParticle();
		}

	}

	/*GSLOG(Log, TEXT("id: %d, grade: %d"), fairyId,
		(int)fairyGrade);*/
}
// 코스튬 스폰(하나)
void AGsActorSummonSceneObject::SummonCostume(int In_dataIndex)
{
	TArray<TSharedPtr<FGsCostumeData>> arrayData = GSSummon()->GetSummonCostumeData();

	if (arrayData.Num() == 0 ||
		arrayData.Num() <= In_dataIndex)
	{
		return;
	}

	TSharedPtr<FGsCostumeData> costumeData = arrayData[In_dataIndex];

	if (costumeData.IsValid() == false)
	{
		return;
	}
	const CostumeGrade costumeGrade = costumeData->GetCostumeGrade();
	int costumeId = costumeData->_costumeData->id;

	if (_localData == nullptr)
	{
		return;
	}
	
	CreatureGenderType genderType = _localData->GetCreatureGenderType();
#if WITH_EDITOR
	if (UWorld* world = GetWorld())
	{
		if (AGsGameModeBase* gameMode = world->GetAuthGameMode<AGsGameModeBase>())
		{
			if (gameMode->GetGameModeType() == EGsGameModeType::TestClient)
			{
				if (AGsGameModeTestClient* testMode = Cast<AGsGameModeTestClient>(gameMode))
				{
					genderType = testMode->_genderType;
				}
			}
		}
	}
#endif

	FString path;
	if (const FGsSchemaSummonCostumeResPath* summonCostume = 
		UGsTableUtil::FindRowById<UGsTableSummonCostumeResPath, FGsSchemaSummonCostumeResPath>(
			genderType))
	{
		path = summonCostume->objectBPPath.ToString();
	}


	if (IGsSummonUnveilObjectInterface* unveilObject = SpawnUnveilObject(path, In_dataIndex))
	{
		if (AGsActorSummonCostume* costume = static_cast<AGsActorSummonCostume*>(unveilObject))
		{
			costume->Initialize(_localData, costumeData->_id, costumeGrade);
			costume->AttachGradeParticle();
		}

	}
}

// 페어리 스폰(모두)
void AGsActorSummonSceneObject::SummonUnveilObjects()
{
	int dataCount = GSSummon()->GetCurrentSummonTypeDataCount();
	if (dataCount == 0)
	{
		return;
	}

	for (int i=0; i< dataCount; ++i)
	{
		// 이미스폰된거면 안함
		if (_mapSummonUnveilObject.Contains(i) == true)
		{
			continue;
		}

		if (_summonType == EGsSummonType::SUMMON_FAIRY)
		{
			SummonFairy(i);
		}
		else
		{
			SummonCostume(i);
		}
	}
}

// veil 오브젝 스폰(모두)
void AGsActorSummonSceneObject::SummonVeilObjects()
{
	MakeLeafEffect();
	// white FadeIn 연출 시작
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("TrayFadeEffect"));
	if (widget.IsValid())
	{
		if (UGsUITrayFadeEffect* fadeEffect = Cast<UGsUITrayFadeEffect>(widget.Get()))
		{
			fadeEffect->SetFadeAnimation(UGsUITrayFadeEffect::EGsFadeAnimationType::FadeIn, FLinearColor::White);
			// crash issue
			// https://sentry.io/organizations/com2us-raon/issues/3771912570/?project=4504115468566528&query=is%3Aunresolved+release%3A%5B%22iOS+Shipping+ProvisioningENT+0.102602%22%2C%22Win64+Shipping+0.102602%22%2C%22Android+Shipping+0.102602%22%5D&referrer=issue-stream


			_isUseFadeEffect = true;
		}
	}

	// 모든 소환석 보이는 단계
	_summonStep = EGsSummonProgressStepType::ShowAllStone;

	// 믹서 변경
	ChangeSoundSummonMixer(true);
}
// 페어리 보여주기(하나)
void AGsActorSummonSceneObject::ShowUnveilObject(int In_dataIndex)
{
	IGsSummonUnveilObjectInterface* fairy =
		_mapSummonUnveilObject.FindRef(In_dataIndex);

	if (fairy == nullptr)
	{
		return;
	}

	fairy->SetHide(false);
}

// unveil object 소환 ani 출력
void AGsActorSummonSceneObject::PlayUnveilObjectSpawnAni(int In_dataIndex)
{
	IGsSummonUnveilObjectInterface* fairy =
		_mapSummonUnveilObject.FindRef(In_dataIndex);

	if (fairy == nullptr)
	{
		return;
	}

	fairy->PlaySpawnAni();

	// unveil spawn ani
	UGsSoundPlayer* soundPlayer = GSoundPlayer();
	if (nullptr == soundPlayer)
		return;

	soundPlayer->PlaySound2D("Snd_wav_UI_Gacha_Unlock");

}
// 페어리 보여주기(저등급만)
void AGsActorSummonSceneObject::ShowFairies()
{
	if (_mapSummonUnveilObject.Num()!= 0)
	{
		for (auto iter: _mapSummonUnveilObject)
		{
			if (iter.Value != nullptr &&
				iter.Value->IsHighGrade() == false)
			{
				iter.Value->SetHide(false);
			}
		}
	}
}
// 소환석 클릭
void AGsActorSummonSceneObject::OnClickSummonVeilObject(int In_index)
{
	_selectVeilObjectIndex = In_index;

	AGsActorSummonVeilObject* veilObject =
		_mapSummonVeilObject.FindRef(In_index);

	if (veilObject == nullptr)
	{
		return;
	}

	if(veilObject->IsHighGrade() == true)
	{
		// 고등급이면 고등급 연출(줌인)
		USceneComponent* posSceneComponent =  GetPosComponent(In_index);
		if (posSceneComponent == nullptr)
		{
			return;
		}
		FVector targetPos = posSceneComponent->GetComponentLocation();
		FVector movePos = FVector::ZeroVector;
		FRotator rotTo = FRotator::ZeroRotator;
		FVector lookTargetPos = FVector::ZeroVector;
		float distCam = 0.0f;
		GSSummon()->GetCamRotAndPos(posSceneComponent, 
			targetPos,
			movePos, 
			rotTo,
			lookTargetPos,
			distCam);

		FGsSummonCameraLocation* locationWork = GetCameraLocation();
		FGsSummonCameraRotation* rotationWork = GetCameraRotation();
		FGsSummonCameraBlur* blurWork = GetCameraBlur();

		if (locationWork == nullptr ||
			rotationWork == nullptr ||
			blurWork == nullptr)
		{
			return;
		}
		FRotator camRot =
			_sceneCamera->GetComponentRotation();
		FVector camLoc =
			_sceneCamera->GetComponentLocation();

		_backupCameraLoc = camLoc;
		_backupCameraRot = camRot;

		rotationWork->SetTargetRot( rotTo ,camRot);

		_arrayZoomWork.Add(EGsCameraWorkType::RotationWork);

		// 현재 거리는 카메라에서 0번 인덱스 위치라고 하자
		USceneComponent* firstSceneComponent = GetPosComponent(0);
		if (firstSceneComponent == nullptr)
		{
			return;
		}
		FVector firstTargetPos =
			firstSceneComponent->GetComponentLocation();
		float nowCamDist = FVector::Distance(firstTargetPos, camLoc);

		// 현재 바라 보고 있는곳
		FVector nowLookPos = camLoc + nowCamDist * camRot.Vector();

		locationWork->SetData(distCam, nowCamDist,
			lookTargetPos, nowLookPos,
			movePos, camLoc);

		_arrayZoomWork.Add(EGsCameraWorkType::LocationWork);

		float locationSpeed = locationWork->MakeDistSpeed();
		float rotationSpeed = rotationWork->MakeDistSpeed();

		// 둘중 더 느린걸로 하자(싱크 맞추가)
		float slowSpeed = FMath::Min(rotationSpeed, locationSpeed);
		float mulVal = GData()->GetGlobalData()->_summonCameraSpeedOffset;
		slowSpeed *= mulVal;

		rotationWork->SetNowSpeed(slowSpeed);
		locationWork->SetNowSpeed(slowSpeed);

		blurWork->Start();


		// 터치 컬리더 액티브 끈다
		SetStoneTouchCollisionActive(false, In_index);

	
		// 줌상태로 바뀜
		_summonStep = EGsSummonProgressStepType::ZoomStart;

		// 버튼 hide
		if (_traySummon != nullptr)
		{
			_traySummon->SetButtonType(EGsSummonBtnType::HIDE);
		}

		UGsUIBillboardSummonStone* widgetUI = veilObject->GetSummonStoneWidget();
		if (widgetUI == nullptr)
		{
			return;
		}
		// ui 표시 변경
		widgetUI->SetSummonStoneInputType(EGsUISummonStoneInputType::Pull);		

		// zoom sound play
		UGsSoundPlayer* soundPlayer = GSoundPlayer();
		if (nullptr == soundPlayer)
			return;

		soundPlayer->PlaySound2D("Snd_wav_UI_Gacha_ZoomIn");
	}
	else
	{
		OpenUnveilObject(In_index);

		// 남은 소환석이 없다면
		if (_arraySummonVeilObject.Num() == 0)
		{
			// 모든 페어리 열렸을때 처리
			ProcAllFairyOpened();
		}
		else
		{
			// 고등급 소환석만 남았는지 체크후 btn 상태 갱신(open all, show result)
			SetBtnTypeOpenAll();
		}
	}
}
// 페어리 오픈됨
void AGsActorSummonSceneObject::OpenUnveilObject(int In_index, bool In_isShowUnveil)
{
	if (In_isShowUnveil == true)
	{
		ShowUnveilObject(In_index);
		PlayUnveilObjectSpawnAni(In_index);
	}
	// 소환석 디스폰
	DespawnVeilObject(In_index);	
}
// 모든 페어리 열렸을때 처리
void AGsActorSummonSceneObject::ProcAllFairyOpened()
{
	// 마지막 스폰이면 ui 카드 리스트 연다
	// 남은 스톤이 없으면 마지막이겠지
	if (_traySummon.IsValid())
	{
		_traySummon->InitList();

		// 카드 보이는 연출이 끝날때 까지 hide
		_traySummon->SetButtonType(EGsSummonBtnType::HIDE);

		// 모든 페어리 보이는 상태
		_summonStep = EGsSummonProgressStepType::ShowAllFairy;

	}
}

// 소환 시작
void AGsActorSummonSceneObject::StartSummon(EGsSummonType In_summonType)
{
	_summonType = In_summonType;

	// 소환석, 페어리 삭제
	DespawnVeilObjects(true);

	DespawnUnveilObjects();

	// 시작 컨텐츠 타입에 따라 처리
	EGsSummonOriginContentType contentType =
		GSSummon()->GetSummonOriginContent();

	
	bool isSkip = false;
	if (UGsSummonManager* summonManager = GSSummon())
	{
		isSkip = summonManager->IsSummonSkip();
	}


	// 스킵이 아니면 도입부 카메라 연출 플레이
	// 동시에 컨텐츠 타입이 아이템이어야 한다
	if (isSkip == false && contentType == EGsSummonOriginContentType::SUMMON_ITEM)
	{
		FGsSequenceMessageParam param(GData()->GetGlobalData()->_startSummonCameraSequenceId, this, true);
		GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_START, &param);
			
		_isPlaySequence = true;

		// 오프닝 컷신 단계
		_summonStep = EGsSummonProgressStepType::OpenCutscene;
		// 컷신 카메라가 스폰석을 안보여주므로
		// 컷신 끝나는 시점에 스폰 시작해야함

		// bp에 호출
		StartOpenningSequence();
	}
	else
	{
		SummonVeilObjects();
	}
	// 페어리 미리스폰
	// 나중 컷신에서 참조해야하므로
	// 미리 스폰후 hide 하기
	// 컷신끝나고 소환석이랑 동시에 스폰하면 버벅대서
	// 처음에 스폰 해보자
	SummonUnveilObjects();
	

	// ui 카드 순서 정렬 처리
	GSSummon()->SortCard( _summonType,  _arraySpawnPos);

	_highGradeDragStone = nullptr;

	_dragProgressVal = 0.0f;

	_stonePress = false;

	_isTouchPress = false;

	if (_setDragSoundId.Num() != 0)
	{
		_setDragSoundId.Empty();
	}
}

// 무비 끝
void AGsActorSummonSceneObject::OnStopMovie(bool In_isOpeningCutscene)
{
	_isPlaySequence = false;

	// 오프닝이 끝났으니 소환석 모드 스폰
	if (In_isOpeningCutscene == true)
	{
		// 씨퀀스 내부에서 white fade out
		SummonVeilObjects();

		// bp에 호출
		EndOpenningSequence();
	}
	// 고등급 획득 연출이 끝났음
	// 0. 페어리 보여주기
	// 1. 소환석 디스폰
	// 3. 카메라 복구
	// 4. 모든페어리 스폰되었으면 ui open
	else
	{
		ShowSelectUnveilObjectResult();
	}
}

// 선택된 페어리 결과 보기
// 1. OpenFairy(위의 함수 호출)
// 2. 카메라 복구(줌상태에서 원래로)
// 3. 충돌 복구
void AGsActorSummonSceneObject::ShowSelectUnveilObjectResult()
{

	// 시퀀스에서 획득 ui 닫기
	UGsUIManager* uiMgr = GUI();
	if (uiMgr == nullptr)
	{
		return;
	}
	// 획득 ui 닫기
	uiMgr->CloseByKeyName(TEXT("TraySummonObtain"));
	// 대사 ui 닫기(스킵일 경우)
	uiMgr->CloseByKeyName(TEXT("TraySummonDialogue"));
	// info ui 닫기(스킵일 경우)
	uiMgr->CloseByKeyName(TEXT("TraySummonInfo"));


	// 시작 컨텐츠 타입에 따라 처리
	EGsSummonOriginContentType contentType =
		GSSummon()->GetSummonOriginContent();
	
	// 대기 교체는 카메라 이동이 없었으므로 바꿀 필요 없음
	if (contentType != EGsSummonOriginContentType::SUMMON_WAIT_CHANGE)
	{
		_sceneCamera->SetWorldLocation(_backupCameraLoc);
		_sceneCamera->SetWorldRotation(_backupCameraRot);
	}

	// 소환석이 아직남아있다면
	if (_arraySummonVeilObject.Num() != 0)
	{
		SetBtnTypeOpenAll();

		// 모든 소환석 보이는 상태
		_summonStep = EGsSummonProgressStepType::ShowAllStone;

		// 충돌 복구
		SetStoneTouchCollisionActive(true);
	}
	else
	{
		// 모든 소환석 열었을때 처리
		ProcAllFairyOpened();		
	}

	// hide 된거 복구
	SetHideObjects(false);

	// 결과 시퀀스 끝난애 spawn 애니 출력
	PlayUnveilObjectSpawnAni(_selectVeilObjectIndex);

	// 복구
	SetCameraExposure(_backupCameraExposureVal);
}

// 모든 페어리 결과 보기
void AGsActorSummonSceneObject::ShowAllHighGradeUnveilObjectResult()
{
	// 이미 처리중이면 return
	if (_isLockAllVeilShow == true)
	{
		return;
	}

	// 이전께 있으면 비운다
	if (_arraySummonOrderUnveilObjectShow.Num() != 0)
	{
		_arraySummonOrderUnveilObjectShow.Empty();
	}

	// 남은 소환석 돌면서 결과 보기
	for (auto stone: _arraySummonVeilObject)
	{
		_arraySummonOrderUnveilObjectShow.Add(stone->GetDataIndex());
	}

	// 기존께 낮은등급에서 고등급 순이라
	// for에선 거꾸로 돌아서 reverse 한다
	Algo::Reverse(_arraySummonOrderUnveilObjectShow);


	_callbackShowFinishOrderUnveilObject = [this]()
	{
		// 모든 소환석 열었을때 처리
		ProcAllFairyOpened();
	};

	// 할께 없으면 바로 끝
	if (_arraySummonOrderUnveilObjectShow.Num() == 0)
	{
		_callbackShowFinishOrderUnveilObject();
	}
	else
	{
		_isLockAllVeilShow = true;
		ShowUnveilObjectSingle();		

		// 모두 보기는 충돌체 끄기
		SetStoneTouchCollisionActive(false);
	}
}

// 모든 일반등급 페어리 결과 보기
void AGsActorSummonSceneObject::ShowAllNormalGradeUnveilObjectResult()
{
	// 이미 처리중이면 return
	if (_isLockAllVeilShow == true)
	{
		return;
	}
	
	// 이전께 있으면 비운다
	if (_arraySummonOrderUnveilObjectShow.Num() != 0)
	{
		_arraySummonOrderUnveilObjectShow.Empty();
	}

	if (_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		TArray<TSharedPtr<FGsFairyData>> arrayFairyData = GSSummon()->GetSummonFairyData();

		for (int i=0; i< arrayFairyData.Num(); ++i)
		{
			TSharedPtr<FGsFairyData> data = arrayFairyData[i];

			// 1. 데이터 유효하고
			// 2. 래어등급 밑이고
			// 3. veil object map에 있는것(안깐거)
			if (data.IsValid() == true && (int)data->GetFairyGrade() < (int)FairyGrade::RARE
				&& (_mapSummonVeilObject.Contains(i) == true))
			{
				_arraySummonOrderUnveilObjectShow.Add(i);
			}
		}
	}
	else
	{
		TArray<TSharedPtr<FGsCostumeData>> arrayCostumeData = GSSummon()->GetSummonCostumeData();

		for (int i=0; i< arrayCostumeData.Num(); ++i)
		{
			TSharedPtr<FGsCostumeData> data = arrayCostumeData[i];

			// 1. 데이터 유효하고
			// 2. 래어등급 밑이고
			// 3. veil object map에 있는것(안깐거)
			if (data.IsValid() == true && (int)data->GetCostumeGrade() < (int)CostumeGrade::RARE
				&& (_mapSummonVeilObject.Contains(i) == true))
			{
				_arraySummonOrderUnveilObjectShow.Add(i);
			}
		}
	}

	_callbackShowFinishOrderUnveilObject = [this]()
	{
		// 소환석이 없다면
		if (IsVeilObjectExist() == false)
		{
			// 모든 소환석 열었을때 처리
			ProcAllFairyOpened();
		}
		// 아직 있다면(고등급)
		else
		{
			if (_traySummon.IsValid())
			{
				_traySummon->SetButtonType(EGsSummonBtnType::SHOW_RESULT);
			}
		}
	};

	// 없다면
	if (_arraySummonOrderUnveilObjectShow.Num() == 0)
	{
		_callbackShowFinishOrderUnveilObject();
	}
	else
	{
		_isLockAllVeilShow = true;
		ShowUnveilObjectSingle();		

		// 모두 보기는 충돌체 끄기
		SetStoneTouchCollisionActive(false);
	}
}

// 카메라 이동 처리 가져오기
FGsSummonCameraLocation* AGsActorSummonSceneObject::GetCameraLocation()
{
	IGsSummonCameraWork* work = GetCameraWork(EGsCameraWorkType::LocationWork);
	if (work == nullptr)
	{
		return nullptr;
	}
	FGsSummonCameraLocation* locationWork = FGsClassUtil::FCheckStaticCast<FGsSummonCameraLocation>(work);
	return locationWork;
}
// 카메라 회전 처리 가져오기
FGsSummonCameraRotation* AGsActorSummonSceneObject::GetCameraRotation()
{
	IGsSummonCameraWork* work = GetCameraWork(EGsCameraWorkType::RotationWork);
	if (work == nullptr)
	{
		return nullptr;
	}
	FGsSummonCameraRotation* rotationWork = FGsClassUtil::FCheckStaticCast<FGsSummonCameraRotation>(work);
	return rotationWork;
}
// 카메라 블러 처리 가져오기
FGsSummonCameraBlur* AGsActorSummonSceneObject::GetCameraBlur()
{
	IGsSummonCameraWork* work = GetCameraWork(EGsCameraWorkType::BlurWork);
	if (work == nullptr)
	{
		return nullptr;
	}
	FGsSummonCameraBlur* blurWork = FGsClassUtil::FCheckStaticCast<FGsSummonCameraBlur>(work);
	return blurWork;
}

// 카메라 처리 타입 가져오기
IGsSummonCameraWork* AGsActorSummonSceneObject::GetCameraWork(EGsCameraWorkType In_type)
{
	IGsSummonCameraWork* work = _mapCamWorks.FindRef(In_type);
	return work;
}

//  모든 소환석 터치 충돌체 active on, off
// 고등급 연출시 사용(연출 시작때 껐다가 끝나고 다시 킴)
// 고등급은 빼고
void AGsActorSummonSceneObject::SetStoneTouchCollisionActive(bool In_isActive
	, int In_highGradeIndex)
{
	for (auto iter: _arraySummonVeilObject)
	{
		if (iter == nullptr)
		{
			continue;
		}
		// 키는거는 무조건 킴
		if (In_isActive == true)
		{
			iter->SetTouchCollisionEnable(In_isActive);
		}
		// 끄는거는 고등급 id인지 체크 후 처리
		else
		{
			if (iter->GetDataIndex() == In_highGradeIndex)
			{
				iter->SetIsUseHighGradeDrag(true);
			}
			// 고등급이 아니면 끔
			else
			{
				iter->SetTouchCollisionEnable(In_isActive);
			}
		}
	}
}
// 페어리, 소환석 hide 처리
void AGsActorSummonSceneObject::SetHideObjects(bool In_isHide,
	int In_highGradeIndex)
{
	for (AGsActorSummonVeilObject* stone : _arraySummonVeilObject)
	{
		if (stone == nullptr)
		{
			continue;
		}
		
		// 키는거(hide 가 false면 키는거)는 무조건 킴
		if (In_isHide == false)
		{
			stone->SetHide(In_isHide);
		}
		else
		{
			if (stone->GetDataIndex() != In_highGradeIndex)
			{
				stone->SetHide(In_isHide);
			}
		}
		
	}


	for (auto iter : _mapSummonUnveilObject)
	{
		if (iter.Value == nullptr)
		{
			continue;
		}

		// 페어리 키는거는 stone이 없는것만 처리(아직 show 안된 페어리 제외)
		if (In_isHide == false)
		{
			int dataIndex = iter.Value->GetDataIndex();
			if (_mapSummonVeilObject.Contains(dataIndex) == false)
			{
				iter.Value->SetHide(In_isHide);
			}
		}
		else
		{
			iter.Value->SetHide(In_isHide);
		}
	}

}
// 고등급 소환석 터치 시작
void AGsActorSummonSceneObject::OnTouchStartHighGradeSummonStone(AGsActorSummonVeilObject* In_stone)
{
	if (_summonStep != EGsSummonProgressStepType::HighGradeZoom)
	{
		return;
	}


	_highGradeDragStone = In_stone;
	_stonePress = true;	

	// 시간 저장
	_startTimeEffectColorChange = FDateTime::UtcNow().GetTicks();


	// bp use(camera shake start)
	TouchStartHighGradeSummonStone();
}
// 고등급 소환석 터치 끝
void AGsActorSummonSceneObject::OnTouchEndHighGradeSummonStone()
{
	_stonePress = false;

	// bp use(camera shake end)
	TouchEndHighGradeSummonStone();
}

// 터치 땠을때 소환석 돌아가는 처리
void AGsActorSummonSceneObject::UpdateReleseStone(float In_deltaTime)
{
	// 터치 누르고 있거나
	// 진행도가 0이면 처리 안함
	// 줌상태 아니어도 안함
	// not drag complete
	if ((_isTouchPress == true &&
		_stonePress == true )||
		_dragProgressVal == 0.0f ||
		_highGradeDragStone == nullptr||
		_summonStep != EGsSummonProgressStepType::HighGradeZoom ||
		_isDragComplete == true)
	{
		return;
	}
	float changeVal = In_deltaTime *
		GData()->GetGlobalData()->_summonStoneTouchReleaseMulVal;

	float oldVal = _dragProgressVal;
	_dragProgressVal =
		FMath::Max( _dragProgressVal - changeVal , 0.0f);

	_highGradeDragStone->OnDragProgress(_dragProgressVal, oldVal);

	if (_dragProgressVal == 0.0f)
	{
		_highGradeDragStone = nullptr;
	}
}

// 고등급 등장 연출 시작
// 1. 해당 씬 로드
// 2. 컷신 시작
void AGsActorSummonSceneObject::StartHighGradeOpen()
{
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (player == nullptr)
	{
		return;
	}

	// 1. 컷신 시작(내부 씬은 컷신내에서 만듬)


	int sequenceId = 0;
	if (_summonType == EGsSummonType::SUMMON_FAIRY)
	{

		AGsActorSummonFairy* fairy =
			GetSelectHighGradeSummonFairy();

		FairyGrade fairyGrade = FairyGrade::NONE;
		if (fairy != nullptr)
		{
			fairyGrade = fairy->GetGrade();
		}

		if (fairyGrade == FairyGrade::RARE)
		{
			sequenceId = GData()->GetGlobalData()->_summonHighGradeOpenCameraFairyRareSequenceId;
		}
		else
		{
			sequenceId = GData()->GetGlobalData()->_summonHighGradeOpenCameraFairyEpicOverSequenceId;
		}
	}
	else
	{
		AGsActorSummonCostume* costume =
			GetSelectHighGradeSummonCostume();

		CostumeGrade costumeGrade = CostumeGrade::NONE;
		if (costume != nullptr)
		{
			costumeGrade = costume->GetGrade();
		}
		if (costumeGrade == CostumeGrade::RARE)
		{
			sequenceId = GData()->GetGlobalData()->_summonHighGradeOpenCameraCostumeRareSequenceId;
		}
		else
		{
			sequenceId = GData()->GetGlobalData()->_summonHighGradeOpenCameraCostumeEpicOverSequenceId;
		}
	}


	FGsSequenceMessageParam param(sequenceId, this, true);

	GMessage()->GetSystemParam().SendMessage(MessageSystem::SEQUENCE_PLAYER_START, &param);

	// 씨네마틱 자체 내부에서 white FadeIn 연출 시작
	
	_isPlaySequence = true;
}

// 고등급 소환 페어리
AGsActorSummonFairy* AGsActorSummonSceneObject::GetSelectHighGradeSummonFairy()
{
	IGsSummonUnveilObjectInterface* unveilObject =
		_mapSummonUnveilObject.FindRef(_selectVeilObjectIndex);

	if (unveilObject == nullptr)
	{
		return nullptr;
	}
	AGsActorSummonFairy* fairy = static_cast<AGsActorSummonFairy*>(unveilObject);
	return fairy;
}
// 선택된 고등급 소환 코스튬
AGsActorSummonCostume* AGsActorSummonSceneObject::GetSelectHighGradeSummonCostume()
{
	IGsSummonUnveilObjectInterface* unveilObject =
		_mapSummonUnveilObject.FindRef(_selectVeilObjectIndex);

	if (unveilObject == nullptr)
	{
		return nullptr;
	}
	AGsActorSummonCostume* costume = static_cast<AGsActorSummonCostume*>(unveilObject);
	return costume;
}

// 선택된 고등급 소환 코스튬 id(등장 컷신에서 참조)
CostumeId AGsActorSummonSceneObject::GetSelectHighGradeSummonCostumeId()
{
	IGsSummonUnveilObjectInterface* unveilObject =
		_mapSummonUnveilObject.FindRef(_selectVeilObjectIndex);

	if (unveilObject == nullptr)
	{
		return 0;
	}
	AGsActorSummonCostume* costume = static_cast<AGsActorSummonCostume*>(unveilObject);
	
	if (costume == nullptr)
	{
		return 0;
	}

	return costume->GetCostumeId();
}

// 남은 스톤이 모두 고등급인지
bool AGsActorSummonSceneObject::IsAllHighGradeStone()
{
	for (AGsActorSummonVeilObject* stone: _arraySummonVeilObject)
	{
		if (stone == nullptr)
		{
			continue;
		}

		if (stone->IsHighGrade() == false)
		{
			return false;
		}
	}

	return true;
}

// open all로 상태 바꾸기(남은 소환석이 모두 고등급이면 show result로)
void AGsActorSummonSceneObject::SetBtnTypeOpenAll()
{
	if (false == _traySummon.IsValid())
	{
		return;
	}

	EGsSummonBtnType btnType = EGsSummonBtnType::OPEN_ALL;
	if (true == IsAllHighGradeStone())
	{
		btnType = EGsSummonBtnType::SHOW_RESULT;
	}
	_traySummon->SetButtonType(btnType);
	
	if (true == GSSummon()->GetIsTestAutoRetrySummon())
	{
		FGsSummonHandler* summonHandler = UGsSummonManager::GetSummonHandler();
		if (summonHandler == nullptr)
		{
			return;
		}
		if (btnType == EGsSummonBtnType::OPEN_ALL)
		{
			summonHandler->ShowAllUnveilObjects();
		}
		else if (btnType == EGsSummonBtnType::SHOW_RESULT)
		{
			summonHandler->OpenHighGradeStoneNow();
		}
	}
}

// 순차 페어리 보여주는 연출
void AGsActorSummonSceneObject::ShowUnveilObjectSingle()
{
	// 할것이 없으면 return
	if (_arraySummonOrderUnveilObjectShow.Num() == 0)
	{
		return;
	}

	_startTimeSummonUnveilObject = FDateTime::UtcNow().GetTicks();

	int index = _arraySummonOrderUnveilObjectShow.Num() - 1;

	int dataIndex = _arraySummonOrderUnveilObjectShow[index];

	_arraySummonOrderUnveilObjectShow.RemoveAt(index);

	OpenUnveilObject(dataIndex);


	// 처리후 0개만 마지막 도착 콜백 처리
	if (_arraySummonOrderUnveilObjectShow.Num() == 0)
	{
		if (_callbackShowFinishOrderUnveilObject != nullptr)
		{
			_callbackShowFinishOrderUnveilObject();
		}

		_isLockAllVeilShow = false;
	}
}

// 나뭇잎 이펙트 만들기
void AGsActorSummonSceneObject::MakeLeafEffect()
{
	const FSoftObjectPath path = GData()->GetGlobalData()->_summonEffectLeafStartVeilShow;

	if (path.IsValid() == false)
	{
		return;
	}

	if (UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(path))
	{
		if (UParticleSystem* particleSystem = Cast<UParticleSystem>(load))
		{		
			UParticleSystemComponent* particleSystemComponent =
				UGameplayStatics::SpawnEmitterAttached(
					particleSystem,
					_leafFxPos,
					NAME_None,
					FVector::ZeroVector,
					FRotator::ZeroRotator,
					(FVector)(1.0f),
					EAttachLocation::Type::KeepRelativeOffset,
					true,
					EPSCPoolMethod::ManualRelease,
					true);			
		}

	}
}
// 카메라 노출값 세팅
void AGsActorSummonSceneObject::SetCameraExposure(float In_val)
{
	_sceneCamera->PostProcessSettings.AutoExposureBias = In_val;
}

// 카메라 노출값 갱신
void AGsActorSummonSceneObject::UpdateCameraExposure(float In_progressVal)
{
	// 드래그 카메라 노출  커브값 처리
	float dragCameraWhiteCurveVal = 0.0f;
	dragCameraWhiteCurveVal = FMath::Clamp<float>(_dragCameraWhiteCurve->GetFloatValue(In_progressVal), 0.f, 1.f);

	float exposureProgress = FMath::Lerp(_backupCameraExposureVal, _dragCameraExposureMax , dragCameraWhiteCurveVal);

	SetCameraExposure(exposureProgress);
}

// 소환 사운드 믹서 교체후 기본 음악(bgm) 출력
void AGsActorSummonSceneObject::ChangeSoundSummonMixer(bool In_isSummonMode)
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
		if (_summonBaseSoundId == 0)
		{
			_summonBaseSoundId = SoundPlayer->PlaySound2D("Snd_Cue_BGM_Gacha_Fairy");
		}
	}
	else
	{
		SoundPlayer->StopSound(_summonBaseSoundId);
		// 초기화 추가
		_summonBaseSoundId = 0;
	}
	
	
}

// 드래그 사운드 시작
void AGsActorSummonSceneObject::StartDragSound()
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
// 드래그 사운드 끝
void AGsActorSummonSceneObject::EndDragSound()
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

UCameraComponent* AGsActorSummonSceneObject::GetCameraComponent()
{
	return _sceneCamera;
}

// call tray fade effect animation finished
void AGsActorSummonSceneObject::OnFadeEffectAniFinished(const IGsMessageParam*)
{
	if (_isUseFadeEffect == false)
	{
		return;
	}

	_isUseFadeEffect = false;

	if (_summonType == EGsSummonType::SUMMON_FAIRY)
	{
		SummonFairyStones();
	}
	else
	{
		SummonVeilCostumes();
	}

	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("TrayFadeEffect"));
	if (widget.IsValid())
	{
		if (UGsUITrayFadeEffect* fadeEffect = Cast<UGsUITrayFadeEffect>(widget.Get()))
		{
			fadeEffect->Close();			
		}
	}
}