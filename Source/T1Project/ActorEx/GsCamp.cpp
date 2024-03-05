// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorEx/GsCamp.h"
#include "ActorEx/GsCompleteCamp.h"
#include "ActorComponentEx/GsEffectComponent.h"
#include "ActorComponentEx/GsQueryFloorHeightComponent.h"
#include "ActorComponentEx/GsWidgetComponentPooling.h"
#include "GsCharacterLocalPlayer.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "NavigationSystem/Public/NavModifierComponent.h"

#include "Components/WidgetComponent.h"
#include "ActorComponentEx/GsWidgetComponentButton.h"
#include "ActorComponentEx/GsCampBuildableQueryComponent.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsCampManager.h"
#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "UTIL/GsLevelUtil.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Billboard/GsUIBillboardCamp.h"
#include "SphereTree/GsSphereTree.h"
#include "Animation/GsCampAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationSystem/Public/NavMesh/RecastNavMesh.h"
#include "T1Project.h"


//for Debug
#include "DrawDebugHelpers.h"


AGsCamp::AGsCamp() : Super()
{
	USkeletalMeshComponent* skelMeshComp = GetMesh();
	if (skelMeshComp)
	{
		// 인터랙션 버튼
		_interactionWidget = CreateDefaultSubobject<UGsWidgetComponentButton>(InteractionButtonComponentName);
		_interactionWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 500.0f));
		_interactionWidget->SetWidgetSpace(EWidgetSpace::Screen);
		_interactionWidget->SetDrawSize(FVector2D(200.f, 200.f));
		_interactionWidget->SetCastShadow(false);
		_interactionWidget->SetupAttachment(skelMeshComp);

		_interactionParticle_1stComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("InteractionParticle1st"));
		_interactionParticle_1stComp->SetupAttachment(skelMeshComp);

		_interactionParticle_2ndComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("InteractionParticle2nd"));
		_interactionParticle_2ndComp->SetupAttachment(skelMeshComp);

		_interactionParticle_3rdComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("InteractionParticle3rd"));
		_interactionParticle_3rdComp->SetupAttachment(skelMeshComp);
	}

	// Hp Bar Widget
	_hpBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	_hpBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f)); // 인게임으로 사이즈 확인 필요
	_hpBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	_hpBarWidget->SetDrawSize(FVector2D(200, 50.0f)); // 인게임으로 사이즈 확인 필요
	_hpBarWidget->SetCastShadow(false);
	_hpBarWidget->SetupAttachment(RootComponent);
	_hpBarWidget->SetVisibility(true);

	_nameplateWidget->SetVisibility(false);

	GetCapsuleComponent()->SetEnableGravity(false);

	_localPlyerPtr.Reset();
}

void AGsCamp::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitializeDecals();
}

// Called when the game starts or when spawned
void AGsCamp::BeginPlay()
{
	Super::BeginPlay();

	//findComps1 = GetComponentsByClass(UNavModifierComponent::StaticClass());
	// 추후 bp 에서 bone attach 시키고자 할때, 사용될 코드라서 지우지 마세요
	/*
	USkeletalMeshComponent* skelMeshComp = GetMesh();
	if (skelMeshComp)
	{
		TArray<USceneComponent*> childComponentList;
		TArray<UParticleSystemComponent*> compList;
		skelMeshComp->GetChildrenComponents(true, childComponentList);
		for (USceneComponent* iter : childComponentList)
		{
			if ("ParticleTest1" == iter->GetName())
			{
				GSLOG(Error, TEXT("FInd !!!!!!!!!!!!!!!!!!!!!!!!!"));
			}
		}
	}
	*/

	USkeletalMeshComponent* skelMeshComp = GetMesh();
	if (skelMeshComp)
	{
		_actorMeshBoundsExtends = skelMeshComp->Bounds.BoxExtent;
	}

	this->GetActorBounds(true, _actorBoundsOrigin, _actorBoundsExtends);

	_campAnim = Cast<UGsCampAnimInstance>(GetMesh()->GetAnimInstance());

	UUserWidget* widget = _hpBarWidget->GetUserWidgetObject();
	UGsUIBillboardCamp* campWidget = Cast<UGsUIBillboardCamp>(widget);
	if (nullptr != campWidget)
	{
		_hpBillboardPtr = campWidget;
	}

	OffInteractionEffect();

	SetAffectNavigation(true);
	GetCapsuleComponent()->SetEnableGravity(false);
	if (GetMesh())
	{
		GetMesh()->SetEnableGravity(false);
	}
}

void AGsCamp::SetAffectNavigation(const bool IsState)
{
	if (GetCapsuleComponent())
	{
		if (IsState != GetCapsuleComponent()->CanEverAffectNavigation())
		{
			GetCapsuleComponent()->SetCanEverAffectNavigation(IsState);
		}
	}
}

void AGsCamp::EndPlay(const EEndPlayReason::Type in_type)
{
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearAllTimersForObject(this);
	}

	Super::EndPlay(in_type);
}

void AGsCamp::DestoryPorc()
{
	Destroy();
}

void AGsCamp::InitializeDecals()
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

void AGsCamp::SetVisibleShadowDecal(bool InVisible)
{
	if (nullptr != _shadowDecal)
	{
		_shadowDecal->SetVisibility(InVisible);
		_shadowDecal->SetComponentTickEnabled(InVisible);
	}
}


bool AGsCamp::IsVisibleShadowDecal() const
{
	if (nullptr == _shadowDecal)
		return false;

	return _shadowDecal->IsVisible();
}

void AGsCamp::FinishedAnim()
{
	if (nullptr != _campAnim)
	{
		EGsStateCamp type = _campAnim->GetCurStateType();
		//GSLOG(Warning, TEXT("[AGsCamp::FinishedAnim()] - type : %d") ,(uint8)type);
		switch (type)
		{
		case EGsStateCamp::Building_Camp:
		case EGsStateCamp::Hit_Camp:
		case EGsStateCamp::BuildingComplete:
			_campAnim->ChangeState(EGsStateCamp::Idle_Camp);
			break;
		case EGsStateCamp::Destroy_Camp:
			break;
		case EGsStateCamp::Idle_Camp:
			break;
		}
	}	
}

void AGsCamp::ClickInteraction()
{
	if (_callbackInteractionClick != nullptr)
	{
		_callbackInteractionClick(false);
	}
}

void AGsCamp::InitWidgetComponent()
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

void AGsCamp::SetUseInteractionButton(bool In_isUse)
{
	if (IsSanctum())
	{
		SetUseInteractionButtonSanctum(In_isUse);
	}
	else
	{
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

// 인터렉션 콜백 등록함수
void AGsCamp::SetInteractionClickCallback(TFunction<void(bool)> In_callback)
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

UGsUIBillboardInteractionButton* AGsCamp::GetInteractionButton() const
{
	if (_interactionWidget)
	{
		return Cast<UGsUIBillboardInteractionButton>(_interactionWidget->GetUserWidgetObject());
	}

	return nullptr;
}

void AGsCamp::SetHpBarWidgetProgress(const float InValue)
{
	if (_hpBillboardPtr.IsValid())
	{
		_hpBillboardPtr->SetProgressBarValue(InValue);
	}
}

void AGsCamp::SetCampName(const FText InText)
{
	if (_hpBillboardPtr.IsValid())
	{
		_hpBillboardPtr->SetCampNameText(InText);
	}
}

void AGsCamp::SetHpBarAndNameWidgetVisibility(const bool InActive)
{
	_hpBarWidget->SetVisibility(InActive);
}

float AGsCamp::GetHpBarValue()
{
	if (_hpBillboardPtr.IsValid())
	{
		return _hpBillboardPtr->GetCurProgressBarValue();
	}

	return 0.f;
}

float AGsCamp::GetCampDiameter()
{
	return _actorBoundsExtends.X + _actorBoundsExtends.Y;
}

void AGsCamp::BeginDestroyAnim()
{
	_campAnim->ChangeState(EGsStateCamp::Destroy_Camp);
	SetHpBarAndNameWidgetVisibility(false);
}

void AGsCamp::BeginHitAnim()
{
	if (EGsStateCamp::Building_Camp == _campAnim->GetCurStateType() || 
		EGsStateCamp::Hit_Camp == _campAnim->GetCurStateType())
		return;

	_campAnim->ChangeState(EGsStateCamp::Hit_Camp);
}

UWidgetComponent* AGsCamp::GetCampInteractionWidgetComponent()
{
	return _interactionWidget;
}

void AGsCamp::OnInteractionEffect(UParticleSystemComponent* InTargetComp)
{
	if (nullptr == InTargetComp || nullptr == InTargetComp->Template)
		return;

	InTargetComp->Activate();
}

void AGsCamp::OffInteractionEffect(UParticleSystemComponent* InTargetComp)
{
	if (nullptr == InTargetComp || nullptr == InTargetComp->Template)
		return;

	InTargetComp->Deactivate();
}

void AGsCamp::OnInteractionEffect()
{
	OffInteractionEffect(_interactionParticle_1stComp);
	OffInteractionEffect(_interactionParticle_2ndComp);
	OffInteractionEffect(_interactionParticle_3rdComp);

	if (_campAnim)
	{
		int32 curStepNum = _campAnim->GetCurStepNum();
		switch (curStepNum)
		{
		case 0:
			OnInteractionEffect(_interactionParticle_1stComp);
			OffInteractionEffect(_interactionParticle_2ndComp);
			OffInteractionEffect(_interactionParticle_3rdComp);
			break;
		case 1:
			OnInteractionEffect(_interactionParticle_2ndComp);
			OffInteractionEffect(_interactionParticle_1stComp);
			OffInteractionEffect(_interactionParticle_3rdComp);
			break;
		case 2:
			OnInteractionEffect(_interactionParticle_3rdComp);
			OffInteractionEffect(_interactionParticle_1stComp);
			OffInteractionEffect(_interactionParticle_2ndComp);
			break;
		}
	}
}

void AGsCamp::OffInteractionEffect()
{
	OffInteractionEffect(_interactionParticle_1stComp);
	OffInteractionEffect(_interactionParticle_2ndComp);
	OffInteractionEffect(_interactionParticle_3rdComp);
}

FVector AGsCamp::GetActorBoundsExtends()
{
	return _actorBoundsExtends;
}

FVector AGsCamp::GetActorMeshBoundsExtends()
{
	return _actorMeshBoundsExtends;
}

UGsUIBillboardCamp* AGsCamp::GetCampNameplateWidget() const
{
	if (_hpBarWidget)
	{
		return Cast<UGsUIBillboardCamp>(_hpBarWidget->GetUserWidgetObject());
	}
	return nullptr;
}

FVector AGsCamp::GetNavModifierSizeVec()
{
	return _navModifierSize;
}

void AGsCamp::SetNavigationAddOffsetValue(FVector InVec)
{
	_navigationAddOffset = InVec;
}

void AGsCamp::SetCampEmblemIcon(const uint8 InEmblemId)
{
	if (_hpBillboardPtr.IsValid())
	{
		_hpBillboardPtr->SetCampEmblemIcon(InEmblemId);
	}
}

void AGsCamp::NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
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

void AGsCamp::SetUseInteractionButtonSanctum(bool InUse)
{
	if (nullptr == _interactionWidget)
	{
		return;
	}

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

void AGsCamp::PlayAnimationHit()
{
	BeginHitAnim();
}

void AGsCamp::PlayAnimationDestroy()
{
	BeginDestroyAnim();
}

void AGsCamp::PlayAnimationConstructionStep(int32 InStep, bool bIsNormal)
{
	const int32 MAX_CAMP_ANIM_STEP = 2;

	// InStep은 0부터 시작하는 값이고 서버에서 0~3으로 4개가 오는데,
	// 애니는 3개라서 3번 애니 두번 출력 함
	if (UGsCampAnimInstance* campAnim = Cast<UGsCampAnimInstance>(GetAnimationInstance()))
	{
		// 현재 상태면 무시
		if (campAnim->GetCurStepNum() == InStep)
		{
			return;
		}

		int32 currStep = (MAX_CAMP_ANIM_STEP > InStep) ? InStep : MAX_CAMP_ANIM_STEP;
		campAnim->SetCampCurrentStep(currStep);
		
		if (bIsNormal)
		{
			campAnim->ChangeState(EGsStateCamp::Building_Camp);
		}
		else
		{
			campAnim->ChangeState(EGsStateCamp::Idle_Camp);
		}
	}
}
