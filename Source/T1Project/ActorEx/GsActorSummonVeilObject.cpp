#include "GsActorSummonVeilObject.h"

#include "Management/ScopeGlobal/GsPathManager.h"

#include "Management/ScopeGame/GsSummonManager.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "UTIL/GsAnimation.h"

#include "UI/UIContent/Billboard/GsUIBillboardSummonStone.h"

#include "Components/WidgetComponent.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/Components/SceneComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Engine/Classes/Particles/ParticleSystem.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Classes/Curves/CurveFloat.h"
#include "Engine/Classes/GameFramework/RotatingMovementComponent.h"
#include "Engine/AssetManager.h"

#include "Classes/Animation/AnimInstance.h"
#include "Classes/Animation/AnimMontage.h"

#include "Core/Public/Misc/DateTime.h"
#include "GsActorSummonFairyStone.h"

const FName NameSummonVeilObjectRootComponent = TEXT("Root");
const FName NameSummonVeilObjectMeshComponent = TEXT("Mesh");
const FName NameSummonVeilObjectCapsuleComponent = TEXT("Capsule");
const FName NameSummonVeilObjectColliderPreset = TEXT("SummonStone");
const FName NameSummonVeilObjectClickWidget = TEXT("ClickWidget");
const FName NameSummonVeilObjectDragParticle = TEXT("DragParticle");
const FName NameSummonVeilObjectRotatingMovementComponent = TEXT("RotMovementComponent");

const FName NameDragParticleParamSize = TEXT("Size");
const FName NameDragParticleParamColor = TEXT("Color");
const FName NameDragParticleParamVelocity = TEXT("Velocity_val");
const FName NameDragParticleParamSpawn = TEXT("Spawn");
const FName NameDragParticleParamLaySize = TEXT("LaySize");
const FName NameDragParticleParamElecRadius = TEXT("ElecRadius");
const FName NameDragParticleParamElecSpawn = TEXT("ElecSpawn");

const FName NameOffsetIntensity = TEXT("Offset Intensity");

AGsActorSummonVeilObject::AGsActorSummonVeilObject()
	:Super()
{

	RootComponent = CreateDefaultSubobject<USceneComponent>(NameSummonVeilObjectRootComponent);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(NameSummonVeilObjectMeshComponent);
	Mesh->SetupAttachment(RootComponent);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(NameSummonVeilObjectCapsuleComponent);
	CapsuleComponent->InitCapsuleSize(34.0f, 88.0f);
	CapsuleComponent->SetCollisionProfileName(NameSummonVeilObjectColliderPreset);

	CapsuleComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
	CapsuleComponent->SetCanEverAffectNavigation(false);
	CapsuleComponent->bDynamicObstacle = true;
	CapsuleComponent->SetupAttachment(Mesh);

	CapsuleComponent->OnInputTouchBegin.AddDynamic(this,
		&AGsActorSummonFairyStone::OnStoneTouchBegin);
	//CapsuleComponent->OnInputTouchEnd.AddDynamic(this,
	//	&AGsActorSummonFairyStone::OnStoneTouchEnd);

	_clickWidget = CreateDefaultSubobject<UWidgetComponent>(NameSummonVeilObjectClickWidget);
	_clickWidget->SetRelativeLocation(FVector::ZeroVector);
	_clickWidget->SetWidgetSpace(EWidgetSpace::Screen);
	_clickWidget->SetDrawSize(FVector2D(100.0f, 100.0f));
	_clickWidget->SetCastShadow(false);
	_clickWidget->SetupAttachment(RootComponent);

	_particleDrag = CreateDefaultSubobject<UParticleSystemComponent>(NameSummonVeilObjectDragParticle);
	_particleDrag->SetupAttachment(Mesh);


	_rotMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(NameSummonVeilObjectRotatingMovementComponent);
}

void AGsActorSummonVeilObject::BeginPlay()
{
	if (_clickWidget)
	{
		if (nullptr == _clickWidget->GetWidgetClass().Get())
		{
			if (UGsPathManager* pathMgr = GPath())
			{
				UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("SummonStoneWidget"));
				if (widgetClass)
				{
					_clickWidget->SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
				}
			}
		}
		_clickWidget->SetRelativeLocation(GData()->GetGlobalData()->_clickWigetOffset);
	}

	float scale = GetObjectScaleValue();

	if (scale > 0.f)
	{
		SetActorRelativeScale3D(FVector(scale, scale, scale));
	}

	// drag morph target max rate 저장
	_dragMorphTargetMaxRate = GData()->GetGlobalData()->_summonDragMorphTargetMaxRate;

	_isSuccessDragMorphTargetFinish = false;

	_dragCurve = GData()->GetSummonDragCurve();
	_dragFinishCurve = GData()->GetSummonDragFinishCurve();

	_successDragSpeed = GData()->GetGlobalData()->_summonDragMorphTargetSpeed;

	// set default materail value
	Mesh->SetScalarParameterValueOnMaterials(NameOffsetIntensity, 0.0f);


	_rotMinVal = GData()->GetGlobalData()->_summonDragRotMinVal;
	_rotMaxVal = GData()->GetGlobalData()->_summonDragRotMaxVal;

	float nowRotRate = FMath::Lerp(_rotMinVal, _rotMaxVal, 0.0f);

	_rotMovementComponent->RotationRate.Yaw = nowRotRate;

	Super::BeginPlay();
}

// 소환석 클릭
void AGsActorSummonVeilObject::OnClickStone()
{
	if (_callbackClick != nullptr)
	{
		_callbackClick(_dataIndex);
	}
}

// 자체 클릭 이벤트
void AGsActorSummonVeilObject::OnStoneTouchBegin(ETouchIndex::Type In_type, UPrimitiveComponent* In_component)
{
	OnStoneTouchBegin();
}

// 터치 끝
// not use player Controller 2368  CurrentComponent == null when focus out release
//void AGsActorSummonVeilObject::OnStoneTouchEnd(ETouchIndex::Type In_type, UPrimitiveComponent* In_component)
//{
//	OnStoneTouchEnd();
//}
void AGsActorSummonVeilObject::OnStoneTouchBegin()
{
	if (_isUseTouch == false)
	{
		return;
	}

	if (_isUseHighGradeDrag == false)
	{
		if (_callbackClick != nullptr)
		{
			_callbackClick(_dataIndex);
		}
	}
	else
	{
		if (_callbackDragTouchStart != nullptr)
		{
			_callbackDragTouchStart(this);
		}

		ActivateDragParticle(true);
	}
}
void AGsActorSummonVeilObject::OnStoneTouchEnd()
{
	if (_isUseHighGradeDrag == true)
	{
		//if (_callbackDragTouchEnd != nullptr)
		//{
		//	_callbackDragTouchEnd();
		//}

		ActivateDragParticle(false);
	}
}
void AGsActorSummonVeilObject::Finalize()
{
	ReleaseGradeParticle();
}

// 파티클 해제
void AGsActorSummonVeilObject::ReleaseGradeParticle()
{
	if (_particleGrade == nullptr)
	{
		return;
	}

	_particleGrade->ReleaseToPool();
	_particleGrade = nullptr;
	//_particleGrade = nullptr;
}

// 드래그 값 변경
void AGsActorSummonVeilObject::OnDragProgress(float In_progressVal, float In_preProgressVal)
{
	if (_dragCurve == nullptr)
	{
		return;
	}

	// 진행도 커브값 처리
	float dragCurveVal = 0.0f;
	dragCurveVal = FMath::Clamp<float>(_dragCurve->GetFloatValue(In_progressVal), 0.f, 1.f);

	// 모프 타겟의 25%까지만 드래그 연출에 쓴다
	// 나머지는 도달후 연출
	float dragProgress = dragCurveVal * _dragMorphTargetMaxRate;
	
	// 모프 타겟 처리
	FName morphTargetName = GetMorphTargetName();
	Mesh->SetMorphTarget(morphTargetName, dragProgress);

	_currentMorphTarget = dragProgress;

	if (_particleDrag == nullptr)
	{
		return;
	}

	for (auto iter : _particleDrag->InstanceParameters)
	{
		// size 처리
		if (iter.Name.Compare(NameDragParticleParamSize) == 0)
		{
			FVector currentVecLow = iter.Vector_Low;
			FVector currentVec = iter.Vector;

			currentVecLow.Y = FMath::Lerp(
				GData()->GetGlobalData()->_summonDragParamSizeVectorYLowMin,
				GData()->GetGlobalData()->_summonDragParamSizeVectorYLowMax,
				In_progressVal);

			currentVec.Y = FMath::Lerp(
				GData()->GetGlobalData()->_summonDragParamSizeVectorYMin,
				GData()->GetGlobalData()->_summonDragParamSizeVectorYMax,
				In_progressVal); 

			_particleDrag->SetVectorRandParameter(NameDragParticleParamSize,
				currentVec, currentVecLow);
		}
		// velocity 처리
		else if (iter.Name.Compare(NameDragParticleParamVelocity) == 0)
		{
			float scalarVal = FMath::Lerp(
				GData()->GetGlobalData()->_summonDragParamVelocityValScalarMin,
				GData()->GetGlobalData()->_summonDragParamVelocityValScalarMax,
				In_progressVal);

			float scalarLowVal = FMath::Lerp(
				GData()->GetGlobalData()->_summonDragParamVelocityValScalarLowMin,
				GData()->GetGlobalData()->_summonDragParamVelocityValScalarLowMax,
				In_progressVal);

			_particleDrag->SetFloatRandParameter(NameDragParticleParamVelocity,
				scalarVal, scalarLowVal);
		}
		// spawn 처리
		else if (iter.Name.Compare(NameDragParticleParamSpawn) == 0)
		{
			float scalarVal = FMath::Lerp(
					GData()->GetGlobalData()->_summonDragParamSpawnScalarMin,
					GData()->GetGlobalData()->_summonDragParamSpawnScalarMax,
					In_progressVal);

			_particleDrag->SetFloatParameter(NameDragParticleParamSpawn, scalarVal);
		}
		// Lay 처리
		else if (iter.Name.Compare(NameDragParticleParamLaySize) == 0)
		{
			float scalarVal = FMath::Lerp(
				GData()->GetGlobalData()->_summonDragParamLaySizeScalarMin,
				GData()->GetGlobalData()->_summonDragParamLaySizeScalarMax,
				In_progressVal);

			_particleDrag->SetFloatParameter(NameDragParticleParamLaySize, scalarVal);
		}
		// ElecRaidus 처리
		else if (iter.Name.Compare(NameDragParticleParamElecRadius) == 0)
		{
			float scalarVal = FMath::Lerp(
				GData()->GetGlobalData()->_summonDragParamElecRadiusScalarMin,
				GData()->GetGlobalData()->_summonDragParamElecRadiusScalarMax,
				In_progressVal);

			_particleDrag->SetFloatParameter(NameDragParticleParamElecRadius, scalarVal);
		}
		// ElecSpawn 처리
		else if (iter.Name.Compare(NameDragParticleParamElecSpawn) == 0)
		{
			float scalarVal = FMath::Lerp(
				GData()->GetGlobalData()->_summonDragParamElecSpawnScalarMin,
				GData()->GetGlobalData()->_summonDragParamElecSpawnScalarMax,
				In_progressVal);

			_particleDrag->SetFloatParameter(NameDragParticleParamElecSpawn, scalarVal);
		}
	}
	

	// change materail value
	Mesh->SetScalarParameterValueOnMaterials(NameOffsetIntensity, In_progressVal);

	float nowRotRate = FMath::Lerp(_rotMinVal, _rotMaxVal, In_progressVal);

	_rotMovementComponent->RotationRate.Yaw = nowRotRate;
}

// 몽타주 드래그 색션으로 이동
void AGsActorSummonVeilObject::MoveMontageDragSection()
{
	UAnimInstance* AnimInstance = (Mesh) ? Mesh->GetAnimInstance() : nullptr;
	if (AnimInstance == nullptr)
	{
		return;
	}

	UAnimMontage* animMontage = GetCurrentMontage();

	if (animMontage == nullptr)
	{
		return;
	}

	AnimInstance->Montage_Pause(animMontage);
}

// 터치 충돌체 사용 할지
void AGsActorSummonVeilObject::SetTouchCollisionEnable(bool In_isActive)
{
	if (CapsuleComponent == nullptr)
	{
		return;
	}

	CapsuleComponent->SetCollisionEnabled((In_isActive == true) ?
		ECollisionEnabled::QueryOnly :
		ECollisionEnabled::NoCollision);

}

// 드래그 사용하는지 세팅
void AGsActorSummonVeilObject::SetIsUseHighGradeDrag(bool In_isUse)
{
	_isUseHighGradeDrag = In_isUse;

	if (In_isUse == true)
	{
		MoveMontageDragSection();
	}
}
// 몽타주 접근
UAnimMontage* AGsActorSummonVeilObject::GetCurrentMontage()
{
	UAnimInstance* AnimInstance = (Mesh) ? Mesh->GetAnimInstance() : nullptr;
	if (AnimInstance)
	{
		return AnimInstance->GetCurrentActiveMontage();
	}

	return nullptr;
}

// 소환석 위젯(ui) 접근
UGsUIBillboardSummonStone* AGsActorSummonVeilObject::GetSummonStoneWidget()
{
	if (_clickWidget)
	{
		return Cast<UGsUIBillboardSummonStone>(_clickWidget->GetUserWidgetObject());
	}
	return nullptr;
}
// hide 처리
void AGsActorSummonVeilObject::SetHide(bool In_isHide)
{
	if (_particleGrade == nullptr)
	{
		return;
	}
	_particleGrade->SetHiddenInGame(In_isHide);

	SetActorHiddenInGame(In_isHide);
}

// 등급 파티클 붙이기
void AGsActorSummonVeilObject::AttachGradeParticle()
{
	const FSoftObjectPath* path = GetGradeParticlePath();

	if (path == nullptr ||
		path->IsValid() == false)
	{
		return;
	}

	if (UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(*path))
	{
		FVector effectOffset = GetGradeEffectOffset();

		if (UParticleSystem* particleSystem = Cast<UParticleSystem>(load))
		{
			USceneComponent* targetcomponent = GetMesh();

			UParticleSystemComponent* particleSystemComponent =
				UGameplayStatics::SpawnEmitterAttached(
					particleSystem,
					targetcomponent,
					NAME_None,
					effectOffset,
					FRotator::ZeroRotator,
					(FVector)(1.0f),
					EAttachLocation::Type::KeepRelativeOffset,
					true,
					EPSCPoolMethod::ManualRelease,
					true);

			SetGradeParticle(particleSystemComponent);
		}

	}
}

// 드래그 파티클 액티브
void AGsActorSummonVeilObject::ActivateDragParticle(bool In_active)
{
	if (In_active == true)
	{
		_particleDrag->Activate(true);

		FParticleSysParam* findData =
			_particleDrag->InstanceParameters.FindByPredicate([](FParticleSysParam param)
		{
			return param.Name.Compare(NameDragParticleParamColor) == 0;
		});
		_isExistColorChangeParam =
			(findData == nullptr) ? false : true;


		// 파티클 색상 변경
		ChangeEffectColor();
	}
	else
	{
		_particleDrag->DeactivateImmediate();
	}
}

// 이펙트 칼라 변경
void AGsActorSummonVeilObject::ChangeEffectColor()
{
	if (_isExistColorChangeParam == false)
	{
		return;
	}


	FLinearColor nowColor =
		GSSummon()->GetRandomGradeColor();

	_particleDrag->SetColorParameter(NameDragParticleParamColor, nowColor);
}
// 드래그 성공 모프  시작
void AGsActorSummonVeilObject::StartSuccessDragMorphTarget()
{
	_successDragProgressVal = 0.0f;
	_startSuccessDragMorphTarget = _currentMorphTarget;
	_endSuccessDragMorphTarget = 1.0f;
	_isSuccessDragMorphTargetFinish = false;

	// bp에 연출 노티 처리
	SuccessDragHighVeilObject();

	_isUseTouch = false;
}

// 드래그 성공 모프  시작
void AGsActorSummonVeilObject::UpdateSuccessDragMorphTarget(float In_deltaTime)
{
	if (_dragFinishCurve == nullptr)
	{
		return;
	}

	float addRate = In_deltaTime * _successDragSpeed;

	_successDragProgressVal = FMath::Clamp<float>(_successDragProgressVal + addRate, 0.f, 1.f);

	// 모프 타겟 처리
	FName morphTargetName = GetMorphTargetName();

	// 드래그 종료  커브값 처리
	float dragFinishCurveVal = 0.0f;	
	dragFinishCurveVal = FMath::Clamp<float>(_dragFinishCurve->GetFloatValue(_successDragProgressVal), 0.f, 1.f);	
	float morphProgress = FMath::Lerp(_startSuccessDragMorphTarget, _endSuccessDragMorphTarget, dragFinishCurveVal);

	Mesh->SetMorphTarget(morphTargetName, morphProgress);

	if (_successDragProgressVal >= 1.0f)
	{
		_isSuccessDragMorphTargetFinish = true;
	}

}

// 위젯 hide 처리
void AGsActorSummonVeilObject::SetHideWidget(bool In_isHide)
{
	_clickWidget->SetHiddenInGame(In_isHide);
}