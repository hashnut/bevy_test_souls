// Fill out your copyright notice in the Description page of Project Settings.

#include "GsWeaponActor.h"
#include "Curves/CurveVector.h"
#include "ActorComponentEx/EGsMeshShaderEffect.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "ActorComponentEx/GsMeshShaderControlComponent.h"
#include "Item/GsSchemaItemEquipWeaponEnchantEffect.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemEquipment.h"
#include "Util/GsTimeUtil.h"
#include "GsTableManager.h"
#include "Message/GsMessageContents.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GsCharacterPlayer.h"
#include "Components/SceneComponent.h"

#define ACTOR_WEAPON_DEBUG false

AGsWeaponActor::AGsWeaponActor()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	_rootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent(Scene)"));
	{
		SetRootComponent(_rootComponent);
	}

	_staticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	if(_staticMeshComponent)
	{
		_staticMeshComponent->SetupAttachment(_rootComponent);
	}
	
	_meshShaderComponent = CreateDefaultSubobject<UGsMeshShaderControlComponent>(TEXT("MeshShaderControlComponent"));

	_particleSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ParticleSceneComponent"));
	if (_particleSceneComponent)
	{
		_particleSceneComponent->SetupAttachment(_rootComponent);
	}
	
	_particleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	if(_particleSystemComponent)
	{
		_particleSystemComponent->SetupAttachment(_particleSceneComponent);
		// 이펙트 제작의 기준이 되는 회전축이 인게임의 기준 회전축과 다르기때문에 동일하게 맞춰준다.
		// (에디터에서는 전방축이 Y, 인게임에선 X를 기준으로 한다.)
		// 원래라면 계층구조상 부모의 회전값이 반영되어야 하지만 이펙트 이미터의 여러 모듈 특성상 문제가 되는경우가 있는듯
		// 따라서 이종류의 이펙트 제작 기준이 바뀌게 되면 문제가 될수도 있다.(동일한 기준으로 제작되어야함)
		_particleSystemComponent->SetRelativeRotation(FRotator(-90.f, 0.f, 0.f));
	}	
}

void AGsWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	RegisterStaticMeshToMeshShader();

	SetActorTickEnabled(false);

	
	/*_timer = NewObject<UGsCurveTimer>();*/
	fLocationAccumTime = fRotationAccumTime = 0.0f;

	FGsMessageHolder* msg = GMessage();
	_listItemEnchantDelegate.Empty();
	_listItemEnchantDelegate.Emplace(msg->GetItemEnchant().AddUObject(MessageContentEnchant::INVALIDATE, this, &AGsWeaponActor::OnEnchantSuccess));


#if WITH_EDITOR && ACTOR_WEAPON_DEBUG
	FString log = TEXT("AGsWeaponActor - BeginPlay - label : ") + GetActorLabel();
	if (_staticMeshComponent->GetStaticMesh())
	{
		log += TEXT("\t static mesh exist");
	}
	else
	{
		log += TEXT("\t static mesh missing");
	}

	GSLOG(Log, TEXT("%s"), *log);
#endif
}

void AGsWeaponActor::EndPlay(const EEndPlayReason::Type inEndPlayReason)
{
	FGsMessageHolder* msg = GMessage();
	for (TPair<MessageContentEnchant, FDelegateHandle>& elemItem : _listItemEnchantDelegate)
	{
		msg->GetItemEnchant().Remove(elemItem);
	}

	_listItemEnchantDelegate.Empty();

#if WITH_EDITOR && ACTOR_WEAPON_DEBUG
	FString log = TEXT("AGsWeaponActor - EndPlay - label : ") + GetActorLabel();	
	GSLOG(Log, TEXT("%s"), *log);
#endif

	if (_timer)
	{
		_timer->Clear();
		_timer->ConditionalBeginDestroy();
		_timer = nullptr;
	}	

	_meshShaderComponent->Stop();

	Super::EndPlay(inEndPlayReason);
}

void AGsWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	static FRotator rotFix = FRotator(-90.0f, 0.0f, 0.0f);

	if (nullptr != UpdateLocationCurve)
	{
		fLocationAccumTime += DeltaTime;

		float TimeMin;
		float TimeMax;
		UpdateLocationCurve->GetTimeRange(TimeMin, TimeMax);
		if (TimeMax > 0.0f)
		{
			while (fLocationAccumTime > TimeMax)
			{
				fLocationAccumTime -= TimeMax;
			}
			FVector ModifyTrans = rotFix.RotateVector(UpdateLocationCurve->GetVectorValue(fLocationAccumTime));
			_rootComponent->SetRelativeLocation(ModifyTrans * LocationMultiply);
		}
	}

	if (nullptr != UpdateRotationCurve)
	{
		fRotationAccumTime += DeltaTime;

		float TimeMin;
		float TimeMax;
		UpdateRotationCurve->GetTimeRange(TimeMin, TimeMax);
		if (TimeMax > 0)
		{
			while (fRotationAccumTime > TimeMax)
			{
				fRotationAccumTime -= TimeMax;
			}
			FVector RotationSpeed = UpdateRotationCurve->GetVectorValue(fRotationAccumTime);
			RotationSpeed *= RatationMultiply;
			FRotator rotation = FRotator::ZeroRotator;
			rotation.Roll = (RotationSpeed.Z * DeltaTime);
			rotation.Yaw = (RotationSpeed.X * DeltaTime);
			rotation.Pitch = (RotationSpeed.Y * DeltaTime);
			_rootComponent->AddRelativeRotation(rotation.Quaternion());
		}
	}
}

void AGsWeaponActor::StartCurveAnim()
{
	if (nullptr == UpdateLocationCurve && nullptr == UpdateRotationCurve)
		return;

	fRotationAccumTime = fLocationAccumTime = 0.0f;
	
	SetActorTickEnabled(true);
}

void AGsWeaponActor::StopCurveAnim()
{
	_rootComponent->SetRelativeLocation(FVector::ZAxisVector);
	_rootComponent->SetRelativeRotation(FRotator::ZeroRotator);

	SetActorTickEnabled(false);
}

void AGsWeaponActor::SetEnchantEffect(const CreatureWeaponType inWeaponType, const FGsSchemaItemEquipWeaponEnchantEffect* inInfo)
{
	_enchantInfo = inInfo;
	TArray<UActorComponent*> componentsArray;
	GetComponents(UStaticMeshComponent::StaticClass(), componentsArray);
	TArray<UMeshComponent*> meshArray;

	for (UActorComponent* comp : componentsArray)
	{
		if (comp)
		{
			if (comp->IsA(UMeshComponent::StaticClass()))
			{
				meshArray.Add(Cast<UMeshComponent>(comp));
			}
		}
	}

	for (UMeshComponent* mesh : meshArray)
	{
		if (mesh)
		{
			mesh->SetVectorParameterValueOnMaterials(TEXT("EmissiveColor"), inInfo->LightColor);
		}
	}

	FSoftObjectPath path = GetWeaponEffectSoftObjectPath(inWeaponType, inInfo);
	if (path.IsValid())
	{
		FString particleBlueprintClassPath = path.ToString();
		TWeakObjectPtr<AGsWeaponActor> weakThis(this);
		GResource()->LoadAsync(particleBlueprintClassPath, FStreamableDelegateParam::CreateLambda([weakThis](UObject* inRes)->void
			{
				if (weakThis.IsValid())
				{
					weakThis->_particleSystemComponent->SetTemplate(Cast<UParticleSystem>(inRes));
				}				
			}));
	}
	else
	{
		_particleSystemComponent->SetTemplate(nullptr);
	}
}

void AGsWeaponActor::SetEnchantEffect(const CreatureWeaponType inWeaponType, const int32 inLevel)
{
	static const FGsSchemaItemEquipWeaponEnchantEffect DEFAULT_ENCHANT_EFFECT;

	const FGsSchemaItemEquipWeaponEnchantEffect* info = GetWeaponEnchantEffectInfo(inLevel);
	if (info)
	{
		SetEnchantEffect(inWeaponType, info);
	}
	else
	{
		SetEnchantEffect(inWeaponType, &DEFAULT_ENCHANT_EFFECT);
	}
}

void AGsWeaponActor::SetEnchantEffect(FGsItem* inItem)
{
	if (inItem)
	{
		int32 level = inItem->GetLevel();
		FGsItemEquipment* equipItem = inItem->GetCastItem<FGsItemEquipment>();
		if (nullptr == equipItem)
		{
			return;
		}

		CreatureWeaponType weapon = equipItem->GetCreatureWeaponType();
		SetEnchantEffect(weapon, level);
	}	
}

void AGsWeaponActor::RegisterStaticMeshToMeshShader()
{
	TArray<UActorComponent*> componentsArray;
	GetComponents(UStaticMeshComponent::StaticClass(), componentsArray);
	TArray<UMeshComponent*> meshArray;

	for (UActorComponent* comp : componentsArray)
	{
		if (comp)
		{
			if (comp->IsA(UMeshComponent::StaticClass()))
			{
				UMeshComponent* meshComponent = Cast<UMeshComponent>(comp);
				meshArray.Add(meshComponent);

				// 먼지 잘모르겠지만 일단 여기서 restore
				_meshShaderComponent->Restore(EGsMeshShaderEffect::Dissolve, meshComponent);
				_meshShaderComponent->Restore(EGsMeshShaderEffect::Fresnel, meshComponent);
			}
		}
	}

	if (_meshShaderComponent)
	{
		_meshShaderComponent->ClearMeshList();
	}
}

const FGsSchemaItemEquipWeaponEnchantEffect* AGsWeaponActor::GetWeaponEnchantEffectInfo(int32 inEnchantLevel)
{
	const FGsSchemaItemEquipWeaponEnchantEffect* effect = nullptr;
	const UGsTable* table = UGsTableManager::GetInstance().GetTable(FGsSchemaItemEquipWeaponEnchantEffect::StaticStruct());
	if (table)
	{
		TArray<const FGsSchemaItemEquipWeaponEnchantEffect*> enchantEffectList;
		if (table->GetAllRows(enchantEffectList))
		{
			int32 num = enchantEffectList.Num();
			for (int i = 0; i < num; ++i)
			{
				//현제 레벨 보다 낮으면 이전 강화 정보이다.
				if (enchantEffectList[i]->EnchantLevel > inEnchantLevel)
				{
					break;
				}

				effect = enchantEffectList[i];

#if WEAPON_DEBUG
				GSLOG(Log, TEXT("weapon debug - level : %d"), inEnchantLevel);
#endif
			}
		}
	}

	return effect;
}

//OnEnchantSuccess is called when only local player enchant item
void AGsWeaponActor::OnEnchantSuccess(const struct IGsMessageParam* InMessageParam)
{
	//check this weapon actor is player weapon
	UGsGameObjectBase* localPlayer = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	UGsGameObjectLocalPlayer* castLocalPlayer = Cast<UGsGameObjectLocalPlayer>(localPlayer);
	if (nullptr == castLocalPlayer)
	{
		return;
	}

	AGsCharacterPlayer* playerCharacter = Cast<AGsCharacterPlayer>(castLocalPlayer->GetCharacter());
	if (nullptr == playerCharacter)
	{
		return;
	}

	if (playerCharacter->GetWeapon() != this && playerCharacter->GetWeaponSub() != this)
	{
		return;
	}

	//if player weapon, update weapon level effect
	if (nullptr == InMessageParam)
		return;

	const FGsUpdateEnchantItem* paramData = InMessageParam->Cast<const FGsUpdateEnchantItem>();
	if (nullptr == paramData)
		return;

	TWeakPtr<FGsItem> equipItem = GItem()->FindItem(paramData->_equipItemDBID, ItemType::EQUIP);
	TWeakPtr<FGsItem> item = GItem()->GetPlayerEquipedItemBySlotType(EGsEquipSlotType::WEAPON_TYPE);
	if (false == equipItem.IsValid() || false == item.IsValid() || equipItem != item)
	{
		return;
	}

	int32 enchantLevel = item.Pin()->GetLevel();
	const FGsItemEquipment* castEquipItem = item.Pin()->GetCastItem<FGsItemEquipment>();
	if (castEquipItem)
	{
		SetEnchantEffect(castEquipItem->GetCreatureWeaponType(), enchantLevel);
	}	
}

FSoftObjectPath AGsWeaponActor::GetWeaponEffectSoftObjectPath(const CreatureWeaponType inWeaponType, const FGsSchemaItemEquipWeaponEnchantEffect* inInfo)
{
	FSoftObjectPath path;
	if (inInfo)
	{
		switch (inWeaponType)
		{
		case CreatureWeaponType::SWORD:
			path = inInfo->SwordEffect;
			break;
		case CreatureWeaponType::DAGGER:
			path = inInfo->DaggerEffect;
			break;
		case CreatureWeaponType::BOW:
			path = inInfo->BowEffect;
			break;
		case CreatureWeaponType::WAND:
			path = inInfo->WandEffect;
			break;
		case CreatureWeaponType::ORB:
			path = inInfo->orbEffect;
			break;
		case CreatureWeaponType::SHOTGUN:
			path = inInfo->shotgunEffect;
			break;
		default:
			break;
		}
	}	

	return path;
}

void AGsWeaponActor::ShowEnchantEffect(UCurveFloat* WeaponDissolve, UCurveFloat* WeaponFrsnel)
{
	// SetActorHiddenInGame(false);

	// 데이터 설정
	/*_timer->SetCurve(_enchanztEffectShowCurve);
	_timer->Reset();
	_timer->Start();*/
	_particleSystemComponent->Deactivate();	
	_meshShaderComponent->Stop();

	TArray<UStaticMeshComponent*> componentsArray;
	GetComponents(componentsArray, true);
	for (UStaticMeshComponent* comp : componentsArray)
	{
		_meshShaderComponent->Add(EGsMeshShaderEffect::Fresnel, comp, WeaponFrsnel);
		_meshShaderComponent->Add(EGsMeshShaderEffect::Dissolve, comp, WeaponDissolve);
	}
	_meshShaderComponent->Start();
}

void AGsWeaponActor::HideEnchantEffect(UCurveFloat* WeaponDissolve, UCurveFloat* WeaponFrsnel)
{	
	_particleSystemComponent->ResetParticles(false);	
	_particleSystemComponent->ActivateSystem();

	_meshShaderComponent->Stop();
	TArray<UStaticMeshComponent*> componentsArray;
	GetComponents(componentsArray, true);
	for (UStaticMeshComponent* comp : componentsArray)
	{
		_meshShaderComponent->Add(EGsMeshShaderEffect::Fresnel, comp, WeaponFrsnel);
		_meshShaderComponent->Add(EGsMeshShaderEffect::Dissolve, comp, WeaponDissolve);
	}
	_meshShaderComponent->Start();
}

void AGsWeaponActor::ShowMeshShaderEffect(EGsMeshShaderEffect inType, const UCurveFloat* inCurveFloat, bool inIsRestor)
{
	TArray<UStaticMeshComponent*> componentsArray;
	GetComponents(componentsArray, true);
	for (UStaticMeshComponent* comp : componentsArray)
	{
		_meshShaderComponent->Add(inType, comp, inCurveFloat, nullptr, inIsRestor);
	}
	_meshShaderComponent->Start();
}

void AGsWeaponActor::ChangeLightingChannels(bool channel1 /*= true*/, bool channel2 /*= false*/, bool channel3 /*= false*/)
{
	_staticMeshComponent->ChangeLightingChannels(channel1, channel2, channel3);
}
