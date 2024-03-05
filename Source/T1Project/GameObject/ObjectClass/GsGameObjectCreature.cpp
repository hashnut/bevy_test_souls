// Fill out your copyright notice in the Description page of Project Settings.

#include "GsGameObjectCreature.h"

#include "DrawDebugHelpers.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "Components/CapsuleComponent.h"

#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsEffectComponent.h"
#include "ActorComponentEx/GsHitEffectComponent.h"
#include "ActorComponentEx/GsHommingProjectileComponent.h"

#include "Animation/GsAnimInstanceState.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Message/MessageParam/GsAIMessageParam.h"
#include "Message/GsMessageGameObject.h"

#include "Data/GsDataContainManager.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Scan/GsScanHandler.h"

#include "GameObject/Stat/GsStatBase.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/Target/GsTargetHandlerBase.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Movement/LocalPlayer/GsMovementNetSender.h"
#include "GameObject/Customize/GsCustomizeHandler.h"
#include "GameObject/Damage/GsDamageHandlerBase.h"
#include "GameObject/UseItemEffect/GsUseItemEffectHandler.h"
#include "GameObject/Abnormality/CrowdControl/GsCrowdControlBase.h"
#include "GameObject/Parts/GsPartsHandlerBase.h"
#include "GameObject/Data/GsGameObjectDataPlayer.h"
#include "GameObject/Skill/IdleEmotion/GsIdleEmotionLocalRunner.h"

#include "Shared/Client/SharedEnums/SharedStatEnum.h"

#include "UI/UIContent/Billboard/GsUIBillboardCreature.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "DataSchema/Abnormality/Category/GsSchemaAbnormalityCategorySet.h"
#include "DataSchema/GameObject/GsSchemaShapeData.h"
#include "DataSchema/Skill/HitEffect/GsSchemaHealByTypeEffectSet.h"
#include "DataSchema/Skill/HitEffect/GsSchemaExtraTypeEffectSet.h"
#include "DataSchema/GsSchemaEnums.h"

#include "UTIL/GsDir.h"
#if WITH_EDITOR
#include "ActorComponentEx/GsDebugStateLogComponent.h"
#endif


EGsGameObjectType UGsGameObjectCreature::GetObjectType() const	{ return EGsGameObjectType::Creature; }
AActor*		      UGsGameObjectCreature::GetActor() const		{ return nullptr; }
AGsCharacterBase* UGsGameObjectCreature::GetCharacter() const	{ return (GetActor()) ? Cast<AGsCharacterBase>(GetActor()) : nullptr; }
UGsAnimInstanceState* UGsGameObjectCreature::GetAnimInstance() const { return (GetCharacter()) ? GetCharacter()->GetAnim() : nullptr; }
FGsUseItemEffectHandler* UGsGameObjectCreature::GetUseItemEffectHandler() { return GetGameObjectHandler<FGsUseItemEffectHandler>(GameObjectHandlerType::USE_ITEM_EFFECT); }


//FActorDeleteSignature* UGsGameObjectCreature::GetDeleteSignature() const 
//{
//	return (GetCharacter()) ? &GetCharacter()->OnActorDelete : nullptr;
//}

UGsGameObjectCreature::UGsGameObjectCreature() : Super(), FGsMovementRunner(this)
{
}

UGsGameObjectCreature::~UGsGameObjectCreature()
{
}

void UGsGameObjectCreature::Initialize(const FGsSpawnParam* SpawnParam)
{
	Super::Initialize(SpawnParam);

	SET_FLAG_TYPE(_objectType, UGsGameObjectCreature::GetObjectType());

	if (FGsGameObjectDataCreature* creatureData = GetCastData<FGsGameObjectDataCreature>())
	{
		if (TSharedPtr<FGsStatBase> statBase = CreateCreatureStat())
		{
			_changeStatHandle = statBase->OnChangeStatDelegate.AddUObject(this, &UGsGameObjectCreature::OnChangedStat);
		}
	}	
	_spawnReason = SpawnParam->_spawnReason;

	// Fsm Initialize는 각 클래스에서 처리
	if (nullptr == _fsm)
	{
		_fsm = CreateBaseFSM();
		InitializeFSM();
	}
}

void UGsGameObjectCreature::Update(float Delta)
{
	Super::Update(Delta);

	if (_fsm) { _fsm->Update(this, Delta); }

	if (_moveLocationInfo.IsValid())
	{
		ClientMoveUpdate(Delta);
	}

	// 루트 모션 자체 계산하여 처리
	if (IsRootmotion(true))
	{
		UpdateRootmotion(Delta);
	}
}

bool UGsGameObjectCreature::IsDeadState() const
{
	if (FGsGameObjectStateManager* fsm = GetBaseFSM())
	{
		return	fsm->IsState(EGsStateBase::Dying) ||
				fsm->IsState(EGsStateBase::Dead)  ||
				fsm->IsState(EGsStateBase::RunAway);
	}

	return true;
}

bool UGsGameObjectCreature::IsBattleMode() const
{
	if (FGsGameObjectDataCreature* creatureData = GetCastData<FGsGameObjectDataCreature>())
	{
		return creatureData->GetUserStatus() != UserStatus::USER_STATE_NORMAL;
	}
	return false;
}

EGsStateBase UGsGameObjectCreature::GetStateType() const
{
	if (FGsGameObjectStateManager* fsm = GetBaseFSM())
	{
		return	fsm->GetCurrentState();
	}

	return EGsStateBase::None;
}

void UGsGameObjectCreature::PrintStateType() const
{
	EGsStateBase stateType = GetStateType();
	FString stateTypeName = FGsTextUtil::GetStringFromEnum(TEXT("EGsStateBase"), stateType);
	GSLOG(Log, TEXT("state type name : %s"), *stateTypeName);
}

//action not battlemode idle, no damgae abnormality
bool UGsGameObjectCreature::IsPeacefulIdle()
{
	bool peaceful = false;
	if (FGsGameObjectStateManager* fsm = GetBaseFSM())
	{
		if (auto curState = fsm->CurrentState())
		{
			peaceful = curState->GetStateID() == static_cast<int>(EGsStateBase::Idle) || 
					   curState->GetStateID() == static_cast<int>(EGsStateBase::Social);
			peaceful = peaceful ? !IsBattleMode() : false;
		}
		else
		{
			peaceful = Super::IsPeacefulIdle();
		}
	}

	if (FGsAbnormalityHandlerBase* abnormalityHandler = peaceful ? GetAbnormalityHandler() : nullptr)
	{
		peaceful = abnormalityHandler->IsDamagedByAbnormality() == false;
	}

	return peaceful;
}

bool UGsGameObjectCreature::IsCrowdControlLock()
{
	FGsAbnormalityHandlerBase* abnormalityHandler = GetAbnormalityHandler();
	if (abnormalityHandler)
	{
		return abnormalityHandler->GetCrowdControl()->IsCreatureAction();
	}
	return false;
}

bool UGsGameObjectCreature::IsCrowdControlLock(CreatureActionType creatureActionType)
{
	FGsAbnormalityHandlerBase* abnormalityHandler = GetAbnormalityHandler();
	if (abnormalityHandler)
	{
		return abnormalityHandler->GetCrowdControl()->IsCreatureActionType(creatureActionType);
	}
	return false;
}

bool UGsGameObjectCreature::IsRootmotion(bool isUpdate) const
{
	if (isUpdate)
	{
		return nullptr != _rootmotionInfo._rootmotionTable && _rootmotionInfo._rootmotionLock == false && 
			_rootmotionInfo._isRootmotionUpdate;
	}

	return nullptr != _rootmotionInfo._rootmotionTable && _rootmotionInfo._rootmotionLock == false;
}

void UGsGameObjectCreature::SetRootmotionLock(bool inIsLock)
{
	_rootmotionInfo._rootmotionLock = inIsLock;
}

void UGsGameObjectCreature::SetRootmotionIgnoreNavData(bool inIsIgnore)
{
	_rootmotionInfo._rootmotionIgnoreNavData = inIsIgnore;
}

bool UGsGameObjectCreature::IsAble()
{
	if (false == Super::IsAble())
	{
		return false;
	}

	if (IsZeroHP())
	{
		return false;
	}

	// 숨은 상태
	if (FGsAbnormalityHandlerBase* abnormality = GetAbnormalityHandler())
	{
		if (false == abnormality->IsTargetedAvailableState())
		{
			return false;
		}
	}
	return true;
}

void UGsGameObjectCreature::InitializeActor(AActor* Owner)
{
	check(Owner);
	Super::InitializeActor(Owner);	

	AGsCharacterBase* characterActor = Cast<AGsCharacterBase>(Owner);

	if (FGsGameObjectDataCreature* data = GetCastData<FGsGameObjectDataCreature>())
	{
		if(const FGsSchemaCreatureCommonInfo* commonTable = GetCommonInfoTable())
		{
			data->SetScaledCapsuleRadius(commonTable->cylinderRadius);
			data->SetScaledCapsuleHalfHeight(commonTable->cylinderHeight);
			data->SetApproachDistanceSquared(commonTable->cylinderRadius * commonTable->cylinderRadius);
		}
		else if(characterActor)
		{			

			UCapsuleComponent* capsule = characterActor->GetCapsuleComponent();
			if (nullptr != capsule)
			{
				data->SetScaledCapsuleRadius(capsule->GetUnscaledCapsuleRadius() * Owner->GetRootComponent()->GetComponentScale().X);
				//GSLOG(Log, TEXT("ScaledCapsuleRadius [%.2f], [%.2f]"), capsule->GetUnscaledCapsuleRadius(), inActor->GetRootComponent()->GetComponentScale().X);
				data->SetScaledCapsuleHalfHeight(capsule->GetUnscaledCapsuleHalfHeight() * Owner->GetRootComponent()->GetComponentScale().Z);
				data->SetApproachDistanceSquared(data->GetScaledCapsuleRadius() * data->GetScaledCapsuleRadius());
			}
		}
	}

	if (characterActor)
	{
		_fsm->InitializeActor();

		SetPartySharedTargetActive(false);

		if (UGsAnimInstanceState* anim = characterActor->GetAnim())
		{
			if (const FGsSchemaCreatureCommonInfo* commoninfo = GetCommonInfoTable())
			{
				anim->InitMoveSpeedValue(commoninfo->walkSpeed, commoninfo->moveSpeed);
			}
		}

		//bak1210 : CreateSubClass에서 옮김
		if (UGsEffectComponent* effComp = characterActor->GetEffectComponent())
		{
			effComp->FindAttachTarget();
		}
		characterActor->InitMeshShaderComponent(GetCommonInfoTable());
		
		// 유도체 히트이벤트 바인딩
		if (UGsHommingProjectileComponent* hommingComponent = characterActor->GetHommingProjectile())
		{
			_homingProjectileHandle = hommingComponent->OnHitHomingProjectile.AddUObject(
				this, &UGsGameObjectCreature::OnHitHomingProjectile);
		}

		UpdateByTarget(_targetMakerVisible, _targetInfo.isTarget, _targetInfo.isFirstTarget, _targetInfo.isEnemy);

	}

	ApplyMoveSpeedFromStatInfo();
}

void UGsGameObjectCreature::BindMessage()
{
	Super::BindMessage();

	if (GetTargetHandler())
	{
		FGsMessageHolder* msg = GMessage();
		AddMessageDelegate(msg->GetGameObject().AddUObject(MessageGameObject::COMMON_DIE,
			this, &UGsGameObjectCreature::OnTargetClear));
		AddMessageDelegate(msg->GetGameObject().AddUObject(MessageGameObject::COMMON_DESPAWN,
			this, &UGsGameObjectCreature::OnTargetClear));
		AddMessageDelegate(msg->GetGameObject().AddUObject(MessageGameObject::COMMON_TARGET_CLEAR,
			this, &UGsGameObjectCreature::OnTargetClear));
	}
}

void UGsGameObjectCreature::OnTargetClear(const struct IGsMessageParam* inParam)
{
	const FGsGameObjectMessageParam* castParam = inParam->Cast<const FGsGameObjectMessageParam>();
	FGsTargetHandlerBase* targetHandler = GetTargetHandler();
	// 내가 타겟으로 잡은 녀석이 주금 or 디스폰
	targetHandler->ClearTarget(castParam->_paramOwner);
}


void UGsGameObjectCreature::UpdateTargetMark(bool inVisible, bool inIsFirst)
{
}

void UGsGameObjectCreature::UpdateByTarget(bool inVisibleTargetMaker, bool isTarget, bool inFirst, bool isEnemy)
{
	if (AGsCharacterBase* actor = Cast<AGsCharacterBase>(GetActor()))
	{
		UpdateTargetMark(inVisibleTargetMaker, inFirst);
		
#pragma todo ("Outside Actor Member call change inside!")
		UGsUIBillboardCreature* widget = actor->GetWidget();
		if (nullptr != widget)
		{
			widget->OnTargeting(isTarget, this, inFirst, isEnemy);
		}
	}
	else
	{
		_targetMakerVisible = inVisibleTargetMaker;
		_targetInfo.isFirstTarget = inFirst;
		_targetInfo.isEnemy = isEnemy;
		_targetInfo.isTarget = isTarget;
	}
}

void UGsGameObjectCreature::StartMoveLocation(const FVector& inCurrPos, const FVector& inDestPos, int inMoveSpeed)
{
	float distance = (GetLocation() - inDestPos).Size();
	float duration = distance / inMoveSpeed;
	StartMoveLocation(inCurrPos, inDestPos, duration);
}

void UGsGameObjectCreature::StartMoveLocation(const FVector& inCurrPos, const FVector& inDestPos, float inDuration)
{
	// 같은 좌표로 허용된 정보라면 DirectMoveUpdate 처리 생략
	const FVector dir = inCurrPos - inDestPos;
	float sameDistance = GData()->GetGlobalData()->_directMoveSameDistance;
	if (dir.SizeSquared2D() < sameDistance * sameDistance)
	{
		return;
	}

	if (FMath::IsNearlyZero(inDuration))
	{
		if (FGsMovementHandlerBase* movementHandler = GetMovementHandler())
		{
			movementHandler->DirectMoveUpdate(inDestPos, GetRotation().Vector());
		}		
	}
	else
	{
		_moveLocationInfo._stratPos = GetLocation2D();
		_moveLocationInfo._destPos = FVector2D(inDestPos.X, inDestPos.Y);
		_moveLocationInfo._duration = inDuration;
		_moveLocationInfo._elapsed = 0.f;
		_moveLocationInfo._isUpdate = true;
	}
}

void UGsGameObjectCreature::StartRootmotion(uint16 inDir, const FVector& inPos, const FVector& inDestPos, int inRootmotionId)
{
	FGsMovementHandlerBase* movementHandler = GetMovementHandler();
	if (nullptr != movementHandler)
	{
		check(movementHandler);
		movementHandler->Stop();
	}

	_rootmotionInfo._rootmotionTable = UGsTableUtil::FindRowById<UGsTableRootmotionSet, FGsSchemaRootmotionSet>(inRootmotionId);
	if (nullptr == _rootmotionInfo._rootmotionTable)
	{
		GSLOG(Error, TEXT("StarRootmotion failed not found table SkillReactionid : %d"), inRootmotionId);
		return;
	}

	// 같은 좌표로 허용된 정보라면 DirectMoveUpdate 처리 생략
	const FVector dir = inPos - inDestPos;
	float sameDistance = GData()->GetGlobalData()->_directMoveSameDistance;
	if (dir.SizeSquared2D() < sameDistance * sameDistance)
	{
		return;
	}

	// 위치, 방향 즉시 동기화
	movementHandler->DirectMoveUpdate(inPos, FGsDir::angleToDir(inDir));

	// 이동 하려는 방향과 서버에서준 DesPos를 향한 방향이 반대일경우 스킵
	const FGsSchemaRootmotionInfo& lastRootmotionInfo = _rootmotionInfo._rootmotionTable->timeLine.Last();
	FVector rootmotionDir = GetRotation().RotateVector(FGsDir::angleToDir(lastRootmotionInfo.Angle));
	FVector destDir = inDestPos - GetLocation();
	float v = FVector::DotProduct(rootmotionDir, destDir);
	if (v < 0.f)
	{
		return;
	}

	// 이동 거리가 실제 데이터와 많이 다르다면 스킵	
	float marginSqr = lastRootmotionInfo.Distance + 88.f;
	if (destDir.SizeSquared() > (marginSqr * marginSqr))
	{
		GSLOG(Error, TEXT("StarRootmotion failed not sync move Distance SkillReactionid : %d realDistance : %f, netDistance : %f"),
			inRootmotionId, lastRootmotionInfo.Distance, destDir.Size());
		return;
	}

	// 이동 하려는 방향과 목적 좌표의 방향이 다르다면 스킵
	// FVector rootmotionDestDir = .RotateAngleAxis()


	// 루트모션 활성 설정
	_rootmotionInfo._isRootmotionUpdate = true;

	AGsCharacterBase* characterActor = GetCharacter();
	// 루트 모션 시작전 현재 tm 저장
	if (characterActor)
	{
		_rootmotionInfo._rootmotionTm = characterActor->GetTransform();
		float halfHeight = characterActor->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		ANavigationData* navData = NavSys->GetDefaultNavDataInstance();
		float extentHeight = navData->GetConfig().DefaultQueryExtent.Z;
		// 네비 데이터 유효 판정 영역의 범위를 넘어가는 경우 높이 보정
		// @See : FPImplRecastNavMesh::Raycast
		if (halfHeight > extentHeight)
		{
			FVector extentLocation = _rootmotionInfo._rootmotionTm.GetLocation();
			extentLocation.Z = extentLocation.Z - (halfHeight - extentHeight);
			_rootmotionInfo._rootmotionTm.SetLocation(extentLocation);
		}

		// 유효 목적 좌표 설정
		_rootmotionInfo._destPos = inDestPos;

		if (GSGameObject()->_isRootmotionDebugShape)
		{
			DrawDebugSphere(GetWorld(), inDestPos, 10.f, 50.f, FColor::Orange, false, 1.f);
		}

		// 충돌 처리 설정
// 		if (false == _rootmotionInfo._rootmotionTable->collisionDisable)
// 		{
// 			UCapsuleComponent* capsulComponet = characterActor->GetCapsuleComponent();
// 			
// 			_rootmotionInfo._collisionQueryParam.ClearIgnoredActors();
// 			_rootmotionInfo._collisionQueryParam.ClearIgnoredComponents();
// 			_rootmotionInfo._collisionQueryParam.AddIgnoredActor(characterActor);
// 			_rootmotionInfo._collisionObjectQueryParam.AddObjectTypesToQuery(ECC_GameTraceChannel1); // PawnBoss
// 			_rootmotionInfo._collisionObjectQueryParam.AddObjectTypesToQuery(ECC_GameTraceChannel9); // OtherCollision
// 			_rootmotionInfo._collisionObjectQueryParam.AddObjectTypesToQuery(ECC_GameTraceChannel10); // LocalBlockOther
// 			_rootmotionInfo._collisionObjectQueryParam.AddObjectTypesToQuery(ECC_GameTraceChannel15); // Camp
// 
// 			_rootmotionInfo._collisionShape = capsulComponet->GetCollisionShape();
// 		}
	}

#if UE_BUILD_DEBUG
	_debugRootmotionStartPos = inPos;
	_debugRootmotioncurrentTime = FDateTime::Now();
	
#endif
	if (GSGameObject()->_isRootmotionDebugShape)
	{
		// 현재 방향 확인
		DrawDebugDirectionalArrow(GetWorld(), _rootmotionInfo._rootmotionTm.GetLocation(), _rootmotionInfo._destPos,
			30.f, FColor::Red, false, 1.f);
	}
}

void UGsGameObjectCreature::EndRootmotion(uint16 inDir, const FVector& inPos, int inRootmotionId)
{
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
	if (GSGameObject()->_isRootmotionDebugShape)
	{
		// 서버에서 보내준 최종 좌표 그리기
		DrawDebugSphere(GetWorld(), inPos, 10.f, 50.f, FColor::Yellow, false, 1.f);
	}

	ClearRootmotionData();
}

void UGsGameObjectCreature::ClearRootmotionData()
{
	_rootmotionInfo._rootmotionTable = nullptr;
	_rootmotionInfo._isRootmotionUpdate = false;
	_rootmotionInfo._rootmotionLock = false;
	_rootmotionInfo._rootmotionIgnoreNavData = false;
}

void UGsGameObjectCreature::OnChangedStat(const FGsStatInfo& inStateInfo)
{
	if (StatType::ATTACK_SPEED_RATE == inStateInfo.GetStatType())
	{
		if (FGsSkillHandlerBase* skillhandler = GetSkillHandler())
		{
			// 1. 만분율 변환
			float speedRate = FGsStatHelper::ConvertValueToPermyriad(&inStateInfo);

			// 2. 하드캡 적용
			speedRate = FGsStatHelper::ApplyHardcapSpeedRateStatType(inStateInfo.GetStatType(), speedRate);

			// 3. 적용
			skillhandler->SetAttackSpeedRate(speedRate);
		}
	}
	else if (StatType::CASTING_SPEED_RATE == inStateInfo.GetStatType())
	{
		if (FGsSkillHandlerBase* skillhandler = GetSkillHandler())
		{
			// 1. 만분율 변환
			float speedRate = FGsStatHelper::ConvertValueToPermyriad(&inStateInfo);

			// 2. 하드캡 적용
			speedRate = FGsStatHelper::ApplyHardcapSpeedRateStatType(inStateInfo.GetStatType(), speedRate);

			// 3. 적용
			skillhandler->SetCastSpeedRate(speedRate);
		}
	}
	else if (StatType::MOVE_SPEED == inStateInfo.GetStatType())
	{
		ApplyMoveSpeedFromStatInfo();
	}

	if (AGsCharacterBase* character = GetCharacter())
	{
		if (UGsUIBillboardCreature* widget = character->GetWidget())
		{
			widget->OnChangedStat(inStateInfo);
		}
	}
}

void UGsGameObjectCreature::OnChandeDisplayStat(const FGsStatInfo& inStateInfo)
{
	if (AGsCharacterBase* character = GetCharacter())
	{
		if (UGsUIBillboardCreature* widget = character->GetWidget())
		{
			//widget->OnDisplayData(inStateInfo);
		}
	}
}

void UGsGameObjectCreature::OnHitHomingProjectile(class AActor* Caster, int32 SkillNotifyId)
{
	if (SkillNotifyId != 0)
	{
		FGsDamageHandlerBase* damagehandler = GetDamageHandler();
		damagehandler->FindExcuteDamage(SkillNotifyId);
	}
}

void UGsGameObjectCreature::ClientMoveUpdate(float inDelta)
{
	if (FGsMovementHandlerBase* movementHandler = GetMovementHandler())
	{
		if (_moveLocationInfo._duration > _moveLocationInfo._elapsed)
		{
			_moveLocationInfo._elapsed += inDelta;
			float rate = FMath::Min(1.f, _moveLocationInfo._elapsed / _moveLocationInfo._duration);
			FVector2D velocity = (_moveLocationInfo._destPos - _moveLocationInfo._stratPos) * rate;
			FVector updatePos = FVector(_moveLocationInfo._stratPos + velocity, GetLocation().Z);
			movementHandler->DirectMoveUpdate(updatePos, GetRotation().Vector());
		}
		else
		{
			// 최종 위치 갱신
			_moveLocationInfo._duration = 0.f;
			_moveLocationInfo._isUpdate = false;
			movementHandler->DirectMoveUpdate(FVector(_moveLocationInfo._destPos, GetLocation().Z), GetRotation().Vector());
		}
	}
}

void UGsGameObjectCreature::UpdateRootmotion(float inDelta)
{
	UGsAnimInstanceState* anim = GetAnimInstance();
	if (nullptr == anim)
	{
		ClearRootmotionData();
		return;
	}

	const FAnimMontageInstance* MontageInstance = anim->GetRootMotionMontageInstance();
	if (nullptr == MontageInstance || false == MontageInstance->IsActive())
	{
		ClearRootmotionData();
		return;
	}

	float prevPos = MontageInstance->GetPreviousPosition();
	// 이동량 얻기
	FTransform tm = MontageInstance->Montage->ExtractRootMotionFromTrackRange(prevPos,
		MontageInstance->GetPosition());

	AGsCharacterBase* characterActor = GetCharacter();
	FQuat meshRelativeRot = characterActor->GetMesh()->GetRelativeRotation().Quaternion();
	// 스켈레탈 메시 기준으로 제작되어 이동량은 변환후 반영, 회전 변화량은 바로 반영
	FTransform deltaTm(tm.GetRotation(), meshRelativeRot.RotateVector(tm.GetLocation()));
	// 현재 정보에 반영
	FTransform::Multiply(&tm, &deltaTm, &_rootmotionInfo._rootmotionTm);

// 	GSLOG(Error, TEXT("rootmotion Debug : PrePos : %f  Pos : %f  X : %f  Y: %f"), prevPos, 
// 		MontageInstance->GetPosition(), deltaTm.GetLocation().X, deltaTm.GetLocation().Y);

	FGsMovementHandlerBase* movementHandler = GetMovementHandler();
	if (_rootmotionInfo._rootmotionIgnoreNavData)
	{
		movementHandler->DirectMoveUpdate(tm.GetLocation(), tm.Rotator());
		//정보 갱신
		_rootmotionInfo._rootmotionTm = tm;
		if (GSGameObject()->_isRootmotionDebugShape)
		{
			DrawDebugSphere(GetWorld(), tm.GetLocation(), 10.f, 100, FColor::Green, false, 1.f);
		}
	}
	else
	{
		bool isMoveUdpate = false;
		FVector prevDir = (_rootmotionInfo._destPos - _rootmotionInfo._rootmotionTm.GetLocation()).GetSafeNormal();
		FVector currDir = (_rootmotionInfo._destPos - tm.GetLocation()).GetSafeNormal();

		float v = FVector2D::DotProduct(FVector2D(prevDir), FVector2D(currDir));
		if (v > 0.f)
		{
			FVector outDetectPos;
			FVector outHitNormal;
			// 네비 데이터 기준으로만 이동 체크
			isMoveUdpate = FGsMovementNetSender::CheckNavMeshRayCast(characterActor,
				_rootmotionInfo._rootmotionTm.GetLocation(), tm.GetLocation(), outDetectPos, outHitNormal);

			// 캐릭터 충돌 체크
// 			if (false == _rootmotionInfo._rootmotionTable->collisionDisable && true == isMoveUdpate)
// 			{
// 				// FHitResult outResult;
// 				if (GetWorld()->SweepTestByObjectType(_rootmotionInfo._rootmotionTm.GetLocation(),
// 					tm.GetLocation(), tm.Rotator().Quaternion(), _rootmotionInfo._collisionObjectQueryParam,
// 					_rootmotionInfo._collisionShape, _rootmotionInfo._collisionQueryParam))
// 				{
// 					isMoveUdpate = false;
// 				}
// 			}
		}
		else
		{
			if (GSGameObject()->_isRootmotionDebugShape)
			{
				DrawDebugSphere(GetWorld(), tm.GetLocation(), 10.f, 100, FColor::Black, false, 1.f);
			}

			if ((tm.GetLocation() - _rootmotionInfo._destPos).SizeSquared() > 100.f)
			{
				isMoveUdpate = true;
			}

			tm.SetLocation(_rootmotionInfo._destPos);
		}

		if (isMoveUdpate)
		{
			movementHandler->DirectMoveUpdate(tm.GetLocation(), tm.Rotator());
			// 성공한 경우에만 정보 갱신
			_rootmotionInfo._rootmotionTm = tm;
			if (GSGameObject()->_isRootmotionDebugShape)
			{
				DrawDebugSphere(GetWorld(), tm.GetLocation(), 10.f, 100, FColor::Green, false, 1.f);
			}
		}
		else
		{
			// 실패할 경우 회전 정보만 반영
			characterActor->SetActorRotation(tm.Rotator());
			if (GSGameObject()->_isRootmotionDebugShape)
			{
				// 실패할 경우 확인을 위해 다른 디버그 Shpae 정보를 출력
				DrawDebugSphere(GetWorld(), tm.GetLocation(), 10.f, 100, FColor::Red, false, 1.f);
			}
		}
	}
}

FGsGameObjectData* UGsGameObjectCreature::CreateData(const FGsSpawnParam* SpawnParam)
{
	return new FGsGameObjectDataCreature(SpawnParam);
}

TSharedPtr<FGsSkillHandlerBase> UGsGameObjectCreature::CreateSkill()
{ 
	return MakeShareable(new FGsSkillHandlerBase()); 
}

TSharedPtr<FGsPartsHandlerBase> UGsGameObjectCreature::CreateParts()
{ 
	return MakeShareable(new FGsPartsHandlerBase()); 
}

TSharedPtr<FGsTargetHandlerBase> UGsGameObjectCreature::CreateTarget()
{
	return MakeShareable(new FGsTargetHandlerBase());
}

FGsGameObjectStateManager* UGsGameObjectCreature::CreateBaseFSM()
{ 
	return new FGsGameObjectStateManager(); 
}

TSharedPtr<FGsMovementHandlerBase> UGsGameObjectCreature::CreateMovement()
{ 
	return MakeShareable(new FGsMovementHandlerBase()); 
}

TSharedPtr<FGsCustomizeHandler> UGsGameObjectCreature::CreateCustomize()
{
	return MakeShareable(new FGsCustomizeHandler());
}

TSharedPtr<FGsAbnormalityHandlerBase> UGsGameObjectCreature::CreateAbnormality()
{
	return MakeShareable(new FGsAbnormalityHandlerBase());
}

TSharedPtr<FGsStatBase> UGsGameObjectCreature::CreateCreatureStat()
{
	return nullptr;
}

TSharedPtr<FGsDamageHandlerBase> UGsGameObjectCreature::CreateDamageHandler()
{
	return MakeShareable(new FGsDamageHandlerBase());
}

TSharedPtr<FGsUseItemEffectHandler> UGsGameObjectCreature::CreateUseItemEffectHandler()
{
	return MakeShareable(new FGsUseItemEffectHandler());
}

void UGsGameObjectCreature::CreateSubClass()
{	
	//// Fsm Initialize는 각 클래스에서 처리
	//if (nullptr == _fsm)
	//{
	//	_fsm = CreateBaseFSM();
	//}

	Super::CreateSubClass();

	AppendInitialize(CreateTarget());
	AppendInitialize(CreateAbnormality());	
	AppendInitialize(CreateUseItemEffectHandler());
	AppendInitialize(CreateDamageHandler());
}

void UGsGameObjectCreature::RemoveSubClass()
{
	// 디버프 걸림 연출 끝처리
	if (AGsCharacterBase* character = GetCharacter())
	{
		if (UGsEffectComponent* effComp = character->GetEffectComponent())
		{
			effComp->OnDebuffDeactive();
		}

		if (_homingProjectileHandle.IsValid())
		{
			if (UGsHommingProjectileComponent* hommingComponent = character->GetHommingProjectile())
			{
				hommingComponent->OnHitHomingProjectile.Remove(_homingProjectileHandle);
			}
		}
	}

	ClearRootmotionData();

	Super::RemoveSubClass();

	if (nullptr != _fsm)
	{
		_fsm->Finalize();
		delete _fsm;
		_fsm = nullptr;
	}
}

void UGsGameObjectCreature::NetSkillDamagedAck(PD::SC::PKT_SC_SKILL_DAMAGED_READ* Packet)
{
	if (Packet)
	{
		UGsGameObjectBase* casterObject = GSGameObject()->FindObject(EGsGameObjectType::Creature, Packet->CasterId());
		UGsGameObjectBase* subCasterObject = nullptr;
		// 프로젝타일 처리(mMediumId 값이 있으면 프로젝타일 정보)
		if (Packet->MediumId() != 0)
		{
			subCasterObject = GSGameObject()->FindObject(EGsGameObjectType::Base, Packet->MediumId());
		}
		OnHit(casterObject, subCasterObject, Packet->SkillId(), Packet->SkillNotifyId(),
			(HitResultType)Packet->HitResultType(), (DamageByType)Packet->DamageByType(), 
			DamageStatType::MAX, HealByType::MAX, Packet->TotalDamage(), 
			PassivityOwnerType::MAX, Packet->NoDamage());

		// 공격자 Localplayer 대상
		if (casterObject && casterObject->IsObjectType(EGsGameObjectType::LocalPlayer))
		{
			GSkill()->NetSkillDamaged(Packet);
		}
	}
}

void UGsGameObjectCreature::NetSkillDamagedAck(PD::SC::PKT_SC_PASSIVITY_DAMAGED_READ* Packet)
{
	// 일단 PKT_SC_SKILL_DAMAGED_READ 정보를 받았을때와 동일한 로직으로 처리
	if (Packet)
	{
		UGsGameObjectBase* casterObject = GSGameObject()->FindObject(EGsGameObjectType::Creature, Packet->CasterId());		
		OnHit(casterObject, nullptr, Packet->PassivityId(), Packet->SkillNotifyId(),
			(HitResultType)Packet->HitResultType(), (DamageByType)Packet->DamageByType(), 
			DamageStatType::MAX, HealByType::MAX, Packet->TotalDamage(), Packet->PassivityOwnerType());
	}
}

void UGsGameObjectCreature::NetSkillHealAck(PD::SC::PKT_SC_SKILL_HEALED_READ* Packet)
{
	if (Packet)
	{
		UGsGameObjectBase* casterObject = GSGameObject()->FindObject(EGsGameObjectType::Creature, Packet->CasterId());
		OnHit(casterObject, nullptr, Packet->SkillId(), Packet->SkillNotifyId(), HitResultType::HEAL, DamageByType::SKILL, 
			Packet->HealStatType(),	Packet->HealByType(), Packet->HpDeltaValue());
	}
}

void UGsGameObjectCreature::NetAbnormalityResultAck(PD::SC::PKT_SC_ADD_ABNORMALITY_RESULT_READ* Packet)
{
	if (Packet)
	{
		UGsGameObjectBase* casterObject = GSGameObject()->FindObject(EGsGameObjectType::Creature, Packet->CasterId());		
		// 캐스터가 LocalPlayer인 경우만 정보만 표시
		if (false == casterObject->IsObjectType(EGsGameObjectType::LocalPlayer))
		{
			return;
		}

		// 이뮨 처리 확인
		if (Packet->HitResultType() == HitResultType::IMMUNE_DAMAGE)
		{
			SetDealScroll(UGsBlueprintFunctionLibraryUI::GetDealScrollKey(Packet->HitResultType(), true),
				TEXT(""), FVector::ZeroVector);
		}
		else if(Packet->HitResultType() == HitResultType::IMMUNE_ABNORMALITY)
		{
			const FGsSchemaAbnormalityCategorySet* categorySet = UGsTableUtil::FindRowById<UGsTableAbnormalityCategorySet,
				FGsSchemaAbnormalityCategorySet>(Packet->AbnormalityCategoryId());
			if (categorySet)
			{
				SetDealScroll(categorySet->immuneDealScrollTextTarget, TEXT(""), FVector::ZeroVector);
			}
		}
	}
}

void UGsGameObjectCreature::OnDie()
{
	Super::OnDie();

	_fsm->ProcessEvent(EGsStateBase::Dying);

	FGsDamageHandlerBase* damageHandler = GetDamageHandler();
	if (damageHandler == nullptr)
	{
		return;
	}
	damageHandler->Clear();

	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	if (nullptr == gameFlow)
	{
		return;
	}

	FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
	if (nullptr == contents)
	{
		return;
	}
	
	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	if (nullptr == hud)
	{
		return;
	}

	FGsScanHandler* scanHandler = hud->GetScanHandler();
	if (scanHandler == nullptr)
	{
		return;
	}	

	if (AGsCharacterBase* characterActor = GetCharacter())
	{
		// 이펙트 컴퍼넌트 비활성
		if (UGsEffectComponent* effectComponent = characterActor->GetEffectComponent())
		{
			effectComponent->ClearResource();
		}
	}

	scanHandler->RemoveScanMemberInfo(GetGameId());

	OnOverlayMaterialChange(nullptr);
	OnRestorePolymorph();
	
	FGsAIReserveParam aiReserveClearParam(EGsAIActionType::ATTACK_ANYONE, this);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::AI_CLEAR_RESERVE_JOB, &aiReserveClearParam);
}

bool UGsGameObjectCreature::OnHit(UGsGameObjectBase* Striker, UGsGameObjectBase* SubStriker, uint32 StriketSkillId,
	uint32 StrikerNotifyId, HitResultType ResultType, DamageByType DamageType, DamageStatType DamageStat, HealByType HealType,
	int Damage, PassivityOwnerType PassivityOwner, bool IsNoDamage)
{
	Super::OnHit(Striker, SubStriker, StriketSkillId, StrikerNotifyId, ResultType, DamageType, DamageStat, HealType,
		Damage, PassivityOwner, IsNoDamage);

	AGsCharacterBase* actorCharacter = GetCharacter();
	if (nullptr == actorCharacter)
	{
		return false;
	}

	// hit 모션 및 연출만 스킵 조건 검사
	bool ignoreHitmotion = (ResultType == HitResultType::HEAL || true == IsNoDamage);
	// hit Effect 처리 안함 조건 (이조건은 hit Motion 연출 까지 포함되어있음)
	bool ignoreHitEffect = (HitResultType::DODGE == ResultType ||
		HitResultType::IMMUNE_DAMAGE == ResultType ||
		DamageByType::OVERTIME == DamageType);

	if (Striker != nullptr)
	{
		const FGsSchemaSkillSet* skillData =
			UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(StriketSkillId);

		// 때린넘이 플레이어인가
		bool isPlayerAttacker = Striker->IsObjectType(EGsGameObjectType::Player);
		if (isPlayerAttacker)
		{
			// 정령탄 연출 출력 스킬인가
			if (skillData != nullptr &&
				skillData->spiritShotEffectShow == true &&
				DamageType == DamageByType::SKILL &&
				// not miss 
				ResultType != HitResultType::DODGE
				)
			{
				if (FGsGameObjectDataPlayer* data = Striker->GetCastData<FGsGameObjectDataPlayer>())
				{
					// 천마석 사용하는가
					if (data->GetIsActiveSpiritShot() == true)
					{
						if (FGsUseItemEffectHandler* useItemEffectHandler = GetUseItemEffectHandler())
						{
							// 천마석 실제 레벨을 반영해 주어야 한다
							useItemEffectHandler->OnUseItemHitEffect(ItemCategorySub::SPIRIT_SHOT,
								data->GetSpiritShotSumLevel());
						}
					}
				}
			}

			// 딜 스크롤 처리 조건 검사
			bool activeDealScroll = Striker->IsObjectType(EGsGameObjectType::LocalPlayer);
			if (IsNoDamage || DamageStat == DamageStatType::MP || DamageStat == DamageStatType::SENSITIVE ||
				DamageStat == DamageStatType::HP_SHIELD)
			{
				activeDealScroll = false;
			}
				
			// 공격자 및 noDamage 정보에 따라 딜스크롤 처리 유무를 설정
			if (activeDealScroll)
			{
				// 공격방향
				FVector dir = GetLocation() - Striker->GetLocation();
				dir.Z = 0.0f;
				dir.Normalize();
				
				// 딜스크롤
				SetDealScroll(UGsBlueprintFunctionLibraryUI::GetDealScrollKey(ResultType, false),
					FString::FromInt(Damage), dir);
			}

			// 공격자의 이펙트 제거
			if (skillData != nullptr)
			{
				if (AGsCharacterBase* strikerActor = Striker->GetCharacter())
				{
					UGsAnimInstanceState* anim = strikerActor->GetAnim();
					if (skillData->targetFilterInfo.targetCount == 1)
					{
						anim->RemoveAnimNotifyParticle(EPlayParticleManagedType::TargetHitSync_OneTarget);
					}
					anim->RemoveAnimNotifyParticle(EPlayParticleManagedType::TargetHitSync_Immediate);
				}
			}
		}

#ifdef HIT_EFFECT_BY_SERVER
		// 각종 Hit처리 생략 타입
		if (ignoreHitEffect)
		{
			return false;
		}
#endif
		const FGsSchemaSkillNotifySet* skillNotifyData = UGsTableUtil::FindRowById<
			UGsTableSkillNotifySet, FGsSchemaSkillNotifySet>(StrikerNotifyId);
		const FGsSchemaPassivitySet* passivityData = nullptr;
		if (PassivityOwner == PassivityOwnerType::FAIRY)
		{
			passivityData = UGsTableUtil::FindRowById<UGsTablePassivitySet, FGsSchemaPassivitySet>(StriketSkillId);
		}

		const FGsSchemaProjectileSet* projectileData = nullptr;
		if (nullptr != SubStriker && SubStriker->IsObjectType(EGsGameObjectType::Projectile))
		{
			UGsGameObjectProjectile* projectile = SubStriker->CastGameObject<UGsGameObjectProjectile>();
			projectileData = projectile->GetProjectileData();
		}

		const FGsSchemaExtraTypeEffectSet* extraTypeEffectSet =
			UGsTableUtil::FindRowById<UGsTableExtraTypeEffectSet, FGsSchemaExtraTypeEffectSet>(ResultType);

		const FGsSchemaHealByTypeEffectSet* hitResultEffect =
			UGsTableUtil::FindRowById<UGsTableHealByTypeEffectSet, FGsSchemaHealByTypeEffectSet>(HealType);

		EGsStateBase state = EGsStateBase::StateBaseMax;
		if (FGsGameObjectStateManager* fsm = GetBaseFSM())
		{
			state = fsm->GetCurrentState();
		}

		if (false == ignoreHitmotion)
		{
			actorCharacter->OnHit(ResultType, state, GetCommonInfoTable());
		}
		actorCharacter->OnHitEffect(Striker->GetLocation(), ResultType,
			skillData, skillNotifyData, passivityData, projectileData,
			extraTypeEffectSet, hitResultEffect, isPlayerAttacker);
	}

	// 상태 전달
	if (_fsm != nullptr && false == ignoreHitmotion)
	{
		_fsm->ProcessEvent(EGsStateBase::Hit);
	}

	return true;
}

void UGsGameObjectCreature::OnOverlayMaterialChange(UGsGameObjectMaterialData* In_Data)
{
	if (AGsCharacterBase* characterActor = GetCharacter())
	{
		if (In_Data)
		{
			characterActor->SetChangeAllMaterials(In_Data, true);
		}
		else
		{
			if (characterActor->IsOverlayMaterialChanged())
				characterActor->RestoreAllMaterials();
		}
	}
}

void UGsGameObjectCreature::OnPolymorpChange()
{
	if (AGsCharacterBase* characterActor = GetCharacter())
	{
		if (characterActor->IsMeshChanged() == false)
		{
			if(auto shapeData = GetPolymorphShapeData())
				characterActor->ChangeMesh(shapeData->bPClass);
		}
	}
}

void UGsGameObjectCreature::OnVehicleEffect(bool useTalk)
{
}

void UGsGameObjectCreature::OnRestoreVehicleEffect()
{
}

void UGsGameObjectCreature::OnRestorePolymorph()
{
	if (AGsCharacterBase* characterActor = GetCharacter())
	{
		if (characterActor->IsMeshChanged())
			characterActor->RestoreMesh();
	}
}

int UGsGameObjectCreature::UpdateCombatStatus(bool InImmediately)
{
	if (AGsCharacterBase* characterActor = GetCharacter())
	{
		// 블루프린트로 이벤트를 알림
		characterActor->ReceiveBattleMode(IsBattleMode());

		// AnimationInstance에 상태 변경 알림
		if (UGsAnimInstanceState* animInstance = GetAnimInstance())
		{
			animInstance->SetBattleIdleType(IsBattleMode());
		}
	}
	return 0;
}

void UGsGameObjectCreature::SetDealScroll(const FName& InDealScrollType, const FString& InValue, const FVector& InDir)
{
	if (InDealScrollType.IsNone())
	{
		return;
	}

	ACharacter* character = Cast<ACharacter>(GetActor());
	if (character)
	{
		UGsBlueprintFunctionLibraryUI::AddDealScroll(InDealScrollType, character->GetMesh(), InValue, InDir,
			(nullptr != _gameObjectData) ? _gameObjectData->GetGameId() : 0);
	}
}

void UGsGameObjectCreature::SetEmoticon(EGsEmoticonType InType)
{
	UGsBlueprintFunctionLibraryUI::AddEmoticon(InType, this);
}

void UGsGameObjectCreature::ChangeIdleType(EGsAnimStateIdleType Type)
{
	if (UGsAnimInstanceState* anim = GetAnimInstance())
	{
		anim->SetIdleType(Type);
	}
}

const FGsStatInfo* UGsGameObjectCreature::GetCreatureStatInfoByType(StatType InStatType) const
{
	FGsGameObjectDataCreature* creatureData = GetCastData<FGsGameObjectDataCreature>();
	if (nullptr != creatureData)
	{
		const FGsStatInfo* localStat = creatureData->GetCreatureStatInfo(InStatType);
		return localStat;
	}
	return nullptr;
}

FGsGameObjectDataCreature* UGsGameObjectCreature::GetGameObjectDataCreature() const
{
	return GetCastData<FGsGameObjectDataCreature>();
}

bool UGsGameObjectCreature::IsZeroHP() const
{
	Super::IsZeroHP();

	FGsGameObjectDataCreature* creatureData = GetCastData<FGsGameObjectDataCreature>();
	if (nullptr != creatureData)
	{
		return creatureData->IsZeroHp();
	}

	return false;
}

/*
// 순수 클라쪽에서 Stat Set() 할 일이 있을지??? 일단 주석처리만 함
void UGsGameObjectCreature::SetCreatureStatInfoByType(StatType InStatType, int32 InValue)
{
	FGsGameObjectDataCreature* creatureData = GetCastData<FGsGameObjectDataCreature>();
	if (nullptr != creatureData)
	{
		creatureData->SetCreatureStatInfo(InStatType, InValue);
	}
}
*/

// visible 상태인지 갱신
void UGsGameObjectCreature::UpdateVisible()
{	
	bool isHidden = IsHidden();
	GetActor()->SetActorHiddenInGame(isHidden);
	// save flag( using in UGsGameObjectBase::ActorSpawned)
	_ActorVisible = !isHidden;
}

bool UGsGameObjectCreature::IsTargetedAvailableState()
{
	bool available = IsZeroHP() == false && IsDeadState() == false;

	if(FGsSkillHandlerBase* skillHandler = available ? GetSkillHandler() : nullptr)
		available = skillHandler->IsTargetedAvailableState();
	if(FGsAbnormalityHandlerBase* abnormalityHandler = available ? GetAbnormalityHandler() : nullptr)
		available = abnormalityHandler->IsTargetedAvailableState();

	return available;
}

// 스텟
void UGsGameObjectCreature::ApplyMoveSpeedFromStatInfo()
{
	FGsMovementHandlerBase* move = GetMovementHandler();
	if (nullptr == move) return;

	FGsGameObjectDataCreature* objdata = GetCastData<FGsGameObjectDataCreature>();
	check(objdata);

	// 이동 속도 설정
	TSharedPtr<FGsStatBase> statData = objdata->GetStatBase();
	check(statData.IsValid());
	
	if (const FGsStatInfo* statInfo = statData->GetStatInfo(StatType::MOVE_SPEED))
	{
		move->SetSpeedMovementComponent(statInfo->GetStatValue());
	}
	else
	{
		// bak1210 
		//비동기 로딩이 적용되고 서버에서 내려준 속도를 덮어쓰는경우가 있다 (서버에서 업데이트를 했다면 초기속도를 갱신하지 않는다.
		if (false == move->IsUpdateByMovetoLocation())
		{
			const FGsSchemaCreatureCommonInfo* commoninfo = GetCommonInfoTable();
			check(commoninfo);

			move->SetSpeedMovementComponent(commoninfo->moveSpeed);
		}
	}
}

void UGsGameObjectCreature::ApplyWalkSpeedFromCommonInfo()
{
	FGsMovementHandlerBase* move = GetMovementHandler();
	if (nullptr == move) return;

	const FGsSchemaCreatureCommonInfo* commoninfo = GetCommonInfoTable();
	check(commoninfo);

	move->SetSpeedMovementComponent(commoninfo->walkSpeed);
}

void UGsGameObjectCreature::ActiveGravity(bool inActive)
{	
	if (AGsCharacterBase* character = Cast<AGsCharacterBase>(GetActor()))
	{
		character->ActiveGravity(inActive);
	}
}

#if WITH_EDITOR
void UGsGameObjectCreature::DebugAddStateLog(const FString& inStateMsg)
{
	if (AGsCharacterBase* character = GetCharacter())
	{
		_debugLogComponent = character->FindComponentByClass<UGsDebugStateLogComponent>();
		if (_debugLogComponent)
		{
			_debugLogComponent->AddLog(inStateMsg);
		}
	}
}
#endif