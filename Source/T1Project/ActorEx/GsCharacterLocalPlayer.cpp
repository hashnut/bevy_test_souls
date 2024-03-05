// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCharacterLocalPlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "T1Project.h"
#include "Engine.h"

#include "UObject/ConstructorHelpers.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectArray.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "ControllerEx/GsPlayerController.h"
#include "ControllerEx/GsPlayerAIController.h"

#include "ActorEx/GsWeaponActor.h"
#include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "ActorComponentEx/GsSkillCollisioHandlerComponent.h"
#include "ActorComponentEx/GsMeshShaderControlComponent.h"
#include "ActorComponentEx/GsLocalCharacterMovementComponent.h"
#include "ActorComponentEx/GsWidgetComponentPooling.h"
#include "ActorComponentEx/GsSpringArmComponentEx.h"

#include "UMG/Public/Components/WidgetInteractionComponent.h"

#include "Input/GsInputBindingBase.h"
#include "Input/GsInputBindingLocalPlayer.h"

#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Stat/GsStatLocalPlayer.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Define/GsGameObjectText.h"
#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"

#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "ActorComponentEx/EGsMeshShaderEffect.h"

#include "AI/Function/GsAIFunc.h"
#include "AI/Data/GsAIDefine.h"
#include "UTIL/GsTimeUtil.h"
#include "Cheat/GsCheatManager.h"
#include "Management/ScopeGlobal/GsSoundManager.h"

#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "UTIL/GsCameraUtil.h"
#include "Data/GsDataContainManager.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"

#include "GsCharacterBase.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "GsCharacterNonPlayer.h"
#include "Components/SphereComponent.h"
#include "T1Project.h"

#include "Option/GsServerOption.h"
#include "Management/ScopeGlobal/GsOptionManager.h"
#include "Components/SceneComponent.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"


const FName SpringArmComponentName = TEXT("CameraBoom");
const FName CameraComponentName = TEXT("FollowCamera");
const FName CameraPostProcess = TEXT("CameraPostProcess");
const FName WidgetInteract = TEXT("WidgetInteract");
const FName QuestWidgetComponentName	= TEXT("QuestWidget");
const FName QuestWidgetName = TEXT("BillboardQuestLocalPlayer");
const FName QUEST_NPC_SPHERE_COMP = TEXT("QuestNpcSphereComp");
const FName NEARBY_PAWN_DETECION_BOX_COMP = TEXT("NearbyPawnDetectionBoxComp");
const FName TRAY_LOW_HP_NAME = TEXT("TrayLowHP");

// Sets default values
AGsCharacterLocalPlayer::AGsCharacterLocalPlayer(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UGsLocalCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	// Create a camera boom (pulls in towards the player if there is a collision)
	_cameraBoom = CreateDefaultSubobject<UGsSpringArmComponentEx>(SpringArmComponentName);
	// The camera follows at this distance behind the character	
	_cameraBoom->TargetArmLength = 300.0f;
	// Rotate the arm based on the controller
	_cameraBoom->bUsePawnControlRotation = true;
	// Attach to Root
	_cameraBoom->SetupAttachment(RootComponent);

	// Create a follow camera
	_followCamera = CreateDefaultSubobject<UCameraComponent>(CameraComponentName);
	// Camera does not rotate relative to arm
	_followCamera->bUsePawnControlRotation = false;
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	_followCamera->SetupAttachment(_cameraBoom, USpringArmComponent::SocketName);

	_skillCollision = CreateDefaultSubobject<UGsSkillCollisionComponent>(SkillCollisionComponentName);
	_skillCollision->SetupAttachment(RootComponent);

	_skillCollisionHandle = CreateDefaultSubobject<UGsSkillCollisioHandlerComponent>(SkillCollisioHandlerComponentName);
	_skillCollisionHandle->SetupAttachment(RootComponent);

	AutoPossessPlayer = EAutoReceiveInput::Type::Disabled;
	AutoPossessAI = EAutoPossessAI::Disabled;

	_cameraPostProcess = CreateDefaultSubobject<UGsCameraPostProcessComponent>(CameraPostProcess);

	if (_nameplateWidget)
	{
		_nameplateWidget->SetInitialSharedLayerName(TEXT("WidgetComponentScreenLayerLocal"));
		_nameplateWidget->SetInitialLayerZOrder(-1);
		_nameplateWidget->SetGenerateOverlapEvents(false);
	}

#if QUEST_NPC_ACTIVE
	//마을 안이거나 필드 밖에 퀘스트 NPC 근처에 가면 플레이어 충돌을 끈다. 
	_questNpcSphereComp = CreateDefaultSubobject<USphereComponent>(QUEST_NPC_SPHERE_COMP);
	_questNpcSphereComp->SetupAttachment(RootComponent);
	_questNpcSphereComp->SetGenerateOverlapEvents(false);

	//퀘스트 NPC가 아니면 Shpere Compnent 끌 것 
#if QUEST_NPC_DEBUG
	_questNpcSphereComp->SetHiddenInGame(false);
#endif
	_questNpcSphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	_questNpcSphereComp->SetSphereRadius(SharedFunc::GetIgnoreCollisionRangeQuestNpc());		

	//클라이언트 스폰으로 스폰되는 대화용 Npc의 콜리전은 PawnNpc이다. 
	//ECC_GameTraceChannel11 = PawnNPC
	_questNpcSphereComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel11, ECR_Overlap);
#endif

	// 2022/11/07 PKT - Active( 마을 밖 / Quest Auto Move )
	_nearbyPawnDetectionBoxComp = CreateDefaultSubobject<UBoxComponent>(NEARBY_PAWN_DETECION_BOX_COMP);
	_nearbyPawnDetectionBoxComp->SetupAttachment(RootComponent);

	float height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float radius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector boxExtent = FVector(50.f, radius, height);
	_nearbyPawnDetectionBoxComp->SetBoxExtent(boxExtent);
	_nearbyPawnDetectionBoxComp->SetRelativeLocation(FVector(boxExtent.X + radius, 0.f, 0.f));
	_nearbyPawnDetectionBoxComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	//ECC_GameTraceChannel9 == OtherCollision
	_nearbyPawnDetectionBoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel9, ECR_Overlap);
	//_nearbyPawnDetectionBoxComp->SetHiddenInGame(true)

	// 2022/11/09 PKT - Default - Off
	SetPawnDetectionBoxEnable(true);
	SetPawnDetectionCollisionEnable(false);
}

// Called when the game starts or when spawned
void AGsCharacterLocalPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (_cameraPostProcess)
	{
		_cameraPostProcess->Initialize(_followCamera);

		UPostProcessCurve* deathDimed = _cameraPostProcess->GetCurveData(EGsPlayerPostProcess::DeathDimed);

		if (deathDimed)
		{
			deathDimed->PostProcessCurveMessage.AddUObject(EGsPostProcessCurveEvent::TickEvent, this, &AGsCharacterLocalPlayer::OnTickDeathDimed);
		}

		UPostProcessCurve* fadeOut = _cameraPostProcess->GetCurveData(EGsPlayerPostProcess::DeathDimedFadeOut);

		if (fadeOut)
		{
			fadeOut->PostProcessCurveMessage.AddUObject(EGsPostProcessCurveEvent::TickEvent, this, &AGsCharacterLocalPlayer::OnTickDeathDimedFadeOut);
		}

		if (UPostProcessCurve* blurEffect = _cameraPostProcess->GetCurveData(EGsPlayerPostProcess::Blur))
		{
			blurEffect->PostProcessCurveMessage.AddUObject(EGsPostProcessCurveEvent::TickEvent, this, &AGsCharacterLocalPlayer::OnTickBlurEffect);
		}
	}

	// For Test 
	if (FApp::GetBuildConfiguration() != EBuildConfiguration::Shipping)
	{
		if (UGsCheatManager::IsPlayerMighty)
		{
			if (UPawnMovementComponent* movement = GetMovementComponent())
			{
				if (UCharacterMovementComponent* characterMovement = Cast<UCharacterMovementComponent>(movement))
				{
					characterMovement->MaxWalkSpeed = PLAYER_MIGHTY_WALK_SPEED;
				}
			}
		}
	}

	StopHpWarningEffect();
}

void AGsCharacterLocalPlayer::EndPlay(EEndPlayReason::Type inReason)
{
	StopHpWarningEffect();

#if QUEST_NPC_ACTIVE
	_questNpcSphereComp->OnComponentBeginOverlap.Clear();
	_questNpcSphereComp->OnComponentEndOverlap.Clear();
#endif

	// 2022/11/07 PKT - 주변 대상 탐지
	_nearbyPawnDetectionBoxComp->OnComponentBeginOverlap.Clear();
	_nearbyPawnDetectionBoxComp->OnComponentEndOverlap.Clear();

	OnHitCampEvent.Clear();

	Super::EndPlay(inReason);
}

// Called every frame
void AGsCharacterLocalPlayer::Tick(float in_deltaTime)
{
	Super::Tick(in_deltaTime);

	if (_debugLocalPlayer._isGlobalObjectsDisplay)
	{
		if (GEngine)
		{
			FChunkedFixedUObjectArray& arrObject = GUObjectArray.GetObjectItemArrayUnsafe();
			float rate = ((float)arrObject.Num() / (float)arrObject.Capacity());
			GEngine->AddOnScreenDebugMessage(99999, 1.0f, FColor::Cyan, FString::Printf(TEXT("GUObjectArray Count %d / %d ( %f )"),
				arrObject.Num(), arrObject.Capacity(), rate * 100.f),
				true, FVector2D(1.2f, 1.2f));
		}
	}
}

// Called to bind functionality to input
void AGsCharacterLocalPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (_inputBinder)
	{
		_inputBinder->SetBinding(PlayerInputComponent);
	}
}

void AGsCharacterLocalPlayer::SetInputBinder(UGsInputBindingBase* Binder)
{
	if (nullptr != Binder)
	{
		_inputBinder = Binder;
		_inputBinder->SetBinding(InputComponent);
	}
}

void AGsCharacterLocalPlayer::SetTouchMoveFactor(float In_val)
{
	UGsInputBindingLocalPlayer* inputBinding =
		Cast<UGsInputBindingLocalPlayer>(_inputBinder);

	if (inputBinding == nullptr)
	{
		return;
	}

	inputBinding->SetTouchMoveFactor(In_val);
}

void AGsCharacterLocalPlayer::SetTouchMoveLimit(float In_val)
{
	UGsInputBindingLocalPlayer* inputBinding =
		Cast<UGsInputBindingLocalPlayer>(_inputBinder);

	if (inputBinding == nullptr)
	{
		return;
	}

	inputBinding->SetTouchMoveLimit(In_val);
}

void AGsCharacterLocalPlayer::SetPitchRotMul(float In_val)
{
	UGsInputBindingLocalPlayer* inputBinding =
		Cast<UGsInputBindingLocalPlayer>(_inputBinder);

	if (inputBinding == nullptr)
	{
		return;
	}

	inputBinding->SetPitchRotMul(In_val);
}

void AGsCharacterLocalPlayer::ShowTouchMoveValue()
{
	UGsInputBindingLocalPlayer* inputBinding =
		Cast<UGsInputBindingLocalPlayer>(_inputBinder);

	if (inputBinding == nullptr)
	{
		return;
	}
#if WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
			FString::Printf(TEXT("TouchMoveFactor: %f, TouchMoveLimitMin: %f, TouchMoveLimitMax: %f"),
				inputBinding->GetTouchMoveFactor(),
				inputBinding->GetTouchMoveLimitMin(),
				inputBinding->GetTouchMoveLimitMax()
			));
	}
#endif
}


void AGsCharacterLocalPlayer::SetTouchZoomFactor(float In_val)
{
	UGsInputBindingLocalPlayer* inputBinding =
		Cast<UGsInputBindingLocalPlayer>(_inputBinder);

	if (inputBinding == nullptr)
	{
		return;
	}

	inputBinding->SetTouchZoomFactor(In_val);
}

void AGsCharacterLocalPlayer::ShowTouchZoomValue()
{
	UGsInputBindingLocalPlayer* inputBinding =
		Cast<UGsInputBindingLocalPlayer>(_inputBinder);

	if (inputBinding == nullptr)
	{
		return;
	}
#if WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
			FString::Printf(TEXT("TouchZoomFactor: %f"),
				inputBinding->GetTouchZoomFactor()
			));
	}
#endif
}


void AGsCharacterLocalPlayer::ShowTouchCMD()
{
#if WITH_EDITOR
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow,
			TEXT("SetTouchMoveFactor (float)")
		);

		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow,
			TEXT("SetTouchMoveLimit (float)")
		);

		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow,
			TEXT("ShowTouchMoveValue")
		);

		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow,
			TEXT("SetTouchZoomFactor (float)")
		);

		GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow,
			TEXT("ShowTouchZoomValue")
		);
	}
#endif
}

void AGsCharacterLocalPlayer::SetSpawnState(bool in_spawn)
{
	if (UPawnMovementComponent * base = GetMovementComponent())
	{
		if (UCharacterMovementComponent * move = Cast<UCharacterMovementComponent>(base))
		{
			if (in_spawn)
			{
				move->bApplyGravityWhileJumping = true;
				move->GravityScale = 1.0f;
			}
			else
			{
				move->bApplyGravityWhileJumping = false;
				move->GravityScale = 0.0f;
			}
		}
	}
}

void AGsCharacterLocalPlayer::ChangeWeaponSocketProcess(TArray<FName> ListSocketName, TArray<FVector> ListOffset,
	UCurveFloat* WeaponDissolve, UCurveFloat* WeaponFrsnel, UParticleSystem* Particle,
	bool IsStartProcess, TFunction<void()> EndCallback)
{
	// XrayVision 효과 처리
	if (IsStartProcess)
	{
		SetupPrimitives(GetWeapon(), false);
		SetupPrimitives(GetWeaponSub(), false);
	}
	

	Super::ChangeWeaponSocketProcess(ListSocketName, ListOffset, WeaponDissolve, WeaponFrsnel, Particle, IsStartProcess,
		[this, EndCallback, IsStartProcess]() {

		if (false == IsStartProcess)
		{
			SetupPrimitives(GetWeapon(), true);
			SetupPrimitives(GetWeaponSub(), true);
		}

		if (EndCallback)
		{
			EndCallback();
		}
	});
}

AGsPlayerController* AGsCharacterLocalPlayer::GetPlayerController()
{
	// AGsPlayerController 컨트롤러가 SpawnActor(BeginPlay) 이후에 Possess로 설정될수 있다.
	if (nullptr == _playerController)
	{
		_playerController = Cast<AGsPlayerController>(GetController());
	}
	return _playerController;
}

class UGsGameObjectLocalPlayer* AGsCharacterLocalPlayer::GetGameObjectPlayer()
{
	return _owner.Get();
}

// 여러번 호출될 수 있으니 주의 해주세요
void AGsCharacterLocalPlayer::OnHitNotifyByCampObject(const AActor* CampObject)
{
	if (OnHitCampEvent.IsBound())
	{
		OnHitCampEvent.Broadcast(CampObject);
	}
}

void AGsCharacterLocalPlayer::OnTickDeathDimed(const UPostProcessCurve* inValue)
{
	if (inValue)
	{
		int32 realMaterialIndex = inValue->GetRealMaterialIndex();
		float weight = inValue->GetCurrentWeight();

		UGsCameraUtil::SetCameraPostProcessWeight(_followCamera, realMaterialIndex, weight);
	}	
}

void AGsCharacterLocalPlayer::OnTickDeathDimedFadeOut(const UPostProcessCurve* inValue)
{
	if (inValue)
	{
		int32 realMaterialIndex = inValue->GetRealMaterialIndex();
		float weight = inValue->GetCurrentWeight();

		UGsCameraUtil::SetCameraPostProcessWeight(_followCamera, realMaterialIndex, weight);
	}	
}

void AGsCharacterLocalPlayer::StopDeathEffect()
{
	if (_cameraPostProcess)
	{
		_cameraPostProcess->Stop(EGsPlayerPostProcess::DeathDimed);

		UPostProcessCurve* data = _cameraPostProcess->GetCurveData(EGsPlayerPostProcess::DeathDimed);

		if (data)
		{
			UGsCameraUtil::SetCameraPostProcessWeight(_followCamera, data->_materialIndex, 0.0f);
		}
	}
}

void AGsCharacterLocalPlayer::PlayResurrectionEffect()
{
	if (_cameraPostProcess)
	{
		_cameraPostProcess->Start(EGsPlayerPostProcess::DeathDimedFadeOut);
	}
}

void AGsCharacterLocalPlayer::PlayDeathEffect()
{
	if (_cameraPostProcess)
	{
		_cameraPostProcess->Start(EGsPlayerPostProcess::DeathDimed);
	}
}

void AGsCharacterLocalPlayer::StopHpWarningEffect()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		if (uiMgr->IsActiveWidget(TRAY_LOW_HP_NAME))
		{
			uiMgr->CloseByKeyName(TRAY_LOW_HP_NAME);
		}
	}
}

void AGsCharacterLocalPlayer::BeginHpWarningEffect()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		if (false == uiMgr->IsActiveWidget(TRAY_LOW_HP_NAME))
		{
			uiMgr->Open(TRAY_LOW_HP_NAME);
		}
	}
}

// 초기화
void AGsCharacterLocalPlayer::Initialize(UGsGameObjectLocalPlayer* In_owner)
{
	_owner = In_owner;
	
	BindMessage();
}

void AGsCharacterLocalPlayer::BindMessage()
{
	if (nullptr == _owner)
		return;

	if (FGsGameObjectDataCreature * creatureData = _owner->GetCastData<FGsGameObjectDataCreature>())
	{
		if (TSharedPtr<FGsStatBase> base = creatureData->GetStatBase())
		{
			base->OnChangeStatDelegate.AddUObject(this, &AGsCharacterLocalPlayer::CheckWarnHp);
		}
	}
}

void AGsCharacterLocalPlayer::CheckWarnHp(const class FGsStatInfo& InInfo)
{
	// 컨퓨즈 상태이상(HP/MP가림) 시엔 HP알림을 발동하지 않음
	if (_owner.IsValid())
	{
		if (FGsAbnormalityHandlerBase* abnormalityHandler = _owner->GetAbnormalityHandler())
		{
			if (abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::UTIL_CONFUSE))
			{
				return;
			}
		}
	}

	if (StatType::HP != InInfo.GetStatType())
		return;

	float curHP = (float)InInfo.GetStatValue();
	float maxHP = (float)InInfo.GetMaxStatValue();	
	float curHPPercent = curHP / maxHP;
	
	float warnValue = 0.f;
	if (FGsServerOption* serverOption = GServerOption())
	{
		warnValue = serverOption->GetOptionNotificationAlert().mHPDangerousNotification.mAlertPercent * 0.01f;

		if (false == GServerOption()->GetIsPvpHUDEffectOn())
		{
			if (curHPPercent <= warnValue)
			{
				BeginHpWarningEffect();
			}
			else
			{
				StopHpWarningEffect();
			}
		}

		//serverOption->CheckHPAlert();
	}
}

void AGsCharacterLocalPlayer::SetConfuseEffect(bool bIsConfused)
{
	// 빌보드 HP/MP 제거
	if (UGsUIBillboardCreature* widget = GetWidget())
	{
		widget->SetAbnormalityConfuse(bIsConfused);
	}

	// HP 연출 및 알림
	if (bIsConfused)
	{
		// HP연출 강제로 제거
		StopHpWarningEffect();
	}
	else
	{
		// HP연출 및 알림 복구를 위해 체크
		if (_owner.IsValid())
		{
			if (const FGsStatInfo* hpStat = _owner->GetCreatureStatInfoByType(StatType::HP))
			{
				CheckWarnHp(*hpStat);
			}
		}
	}
}

void AGsCharacterLocalPlayer::PlaySkillLearnEffect(int32 InEffectIndex)
{
	const UGsGlobalConstant* globalConstData = GData()->GetGlobalData();
	if (nullptr == globalConstData)
	{
		return;
	}

	if (globalConstData->_learnSkillParticle.IsValidIndex(InEffectIndex))
	{
		if (UGsEffectComponent* effectComp = GetEffectComponent())
		{
			if (UPawnMovementComponent* moveComp = GetMovementComponent())
			{
				effectComp->OnEffectPos(globalConstData->_learnSkillParticle[InEffectIndex],
					moveComp->GetActorFeetLocation(),
					FVector::OneVector);
			}
		}
	}
}

void AGsCharacterLocalPlayer::PlayBlurEffect()
{
	if (_cameraPostProcess)
	{
		_cameraPostProcess->Start(EGsPlayerPostProcess::Blur);
	}
}

void AGsCharacterLocalPlayer::StopBlurEffect()
{
	if (_cameraPostProcess)
	{
		_cameraPostProcess->Stop(EGsPlayerPostProcess::Blur);

		if (UPostProcessCurve* data = _cameraPostProcess->GetCurveData(EGsPlayerPostProcess::Blur))
		{
			UGsCameraUtil::SetCameraPostProcessWeight(_followCamera, data->_materialIndex, 0.0f);
		}
	}
}

#if WITH_EDITOR
void AGsCharacterLocalPlayer::PlayPlayerDissolve()
{
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(local))
		{
			if (AActor* actor = player->GetActor())
			{
				if (AGsCharacterLocalPlayer* character = Cast<AGsCharacterLocalPlayer>(actor))
				{
					if (UGsMeshShaderControlComponent* comp = GetMeshShaderComponent())
					{
						const FGsSchemaCreatureCommonInfo* commonTable = player->GetCommonInfoTable();
						const UCurveFloat* curveData = commonTable->meshShader[EGsMeshShaderEffect::Dissolve];
						if (curveData)
						{
							comp->Add(EGsMeshShaderEffect::Dissolve, curveData);
						}
						else
						{
							comp->Add(EGsMeshShaderEffect::Dissolve);
						}
						character->GetMeshShaderComponent()->Start();
					}
				}
			}
		}
	}
}

void AGsCharacterLocalPlayer::PlayPlayerFresnel()
{
	if (UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer))
	{
		if (UGsGameObjectLocalPlayer* player = Cast<UGsGameObjectLocalPlayer>(local))
		{
			if (AActor* actor = player->GetActor())
			{
				if (AGsCharacterLocalPlayer* character = Cast<AGsCharacterLocalPlayer>(actor))
				{
					if (UGsMeshShaderControlComponent* comp = character->GetMeshShaderComponent())
					{
						const FGsSchemaCreatureCommonInfo* commonTable = player->GetCommonInfoTable();
						if (commonTable->meshShader.Contains(EGsMeshShaderEffect::Fresnel))
						{
							comp->Add(EGsMeshShaderEffect::Fresnel, commonTable->meshShader[EGsMeshShaderEffect::Fresnel]);
						}
						else
						{
							comp->Add(EGsMeshShaderEffect::Fresnel);
						}
						character->GetMeshShaderComponent()->Start();
					}
				}
			}
		}
	}
}
#endif

void AGsCharacterLocalPlayer::OnTickBlurEffect(const UPostProcessCurve* InValue)
{
	if (InValue)
	{
		UGsCameraUtil::SetCameraPostProcessWeight(_followCamera, 
			InValue->GetRealMaterialIndex(), InValue->GetCurrentWeight());
	}
}

void AGsCharacterLocalPlayer::SetAttachFollowCamera(bool In_isAttach)
{
	if (_followCamera == nullptr)
	{
		return;
	}

	if (In_isAttach)
	{
		_followCamera->AttachToComponent(_cameraBoom, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		_followCamera->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void AGsCharacterLocalPlayer::SetAttachFollowCameraToController(bool In_isAttach)
{
	if (_followCamera == nullptr)
	{
		return;
	}

	if (In_isAttach)
	{
		_followCamera->AttachToComponent(_cameraBoom, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		AGsPlayerController* playerController = GetPlayerController();

		if (playerController == nullptr)
		{
			return;
		}
		USceneComponent* controllerRootComponent = playerController->GetRootComponent();
		if (controllerRootComponent == nullptr)
		{
			return;
		}

		_followCamera->AttachToComponent(controllerRootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

void AGsCharacterLocalPlayer::MoveBlockedBy(const FHitResult& Impact)
{
	// 수도
	// 1. 최초 임펙트 발생(배열관리)시 시간을 기록
	// 2. 임펙트가 일정시간 유지될며 이동량이 없을때 시에 Block이라고 판단하고
	// 3. 이벤트를 발생한다.
	// 4. 이벤트를 받은 MovementHandlerLocal은 전송을 Block으로 변경하고 전송을 시도한다.
	// 5. 실제 이동 Delta를 체크

}

bool AGsCharacterLocalPlayer::SetWeaponBlueprintClass(FSoftObjectPath inPath, FGsWeaponLoadCompleteDelegate& inAfter, bool isAsync)
{
	TWeakObjectPtr<AGsCharacterPlayer> weakThis(this);
	inAfter.AddLambda([weakThis]()->void {
		if (weakThis.IsValid())
		{
			weakThis->SetupPrimitives(weakThis->GetWeapon(), true);
		}
	});

	return Super::SetWeaponBlueprintClass(inPath, inAfter, isAsync);
	
}

bool AGsCharacterLocalPlayer::SetWeaponSubBlueprintClass(FSoftObjectPath inPath, FGsWeaponLoadCompleteDelegate& inAfter, bool isAsync)
{
	TWeakObjectPtr<AGsCharacterPlayer> weakThis(this);
	inAfter.AddLambda([weakThis]()->void {
		if (weakThis.IsValid())
		{
			weakThis->SetupPrimitives(weakThis->GetWeaponSub(), true);
		}
	});

	return Super::SetWeaponSubBlueprintClass(inPath, inAfter, isAsync);	
}

void AGsCharacterLocalPlayer::SetQuestNpcSphereRadius(float inRadius)
{
#if QUEST_NPC_ACTIVE	
	_questNpcSphereComp->SetSphereRadius(inRadius);
#endif
}

class USphereComponent* AGsCharacterLocalPlayer::GetQuestNpcSphereComp()
{
	return _questNpcSphereComp;
}

void AGsCharacterLocalPlayer::ActiveQuestNpcSphere(bool inActive /*= true*/)
{
#if QUEST_NPC_ACTIVE	
	if (inActive)
	{
		_questNpcSphereComp->Activate();
	}
	else
	{
		_questNpcSphereComp->Deactivate();
	}	
#endif
}

void AGsCharacterLocalPlayer::SetPawnDetectionBoxEnable(bool InEnable)
{
	_isPawnDetectionEnable = InEnable;

	SetPawnDetectionCollisionEnable(_isPawnDetectionCollisionEnable);
}

void AGsCharacterLocalPlayer::ClearPawnDetectionCollisionOverlaps()
{
	if (nullptr != _nearbyPawnDetectionBoxComp && _nearbyPawnDetectionBoxComp->IsCollisionEnabled())
	{
		const TArray<FOverlapInfo> overlapInfos = _nearbyPawnDetectionBoxComp->GetOverlapInfos();
		for (const auto& item : overlapInfos)
		{
			const static bool bDoNotifies = false;
			const static bool bSkipNotifySelf = false;
			_nearbyPawnDetectionBoxComp->EndComponentOverlap(item, bDoNotifies, bSkipNotifySelf);
		}
	}
}

void AGsCharacterLocalPlayer::SetPawnDetectionCollisionEnable(bool InEnable)
{
	_isPawnDetectionCollisionEnable = InEnable;

	bool collisionEnable = (_isPawnDetectionEnable && _isPawnDetectionCollisionEnable);
	ECollisionEnabled::Type collisionType = ECollisionEnabled::QueryOnly;
	if (collisionEnable == false)
	{
		collisionType = ECollisionEnabled::NoCollision;
		if (UGsCharacterMovementComponent* castMoveCompoennt = Cast<UGsCharacterMovementComponent>(GetMovementComponent()))
		{
			castMoveCompoennt->ResetCheckOverlapActorCount();
		}
	}
	
	_nearbyPawnDetectionBoxComp->SetCollisionEnabled(collisionType);
}

void AGsCharacterLocalPlayer::SetAvoidanceConsiderationRadius(int InIdex)
{
	if (UGsCharacterMovementComponent* castMoveCompoennt = Cast<UGsCharacterMovementComponent>(GetMovementComponent()))
	{
		if (InIdex == 0)
		{
			castMoveCompoennt->SetAvoidanceEnabled(false);
		}
		else if (_customAvoidanceConsiderationRadius.IsValidIndex(InIdex))
		{
			castMoveCompoennt->AvoidanceConsiderationRadius = _customAvoidanceConsiderationRadius[InIdex];
			castMoveCompoennt->SetAvoidanceEnabled(true);
		}
	}
}

UBoxComponent* AGsCharacterLocalPlayer::GetPawnDetectionBoxComp()
{
	return _nearbyPawnDetectionBoxComp;
}