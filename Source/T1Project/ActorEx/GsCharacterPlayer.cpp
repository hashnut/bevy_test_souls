// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCharacterPlayer.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "GameObject/ObjectClass/Data/GsShapeData.h"
#include "GameObject/Define/EGsFootPrintMethod.h"
#include "GameObject/ObjectClass/Data/GsGameObjectMaterialData.h"

#include "ActorComponentEx/EGsMeshShaderEffect.h"
#include "ActorComponentEx/GsEffectComponent.h"
#include "ActorEx/GsPhysicalMaterial.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsResourceManager.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "DataSchema/Costume/GsSchemaCostumeEffect.h"
#include "DataSchema/User/GsSchemaUserData.h"
#include "DataSchema/User/WeaponSocket/GsSchemaWeaponSocketInfo.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGame/GsCampManager.h"

#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"


#include "GsWeaponActor.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItem.h"
#include "Classes/GsSpawn.h"
#include "Animation/GsAnimInstanceState.h"


#include "ActorComponentEx/GsCostumeEffectComponent.h"
#include "ActorComponentEx/GsWidgetComponentPooling.h"
#include "ForEach.h"


const FName FairySpringArmComponentName = TEXT("FairyAttachSpringArm");

// Sets default values
AGsCharacterPlayer::AGsCharacterPlayer(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	if (auto mesh = GetMesh())
	{
		_orgMesh = mesh;
		// 파츠 처리관련 컴퍼넌트 
		_partsSkeletalComponentHAIR = CreateDefaultSubobject<USkeletalMeshComponent>(PartsMeshComponentName[1]);
		_partsSkeletalComponentHAIR->SetupAttachment(GetMesh());
		_partsSkeletalComponentHAIR->SetMasterPoseComponent(GetMesh());

		_partsWeaponChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("PartsWeaponChildActor"));
		_partsWeaponChildActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

		_partsWeaponSubChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("PartsWweaponSubChildActor"));
		_partsWeaponSubChildActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

		_costumeEffectChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("CostumeEffectChildActor"));
		_costumeEffectChildActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

		GetMesh()->bBoneScaleMode = true;
		GetMesh()->bPropagateCurvesToSlaves = true;
		//_partsFACE->bBoneScaleMode = true;

		_toolEquipmentStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ToolStaticMeshComp"));
		_toolEquipmentStaticMesh->SetupAttachment(GetMesh());
		_toolEquipmentStaticMesh->SetAttachSocketName(TEXT("SOC_BattleWeapon_R"));

		_fairySpringArm = CreateDefaultSubobject<USpringArmComponent>(FairySpringArmComponentName);
		_fairySpringArm->SetupAttachment(RootComponent);
	}

	if (UCapsuleComponent* capsule = GetCapsuleComponent())
	{
		capsule->SetUseCCD(true);
	}
}


void AGsCharacterPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 캐릭터별 초기화는 BeginPlay 이전에~
	InitializeDecals();
}

// Called when the game starts or when spawned
void AGsCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	UGsDataContainManager* dataContainerManager = GData();
	if (nullptr != dataContainerManager)
	{
		const UGsGlobalConstant* globalConstant = dataContainerManager->GetGlobalData();
		if (nullptr != globalConstant)
		{
			FInteractionMotionWeaponData data = globalConstant->_buildCampHammerMeshData;
			FString toolMeshPath = data.ToolStaticMeshPath.ToString();
			UStaticMesh* staticmeshTool = LoadObject<UStaticMesh>(nullptr, *toolMeshPath);
			if (nullptr != staticmeshTool)
			{
				//_toolEquipmentStaticMesh->SetAttachSocketName(TEXT("SOC_BattleWeapon_R"));
				_toolEquipmentStaticMesh->SetStaticMesh(staticmeshTool);
				_toolEquipmentStaticMesh->SetMaterial(0, data.ToolMaterial);
				_toolEquipmentStaticMesh->SetVisibility(false);
			}
		}
	}

	// bak1210 : 맵전용 발먼지 PS 세팅 
	RestoreDefaultFootPrint();

#if WITH_EDITOR
	InvalidateCostumeEffectChildActor();
#endif
}

void AGsCharacterPlayer::EndPlay(EEndPlayReason::Type inReason)
{
	//_weaponChangeInfo.Clear();
	//StopMeshShader();
	SetEnableDeathActor(false);
	//RestoreMesh();

	Super::EndPlay(inReason);
}

bool AGsCharacterPlayer::LoadHitAnimInstance(const FSoftObjectPath& path)
{
	SetWeaponTypeChangeHitPose(_weaponType);
	
	return Super::LoadHitAnimInstance(path);
}

void AGsCharacterPlayer::SetWeaponTypeChangeHitPose(CreatureWeaponType inType)
{
	_weaponType = inType;

	if (inType != CreatureWeaponType::ALL)
	{
		auto Findone = _hitPlayerInfo.FindByPredicate([inType](const FGsHitInfoStruct& in) -> bool
			{
				if (in.weaponType == inType)
				{
					return true;
				}
				return false;
			});

		if (nullptr != Findone)
		{
			_hitMotionInterface = Findone->hitMotionInterface;
		}
	}
}

// Called every frame
void AGsCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	_weaponChangeInfo.Update(DeltaTime);
}

void AGsCharacterPlayer::BeginDestroy()
{	
	Super::BeginDestroy();
}

// Called to bind functionality to input
void AGsCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGsCharacterPlayer::InitializePartsComponent(UPrimitiveComponent* Component)
{
	Component->AlwaysLoadOnClient = true;
	Component->bOwnerNoSee = false;
	//스킨 메쉬 애니메이션 Update 옵션
	if (USkinnedMeshComponent* skinnedMeshCom = Cast<USkinnedMeshComponent>(Component))
	{
		skinnedMeshCom->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	}
	//primitive 쉐도우 연산 관련 플레그
	Component->bCastDynamicShadow = true;
	//primitive 라이트 연산 관련 플레그
	Component->bAffectDynamicIndirectLighting = true;
	//컴퍼넌트의 Tick 간격 설정 :
	//http://api.unrealengine.com/KOR/Programming/UnrealArchitecture/Actors/Ticking/index.html#%ED%8B%B1%EA%B7%B8%EB%A3%B9
	Component->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	static FName CollisionProfileName(TEXT("CharacterMesh"));
	//콜리 전 오브젝트 유형 설정
	Component->SetCollisionObjectType(ECC_Pawn);
	Component->SetCollisionProfileName(CollisionProfileName);
}

void AGsCharacterPlayer::InitializeDecals()
{
	float fdecalHeight = 50.0f;
	float fdecalSize = 64.0f;
	UCapsuleComponent* capsule = GetCapsuleComponent();
	if (nullptr != capsule)
	{
		fdecalSize = capsule->GetUnscaledCapsuleRadius() * RootComponent->GetComponentScale().X;
		fdecalHeight = capsule->GetUnscaledCapsuleHalfHeight();
	}

	// 타겟을 마킹하기위한 컴포넌트
	_targetMaker = FindComponentByClass<UDecalComponent>();
	if (_targetMaker)
	{
		_targetMaker->SetRelativeLocation(FVector(0, 0, -fdecalHeight));
		_targetMaker->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
		_targetMaker->SetFadeIn(0.0f, 0.1f);
		_targetMaker->DecalSize = FVector(fdecalSize, fdecalSize * 2.0f, fdecalSize * 2.0f);
		_targetMaker->SetVisibility(false);
		_targetMaker->SetComponentTickEnabled(false);
	}
}


void AGsCharacterPlayer::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	UStaticMeshComponent* meshComp = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (nullptr == meshComp)
		return;

	if (nullptr == meshComp->BodyInstance.GetSimplePhysicalMaterial())
		return;

	FGsFootPrint overlapedInfo;
	overlapedInfo.material = Cast<UGsPhysicalMaterial>(meshComp->BodyInstance.GetSimplePhysicalMaterial());
	overlapedInfo.location = OtherActor->GetActorLocation();
	overlapedInfo.rotation = OtherActor->GetActorRotation();

	_footPrintOverlapList.Emplace(overlapedInfo);
}

void AGsCharacterPlayer::NotifyActorEndOverlap(AActor* OtherActor)
{
	UStaticMeshComponent* meshComp = Cast<UStaticMeshComponent>(OtherActor->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (nullptr == meshComp)
		return;

	if (nullptr == meshComp->BodyInstance.GetSimplePhysicalMaterial())
		return;

	UGsPhysicalMaterial* otherFootPrintMaterial = Cast<UGsPhysicalMaterial>(meshComp->BodyInstance.GetSimplePhysicalMaterial());
	for (int i = 0; i < _footPrintOverlapList.Num(); ++i)
	{
		if (otherFootPrintMaterial == _footPrintOverlapList[i].material)
		{
			_footPrintOverlapList.RemoveAt(i);
			break;
		}
	}

	Super::NotifyActorEndOverlap(OtherActor);
}

void AGsCharacterPlayer::SetActorHiddenInGame(bool bNewHidden)
{ 
	if (IsHidden() != bNewHidden)
	{
		Super::SetActorHiddenInGame(bNewHidden);

		if (!IsMeshChanged())
		{
			SetHiddenCostumeEffect(bNewHidden);
		}
	}
}

void AGsCharacterPlayer::InitWidgetComponent()
{
	Super::InitWidgetComponent();

	FName widgetClassName = GetWidgetClassName();
	if (TEXT("") == widgetClassName)
	{
		return;
	}

	if (nullptr == _nameplateWidget->GetWidgetClass().Get())
	{
		if (UGsPathManager* pathMgr = GPath())
		{
			UClass* widgetClass = pathMgr->GetClass(EGsPathType::UI, widgetClassName);
			if (widgetClass)
			{
				_nameplateWidget->SetWidgetClassWithPooling(TSubclassOf<UUserWidget>(widgetClass));
			}
		}
	}

	FTransform trans = GetMesh()->GetSocketTransform(TEXT("OverHeadSocket"), RTS_Component);
	FVector newTrans = FVector::ZeroVector;
	newTrans.Z = trans.GetTranslation().Z + _namePlateZOffset;
	
	_nameplateWidget->SetPivot(FVector2D(0.5f, 1.0f));
	_nameplateWidget->SetRelativeLocation(newTrans);
}

void AGsCharacterPlayer::SetEnableDeathActor(bool bInEnable)
{
	// 액터 감추기/보이게 하기
	SetActorHiddenInGame(bInEnable);

	// 충돌은 꺼준다 (https://jira.com2us.com/jira/browse/CHR-14978)
	SetActorEnableCollision(!bInEnable);

	if (bInEnable)
	{
		// 무덤 액터 스폰
		if (nullptr == _deathActor)
		{
			UClass* tombClass = nullptr;
			if (UGsPathManager* pathManager = GPath())
			{
				tombClass = pathManager->GetClass(EGsPathType::GameObject, TEXT("TombPath"));
			}

			if (nullptr != tombClass)
			{
				if (UWorld* world = GetWorld())
				{
					_deathActor = world->SpawnActor(tombClass, &FTransform::Identity);
				}
			}
		}

		FTransform trans = GetTransform();
		float yaw = trans.GetRotation().Rotator().Yaw;
		FRotator rot = FRotator(0.0f, yaw, 0.0f);

		_deathActor->SetActorLocationAndRotation(trans.GetLocation(), rot);
	}
	else
	{
		// 액터 디스폰
		if (_deathActor)
		{
			if (UWorld* world = GetWorld())
			{
				world->DestroyActor(_deathActor);
				_deathActor = nullptr;
			}
		}
	}
}

EGsAnimStateBlendType AGsCharacterPlayer::DecideBlendTypebyAnimState(EGsStateBase currentState)
{
	EGsAnimStateBlendType result;
	switch (currentState)
	{
	case EGsStateBase::Idle:
	case EGsStateBase::ChangeIdle:
		result = EGsAnimStateBlendType::BodyFull;
		break;
	case EGsStateBase::ForwardWalk:
	case EGsStateBase::Run:
	case EGsStateBase::AutoMove:	
		result = EGsAnimStateBlendType::BodyUpper;
		break;
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

void AGsCharacterPlayer::StopMeshShader()
{
	if (UGsMeshShaderControlComponent* meshShaderComp = GetMeshShaderComponent())
	{
		meshShaderComp->Stop();
	}

	if (AGsWeaponActor* weapon = GetWeapon())
	{
		if (weapon->GetMeshShaderControlComponent())
		{
			weapon->GetMeshShaderControlComponent()->Stop();
		}		
	}

	if (AGsWeaponActor* subWeapon = GetWeaponSub())
	{
		if (subWeapon->GetMeshShaderControlComponent())
		{
			subWeapon->GetMeshShaderControlComponent()->Stop();
		}
	}
}

void AGsCharacterPlayer::StartMeshShader(EGsMeshShaderEffect inType, bool inIsRestor)
{
	Super::StartMeshShader(inType, inIsRestor);

	// [B1] | ejrrb10 | 이 노티파이는 워프 시 플레이어 텔레포트 몽타주에 붙어서 호출됨
	// 자기 자신 뿐만 아니라 다른 사람이 보기에도 무기가 자연스럽게 사라지도록 연출해야 함
	// 현 시점으로는 플레이어, 무기 메시를 처리해야 함
	if (UGsGameObjectManager* mgr = GSGameObject())
	{
		if (UGsGameObjectBase* local = mgr->FindObject(EGsGameObjectType::LocalPlayer))
		{
			const FGsSchemaCreatureCommonInfo* commonInfoTable = local->GetCommonInfoTable();

			const UCurveFloat* curveData = (commonInfoTable->meshShader.Contains(inType)) ? commonInfoTable->meshShader[inType] : nullptr;
			if (AGsWeaponActor* weapon = GetWeapon())
			{
				weapon->ShowMeshShaderEffect(inType, curveData, inIsRestor);
			}

			if (AGsWeaponActor* subWeapon = GetWeaponSub())
			{
				subWeapon->ShowMeshShaderEffect(inType, curveData, inIsRestor);
			}
		}
	}
}

void AGsCharacterPlayer::SetVisibleTargetMaker(bool InVisible, UMaterialInterface* InMaterial)
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

void AGsCharacterPlayer::StartMeshShader(EGsMeshShaderEffect inType, const FGsSchemaCreatureCommonInfo* inInfo)
{
	const UCurveFloat* findCurve = (inInfo->meshShader.Contains(inType)) ? inInfo->meshShader[inType] : nullptr;
	if (UGsMeshShaderControlComponent* component = GetMeshShaderComponent())
	{
		component->Add(EGsMeshShaderEffect::Fresnel, findCurve);
		component->Start();
	}

	if (AGsWeaponActor* weapon = GetWeapon())
	{
		weapon->GetMeshShaderControlComponent()->Add(inType, findCurve);
		weapon->GetMeshShaderControlComponent()->Start();
	}

	if (AGsWeaponActor* subWeapon = GetWeaponSub())
	{
		subWeapon->GetMeshShaderControlComponent()->Add(inType, findCurve);
		subWeapon->GetMeshShaderControlComponent()->Start();
	}
}

void AGsCharacterPlayer::ChangeWeaponSocketProcess(TArray<FName> ListSocketName, TArray<FVector> ListOffset, 
	UCurveFloat* WeaponDissolve, UCurveFloat* WeaponFrsnel, UParticleSystem* Particle, bool IsStartProcess,
	TFunction<void()> EndCallback)
{
	if (UGsMeshShaderControlComponent* comp = GetMeshShaderComponent())
	{
		if (nullptr == WeaponDissolve && nullptr == WeaponFrsnel)
		{
			if (EndCallback)
			{
				EndCallback();
			}
		}
		else
		{
			AGsWeaponActor* weaponActor = GetWeapon();
			AGsWeaponActor* subWeaponActor = GetWeaponSub();
			UStaticMeshComponent* weaponMeshR = ((nullptr != weaponActor) ? weaponActor->GetStaticMeshComponent() : nullptr);
			UStaticMeshComponent* weaponMeshL = ((nullptr != subWeaponActor) ? subWeaponActor->GetStaticMeshComponent() : nullptr);
			bool isSubMesh = (weaponMeshL && weaponMeshL->GetStaticMesh());

			// 세 데이터중 종료 시간이 긴쪽에 Callback 처리
			// 를 하기엔 쓸데없는 처리가 되어 AD님과 동일 시간으로 가기로 합의됨
			/*float fresnelMax = 0.f;
			float dissolveMax = 0.f;
			float particleDissolveMax = 0.f;
			float min = 0.f;
			if (WeaponDissolve)
			{
				WeaponDissolve->GetTimeRange(min, dissolveMax);
			}
			if (WeaponFrsnel)
			{
				WeaponFrsnel->GetTimeRange(min, fresnelMax);
			}
			if (ParticleDissolve)
			{
				ParticleDissolve->GetTimeRange(min, particleDissolveMax);
			}*/
			if (isSubMesh && subWeaponActor)
			{
				if (IsStartProcess)
				{
					subWeaponActor->ShowEnchantEffect(WeaponDissolve, WeaponFrsnel);
				}
				else
				{
					subWeaponActor->HideEnchantEffect(WeaponDissolve, WeaponFrsnel);
				}
			}

			if (weaponActor)
			{
				if (IsStartProcess)
				{
					weaponActor->ShowEnchantEffect(WeaponDissolve, WeaponFrsnel);
				}
				else
				{
					weaponActor->HideEnchantEffect(WeaponDissolve, WeaponFrsnel);
				}
			}

			_weaponChangeInfo.Start(WeaponDissolve, EndCallback);
		}
	}

	if (Particle && false == IsHidden())
	{
		for (int i = 0; i < ListSocketName.Num(); ++i)
		{
			FVector offset = ListOffset.IsValidIndex(i) ? ListOffset[i] : FVector::ZeroVector;

			
			UGameplayStatics::SpawnEmitterAttached(Particle,
				GetRootComponent(),
				ListSocketName[i],
				offset,
				FRotator::ZeroRotator,
				FVector(1.f),
				EAttachLocation::SnapToTargetIncludingScale,
				true,
				EPSCPoolMethod::AutoRelease);
		}
	}
}

AGsWeaponActor* AGsCharacterPlayer::GetWeapon_Internal(UChildActorComponent* inChildActor) const
{
	if (inChildActor)
	{
		return Cast<AGsWeaponActor>(inChildActor->GetChildActor());
	}

	return nullptr;
}

bool AGsCharacterPlayer::SetWeaponBlueprintClass_Internal(UChildActorComponent* inChildActor, FSoftObjectPath inPath, FGsWeaponLoadCompleteDelegate& inAfter, bool isAsync)
{
	if (inChildActor)
	{
		inChildActor->DestroyChildActor();

		if (inPath.IsValid())
		{
			FString weaponBlueprintClassPath = inPath.ToString() + TEXT("_C");
			if (false == isAsync)
			{
				TSubclassOf<AActor> weaponBlueprintClass = TSoftClassPtr<AActor>(weaponBlueprintClassPath).LoadSynchronous();
				inChildActor->SetChildActorClass(weaponBlueprintClass);				
				if (inAfter.IsBound())
				{
					inAfter.Broadcast();
				}
			}
			else
			{
				TWeakObjectPtr<UChildActorComponent> weakChild(inChildActor);
				GResource()->LoadAsync(weaponBlueprintClassPath, FStreamableDelegateParam::CreateLambda([weakChild, inAfter](UObject* inRes)->void
					{
						if (weakChild.IsValid())
						{
							TSubclassOf<AActor> weaponBlueprintClass = Cast<UClass>(inRes);
							weakChild->SetChildActorClass(weaponBlueprintClass);							
							if (inAfter.IsBound())
							{
								inAfter.Broadcast();
							}
						}						
					}));
			}
			return true;
		}
	}

	return false;
}

bool AGsCharacterPlayer::SetWeaponEnchantEffect_Internal(AGsWeaponActor* inWeapon, FGsItem* inItem)
{
	if (nullptr == inItem || nullptr == inWeapon)
	{
		return false;
	}

	const FGsItemEquipment* equipItem = inItem->GetCastItem<FGsItemEquipment>();
	if (nullptr == equipItem)
	{
		return false;
	}

	inWeapon->SetEnchantEffect(inItem);

	return true;
}

bool AGsCharacterPlayer::SetWeaponEnchantEffect_Internal(AGsWeaponActor* inWeapon, CreatureWeaponType inWeaponType, int32 inWeaponLevel)
{
	if (CreatureWeaponType::NONE == inWeaponType)
	{
		return false;
	}

	if (nullptr == inWeapon)
	{
		return false;
	}

	inWeapon->SetEnchantEffect(inWeaponType, inWeaponLevel);

	return true;
}

void AGsCharacterPlayer::DestroyWeapon_Internal(UChildActorComponent* inChildActor)
{
	if (inChildActor)
	{
		inChildActor->DestroyChildActor();
	}
}

void AGsCharacterPlayer::RestoreDefaultFootPrint()
{
	if (GLevel())
	{
		if (const FGsSchemaMapData* mapInfo = GLevel()->GetCurrentMapInfo())
		{
			_defaultFootPrint.effect = mapInfo->footPrintEffect;
			_defaultFootPrint.material = mapInfo->footPrintMaterial;
			_defaultFootPrint.method = mapInfo->footPrintMethod;
		}
		else
		{
			_defaultFootPrint.effect = nullptr;
			_defaultFootPrint.material = nullptr;
			_defaultFootPrint.method = EGsFootPrintMethod::MAP;
		}
	}
}

void AGsCharacterPlayer::AddFootPrintEffectByTrigger(class UParticleSystem* inEffect, class UGsPhysicalMaterial* inMaterial)
{
	FGsFootPrint footPrintInfo;
	footPrintInfo.effect = inEffect;
	footPrintInfo.material = inMaterial;
	footPrintInfo.method = EGsFootPrintMethod::MAP;
	
	_footPrintTriggerList.Emplace(footPrintInfo);
}

void AGsCharacterPlayer::RemoveFootPrintEffectByTrigger(class UParticleSystem* inEffect, class UGsPhysicalMaterial* inMaterial)
{
	for (int i = 0; i < _footPrintTriggerList.Num(); ++i)
	{
		if ((inEffect && inEffect == _footPrintTriggerList[i].effect) ||
			(inMaterial && inMaterial == _footPrintTriggerList[i].material))
		{
			_footPrintTriggerList.RemoveAt(i);
			return;
		}
	}
}

//UGsAnimInstanceState* AGsCharacterPlayer::GetAnim() const
//{
//	if (_polymorpActor && _polymorpSkeletalMesh)
//	{
//		return Cast<UGsAnimInstanceState>(_polymorpSkeletalMesh->GetAnimInstance());
//	}
//
//	return Super::GetAnim();
//}

AGsWeaponActor* AGsCharacterPlayer::GetWeapon() const
{
	return GetWeapon_Internal(_partsWeaponChildActor);
}

AGsWeaponActor* AGsCharacterPlayer::GetWeaponSub() const
{
	return GetWeapon_Internal(_partsWeaponSubChildActor);
}

bool AGsCharacterPlayer::SetWeaponBlueprintClass(FSoftObjectPath inPath, FGsWeaponLoadCompleteDelegate& inAfter, bool isAsync)
{
	return SetWeaponBlueprintClass_Internal(_partsWeaponChildActor, inPath, inAfter, isAsync);
}

bool AGsCharacterPlayer::SetWeaponSubBlueprintClass(FSoftObjectPath inPath, FGsWeaponLoadCompleteDelegate& inAfter, bool isAsync)
{
	return SetWeaponBlueprintClass_Internal(_partsWeaponSubChildActor, inPath, inAfter, isAsync);
}

bool AGsCharacterPlayer::SetWeaponEnchantEffect(FGsItem* inItem)
{
	return SetWeaponEnchantEffect_Internal(GetWeapon(), inItem);
}

bool AGsCharacterPlayer::SetWeaponSubEnchantEffect(FGsItem* inItem)
{
	return SetWeaponEnchantEffect_Internal(GetWeaponSub(), inItem);
}

bool AGsCharacterPlayer::SetWeaponEnchantEffect(CreatureWeaponType inWeaponType, int32 inWeaponLevel)
{
	return SetWeaponEnchantEffect_Internal(GetWeapon(), inWeaponType, inWeaponLevel);
}

bool AGsCharacterPlayer::SetWeaponSubEnchantEffect(CreatureWeaponType inWeaponType, int32 inWeaponLevel)
{
	return SetWeaponEnchantEffect_Internal(GetWeaponSub(), inWeaponType, inWeaponLevel);
}

void AGsCharacterPlayer::DestroyWeapon()
{
	DestroyWeapon_Internal(_partsWeaponChildActor);
}

void AGsCharacterPlayer::DestroyWeaponSub()
{
	DestroyWeapon_Internal(_partsWeaponSubChildActor);
}

void AGsCharacterPlayer::FGsCharacterPlayerWeaponChangeInfo::Clear()
{
	_startTime = -1.f;
	_endTime = -1.f;
	_curveData = nullptr;
}

void AGsCharacterPlayer::FGsCharacterPlayerWeaponChangeInfo::Start(UCurveFloat* CurveData, TFunction<void()> EndCallback)
{
	_curveData = CurveData;
	_endCallback = EndCallback;
	_delta = 0.f;
	_startTime = FApp::GetCurrentTime();
	float minTime;
	_curveData->GetTimeRange(minTime, _endTime);
}

void AGsCharacterPlayer::FGsCharacterPlayerWeaponChangeInfo::Update(float Delta)
{
	if (IsValid())
	{
		_delta += Delta;
		float elapsed = FApp::GetCurrentTime() - _startTime;
		if (elapsed > _endTime)
		{
			Clear();

			if (_endCallback)
			{
				_endCallback();
			}
		}
	}
}

void AGsCharacterPlayer::SetHiddenWeapon(bool inHidden)
{
	if (_partsWeaponChildActor && _partsWeaponChildActor->GetChildActor())
	{
		_partsWeaponChildActor->GetChildActor()->SetActorHiddenInGame(inHidden);
	}

	if (_partsWeaponSubChildActor && _partsWeaponSubChildActor->GetChildActor())
	{
		_partsWeaponSubChildActor->GetChildActor()->SetActorHiddenInGame(inHidden);
	}
}

void AGsCharacterPlayer::SetWeaponTransformByCurve(UChildActorComponent* weaponComponent, const FGsSchemaWeaponPeaceInfo* Info, bool InBattle)
{
	if (InBattle || nullptr == weaponComponent || nullptr == Info)
	{
		return;
	}
	if (auto WeaponActor = Cast<AGsWeaponActor>(weaponComponent->GetChildActor()))
	{
		WeaponActor->UpdateLocationCurve = Info->UpdateLocation;
		WeaponActor->UpdateRotationCurve = Info->UpdateRotation;
		if (GData())
		{
			WeaponActor->LocationMultiply = GData()->GetGlobalData()->WeaponCurveLocationSpeed;
			WeaponActor->RatationMultiply = GData()->GetGlobalData()->WeaponCurveRotationSpeed;
		}
		WeaponActor->StartCurveAnim();
	}
}


void AGsCharacterPlayer::StopWeaponTransformByCurve()
{
	if (auto WeaponActor = Cast<AGsWeaponActor>(GetWeaponChildActorComponent()->GetChildActor()))
	{
		WeaponActor->StopCurveAnim();
	}

	if (auto WeaponActorSub = Cast<AGsWeaponActor>(GetWeaponSubChildActorComponent()->GetChildActor()))
	{
		WeaponActorSub->StopCurveAnim();
	}
}

void AGsCharacterPlayer::SetCostumeEffectBlueprintClass(const FGsSchemaCostumeEffect* InEffectInfo, bool isAsync)
{
	//이전에 착용한 이펙트가 있다면 하위 정보 소멸
	if (nullptr == InEffectInfo)
	{
		return;
	}

	auto fnLoadAfterEffect = [this](const FGsSchemaCostumeEffect* InEffectInfo)->void
	{
		_costumeEffectChildActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, InEffectInfo->socketName);
		_costumeEffectChildActor->SetRelativeTransform(InEffectInfo->socketTransform);

		TArray<USceneComponent*> childSceneComponents;
		_costumeEffectChildActor->GetChildrenComponents(true, childSceneComponents);
		for (USceneComponent* childComponent : childSceneComponents)
		{
			if (UGsCostumeEffectComponent* costumeEffectComponent = Cast<UGsCostumeEffectComponent>(childComponent))
			{
				costumeEffectComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, costumeEffectComponent->GetSocketName());
				costumeEffectComponent->SetRelativeTransform(costumeEffectComponent->GetSocketOffsetTransform());
			}
		}

		SetHiddenCostumeEffect(IsHidden());

		if (OnCostumeEffectLoadComplate.IsBound())
		{
			OnCostumeEffectLoadComplate.Broadcast(_costumeEffectChildActor->GetChildActor());
		}
	};

	if (nullptr != _equipCostumeEffectInfo)
	{
		if (InEffectInfo->path != _equipCostumeEffectInfo->path)
		{
			_costumeEffectChildActor->DestroyChildActor();
		}
		else
		{
			//리소스가 같을때는 새로로드하지 않고 위치만 변경시킨다.
			fnLoadAfterEffect(InEffectInfo);
			_equipCostumeEffectInfo = InEffectInfo;
			return;
		}
	}

	if (InEffectInfo->path.IsValid())
	{
		FString blueprintClassPath = InEffectInfo->path.ToString() + TEXT("_C");

		if (false == isAsync)
		{	
			TSubclassOf<AActor> blueprintClass = TSoftClassPtr<AActor>(blueprintClassPath).LoadSynchronous();
			_costumeEffectChildActor->SetChildActorClass(blueprintClass);
			_equipCostumeEffectInfo = InEffectInfo;

			fnLoadAfterEffect(InEffectInfo);
		}
		else
		{
			TWeakObjectPtr<AGsCharacterPlayer> weakThis(this);
			GResource()->LoadAsync(blueprintClassPath, FStreamableDelegateParam::CreateLambda([weakThis, InEffectInfo, fnLoadAfterEffect](UObject* inRes)->void
				{
					if (weakThis.IsValid())
					{
						TSubclassOf<AActor> weaponBlueprintClass = Cast<UClass>(inRes);
						weakThis->_costumeEffectChildActor->SetChildActorClass(weaponBlueprintClass);
						weakThis->_equipCostumeEffectInfo = InEffectInfo;

						fnLoadAfterEffect(InEffectInfo);
					}					
				}));
		}
	}
	else
	{
		_costumeEffectChildActor->DestroyChildActor();
		_equipCostumeEffectInfo = InEffectInfo;
	}
}

void AGsCharacterPlayer::DestroyCostumeEffect()
{
	if (_costumeEffectChildActor)
	{
		_costumeEffectChildActor->DestroyChildActor();
	}
}

void AGsCharacterPlayer::SetHiddenCostumeEffect(bool inHidden)
{
	if (_costumeEffectChildActor && _costumeEffectChildActor->GetChildActor())
	{
		_costumeEffectChildActor->GetChildActor()->SetActorHiddenInGame(inHidden);
	}
}

void AGsCharacterPlayer::SetToolWeaponStaticMeshVisible(bool InVisibilityFlag)
{
	if (nullptr == _toolEquipmentStaticMesh)
		return;

	_toolEquipmentStaticMesh->SetVisibility(InVisibilityFlag);
}

#if WITH_EDITOR
void AGsCharacterPlayer::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	InvalidateCostumeEffectChildActor();
}

void AGsCharacterPlayer::InvalidateCostumeEffectChildActor()
{
	if (_costumeEffectChildActor)
	{
		TArray<USceneComponent*> childSceneComponents;
		_costumeEffectChildActor->GetChildrenComponents(true, childSceneComponents);

		for (USceneComponent* childComponent : childSceneComponents)
		{
			if (UGsCostumeEffectComponent* costumeEffectComponent = Cast<UGsCostumeEffectComponent>(childComponent))
			{
				costumeEffectComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, costumeEffectComponent->GetSocketName());
			}
		}

		if (false == _costumeEffectChildActor->IsRegistered() && GetWorld() != nullptr)
		{
			_costumeEffectChildActor->RegisterComponent();
		}
	}
}

#endif