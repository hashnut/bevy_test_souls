#include "GsGameObjectPlayableEventLocalPlayer.h"

#include "GameObject/ObjectClass/GsGameObjectPlayableEventNonPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/State/LocalPlayer/GsStateLocalPlayerSpawn.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerPlayableEventLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerPlayableEventLocalPlayer.h"
#include "GameObject/Define/GsGameObjectText.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Target/GsTargetHandlerPlayableEventLocalPlayer.h"
#include "GameObject/Damage/GsDamageHandlerBase.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/Environment/GsAreaEnvHandler.h"
#include "GameObject/Parts/GsPartsHandlerBase.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "Animation/GsAnimInstanceState.h"

#include "ActorEx/GsCharacterBase.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsCharacterNonPlayer.h"
#include "ActorEx/GsActorPlayableEventMonsterSpawn.h"
#include "ActorEx/GsWeaponActor.h"

#include "ActorComponentEx/GsEffectComponent.h"
#include "ActorComponentEx/GsHitEffectComponent.h"
#include "ActorComponentEx/GsPlayableEventNonPlayerComponent.h"
#include "ActorComponentEx/GsWidgetComponentPooling.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsResourceManager.h"

#include "DataSchema/Sound/GsSchemaSoundResData.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/User/GsSchemaUserData.h"

#include "Classes/GsSpawn.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsPathManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Message/GsMessageGameObject.h"

#include "Input/GsInputBindingLocalPlayer.h"

#include "UTIL/GsTableUtil.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/GameFramework/Controller.h"
#include "AIModule/Classes/AIController.h"

#include "UMG/Public/Components/WidgetComponent.h"


// 필요한 핸들러만 쓰자
void UGsGameObjectPlayableEventLocalPlayer::CreateSubClass()
{
	// Fsm Initialize는 각 클래스에서 처리
	if (nullptr == _fsm)
	{
		_fsm = CreateBaseFSM();
	}

	UGsGameObjectBase::CreateSubClass();

	// gameobectCreatrue에서 하던거
	AppendInitialize(CreateTarget());
	AppendInitialize(CreateDamageHandler());

	//if (AGsCharacterBase* actorCharacter = GetCharacter())
	//{
	//	if (UGsEffectComponent* effComp = actorCharacter->GetEffectComponent())
	//	{
	//		effComp->FindAttachTarget();
	//	}
	//	actorCharacter->InitMeshShaderComponent(GetCommonInfoTable());

	//	// 네임 플레이트 hide
	//	if (UWidgetComponent* nameWidgetComponent = actorCharacter->GetNameWidgetComponent())
	//	{
	//		nameWidgetComponent->SetActive(false);
	//		nameWidgetComponent->SetVisibility(false);
	//	}
	//}

	// gameobjectPlayer에서 하던거
	AppendInitialize(CreateParts());
	AppendInitialize(CreateCustomize());


	//if (FGsCustomizeHandler* customize = GetCustomizeHandler())
	//{
	//	if (FGsGameObjectDataPlayer* playerData = GetCastData<FGsGameObjectDataPlayer>())
	//	{
	//		customize->InitializeActor(GetCharacter());
	//		customize->InitializeScaleDisplacementAllBone(playerData->GetCreatureGenderType());
	//	}
	//}

	
	//// 플레이어 타입 히트 이펙트 데이터 설정
	//if (AGsCharacterBase* actorCharacter = GetCharacter())
	//{
	//	if (UGsHitEffectComponent* hitComp = actorCharacter->GetHitEffect())
	//	{
	//		if (_pcData)
	//		{
	//			UGsHitEffectData* effectInfo = _pcData->HitEffectDataOverride == nullptr ?
	//				GData()->GetGlobalData()->HitEffectData : _pcData->HitEffectDataOverride;
	//			const FGsSchemaSoundResData* soundInfo = _pcData->HitSoundDataOverride.IsNull() ?
	//				GData()->GetGlobalData()->HitSoundData.GetRow() : _pcData->HitSoundDataOverride.GetRow();

	//			hitComp->OverrideData(effectInfo, soundInfo);
	//		}
	//	}
	//}

	//// 전투 평화 상태 동기화
	//UpdateCombatStatus();

	//// gameobjectLocal에서 하던거

	//// 키입력 바인딩
	if (nullptr == _inputBinder)
	{
		FString path = GPath()->GetClassPath(EGsPathType::GameObject, BPLocalInputBinderName);
		if (UClass* binderClass = LoadClass<UObject>(NULL, *path, NULL, LOAD_None, NULL))
		{
			_inputBinder = NewObject<UGsInputBindingLocalPlayer>(this, binderClass, NAME_None);
		}
	}
	//==================================================================================================================
	AppendInitialize(CreateMovement());
	AppendInitialize(CreateSkill());
	

	//if (_inputBinder)
	//{
	//	_inputBinder->Initialize(this);
	//	_actor->SetInputBinder(_inputBinder);
	//}

	//// 인풋 바인딩 생성후 초기화해야함!!!
	//// 액터 초기화때 플레이어 컨트롤도 초기화함
	//if (_actor)
	//{
	//	_actor->Initialize(this);
	//}
	
	// cam mode 앞에 세팅 될 것
	// 1. 액터 인풋바인딩 세팅
	// 2. look handler 메시지 바인딩
	AppendInitialize(MakeShareable(new FGsCameraModeHandler()));
	AppendInitialize(MakeShareable(new FGsAreaEnvHandler()));	


	if (FGsMovementHandlerBase* move = GetMovementHandler())
	{
		const FGsSchemaCreatureCommonInfo* commonInfo = GetCommonInfoTable();
		move->InitSpeed(commonInfo->moveSpeed, commonInfo->walkSpeed);
	}

}

void UGsGameObjectPlayableEventLocalPlayer::InitializeActor(AActor* Owner)
{

	_actor = Cast<AGsCharacterLocalPlayer>(Owner);
	_fsm->InitializeActor();
	// 컨트롤러 빙의
	if (APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		controller->Possess(_actor);
	}

	UGsGameObjectBase::InitializeActor(Owner);
	
	// 필요 Primitive 셋업
	_actor->SetupPrimitives(true);

	//Hit 모션 추가
	FSoftObjectPath path;
	if (_actor->LoadHitAnimInstance(path))
	{
		LegacyHit = false;
	}


	if (AGsCharacterBase* actorCharacter = GetCharacter())
	{
		if (UGsEffectComponent* effComp = actorCharacter->GetEffectComponent())
		{
			effComp->FindAttachTarget();
		}
		actorCharacter->InitMeshShaderComponent(GetCommonInfoTable());

		// 네임 플레이트 hide
		if (UWidgetComponent* nameWidgetComponent = actorCharacter->GetNameWidgetComponent())
		{
			nameWidgetComponent->SetActive(false);
			nameWidgetComponent->SetVisibility(false);
		}

		if (FGsCustomizeHandler* customize = GetCustomizeHandler())
		{
			if (FGsGameObjectDataPlayer* playerData = GetCastData<FGsGameObjectDataPlayer>())
			{
				customize->InitializeActor(GetCharacter());
				customize->InitializeScaleDisplacementAllBone(playerData->GetCreatureGenderType());

			// 무조건 배틀 상태로 설정
			playerData->SetUserStatus(UserStatus::USER_STATE_COMBAT);
			}
		}

		// 플레이어 타입 히트 이펙트 데이터 설정	
		if (UGsHitEffectComponent* hitComp = actorCharacter->GetHitEffect())
		{
			if (_pcData)
			{
				const FGsSchemaCreatureCommonInfo* creatureInfo = GetCommonInfoTable();

				UGsHitEffectData* effectInfo = _pcData->HitEffectDataOverride == nullptr ?
					GData()->GetGlobalData()->HitEffectData : _pcData->HitEffectDataOverride;
				const FGsSchemaSoundResData* soundInfo = _pcData->HitSoundDataOverride.IsNull() ?
					GData()->GetGlobalData()->HitSoundData.GetRow() : _pcData->HitSoundDataOverride.GetRow();

				hitComp->OverrideData(effectInfo, soundInfo, 
					creatureInfo->hitEffectParticleSizeRate, creatureInfo->hitEffectModelRadius);
			}
		}

		// 전투 평화 상태 동기화
		UpdateCombatStatus();


		// gameobjectLocal에서 하던거
		if (_inputBinder)
		{
			_inputBinder->Initialize(this);
			_actor->SetInputBinder(_inputBinder);
		}

		// 인풋 바인딩 생성후 초기화해야함!!!
		// 액터 초기화때 플레이어 컨트롤도 초기화함
		if (_actor)
		{
			_actor->Initialize(this);
		}

		// 무기 타입에 따른 AnimationBP설정
		UpdateWeaponChange(CreatureWeaponType::SWORD);


		if (APlayerController* controller = GetWorld()->GetFirstPlayerController())
		{
			controller->SetViewTarget(actorCharacter);
		}

	}

	ChangeIdleType(EGsAnimStateIdleType::Battle);
}

TSharedPtr<FGsTargetHandlerBase> UGsGameObjectPlayableEventLocalPlayer::CreateTarget()
{
	return MakeShareable(new FGsTargetHandlerPlayableEventLocalPlayer());
}

void UGsGameObjectPlayableEventLocalPlayer::ActorSpawned(AActor* Spawn)
{
	Super::ActorSpawned(Spawn);


}

TSharedPtr<FGsSkillHandlerBase> UGsGameObjectPlayableEventLocalPlayer::CreateSkill()
{
	return MakeShareable(new FGsSkillHandlerPlayableEventLocalPlayer());
}


TSharedPtr<FGsMovementHandlerBase> UGsGameObjectPlayableEventLocalPlayer::CreateMovement()
{
	return MakeShareable(new FGsMovementHandlerPlayableEventLocalPlayer());
}

void UGsGameObjectPlayableEventLocalPlayer::Finalize()
{

	Super::Finalize();
}

void UGsGameObjectPlayableEventLocalPlayer::Update(float Delta)
{
	Super::Update(Delta);

}

void UGsGameObjectPlayableEventLocalPlayer::ApplyMoveSpeedFromStatInfo()
{
	FGsMovementHandlerBase* move = GetMovementHandler();
	if (nullptr == move) return;

	const FGsSchemaCreatureCommonInfo* commoninfo = GetCommonInfoTable();
	check(commoninfo);

	move->SetSpeedMovementComponent(commoninfo->moveSpeed);
}
int UGsGameObjectPlayableEventLocalPlayer::UpdateCombatStatus(bool InImmediately)
{	
	int result = UGsGameObjectPlayer::UpdateCombatStatus(InImmediately);

	// 디졸브 효과로 인해 xray vision효과가 반영되는 상황이므로 잠시 비활성
	// 추후 다른곳에서도 Local Player의 디졸브 효과 처리가 들어갈경우 
	// UGsMeshShaderControlComponent 클래스 확장을 통하여 적용할것을 고려
	if (false == InImmediately && false == IsBattleMode() && false == GetCharacter()->IsOverlayMaterialChanged() && result != 0)
	{
		// 무기 메시만 반영
		if (AGsWeaponActor* weaponR = _actor->GetWeapon())
		{
			_actor->SetupPrimitives(weaponR, false);
		}
		if (AGsWeaponActor* weaponL = _actor->GetWeaponSub())
		{
			_actor->SetupPrimitives(weaponL, false);
		}
	}

	SetFairyVehicleCondition(EGsFairyVehicleCondition::USER_STATE_NORMAL, IsBattleMode());

	GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_USER_STATUS, nullptr);
	return result;
}

bool UGsGameObjectPlayableEventLocalPlayer::CanTarget(UGsGameObjectBase* Target)
{
	// 무조건 가능하게 오버라이드
	return true;
}

// server option -> GSAI()
// crash issue
bool UGsGameObjectPlayableEventLocalPlayer::OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker,
	uint32 StriketSkillId, uint32 StrikerNotifyId, HitResultType ResultType, DamageByType DamageType, 
	DamageStatType DamageStat, HealByType HealType, int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage)
{
	UGsGameObjectPlayer::OnHit(Striker, SubStriker, StriketSkillId, StrikerNotifyId, ResultType,
		DamageType, DamageStat, HealType, Damage, PassivityOwner, IsNoDamage);

	return true;
}