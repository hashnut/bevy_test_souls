// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorEx/GsCompleteCamp.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GsCharacterLocalPlayer.h"
#include "ActorComponentEx/GsWidgetComponentPooling.h"
#include "ActorComponentEx/GsWidgetComponentButton.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsCampManager.h"
#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "UTIL/GsLevelUtil.h"
#include "UI/UIContent/Billboard/GsUIBillboardCamp.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Materials/MaterialInterface.h"



AGsCompleteCamp::AGsCompleteCamp() : Super()
{
	USkeletalMeshComponent* skelMeshComp = GetMesh();
	if (skelMeshComp)
	{
		// 인터랙션 버튼
		_interactionWidget = CreateDefaultSubobject<UGsWidgetComponentButton>(InteractionButtonComponentName);
		_interactionWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
		_interactionWidget->SetWidgetSpace(EWidgetSpace::Screen);
		_interactionWidget->SetDrawSize(FVector2D(100.f, 100.f));
		_interactionWidget->SetCastShadow(false);
		_interactionWidget->SetupAttachment(skelMeshComp);
		_interactionWidget->SetVisibility(false);
	}

	// Hp Bar Widget
	_hpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	_hpBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f)); // 인게임으로 사이즈 확인 필요
	_hpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	_hpBarWidget->SetDrawSize(FVector2D(200, 50.0f)); // 인게임으로 사이즈 확인 필요
	_hpBarWidget->SetCastShadow(false);
	_hpBarWidget->SetupAttachment(RootComponent);
	_hpBarWidget->SetVisibility(true);

	_idleParticleEffect = CreateDefaultSubobject<UParticleSystem>(TEXT("idleParticleEffect"));
	_damageParticleEffect85 = CreateDefaultSubobject<UParticleSystem>(TEXT("damageParticleEffect85"));
	_damageParticleEffect50 = CreateDefaultSubobject<UParticleSystem>(TEXT("damageParticleEffect50"));
	_damageParticleEffect25 = CreateDefaultSubobject<UParticleSystem>(TEXT("damageParticleEffect25"));

	_nameplateWidget->SetVisibility(false);
}

void AGsCompleteCamp::EndPlay(const EEndPlayReason::Type in_type)
{
	OffIdleEffect();
	OffDamageEffect(CampParticleType::DamageFire25Type);
	OffDamageEffect(CampParticleType::DamageFire50Type);
	OffDamageEffect(CampParticleType::DamageFire85Type);

	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearAllTimersForObject(this);
	}

	Super::EndPlay(in_type);
}

void AGsCompleteCamp::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitializeDecals();
}

void AGsCompleteCamp::BeginPlay()
{
	Super::BeginPlay();

	FVector originVec;
	this->GetActorBounds(true, originVec, _actorBoundsExtends);

	USkeletalMeshComponent* skelMeshComp = GetMesh();
	if (skelMeshComp)
	{
		_actorMeshBoundsExtends = skelMeshComp->Bounds.BoxExtent;
	}

	//float resultZ;
	//resultZ = originVec.Z - (_actorBoundsExtends.Z / 2.f);
	//GSLOG(Warning, TEXT("resultZ : %f "), resultZ);

	_completeCampAnim = Cast<UGsCompleteCampAnimInstance>(GetMesh()->GetAnimInstance());

	UUserWidget* widget = _hpBarWidget->GetUserWidgetObject();
	UGsUIBillboardCamp* campWidget = Cast<UGsUIBillboardCamp>(widget);
	if (nullptr != campWidget)
	{
		_hpBillboardPtr = campWidget;
	}

	OffIdleEffect();
	OffDamageEffect(CampParticleType::DamageFire25Type);
	OffDamageEffect(CampParticleType::DamageFire50Type);
	OffDamageEffect(CampParticleType::DamageFire85Type);

	GetCapsuleComponent()->SetCanEverAffectNavigation(true);
}

void AGsCompleteCamp::SetSpawnIndex(uint8 InNum)
{
	if (nullptr != _completeCampAnim)
	{
		_completeCampAnim->SetSpawnTypeIndex(InNum);
	}
}

void AGsCompleteCamp::FinishedAnim()
{
	if (nullptr != _completeCampAnim)
	{
		EGsStateCampComplete type = _completeCampAnim->GetCurStateType();
		switch (type)
		{
		case EGsStateCampComplete::None:
		case EGsStateCampComplete::PreSpawn_CompleteCamp:
			_completeCampAnim->ChangeState(EGsStateCampComplete::Spawn_CompleteCamp);
			break;
		case EGsStateCampComplete::Hit_CompleteCamp:
		case EGsStateCampComplete::Spawn_CompleteCamp:
			_completeCampAnim->ChangeState(EGsStateCampComplete::Idle_CompleteCamp);
			break;
		case EGsStateCampComplete::Destroy_CompleteCamp:
			break;
		}
	}
}

void AGsCompleteCamp::DestoryPorc()
{
	Destroy();
}

void AGsCompleteCamp::InitializeDecals()
{
	float fdecalHeight = 50.0f;
	float fdecalSize = 64.0f;
	UCapsuleComponent* capsule = GetCapsuleComponent();
	if (nullptr != capsule)
	{
		fdecalSize = capsule->GetUnscaledCapsuleRadius() * RootComponent->GetComponentScale().X;
		fdecalHeight = capsule->GetUnscaledCapsuleHalfHeight();
	}
	if (_shadowDecal)
	{
		_shadowDecal->SetRelativeLocation(FVector(0, 0, -fdecalHeight));
		_shadowDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		_shadowDecal->SetFadeIn(0.0f, 0.1f);

		fdecalSize *= 0.66f;
		_shadowDecal->DecalSize = FVector(fdecalSize, fdecalSize * 2.0f, fdecalSize * 2.0f);
	}
}

UGsUIBillboardInteractionButton* AGsCompleteCamp::GetInteractionButton() const
{
	if (_interactionWidget)
	{
		return Cast<UGsUIBillboardInteractionButton>(_interactionWidget->GetUserWidgetObject());
	}
	return nullptr;
}


void AGsCompleteCamp::InitWidgetComponent()
{
	Super::InitWidgetComponent();

	// 인터랙션 버튼
	FTransform trans = GetMesh()->GetSocketTransform(TEXT("OverHeadSocket"), RTS_Component);
	if (_interactionWidget)
	{
		if (nullptr == _interactionWidget->GetWidgetClass().Get())
		{
			if (UGsPathManager* pathMgr = GPath())
			{
				UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("InteractionButton"));
				if (widgetClass)
				{
					_interactionWidget->SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
				}
			}
		}

		FVector pos = trans.GetTranslation();
		FVector finalPos = pos - FVector(0, 0, 400.f);
		_interactionWidget->SetRelativeLocation(finalPos);
	}
}

void AGsCompleteCamp::ClickInteraction()
{
	if (_callbackInteractionClick != nullptr)
	{
		_callbackInteractionClick(false);
	}
}

// 인터렉션 콜백 등록함수
void AGsCompleteCamp::SetInteractionClickCallback(TFunction<void(bool)> In_callback)
{
	_callbackInteractionClick = In_callback;
	if (_interactionWidget != nullptr)
	{
		if (UGsWidgetComponentButton* wcButton = Cast<UGsWidgetComponentButton>(_interactionWidget))
		{
			wcButton->SetClickCallback(In_callback);
		}
	}
}
	
void AGsCompleteCamp::SetUseInteractionButton(bool In_isUse)
{
	if (IsSanctum())
	{
		SetUseInteractionButtonSanctum(In_isUse);
	}
	else
	{
		// 인터랙션 버튼
		if (_interactionWidget)
		{
			if (In_isUse == true)
			{
				if (nullptr == _interactionWidget->GetUserWidgetObject())
				{
					_interactionWidget->InitWidget();
				}
			}
		}
	}
}

void AGsCompleteCamp::SetVisibleShadowDecal(bool InVisible)
{
	if (nullptr != _shadowDecal)
	{
		_shadowDecal->SetVisibility(InVisible);
		_shadowDecal->SetComponentTickEnabled(InVisible);
	}
}

void AGsCompleteCamp::CheckDamageEffect(const float InCurValue)
{
	if (0.f >= InCurValue)
	{
		OffIdleEffect();
		OffDamageEffect(CampParticleType::DamageFire25Type);
		OffDamageEffect(CampParticleType::DamageFire50Type);
		OffDamageEffect(CampParticleType::DamageFire85Type);

		BeginDestroyAnim();
	}
	else if (DAMAGE_EFFECT_THIRD >= InCurValue)
	{
		OnDamageEffect(CampParticleType::DamageFire25Type);
		OffDamageEffect(CampParticleType::DamageFire50Type);
		OffDamageEffect(CampParticleType::DamageFire85Type);
	}
	else if (DAMAGE_EFFECT_SECOND >= InCurValue)
	{
		OnDamageEffect(CampParticleType::DamageFire50Type);
		OffDamageEffect(CampParticleType::DamageFire25Type);
		OffDamageEffect(CampParticleType::DamageFire85Type);
	}
	else if (DAMAGE_EFFECT_FIRST >= InCurValue)
	{
		OnDamageEffect(CampParticleType::DamageFire85Type);
		OffDamageEffect(CampParticleType::DamageFire25Type);
		OffDamageEffect(CampParticleType::DamageFire50Type);
	}
	else
	{
		OffDamageEffect(CampParticleType::DamageFire85Type);
		OffDamageEffect(CampParticleType::DamageFire25Type);
		OffDamageEffect(CampParticleType::DamageFire50Type);
	}
}

void AGsCompleteCamp::AllOffDamageEffect()
{
	OffDamageEffect(CampParticleType::DamageFire25Type);
	OffDamageEffect(CampParticleType::DamageFire50Type);
	OffDamageEffect(CampParticleType::DamageFire85Type);
}

void AGsCompleteCamp::OnDamageEffect(const CampParticleType InParticleType)
{
	switch (InParticleType)
	{
	case CampParticleType::DamageFire25Type:
	{
		if (nullptr == _campDamage25ParticleComp)
		{
			_campDamage25ParticleComp = UGameplayStatics::SpawnEmitterAttached(_damageParticleEffect25, GetMesh(), NAME_None, _damageParticleEffect25Pos, FRotator::ZeroRotator,
				FVector(1.f), EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::ManualRelease, true);
		}
		break;
	}	
	case CampParticleType::DamageFire50Type:
	{
		if (nullptr == _campDamage50ParticleComp)
		{
			_campDamage50ParticleComp = UGameplayStatics::SpawnEmitterAttached(_damageParticleEffect50, GetMesh(), NAME_None, _damageParticleEffect50Pos, FRotator::ZeroRotator,
				FVector(1.f), EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::ManualRelease, true);
		}
		break;
	}
	case CampParticleType::DamageFire85Type:
	{
		if (nullptr == _campDamage85ParticleComp)
		{
			_campDamage85ParticleComp = UGameplayStatics::SpawnEmitterAttached(_damageParticleEffect85, GetMesh(), NAME_None, _damageParticleEffect85Pos, FRotator::ZeroRotator,
				FVector(1.f), EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::ManualRelease, true);
		}
		break;
	}
	}
}

void AGsCompleteCamp::OffDamageEffect(const CampParticleType InParticleType, bool InImmediately)
{
	switch (InParticleType)
	{
	case CampParticleType::DamageFire25Type:
	{
		if (nullptr != _campDamage25ParticleComp)
		{
			if (InImmediately)
			{
				_campDamage25ParticleComp->SetVisibility(false);
			}
			_campDamage25ParticleComp->ReleaseToPool();
			_campDamage25ParticleComp = nullptr;
		}
		break;
	}
	case CampParticleType::DamageFire50Type:
	{
		if (nullptr != _campDamage50ParticleComp)
		{
			if (InImmediately)
			{
				_campDamage50ParticleComp->SetVisibility(false);
			}
			_campDamage50ParticleComp->ReleaseToPool();
			_campDamage50ParticleComp = nullptr;
		}
		break;
	}
	case CampParticleType::DamageFire85Type:
	{
		if (nullptr != _campDamage85ParticleComp)
		{
			if (InImmediately)
			{
				_campDamage85ParticleComp->SetVisibility(false);
			}
			_campDamage85ParticleComp->ReleaseToPool();
			_campDamage85ParticleComp = nullptr;
		}
		break;
	}
	}
}

void AGsCompleteCamp::OnIdleEffect()
{
	_campIdleParticleComp = UGameplayStatics::SpawnEmitterAttached(_idleParticleEffect, GetMesh(), NAME_None, _idleParticleEffectPos, FRotator::ZeroRotator,
		FVector(1.f), EAttachLocation::KeepRelativeOffset, true, EPSCPoolMethod::ManualRelease, true);
}

void AGsCompleteCamp::OffIdleEffect(bool InImmediately)
{
	// ReleaseToPool() 하면 내부에서 Deactive() 호출함
	if (nullptr != _campIdleParticleComp)
	{
		if (InImmediately)
		{
			_campIdleParticleComp->SetVisibility(false);
		}
		_campIdleParticleComp->ReleaseToPool();
		_campIdleParticleComp = nullptr;
	}
}

void AGsCompleteCamp::SetHpBarWidgetProgress(const float InValue)
{
	if (_hpBillboardPtr.IsValid())
	{
		_hpBillboardPtr->SetProgressBarValue(InValue);
		CheckDamageEffect(InValue);
	}
}

void AGsCompleteCamp::SetCampName(const FText InText)
{
	if (_hpBillboardPtr.IsValid())
	{
		_hpBillboardPtr->SetCampNameText(InText);
	}
}

void AGsCompleteCamp::SetCampEmblemIcon(const uint8 InEmblemId)
{
	if (_hpBillboardPtr.IsValid())
	{
		_hpBillboardPtr->SetCampEmblemIcon(InEmblemId);
	}
}

void AGsCompleteCamp::SetHpBarAndNameWidgetVisibility(const bool InActive)
{
	_hpBarWidget->SetVisibility(InActive);
}
	
bool AGsCompleteCamp::IsVisibleShadowDecal() const
{
	if (nullptr == _shadowDecal)
		return false;

	return _shadowDecal->IsVisible();
}

void AGsCompleteCamp::ChangeAnimState(EGsStateCampComplete InType)
{
	if (nullptr == _completeCampAnim)
	{
		_completeCampAnim = Cast<UGsCompleteCampAnimInstance>(GetMesh()->GetAnimInstance());
		if (nullptr == _completeCampAnim)
			return;
	}

	_completeCampAnim->ChangeState(InType);
}

void AGsCompleteCamp::BeginDestroyAnim()
{
	if (nullptr == _completeCampAnim)
	{
		_completeCampAnim = Cast<UGsCompleteCampAnimInstance>(GetMesh()->GetAnimInstance());
		if (nullptr == _completeCampAnim)
			return;
	}
	_completeCampAnim->ChangeState(EGsStateCampComplete::Destroy_CompleteCamp);
	SetHpBarAndNameWidgetVisibility(false);

	OffIdleEffect();
	OffDamageEffect(CampParticleType::DamageFire25Type);
	OffDamageEffect(CampParticleType::DamageFire50Type);
	OffDamageEffect(CampParticleType::DamageFire85Type);
}

void AGsCompleteCamp::BeginHitAnim()
{
	if (nullptr == _completeCampAnim)
	{
		_completeCampAnim = Cast<UGsCompleteCampAnimInstance>(GetMesh()->GetAnimInstance());
		if (nullptr == _completeCampAnim)
			return;
	}

	if (EGsStateCampComplete::Hit_CompleteCamp == _completeCampAnim->GetCurStateType())
		return;

	_completeCampAnim->ChangeState(EGsStateCampComplete::Hit_CompleteCamp);
}

FVector AGsCompleteCamp::GetActorBoundsExtends()
{
	return _actorBoundsExtends;
}

FVector AGsCompleteCamp::GetActorMeshBoundsExtends()
{
	return _actorMeshBoundsExtends;
}

UGsUIBillboardCamp* AGsCompleteCamp::GetCampNameplateWidget() const
{
	if (_hpBarWidget)
	{
		return Cast<UGsUIBillboardCamp>(_hpBarWidget->GetUserWidgetObject());
	}
	return nullptr;
}

FVector AGsCompleteCamp::GetNavModifierSizeVec()
{
	return _navModifierSize;
}

void AGsCompleteCamp::SetNavigationAddOffsetValue(FVector InVec)
{
	_navigationAddOffset = InVec;
}

void AGsCompleteCamp::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

	if (Other)
	{
		AGsCharacterLocalPlayer* localActor = Cast<AGsCharacterLocalPlayer>(Other);
		if (localActor)
		{
			if (false == _localPlyerPtr.IsValid())
			{
				if (false == FindLocalPlayerPtr())
					return;
			}
			_localPlyerPtr.Get()->OnHitNotifyByCampObject(this);
		}
	}
}

void AGsCompleteCamp::SetUseInteractionButtonSanctum(bool InUse)
{
	if (nullptr == _interactionWidget)
	{
		return;
	}

	// InitWidgetComponent 쪽 코드 때문에 너무 내려가 있어서 강제로 기본 값으로 올렸다
	_interactionWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));

	if (InUse)
	{
		if (nullptr == _interactionWidget->GetUserWidgetObject())
		{
			_interactionWidget->InitWidget();
		}

		_interactionWidget->SetActive(true);
		_interactionWidget->SetVisibility(true);
	}
	else
	{
		if (UGsUIBillboardInteractionButton* interactionButton = GetInteractionButton())
		{
			interactionButton->ReleaseResource();
		}

		_interactionWidget->SetActive(false);
		_interactionWidget->SetVisibility(false);
	}
}

void AGsCompleteCamp::PlayAnimationHit()
{
	BeginHitAnim();
}

void AGsCompleteCamp::PlayAnimationDestroy()
{
	BeginDestroyAnim();
}

void AGsCompleteCamp::SetDammageEffect(float InRate)
{
	CheckDamageEffect(InRate);
}

void AGsCompleteCamp::ShowHpBar(bool bVisible)
{
	if (UGsUIBillboardCamp* billboardCamp = GetCampNameplateWidget())
	{
		billboardCamp->ShowHPBar(bVisible);
	}
}

void AGsCompleteCamp::SetMaterial(UMaterialInterface* InMaterial)
{
	if (USkeletalMeshComponent* skeletalMesh = GetMesh())
	{
		skeletalMesh->SetMaterial(0, InMaterial);
	}
}

void AGsCompleteCamp::SetMaterialColor(bool bInEnable)
{
	if (USkeletalMeshComponent* skeletalMesh = GetMesh())
	{
		if (bInEnable)
		{
			// 푸른색(인터렉션 가능 색)
			skeletalMesh->SetVectorParameterValueOnMaterials(TEXT("Ghost Color"), FVector(0.f, 0.235638f, 1.498511f));
			skeletalMesh->SetVectorParameterValueOnMaterials(TEXT("Base Color"), FVector(0.f, 1.0f, 0.647124f));

		}
		else
		{
			// 붉은색(인터렉션 불가능 색)
			skeletalMesh->SetVectorParameterValueOnMaterials(TEXT("Ghost Color"), FVector(1.498511f, 0.203f, 0.f));
			skeletalMesh->SetVectorParameterValueOnMaterials(TEXT("Base Color"), FVector(0.647124f, 0.422f, 0.f));
		}
	}
}
