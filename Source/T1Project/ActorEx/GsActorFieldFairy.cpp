
#include "GsActorFieldFairy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/GsFairyAnimInstance.h"
#include "ActorComponentEx/GsMeshShaderControlComponent.h"
#include "ActorComponentEx/GsFairyEffectComponent.h"
#include "ActorComponentEx/GsWidgetComponentFairyTalk.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Data/GsResourceManager.h"
#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Sound/GsSoundPlayer.h"
#include "Sound/GsSchemaSoundResData.h"
#include "GameObject/Define/GsGameObjectText.h"

#include "WidgetComponent.h"

const float despawnTime = 0.5f;
const FName SpawnEffectSocketName = TEXT("HitEffectSocket");

AGsActorFieldFairy::AGsActorFieldFairy()
: Super()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootObject"));
	_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (_mesh) 
	{
		_mesh->SetupAttachment(RootComponent);

		_meshFace = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshFace"));
		_meshFace->SetupAttachment(_mesh);
		_meshFace->SetMasterPoseComponent(_mesh);

		// C2URWQ-3075 : 페어리 얼굴 메시 사라지는 현상 수정
#if PLATFORM_WINDOWS
		_meshFace->bRenderCustomDepth = true;
#endif
	}
	_skillEffectComponent = CreateDefaultSubobject<UGsFairyEffectComponent>(TEXT("SkillEffectComponent"));
	_meshShaderComponent = CreateDefaultSubobject<UGsMeshShaderControlComponent>(TEXT("MeshShader"));

	// 말풍선
	_talkBalloonWidget = CreateDefaultSubobject<UGsWidgetComponentFairyTalk>(FairyTalkBalloonComponentName);
	_talkBalloonWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 300.f));
	_talkBalloonWidget->SetWidgetSpace(EWidgetSpace::Screen);
	_talkBalloonWidget->SetDrawSize(FVector2D(100, 100.0f));
	_talkBalloonWidget->SetCastShadow(false);
	_talkBalloonWidget->SetupAttachment(_mesh);
	_talkBalloonWidget->SetGenerateOverlapEvents(false);
	
	PrimaryActorTick.bCanEverTick = true;
}

void AGsActorFieldFairy::BeginPlay()
{
	Super::BeginPlay();

	InitActorAction();

	InitSkeletalMeshURO();
}

void AGsActorFieldFairy::EndPlay(EEndPlayReason::Type Reaseon)
{
	//GetWorld()->GetTimerManager().ClearTimer(_desapwnTimer);
	ClearParticle();

	Super::EndPlay(Reaseon);
}

void AGsActorFieldFairy::Destroyed()
{
	Super::Destroyed();
}

void AGsActorFieldFairy::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AGsActorFieldFairy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UGsFairyAnimInstance* AGsActorFieldFairy::GetAnim()
{
	if (!_mesh) return nullptr;
	return Cast<UGsFairyAnimInstance>(_mesh->GetAnimInstance());
}

void AGsActorFieldFairy::SetSpawn( bool instant)
{
	UGsFairyAnimInstance* animInst = GetAnim();

	if (nullptr != animInst)
	{
		if(instant)
			 animInst->ChangeState(EGsStateFairy::Idle);
		else animInst->ChangeState(EGsStateFairy::Spawn);
	}

	if (EGsStateFairy::Spawn == animInst->GetCurrentState())
	{
		if (_fairySpawnEffect.IsValid() && _skillEffectComponent)
		{
			FString particleBlueprintClassPath = _fairySpawnEffect.ToString();

			TWeakObjectPtr<AGsActorFieldFairy> weakThis(this);
			GResource()->LoadAsync(particleBlueprintClassPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)->void
				{
					if (weakThis.IsValid() && weakThis->_skillEffectComponent)
					{
						weakThis->_skillEffectComponent->OnEffect(Cast<UParticleSystem>(inRes), weakThis->_mesh, true,
							EPSCPoolMethod::AutoRelease,
							EAttachLocation::Type::KeepRelativeOffset,
							FVector::ZeroVector,
							FRotator::ZeroRotator,
							FVector(1.f),
							SpawnEffectSocketName);
					}
				}));
		}
	}
}

void AGsActorFieldFairy::SetSkill(bool instant /*= true*/ )
{
	if (UGsFairyAnimInstance* animInst = GetAnim())
	{
		animInst->ChangeState(EGsStateFairy::Skill);
	}

	if (_skillEffect 
	&& _skillEffect->SoundRes.GetRow()
	&& _skillEffect->SoundRes.GetRow()->id > 0)
	{
		if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
		{
			auto soundRes = _skillEffect->SoundRes.GetRow();
			soundPlayer->PlaySoundTarget(soundRes, RootComponent);
		}
	}
}

void AGsActorFieldFairy::SetDespawn(bool instant, FFairyDespawnAnimEnd endSpawn)
{
	if (UGsFairyAnimInstance* animInst = GetAnim())
	{
		if (instant)
		{
			//GSLOG(Log, TEXT("[[[[[[[ START ]]]]]]] AGsActorFieldFairy::SetDespawn"));

			TWeakObjectPtr<AGsActorFieldFairy> thiz = this;
			animInst->ChangeState(EGsStateFairy::Destroy, false, [thiz, endSpawn](EGsStateFairy e) {
					if (thiz.IsValid()) thiz.Get()->OnDestroy();
					endSpawn.ExecuteIfBound(); //destroy 만 가능
				});
			//GetWorld()->GetTimerManager().SetTimer(_desapwnTimer, this, &AGsActorFieldFairy::OnDestroy, despawnTime, false, 0.1f);
		}
		else
		{ 
			//play animation
			TWeakObjectPtr<AGsActorFieldFairy> thiz = this;
			animInst->ChangeState(EGsStateFairy::Despawn, false, [thiz, endSpawn](EGsStateFairy e) {
				if (thiz.IsValid()) thiz.Get()->SetDespawn(true, endSpawn);
			});
		}
	}
}

void AGsActorFieldFairy::SetSpecial()
{
	if (UGsFairyAnimInstance* animInst = GetAnim())
	{
		animInst->ChangeState(EGsStateFairy::Special);
	}
}

void AGsActorFieldFairy::SetIsMove(bool isMove)
{
	if (UGsFairyAnimInstance* animInst = GetAnim())
	{
		animInst->SetMoveState(isMove);
	}
}

void AGsActorFieldFairy::SetIsVehicleMove(bool isMove)
{
	if (UGsFairyAnimInstance* animInst = GetAnim())
	{
		animInst->SetVehicleState(isMove);
	}
}

void AGsActorFieldFairy::SetIsWaiting(bool isWaiting)
{
	if (UGsFairyAnimInstance* animInst = GetAnim())
	{
		animInst->SetWaitingState(isWaiting);
	}
}

void AGsActorFieldFairy::SetBaseEffect(const TArray<FGsSchemaFairyEffect>& baseEffectData, 
	const FGsSchemaFairyEffect* skillEffect, const FGsSchemaFairyEffect* vehicleEffect)
{
	for (auto& effect : baseEffectData)
	{
		if (!effect.ParticlePath.IsValid()) continue;


		FString particleBlueprintClassPath = effect.ParticlePath.ToString();
		TWeakObjectPtr<AGsActorFieldFairy> weakThis(this);
		GResource()->LoadAsync(particleBlueprintClassPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, effect](UObject* inRes)->void
			{
				if (weakThis.IsValid())
				{
					UParticleSystem* Particle = Cast<UParticleSystem>(inRes);
					if (auto baseParticle = weakThis->_skillEffectComponent->OnEffect(Particle, weakThis->_mesh, false,
						EPSCPoolMethod::ManualRelease,
						EAttachLocation::Type::KeepRelativeOffset,
						effect.EffectOffset,
						effect.EffectRotator,
						FVector(effect.Scale),
						*effect.SocketName))
						{
							weakThis->_baseParticles.Add(baseParticle);
						}
				}
			}));
		
	}

	_skillEffect = skillEffect;
	_vehicleEffect = vehicleEffect;
}

void AGsActorFieldFairy::SetTeleportEffect(const FSoftObjectPath& fairyTeleportStartMontagePath, const FSoftObjectPath& fairyTeleportArriveMontagePath, const TArray<UParticleSystem*>& fairyTeleportParticleList)
{
	_fairyTeleportStartMontagePath = fairyTeleportStartMontagePath;
	_fairyTeleportArriveMontagePath = fairyTeleportArriveMontagePath;
	_fairyTeleportParticleList = fairyTeleportParticleList;
}

void AGsActorFieldFairy::OnFairyWarpEffect()
{
	// [B1] | ejrrb10 | 워프 시작시, 페어리의 상태를 변경하는 로직에서 몽타주 재생으로 변경
	//if (UGsFairyAnimInstance* animInst = GetAnim())
	//{
	//	TWeakObjectPtr<AGsActorFieldFairy> thiz = this;
	//	animInst->ChangeState(EGsStateFairy::Despawn, false, nullptr);
	//}

	 UAnimMontage* fairyTeleportStartMontage = GResource()->LoadSync<UAnimMontage>(_fairyTeleportStartMontagePath);

	 if (UGsFairyAnimInstance* animInst = GetAnim())
	 {
		 animInst->Montage_Play(fairyTeleportStartMontage);
	 }
}

void AGsActorFieldFairy::OnFairySpawnEffect()
{
	// [B1] | ejrrb10 | 스폰 시에는 페어리 연출 대신 Idle 상태로 나오도록 수정 (by 이병재)
	//if (UGsFairyAnimInstance* animInst = GetAnim())
	//{
	//	TWeakObjectPtr<AGsActorFieldFairy> thiz = this;
	//	animInst->ChangeState(EGsStateFairy::Spawn, false, nullptr);
	//}

	// 지금은 하드코딩되어 있는 나쁜 코드인데, 워프 시작때처럼 몽타주 재생하도록 바꾸면 짝이 맞을 예정
	//OnFairyTeleportParticleEffect(
	//	1,
	//	FName(TEXT("FXCenterSocket")),
	//	FVector::OneVector,
	//	FVector::ZeroVector,
	//	FRotator::ZeroRotator,
	//	_mesh);

	UAnimMontage* fairyTeleportArriveMontage = GResource()->LoadSync<UAnimMontage>(_fairyTeleportArriveMontagePath);

	if (UGsFairyAnimInstance* animInst = GetAnim())
	{
		animInst->Montage_Play(fairyTeleportArriveMontage);
	}
}

void AGsActorFieldFairy::SetSelected(bool instant)
{
	if (UGsFairyAnimInstance* animInst = GetAnim())
	{
		animInst->ChangeState(EGsStateFairy::Spawn);
	}
}

void AGsActorFieldFairy::InitActorAction()
{
	specialState = 10.f;

	_fairyScale = GData()->GetGlobalData()->_fairyScale;
	FVector offset = GData()->GetGlobalData()->_fairyAttachOffset;

	if (!_fairySpawnEffect.IsValid())
	{
		_fairySpawnEffect = GData()->GetGlobalData()->_fairySpawnEffectPath;
	}



	if (_fairyScale > 0.f)
		SetActorRelativeScale3D(FVector(_fairyScale, _fairyScale, _fairyScale));
	SetActorRelativeLocation(offset);
}

void AGsActorFieldFairy::OnDestroy()
{
	//GSLOG(Log, TEXT("[[[[[[[ END ]]]]]]] AGsActorFieldFairy::OnDestroy"));
	GetWorld()->DestroyActor(this);
}

void AGsActorFieldFairy::ClearParticle()
{
	if (_baseParticles.Num() > 0)
	{
		for (auto baseParticle : _baseParticles)
		{
			if (baseParticle)
			{
				// EPSCPoolMethod::ManualRelease 로 생성된 객체는 ReleaseToPool로 해지한다.
				baseParticle->ReleaseToPool();
				baseParticle = nullptr;
			}
		}

		_baseParticles.Empty();
	}
}

void AGsActorFieldFairy::SetActorHiddenInGame(bool bNewHidden)
{
	if (IsHidden() == bNewHidden) return;

	Super::SetActorHiddenInGame(bNewHidden);

	for (auto baseParticle : _baseParticles)
	{
		if (baseParticle)
		{
			if (bNewHidden)
				 baseParticle->Deactivate();
			else 
				baseParticle->Activate(true);
		}
	}
}

void AGsActorFieldFairy::ChangeLightingChannels(bool channel1, bool channel2, bool channel3)
{
	_mesh->ChangeLightingChannels(channel1, channel2, channel3);
	_meshFace->ChangeLightingChannels(channel1, channel2, channel3);
}

//void AGsActorFieldFairy::AnimNoti_FairySkillEffect()
//{
//	if (_skillEffect && _skillEffect->ParticlePath.IsValid())
//	{
//		FString particleBlueprintClassPath = _skillEffect->ParticlePath.ToString();
//		UParticleSystem* particle = LoadObject<UParticleSystem>(this, *particleBlueprintClassPath);
//		
//		_skillEffectComponent->OnEffect(particle, _mesh, true, EPSCPoolMethod::AutoRelease,
//			EAttachLocation::Type::KeepRelativeOffset, FVector::ZeroVector, FRotator::ZeroRotator, FVector(_skillEffect->Scale),
//			*(_skillEffect->SocketName.ToString()));
//	}
//}

bool AGsActorFieldFairy::AnimNoti_AttachFairySkillEffect(FName socket, FVector scale, FVector locOffset, FRotator rotOffset, USkeletalMeshComponent* mesh)
{
	if (mesh == nullptr) mesh = _mesh;
	if (_skillEffect && _skillEffect->ParticlePath.IsValid())
	{
		FString particleBlueprintClassPath = _skillEffect->ParticlePath.ToString();
		
		if (_isAsyncSkillEffect)
		{
			TWeakObjectPtr<AGsActorFieldFairy> weakThis(this);
			GResource()->LoadAsync(particleBlueprintClassPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis, socket, scale, locOffset, rotOffset, mesh](UObject* inRes)->void
				{
					if (weakThis.IsValid())
					{
						weakThis->_skillEffectComponent->OnEffect(Cast<UParticleSystem>(inRes), mesh, true, EPSCPoolMethod::AutoRelease,
							EAttachLocation::Type::KeepRelativeOffset, locOffset, rotOffset, scale,
							socket != NAME_None ? socket : SpawnEffectSocketName);
					}					
				}));
			return true;
		}
		else
		{
			UParticleSystem* particle = LoadObject<UParticleSystem>(this, *particleBlueprintClassPath);
			if (_skillEffectComponent->OnEffect(particle, mesh, true, EPSCPoolMethod::AutoRelease,
				EAttachLocation::Type::KeepRelativeOffset, locOffset, rotOffset, scale,
				socket != NAME_None ? socket : SpawnEffectSocketName))
				return true;
		}
	}
	return false;
}

void AGsActorFieldFairy::OnActiveVehicleEffect()
{
	if (_vehicleEffect)
	{
		TWeakObjectPtr<AGsActorFieldFairy> weakThis(this);
		const FGsSchemaFairyEffect* effectTable = _vehicleEffect;
		USceneComponent* targetComponent = _mesh;
		FString particleBlueprintClassPath = effectTable->ParticlePath.ToString();
		GResource()->LoadAsync(particleBlueprintClassPath, FStreamableDelegateParam::CreateWeakLambda(this,
			[weakThis, targetComponent, effectTable](UObject* inRes)->void
			{
				if (weakThis.IsValid())
				{
					FName socketName = NAME_None;
					if (false == effectTable->SocketName.IsEmpty())
					{
						socketName = (*effectTable->SocketName);
					}
					if (UParticleSystemComponent* particleComponent = weakThis->_skillEffectComponent->OnEffect(
						Cast<UParticleSystem>(inRes), targetComponent, false, EPSCPoolMethod::ManualRelease,
						EAttachLocation::Type::KeepRelativeOffset, effectTable->EffectOffset,
						effectTable->EffectRotator, FVector(effectTable->Scale),
						socketName != NAME_None ? socketName : SpawnEffectSocketName))
					{
						weakThis->_baseParticles.Add(particleComponent);
					}
				}				
			}));

		if (const FGsSchemaSoundResData* soundRes = _vehicleEffect->SoundRes.GetRow())
		{
			if (UGsSoundPlayer* soundPlayer = GSoundPlayer())
			{
				soundPlayer->PlaySoundTarget(soundRes, RootComponent);
			}
		}
	}
}

void AGsActorFieldFairy::OnDeactiveVehicleEffect()
{
	if (_vehicleEffect)
	{
		FString deactiovePath = _vehicleEffect->ParticlePath.GetLongPackageName();
		int32 idx = _baseParticles.IndexOfByPredicate(
			[deactiovePath](UParticleSystemComponent* el)
			{
				FString path = el->Template->GetPackage()->GetPathName();
				if (deactiovePath.Equals(path))
				{
					return true;
				}
				return false;
			});
		if (idx != -1)
		{
			_baseParticles[idx]->ReleaseToPool();
			_baseParticles.RemoveAt(idx);
		}
	}
}

UParticleSystem* AGsActorFieldFairy::GetPSTemplate()
{
	if (_skillEffect && _skillEffect->ParticlePath.IsValid())
	{
		FString particleBlueprintClassPath = _skillEffect->ParticlePath.ToString();
		return LoadObject<UParticleSystem>(this, *particleBlueprintClassPath);
	}
	return nullptr;
}

void AGsActorFieldFairy::GetSkeletalMeshs(TArray<USkeletalMeshComponent*>& Out_skeletalMesh)
{
	Out_skeletalMesh.Add(_mesh);
	Out_skeletalMesh.Add(_meshFace);

}

void AGsActorFieldFairy::OnFairyTeleportParticleEffect(ETeleportEffectType teleportEffectType, FName socket, FVector scale, FVector locOffset, FRotator rotOffset, USkeletalMeshComponent* MeshComp, bool attached)
{
	if (MeshComp == nullptr) 
		MeshComp = _mesh;

	if ((uint8) teleportEffectType >= _fairyTeleportParticleList.Num())
	{
		GSLOG(Warning, TEXT("Teleport particle effect for player is not properly set in the table!!!"));
		return;
	}
	else
	{
		if (UParticleSystem* particle = _fairyTeleportParticleList[(uint8)teleportEffectType])
		{
			if (attached)
			{
				UGameplayStatics::SpawnEmitterAttached(
					particle,
					MeshComp,
					socket,
					locOffset,
					rotOffset,
					scale,
					EAttachLocation::SnapToTargetIncludingScale,
					true,
					EPSCPoolMethod::AutoRelease);
			}
			else
			{
				UGameplayStatics::SpawnEmitterAtLocation(
					GetWorld(),
					particle,
					GetActorLocation(),
					rotOffset,
					scale,
					true,
					EPSCPoolMethod::AutoRelease
				);
			}
		}
	}
}