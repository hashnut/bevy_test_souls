// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCharacterBase.h"
#include "CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine/Classes/Engine/World.h"
#include "Engine/Public/TimerManager.h"
#include "Engine/Classes/Curves/CurveFloat.h"
#include "Engine/Classes/Materials/Material.h"
#include "Engine/Classes/Materials/MaterialInstance.h"
#include "Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Components/AudioComponent.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Engine/Classes/Animation/AnimMontage.h"
#include "Engine/Classes/Animation/AnimBlueprint.h"
#include "Engine/Classes/Animation/AnimCompositeBase.h"
#include "Engine/Classes/Animation/AnimNotifies/AnimNotify_PlayParticleEffect.h"
#include "Engine/Classes/Animation/AnimNotifies/AnimNotify_PlaySound.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/Particles/WorldPSCPool.h"
#include "Runtime/Core/Public/Math/UnrealMathUtility.h"
#include "Engine/Classes/Engine/EngineTypes.h"

#include "Runtime/Engine/Classes/Engine/AssetManager.h"
#include "Curves/CurveVector.h"
#include "Components/MeshComponent.h"
#include "Components/WidgetComponent.h"

#include "DrawDebugHelpers.h"

#include "Animation/AnimNotify/EGsSkeletalMeshMaterialParam.h"

#include "Animation/GsAnimInstanceState.h"

#include "ActorComponentEx/GsHitEffectComponent.h"
#include "ActorComponentEx/GsCharacterMovementComponent.h"
#include "ActorComponentEx/GsMeshShaderControlComponent.h"
#include "ActorComponentEx/GsSkillGuideEffectComponent.h"
#include "ActorComponentEx/GsQueryFloorHeightComponent.h"
#include "ActorComponentEx/GsHommingProjectileComponent.h"
#include "ActorComponentEx/GsEffectComponent.h"
#include "ActorComponentEx/GsWidgetComponentPooling.h"

#include "UTIL/GsDebugUtil.h"
#include "UTIL/GsLevelUtil.h"
#include "UTIL/GsTimeUtil.h"

#include "ActorComponentEx/EGsMeshShaderEffect.h"

#include "GameMode/GsGameModeTestClient.h"
#include "Cheat/GsCheatManager.h"

#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "UI/UILib/Base/GsUIBillboard.h"

#include "GameObject/Define/GsGameObjectText.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/Data/GsHitEffectData.h"
#include "GameObject/ObjectClass/Data/GsGameObjectMaterialData.h"

#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Shared/Client/SharedEnums/SharedSkillNotifyEnum.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"
#include "DataSchema/Skill/HitEffect/GsSchemaExtraTypeEffectSet.h"
#include "DataSchema/Skill/HitEffect/GsSchemaHealByTypeEffectSet.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "GameObject/Define/HitEffectIgnoreType.h"
#include "GameObject/ObjectClass/Data/GsMaterialParamEx.h"

#include "Blueprint/UserWidget.h"

#if WITH_EDITOR
#include "Kismet/GameplayStatics.h"
#endif
#include "ForEach.h"
#include "Copy.h"

// Sets default values

AGsCharacterBase::AGsCharacterBase(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGsCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.	
	PrimaryActorTick.bCanEverTick = true;

	// 언리얼 네트워크 처리 사용 안함	
	SetRemoteRoleForBackwardsCompat(ROLE_None);
	SetReplicatingMovement(false);

	USkeletalMeshComponent* mesh = GetMesh();
	if (nullptr != mesh)
	{
		mesh->bApplyImpulseOnDamage = false;

		// 파츠 처리관련 컴퍼넌트 
		_partsSkeletalComponentFACE = CreateDefaultSubobject<USkeletalMeshComponent>(PartsMeshComponentName[0]);
		_partsSkeletalComponentFACE->SetupAttachment(mesh);

		_partsSkeletalComponentFACE->SetMasterPoseComponent(mesh);
	}	

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (ensure(MoveComp))
	{
		MoveComp->bEnablePhysicsInteraction = false;
	}

	_queryHeight = CreateDefaultSubobject<UGsQueryFloorHeightComponent>(QueryFloorHeightComponentName);

	if (mesh)
	{
		// Add WidgetComponent
		_nameplateWidget = CreateDefaultSubobject<UGsWidgetComponentPooling>(NameplateWidgetComponentName);
		_nameplateWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
		_nameplateWidget->SetWidgetSpace(EWidgetSpace::Screen);
		_nameplateWidget->SetDrawSize(FVector2D(200, 50.0f));
		_nameplateWidget->SetCastShadow(false);
		_nameplateWidget->SetupAttachment(mesh);
		_nameplateWidget->SetGenerateOverlapEvents(false);	
		
		//_lockOnWidget = CreateDefaultSubobject<UWidgetComponent>(LockOnWidgetComponentName);
		//_lockOnWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
		//_lockOnWidget->SetWidgetSpace(EWidgetSpace::Screen);
		//_lockOnWidget->SetDrawSize(FVector2D(100, 100));
		//_lockOnWidget->SetCastShadow(false);
		//_lockOnWidget->SetupAttachment(mesh);

		_partySharedTargetWidget = CreateDefaultSubobject<UWidgetComponent>(LockOnWidgetComponentName);
		_partySharedTargetWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
		_partySharedTargetWidget->SetWidgetSpace(EWidgetSpace::Screen);
		_partySharedTargetWidget->SetDrawSize(FVector2D(100, 100));
		_partySharedTargetWidget->SetCastShadow(false);
		_partySharedTargetWidget->SetupAttachment(mesh);
		_partySharedTargetWidget->SetGenerateOverlapEvents(false);
	}

	// GarbageCollection 관련 설정
	// 활성화시키면, Actor Clustering에 병합되며 레벨 언로드시 모두 파괴된다
	//bCanBeInCluster = true;	

	_hitEffect = CreateDefaultSubobject<UGsHitEffectComponent>(NameHitEffectComponent);
	_meshShaderComponent = CreateDefaultSubobject<UGsMeshShaderControlComponent>(MeshShaderComponentName);
	_hommingProjectile = CreateDefaultSubobject<UGsHommingProjectileComponent>(HommingProjectileComponentName);
	_effectComponent = CreateDefaultSubobject<UGsEffectComponent>(NameEffectComponent);	
}

void AGsCharacterBase::OnConstruction(const FTransform& in_trs)
{
	Super::OnConstruction(in_trs);	
	SetLastLocation(in_trs.GetLocation());
}

void AGsCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (_ignoreNameplateWidget)
	{
		_nameplateWidget->DestroyComponent();
		_nameplateWidget = nullptr;
	}

	_guideEffectComponent = FindComponentByClass<UGsSkillGuideEffectComponent>();
}

// Called when the game starts or when spawned
void AGsCharacterBase::BeginPlay()
{
	InitWidgetComponent();

	Super::BeginPlay();

	PostWidgetComponentMakeWidget();

	if (USkeletalMeshComponent* mesh = GetMesh())
	{
		_originMeshRotation = mesh->GetRelativeRotation();
	}
	if (UCapsuleComponent* capsule = GetCapsuleComponent())
	{
		_oringinCollisionType = capsule->GetCollisionProfileName();
	}

	if (FMath::IsNearlyZero(_smoothRotationInfo._rotateSpeed ))
	{
		if (GData())
		{
			if (const UGsGlobalConstant* globalConstData = GData()->GetGlobalData())
			{
				_smoothRotationInfo = globalConstData->_smoothRotationInfo;
			}
		}
	}
}

void AGsCharacterBase::EndPlay(const EEndPlayReason::Type in_type)
{
	RestoreAllMaterials();
	RestoreMesh();

	if (nullptr != _hitMotionTimer)
	{
		_hitMotionTimer->Clear();
		_hitMotionTimer = nullptr;
	}

	if (nullptr != _hitAmplitudeTimer)
	{
		_hitAmplitudeTimer->Clear();
		_hitAmplitudeTimer = nullptr;
	}

	Super::EndPlay(in_type);
}

void AGsCharacterBase::FinishDestroy()
{
	OnActorDelete.Broadcast(this);

	Super::FinishDestroy();
}

// Called every frame
void AGsCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ProcessHitMotion(DeltaTime);
	ProcessAmplitude(DeltaTime);
	ProcessSmoothRotaiton(DeltaTime);
	ProcessMaterialParams(DeltaTime);

	if (GetCharacterMovement())
	{		
		if (EMovementMode::MOVE_Falling == GetCharacterMovement()->MovementMode)
		{
			//RelocationCharacter();												

			//GSLOG(Warning, TEXT("AGsCharacterBase::Tick - falling - name : %s"), *GetName());

#if SPAWN_LOCATION_DEBUG && WITH_EDITOR
			if (UGsCheatManager::IsShowSpawnLocation)
			{
				if (false == _isShowLogOnce)
				{
					_isShowLogOnce = true;

					DrawDebugString(GetWorld()
						, _spawnLog.clientSpawnLocation
						, FString::Printf(TEXT("actor name : %s\nhit actor : %s\nclient capsule size : %f\nreal capsule size%f")
						, *GetName()
						, *_spawnLog.hitActorName
						, _spawnLog.capsuleSize
						, GetCapsuleComponent()->GetScaledCapsuleHalfHeight())
						, nullptr
						, FColor::Black
						, 180.0f);

					DrawDebugSphere(GetWorld()
						, _spawnLog.serverSpawnLocation
						, 10
						, 8
						, FColor::Blue
						, false
						, 180.0f);

					DrawDebugSphere(GetWorld()
						, _spawnLog.clientSpawnLocation
						, 15
						, 8
						, FColor::Yellow
						, false
						, 180.0f);

					DrawDebugSphere(GetWorld()
						, _spawnLog.hitActorLocation
						, 20
						, 8
						, FColor::Red
						, false
						, 180.0f);

					DrawDebugLine(GetWorld()
						, FVector(_spawnLog.serverSpawnLocation.X, _spawnLog.serverSpawnLocation.Y, _spawnLog.serverSpawnLocation.Z + _spawnLog.capsuleSize)
						, FVector(_spawnLog.serverSpawnLocation.X, _spawnLog.serverSpawnLocation.Y, -TNumericLimits<int32>::Max())
						, FColor::Red, true, 180);		
				}
			}	
#endif
		}
		else
		{
			_lastLocation = GetActorLocation();
		}
	}

#if WITH_EDITOR
	_queryHeight->DrawCharacterLocation();
#endif

	// 이하 네임플레이트 컬링 로직
	bool bIsRendered = true;
	if (const UWorld* const World = GetWorld())
	{
		float renderTime = GetLastRenderTimeOnScreenMesh();

		// renderTime가 -1000 값이 나올 때가 있어서 체크 
		if (0 < renderTime)
		{
			// 깜빡임 현상 방지를 위한 0.2초 딜레이 체크 (WasRecentlyRendered 로직 참고)
			const float RenderTimeThreshold = FMath::Max(0.2f, DeltaTime + KINDA_SMALL_NUMBER);
			float diff = World->GetTimeSeconds() - renderTime;
			bIsRendered = (diff <= RenderTimeThreshold) ? true : false;

			// 프레임 드랍 시 깜빡임 현상 방지를 위해 이전 상태를 유지시킴
			if (false == bIsRendered)
			{
				if (0.2f < DeltaTime)
				{	
					bIsRendered = _bWasRendered;
				}
			}
		}
		else
		{
			// 값이 이상할 경우에도 이전 상태 유지
			bIsRendered = _bWasRendered;
		}
	}

	if (_bWasRendered != bIsRendered)
	{
		OnChangeRenderState(bIsRendered);
	}

	_bWasRendered = bIsRendered;
}

float AGsCharacterBase::PlayAnimMontage(UAnimMontage* AnimMontage, float InPlayRate, FName StartSectionName)
{
	// 메시 오버라이드(IsMeshChanged) 가 발생된 경우엔 기존 메시의 몽타주 애니메이션 플레이를 막는다.
	if (AnimMontage && GetMesh())
	{
		if (USkeletalMesh* SkeletalMesh = GetMesh()->SkeletalMesh)
		{
			if (SkeletalMesh->GetSkeleton() == AnimMontage->GetSkeleton() && IsMeshChanged())
			{
				return 0.f;
			}
		}
	}

	return Super::PlayAnimMontage(AnimMontage, InPlayRate, StartSectionName);
}

// Main 스켈레탈 메쉬와 그 자식들이 가지는 PrimiviteComponent들의 필요 셋업
void AGsCharacterBase::SetupPrimitives(bool drawXRayVision)
{
	SetupPrimitives(this, drawXRayVision);
}

void AGsCharacterBase::SetupPrimitives(AActor* Actor, bool drawXRayVision)
{
	if (Actor)
	{
		TInlineComponentArray<UPrimitiveComponent*> ComponentArray;
		Actor->GetComponents(ComponentArray);
		TArray<USceneComponent*> childComponents;
		for (UPrimitiveComponent* Component : ComponentArray)
		{
			Component->GetChildrenComponents(true, childComponents);
			for (USceneComponent* childComponent : childComponents)
			{
				if (UPrimitiveComponent* casted = Cast<UPrimitiveComponent>(childComponent))
				{
					// 캐릭터 위에 데칼이 그려지지 않도록
					casted->bReceivesDecals = false;
				}
			}

			// 캐릭터 위에 데칼이 그려지지 않도록
			Component->bReceivesDecals = false;
		}
	}
}

void AGsCharacterBase::OnHit(HitResultType ResultType, 	EGsStateBase CurrentState,
	const FGsSchemaCreatureCommonInfo* CommonData)
{
	if (ResultType != HitResultType::HEAL)
	{
		// 피격 공통 처리
		// 히트 프레넬 효과 적용
		if (_materialInfo._currentMaterialData == nullptr)
		{
			if (IsHitFresnel())
			{
				if (CommonData)
				{
					StartMeshShader(EGsMeshShaderEffect::Fresnel, CommonData);
				}
				else
				{
					StartMeshShader(EGsMeshShaderEffect::Fresnel);
				}

			}
		}

		// 히트 모션 출력
		DoHitMotion(CurrentState);

		// 히트 진폭 출력
		DoHitAplitude();

		// 오브젝트 자체의 Hit Sound 출력
		// 기존 Hit애니메이션에 Notify로 존재하던 사운드
		if (CommonData 
		&& CommonData->hitAnimationSoundData.GetRow())
		{
			const FGsSchemaSoundResData* soundData = CommonData->hitAnimationSoundData.GetRow();
			// 사운드 재생
			if (soundData)
			{
				if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
				{
					soundPlayer->PlaySoundTarget(soundData, GetRootComponent());
				}
			}
		}		
	}	
}

void AGsCharacterBase::OnHitEffect(
	const FVector& attackerLocation,
	HitResultType ResultType,	
	const FGsSchemaSkillSet* SkillData,
	const FGsSchemaSkillNotifySet* SkillNotifyData,
	const FGsSchemaPassivitySet* PassivityData,
	const FGsSchemaProjectileSet* ProjectileData,
	const FGsSchemaExtraTypeEffectSet* DamageTypeEffectSet,
	const FGsSchemaHealByTypeEffectSet* HealTypeEffectSet,
	bool IsAttackerHeightFixed)
{
	// 히트 이펙트 출력
	if (IsHitEffect())
	{
		UGsHitEffectData* hitEffectData = nullptr;
		UGsHitEffectData* additiveHitEffectData = nullptr;
		const FGsSchemaSoundResData* hitSoundData = nullptr;
		CreatureWeaponType weaponType = CreatureWeaponType::MAX;

		// 각각 상황에 맞는 HitEffect 정보 얻기
		ResultHitEffectInfo(hitEffectData, additiveHitEffectData, hitSoundData, weaponType,
			SkillData, SkillNotifyData, PassivityData, ProjectileData, DamageTypeEffectSet, HealTypeEffectSet);

		if (UGsHitEffectComponent* hitComp = GetHitEffect())
		{
			bool isEmpty = true;
			if (hitEffectData)
			{
				hitComp->OnHitEffect(attackerLocation, hitEffectData, IsAttackerHeightFixed);
				isEmpty = false;
			}
			if (additiveHitEffectData)
			{
				hitComp->OnHitEffect(attackerLocation, additiveHitEffectData, IsAttackerHeightFixed);
				isEmpty = false;
			}

			// 아무것도 없을때만 CommonInfo, GlobalConst 적용
			if (isEmpty)
			{
				hitComp->OnHitEffect(attackerLocation, nullptr, IsAttackerHeightFixed);
			}

			hitComp->OnHitSound(hitSoundData, (ResultType == HitResultType::CRITICAL), weaponType);
		}
	}
}

class UGsUIBillboardCreature* AGsCharacterBase::GetWidget()
{
	if (_nameplateWidget)
	{
		if (UUserWidget* widget = _nameplateWidget->GetUserWidgetObject())
		{
			if (widget->IsValidLowLevel())
			{
				return Cast<UGsUIBillboardCreature>(widget);
			}
		}
	}

	return nullptr;
}

EGsAnimStateBlendType AGsCharacterBase::DecideBlendTypebyAnimState(EGsStateBase currentState)
{
	EGsAnimStateBlendType result;
	switch (currentState)
	{
	case EGsStateBase::Idle:
		result = EGsAnimStateBlendType::BodyFull;
		break;
	case EGsStateBase::ForwardWalk:
	case EGsStateBase::Run:
	case EGsStateBase::AutoMove:
		result = EGsAnimStateBlendType::BodyUpper;
		break;
	case EGsStateBase::ChangeWeapon:
	case EGsStateBase::ChangeIdle:
	case EGsStateBase::Casting:
	case EGsStateBase::Attack:
	case EGsStateBase::CommonAction:
	case EGsStateBase::Social:
	case EGsStateBase::SocialAction:
	case EGsStateBase::Interaction:
	case EGsStateBase::Looting:
		result = EGsAnimStateBlendType::BodyHead;
		break;
	default:
		result = EGsAnimStateBlendType::BodyNone;
		break;
	}
	return result;
}

void AGsCharacterBase::DoHitMotion(EGsStateBase inCurrentState)
{
	if (nullptr == _hitMotionInterface)
	{
		return;
	}

	if (nullptr == _hitMotionTimer)
	{
		InitInternalTimer();
	}

	if (_hitMotionPlay == true)
	{
		return;
	}
	
	EGsAnimStateBlendType blendType = DecideBlendTypebyAnimState(inCurrentState);
	if (EGsAnimStateBlendType::BodyNone == blendType)
	{
		return;
	}

	GetMesh()->LinkAnimClassLayers(_hitMotionInterface);
	_hitInstance = Cast<UGsHitAnimInstance>(GetMesh()->GetLinkedAnimLayerInstanceByClass(_hitMotionInterface));
	if (nullptr == _hitInstance)
	{
		return;
	}
	


	_hitInstance->HitMotionDirection = static_cast<EGsAnimStateHitDirection>((FMath::RandHelper((int)EGsAnimStateHitDirection::DIR_MAX)));
	_hitInstance->BodyBlendType = blendType;

#if WITH_EDITOR
	SelectHitDirection = static_cast<int>(_hitInstance->HitMotionDirection);
	SelectHitBlendType = static_cast<int>(_hitInstance->BodyBlendType);
#endif

	_hitMotionTimer->Reset();	
	_hitMotionTimer->SetTick(true);	
	
	_hitMotionPlay = true;
}

void AGsCharacterBase::DoHitAplitude()
{
	if (nullptr == _hitAmplitudeTimer)
	{
		return;
	}

	if (true == _hitAmplitudePlay)
	{
		return;
	}

	USkeletalMeshComponent* mesh = GetMesh();
	if (nullptr != mesh)
	{
		// 기타등등 옵션을 추가해줘야 함

		_hitAmplitudeTimer->Reset();
		_hitAmplitudeTimer->SetTick(true);

		_hitAmplitudePlay = true;
		_originMeshRelativeLocation = mesh->GetRelativeLocation();
	}
}

void AGsCharacterBase::ProcessHitMotion(float fDelta)
{
	if (nullptr == _hitMotionTimer || false == _hitMotionPlay)
	{
		return;
	}

	_hitMotionTimer->Tick(fDelta);

	if (_hitInstance)
	{
		_hitInstance->BlendWeightsHitMotion = _hitMotionTimer->GetCurrentValue();
	}
}

void AGsCharacterBase::ProcessAmplitude(float fDelta)
{
	if (nullptr == _hitAmplitudeTimer || false == _hitAmplitudePlay)
	{
		return;
	}

	_hitAmplitudeTimer->Tick(fDelta);

	// Do DeltaMove
	float fAmplitude = _hitAmplitudeTimer->GetCurrentValue() * _hitAmplitudeMultiplier;
	USkeletalMeshComponent* mesh = GetMesh();
	check(mesh);

	FVector RelativeLocation = mesh->GetRelativeLocation();
	RelativeLocation += (FVector::ForwardVector * (fDelta * fAmplitude));
	mesh->SetRelativeLocation(RelativeLocation);	
}

void AGsCharacterBase::ProcessSmoothRotaiton(float fDelta)
{
	if (_smoothRotationInfo._isRotation)
	{
		USkeletalMeshComponent* mesh = GetMesh();
		_smoothRotationInfo._currentRotation = UKismetMathLibrary::RInterpTo_Constant(_smoothRotationInfo._currentRotation,
			_originMeshRotation, fDelta, _smoothRotationInfo._rotateSpeed);
		mesh->SetRelativeRotation(_smoothRotationInfo._currentRotation);
		if (_smoothRotationInfo._currentRotation.Equals(_originMeshRotation))
		{
			_smoothRotationInfo._isRotation = false;
		}
	}
}

void AGsCharacterBase::ProcessMaterialParams(float fDelta)
{	
	if (_materialInfo._isUpdate)
	{
		// 등록되어있는 모든 메시에 적용
		for (FGsCharacterOrgMaterialInfo& el : _materialInfo._originMaterials)
		{
			UpdateMaterialParams(el._meshComponent, _materialInfo._currentMaterialData, 
				el._materialIndex, _materialInfo._currTime, fDelta, el._isOverlayMaterial);
		}
		_materialInfo._currTime += fDelta;
	}
}

void AGsCharacterBase::UpdateMaterialParams(UMeshComponent* MeshComp, UGsGameObjectMaterialData* MaterialData,
	int MaterialIndex, float fCurrent, float fDelta, bool isOverlayMaterial)
{
	if (nullptr == MaterialData)
	{
		return;
	}

	for (FGsGameObjectMaterialParam& el : MaterialData->_materialPrarams)
	{
		if (auto material = isOverlayMaterial ? MeshComp->GetOverlayMaterial() : MeshComp->GetMaterial(MaterialIndex))
		{
			UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(material);
			if (DynamicMaterial == nullptr)
			{
				if(isOverlayMaterial)
				{
					DynamicMaterial = UMaterialInstanceDynamic::Create(material, MeshComp);
					MeshComp->SetOverlayMaterial(DynamicMaterial);
				}
				else
				{
					DynamicMaterial = MeshComp->CreateAndSetMaterialInstanceDynamic(MaterialIndex);
				}
			}

			bool impossibleApplied = false;
			switch (el.ParamType)
			{
			case EGsSkeletalMeshMaterialParam::CurveScalar:
			{
				if (nullptr == el.MaterialParamCF)
				{
					impossibleApplied = true;
					break;
				}
				DynamicMaterial->SetScalarParameterValue(el.MaterialParameterName,
					el.MaterialParamCF->GetFloatValue(fCurrent));
			}
			break;
			case EGsSkeletalMeshMaterialParam::CurveVector:
			{
				if (nullptr == el.MaterialParamCV)
				{
					impossibleApplied = true;
					break;
				}

				DynamicMaterial->SetVectorParameterValue(el.MaterialParameterName,
					el.MaterialParamCV->GetVectorValue(fCurrent));
			}
			break;
			case EGsSkeletalMeshMaterialParam::Scalar:
				DynamicMaterial->SetScalarParameterValue(el.MaterialParameterName, el.MaterialParamF);
				break;
			case EGsSkeletalMeshMaterialParam::Vector:
				DynamicMaterial->SetVectorParameterValue(el.MaterialParameterName, el.MaterialParamV);
				break;
			}

			if (impossibleApplied) break;
		}
	}
}

void AGsCharacterBase::FinishBlendWeight(const UObject*)
{
	_hitMotionPlay = false;
	_hitMotionTimer->Stop();

	GetMesh()->UnlinkAnimClassLayers(_hitMotionInterface);
}

void AGsCharacterBase::FinishAmplitude(const UObject* timer)
{
	_hitAmplitudePlay = false;
	_hitAmplitudeTimer->Stop();

	USkeletalMeshComponent* mesh = GetMesh();
	check(mesh);
	mesh->SetRelativeLocation(_originMeshRelativeLocation);
}

void AGsCharacterBase::ResultHitEffectInfo(OUT UGsHitEffectData*& OutEffectData, OUT UGsHitEffectData*& OutAdditiveEffectData, 
	OUT const FGsSchemaSoundResData*& OutSoundData, OUT CreatureWeaponType& OutWeaponType, 
	const FGsSchemaSkillSet* SkillData, 
	const FGsSchemaSkillNotifySet* SkillNotifyData,
	const FGsSchemaPassivitySet* PassivityData, 
	const FGsSchemaProjectileSet* ProjectileData,
	const FGsSchemaExtraTypeEffectSet* DamageTypeEffectSet,
	const FGsSchemaHealByTypeEffectSet* HealTypeEffectSet
)
{
	OutEffectData = nullptr;
	OutAdditiveEffectData = nullptr;
	OutSoundData = nullptr;
	OutWeaponType = CreatureWeaponType::MAX;

	// 스킬 데이터 오버라이드
	if (SkillData)
	{
		if (SkillData->hiteffectData)
		{
			OutEffectData = SkillData->hiteffectData;
		}

		if (SkillData->hitSoundData.GetRow())
		{
			OutSoundData = SkillData->hitSoundData.GetRow();
		}
		OutWeaponType = SkillData->requireWeapon;
	}

	if (PassivityData)
	{
		if (PassivityData->hiteffectData)
		{
			OutEffectData = PassivityData->hiteffectData;
		}

		if (PassivityData->hitSoundData.GetRow())
		{
			OutSoundData = PassivityData->hitSoundData.GetRow();
		}
	}

	// 프로젝타일 타입 오버라이드
	if (ProjectileData)
	{
		if (ProjectileData->hiteffectData)
		{
			OutEffectData = ProjectileData->hiteffectData;
		}

		if (ProjectileData->hitSoundData.GetRow())
		{
			OutSoundData = ProjectileData->hitSoundData.GetRow();
		}
	}

	// 스킬 노티파이 데이터 오버라이드
	if (SkillNotifyData)
	{
		if (SkillNotifyData->hiteffect)
		{
			switch (SkillNotifyData->hiteffect->IgnoreType)
			{
			case HitEffectIgnoreType::NONE:
				OutAdditiveEffectData = SkillNotifyData->hiteffect;
				break;
			case HitEffectIgnoreType::IGNORE_MYSELP:
				OutAdditiveEffectData = nullptr;
				break;
			case HitEffectIgnoreType::IGNORE_PARENT:
				OutEffectData = SkillNotifyData->hiteffect;
				OutAdditiveEffectData = nullptr;
				break;
			default:
				break;
			}
		}

		if (SkillNotifyData->hitsound.GetRow())
		{
			OutSoundData = SkillNotifyData->hitsound.GetRow();
		}
	}

	// 더 상위 개념 추가...  DamageTypeEffectSet,  HealTypeEffectSet,
 	if (DamageTypeEffectSet)
 	{
 		OutEffectData = DamageTypeEffectSet->effectPath;
 	}
 
 	if (HealTypeEffectSet)
 	{
 		OutEffectData = HealTypeEffectSet->effectPath;
		OutSoundData = HealTypeEffectSet->hitSound.GetRow();
 	}
}

void AGsCharacterBase::SetSmoothRotaion(const FRotator& NewRotation)
{
	const FRotator& currentRot = GetActorRotation();

	FRotator interpolationRot = currentRot - NewRotation;
	interpolationRot.Normalize();

	// RootComponent 즉시 회전
	SetActorRotation(NewRotation);

	// 10도정도 연출 처리 안함 (기존 1도)
	if (FMath::Abs(interpolationRot.Yaw) < 10.f)
	{
		return;
	}

	if (USkeletalMeshComponent* mesh = GetMesh())
	{
// 		if (nullptr == _smoothRotationInfo._curveData)
// 		{
// 			const UGsGlobalConstant* globalConstData = GData()->GetGlobalData();
// 			_smoothRotationInfo._curveData = globalConstData->_smoothRotation;
// 		}
// 
// 		if (nullptr == _smoothRotationInfo._curveData)
// 		{
// 			return;
// 		}
// 
		FRotator relativeRot = _originMeshRotation - interpolationRot.GetInverse();
		mesh->SetRelativeRotation(relativeRot);
		_smoothRotationInfo._currentRotation = mesh->GetRelativeRotation();
		_smoothRotationInfo._isRotation = true;

		// GSLOG(Error, TEXT("[Debug Text] Start Rotation Yaw : %f Delta yaw : %f RotationTime : %f"), _smoothRotationInfo._currentRotation.Yaw,
		//	interpolationRot.Yaw, _smoothRotationInfo.InterpRotationTime);
	}
}

void AGsCharacterBase::StopCurrentSkillEffectNSound()
{
	USkeletalMeshComponent* meshcomp = GetMesh();
	TArray<USceneComponent*> childrenCom = meshcomp->GetAttachChildren();
	for (int i = 0; i < childrenCom.Num(); ++i)
	{
		if (true == childrenCom[i]->IsA<UParticleSystemComponent>())
		{
			UParticleSystemComponent* particleComp = Cast<UParticleSystemComponent>(childrenCom[i]);
			if (nullptr != particleComp)
			{
				particleComp->DeactivateImmediate();
			}
		}
	}

	// Sound Stop
	if (nullptr != GSound())
	{
		if (USkeletalMeshComponent * mesh = GetMesh())
		{
			if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
			{
				soundPlayer->StopSound(mesh);
			}
		}
	}

	/*
	TInlineComponentArray<UPrimitiveComponent*> ComponentArray;
	GetComponents(ComponentArray);
	TArray<USceneComponent*> childComponents;
	for (UPrimitiveComponent* Component : ComponentArray)
	{
	Component->GetChildrenComponents(true, childComponents);
	for (USceneComponent* childComponent : childComponents)
	{
	if (true == childComponent->IsA<UParticleSystemComponent>())
	{
	GSLOG(Log ,TEXT("childComponents Name : %s"),*childComponent->GetName());
	childComponent->DestroyComponent();
	}
	}
	}
	*/
}

void AGsCharacterBase::InitMeshShaderComponent(const FGsSchemaCreatureCommonInfo* inCommonInfo)
{
	if (inCommonInfo)
	{
		if (UGsMeshShaderControlComponent* comp = GetMeshShaderComponent())
		{
			comp->Initialize(inCommonInfo);
		}
	}
}

// 액터 위치기준 구 그리기
void AGsCharacterBase::DrawActorDebugSphere(float In_range)
{
	DrawDebugSphere(GetWorld(), GetActorLocation(), In_range, 16, FColor::Magenta, false, 1.0f);
}


void AGsCharacterBase::RelocationCharacter()
{
	TeleportTo(_lastLocation, GetActorRotation());	
}

void AGsCharacterBase::SetLastLocation(const FVector& inLocation)
{
	_lastLocation = inLocation;		
}

FVector AGsCharacterBase::GetTargetHitPoint(const FVector& inCasterPos, const FVector& inCasterDir,
	const FName& inHitSoketName, bool inRandom, const FVector& inRandomSize)
{
	if (USkeletalMeshComponent* mesh = GetMesh())
	{
		float radius = GetCapsuleComponent()->GetScaledCapsuleRadius();
		// const FVector& dir = inCasterDir.GetSafeNormal();
		
		const FVector& hitPos = mesh->GetSocketLocation(inHitSoketName);
		FVector dir = inCasterPos - hitPos;
		FVector hitDestPos = hitPos + dir.GetSafeNormal() * radius;
		if (inRandom)
		{
			// 캐스터 위치와 히트 위치의 거리에 따라 간격을 좁혀준다.
			/*float randomXSizeRate = (inCasterPos - hitPos).SizeSquared() / (500.f * 500.f);*/
			float randomXSize = inRandomSize.X /** randomXSizeRate*/;
			float randRight = FMath::RandRange(randomXSize * 0.8f, randomXSize);
			randRight *= FMath::RandBool() ? -1.f : 1.f;

			// 높이는 약간만 올려만 준다. 밑으로 발사되면 이상해 보일듯
			// float randUp = FMath::RandRange(0.f, inRandomSize.Z * 0.25f);

			hitDestPos += dir.Rotation().RotateVector(FVector(0.f, randRight, 0.f));
			// 			#if WITH_EDITOR
			// 						// 디버깅 확인용
			// 						DrawDebugSphere(GetWorld(), hitTargetPos, 5, 50, FColor::Red,
			// 							false, 5.f);
			// 						DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), hitTargetPos,
			// 							20.f, FColor::Green, false, 5.f);
			// 			#endif
		}
		return hitDestPos;
	}
	return GetActorLocation();
}

void AGsCharacterBase::OnChangeRenderState(bool bIsRendered)
{
	if (UGsUIBillboardCreature* widget = GetWidget())
	{
		widget->OnChangeRenderState(bIsRendered);
	}
}

float AGsCharacterBase::GetLastRenderTimeOnScreenMesh() const
{
	if (_partsSkeletalComponentFACE->MeshObject)
	{
		return _partsSkeletalComponentFACE->GetLastRenderTimeOnScreen();
	}

	return GetMesh()->GetLastRenderTimeOnScreen();
}

UGsAnimInstanceState* AGsCharacterBase::GetAnim() const
{
	if (_polymorpActor && _polymorpSkeletalMesh)
	{
		return Cast<UGsAnimInstanceState>(_polymorpSkeletalMesh->GetAnimInstance());
	}

	return (GetMesh()) ? Cast<UGsAnimInstanceState>(GetMesh()->GetAnimInstance()) : nullptr;
}

UAnimInstance* AGsCharacterBase::GetAnimationInstance() const
{
	return (GetMesh()) ? GetMesh()->GetAnimInstance() : nullptr;
}

bool AGsCharacterBase::LoadHitAnimInstance(const FSoftObjectPath& path)
{
	InitInternalTimer();
	
	if (nullptr != _hitMotionInterface)
	{
		HitAnimInstanceDeferred();
		return true;
	}

	if (false == path.IsValid())
	{
		return false;
	}

	FString path_c = path.ToString() + TEXT("_C");


	_hitMotionLoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(path_c, FStreamableDelegate::CreateUObject(this, &AGsCharacterBase::HitAnimInstanceDeferred));
	return true;
}

void AGsCharacterBase::HitAnimInstanceDeferred()
{
	if (_hitMotionLoadHandle.IsValid() && (nullptr == _hitMotionInterface))
	{
		_hitMotionInterface = Cast<UClass>(_hitMotionLoadHandle->GetLoadedAsset());
	}	
}
	

void AGsCharacterBase::InitInternalTimer()
{
	// Hit Motion Blend 타이머
	_hitMotionTimer = NewObject<UGsCurveTimer>();
	_hitMotionTimer->SetCurve(_hitMotionBlendWeight);
	_hitMotionTimer->_finishDelegate.AddDynamic(this, &AGsCharacterBase::FinishBlendWeight);

	if (GData())
	{
		// Hit 진폭 처리 타이머
		if (const UGsGlobalConstant* globalConstData = GData()->GetGlobalData())
		{
			_hitAmplitudeTimer = NewObject<UGsCurveTimer>();
			_hitAmplitudeTimer->SetCurve(globalConstData->_hitAmplitudeTime);
			_hitAmplitudeTimer->_finishDelegate.AddDynamic(this, &AGsCharacterBase::FinishAmplitude);
			_hitAmplitudeMultiplier = globalConstData->_hitAmplitudeMultiplier;

			if (nullptr == _hitMotionBlendWeight)
			{
				_hitMotionTimer->SetCurve(globalConstData->_hitMotionBlendTime);
			}
		}
	}	
}

void AGsCharacterBase::ActiveGravity(bool inActive)
{		
	if (UCharacterMovementComponent* charMoveComp = Cast<UCharacterMovementComponent>(GetMovementComponent()))
	{
		charMoveComp->GravityScale = inActive ? 1.0f : 0.0f;
	}
}

void AGsCharacterBase::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
}

void AGsCharacterBase::SetCollisionProfileName(FName InCollisionProfileName, bool bUpdateOverlaps /*= true*/)
{
	if (UCapsuleComponent* capsule = GetCapsuleComponent())
	{
		if (capsule->GetCollisionProfileName() != InCollisionProfileName)
		{
			capsule->SetCollisionProfileName(InCollisionProfileName, bUpdateOverlaps);
		}		
	}
}

void AGsCharacterBase::SetDefaultCollisionProfile()
{
	SetCollisionProfileName(_oringinCollisionType);
}

void AGsCharacterBase::StartMeshShader(EGsMeshShaderEffect inType, bool inIsRestor)
{
	if (UGsMeshShaderControlComponent* comp = GetMeshShaderComponent())
	{
		comp->Add(inType, inIsRestor);
		comp->Start();
	}
}

void AGsCharacterBase::StartMeshShader(EGsMeshShaderEffect inType, const FGsSchemaCreatureCommonInfo* inInfo)
{
	if (UGsMeshShaderControlComponent* component = GetMeshShaderComponent())
	{
		if (inInfo->meshShader.Contains(inType))
		{
			component->Add(EGsMeshShaderEffect::Fresnel, inInfo->meshShader[inType]);
		}
		else
		{
			component->Add(EGsMeshShaderEffect::Fresnel);
		}

		component->Start();
	}	
}

void AGsCharacterBase::StopMeshShader()
{	
	if (UGsMeshShaderControlComponent* meshShaderComp = GetMeshShaderComponent())
	{
		meshShaderComp->Stop();
	}
}

#if WITH_EDITOR
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
void AGsCharacterBase::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
}

#endif

bool AGsCharacterBase::ShouldTickIfViewportsOnly() const
{
#if WITH_EDITOR
	AGameModeBase* mode = UGameplayStatics::GetGameMode(GetWorld());
	if (mode && mode->IsA(AGsGameModeTestClient::StaticClass()))
	{
		return true;
	}
#endif

	return Super::ShouldTickIfViewportsOnly();
}

void AGsCharacterBase::InitWidgetComponent()
{
//	FTransform trans = GetMesh()->GetSocketTransform(TEXT("HitEffectSocket"), RTS_Component);

// 	if (_lockOnWidget)
// 	{
// 		if (nullptr == _lockOnWidget->GetWidgetClass().Get())
// 		{
// 			if (UGsPathManager* pathMgr = GPath())
// 			{
// 				UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("LockOnWidget"));
// 				if (widgetClass)
// 				{
// 					_lockOnWidget->SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
// 				}
// 			}
// 		}
// 		_lockOnWidget->SetRelativeLocation(trans.GetTranslation());
// 	}

	FTransform trans = GetMesh()->GetSocketTransform(TEXT("HitEffectSocket"), RTS_Component);

 	if (_partySharedTargetWidget)
 	{
 		if (nullptr == _partySharedTargetWidget->GetWidgetClass().Get())
 		{
 			if (UGsPathManager* pathMgr = GPath())
 			{
 				UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("PartySharedTargetWidget"));
 				if (widgetClass)
 				{
					_partySharedTargetWidget->SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
 				}
 			}
 		}
		_partySharedTargetWidget->SetRelativeLocation(trans.GetTranslation());
 	}
}

// 위젯 컴퍼넌트의 위젯이 생성 된뒤
void AGsCharacterBase::PostWidgetComponentMakeWidget()
{
	// 처음에 hide 시키기(시퀀스틑 gameobjectbase를 사용안하는거 같다)
	if (UGsUIBillboard* billboard = GetLockOnWidget())
	{
		ESlateVisibility visibility = ESlateVisibility::Hidden;
		billboard->SetVisibility(visibility);
	}

	if (UGsUIBillboard* partySharedTargetBillboard = GetPartySharedTargetWidget())
	{
		ESlateVisibility visibility = ESlateVisibility::Hidden;
		partySharedTargetBillboard->SetVisibility(visibility);
	}
}

UGsUIBillboard* AGsCharacterBase::GetLockOnWidget() const
{
	if (_lockOnWidget)
	{
		return Cast<UGsUIBillboard>(_lockOnWidget->GetUserWidgetObject());
	}
	return nullptr;
}

UGsUIBillboard* AGsCharacterBase::GetPartySharedTargetWidget() const
{
	if (_partySharedTargetWidget)
	{
		return Cast<UGsUIBillboard>(_partySharedTargetWidget->GetUserWidgetObject());
	}
	return nullptr;
}

void AGsCharacterBase::AddOriginalMaterialInfo(UMeshComponent* inMeshComponent, UMaterialInterface* inMaterial, 
	bool isOverlayMaterial, int inMaterialIndex)
{
	FGsCharacterOrgMaterialInfo newMatInfo;
	newMatInfo._meshComponent = inMeshComponent;
	newMatInfo._material = inMaterial;
	newMatInfo._materialIndex = isOverlayMaterial ? 0 : inMaterialIndex;
	newMatInfo._isOverlayMaterial = isOverlayMaterial;
	_materialInfo._originMaterials.Emplace(newMatInfo);
}

void AGsCharacterBase::SetChangeAllMaterials(UGsGameObjectMaterialData* inMaterialData, bool useOnlyOverlayMaterial)
{
	// 제거된 머터리얼 정보 복원후 삭제
	if (IsMaterialChanged())
	{
		if (_materialInfo._currentMaterialData == inMaterialData)
		{
			UpdateAllChangedMaterials();
			return;
		}

		for (FGsCharacterOrgMaterialInfo& el : _materialInfo._originMaterials)
		{
			if (el._meshComponent)
			{
				if (el._isOverlayMaterial)
					 el._meshComponent->SetOverlayMaterial(el._material);
				else el._meshComponent->SetMaterial(el._materialIndex, el._material);
			}
		}

		_materialInfo._originMaterials.Empty();
	}


	_materialInfo._currentMaterialData = inMaterialData;
	_materialInfo._isAppliedOverlayMaterial = useOnlyOverlayMaterial;
	// 머터리얼 Param 업데이트가 필요한가
	_materialInfo._isUpdate = inMaterialData->_materialPrarams.Num() > 0;
	_materialInfo._currTime = 0.f;

	if (inMaterialData->_materialControlType != EGsGameObjectMaterial::CUSTOM) return;

	// 하위 메시 정보 찾기
	TArray<UMeshComponent*> outComponents;
	GetComponents(outComponents, true);

	if (IsMeshChanged())
	{
		TArray<UMeshComponent*> polyMonphMeshCompos;
		_polymorpActor->GetComponents(polyMonphMeshCompos, true);

		outComponents.Append(polyMonphMeshCompos);
	}

	TArray<FGsGameObjectMaterialParam> useParams = inMaterialData->_materialPrarams.FilterByPredicate([](auto& param) {
		return param.ParamType == EGsSkeletalMeshMaterialParam::Vector; });
	
	if (useOnlyOverlayMaterial)
	{
		Algo::ForEach(outComponents,  [this, inMaterialData, &useParams](auto el) {
	
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(inMaterialData->_material, el);
				AddOriginalMaterialInfo(el, el->GetOverlayMaterial(), true);
				el->SetOverlayMaterial(DynamicMaterial);

				for(auto& param : useParams) DynamicMaterial->SetVectorParameterValue(param.MaterialParameterName, FLinearColor(param.MaterialParamV));
			});
	}
	else // all
	{
		for (UMeshComponent* el : outComponents)
		{
			auto materialCount = el->GetNumMaterials();
			for (int idx = 0; idx < materialCount; ++idx)
			{
				AddOriginalMaterialInfo(el, el->GetMaterial(idx), false, idx);
				el->SetMaterial(idx, inMaterialData->_material);
			}

			AddOriginalMaterialInfo(el, el->GetOverlayMaterial(), true);
			UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(inMaterialData->_material, el);
			el->SetOverlayMaterial(DynamicMaterial);

			for (auto& param : useParams) DynamicMaterial->SetVectorParameterValue(param.MaterialParameterName, FLinearColor(param.MaterialParamV));
		}
	}

	// 커브데이터 시간 기록
	for (FGsGameObjectMaterialParam& el : inMaterialData->_materialPrarams)
	{
		if (el.ParamType == EGsSkeletalMeshMaterialParam::CurveScalar)
		{
			if (el.MaterialParamCF)
			{
				float minTime;
				el.MaterialParamCF->GetValueRange(minTime, el.TotalTime);
			}
		}
		else if (el.ParamType == EGsSkeletalMeshMaterialParam::CurveVector)
		{
			if (el.MaterialParamCV)
			{
				float minTime;
				el.MaterialParamCV->GetValueRange(minTime, el.TotalTime);
			}
		}
	}
}

void AGsCharacterBase::RestoreAllMaterials()
{
	if (IsMaterialChanged())
	{
		for (FGsCharacterOrgMaterialInfo& el : _materialInfo._originMaterials)
		{
			// Parameter 초기화
			UpdateMaterialParams(el._meshComponent, _materialInfo._currentMaterialData,
				el._materialIndex, 0.f, 0.f, el._isOverlayMaterial);

			// 머터리얼 되돌림
			if (el._meshComponent)
			{
				if (el._isOverlayMaterial)
					 el._meshComponent->SetOverlayMaterial(el._material);
				else el._meshComponent->SetMaterial(el._materialIndex, el._material);
			}
		}
	}
	_materialInfo._originMaterials.Reset();
	_materialInfo._isUpdate = false;
	_materialInfo._currTime = 0.f;
	_materialInfo._currentMaterialData = nullptr;

	// 기존 머터리얼 효과 복원
	if (UGsMeshShaderControlComponent* meshShaderComp = GetMeshShaderComponent())
	{
		meshShaderComp->Restore(EGsMeshShaderEffect::Dissolve);
		meshShaderComp->Restore(EGsMeshShaderEffect::Fresnel);
	}
}

// 특정 매쉬에 적용된 original material 정보를 바꾸고 현재 적용되어야 하는 material을 재적용
void AGsCharacterBase::UpdateOriginalMaterial(UMeshComponent* meshCompo, UMaterialInterface* newMaterial)
{
	if (IsMaterialChanged() && meshCompo)
	{
		auto useData = _materialInfo._currentMaterialData;
		auto curSetMaterial = useData->_material;

		if (useData->_materialControlType == EGsGameObjectMaterial::CUSTOM)
		{
			_materialInfo._originMaterials.RemoveAll([meshCompo](auto& e) { return e._meshComponent == meshCompo; });

			if (_materialInfo._isAppliedOverlayMaterial)
			{
				AddOriginalMaterialInfo(meshCompo, newMaterial, true);
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(curSetMaterial, meshCompo);
				meshCompo->SetOverlayMaterial(newMaterial == nullptr ? nullptr : DynamicMaterial);
			}
			else
			{
				auto materialCount = meshCompo->GetNumMaterials();
				for (int index = 0; index < materialCount; index++)
				{
					AddOriginalMaterialInfo(meshCompo, newMaterial, false, index);
					meshCompo->SetMaterial(index, newMaterial == nullptr ? nullptr : curSetMaterial);
				}

				AddOriginalMaterialInfo(meshCompo, meshCompo->GetOverlayMaterial(), true);
				UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(curSetMaterial, meshCompo);
				meshCompo->SetOverlayMaterial(newMaterial == nullptr ? nullptr : DynamicMaterial);
			}

			_materialInfo._isUpdate = _materialInfo._currentMaterialData->_materialPrarams.Num() > 0;
		}
	}
}

// material changed 상태일 때 매쉬 change 등으로 모든 메쉬에 changed 되어 있는 material을 재적용
void AGsCharacterBase::UpdateAllChangedMaterials()
{
	if (IsMaterialChanged())
	{
		TArray<UMeshComponent*> outComponents;
		GetComponents(outComponents, true);

		if (IsMeshChanged())
		{
			TArray<UMeshComponent*> polyMonphMeshCompos;
			_polymorpActor->GetComponents(polyMonphMeshCompos, true);

			outComponents.Append(polyMonphMeshCompos);
		}

		bool useOverlayMaterial = _materialInfo._isAppliedOverlayMaterial;
		auto useData = _materialInfo._currentMaterialData;
		auto curSetMaterial = useData->_material;

		if (useData->_materialControlType != EGsGameObjectMaterial::CUSTOM) return;

		int checkIndex = 0;
		TArray<UMeshComponent*> checkList;
		if (useOverlayMaterial) 
		{
			checkList = outComponents.FilterByPredicate([curSetMaterial](auto el) {
					if (auto dynamicMat = Cast<UMaterialInstanceDynamic>(el->GetOverlayMaterial()))
					{
						return dynamicMat->IsChildOf(curSetMaterial) == false;
					}
					return true;
				});
		}
		else
		{
			checkList = outComponents.FilterByPredicate([curSetMaterial](auto el) {
				if (auto dynamicMat = Cast<UMaterialInstanceDynamic>(el->GetOverlayMaterial()))
				{
					return dynamicMat->IsChildOf(curSetMaterial) == false;
				}
				if (auto dynamicMat = Cast<UMaterialInstanceDynamic>(el->GetMaterial(0)))
				{
					return dynamicMat->IsChildOf(curSetMaterial) == false;
				}

				return true;
				});
		}

		Algo::ForEach(checkList, [useOverlayMaterial, checkIndex, useData, curSetMaterial, this](auto el) {
				bool add = _materialInfo._originMaterials.FindByPredicate([el](auto org) { return el == org._meshComponent; }) == nullptr;
				if (useOverlayMaterial)
				{
					if (add) AddOriginalMaterialInfo(el, el->GetOverlayMaterial(), true);

					UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(curSetMaterial, el);
					el->SetOverlayMaterial(DynamicMaterial);
				}
				else
				{
					auto index = 0;
					for (auto er : el->OverrideMaterials)
					{
						if (add) AddOriginalMaterialInfo(el, er, false, index);
						el->SetMaterial(index, er == nullptr ? nullptr : curSetMaterial);
						index++;
					}

					if (add) AddOriginalMaterialInfo(el, el->GetOverlayMaterial(), true);
					UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(curSetMaterial, el);
					el->SetOverlayMaterial(DynamicMaterial);
				}
			});
	}
}

void AGsCharacterBase::RestorePreEquipMaterial(UMeshComponent* skeletalMeshComp)
{
	if (IsMaterialChanged())
	{
		auto filterMaterial = _materialInfo._originMaterials.FilterByPredicate([skeletalMeshComp](auto& e) { return e._meshComponent == skeletalMeshComp; });
		auto useData = _materialInfo._currentMaterialData;

		if (_materialInfo._isAppliedOverlayMaterial)
		{
			Algo::ForEach(filterMaterial, [](auto el)
				{
					if (el._meshComponent) el._meshComponent->SetOverlayMaterial(el._material);
				});
		}
		else
		{
			Algo::ForEach(filterMaterial, [](auto el)
				{
					if (el._meshComponent) el._meshComponent->SetMaterial(el._materialIndex, el._material);
				});
		}

		_materialInfo._originMaterials.RemoveAll([skeletalMeshComp](auto& e) { return e._meshComponent == skeletalMeshComp; });
	}
}

// 다른 타입의 무기로 바꾸거나 무기 해제후, 다시 해제 또는 바뀌기 전의 무기로 바로 착용하면 캐싱된 material이 변경 material로 남아 있다
// restore 한 후에 삭제한다. 
// material 변경 기능이 아닌 material changed info가 actor에 있어야 하는지는 나중에 다시 생각(오브젝트 단위나 customizeHandler로 올릴 필요가 있는지)
void AGsCharacterBase::RestoreAttachedActorMaterials(TArray<AActor*>& childActors) 
{
	if (IsMaterialChanged())
	{
		TArray<UMeshComponent*> outComponents;
		Algo::ForEach(childActors, [&outComponents](auto actor) {
				if (actor)
				{
					TArray<UMeshComponent*> comps;
					actor->GetComponents(comps);
					outComponents.Append(comps);
				}
			});

		for (auto meshCompo : outComponents)
		{
			auto filterMaterial = _materialInfo._originMaterials.FilterByPredicate([meshCompo](auto& e) { return e._meshComponent == meshCompo; });
			auto useData = _materialInfo._currentMaterialData;

			if (_materialInfo._isAppliedOverlayMaterial)
			{
				Algo::ForEach(filterMaterial, [](auto el) {
						if (el._meshComponent) el._meshComponent->SetOverlayMaterial(el._material);
					});
			}
			else
			{
				Algo::ForEach(filterMaterial, [](auto el) {
						if (el._meshComponent) el._meshComponent->SetMaterial(el._materialIndex, el._material);
					});
			}

			_materialInfo._originMaterials.RemoveAll([meshCompo](auto& e) { return e._meshComponent == meshCompo; });
		}
	}
}

void AGsCharacterBase::ChangeLightingChannels(bool inChannel1, bool inChannel2, bool inChannel3)
{
	if (GetMesh())
	{
		GetMesh()->ChangeLightingChannels(inChannel1, inChannel2, inChannel3);
	}	

	if (_partsSkeletalComponentFACE)
	{
		_partsSkeletalComponentFACE->ChangeLightingChannels(inChannel1, inChannel2, inChannel3);
	}	
}

void AGsCharacterBase::GetSkeletalMeshs(TArray<USkeletalMeshComponent*>& Out_skeletalMesh)
{
	Out_skeletalMesh.Add(GetMesh());
	Out_skeletalMesh.Add(GetPartsSkeletalComponentFACE());
}

void AGsCharacterBase::UpdateRenderState()
{
	OnChangeRenderState(_bWasRendered);
}

void AGsCharacterBase::EffectsHiddenInGame(bool set)
{
	USkeletalMeshComponent* meshcomp = GetMesh();
	TArray<USceneComponent*> childrenCom = meshcomp->GetAttachChildren();
	Algo::ForEachIf(childrenCom, [](auto e) {
		return e != nullptr; }, [set](auto e) {

			if (auto particleComp = Cast<UParticleSystemComponent>(e))
			{
				particleComp->SetHiddenInGame(set, true);
			}
		});
}

void AGsCharacterBase::SetActorHiddenInGame(bool bNewHidden)
{
	if (IsHidden() != bNewHidden)
	{
		SetHidden(bNewHidden); //Hidden 되어 있다는 표시만

		if (IsMeshChanged())
		{
			_polymorpActor->SetActorHiddenInGame(bNewHidden);
		}
		else
		{
			SetHidden(!bNewHidden); //반대로 해주지 않으면 SetActorHiddenInGame 이 실행되지 않음

			Super::SetActorHiddenInGame(bNewHidden);
		}

		// 스켈레탈 메시 제어
		if (USkeletalMeshComponent* skeletalMesh = GetMesh())
		{
			skeletalMesh->SetHiddenInGame(bNewHidden);
		}

		// 이펙트 컴퍼넌트 제어
		if (_effectComponent)
		{
			_effectComponent->SetRepresentAbnormalityEffectVisible(!bNewHidden);
		}

		
	}
}

//TArray<class UMaterialInterface*> AGsCharacterBase::GetMaterialsByMesh(USkeletalMeshComponent* InSkeletalMeshComp)
//{
//	if (IsMaterialChanged())
//	{
//		TArray<class UMaterialInterface*> OutMaterials;
//		TArray<FGsCharacterOrgMaterialInfo> materialOnMeshCompos;
//
//		Algo::CopyIf(_materialInfo._originMaterials, materialOnMeshCompos, [InSkeletalMeshComp](auto& e) {
//			return e._meshComponent == InSkeletalMeshComp; }); 
//
//		materialOnMeshCompos.StableSort([](auto& el, auto& er) { return el._materialIndex > er._materialIndex; });
//
//		OutMaterials.AddZeroed(materialOnMeshCompos[0]._materialIndex + 1);
//		for (auto& e : materialOnMeshCompos)
//		{
//			OutMaterials[e._materialIndex] = e._material;
//		}
//		return OutMaterials;
//	}
//
//	return InSkeletalMeshComp->GetMaterials();
//}

class USkeletalMeshComponent* AGsCharacterBase::GetCandidateMesh()
{
	return _polymorpSkeletalMesh != nullptr ? _polymorpSkeletalMesh : GetMesh();
}

void AGsCharacterBase::ChangeMesh(const FSoftObjectPath& inModelPath)
{
	// 기존 Actor가 존재 하면 Despawn 처리
	if (_polymorpActor)
	{
		_polymorpActor->Destroy();
		_polymorpActor->MarkPendingKill();
		_polymorpActor = nullptr;
		_polymorpSkeletalMesh = nullptr;
	}

	// Fsm 초기화
	EGsStateBase lastStatType = EGsStateBase::Idle;
	if (UGsAnimInstanceState* anim = GetAnim())
	{
		lastStatType = anim->CurrentStateType;
		anim->ResetCurrentState(anim->StateMachineName);
	}

	// 사운드 이펙트 제거
	//StopCurrentSkillEffectNSound();

	// 새로운 Actor 스폰
	FString path = inModelPath.ToString();
	path += TEXT("_C");
	UClass* bpClass = LoadObject<UClass>(nullptr, *path);
	_polymorpActor = GsSpawn::BPClass(GetWorld(), bpClass, GetActorLocation(), GetActorRotation());
	if (_polymorpActor)
	{
		_hiddenComponents.Reset();

		const TArray<USceneComponent*>& AttachedChildren = GetMesh()->GetAttachChildren();
		if (AttachedChildren.Num() > 0)
		{
			TInlineComponentArray<USceneComponent*, NumInlinedActorComponents> ComponentStack;
			ComponentStack.Append(AttachedChildren);

			while (ComponentStack.Num() > 0)
			{
				USceneComponent* const CurrentComp = ComponentStack.Pop(false);
				if (CurrentComp && CurrentComp->bHiddenInGame == false)
				{
					_hiddenComponents.Add(CurrentComp);
					ComponentStack.Append(CurrentComp->GetAttachChildren());
				}
			}
		}
		GetMesh()->SetHiddenInGame(true, true);

		_polymorpActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		_polymorpSkeletalMesh = _polymorpActor->FindComponentByClass<USkeletalMeshComponent>();
		if (_polymorpSkeletalMesh)
		{
			// 애니메이션 처리 동기화
			if (UGsAnimInstanceState* polyAnim = Cast<UGsAnimInstanceState>(_polymorpSkeletalMesh->GetAnimInstance()))
			{
				polyAnim->ChangeState((uint8)lastStatType);
			}

			// 높이 보정
			float offsetZ = UGsAnimInstanceState::GetCapsuleRelativeHeight(
				GetCapsuleComponent(), _polymorpSkeletalMesh, TEXT("Root"));
			_polymorpSkeletalMesh->SetRelativeLocation(FVector(0.f, 0.f, offsetZ));
		}

		if (IsMaterialChanged())
			SetChangeAllMaterials(_materialInfo._currentMaterialData);		
	}
	_polymorpActor->SetActorHiddenInGame(IsHidden());

	if (IsHidden() == false)
	{
		EffectsHiddenInGame(false);
	}
}

void AGsCharacterBase::RestoreMesh()
{
	if (_polymorpActor)
	{
		if (IsMaterialChanged())
		{
			TArray<UMeshComponent*> outComponents;
			_polymorpActor->GetComponents(outComponents, true);

			for (auto mesh : outComponents)
			{
				auto filterMaterial = _materialInfo._originMaterials.FilterByPredicate([mesh](auto& e) { return e._meshComponent == mesh; });
				Algo::ForEach(filterMaterial, [](auto el) {
						if (el._meshComponent) el._meshComponent->SetMaterial(el._materialIndex, el._material);
					});

				_materialInfo._originMaterials.RemoveAll([mesh](auto& e) { return e._meshComponent == mesh; });
			}
		}

		EGsStateBase lastStatType = EGsStateBase::Idle;
		if (UGsAnimInstanceState* anim = GetAnim())
		{
			lastStatType = anim->CurrentStateType;
		}

		_polymorpActor->Destroy();
		_polymorpActor->MarkPendingKill();
		_polymorpActor = nullptr;
		_polymorpSkeletalMesh = nullptr;

		if (UGsAnimInstanceState* anim = GetAnim())
		{
			anim->ChangeState((uint8)lastStatType);
		}
	}

	GetMesh()->SetHiddenInGame(false);
	for (auto compo : _hiddenComponents)
	{
		if(compo)compo->SetHiddenInGame(false);
	}
	_hiddenComponents.Reset();
	SetActorHiddenInGame(IsHidden());

	if (IsHidden())
	{
		EffectsHiddenInGame(true);
	}
}

// 다른 타입의 무기로 바꾸거나 무기 해제후, 다시 해제 또는 바뀌기 전의 무기로 바로 착용하면 캐싱된 meshComponent가 hidden 상태로 남아 있다
// restore 한 후에 삭제한다
void AGsCharacterBase::RestoreAttachedHiddenMesh(TArray<AActor*>& childActors)
{
	if (IsMeshChanged())
	{
		TArray<USceneComponent*> hiddenComponents = MoveTemp(_hiddenComponents);

		Algo::ForEach(childActors, [&hiddenComponents](auto actor) {
			if (actor)
			{
				TArray<UMeshComponent*> outComponents;
				actor->GetComponents(outComponents);

				if (outComponents.Num() > 0)
				{
					TInlineComponentArray<USceneComponent*, NumInlinedActorComponents> ComponentStack;
					ComponentStack.Append(outComponents);

					while (ComponentStack.Num() > 0)
					{
						USceneComponent* const CurrentComp = ComponentStack.Pop(false);
						if (CurrentComp && CurrentComp->bHiddenInGame && hiddenComponents.Find(CurrentComp) != INDEX_NONE)
						{
							CurrentComp->SetHiddenInGame(false);
							hiddenComponents.Remove(CurrentComp);

							ComponentStack.Append(CurrentComp->GetAttachChildren());
						}
					}
				}
			}
			});

		_hiddenComponents = MoveTemp(hiddenComponents);
	}
}

void AGsCharacterBase::ApplyAttachedHiddenMesh(TArray<AActor*>& childActors)
{
	if (IsMeshChanged())
	{
		for (auto actor : childActors)
		{
			if (actor)
			{
				TArray<UMeshComponent*> outComponents;
				actor->GetComponents(outComponents);

				if (outComponents.Num() > 0)
				{
					TInlineComponentArray<USceneComponent*, NumInlinedActorComponents> ComponentStack;
					ComponentStack.Append(outComponents);

					while (ComponentStack.Num() > 0)
					{
						USceneComponent* const CurrentComp = ComponentStack.Pop(false);
						if (CurrentComp && CurrentComp->bHiddenInGame == false)
						{
							CurrentComp->SetHiddenInGame(true);
							_hiddenComponents.Add(CurrentComp);

							ComponentStack.Append(CurrentComp->GetAttachChildren());
						}
					}
				}
			}
		}
	}
}

void AGsCharacterBase::RestorePreEquipHiddenMesh(UMeshComponent* skeletalMeshComp)
{
	if (IsMeshChanged())
	{
		if (skeletalMeshComp && skeletalMeshComp->bHiddenInGame) skeletalMeshComp->SetHiddenInGame(false, true);
	}
}

void AGsCharacterBase::RestoreMeshState(UMeshComponent* skeletalMeshComp)
{
	if (IsMeshChanged())
	{
		if (skeletalMeshComp && !skeletalMeshComp->bHiddenInGame) skeletalMeshComp->SetHiddenInGame(true, true);
	}
}
