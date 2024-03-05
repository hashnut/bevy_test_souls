// Fill out your copyright notice in the Description page of Project Settings.


#include "GsGameObjectPropInteraction.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

#include "ActorEx/GsActorDropObject.h"
#include "ActorEx/GsActorPropObject.h"

#include "Item/GsSchemaItemCommon.h"
#include "Item/GsItem.h"
#include "UTIL/GsTableUtil.h"
#include "UTIL/GsItemUtil.h"
#include "GsTableManager.h"
#include "WidgetComponent.h"

#include "UI/UIContent/Billboard/GsUIBillboardDropObject.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Billboard/GsUIBillboardInteractionButton.h"

#include "GameFramework/Actor.h"

#include "GameObject/DropObject/GsSchemaDropObject.h"
#include "GameObject/DropObject/GsSchemaDropObjectEffect.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/State/Prop/GsStatePropIdle.h"
#include "GameObject/State/GsStateBase.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Interaction/GsActorInteractionInterface.h"
#include "GameObject/Define/GsGameObjectText.h"
#include "GameObject/Quest/GsQuestHandler.h"

#include "Components/SphereComponent.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsQuestManager.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsResourceManager.h"
#include "DataSchema/GameObject/Prop/GsSchemaPropData.h"

#include "Quest/GsActorQuestTargetInterface.h"
#include "Quest/Management/GsQuestTargetManagement.h"
#include "Runtime/DataCenter/Public/Quest/GsQuestEnum.h"

#include "Message/MessageParam/GsAIMessageParam.h"

#include "Animation/GsPropAnimInstance.h"
#include "UTIL/GsTimeUtil.h"
#include "ActorEx/GsActorPropTouch.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"

#include "ActorComponentEx/GsEffectComponent.h"
#include "ActorEx/GsCharacterBase.h"
#include "ActorEx/GsActorInvasionPropTouch.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"

#define PROP_ACTION_TEST false

const FName PROP_LOCOMOTION = TEXT("Locomotion");
const FName PROP_ACTION = TEXT("Action");

UGsGameObjectPropInteraction::UGsGameObjectPropInteraction()
{

}

UGsGameObjectPropInteraction::~UGsGameObjectPropInteraction()
{

}

void UGsGameObjectPropInteraction::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectPropInteraction::GetObjectType());

	const FGsSpawnParamProp* castParam = static_cast<const FGsSpawnParamProp*>(SpawnParam);
	_propData = castParam->GetPropData();
	_canInteraction = false;

	_newInteractionButtonSize = 0;
	_oldInteractionButtonSize = 0;
}

void UGsGameObjectPropInteraction::Update(float inDelta)
{
	Super::Update(inDelta);

	if (_fsm)
	{
		_fsm->Update(this, inDelta);
	}

	if(_actionTimer)
	{
		_actionTimer->Tick(inDelta);
	}
}

void UGsGameObjectPropInteraction::Finalize()
{	
	if(_actionTimer)
	{
		_actionTimer->Clear();
	}
	
	_actionTimer = nullptr;
	_camera = nullptr;

	Super::Finalize();
}


void UGsGameObjectPropInteraction::InitializeActor(AActor* inOwner)
{
	if (nullptr == inOwner)
	{
		return;
	}	

	Super::InitializeActor(inOwner);
			
	InitActionTimer();

	InvalidWidget();

	/*if (AGsActorPropTouch* propActor = Cast<AGsActorPropTouch>(_actor))
	{
		if (propActor->GetAnim())
		{
			propActor->GetAnim()->ChangeState(EGsStateProp::Idle);
		}
	}*/
}

void UGsGameObjectPropInteraction::ActorDespawn()
{
	// 삭제됨을 알림
	FGsGameObjectMessageParam messageParam(this);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::PROP_DESPAWN, &messageParam);

	Super::ActorDespawn();
}

//Initialize actor 다음에 호출된다. 
void UGsGameObjectPropInteraction::CreateSubClass()
{
	// Fsm Initialize는 각 클래스에서 처리
	if (nullptr == _fsm)
	{
		_fsm = CreateBaseFSM();
	}

	// Fsm Initialize는 각 클래스에서 처리
	Super::CreateSubClass();
}

void UGsGameObjectPropInteraction::InitializeFSM()
{
	if (_fsm)
	{
		_fsm->Initialize(BPStateMachineName, this, FGsStatePropIdle::GetInstance());
	}
}

void UGsGameObjectPropInteraction::RemoveSubClass()
{
	Super::RemoveSubClass();

	if (_fsm)
	{
		_fsm->Finalize();
		delete _fsm;
		_fsm = nullptr;
	}

	if(_propData)
	{
		_propData = nullptr;
	}
}

EGsGameObjectType UGsGameObjectPropInteraction::GetObjectType() const
{
	return EGsGameObjectType::PropInteraction;
}

void UGsGameObjectPropInteraction::SetInteractionActive(bool In_isActive, int In_Index)
{

}

//퀘스트 타겟 마크와 퀘스트 인터렉션 위젯 활성화 설정 하는 함수
void UGsGameObjectPropInteraction::SetQuestTargetActive(bool inIsActive, int In_Index)
{	
	bool isActive = _canInteraction
		&& inIsActive
		&& (false == _isInteracting);

	SetQuestMarkActive(false);
	SetInteractionActive(isActive);
}

bool UGsGameObjectPropInteraction::GetQuestTargetActive() const
{
	return (_isQuestTargetActive &&
		_canInteraction &&
		_fsm && _fsm->IsState((uint8)EGsStateProp::Idle));
}

void UGsGameObjectPropInteraction::SetQuestMarkActive(bool In_isActive, int In_typeIndex)
{
	//Spawn 상태일 수 있으므로 상태는 저장한다
	_isQuestTargetActive = In_isActive;

	if (nullptr == _actor ||
		_actor->IsPendingKill() == true)
	{
		return;
	}

	IGsActorQuestTargetInterface* interaction = Cast<IGsActorQuestTargetInterface>(_actor);
	if (nullptr == interaction)
	{
		return;
	}

	UStaticMeshComponent* meshComponent = interaction->GetQuestTargetMark();
	if (meshComponent == nullptr)
	{
		return;
	}
	
	if (IsActiveQuestTargetMark())
	{		
#if PROP_INTERACTION_DEBUG
		GSLOG(Log, TEXT("Show Prop quest mark - id : %lld"), GetGameId());
#endif
		meshComponent->SetVisibility(true);			
	}
	else
	{
#if PROP_INTERACTION_DEBUG
		GSLOG(Log, TEXT("Hide Prop quest mark - id : %lld"), GetGameId());
#endif

		meshComponent->SetVisibility(false);
	}
}

bool UGsGameObjectPropInteraction::IsActiveQuestTargetMark()
{
#if PROP_INTERACTION_DEBUG
	if (false == _isQuestTargetActive)
	{
		GSLOG(Log, TEXT("prop - IsActiveQuestTargetMark - isQuestTargetActive is false - id : %lld"), GetGameId());
		return false;
	}

	if (false == _canInteraction)
	{
		GSLOG(Log, TEXT("prop - IsActiveQuestTargetMark - _canInteraction is false - id : %lld"), GetGameId());
		return false;
	}

	if (false == (_fsm && _fsm->IsState((uint8)EGsStateProp::Idle)))
	{
		GSLOG(Log, TEXT("prop - IsActiveQuestTargetMark - fsm is not idle - id : %lld"), GetGameId());
		return false;
	}

	return true;
#endif

	return _isQuestTargetActive &&
		_canInteraction &&
		_fsm && _fsm->IsState((uint8)EGsStateProp::Idle);
}

void UGsGameObjectPropInteraction::SetCanInteraction(bool inInteraction)
{
	_canInteraction = inInteraction;

#if PROP_INTERACTION_DEBUG && false
	//더 딥하게 볼때 로그 사용 
	if (false == _canInteraction)
	{
		GSLOG(Log, TEXT("prop debug - _canInteraction is false - id : %lld"), GetGameId());
		FDebug::DumpStackTraceToLog(ELogVerbosity::Error);
	}	
	else
	{
		GSLOG(Log, TEXT("prop debug - _canInteraction is true - id : %lld"), GetGameId());
		FDebug::DumpStackTraceToLog(ELogVerbosity::Error);
	}
#endif
}

bool UGsGameObjectPropInteraction::IsQuestTarget()
{
	if (GetPropTableData())
	{
		int tblId = GetPropTableData()->id;

		FGsQuestTargetManagement* questTargetManagement = GSQuest()->GetQuestTargetManagement();
		if (questTargetManagement == nullptr)
		{
			return false;
		}

		return questTargetManagement->IsQuestProp(tblId);
	}

	return false;
}


void UGsGameObjectPropInteraction::OnDie()
{
	Super::OnDie();

	if (_fsm)
	{
		_fsm->ProcessEvent((uint8)EGsStateProp::Finish);
	}

	SetQuestMarkActive(false);
}

void UGsGameObjectPropInteraction::UpdateInteractionColliderSize(float In_rate)
{

}

void UGsGameObjectPropInteraction::DrawDebugInteractionRange()
{

}

void UGsGameObjectPropInteraction::OnClickInteract(bool In_isByWidget)
{

}

FGsGameObjectStateManager* UGsGameObjectPropInteraction::CreateBaseFSM()
{
	return new FGsGameObjectStateManager();
}

bool UGsGameObjectPropInteraction::IsFinish(int32 inInteractionEndUserCount)
{
	if (_propData)
	{
		return UGsGameObjectPropInteraction::IsFinish(_propData->multipleInteractionCountMax, inInteractionEndUserCount);
	}

	return true;
}

bool UGsGameObjectPropInteraction::IsFinish(int32 inMultipleInteractionCountMax, int32 inInteractionEndUserCount)
{
	//inMultipleInteractionCountMax 이 0이면 무한으로 클릭할 수 있다는 의미이다. 따라서 Finish 상태에 도달할 수 없다. 
	if (inMultipleInteractionCountMax == 0)
	{
		return false;		 
	}

	return (inInteractionEndUserCount >= inMultipleInteractionCountMax);
}

bool UGsGameObjectPropInteraction::IsFinish()
{
	return IsFinish(_interactionEndUserCount);
}

bool UGsGameObjectPropInteraction::CanInteraction(int32 inMultipleInteractionCountMax, int32 inMultipleInteractionUserCount, int32 inInteractingUserCount, int32 inInteractionFinishUserCount)
{
	//inInteractingUserCount 현재 인터렉션 하는 유저 수 
	//inInteractionFinishUserCount 인터렉션이 끝난 유저 수 
	int32 totalInteractionUserCount = inInteractingUserCount + inInteractionFinishUserCount;

	//인터렉션 횟수가 무한인가?
	if (inMultipleInteractionCountMax == 0)
	{
		//한번에 터치 가능한 수가 제한이 없는가?
		if (inMultipleInteractionUserCount == 0)
		{
			return true;
		}
		else
		{
			return inInteractingUserCount < inMultipleInteractionUserCount;
		}		
	}
	
	//한번에 인터렉션 가능한 유저 수인가?
	//현재 인터렉션한 사람이 최대 인터렉션 가능한 사람 이하인가?
	return ((inInteractingUserCount < inMultipleInteractionUserCount) && (totalInteractionUserCount < inMultipleInteractionCountMax));
}

bool UGsGameObjectPropInteraction::CanInteraction(int32 inInteractionStartUserCount, int32 inInteractionEndUserCount)
{
	if (_propData)
	{
		return UGsGameObjectPropInteraction::CanInteraction(_propData->multipleInteractionCountMax, _propData->multipleInteractionUserCount, inInteractionStartUserCount, inInteractionEndUserCount);
	}

	return false;
}


void UGsGameObjectPropInteraction::InvalidWidget()
{
	//콜리전 킬 수 있는 여부를 먼저 업데이트 하고 퀘스트 마크를 킬지 말지 결정한다. 
	bool isFinish = IsFinish(_interactionEndUserCount);
	bool canInteraction = false;
	if (false == isFinish)
	{
		canInteraction = CanInteraction(_interactionStartUserCount, _interactionEndUserCount);

#if PROP_INTERACTION_DEBUG && false
		if (false == canInteraction)
		{
			GSLOG(Log, TEXT("prop debug - InvalidWidget - can interaciotn fail - id : %lld\tinteraciton start count : %d\tinteraciotn end count : %d"), GetGameId(), _interactionStartUserCount, _interactionEndUserCount);
		}		
#endif
	}
	else
	{
#if PROP_INTERACTION_DEBUG && false
		GSLOG(Log, TEXT("prop debug - InvalidWidget - prop interactio is finish - id : %lld\tmulti interaction count : %d\tcurrent interaction count : %d"), GetGameId(), _propData->multipleInteractionCountMax, _interactionEndUserCount);
#endif
	}

	SetCanInteraction(canInteraction);
	
	bool isActive = (false == _isInteracting) && canInteraction;
	SetQuestTargetActive(isActive);
}

void UGsGameObjectPropInteraction::HideWidget()
{
	SetQuestTargetActive(false);
}

void UGsGameObjectPropInteraction::InitActionTimer()
{
	_actionTimer = NewObject<UGsSimpleTimer>();
	_actionTimer->_finishDelegate.AddDynamic(this, &UGsGameObjectPropInteraction::OnActionFinish);

	if (nullptr == GetActor())
	{
		return;
	}

	AGsActorPropTouch* propActor = Cast<AGsActorPropTouch>(GetActor());
	if (nullptr == propActor)
	{
		return;
	}

	if (propActor->GetAnim())
	{
		float animLength = propActor->GetAnim()->GetStateAnimationLength(PROP_LOCOMOTION, PROP_ACTION) * 0.8f;

#if PROP_ACTION_TEST 
		GSLOG(Log, TEXT("UGsGameObjectPropInteraction - StartActionTimer - animlength : %f"), animLength);
#endif

		if (_actionTimer)
		{
			_actionTimer->SetMaxTime(animLength);
		}
	}
	else
	{
		if (_actionTimer)
		{
			_actionTimer->SetMaxTime(0);
		}
	}
}

void UGsGameObjectPropInteraction::StartActionTimer()
{	
	if (_actionTimer)
	{
		_actionTimer->Reset();
		_actionTimer->Start();
	}
}

void UGsGameObjectPropInteraction::OnActionFinish(const UObject* inTimer)
{
	FGsGameObjectStateManager* fsm = GetBaseFSM();	
	if (nullptr == fsm)
	{
		return;
	}

	if (AGsActorPropTouch* propActor = Cast<AGsActorPropTouch>(GetActor()))
	{		
		if (IsFinish())
		{
#if PROP_ACTION_TEST
			GSLOG(Log, TEXT("UGsGameObjectPropInteraction - OnActionFinish - Finish - name : %s"), *propActor->GetActorLabel());
#endif
			fsm->ProcessEvent((uint8)EGsStateProp::Finish);
		}
		else
		{
#if PROP_ACTION_TEST
			GSLOG(Log, TEXT("UGsGameObjectPropInteraction - OnActionFinish - Idle - name : %s"), *propActor->GetActorLabel());
#endif
			fsm->ProcessEvent((uint8)EGsStateProp::Idle);
		}
	}

	if(_actionTimer)
	{
		_actionTimer->Stop();
	}

	FGsAIReserveParam aiReserveClearParam(EGsAIActionType::INTERACTION, this);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_JOB, &aiReserveClearParam);
}

void UGsGameObjectPropInteraction::SetPropSpawnState(PD::SC::PKT_SC_SPAWN_PROP_READ* inPkt)
{
	SPAWN_REASON reason = inPkt->Reason();
	//현재 인터랙션 하는 유저수
	_interactionStartUserCount = inPkt->InteractionStartUserCount();
	//인터랙션이 끝난 유저 수
	_interactionEndUserCount = inPkt->InteractionEndUserCount();

	//인터렉션을 다하여 Prop이 완전히 죽은 상태인지 결정하는 여부
	bool isFinish = IsFinish(_interactionEndUserCount);
	//인터렉션 가능한 여부

	if (nullptr == _fsm)
	{
		return;
	}

	if (isFinish)
	{		
		_fsm->ProcessEvent((uint8)EGsStateProp::Finish);	
	}
	else
	{		
		//Quest 마크는 Spawn시 꺼져있는 상태이다.
		//Spawn
		if (reason == SPAWN_REASON::NORMAL)
		{
			//Quest 인지 체크 후 Quest 마크 활성화
			StartFadeIn();
		}		
		else
		{			
			_fsm->ProcessEvent((uint8)EGsStateProp::Idle);
		}
	}

	UpdatePropState();
}

void UGsGameObjectPropInteraction::UpdatePropState(PD::SC::PKT_SC_UPDATE_PROP_READ* inPkt)
{
	//현재 동시에 인터렉션 하고 있는 유저 수 
	int32 interactionStartUserCount = inPkt->InteractionStartUserCount();
	//이미 인터렉션한 유저 수
	int32 interactionEndUserCount = inPkt->InteractionEndUserCount();	

	bool isPlayerCast = false;
	UGsGameObjectBase* player = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);			
	if (player)
	{
		isPlayerCast = (inPkt->CasterGameId() == player->GetGameId());
	}	

	//Player에 의한 finsih
	if (isPlayerCast)
	{	
		if (interactionEndUserCount != _interactionEndUserCount)
		{
			if (_fsm)
			{
				//타유저가 미세한 차이로 먼저 인터렉션 한 경우 Action 상태로 갈 수 있다.
				//이미 Action 상태로 들어간 경우 Action 상태의 End에서 Finish를 판별해야한다. 
				if (_fsm->IsState((uint8)EGsStateProp::Idle))
				{

#if PROP_INTERACTION_DEBUG
	GSLOG(Log, TEXT("prop debug - prop update - id : %lld\tcurrent : %d\tmax : %d"), inPkt->CasterGameId(), _interactionStartUserCount, _interactionEndUserCount);
#endif

					_fsm->ProcessEvent((uint8)EGsStateProp::Action);
				}				
			}			
		}

		_interactionStartUserCount = interactionStartUserCount;
		_interactionEndUserCount = interactionEndUserCount;		
	}
	else
	{
		_interactionStartUserCount = interactionStartUserCount;
		_interactionEndUserCount = interactionEndUserCount;		

		//플레이어가 미세한 차이로 먼저 인터렉션 한 경우 Action 상태로 갈 수 있다. 
		//플레이어가 Action 상태에서 온 후 Finish가 왔다면 플레이어의 Action을 진행 한 뒤 Action 상태의 End에서 Finish를 판별한다. 
		if (IsFinish(_interactionEndUserCount))
		{		
			if (_fsm->IsState((uint8)EGsStateProp::Idle))
			{
				_fsm->ProcessEvent((uint8)EGsStateProp::Action);
			}
		}
	}

	if (_fsm->IsState((uint8)EGsStateProp::Idle))
	{
		InvalidWidget();
	}

	UpdatePropState();
}

void UGsGameObjectPropInteraction::PlayActionFinishEffect()
{
	if (nullptr == _propData)
	{
		return;
	}

	const FSoftObjectPath& softPath = _propData->actionFinishPlayEffectObject;
	if (false == softPath.IsValid())
	{
		return;
	}

	PlayEffect_Internal(softPath);
}

void UGsGameObjectPropInteraction::PlayFinishEffect()
{
	if (nullptr == _propData)
	{
		return;
	}
			
	const FSoftObjectPath& softPath = _propData->playEffectObject;
	if (false == softPath.IsValid())
	{
		return;
	}

	PlayEffect_Internal(softPath);
}

void UGsGameObjectPropInteraction::PlayEffect_Internal(const FSoftObjectPath& inSoftPath)
{
	UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(inSoftPath);
	if (load == nullptr)
	{
		return;
	}

	UParticleSystem* particle = Cast<UParticleSystem>(load);
	if (particle == nullptr)
	{
		return;
	}

	if (nullptr == GetActor())
	{
		return;
	}

	AGsActorPropTouch* propActor = Cast<AGsActorPropTouch>(GetActor());
	if (nullptr == propActor)
	{
		return;
	}

	if (UGsEffectComponent* effComp = propActor->GetEffectComponent())
	{
		effComp->OnEffectPos(particle,
			propActor->GetActorLocation() + _propData->playEffectOffset,
			FVector(_propData->playEffectScale));
	}
}

void UGsGameObjectPropInteraction::SetInteractPropEnd(bool inSuccess)
{		
	if (false == inSuccess)
	{		
		_isInteracting = false;
		//인터렉션 실패시 위젯 갱신
		InvalidWidget();
	}
	else
	{		
		if(CheckNeedToDeactiveInteractionWidget())
		{
			_isCanActiveInteractionWidget = false;
		}

		//인터렉션 성공시 프랍은 인터렉션 버튼 숨김 처리 
		HideWidget();

		_isInteracting = false;
	}
}

void UGsGameObjectPropInteraction::DrawDebugProp()
{
	if (_propData)
	{
		if (AActor* actor = GetActor())
		{
			FString destLocationText = "tid : " + FString::FromInt(_propData->id)
			+ "\npos : " + actor->GetActorLocation().ToString()
			+ FString::Printf(TEXT(" interaction max count : %d"), _propData->multipleInteractionCountMax)
			+ FString::Printf(TEXT(" interaction user count : %d"), _propData->multipleInteractionUserCount);

			if (AGsActorPropTouch* propTouch = Cast<AGsActorPropTouch>(actor))
			{
				FVector location = propTouch->GetInteractionWidgetComponent()->GetComponentLocation();

				DrawDebugString(actor->GetWorld(), location, destLocationText, actor, FColor::White, 60.0, false, 1);

				GSLOG(Log, TEXT("%s"), *destLocationText);
			}			
		}		
	}	
}