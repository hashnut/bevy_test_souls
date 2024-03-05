
#include "GsGameObjectLocalPlayer.h"

#include "T1Project.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "ActorEx/GsCharacterRemotePlayer.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ActorEx/GsWeaponActor.h"
#include "ActorComponentEx/GsEffectComponent.h"
#include "ActorComponentEx/GsQueryFloorHeightComponent.h"
#include "ControllerEx/GsPlayerController.h"
#include "Input/GsInputBindingLocalPlayer.h"
#include "Sound/SoundBase.h"
#include "Classes/GsSpawn.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Shared/SharedInclude/SharedFuncs.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Net/GsNetSendService.h"
#include "Net/GsNetSendServiceWorld.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGlobal/GsQuestManager.h"
#include "Management/ScopeGame/GsMinimapManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"
#include "Management/ScopeGame/GsInteractionManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGame/GsObserverManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsGuildManager.h"
#include "Management/ScopeGame/GsFairyManager.h"

#include "Skill/GsSkill.h"
#include "Guild/GsGuildHelper.h"
#include "Option/GsServerOption.h"

#include "Message/GsMessageGameObject.h"
#include "Message/MessageParam/GsCoolTimeMessageParam.h"
#include "Message/MessageParam/GsMinimapMessageParam.h"
#include "Message/MessageParam/GsStageMessageParam.h"
#include "Message/MessageParam/GsFairyMessageParam.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"

#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"
#include "DataSchema/GameObject/Projectile/GsSchemaProjectileSet.h"
#include "DataSchema/Abnormality/Category/GsSchemaAbnormalityCategorySet.h"
#include "DataSchema/Fairy/GsSchemaVehicleConfig.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsGameClientVersion.h"

#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/Define/GsGameObjectText.h"
#include "GameObject/Define/EGsProjectileApplyTargetType.h"
#include "GameObject/Data/GsGameObjectDataLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Skill/GsSkillHandlerLocalPlayer.h"
#include "GameObject/Parts/GsPartsHandlerPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/GsAutoMoveInfo.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Movement/LocalPlayer/Auto/GsWaypointWork.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerLocalPlayer.h"
#include "GameObject/Quest/GsQuestHandler.h"
#include "GameObject/Camera/GsCameraModeHandler.h"
#include "GameObject/Environment/GsAreaEnvHandler.h"
#include "GameObject/Looting/GsLootingHandlerLocalPlayer.h"
#include "GameObject/Fence/GsFenceHandler.h"
#include "GameObject/HUD/GsScreenFilterLocalPlayer.h"
#include "GameObject/Reserve/GsReserveWork.h"
#include "GameObject/Interaction/GsInteractionHandlerLocalPlayer.h"
#include "GameObject/State/LocalPlayer/GsStateLocalPlayerSpawn.h"
#include "GameObject/State/LocalPlayer/GsStateLocalPlayerIdle.h"
#include "GameObject/HUD/GsHUDLocalPlayer.h"
#include "GameObject/Target/AutoTarget/GsTargetRule.h"
#include "GameObject/Stat/GsStatLocalPlayer.h"
#include "GameObject/Fairy/GsFairyHandlerLocalPlayer.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/ObjectClass/GsGameObjectProjectile.h"
#include "GameObject/ObjectClass/GsGameObjectCamp.h"
#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/Abnormality/CrowdControl/GsCrowdControlLocalPlayer.h"
#include "GameObject/AI/GsAIReserveHandler.h"
#include "GameObject/AI/GsAIAvoidCollisionHandler.h"

#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "Camera/LookAt/GsCameraLookAtHandlerLocal.h"

#include "UTIL/GsSoundUtil.h"
#include "UTIL/GsGameObjectUtil.h"
#include "UTIL/GsDir.h"

#if WITH_EDITOR
#include "Animation/GsHitAnimInstance.h"
#endif

#include "DrawDebugHelpers.h"


//프로퍼티
EGsGameObjectType			UGsGameObjectLocalPlayer::GetObjectType() const		{ return EGsGameObjectType::LocalPlayer; }
AActor*						UGsGameObjectLocalPlayer::GetActor() const			{ return GetLocalCharacter(); }
AGsCharacterLocalPlayer*	UGsGameObjectLocalPlayer::GetLocalCharacter() const { return _actor; }
void						UGsGameObjectLocalPlayer::ClearActor()
{ 
	_actor = nullptr;
	_ActorState = ActorState::CLEAR;
}

class FGsQuestHandler* UGsGameObjectLocalPlayer::GetQuestHandler()
{ 
	return GetGameObjectHandler<class FGsQuestHandler>(GameObjectHandlerType::QUEST); 
}
class FGsFenceHandler* UGsGameObjectLocalPlayer::GetFenceHandler()
{ 
	return GetGameObjectHandler<class FGsFenceHandler>(GameObjectHandlerType::FENCE); 
}
class FGsCameraModeHandler*	UGsGameObjectLocalPlayer::GetCameraModeHandler()
{ 
	return GetGameObjectHandler<class FGsCameraModeHandler>(GameObjectHandlerType::CAMERA); 
}
class FGsAreaEnvHandler* UGsGameObjectLocalPlayer::GetAreaEnvHandler()
{ 
	return GetGameObjectHandler<class FGsAreaEnvHandler>(GameObjectHandlerType::AREAENV); 
}
class FGsLootingHandlerLocalPlayer* UGsGameObjectLocalPlayer::GetLootingHandler()
{ 
	return GetGameObjectHandler<class FGsLootingHandlerLocalPlayer>(GameObjectHandlerType::LOOT); 
}

class FGsAIReserveHandler* UGsGameObjectLocalPlayer::GetAIReserve()
{
	return GetGameObjectHandler<class FGsAIReserveHandler>(GameObjectHandlerType::AI_RESERVE);
}

class FGsAIAvoidCollisionHandler* UGsGameObjectLocalPlayer::GetAIAvoidCollision()
{
	return GetGameObjectHandler<class FGsAIAvoidCollisionHandler>(GameObjectHandlerType::AI_AVOIDCOLLISION);
}

const TArray<class IGsReserveWork*>& UGsGameObjectLocalPlayer::GetReserveWorks()
{ 
	return _arrayReserveWork; 
}

void UGsGameObjectLocalPlayer::FairyVehicleInfo::ClearTimer(UWorld* inWorld)
{
	if (_timerHandle.IsValid())
	{
		inWorld->GetTimerManager().ClearTimer(_timerHandle);
	}
}

void UGsGameObjectLocalPlayer::FairyVehicleInfo::StartTimer(UWorld* inWorld, float inDelayTime)
{
	ClearTimer(inWorld);
	inWorld->GetTimerManager().SetTimer(_timerHandle, FTimerDelegate::CreateLambda([this]() {
		Send();
		}), inDelayTime, false);
}

void UGsGameObjectLocalPlayer::FairyVehicleInfo::Send()
{
	FGsNetSendServiceWorld::SendMountVehicle();
#if WITH_EDITOR
	GSLOG(Error, TEXT("[fairyVehicle] SendMountVehicle()"));
#endif
}

void UGsGameObjectLocalPlayer::FairyVehicleInfo::Clear(UWorld* inWorld)
{
	ClearTimer(inWorld);
	_condition = 0;
}

bool UGsGameObjectLocalPlayer::FairyVehicleInfo::IsSuccess() const
{
	return (_condition == (uint16)EGsFairyVehicleCondition::SUCCESS);
}

void UGsGameObjectLocalPlayer::ForceDisconnect::ClearTimer()
{
	_timer = 0.f;
	_elapsed = 0.f;
}

bool UGsGameObjectLocalPlayer::ForceDisconnect::IsActive() const
{
	return _isSafeZone && _isIdle;
}

void UGsGameObjectLocalPlayer::OnStiffen(const FVector& StiffenPos, float fDuration)
{
	if (FGsMovementHandlerLocalPlayer* moveHandler = GetCastMovement<FGsMovementHandlerLocalPlayer>())
	{
		moveHandler->Stiffen(StiffenPos);
	}

	MoveLock(UGsGameObjectLocalPlayer::EMoveLockType::Stiffen, true);

	if (_moveLockTimer.IsValid())
	{
		if (nullptr != GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(_moveLockTimer);
		}
		
	}

	if (nullptr != GetWorld())
	{
		TWeakObjectPtr< UGsGameObjectLocalPlayer> weakThis(this);
		// 디스폰 딜레이 타이머 설정	
		GetWorld()->GetTimerManager().SetTimer(_moveLockTimer, FTimerDelegate::CreateWeakLambda(this, [weakThis]() {
			if (weakThis.IsValid())
			{
				weakThis->MoveLock(UGsGameObjectLocalPlayer::EMoveLockType::Stiffen, false);
				if (nullptr != weakThis->GetWorld())
				{
					weakThis->GetWorld()->GetTimerManager().ClearTimer(weakThis->_moveLockTimer);
				}
			}}), fDuration, false);
	}
}

void UGsGameObjectLocalPlayer::MoveLock(EMoveLockType inType, bool isEnable)
{
	if (_moveLockFlag[(int)inType] == isEnable)
		return;

	_moveLockFlag[(int)inType] = isEnable;

	// 최종 결과물 계산
	int MoveLock = 0;
	for (auto& flag : _moveLockFlag)
	{
		if (flag)
		{
			MoveLock++;
		}
	}
	InternalMoveLock(MoveLock != 0);
}

void UGsGameObjectLocalPlayer::InitMoveLock()
{
	_moveLockFlag.SetNum((int)EMoveLockType::MoveLockTypeMax);
	for (int i = 0; i < _moveLockFlag.Num(); ++i)
	{
		_moveLockFlag[i] = false;
	}
}

void UGsGameObjectLocalPlayer::InternalMoveLock(bool isEnable)
{
	// 내플레이어 행동 제어 설정
	if (FGsMovementHandlerLocalPlayer* movement = GetCastMovement<FGsMovementHandlerLocalPlayer>())
	{
		movement->SetMoveable(!isEnable);
	}

	AGsCharacterLocalPlayer* actorCharacter = GetLocalCharacter();
	if (nullptr == actorCharacter)
	{
		return;
	}

	AGsPlayerController* playerController = actorCharacter->GetPlayerController();
	if (playerController)
	{
		// SetIgnoreMoveInput 메서드는 Stack 구조로 되어있으므로 활성시에는 1번만 적용되게 수정
		bool ignoreFlag = (isEnable && false == playerController->IsMoveInputIgnored());
		playerController->SetIgnoreMoveInput(ignoreFlag);
	}

	// 이동 불가 해제시 예약데이터 처리
	if (false == isEnable)
	{
		TArray<IGsReserveWork*> resrveWorks = GetReserveWorks();
		for (IGsReserveWork* iter : resrveWorks)
		{
			if (iter == nullptr)
			{
				continue;
			}

			if (iter->IsReserveData() == true)
			{
				iter->DoRetry();
				return;
			}
		}
	}
}

FGsScreenFilterLocalPlayer* UGsGameObjectLocalPlayer::GetScreenFilter()
{
	return _screenFiler;
}

void UGsGameObjectLocalPlayer::Initialize(const FGsSpawnParam* SpawnParam)
{
	//GSLOG(Warning, TEXT("UGsGameObjectLocalPlayer::Initialize!!"));
	Super::Initialize(SpawnParam);	

	//타입 정의
	SET_FLAG_TYPE(_objectType, UGsGameObjectLocalPlayer::GetObjectType());

	_actor = nullptr;

	_MovementType = EMovementType::LocalPlayer;
	_movementHandlerspectator = nullptr;

	if (FGsGameObjectDataLocalPlayer* localData = GetCastData<FGsGameObjectDataLocalPlayer>())
	{
		check(localData);
	}	

	//bak1210 : final 에서호출
	CreateSubClass();	
}

void UGsGameObjectLocalPlayer::BindMessage()
{
	Super::BindMessage();
	
	MGameObject& msgGameObject = GMessage()->GetGameObject();

	AddMessageDelegate(msgGameObject.AddUObject(MessageGameObject::PASSIVITY_UPDATE_COLLECTION,
		this, &UGsGameObjectLocalPlayer::OnUpdateCollectionPassivity));

#if QUEST_NPC_ACTIVE
	//플레이어 콜리전 스폰 전에 NPC가 플레이어 근처에 이미 스폰될 경우가 있다. 
	//콜리전 내부에 액터가 스폰되면 OnOverlapBegin event가 실행되지 않는다. 
	//이를 체크하기 위해 NPC game object 생성시 거리 체크를 하여 QuestNpc를 추가한다.  
	AddMessageDelegate(msgGameObject.AddUObject(MessageGameObject::COMMON_SPAWN,
		this, &UGsGameObjectLocalPlayer::OnQuestNpcSpawn));
	AddMessageDelegate(msgGameObject.AddUObject(MessageGameObject::COMMON_DESPAWN,
		this, &UGsGameObjectLocalPlayer::OnQuestNpcDespawn));
#endif

	MStageParam& msgStage = GMessage()->GetStageParam();
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::ENTER_TOWN_CALLBACK,
		this, &UGsGameObjectLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::LEAVE_TOWN_CALLBACK,
		this, &UGsGameObjectLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::ENTER_CHAOS_CALLBACK,
		this, &UGsGameObjectLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::LEAVE_CHAOS_CALLBACK,
		this, &UGsGameObjectLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::ENTER_SAFETY_CALLBACK,
		this, &UGsGameObjectLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::LEAVE_SAFETY_CALLBACK,
		this, &UGsGameObjectLocalPlayer::OnChangeSafetyMode));
	_msgStageHandleList.Emplace(msgStage.AddUObject(MessageStage::ENTER_STAGE_CALLBACK,
		this, &UGsGameObjectLocalPlayer::OnStageChange));
	
	MFairy& msgFairy = GMessage()->GetFairy();
	_msgFairyHandleList.Emplace(msgFairy.AddUObject(MessageFairy::CHANGE_EQUIP,
		this, &UGsGameObjectLocalPlayer::OnFairyChanged));
}

void UGsGameObjectLocalPlayer::UnBindMessage()
{
	FGsMessageHolder* message = GMessage();
	MStageParam& msgStage = message->GetStageParam();
	for (MsgStageHandle& msgStageHandle : _msgStageHandleList)
	{
		msgStage.Remove(msgStageHandle);
	}
	_msgStageHandleList.Empty();

	MFairy& msgFairy = GMessage()->GetFairy();
	for (MsgFairyHandle& msgFairyHandle : _msgFairyHandleList)
	{
		msgFairy.Remove(msgFairyHandle);
	}
	_msgFairyHandleList.Empty();

	Super::UnBindMessage();
}

void UGsGameObjectLocalPlayer::OnChangeSafetyMode(const IGsMessageParam* inParam)
{
	const FGsStageFenceMsgParam* castParam = inParam->Cast<const FGsStageFenceMsgParam>();
	if (nullptr == castParam)
	{
		return;
	}
	_localCollisionRule = castParam->_isSafety == false;

	if (IsBranch(EGsGameClientBranchType::PC_RVO))
	{
		UpdateRVOAvoidance(castParam->_isSafety);
	}

	UpdateCollisionPreset();
}

void UGsGameObjectLocalPlayer::OnFairyChanged(const IGsMessageParam* inParam)
{
// 	const FGsFairyMsgParamInt* castParam = inParam->Cast<const FGsFairyMsgParamInt>();
// 	if (nullptr == castParam)
// 	{
// 		return;
// 	}

	// 탑승 버프가 있는 페어리인지 확인
	bool isVehicleFairy = false;
	TWeakPtr<FGsFairyItem> equipFairyData = GSFairy()->GetEquipFairyData();
	const TWeakPtr<FGsFairyData> fairyData = GSFairy()->GetFairyData(equipFairyData.Pin()->GetFaryID());
	const FGsSchemaPassivitySet* passivity = fairyData.Pin()->GetByVehiclePassivity(
		equipFairyData.Pin()->GetCurrentLevel()
	);
	if (passivity)
	{
		FGsFairyLevelInfo levelinfo;
		fairyData.Pin()->GetFairyCurLevelPassivityList(levelinfo);
		isVehicleFairy = FGsAbnormalityHandlerBase::IsAbnormalityEffectType(
			AbnormalityEffectType::UTIL_MOUNT_VEHICLE, levelinfo._byVehicleAbnormalitySet);
	}
	SetFairyVehicleCondition(EGsFairyVehicleCondition::VEHICLE_FAIRY, !isVehicleFairy);
}

void UGsGameObjectLocalPlayer::ActorSpawned(AActor* Spawn)
{
	Super::ActorSpawned(Spawn);

	// 페어리 탈것 관련 갱신
	UpdateAllFairyVehicleCondition();
	OnVehicleSendTimer();	
}

void UGsGameObjectLocalPlayer::ActorDespawn()
{	
#if QUEST_NPC_ACTIVE
	_questNpcArray.Empty();
#endif

	if (nullptr != GetWorld() && _moveLockTimer.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(_moveLockTimer);
	}

	// 모든 SubClass들을 정리하고 난후 _actor 포인터 소멸
	Super::ActorDespawn();
}

void UGsGameObjectLocalPlayer::SetLocalPlayerHidden()
{
	if (true == _actor->IsHidden())
		return;

	_actor->SetActorHiddenInGame(true);

	if (FGsFairyHandlerPlayer* fairyHandler = GetFairyHandler())
	{
		fairyHandler->SetFairyHiddenInGame();
	}
}

void UGsGameObjectLocalPlayer::InitializeActor(AActor* Owner)
{
#if QUEST_NPC_ACTIVE
	_questNpcArray.Empty();
#endif

	InitMoveLock();

#if QUEST_NPC_DEBUG
	GSLOG(Log, TEXT("QUEST_NPC_DEBUG - UGsGameObjectLocalPlayer::InitializeActor"));
#endif

	_actor = Cast<AGsCharacterLocalPlayer>(Owner);

	//bak1210 : 케릭터끼리 오버랩된 상황 캐취
	_actor->OnOverlapedCharacterBase.BindUObject(this, &UGsGameObjectLocalPlayer::CallbackOverlapedCharacterBase);
	_clientCollisionRule = false;

	// 컨트롤러 빙의
	if (APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		controller->Possess(_actor);
	}

	Super::InitializeActor(Owner);

#if QUEST_NPC_ACTIVE	
	_actor->GetQuestNpcSphereComp()->OnComponentBeginOverlap.AddDynamic(this, &UGsGameObjectLocalPlayer::OnEnterQuestNpc);
	_actor->GetQuestNpcSphereComp()->OnComponentEndOverlap.AddDynamic(this, &UGsGameObjectLocalPlayer::OnLeaveQuestNpc);
#endif	

	if (IsBranch(EGsGameClientBranchType::PC_RVO))
	{
		UpdateRVOAvoidance();
	}
	else
	{
		// 2022/11/07 PKT - Nearby Pawn Detection Sphere
		_actor->GetPawnDetectionBoxComp()->OnComponentBeginOverlap.AddDynamic(this, &UGsGameObjectLocalPlayer::OnEnterDetectionPawn);
		_actor->GetPawnDetectionBoxComp()->OnComponentEndOverlap.AddDynamic(this, &UGsGameObjectLocalPlayer::OnleaveDetectionPawn);
	}

	// 필요 Primitive 셋업
	_actor->SetupPrimitives(true);	

	//Hit 모션 추가
	FSoftObjectPath path;
	if (_actor->LoadHitAnimInstance(path))
	{
		LegacyHit = false;
	}

	//bak1210 CreateSubClass에서 옮김------------------------------------------------------------------------------------------------
	
	// 인풋 바인딩 생성후 초기화해야함!!!
	// 액터 초기화때 플레이어 컨트롤도 초기화함
	if (_actor)
	{
		_actor->Initialize(this);

		if (_inputBinder)
		{
			_inputBinder->Initialize(this);
			_actor->SetInputBinder(_inputBinder);
		}
#ifdef USE_INGAME_LOOKAT
		//bak1210 : 내부에서 호출
		/*if (FGsCameraLookAtHandlerLocal* lookat = GetGameObjectHandler< FGsCameraLookAtHandlerLocal>(GameObjectHandlerType::LOOKAT))
		{
			lookat->InitializeActor(_actor);
		}*/
#endif

		// 네임플레이트 HP바 바인딩 설정
		if (UGsUIBillboardCreature* widget = _actor->GetWidget())
		{
			widget->SetTarget(this);
		}

		// HUD 바인딩
		if (APlayerController* controller = Cast<APlayerController>(_actor->GetController()))
		{
			if (AGsHUDLocalPlayer* hud = Cast<AGsHUDLocalPlayer>(controller->GetHUD()))
			{
				hud->SetPlayer(this);
			}
		}

		// 무기 타입에 따른 AnimationBP설정
		if (FGsGameObjectDataLocalPlayer* localData = GetCastData<FGsGameObjectDataLocalPlayer>())
		{
			UpdateWeaponChange(localData->GetCurrentWeaponType());
			// 스킬셋 설정1
			GSkill()->ChangeWeapon(localData->GetCurrentWeaponType());
		}

	}
	//-----------------------------------------------------------------------------------------------------------------------------------

	// 현재 씬에 모든 AreaNameHud 를 찾는다
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(Owner->GetWorld(), AGsAreaNameHUD::StaticClass(), FoundActors);

	bool isEnter = false;
	for (AActor* iter : FoundActors)
	{
		if (iter == nullptr)
		{
			continue;
		}
		AGsAreaNameHUD* areaNameHud = Cast<AGsAreaNameHUD>(iter);
		if (areaNameHud == nullptr)
		{
			continue;
		}

		if (areaNameHud->GetIsInCollider() == true)
		{
			isEnter = true;
			break;
		}
	}
	// 엔터된게 없으면
	if (isEnter == false)
	{
		if (APlayerController* controller = Cast<APlayerController>(_actor->GetController()))
		{
			if (AGsHUDLocalPlayer* hud = Cast<AGsHUDLocalPlayer>(controller->GetHUD()))
			{
				// 현재맵 기준으로 출력
				hud->ShowCurrentMapInfo();
			}
		}
	}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_MODE_SAVE_AUTO_POSITION, nullptr);

	_actor->OnHitCampEvent.AddLambda([this](const AActor* In_camp)
	{
		OnCampHit(In_camp);
	});	
}

FGsGameObjectData* UGsGameObjectLocalPlayer::CreateData(const FGsSpawnParam* SpawnParam)
{
	return new FGsGameObjectDataLocalPlayer(SpawnParam);
}

TSharedPtr<FGsTargetHandlerBase> UGsGameObjectLocalPlayer::CreateTarget()
{
	return MakeShareable(new FGsTargetHandlerLocalPlayer());
}

TSharedPtr<FGsSkillHandlerBase> UGsGameObjectLocalPlayer::CreateSkill()
{
	return MakeShareable(new FGsSkillHandlerLocalPlayer());
}

TSharedPtr<FGsPartsHandlerBase> UGsGameObjectLocalPlayer::CreateParts()
{
	return MakeShareable( new FGsPartsHandlerPlayer());
}

TSharedPtr<FGsMovementHandlerBase> UGsGameObjectLocalPlayer::CreateMovement()
{
	TSharedPtr<FGsMovementHandlerBase> Result = MakeShareable( new FGsMovementHandlerLocalPlayer());
	Result->SetPrivateUpdate(true);
	return Result;
}

TSharedPtr<FGsAbnormalityHandlerBase> UGsGameObjectLocalPlayer::CreateAbnormality()
{
	return MakeShareable(new  FGsAbnormalityHandlerLocalPlayer());
}

void UGsGameObjectLocalPlayer::CreateSubClass()
{
	Super::CreateSubClass();
	

	// 키입력 바인딩
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
#ifdef USE_INGAME_LOOKAT
	AppendInitialize(new FGsCameraLookAtHandlerLocal()));
#endif

	AppendInitialize(MakeShareable(new FGsQuestHandler()));
	if (FGsQuestHandler* quest = GetQuestHandler())
	{
		quest->Init();
	}
	// cam mode 앞에 세팅 될 것
	// 1. 액터 인풋바인딩 세팅
	// 2. look handler 메시지 바인딩	
	AppendInitialize(MakeShareable(new FGsCameraModeHandler()));

	//Fence 핸들러는 Env보다 앞에 있어야한다. 그 이유는 레벨 이동시 Env를 꺼줘야하는데 Env가 먼저 메시지를 언바인드하면 문제가 생기기 때문이다.
	AppendInitialize(MakeShareable(new FGsFenceHandler()));
	AppendInitialize(MakeShareable(new FGsAreaEnvHandler()));
	AppendInitialize(MakeShareable(new FGsLootingHandlerLocalPlayer()));
	AppendInitialize(MakeShareable(new FGsAIReserveHandler()));
	AppendInitialize(MakeShareable(new FGsAIAvoidCollisionHandler()));	

	if (nullptr == _screenFiler)
	{
		_screenFiler = CreateScreenFilter();
		_screenFiler->Initialize(this);
	}

	if (FGsMovementHandlerBase* move = GetMovementHandler())
	{
		const FGsSchemaCreatureCommonInfo* commonInfo = GetCommonInfoTable();
		move->InitSpeed(commonInfo->moveSpeed, commonInfo->walkSpeed);
	}

	GSAI()->SetCharacter(this);
	// 현재 맵이 시공의 틈새면 카메라 백업 데이터를 지우지 않는다
	// 시공의 틈새 전용 모드로 세팅하고
	// 시공의 틈새 나가서 백업데이터 사용한다
	// 매니저안에서(카메라 모드 매니저) 다른 매니저(레벨 매니저) 접근 안할려고 외부에서 처리
	bool isClearBackup = true;
	if (GLevel()->IsSpaceCrackMapContents() == true)
	{
		isClearBackup = false;
	}
	GSCameraMode()->SetCharacter(this, isClearBackup);
	GSInteraction()->SetCharacter(this);
	GSMinimap()->SetCenterObject(this);
	GSWorldMap()->SetCharacter(this);	

	_arrayReserveWork.Add(_inputBinder);
	// 
	_arrayReserveWork.Add(GetLootingHandler());

	FGsInteractionHandlerLocalPlayer* localInteraction = GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
	if (localInteraction != nullptr)
	{
		_arrayReserveWork.Add(localInteraction);
	}
	
	// 지도 땅찍고 이동 할때 
	// 피격시 이동 멈추는 현상 수정
	// https://jira.com2us.com/jira/browse/C2URWQ-6334
	if (FGsMovementHandlerLocalPlayer* localMovement = GetCastMovement<FGsMovementHandlerLocalPlayer>())
	{
		if (FGsWaypointWork* wayPointWork =
			localMovement->GetStateAutoMove().GetWaypointWork())
		{
			_arrayReserveWork.Add(wayPointWork);
		}
	}

	FGsSkillHandlerLocalPlayer* localSkillHandler = GetCastSkill<FGsSkillHandlerLocalPlayer>();
	if (localSkillHandler != nullptr)
	{
		_arrayReserveWork.Add(localSkillHandler);
	}
	//==================================================================================================================


	FGsGameObjectMessageParamMapIcon paramIcon(this, EGsMapIconType::LocalPlayer);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::MINIMAP_SPAWN_ICON, &paramIcon);
}

void UGsGameObjectLocalPlayer::RemoveSubClass()
{
	if (_inputBinder)
	{
		_inputBinder->Finalize();
		_inputBinder = nullptr;
	}

	if (_arrayReserveWork.Num() != 0)
	{
		_arrayReserveWork.Empty();
	}

	if (_screenFiler)
	{
		_screenFiler->Finalize();
		delete _screenFiler;
		_screenFiler = nullptr;
	}

	_movementHandlerspectator = nullptr;

	if (UGsQuestManager* questManager = GSQuest())
	{
		questManager->RemoveCharacter();
	}
	if (FGsCameraModeManager* cameraModeManager = GSCameraMode())
	{
		cameraModeManager->RemoveCharacter();
	}
	if (FGsAIManager* aiManager = GSAI())
	{
		aiManager->RemoveCharacter();
	}
	if (FGsInteractionManager* interactionManager = GSInteraction())
	{
		interactionManager->RemoveCharacter();
	}
	if (UGsMinimapManager* minimapManager = GSMinimap())
	{
		minimapManager->RemoveCenterObject();
	}
	if (FGsWorldMapManager* worldmapManager = GSWorldMap())
	{
		worldmapManager->RemoveCharacter();
	}

	_fairyVehicleInfo.Clear(GetWorld());

	Super::RemoveSubClass();
}


void UGsGameObjectLocalPlayer::NetChangeWeaponFinishedAck(
	PD::SC::PKT_SC_ITEM_EQUIP_CHANGE_WEAPON_TYPE_FINISHED_READ* Packet)
{
	// Super::NetChangeWeaponFinishedAck(Packet);

	_fsm->ProcessEvent(EGsStateBase::ChangeWeaponEnd);

	// 스킬, 이동을 Stop
	if (FGsSkillHandlerBase* skill = GetSkillHandler())
	{
		skill->StopSkill();
	}

	if (FGsMovementHandlerBase* move = GetMovementHandler())
	{
		move->Stop();
	}

	// 성공 실패 여부에 따라 실행
	if (Packet->Reason() == ChangeWeaponTypeFinishReason::NORMAL)
	{
		UpdateWeaponChange(Packet->WeaponType());
	}
}

void UGsGameObjectLocalPlayer::NetAbnormalityResultAck(PD::SC::PKT_SC_ADD_ABNORMALITY_RESULT_READ* Packet)
{
	if (Packet)
	{
		// 내캐릭터 이뮨표시는 무조건 출력
		// 이뮨 처리 확인
		if (Packet->HitResultType() == HitResultType::IMMUNE_DAMAGE)
		{
			SetDealScroll(UGsBlueprintFunctionLibraryUI::GetDealScrollKey(Packet->HitResultType(), true),
				TEXT(""), FVector::ZeroVector);
		}
		else if (Packet->HitResultType() == HitResultType::IMMUNE_ABNORMALITY)
		{
			const FGsSchemaAbnormalityCategorySet* categorySet = UGsTableUtil::FindRowById<UGsTableAbnormalityCategorySet,
				FGsSchemaAbnormalityCategorySet>(Packet->AbnormalityCategoryId());
			if (categorySet)
			{
				SetDealScroll(categorySet->immuneDealScrollTextMe, TEXT(""), FVector::ZeroVector);
			}
		}
	}
}

void UGsGameObjectLocalPlayer::CombatOptionUpdate()
{
	// 타겟룰 갱신
	if (FGsTargetHandlerLocalPlayer* targetHandler = GetCastTarget<FGsTargetHandlerLocalPlayer>())
	{
		if (FGsTargetRule* targetRule = targetHandler->GetTargetRule())
		{
			targetRule->ApplyTargetList();
		}
		else
		{
			// 없으면 안되는 상황으로 보이지만 일단 로그 추가
			GSLOG(Error, TEXT("not create subclass to FGsTargetRule"));
		}
	}
	else
	{
		// 없으면 안되는 상황으로 보이지만 일단 로그 추가
		GSLOG(Error, TEXT("not create subclass to FGsTargetHandlerLocalPlayer"));
	}
	
}

void UGsGameObjectLocalPlayer::NetIffFactionActorUpdate(const IffFactionActor& IffFactionActor)
{
	// IffFactionActor 정보 동기화
	GetData()->SetIffFaction(IffFactionActor);
	// IsEnemy함수를 사용하기 위해 2개의 데이터에 동기화 하는 경우
	_netSyncIffhellper.mOwnerIffFaction = IffFactionActor;
}

void UGsGameObjectLocalPlayer::NetSkillDamagedAck(PD::SC::PKT_SC_SKILL_DAMAGED_READ* Packet)
{
	Super::NetSkillDamagedAck(Packet);
}

bool UGsGameObjectLocalPlayer::OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId,
	uint32 StrikerNotifyId, HitResultType ResultType, DamageByType DamageType, DamageStatType DamageStat,
	HealByType HealType, int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage)
{
	Super::OnHit(Striker, SubStriker, StriketSkillId, StrikerNotifyId, ResultType,
		DamageType, DamageStat, HealType, Damage, PassivityOwner, IsNoDamage);

	if (Striker && Striker != this)
	{
#ifdef HIT_EFFECT_BY_SERVER
		// 공격방향
		FVector dir = GetLocation() - Striker->GetLocation();
		dir.Z = 0.0f;
		dir.Normalize();

		// 딜스크롤
		SetDealScroll(UGsBlueprintFunctionLibraryUI::GetDealScrollKey(ResultType, true),
			FString::FromInt(Damage), dir);
		
#endif // HIT_EFFECT_BY_SERVER
		// 누가 local을 침
		if (ResultType != HitResultType::HEAL) // https://jira.com2us.com/jira/browse/C2URWQ-5268
		{
			FGsGameObjectMessageParamTarget paramTarget(this, Striker);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_HIT, &paramTarget);
		}
	}

	return true;
}

void UGsGameObjectLocalPlayer::OnDie()
{
	// Super 에서 Die Proc 처리(소멸) 전에, 해당 Player에 이펙트를 Play 하기 위하여
	// Super::OnDie() 전에 실행

	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_DIE, nullptr);
	
	if (_inputBinder != nullptr)
	{
		_inputBinder->OnLocalDie();
	}
	Super::OnDie();
}

// 부활 Action
void UGsGameObjectLocalPlayer::OnRevive()
{
	Super::OnRevive();

	int32 currentMapId = GLevel()->GetCurrentLevelId();
	FGsPrimitiveInt32 paramMapId(currentMapId);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_REVIVE, &paramMapId);
}

void UGsGameObjectLocalPlayer::OnWarp()
{
	//Super::OnWarp();

	//// 카메라 백업
	//// 기존엔 remove character 쪽이었는데
	//// 워프 연출이 생겨서
	//// 워프 연출상태가 저장되면 안되므로
	//// 연출전 저장한다
	//// 시공의 틈새에서 나갈때는 
	//// 1. 현재 카메라 모드는 시공의 틈새 전용 모드라 저장 x
	//// 2, 이전 백업 데이터로 복구해야되어서 저장 x
	//// 관전 모드에서도 저장 x
	//if (GLevel()->IsSpaceCrackMapContents() == false &&
	//	GSObserver()->GetIsObserveStart() == false)
	//{
	//	GSCameraMode()->SaveBackUpTransform();
	//}

	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_WARP, nullptr);
	
	// 카메라 모드 워프 시작으로 변경
	if (FGsCameraModeHandler* camModeHandler = GetCameraModeHandler())
	{
		//camModeHandler->ChangeState(EGsCameraMode::WarpStart);
		camModeHandler->OnWarp([this]()
			{
				Super::OnWarp();
			});
	}
}

void UGsGameObjectLocalPlayer::OnSpawn(bool In_isAlive, bool In_isNormalReason)
{
	if (true == In_isAlive)
	{
		if (In_isNormalReason)
		{
			_fsm->ProcessEvent(EGsStateBase::Spawn);
			Super::OnPlayerTeleportEffect(ETeleportEffectType::ARRIVE);

			// https://jira.com2us.com/jira/browse/C2URWQ-4873
			// 맵전환뒤 예약된 이동이
			// 스폰 연출시 서버에서 강제로 stop시켜서 이동 안하는 현상 대응
			GSAI()->SetNextMapMoveWait();
		}
		else
		{
			_fsm->ProcessEvent(EGsStateBase::Idle);
			FGsNetSendServiceWorld::SendReqSpawnMeAction();
		}
	}
	else
	{
		_fsm->ProcessEvent(EGsStateBase::Dead);
		FGsNetSendServiceWorld::SendReqSpawnMeAction();
	}

	GMessage()->GetSystem().SendMessage(MessageSystem::LOAD_TOPO_START_SPAWN);

	GMessage()->GetSound().SendMessage(MessageSound::PLAY_BGM, nullptr);
}

void UGsGameObjectLocalPlayer::OnUpdateCollectionPassivity(const IGsMessageParam* InParam)
{
	const FGsPrimitiveInt32* param = InParam->Cast<const FGsPrimitiveInt32>();
	if (param)
	{
		if (FGsAbnormalityHandlerBase* abnormalityHandler = GetAbnormalityHandler())
		{
			abnormalityHandler->OnUpdateCollectionPassivity(param->_data);
		}
	}
}

void UGsGameObjectLocalPlayer::UpdateSpeedMovementHandler()
{
	// Run / Walk 타입을 구분하여 Speed값을 설정
	MotionType motiontype = GetCastMovement<FGsMovementHandlerLocalPlayer>()->GetMotionType();
	if (motiontype == MotionType::WALK)
	{
		ApplyWalkSpeedFromCommonInfo();
	}
	else
	{
		ApplyMoveSpeedFromStatInfo();
	}
}

bool UGsGameObjectLocalPlayer::SetTeleport()
{
	_fsm->ProcessEvent(EGsStateBase::Warp);

	return (static_cast<uint8>(EGsStateBase::Warp) == _fsm->CurrentState()->GetStateID()) ? true : false;
}

void UGsGameObjectLocalPlayer::CallbackOverlapedCharacterBase(int32 inOverlapCount)
{
	_clientCollisionRule = inOverlapCount == 0;
	UpdateCollisionPreset();
}

// 시퀀스 시작 이벤트
void UGsGameObjectLocalPlayer::OnStartSequence()
{
	GetMovementHandler()->Stop();

	GSAI()->SetAIStop(true);

	//FGsInputEventMsgBase msg;
	//msg.Id = 1;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_CINEMATIC, true);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	/*
	if (false == OnEnterOnceCutSceneArea.IsBoundToObject(this))
	{
		OnEnterOnceCutSceneArea.AddUObject(this, &UGsGameObjectLocalPlayer::ChangeFreezeState);
	}
	*/
}

// 시퀀스 끝 이벤트
void UGsGameObjectLocalPlayer::OnEndSequence()
{
	//FGsInputEventMsgBase msg;
	//msg.Id = 0;
	FGsInputEventMsgFlag msg(EGsKeyMappingInputFlags::SETTING_CINEMATIC, false);
	GMessage()->GetInput().SendMessage(MessageInput::BlockInput, msg);

	// 여기서  idle 스테이트로
	_fsm->ProcessEvent(EGsStateBase::Idle);
	//OnEnterOnceCutSceneArea.Clear();

	GSAI()->SetAIStop(false);
}

void UGsGameObjectLocalPlayer::ChangeFreezeState()
{
	if (nullptr != _fsm)
	{
		_fsm->ProcessEvent(EGsStateBase::Freeze);
	}
}

TSharedPtr<FGsInteractionHandlerBase> UGsGameObjectLocalPlayer::CreateInteractionHandler()
{
	return MakeShareable(new FGsInteractionHandlerLocalPlayer());
}

TSharedPtr<FGsStatBase> UGsGameObjectLocalPlayer::CreateCreatureStat()
{
	if (FGsGameObjectDataCreature* creatureData = GetCastData<FGsGameObjectDataCreature>())
	{
		if (false == creatureData->GetStatBase().IsValid())
		{
			return creatureData->CreateCreatureStat<FGsStatLocalPlayer>(this);
		}

		return StaticCastSharedPtr< FGsStatBase>(creatureData->GetStatBase());
	}

	return nullptr;
}

FGsScreenFilterLocalPlayer* UGsGameObjectLocalPlayer::CreateScreenFilter()
{
	return new FGsScreenFilterLocalPlayer();
}

TSharedPtr<FGsFairyHandlerPlayer> UGsGameObjectLocalPlayer::CreateFairyHandler()
{
	return MakeShareable(new FGsFairyHandlerLocalPlayer());
}

// iff 변경됨
void UGsGameObjectLocalPlayer::OnIFFChanged()
{
	// 나 미니맵 팀 변경
	FGsMinimapIconTeamMessageParam param(true, 0);
	GMessage()->GetUIMinimapIconTeam().SendMessage(MessageContentMinimap::INVALIDATE_IFF, &param);
}

void UGsGameObjectLocalPlayer::StopAuto(bool In_isStopAI)
{
	if (true == In_isStopAI)
	{
		GSAI()->SetActiveAI(false);
	}

	GetQuestHandler()->OnUserAutoMoveStop();
}

void UGsGameObjectLocalPlayer::OnGameObjectDataUpdate(EGsGameObjectDataMember MemberType)
{
	Super::OnGameObjectDataUpdate(MemberType);

	switch (MemberType)
	{
	case EGsGameObjectDataMember::Level:
	{
		FGsGameObjectDataLocalPlayer* gameData = GetCastData<FGsGameObjectDataLocalPlayer>();
		// Send Message
		FGsMessageHolder* message = GMessage();
		message->GetUserBasicInfo().SendMessage(MessageUserInfo::LEVEL, gameData->GetLevel());
		// 레벨 갱신으로 exp도 갱신시킨다.
		message->GetUserBasicInfo().SendMessage(MessageUserInfo::EXP, gameData->GetUserExp());
	}
		break;
	}
}

void UGsGameObjectLocalPlayer::OnLearnSkill(SkillId InSkillId)
{
	Super::OnLearnSkill(InSkillId);	

	// 필요 스킬 재로드
	if (FGsSkillHandlerBase* skillHandler = GetSkillHandler())
	{
		skillHandler->LoadSkill();
	}
}

void UGsGameObjectLocalPlayer::ChangeFairyEquip(FairyId id)
{
	auto fairyHandler = GetFairyHandler();
	if (!fairyHandler) return;

	// 다음의 이슈를 클라이언트에서 해결하기 위해 대화 연출중에 스킵
	// C2URWQ-2536, C2URWQ-2509, C2URWQ-2255, C2URWQ-2402
	// PVP시에 SC_USER_LOOK_INFO_CHANGED 패킷이 오기 때문에 이 함수가 호출된다.
	if (GSCameraMode()->GetIsCameraDialogPlay() == true)
	{
		return;
	}

	//equip Ack때 effect를 처리하면 좋지만, 페어리가 lookinfo 변경시 create 되고 ack에서 effect하면 동기가 잘 안맞음
	if (fairyHandler->CheckFairyChange(id)) 
	{
		Super::ChangeFairyEquip(id);
		
		if (auto fairy = fairyHandler->GetFairyActor())
		{
			const auto globalConstData = GData()->GetGlobalData();

			if (!fairy->IsHidden() && globalConstData) //hidden일 때는 연출하지 않는다.
			{ 
				USoundBase* soundRes = globalConstData->_fairyFieldSpawnSound.Get();
				if (globalConstData->_fairyFieldSpawnSound.IsPending())
				{
					const FSoftObjectPath& AssetRef = globalConstData->_fairyFieldSpawnSound.ToSoftObjectPath();
					UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(AssetRef);
					soundRes = Cast<USoundBase>(load);
				}

				if (soundRes)
				{
					UGameplayStatics::SpawnSoundAttached(
						soundRes, fairy->GetRootComponent(), NAME_None, FVector(ForceInit),
						EAttachLocation::SnapToTarget, false, 1, 1);
				}
			}
		}

		return;
	}
	
	// [B1] | ejrrb10 | 일반적인 스폰일 때만 페어리 스폰 연출을 타 플레이어에게 노출
	// 무기 교체 시에 페어리 연출 나오는 버그 수정
	//fairyHandler->ShowFairySpawnEffect();

	Super::ChangeFairyEquip(id);
}

//state, animation 없는 그냥 연출이라, 일정 시간 후에 착용되도록 한다.
void UGsGameObjectLocalPlayer::ChangeCostumeEquip(CostumeId lookatId, CostumeId costumeId, bool isHairOn)
{
	auto costumeHandler = GetCustomizeHandler();
	if (!costumeHandler) return;

	if (costumeHandler->CheckCostumeChange(lookatId, costumeId))
	{
		if (const UGsGlobalConstant* globalConstData = GData()->GetGlobalData())
		{
			auto effectRes = globalConstData->_equipChangeParticle;
			auto soundObj = globalConstData->_equipSound;

			if (AGsCharacterBase* character = GetCharacter())
			{
				UGsEffectComponent* effectComp = character->GetEffectComponent();
				if (effectRes && effectComp)
				{
					effectComp->OnEffect(effectRes, true, EPSCPoolMethod::AutoRelease,
						EAttachLocation::KeepRelativeOffset, FVector(ForceInit), FVector::OneVector, NAME_None, true);

					if (auto world = GetWorld())
					{
						world->GetTimerManager().SetTimer(_timeCostumeEffect, [lookatId, costumeId, isHairOn, soundObj, this]() {

							UpdateCostumeEquip(lookatId, costumeId, isHairOn);
							if (auto character = GetCharacter())
							{
								auto soundRes = soundObj.Get();
								if (soundObj.IsPending())
								{
									const FSoftObjectPath& AssetRef = soundObj.ToSoftObjectPath();

									UObject* load = UAssetManager::GetStreamableManager().LoadSynchronous(AssetRef);
									soundRes = Cast<USoundBase>(load);
								}

								UGameplayStatics::SpawnSoundAttached(
									soundRes, character->GetRootComponent(), NAME_None, FVector(ForceInit),
									EAttachLocation::SnapToTarget, false, 1, 1);
							} 
						}, 0.24f, false);
						return;
					}
				}
			}
		}
	}

	Super::ChangeCostumeEquip(lookatId, costumeId, isHairOn);
}

void UGsGameObjectLocalPlayer::OnTrigger(int64 TriggerGameId, EGsTriggerType TriggerType, bool IsActive)
{
	if (TriggerType == EGsTriggerType::Projectile)
	{
		if (false == IsActive)
		{
			_screenFiler->DeactiveScreenFilter(EGsScreenFilterType::SkillCollision);
		}
		else if (UGsGameObjectBase* findObject = GSGameObject()->FindObject(EGsGameObjectType::Projectile, TriggerGameId))
		{
			UGsGameObjectProjectile* projectile = findObject->CastGameObject<UGsGameObjectProjectile>();
			FGsGameObjectData* projectileData = projectile->GetData();
			IffFactionActor& iffInfo = projectileData->GetIff();
			const FGsSchemaProjectileSet* projectileTableData = projectile->GetProjectileData();

			int64 gameId = GetGameId();
			// All은 무조건
			if (projectileTableData->screenFilter.applyTarget != EGsProjectileApplyTargetType::All)
			{
				bool isEnemy = IsEnemy(projectile);
				if (projectileTableData->screenFilter.applyTarget == EGsProjectileApplyTargetType::Enemy &&
					false == isEnemy)
				{
					return;
				}

				if (projectileTableData->screenFilter.applyTarget == EGsProjectileApplyTargetType::Ally &&
					isEnemy)
				{
					return;
				}
			}
			_screenFiler->ActiveScreenFilter(EGsScreenFilterType::SkillCollision, projectileTableData->screenFilter);
		}
	}
}

void UGsGameObjectLocalPlayer::OnTargetClear(const IGsMessageParam* inParam)
{
	// Super에서 타겟이 클리어 될수 있기 때문이 미리 얻어둔다.
	FGsTargetHandlerBase* targetHandler = GetTargetHandler();
	UGsGameObjectBase* firstTarget = targetHandler->GetTarget();
	UGsGameObjectBase* secondTarget = targetHandler->GetSecondTarget();

	// 기존 타겟정보 제거
	Super::OnTargetClear(inParam);

	const FGsGameObjectMessageParam* castParam = inParam->Cast<const FGsGameObjectMessageParam>();
	// 플레이어가 아니면 2차 타겟은 1차 타겟으로 승격
	if (castParam->_paramOwner == firstTarget && secondTarget && 
		false == secondTarget->IsObjectType(EGsGameObjectType::Player))
	{
		targetHandler->SetTarget(secondTarget);
	}
}

//https://jira.gamevilcom2us.com/wiki/pages/viewpage.action?pageId=251258440
const FName DefaultLocalPreset = TEXT("LocalCollision");
const FName BlockOtherPreset = TEXT("LocalBlockOther");
void UGsGameObjectLocalPlayer::BlockOtherPlayer(bool InBlock)
{
	if (AGsCharacterLocalPlayer* localActor = GetLocalCharacter())
	{
		if (_serverCollisionRule)
		{
			localActor->SetCollisionProfileName(InBlock ? BlockOtherPreset : DefaultLocalPreset);

			// 2022/11/11 PKT - Pawn Detection Enable ( Town : False, etc : True )
			localActor->SetPawnDetectionBoxEnable(InBlock);
		}
		else
		{
			localActor->SetCollisionProfileName(DefaultLocalPreset);
			localActor->SetPawnDetectionBoxEnable(false);
		}
	}
}

void UGsGameObjectLocalPlayer::UpdateCollisionPreset()
{
	BlockOtherPlayer(_localCollisionRule && _clientCollisionRule);
}

void UGsGameObjectLocalPlayer::Update(float delta)
{
	Super::Update(delta);

	if (EMovementType::Spectator == _MovementType && nullptr != _movementHandlerspectator)
	{
		_movementHandlerspectator->Update(delta);
	}

	if (_inputBinder != nullptr)
	{
		_inputBinder->Update(delta);
	}

	if (_forceDisconnect.IsActive())
	{
		_forceDisconnect._timer += delta;
		_forceDisconnect._elapsed += delta;
		const UGsGlobalConstant* globalConstData = GData()->GetGlobalData();
		if (_forceDisconnect._timer > globalConstData->ForceDiconnetTime)
		{
			_forceDisconnect._timer = 0.f;

			// Send
			FGsNetSendService::SendReqLeaveToInActivity(_forceDisconnect._elapsed);
		}
	}


#if WITH_EDITOR
	if (showDebug)
	{
		if (FGsTargetHandlerBase* TargetHandler = GetTargetHandler())
		{
			if (UGsGameObjectBase* Taget = TargetHandler->GetTarget())
			{
				auto charecter = Taget->GetCharacter();

				EGsAnimStateBlendType blendType = static_cast<EGsAnimStateBlendType> (charecter->SelectHitBlendType);
				EGsAnimStateHitDirection hitDir = static_cast<EGsAnimStateHitDirection> (charecter->SelectHitDirection);

				FString strBlendType = FGsTextUtil::GetEnumValueAsString<EGsAnimStateBlendType>("EGsAnimStateBlendType", blendType);
				FString strHitDir = FGsTextUtil::GetEnumValueAsString<EGsAnimStateHitDirection>("EGsAnimStateHitDirection", hitDir);

				const FString tString = FString::Printf(TEXT("Target BlendType[%s] HitDirection[%s]"), *strBlendType, *strHitDir);

				FVector2D textScale{ 2.f, 2.f };
				GEngine->AddOnScreenDebugMessage(1002, 15.0f, FColor::Green, tString, true, textScale);
			}
		}
	}
#endif
}

void UGsGameObjectLocalPlayer::Finalize()
{
	Super::Finalize();
}

int UGsGameObjectLocalPlayer::UpdateCombatStatus(bool InImmediately)
{
	// 특정 상태에서 연출 처리 생략
	bool isImmediately = InImmediately;
	if (GSCameraMode()->GetIsCameraDialogPlay())
	{
		isImmediately = true;
	}

	int result = Super::UpdateCombatStatus(isImmediately);

	// 디졸브 효과로 인해 xray vision효과가 반영되는 상황이므로 잠시 비활성
	// 추후 다른곳에서도 Local Player의 디졸브 효과 처리가 들어갈경우 
	// UGsMeshShaderControlComponent 클래스 확장을 통하여 적용할것을 고려
	if (AGsCharacterBase* characterActor = GetCharacter())
	{
		if (false == isImmediately && false == IsBattleMode() && false == characterActor->IsOverlayMaterialChanged() && result != 0)
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
	}

	SetFairyVehicleCondition(EGsFairyVehicleCondition::USER_STATE_NORMAL, IsBattleMode());

	GMessage()->GetGameObject().SendMessage(MessageGameObject::UPDATE_USER_STATUS, nullptr);
	return result;
}

void UGsGameObjectLocalPlayer::InitializeFSM()
{
	// FSM생성 초기화
	if (_fsm)
	{
		// [B1] | ejrrb10 | LocalPlayer 의 기본 상태는 Idle 로 설정.
		// 이는 PlayableEvent 에서 Spawn Effect 나 나오는 부분을 고친 결과이다
		//_fsm->Initialize(BPStateMachineName, this, FGsStateLocalPlayerSpawn::GetInstance());
		_fsm->Initialize(BPStateMachineName, this, FGsStateLocalPlayerIdle::GetInstance());
	}

}

bool UGsGameObjectLocalPlayer::IsEnemy(UGsGameObjectBase* Target)
{
	if (Target && Target != this && Target->IsAble())
	{
		FGsGameObjectData* targetData = Target->GetData();
		return (nullptr != targetData) ? _netSyncIffhellper.IsEnemy(targetData->GetIff()) : false;

//		if (Target->IsObjectType(EGsGameObjectType::Player))
//		{
//		// 플레이어 타입만 조건 있음 그 외 모든 타입 적판단
//// 		1. PLAYER_INVADE_DEFENSE, PLAYER_INVADE_ATTACK
//// 		2. PLYAER_ENEMY, PLAYER_ALLY
//// 			2.1 파티원이라면 우선적으로 ALLY
//// 			2.2 기사단원이라면 우선적으로 ALLY
//// 			2.3 동맹 기사단원이라면 우선적으로 ALLY
//// 			2.4 경계, 적대 기사단, 카오 플레이인 경우 ENEMY
//// 			2.5 그 외 ALLY
//// 		3. PLAYER
//			bool IsAlly = SharedFunc::CheckPlayerAlly(_netSyncIffhellper.mOwnerIffFaction, targetData->GetIff(), IffFactionType::PLAYER);
//			return !IsAlly;
//		}
//		return true;
	}
	return false;
}

bool UGsGameObjectLocalPlayer::IsParty(UGsGameObjectBase* Target)
{
	if (Target)
	{
		FGsGameObjectData* targetData = Target->GetData();
		return _netSyncIffhellper.IsSameParty(targetData->GetIff());
	}
	return false;
}

bool UGsGameObjectLocalPlayer::IsGuildMemeber(UGsGameObjectBase* Target)
{
	if (Target)
	{
		FGsGameObjectData* targetData = Target->GetData();
		return _netSyncIffhellper.IsSameGuild(targetData->GetIff());
	}
	return false;
}

bool UGsGameObjectLocalPlayer::IsAllyGuildIdMember(UGsGameObjectBase* Target)
{
	if (Target)
	{
		if (FGsGameObjectDataPlayer* targetData = Target->GetCastData<FGsGameObjectDataPlayer>())
		{
			return targetData->IsAllyGuild();
		}
	}

	return false;
}

bool UGsGameObjectLocalPlayer::CanTarget(UGsGameObjectBase* Target)
{
	if (Target && Target != this)
	{
		// 기본적인 Able 검사
		if (false == Target->IsAble())
		{
			return false;
		}

		FGsGameObjectData* targetData = Target->GetData();
		return _netSyncIffhellper.IsAbleIffFactionType(targetData->GetIff(), IffCheckType::TARGET);
	}
	return false;
}

bool UGsGameObjectLocalPlayer::CanScan(UGsGameObjectBase* Target)
{
	if (Target && Target != this)
	{
		// 기본적인 Able 검사
		if (false == Target->IsAble())
		{
			return false;
		}

		FGsGameObjectData* targetData = Target->GetData();
		return _netSyncIffhellper.IsAbleIffFactionType(targetData->GetIff(), IffCheckType::SCAN);
	}
	return false;
}

bool UGsGameObjectLocalPlayer::CanInteraction(UGsGameObjectBase* Target)
{
	if (Target && Target != this)
	{
		// 기본적인 Able 검사
		if (false == Target->IsAble())
		{
			return false;
		}
		FGsGameObjectData* targetData = Target->GetData();
		return _netSyncIffhellper.CanInteraction(targetData->GetIff());
	}
	return false;
}

void UGsGameObjectLocalPlayer::SwitchSpectatorMode(bool InActive)
{
	if (InActive)
	{
		if (_MovementType == EMovementType::Spectator)
			return;
		_MovementType = EMovementType::Spectator;

		// 2021/10/21 PKT - 관전자 모드로 세팅
		FGsMovementHandlerLocalPlayer* movementHandler = GetCastMovement<FGsMovementHandlerLocalPlayer>();
		if (movementHandler)
		{
			movementHandler->ChangeSpectatorState();
			_movementHandlerspectator = movementHandler->GetSpectatorLocalPlayer();
			if(_movementHandlerspectator) _movementHandlerspectator->ResetNetLocation();
		}
	}	
	else
	{
		if (_MovementType == EMovementType::LocalPlayer)
			return;

		_MovementType = EMovementType::LocalPlayer;

		// 2021/10/21 PKT - 일반 모드로 셋팅
		FGsMovementHandlerLocalPlayer* movementHandler = GetCastMovement<FGsMovementHandlerLocalPlayer>();
		if (movementHandler)
		{
			movementHandler->SpectatorEnd();
			_movementHandlerspectator = nullptr;
		}
	}
}

void UGsGameObjectLocalPlayer::CheckInteractionCampState(UGsGameObjectBase* InCampObject)
{
	FGsInteractionHandlerLocalPlayer* localInteraction =
		this->GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
	if (nullptr != localInteraction)
	{
		if (localInteraction->IsInteraction())
		{
			if (UGsGameObjectBase* target = localInteraction->GetInteractionTarget())
			{
				if (this->IsEnemy(InCampObject) && InCampObject == target)
				{
					localInteraction->CancelInteration();
					return;
				}
			}
		}
		if (false == FGsGuildHelper::CanManageCamp(GGuild()->GetAuthority()))
		{
			localInteraction->CancelInteration();
		}
	}
}

void UGsGameObjectLocalPlayer::CheckInteractionSanctumState(UGsGameObjectBase* InSanctumObject)
{
	FGsInteractionHandlerLocalPlayer* localInteraction = GetCastInteraction<FGsInteractionHandlerLocalPlayer>();
	if (nullptr == localInteraction)
	{
		return;
	}

	if (false == localInteraction->IsInteraction())
	{
		return;
	}
	
	if (UGsGameObjectBase* target = localInteraction->GetInteractionTarget())
	{
		if (target->GetGameId() == InSanctumObject->GetGameId())
		{
			if (IsEnemy(InSanctumObject))
			{
				localInteraction->CancelInteration();
				return;
			}

			// 권한을 잃었을 때
			if (false == FGsGuildHelper::CanManageCamp(GGuild()->GetAuthority()))
			{
				localInteraction->CancelInteration();
			}
		}
	}
}

bool UGsGameObjectLocalPlayer::AddQuestNpc(class AActor* inActor)
{
	//마을, 안전 지대에서 처리 안하는 Fence는 클라에서 충돌 처리한다. 	
	FGsFenceHandler* fenceHandler = GetFenceHandler();
	if (nullptr == fenceHandler)
	{
		return false;
	}
	
	if (fenceHandler->IsInNonPVPFence(inActor->GetActorLocation()))
	{
		return false;
	}

	if(false == _questNpcArray.Contains(inActor))
	{
		_questNpcArray.Emplace(inActor);
	}

#if QUEST_NPC_DEBUG
	GSLOG(Log, TEXT("QUEST_NPC_DEBUG - Quest Npc Count : %d"), _questNpcArray.Num());
#endif	

	//0 이상이 될때 플레이어의 충돌을 꺼준다.
	if (1 == _questNpcArray.Num())
	{
#if QUEST_NPC_DEBUG
		GSLOG(Log, TEXT("QUEST_NPC_DEBUG - Active collision"));
#endif					
		_localCollisionRule = false;
		UpdateCollisionPreset();
		return true;
	}

	return false;
}

void UGsGameObjectLocalPlayer::RemoveQuestNpc(class AActor* inActor)
{
	FGsFenceHandler* fenceHandler = GetFenceHandler();
	if (nullptr == fenceHandler)
	{
		return;
	}

	//마을, 안전 지대에서 처리 안하는 Fence는 클라에서 충돌 처리한다. 		
	if (fenceHandler->IsInNonPVPFence(inActor->GetActorLocation()))
	{
		return;
	}

	if(_questNpcArray.Contains(inActor))
	{
		_questNpcArray.Remove(inActor);
	}	

#if QUEST_NPC_DEBUG
	GSLOG(Log, TEXT("QUEST_NPC_DEBUG - Quest Npc Count : %d"), _questNpcArray.Num());
#endif	

	//퀘스트 NPC 카운트가 0이 되면 플레이어의 콜리전을 켜준다.
	if (0 == _questNpcArray.Num())
	{
#if QUEST_NPC_DEBUG
		GSLOG(Log, TEXT("QUEST_NPC_DEBUG - Deactive Collision"));
#endif					
		_localCollisionRule = true;
		UpdateCollisionPreset();
	}
}

void UGsGameObjectLocalPlayer::ClearPawnDetectionCollisionOverlaps()
{
	if (_actor)
	{
		_actor->ClearPawnDetectionCollisionOverlaps();
	}
}

void UGsGameObjectLocalPlayer::SetPawnDetectionCollisionEnable(bool InFlag)
{
	if (_actor)
	{
		_actor->SetPawnDetectionCollisionEnable(InFlag);
	}
}

void UGsGameObjectLocalPlayer::UpdateRVOAvoidance(bool inDisable)
{
	// Rvo활성화 추후 OptionManager에서 제어
	AGsCharacterLocalPlayer* characterActor = Cast<AGsCharacterLocalPlayer>(GetCharacter());
	if (nullptr == characterActor)
	{
		return;
	}

	FGsGameObjectData* gameData = GetData();
	if (nullptr == gameData)
	{
		return;
	}

	const IffFactionActor& iffActor = gameData->GetIff();

	// inDisable 플레그 활성화시 강제 비활성 처리
	if (inDisable || FGsFenceHandler::IsTown(iffActor.mTerritoryType))
	{
		characterActor->SetAvoidanceConsiderationRadius(0);
	}
	else if (FGsServerOption* serverOption = GServerOption())
	{
		characterActor->SetAvoidanceConsiderationRadius(serverOption->GetPcRvoIndex());
	}
}

void UGsGameObjectLocalPlayer::OnVehicleSendTimer(float inDelayTime)
{
	if (false == _fairyVehicleInfo.IsSuccess())
	{
		_fairyVehicleInfo.ClearTimer(GetWorld());
	}
	else
	{
		float delayTime = inDelayTime;
		if (FMath::IsNearlyZero(delayTime))
		{
			const FGsSchemaVehicleConfig* vehicleConfigTable = GSFairy()->GetVehicleConfigTable();
			delayTime = vehicleConfigTable->nonCombatMoveTime;

		}
		_fairyVehicleInfo.StartTimer(GetWorld(), delayTime);

#if WITH_EDITOR
		if (showDebug)
		{
			GSLOG(Error, TEXT("[fairyVehicle] _fairyVehicleInfo.StartTimer"));
		}
#endif
	}	
}

void UGsGameObjectLocalPlayer::UpdateAllFairyVehicleCondition()
{
	// USER_STATE_NORMAL
	// 배틀 상태이면 해제
	SetFairyVehicleCondition(EGsFairyVehicleCondition::USER_STATE_NORMAL, IsBattleMode(), false);

	// EGsFairyVehicleCondition::MOVE_STATE
	// 이동 상태가 아니면 해제
	if (FGsMovementHandlerLocalPlayer* localMovementHandler = GetCastMovement<FGsMovementHandlerLocalPlayer>())
	{
		SetFairyVehicleCondition(EGsFairyVehicleCondition::MOVE_STATE, !localMovementHandler->IsMoveState(), false);
	}

	// EGsFairyVehicleCondition::VEHICLE_FAIRY
	// Vehicle 능력이 없는 페어리라면 해제
	bool isVehicleFairy = false;
	if (GSFairy())
	{
		TWeakPtr<FGsFairyItem> equipFairyData = GSFairy()->GetEquipFairyData();
		if (equipFairyData.IsValid())
		{
			const TWeakPtr<FGsFairyData> fairyData = GSFairy()->GetFairyData(equipFairyData.Pin()->GetFaryID());
			const FGsSchemaPassivitySet* passivity = fairyData.Pin()->GetByVehiclePassivity(
				GSFairy()->GetEquipFairyData().Pin()->GetCurrentLevel()
			);
			if (passivity)
			{
				FGsFairyLevelInfo levelinfo;
				fairyData.Pin()->GetFairyCurLevelPassivityList(levelinfo);
				isVehicleFairy = FGsAbnormalityHandlerBase::IsAbnormalityEffectType(
					AbnormalityEffectType::UTIL_MOUNT_VEHICLE, levelinfo._byVehicleAbnormalitySet);
			}
		}
	}
	
	SetFairyVehicleCondition(EGsFairyVehicleCondition::VEHICLE_FAIRY, !isVehicleFairy, false);

	// EGsFairyVehicleCondition::VEHICLE_NOT_ABNORMALITY
	// 이미 탑승 버프 중이면 해제
	if (FGsAbnormalityHandlerBase* abnormalityHandler = GetAbnormalityHandler())
	{
		SetFairyVehicleCondition(EGsFairyVehicleCondition::VEHICLE_NOT_ABNORMALITY,
			abnormalityHandler->IsAbnormalityEffectType(AbnormalityEffectType::UTIL_MOUNT_VEHICLE), false);
	}

	// EGsFairyVehicleCondition::VEHICLE_CREATURE_ACTION
	// 탑승 불가 상태중이면 해제
	SetFairyVehicleCondition(EGsFairyVehicleCondition::VEHICLE_CREATURE_ACTION,
		IsCrowdControlLock(CreatureActionType::CREATURE_ACTION_TYPE_MOUNT_VEHICLE), false);
}

void UGsGameObjectLocalPlayer::SetFairyVehicleCondition(EGsFairyVehicleCondition inType, bool inRemove, bool inUpdate)
{
	if (inRemove)
	{
		_fairyVehicleInfo._condition &= ~(uint16)inType;
	}
	else
	{
		_fairyVehicleInfo._condition |= (uint16)inType;
	}

	if (inUpdate)
	{
		OnVehicleSendTimer();

#if WITH_EDITOR
		if (showDebug)
		{
			// 기획 친화적으로 로그 수정
			GSLOG(Error, TEXT("[fairyVehicle] UserStatus::USER_STATE_NORMAL 평화 상태 인가? %d"),
				(bool)(_fairyVehicleInfo._condition & (uint16)EGsFairyVehicleCondition::USER_STATE_NORMAL));
			GSLOG(Error, TEXT("[fairyVehicle] 자동 이동 또는 이동 상태 인가? %d"),
				(bool)(_fairyVehicleInfo._condition & (uint16)EGsFairyVehicleCondition::MOVE_STATE));
			GSLOG(Error, TEXT("[fairyVehicle] 탈것 버프가 있는 페어리 인가? %d"),
				(bool)(_fairyVehicleInfo._condition & (uint16)EGsFairyVehicleCondition::VEHICLE_FAIRY));
			GSLOG(Error, TEXT("[fairyVehicle] 탈것 버프가 없는 상황 인가? %d"),
				(bool)(_fairyVehicleInfo._condition & (uint16)EGsFairyVehicleCondition::VEHICLE_NOT_ABNORMALITY));
			GSLOG(Error, TEXT("[fairyVehicle] 탑승 불가 상태가 아닌가? %d"),
				(bool)(_fairyVehicleInfo._condition & (uint16)EGsFairyVehicleCondition::VEHICLE_CREATURE_ACTION));
		}
#endif
	}
}

void UGsGameObjectLocalPlayer::SetForceDisconnectIdle(bool isIdleState)
{
	_forceDisconnect._isIdle = isIdleState;
	if (false == _forceDisconnect.IsActive())
	{
		_forceDisconnect.ClearTimer();
	}
}

void UGsGameObjectLocalPlayer::OnStageChange(const IGsMessageParam* inParam)
{
	if (const FGsGameStageEnterMsgParam* castParam = inParam->Cast< const FGsGameStageEnterMsgParam>())
	{
		_forceDisconnect._isSafeZone = (castParam->_type == FGsStageMode::TOWN || castParam->_type == FGsStageMode::SAFETY_ZONE);
		if (false == _forceDisconnect.IsActive())
		{
			_forceDisconnect.ClearTimer();
		}
	}
}

void UGsGameObjectLocalPlayer::OnQuestNpcSpawn(const struct IGsMessageParam* inParam)
{
#if QUEST_NPC_ACTIVE
	const FGsGameObjectMessageParam* param = inParam->Cast<const FGsGameObjectMessageParam>();
	if (nullptr == param
	|| nullptr == param->_paramOwner)
	{
		return;
	}

	UGsGameObjectNonPlayer* npcGameObject = Cast<UGsGameObjectNonPlayer>(param->_paramOwner);
	if (nullptr == npcGameObject)
	{
		return;
	}

	if (npcGameObject->GetNpcFunctionType() != NpcFunctionType::QUEST_DIALOG)
	{
		return;
	}

	AActor* npcActor = npcGameObject->GetActor();
	AActor* playerActor = GetActor();
	if (nullptr == npcActor || nullptr == playerActor)
	{
		return;
	}

	float dist = (npcActor->GetActorLocation() - playerActor->GetActorLocation()).Size();
	if (dist <= SharedFunc::GetIgnoreCollisionRangeQuestNpc())
	{
#if QUEST_NPC_DEBUG
#if WITH_EDITOR
		GSLOG(Log, TEXT("QUEST_NPC_DEBUG - UGsGameObjectLocalPlayer::OnQuestNpcSpawn - lable : %s"), *npcActor->GetActorLabel());
#endif
#endif

		AddQuestNpc(npcActor);
	}	
#endif
}

void UGsGameObjectLocalPlayer::OnQuestNpcDespawn(const struct IGsMessageParam* inParam)
{
#if QUEST_NPC_ACTIVE
	const FGsGameObjectMessageParam* param = inParam->Cast<const FGsGameObjectMessageParam>();
	if (nullptr == param
	|| nullptr == param->_paramOwner)
	{
		return;
	}

	UGsGameObjectNonPlayer* npcGameObject = Cast<UGsGameObjectNonPlayer>(param->_paramOwner);
	if (nullptr == npcGameObject)
	{
		return;
	}

	if (npcGameObject->GetNpcFunctionType() != NpcFunctionType::QUEST_DIALOG)
	{
		return;
	}

	AActor* npcActor = npcGameObject->GetActor();
	AActor* playerActor = GetActor();
	if (nullptr == npcActor || nullptr == playerActor)
	{
		return;
	}

	float dist = (npcActor->GetActorLocation() - playerActor->GetActorLocation()).Size();
	if (dist <= SharedFunc::GetIgnoreCollisionRangeQuestNpc())
	{
#if QUEST_NPC_DEBUG
#if WITH_EDITOR
		GSLOG(Log, TEXT("QUEST_NPC_DEBUG - UGsGameObjectLocalPlayer::OnQuestNpcDespawn - lable : %s"), *npcActor->GetActorLabel());
#endif
#endif

		RemoveQuestNpc(npcActor);
	}	
#endif
}


void UGsGameObjectLocalPlayer::OnEnterQuestNpc(class UPrimitiveComponent* inOverlappedComponent, class AActor* inOtherActor, class UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex, bool inbFromSweep, const FHitResult& inSweepResult)
{
#if QUEST_NPC_ACTIVE
	//check player
	AGsCharacterNonPlayer* npcActor = Cast<AGsCharacterNonPlayer>(inOtherActor);
	if (nullptr == npcActor)
	{
		return;
	}

	UGsGameObjectBase* gameObject = GSGameObject()->FindObject(npcActor, EGsGameObjectType::NonPlayer);
	if (nullptr == gameObject)
	{
		return;
	}

	UGsGameObjectNonPlayer* nonPlayerGameObject = Cast<UGsGameObjectNonPlayer>(gameObject);
	if (nullptr == nonPlayerGameObject)
	{
		return;
	}

	if (nonPlayerGameObject->GetNpcFunctionType() != NpcFunctionType::QUEST_DIALOG)
	{
		return;
	}

#if QUEST_NPC_DEBUG
#if WITH_EDITOR
	GSLOG(Log, TEXT("QUEST_NPC_DEBUG - UGsGameObjectLocalPlayer::OnEnterQuestNpc - lable : %s"), *npcActor->GetActorLabel());
#endif
#endif

	AddQuestNpc(npcActor);
#endif
}

void UGsGameObjectLocalPlayer::OnLeaveQuestNpc(class UPrimitiveComponent* inOverlappedComponent, class AActor* inOtherActor, class UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex)
{
#if QUEST_NPC_ACTIVE
	//check player
	AGsCharacterNonPlayer* npcActor = Cast<AGsCharacterNonPlayer>(inOtherActor);
	if (nullptr == npcActor)
	{
		return;
	}

	UGsGameObjectBase* gameObject = GSGameObject()->FindObject(npcActor, EGsGameObjectType::NonPlayer);
	if (nullptr == gameObject)
	{
		return;
	}

	UGsGameObjectNonPlayer* nonPlayerGameObject = Cast<UGsGameObjectNonPlayer>(gameObject);
	if (nullptr == nonPlayerGameObject)
	{
		return;
	}

	if (nonPlayerGameObject->GetNpcFunctionType() != NpcFunctionType::QUEST_DIALOG)
	{
		return;
	}

#if QUEST_NPC_DEBUG
#if WITH_EDITOR
	GSLOG(Log, TEXT("QUEST_NPC_DEBUG - UGsGameObjectLocalPlayer::OnLeaveQuestNpc - lable : %s"), *npcActor->GetActorLabel());
#endif
#endif

	RemoveQuestNpc(npcActor);
#endif
}

void UGsGameObjectLocalPlayer::OnEnterDetectionPawn(UPrimitiveComponent* inOverlappedComponent, AActor* inOtherActor, UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex, bool inbFromSweep, const FHitResult& inSweepResult)
{
	AGsCharacterRemotePlayer* remoteActor = Cast<AGsCharacterRemotePlayer>(inOtherActor);
	UGsGameObjectBase* remoteObject = (nullptr != remoteActor) ? GSGameObject()->FindObject(remoteActor, EGsGameObjectType::RemotePlayer) : nullptr;
	if (nullptr == remoteObject)
	{
		return;
	}

	FGsMovementHandlerLocalPlayer* movement = GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}

	movement->AddBlockMoveObject(remoteObject);
}

void UGsGameObjectLocalPlayer::OnleaveDetectionPawn(UPrimitiveComponent* inOverlappedComponent, AActor* inOtherActor, UPrimitiveComponent* inOtherComp, int32 inOtherBodyIndex)
{
	/**
	 * None..
	 */
}

void UGsGameObjectLocalPlayer::StartRootmotion(uint16 inDir, const FVector& inPos, const FVector& inDestPos, int inRootmotionId)
{
	Super::StartRootmotion(inDir, inPos, inDestPos, inRootmotionId);

	// 이전 동기화용 포지션 정보가 남아 있다면 Clear
	_rootmotionSyncPos = FVector::ZeroVector;
	_rootmotionSyncDir = 0;
}

void UGsGameObjectLocalPlayer::EndRootmotion(uint16 inDir, const FVector& inPos, int inRootmotionId)
{
	// Super::EndRootmotion(inDir, inPos, inRootmotionId);
#if UE_BUILD_DEBUG
// 디버그용 현재 서버와의 위치 차이 확인
// 서버에서 이동 거리
	FVector netdir = _debugRootmotionStartPos - inPos;
	float netdistance = netdir.Size();
	// 클라이언트 이동 거리
	FVector clientdir = inPos - GetLocation();
	float clientdistance = clientdir.Size();

	// 경과 시간
	float elapsedTime = (FDateTime::Now() - _debugRootmotioncurrentTime).GetTotalSeconds();

	GSLOG(Error, TEXT("Rootmotion End , Move NetDistance : %f, ClientDistance : %f,  elapsedTime : %f"),
		netdistance, clientdistance, elapsedTime);	
#endif

	// 서버에서 보내준 최종 좌표 그리기
// 	if (GSGameObject()->_isRootmotionDebugShape)
// 	{
// 		DrawDebugSphere(GetWorld(), inPos, 10.f, 50.f, FColor::Yellow, false, 2.f);
// 	}

	_rootmotionSyncPos = inPos;
	_rootmotionSyncDir = inDir;
}

void UGsGameObjectLocalPlayer::SyncRootmotionPosition()
{
	if (_rootmotionSyncPos.IsNearlyZero())
	{
		return;
	}

	// 거리 이격이 있을경우에만 동기화 처리
	// TeleportTo 함수가 현재 지형보다 높게 처리될때가 있다. 추후 확인해 볼것
	if (false == GSGameObject()->_isNotSyncRootmotion)
	{
		FVector2D targetDir2D = GetLocation2D() - FVector2D(_rootmotionSyncPos.X, _rootmotionSyncPos.Y);
		if (targetDir2D.SizeSquared() > 50.f * 50.f)
		{
			FGsMovementHandlerBase* movementhandler = GetMovementHandler();
			// 위치, 방향 즉시 동기화
			movementhandler->DirectMoveUpdate(_rootmotionSyncPos, FGsDir::angleToDir(_rootmotionSyncDir));
		}
	}

	ClearRootmotionData();
	_rootmotionSyncPos = FVector::ZeroVector;
	_rootmotionSyncDir = 0;
}

void UGsGameObjectLocalPlayer::ClientMoveUpdate(float inDelta)
{
	// 이동 연출이 진행중이라면 블록처리 끔
	BlockOtherPlayer(false);

	Super::ClientMoveUpdate(inDelta);
}

// 대화 연출 안에서 로컬 사라지는 이슈
// abnormality remove시 update visible에서
// UGsGameObjectBase::IsHidden 타면
// 대화 연출중이라 사라지게됨
// override해서 actor hidden만 뱉게 하자
bool UGsGameObjectLocalPlayer::IsHidden()
{
	AActor* actor = GetActor();
	if (actor == nullptr)
	{
		return false;
	}
	return actor->IsHidden();
}

// 예약된 데이터 삭제(현재 저장된 예약데이터 제외)
// https://jira.com2us.com/jira/browse/C2URWQ-6334
void UGsGameObjectLocalPlayer::ClearReserveDataByReserveWork(IGsReserveWork* In_currentReserveWork)
{

	TArray<IGsReserveWork*> resrveWorks = GetReserveWorks();
	for (IGsReserveWork* iter : resrveWorks)
	{
		if (iter == nullptr)
		{
			continue;
		}

		bool isClearData = false;
		// 1. 요청자가 nullptr 이거나(reserve work 외에 것들이 요청)
		// 2. 해당 요청 reserve work가 아니거나
		if(In_currentReserveWork == nullptr ||
			iter != In_currentReserveWork)
		{
			isClearData = true;
		}

		// 현제 타겟 work 빼고 삭제
		if (isClearData == true &&
			iter->IsReserveData() == true )
		{
			iter->ClearReserveData();			
		}
	}
}

// 캠프랑 히트되었다면
// 충돌체 안으로 들어온 이벤트(수호탑 내에 퀘스트 목표가 있을 때 진행 불가 이슈)
// https://jira.com2us.com/jira/browse/C2URWQ-6363
void UGsGameObjectLocalPlayer::OnCampHit(const AActor* In_camp)
{
	if (In_camp == nullptr)
	{
		return;
	}
		
	FGsCampActorMessageParam param(Cast<AGsCampBase>(In_camp));
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CAMP_HIT, &param);

	// 1. 자동 이동 중이었다면
	if (_fsm == nullptr)
	{
		return;
	}
	if (_fsm->IsState(EGsStateBase::AutoMove) == false)
	{
		return;
	}
	// 2. 퀘스트 이동인가
	FGsMovementHandlerLocalPlayer* moveHandler = GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (moveHandler == nullptr)
	{
		return;
	}
	if (moveHandler->GetStateAutoMove().GetMovementAutoContentsType() != EGsMovementAutoContentsType::QuestMove)
	{
		return;
	}
	float dist = 0.0f;
	// 3. 목적지 위치가 캠프 안에 있다면
	if(moveHandler->GetStateAutoMove().IsGolPosInTargetCamp(In_camp, dist) == false)
	{
		return;
	}
	// 4. acceptance radius만 늘려서 다시 이동
	FGsMovementStateAuto& moveStateAuto = moveHandler->GetStateAutoMove();
	const FGsAutoMoveInfo& autoMoveInfo = moveStateAuto.GetData();
	if(autoMoveInfo.IsValid())
	{
		float acceptanceRadius = 0.0f;
		if (const UGsGlobalConstant* globalConstData = GData()->GetGlobalData())
		{
			acceptanceRadius =
				globalConstData->_autoMoveCampHitAddAcceptanceRadius;
		}
		// 캠프 HIT후 이동 재시작시(예약된 정보 삭제시 사용) 
		// https://jira.com2us.com/jira/browse/C2URWQ-6363
		GMessage()->GetGameObject().SendMessage(MessageGameObject::AUTO_MOVE_CAMP_HIT_RESTART, nullptr);

		// 현재 거리에서 100 추가(무조건 성공하게 처리)
		moveStateAuto.SetAcceptanceRadius(dist + acceptanceRadius);
		moveHandler->GetStateAutoMove().Start(this, autoMoveInfo);
	}
}