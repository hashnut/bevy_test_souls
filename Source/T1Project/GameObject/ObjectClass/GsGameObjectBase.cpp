// Fill out your copyright notice in the Description page of Project Settings.

#include "GsGameObjectBase.h"

#include "TimerManager.h"
#include "Components/WidgetComponent.h"

#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsEffectComponent.h"

#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Parts/GsPartsHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/Damage/GsDamageHandlerBase.h"
#include "GameObject/Data/GsGameObjectData.h"

#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "Quest/Management/GsQuestActionManagement.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsPartyManager.h"

#include "UI/UILib/Base/GsUIBillboard.h"

#include "T1Project.h"

FGsTargetHandlerBase*		UGsGameObjectBase::GetTargetHandler()		{ return GetGameObjectHandler<FGsTargetHandlerBase>(GameObjectHandlerType::TARGET); }
FGsSkillHandlerBase*		UGsGameObjectBase::GetSkillHandler()		{ return GetGameObjectHandler<FGsSkillHandlerBase>(GameObjectHandlerType::SKILL); }
FGsPartsHandlerBase*		UGsGameObjectBase::GetPartsHandler()		{ return GetGameObjectHandler<FGsPartsHandlerBase>(GameObjectHandlerType::PARTS); }
FGsMovementHandlerBase*		UGsGameObjectBase::GetMovementHandler()		{ return GetGameObjectHandler<FGsMovementHandlerBase>(GameObjectHandlerType::MOVEMENT); }
FGsCustomizeHandler*		UGsGameObjectBase::GetCustomizeHandler()	{ return GetGameObjectHandler<FGsCustomizeHandler>(GameObjectHandlerType::CUSTOMIZE); }
FGsAbnormalityHandlerBase*	UGsGameObjectBase::GetAbnormalityHandler() 	{ return GetGameObjectHandler<FGsAbnormalityHandlerBase>(GameObjectHandlerType::ABNORMALITY); }
FGsDamageHandlerBase*		UGsGameObjectBase::GetDamageHandler()		{ return GetGameObjectHandler<FGsDamageHandlerBase>(GameObjectHandlerType::DAMAGE); }

bool UGsGameObjectBase::IsVaildClass(UGsGameObjectBase* GameObjectClass)
{
	if (GameObjectClass && GameObjectClass->IsVaildClass())
	{
		return true;
	}
	return false;
}

bool UGsGameObjectBase::IsVaildClass() const
{
	if (false == _isSubClassCreate || false == IsValidLowLevel())
	{
		return false;
	}

	AActor* actor = GetActor();
	if (nullptr == actor)
	{
		return false;
	}
	return true;
}

FVector UGsGameObjectBase::GetLocation() const
{ 
	return (GetActor()) ? GetActor()->GetActorLocation() : _ActorTM.Location;
}

FVector2D UGsGameObjectBase::GetLocation2D() const
{
	return (GetActor()) ? FVector2D(GetActor()->GetActorLocation()) : FVector2D(_ActorTM.Location);
}

FRotator UGsGameObjectBase::GetRotation() const
{
	return (GetActor()) ? GetActor()->GetActorRotation() : _ActorTM.Rotation;
}

FVector UGsGameObjectBase::GetScale() const
{
	return (GetActor()) ? GetActor()->GetActorScale() : _ActorTM.Scale;
}

FVector UGsGameObjectBase::GetLootTargetPoint(UGsGameObjectBase* LootTarget) const
{
	return GetLocation();
}

float UGsGameObjectBase::GetTargetDistSquared(const UGsGameObjectBase* TargetObj) const
{
	return (TargetObj) ? (TargetObj->GetLocation2D() - GetLocation2D()).SizeSquared() : 0.f;
}

bool UGsGameObjectBase::IsUseAutoIdleEmotion()
{
	if (auto skillHandler = GetSkillHandler())
	{
		return skillHandler->GetIdleEmotionRunner().IsValid();
	}
	return false;
}

void UGsGameObjectBase::SetAutoIdleEmotionEnable(bool set)
{
	if (auto skillHandler = GetSkillHandler())
	{
		return skillHandler->SetAutoIdleEmotionEnable(set);
	}
}

int64 UGsGameObjectBase::GetGameId() const
{
	return GetData() == nullptr ? 0 : GetData()->GetGameId();
}

UGsGameObjectBase::UGsGameObjectBase()
{
}
UGsGameObjectBase::~UGsGameObjectBase()
{
}

void UGsGameObjectBase::BeginDestroy()
{
	Super::BeginDestroy();
}

FString UGsGameObjectBase::GetObjectTypeName(EGsGameObjectType Type)
{
	switch (Type)
	{
	case EGsGameObjectType::Base:
		return TEXT("Base");
	case EGsGameObjectType::PropObject:
		return TEXT("PropObject");	
	case EGsGameObjectType::Projectile:
		return TEXT("Projectile");
	case EGsGameObjectType::Vehicle:
		return TEXT("Vehicle");
	case EGsGameObjectType::Creature:
		return TEXT("Creature");
	case EGsGameObjectType::Warp:
		return TEXT("Warp");
	case EGsGameObjectType::Player:
		return TEXT("Player");
	case EGsGameObjectType::NonPlayer:
		return TEXT("NonPlayer");
	case EGsGameObjectType::RemotePlayer:
		return TEXT("RemotePlayer");
	case EGsGameObjectType::LocalPlayer:
		return TEXT("LocalPlayer");
	case EGsGameObjectType::DropObject:
		return TEXT("DropObject");
	case EGsGameObjectType::PropInteraction:
		return TEXT("PropInteraction");
	case EGsGameObjectType::Sandbag:
		return TEXT("Sandbag");
	case EGsGameObjectType::PropInteractInvasion:
		return TEXT("PropInteractInvasion");
	default:
		return TEXT("Unknown Object");
	}
}

void UGsGameObjectBase::Initialize(const FGsSpawnParam* SpawnParam)
{
	// 자기 자신의 함수호출을 명시적으로 해주어야함 UGsGameObjectBase::GetObjectType()
	// 그렇지 않으면 가상 함수 오버라이딩 호출로 의도치 않은 값을 리턴한다.
    SET_FLAG_TYPE(_objectType, UGsGameObjectBase::GetObjectType());
	
	_messageParam._paramOwner = this;

	_gameObjectData = CreateData(SpawnParam);
	if (_gameObjectData)
	{
		_gameObjectData->Initialize();
	}
	else
	{
		GSLOG(Error, TEXT("Failed! Create GameObjectData"));
	}
}

void UGsGameObjectBase::Update(float Delta)
{
	_gameObjectMessage.Update();

	for (TWeakPtr<IGsGameObjectHandler> handler : _handlerHolder)
	{
		if (handler.IsValid() && false == handler.Pin()->IsPrivateUpdate())
		{
			handler.Pin()->Update(Delta);
		}
	}
}

void UGsGameObjectBase::Finalize()
{
	ActorDespawn();

	if (_gameObjectData)
	{
		_gameObjectData->OnDelegate().RemoveAll(this);
		_gameObjectData->Finalize();
		delete _gameObjectData;
		_gameObjectData = nullptr;
	}
}

void UGsGameObjectBase::BindMessage()
{
	MGameObject& objMessage = GMessage()->GetGameObject();
	// 메세지 Delegate 관리 활성
	InitializeMessageDelegateList(&objMessage);

	// GameObjectData 맴버 변경 델리게이트 바인딩
	if (_gameObjectData)
	{
		_gameObjectData->OnDelegate().RemoveAll(this);
		_gameObjectData->OnDelegate().Add(
			FGsGameObjectData::FGsGameObjectDataDelegate::FDelegate::CreateUObject(this,
				&UGsGameObjectBase::OnGameObjectDataUpdate));
	}	
}

void UGsGameObjectBase::UnBindMessage()
{
	_gameObjectData->OnDelegate().RemoveAll(this);
	FinalizeMessageDelegateList();
}

FGsGameObjectData* UGsGameObjectBase::CreateData(const FGsSpawnParam* SpawnParam)
{
	return new FGsGameObjectData(SpawnParam);
}

#ifdef USE_GAMEOBJECT_DISPLAY_DATA
FGsDisplayData* UGsGameObjectBase::CreateDisplayData()
{
	return new FGsDisplayData();
}
#endif

void UGsGameObjectBase::CreateSubClass()
{
	_isSubClassCreate = true;
	// SetLockOnActive(false);	
}

void UGsGameObjectBase::LateCreateSubClass()
{
	ExecuteLateEvent();
}

void UGsGameObjectBase::AddLateCreateSubClassEvent(TFunction<void()> Command)
{
	_queueLateInitializeFunc.Enqueue(Command);
}

void UGsGameObjectBase::ExecuteLateEvent()
{
	if (false == _queueLateInitializeFunc.IsEmpty())
	{
		TFunction<void()> command;
		_queueLateInitializeFunc.Dequeue(command);
		command();

		if (UWorld* world = GetWorld())
		{
			FTimerManager& timerManager = world->GetTimerManager();
			_lateInitializeTimer = timerManager.SetTimerForNextTick(this, &UGsGameObjectBase::ExecuteLateEvent);
		}
		else
		{
			ExecuteLateEvent();
		}
	}
}

void UGsGameObjectBase::InitializeActor(class AActor* Owner)
{
	for (TSharedPtr<IGsGameObjectHandler> handler : _handlerHolder)
	{
		if (nullptr != handler)
		{
			handler->InitializeActor(Owner);
		}
	}	
}

void UGsGameObjectBase::RemoveSubClass()
{
	RemoveAllHandler();

	_isSubClassCreate = false;		
}

void UGsGameObjectBase::AppendInitialize(TSharedPtr<IGsGameObjectHandler> inHandler)
{
	if (inHandler)
	{
		inHandler->Initialize(this);
		_handlerHolder.Add(inHandler);
	}
}

void UGsGameObjectBase::RemoveAllHandler()
{
	for (TSharedPtr<IGsGameObjectHandler> handler : _handlerHolder)
	{
		check(handler)
		handler->Finalize();
	}
	_handlerHolder.Reset();
}

void UGsGameObjectBase::ActorSpawned(AActor* Spawn)
{
	if (Spawn)
	{
		InitializeActor(Spawn);

		SetGameObjectActorVisible(_ActorVisible);

		//GSLOG(Warning, TEXT("%s Actor Spawn Complete"), *Spawn->GetName());

		// LateLateCreateSubClass 등록
		if (UWorld* world = GetWorld())
		{
			FTimerManager& timerManager = world->GetTimerManager();
			_lateInitializeTimer = timerManager.SetTimerForNextTick(this, &UGsGameObjectBase::LateCreateSubClass);
		}
		//// 최하위 종속 클래스에서 Sub제어기 클래스들을 타입에 맞게 생성
		//CreateSubClass();		
		
		// 메세지 바인딩
		BindMessage();

		GMessage()->GetGameObject().SendMessage(MessageGameObject::COMMON_SPAWN, &_messageParam);

		// 파티 공유 타겟이면 타겟 표시
		// https://jira.com2us.com/jira/browse/C2URWQ-1795
		FGsPartyManager* partyManager = GSParty();
		if(partyManager != nullptr)
		{ 
			int64 sharedTargetId = GSParty()->GetSharedPartyTargetId();
			int64 objGameId = GetGameId();

			if (sharedTargetId == objGameId)
			{
				SetPartySharedTargetActive(true);
			}
		}
	}
}

void UGsGameObjectBase::ActorDespawn()
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::COMMON_DESPAWN, &_messageParam);

	UnBindMessage();

	// 액터 포인터가 삭제되기 전에 SubClass 정리
	RemoveSubClass();
}

void UGsGameObjectBase::OnDie()
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::COMMON_DIE, &_messageParam);
	ClearPendingKill();

	// https://jira.com2us.com/jira/browse/C2URWQ-3547
	// party target clear
	FGsPartyManager* partyManager = GSParty();
	if(partyManager != nullptr)
	{ 
		int64 sharedTargetId = GSParty()->GetSharedPartyTargetId();
		int64 objGameId = GetGameId();

		if (sharedTargetId == objGameId)
		{
			SetPartySharedTargetActive(false);
		}
	}
}

void UGsGameObjectBase::OnRevive()
{
	ClearPendingKill();
}

bool UGsGameObjectBase::OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId,
	uint32 StrikerNotifyId, HitResultType ResultType, DamageByType DamageType, DamageStatType DamageStat,
	HealByType HealType, int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage)
{
	// 현재는 사용하지 않는 메세지라 주석처리
	//GMessage()->GetGameObject().SendMessage(MessageGameObject::COMMON_HIT, &_messageParam);
	return true;
}

void UGsGameObjectBase::OnRunAway()
{
	GMessage()->GetGameObject().SendMessage(MessageGameObject::COMMON_DIE, &_messageParam);
}

void UGsGameObjectBase::OnGameObjectDataUpdate(EGsGameObjectDataMember MemberType)
{
}

void UGsGameObjectBase::NetDeathAck(PD::SC::PKT_SC_CREATURE_DEATH_READ* Packet)
{
	if (Packet->Reason() == SPAWN_REASON::RUNAWAY)
	{
		OnRunAway();
	}
	else
	{
		if (FGsDamageHandlerBase* damageHandler = GetDamageHandler())
		{
			// 대미지 딜레이 정보와 현재 Hp검사등으로 죽음 지연처리를 결정한다.
			if (false == damageHandler->IsEmpty() /*&& false == IsZeroHP()*/)
			{
				_isPendingKill = true;
				return;
			}
		}
		OnDie();
	}
}

void UGsGameObjectBase::LookAtTarget(const UGsGameObjectBase* LookTarget, bool IsSmoothRotation)
{
	if (LookTarget)
	{
		LookAtPosition(LookTarget->GetLocation(), IsSmoothRotation);
	}
}

void UGsGameObjectBase::LookAtPosition(const FVector& WorldPos, bool IsSmoothRotation)
{
	FVector lookVec = WorldPos - GetLocation();
	// 라돈 같이 키큰녀석들은 z값이 높아서 위로 들림
	// z값 0로 세팅
	lookVec.Z = 0;
	lookVec.Normalize();

	LookAt(lookVec.Rotation(), IsSmoothRotation);
}

void UGsGameObjectBase::LookAt(const FRotator& LookRotator, bool IsSmoothRotation)
{
	if (AActor* actor = GetActor())
	{
		if (IsSmoothRotation)
		{
			// 현재 GsCharacterBase 클래스 타입만 기능 제공
			if (AGsCharacterBase* characterActor = GetCharacter())
			{
				characterActor->SetSmoothRotaion(LookRotator);
			}
			else
			{
				actor->SetActorRotation(LookRotator);
			}
		}
		else
		{
			actor->SetActorRotation(LookRotator);
		}
	}
}

bool UGsGameObjectBase::IsZeroHP() const
{
	return false;
}

bool UGsGameObjectBase::IsAble()
{
	return false == GetIsInvincible();
}

bool UGsGameObjectBase::IsSkipTerrainCheck() const
{
	return false;
}

bool UGsGameObjectBase::IsPendingKill()
{
	return _isPendingKill;
}


void UGsGameObjectBase::ClearPendingKill()
{
	_isPendingKill = false;
}

// visible 바로 세팅(대사 연출 관련 처리)
void UGsGameObjectBase::SetGameObjectActorVisible(bool In_isVisible)
{
	_ActorVisible = In_isVisible;

	if (AActor* actor = GetActor())
	{
		actor->SetActorHiddenInGame(!In_isVisible);
	}
}
// represent abnormality effect visible(dialog)
void UGsGameObjectBase::SetRepresentAbnormalityEffectVisible(bool In_isVisible)
{
	if (AGsCharacterBase* actorCharacter = GetCharacter())
	{
		if (UGsEffectComponent* effComp = actorCharacter->GetEffectComponent())
		{
			effComp->SetRepresentAbnormalityEffectVisible(In_isVisible);
		}
	}
}
bool UGsGameObjectBase::IsHidden()
{
	bool result = false;
	if (UGsQuestManager* questManager = GSQuest())
	{
		if (FGsQuestActionManagement* action = questManager->GetQuestActionManagement())
		{
			result = action->IsActioning();
		}
	}

	auto actor = GetActor();
	return !result ? actor && actor->IsHidden() : result;
}

// 락온 액티브
// void UGsGameObjectBase::SetLockOnActive(bool In_isActive)
// {
// 	AGsCharacterBase* character = GetCharacter();
// 	if (nullptr == character)
// 	{
// 		return;
// 	}
// 	UWidgetComponent* lockOnWidget = character->GetLockOnWidgetComponent();
// 	if (lockOnWidget == nullptr)
// 	{
// 		return;
// 	}
// 
// 	lockOnWidget->SetActive(In_isActive);
// 	lockOnWidget->SetVisibility(In_isActive);
// 
// 	if (UGsUIBillboard* billboard = character->GetLockOnWidget())
// 	{
// 		ESlateVisibility visibility = (In_isActive == true) ?
// 			ESlateVisibility::Visible : ESlateVisibility::Hidden;
// 		billboard->SetVisibility(visibility);
// 	}
// 
// }

void UGsGameObjectBase::SetPartySharedTargetActive(bool In_isActive)
{
	AGsCharacterBase* character = GetCharacter();
 	if (nullptr == character)
 	{
 		return;
 	}
 	UWidgetComponent* widget = character->GetPartySharedTargetWidgetComponent();
 	if (widget == nullptr)
 	{
 		return;
 	}
 
	widget->SetActive(In_isActive);
	widget->SetVisibility(In_isActive);
 
 	if (UGsUIBillboard* billboard = character->GetPartySharedTargetWidget())
 	{
 		ESlateVisibility visibility = (In_isActive == true) ?
 			ESlateVisibility::Visible : ESlateVisibility::Hidden;
 		billboard->SetVisibility(visibility);
 	}
}

void UGsGameObjectBase::SetActorLocation(const FVector& loc)
{
	_ActorTM.Location = loc;
}

void UGsGameObjectBase::SetActorRotation(const FRotator& rot)
{
	_ActorTM.Rotation = rot;
}

const FVector& UGsGameObjectBase::GetActorLocation() const
{
	return _ActorTM.Location;
}

const FRotator& UGsGameObjectBase::GetActorRotation() const
{
	return _ActorTM.Rotation;
}
