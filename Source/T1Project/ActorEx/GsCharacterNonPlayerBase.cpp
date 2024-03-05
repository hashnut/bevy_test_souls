// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCharacterNonPlayerBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"

#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/AIModule/Classes/AIController.h"

#include "Components/WidgetComponent.h"

#include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "ActorComponentEx/GsSkillCollisioHandlerComponent.h"
#include "ActorComponentEx/GsSkillGuideEffectComponent.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "T1Project/Sequence/GsSequencePlayer.h"
#include "GameObject/Define/GsGameObjectText.h"
#include "Management/ScopeGlobal/GsPathManager.h"

#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"
#include "UI/UIContent/Billboard/GsUIBillboardTalkBalloon.h"

#include "ActorComponentEx/GsUROComponent.h"
#include "ActorComponentEx/GsWidgetComponentPooling.h"
#include "ActorComponentEx/GsWidgetComponentTalkBalloon.h"

#include "Components/SphereComponent.h"
#include "TimerManager.h"

#include "Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "Engine/Classes/Materials/MaterialParameterCollection.h"
#include "Engine/Classes/Materials/MaterialParameterCollectionInstance.h"

// Sets default values
AGsCharacterNonPlayerBase::AGsCharacterNonPlayerBase() : Super()
{
	AutoPossessAI = EAutoPossessAI::Spawned;	

	_skillCollision = CreateDefaultSubobject<UGsSkillCollisionComponent>(SkillCollisionComponentName);
	_skillCollision->SetupAttachment(RootComponent);

	//_skillCollisionHandle = CreateDefaultSubobject<UGsSkillCollisioHandlerComponent>(SkillCollisioHandlerComponentName);
	//_skillCollisionHandle->SetupAttachment(RootComponent);

	// 타겟을 마킹하기위한 컴포넌트
	_targetMaker = CreateDefaultSubobject<UDecalComponent>(TargetMarkerComponentName);
	check(_targetMaker);
	_targetMaker->SetupAttachment(RootComponent);
	_targetMaker->SetVisibility(false);
	_targetMaker->SetComponentTickEnabled(false);	

#if 0	// ShadowDecal 블럭, 일단 실시간 그림자로 간다
	USkeletalMeshComponent* skelMeshComp = GetMesh();
	if (skelMeshComp)
	{
		// 싼마이 그림자를 위한 데칼 컴포넌트
		ShadowDecal = CreateDefaultSubobject<UDecalComponent>(ShadowDecalComponentName);
		check(ShadowDecal);

		// 전역 그림자데칼이 지정되어 있을 때만
		if (nullptr != globalConstant && nullptr != globalConstant->ShadowDecalMI)
		{
			ShadowDecal->SetDecalMaterial(globalConstant->ShadowDecalMI);
			SetVisibleShadowDecal(true);
		}

		ShadowDecal->SetupAttachment(skelMeshComp, ShadowAttchSocketName);
	}
#endif

	_uroComponent = CreateDefaultSubobject<UGsUROComponent>(UROName);

	USkeletalMeshComponent* skelMeshComp = GetMesh();
	if (skelMeshComp)
	{
		// 퀘스트 마크
		_questTargetMark = CreateDefaultSubobject<UStaticMeshComponent>(QuestTargetMarkName);
		_questTargetMark->SetCastShadow(false);
		_questTargetMark->SetupAttachment(skelMeshComp);
		_questTargetMark->SetGenerateOverlapEvents(false);

		// 말풍선
		_talkBalloonWidget = CreateDefaultSubobject<UGsWidgetComponentTalkBalloon>(NpcTalkBalloonComponentName);
		_talkBalloonWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
		_talkBalloonWidget->SetWidgetSpace(EWidgetSpace::Screen);
		_talkBalloonWidget->SetDrawSize(FVector2D(100, 100.0f));
		_talkBalloonWidget->SetCastShadow(false);
		_talkBalloonWidget->SetupAttachment(skelMeshComp);
		_talkBalloonWidget->SetGenerateOverlapEvents(false);
	}

	SpawnEffectPlay = false;
}

void AGsCharacterNonPlayerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 캐릭터별 초기화는 BeginPlay 이전에~
	InitializeDecals();
}

const FName AGsCharacterNonPlayerBase::DISSOLVE_INTENSITY = TEXT("Dissolve Intensity");
const FName AGsCharacterNonPlayerBase::GLOW_WIDTH = TEXT("Glow Width");
const FName AGsCharacterNonPlayerBase::SELECT_GLOW_COLOR = TEXT("Select Glow Color");
const FName AGsCharacterNonPlayerBase::GLOW_SECOND_COLOR = TEXT("Glow Second Color");

// geter
const FName AGsCharacterNonPlayerBase::SPAWN_EFFECT_TIME = TEXT("Spawn Effect Time");
const FName AGsCharacterNonPlayerBase::SPAWN_EFFECT_RANDOM_TIME = TEXT("Spawn Effect Random Time");

const float AGsCharacterNonPlayerBase::GLOW_WIDTH_FACTOR = 0.3f;
const float AGsCharacterNonPlayerBase::GLOW_WIDTH_DEFAULT_FACTOR = 0.02f;
const float AGsCharacterNonPlayerBase::SELECT_GLOW_COLOR_FACTOR = 1.0F;
const float AGsCharacterNonPlayerBase::SELECT_GLOW_COLOR_DEFAULT_FACTOR = 0.0f;
const float AGsCharacterNonPlayerBase::DISSOLVE_START_DEFAULT_FACTOR = -1.0f;
const float AGsCharacterNonPlayerBase::DISSOLVE_END_DEFAULT_FACTOR = 2.0f;
const float AGsCharacterNonPlayerBase::DISSOLVE_TIME_DEFAULT_FACTOR = 2.0f;
const float AGsCharacterNonPlayerBase::SPAWN_EFFECT_RANDOM_TIME_DEFAULT_FACTOR = 2.0f;

void AGsCharacterNonPlayerBase::PlaySpawnEffect()
{
	SpawnEffectPlay = false;
	SpawnEffectTimeCur = KINDA_SMALL_NUMBER;
	SpawnEffectTime = DISSOLVE_TIME_DEFAULT_FACTOR;
	SpawnDIStartValue = DISSOLVE_START_DEFAULT_FACTOR;
	SpawnDIEndValue = DISSOLVE_END_DEFAULT_FACTOR;
	SpawnEffectRandomTime = SPAWN_EFFECT_RANDOM_TIME_DEFAULT_FACTOR;

	ApplySpawnEffectMesh = GetMesh();
	ApplySpawnEffectFaceMesh = _partsSkeletalComponentFACE;
	if (nullptr != ApplySpawnEffectMesh)
	{
		// 다이나믹 머터리얼 인스턴스 생성
		TempDynamicMaterial = ApplySpawnEffectMesh->CreateDynamicMaterialInstance(0);
		// 오버레이 머터리얼 임시 저장 후 비우기
		TempDynamicOverlayMaterial = ApplySpawnEffectMesh->GetOverlayMaterial();
		ApplySpawnEffectMesh->SetOverlayMaterial(nullptr);
		ApplySpawnEffectMesh->SetMaterial(0, TempDynamicMaterial);

		if (ApplySpawnEffectFaceMesh != nullptr)
		{
			ApplySpawnEffectFaceMesh->SetOverlayMaterial(nullptr);
			ApplySpawnEffectFaceMesh->SetMaterial(0, TempDynamicMaterial);
		}
		
		if (_mapParameterCollection != nullptr)
		{
			const FCollectionScalarParameter* SpawnEffectTimeScalarParameter;
			SpawnEffectTimeScalarParameter = _mapParameterCollection->GetScalarParameterByName(SPAWN_EFFECT_TIME);
			if (SpawnEffectTimeScalarParameter)
				SpawnEffectTime = SpawnEffectTimeScalarParameter->DefaultValue;

			const FCollectionScalarParameter* SpawnEffectRandomTimeScalarParameter;
			SpawnEffectRandomTimeScalarParameter = _mapParameterCollection->GetScalarParameterByName(SPAWN_EFFECT_RANDOM_TIME);
			if (SpawnEffectRandomTimeScalarParameter)
				SpawnEffectRandomTime = SpawnEffectRandomTimeScalarParameter->DefaultValue;
		}

		if (TempDynamicMaterial != nullptr)
		{
			TempDynamicMaterial->SetScalarParameterValue(GLOW_WIDTH, GLOW_WIDTH_FACTOR);
			TempDynamicMaterial->SetScalarParameterValue(SELECT_GLOW_COLOR, SELECT_GLOW_COLOR_FACTOR);
			TempDynamicMaterial->SetScalarParameterValue(DISSOLVE_INTENSITY, SpawnDIStartValue);
		}

		SpawnEffectPlay = true;
		SpawnEffectTotalDeltaTime = 0.0f;

		// TA님 요청으로 0~2초사이에 랜덤하게 호출
		SpawnRandomTime = FMath::FRandRange(0.0f, SpawnEffectRandomTime);
		/*if (UWorld* world = GetWorld())
		{
			world->GetTimerManager().SetTimer(DelayTimerHandle, FTimerDelegate::CreateLambda([&]()
			{
				SpawnEffectPlay = true;
			}), randomTime, false);
		}*/
	}
}

void AGsCharacterNonPlayerBase::UpdateSpawnEffect(float DeltaTime)
{
	if (SpawnEffectPlay && nullptr != TempDynamicMaterial)
	{
		if (SpawnEffectTimeCur <= SpawnEffectTime)
		{
			float TimeLerp = SpawnEffectTimeCur / SpawnEffectTime; // 진행 시간을 0.0~1.0으로 보간
			float DIDiffValue = FMath::Abs(SpawnDIStartValue) + FMath::Abs(SpawnDIEndValue); // 해당수 사이의 범위 절대값
			float ConvertToActualRangeValue = (0.0f < SpawnDIStartValue ? -SpawnDIStartValue : SpawnDIStartValue); // 실제 범위 값으로 변환
			float DissolveIntensity = (TimeLerp * DIDiffValue) + ConvertToActualRangeValue;
			TempDynamicMaterial->SetScalarParameterValue(DISSOLVE_INTENSITY, DissolveIntensity);
		}
		else
		{
			TempDynamicMaterial->SetScalarParameterValue(GLOW_WIDTH, GLOW_WIDTH_DEFAULT_FACTOR);
			TempDynamicMaterial->SetScalarParameterValue(SELECT_GLOW_COLOR, SELECT_GLOW_COLOR_DEFAULT_FACTOR);

			ApplySpawnEffectMesh->SetOverlayMaterial(TempDynamicOverlayMaterial);
			if (ApplySpawnEffectFaceMesh != nullptr)
				ApplySpawnEffectFaceMesh->SetOverlayMaterial(TempDynamicOverlayMaterial);

			SpawnEffectPlay = false;
			TempDynamicMaterial = nullptr;
			TempDynamicOverlayMaterial = nullptr;
			ApplySpawnEffectMesh = nullptr;
			ApplySpawnEffectFaceMesh = nullptr;
			SpawnEffectTimeCur = 0.0f;
		}

		SpawnEffectTimeCur += DeltaTime;
	}
}

void AGsCharacterNonPlayerBase::BeginPlay()
{
	Super::BeginPlay();

	if (_spawnEffectFlag)
	{		
		PlaySpawnEffect();
	}
}

void AGsCharacterNonPlayerBase::EndPlay(const EEndPlayReason::Type in_type)
{
	if (UWorld* world = GetWorld())
	{
		world->GetTimerManager().ClearAllTimersForObject(this);
	}

	Super::EndPlay(in_type);
}

void AGsCharacterNonPlayerBase::Tick(float DeltaTime)
{
#if 0	// ShadowDecal 블럭, 일단 실시간 그림자로 간다
	// 각종 Culling에 의해 렌더링 되지 않고 있다면
	// 그림자 데칼이 보여지고 있었다면 꺼주자
	// 그림자 데칼이 안 보여지고 있다면 켜는걸 시도해보자
	bool bUseRnder = WasRecentlyRendered();
	if (bUseRnder != IsVisibleShadowDecal())
	{
		SetVisibleShadowDecal(bUseRnder);
	}
#endif

	Super::Tick(DeltaTime);
	
	if (SpawnEffectPlay)
	{
		SpawnEffectTotalDeltaTime += DeltaTime;
		if (SpawnEffectTotalDeltaTime >= SpawnRandomTime)
		{
			UpdateSpawnEffect(DeltaTime);
		}
	}	
}

void AGsCharacterNonPlayerBase::InitWidgetComponent()
{
	Super::InitWidgetComponent();

	FTransform trans = GetMesh()->GetSocketTransform(TEXT("OverHeadSocket"), RTS_Component);

	// 약속된 소켓 위치로 네임태그 이동
	if (_nameplateWidget)
	{
		if (nullptr == _nameplateWidget->GetWidgetClass().Get())
		{
			if (UGsPathManager* pathMgr = GPath())
			{
				UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("NonPlayerWidget"));
				if (widgetClass)
				{
					_nameplateWidget->SetWidgetClassWithPooling(TSubclassOf<UUserWidget>(widgetClass));
				}
			}
		}

		_nameplateWidget->SetRelativeLocation(trans.GetTranslation());
	}

	// 말풍선
	if (_talkBalloonWidget)
	{
		if (nullptr == _talkBalloonWidget->GetWidgetClass().Get())
		{
			if (UGsPathManager* pathMgr = GPath())
			{
				UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, TEXT("TalkBalloon"));
				if (widgetClass)
				{
					_talkBalloonWidget->SetWidgetClass(TSubclassOf<UUserWidget>(widgetClass));
				}
			}
		}
		_talkBalloonWidget->SetRelativeLocation(trans.GetTranslation());
	}
}

void AGsCharacterNonPlayerBase::InitializeDecals()
{
	float fdecalHeight = 50.0f;
	float fdecalSize = 64.0f;
	UCapsuleComponent* capsule = GetCapsuleComponent();
	if (nullptr != capsule)
	{
		fdecalSize = capsule->GetUnscaledCapsuleRadius() * RootComponent->GetComponentScale().X;
		fdecalHeight = capsule->GetUnscaledCapsuleHalfHeight();
	}

	_targetMaker->SetRelativeLocation(FVector(0, 0, -fdecalHeight));
	_targetMaker->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	_targetMaker->SetFadeIn(0.0f, 0.1f);
	_targetMaker->DecalSize = FVector(fdecalSize, fdecalSize * 2.0f, fdecalSize * 2.0f);

	if (_shadowDecal)
	{
		_shadowDecal->SetRelativeLocation(FVector(0, 0, -fdecalHeight));
		_shadowDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		_shadowDecal->SetFadeIn(0.0f, 0.1f);

		fdecalSize *= 0.66f;
		_shadowDecal->DecalSize = FVector(fdecalSize, fdecalSize * 2.0f, fdecalSize * 2.0f);
	}
}

void AGsCharacterNonPlayerBase::OnHit(HitResultType ResultType, EGsStateBase CurrentState,
	const FGsSchemaCreatureCommonInfo* CommonData)
{
	Super::OnHit(ResultType, CurrentState, CommonData);

	if (UGsUIBillboardTalkBalloon* talkBalloon = Cast<UGsUIBillboardTalkBalloon>(_talkBalloonWidget->GetUserWidgetObject()))
	{
		talkBalloon->OnHit();
	}
}

void AGsCharacterNonPlayerBase::SetVisibleTargetMaker(bool InVisible, UMaterialInterface* InMaterial)
{
	if (nullptr != _targetMaker)
	{
		_targetMaker->SetVisibility(InVisible);
		_targetMaker->SetComponentTickEnabled(InVisible);
		if (nullptr != InMaterial)
		{
			_targetMaker->SetDecalMaterial(InMaterial);
		}
	}
}

void AGsCharacterNonPlayerBase::SetVisibleShadowDecal(bool InVisible)
{
	if (nullptr != _shadowDecal)
	{
		_shadowDecal->SetVisibility(InVisible);
		_shadowDecal->SetComponentTickEnabled(InVisible);
	}
}

void AGsCharacterNonPlayerBase::SetUseTalkBalloon(bool In_isUse)
{
	if (_talkBalloonWidget)
	{
		if (In_isUse == true)
		{
			if (nullptr == _talkBalloonWidget->GetUserWidgetObject())
			{
				_talkBalloonWidget->InitWidget();
			}
		}
	}
}

UGsUIBillboardTalkBalloon* AGsCharacterNonPlayerBase::GetTalkBalloon() const
{
	if (_talkBalloonWidget)
	{
		return Cast<UGsUIBillboardTalkBalloon>(_talkBalloonWidget->GetUserWidgetObject());
	}
	return nullptr;
}

void AGsCharacterNonPlayerBase::ShowTalkBalloonOnChangedState(uint8 InState)
{
	if (UGsUIBillboardTalkBalloon* talkBalloon = Cast<UGsUIBillboardTalkBalloon>(_talkBalloonWidget->GetUserWidgetObject()))
	{
		talkBalloon->SetActorState(InState);
	}
}

bool AGsCharacterNonPlayerBase::IsVisibleShadowDecal() const
{
	if (nullptr == _shadowDecal)
		return false;

	return _shadowDecal->IsVisible();
}

void AGsCharacterNonPlayerBase::SetInteractionClickCallback(TFunction<void(bool)> In_callback)
{
	_callbackInteractionClick = In_callback;
}

void AGsCharacterNonPlayerBase::ClickInteraction()
{
	if (_callbackInteractionClick != nullptr)
	{
		_callbackInteractionClick(false);
	}
}