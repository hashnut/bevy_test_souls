// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillHandlerLocalPlayer.h"
#include "Runner/GsSkillRunnerLocalPlayer.h"

#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Net/GsNetSendServiceWorld.h"

#include "ControllerEx/GsPlayerController.h"
#include "AI/Navigation/NavigationTypes.h"
#include "AI/Function/GsAIFunc.h"
#include "NavigationSystem/Public/NavMesh/RecastNavMesh.h"

#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsHommingProjectileComponent.h"
#include "ActorComponentEx/GsCharacterMovementComponent.h"
#include "Input/GsInputBindingLocalPlayer.h"
#include "Input/GsInputEventMsgBase.h"

#include "GameObject/ObjectClass/GsGameObjectNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Skill/Runner/GsBuffSkillRunnerLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerBase.h"
#include "GameObject/Movement/GsMovementHandlerBase.h"
#include "GameObject/Data/GsGameObjectDataCreature.h"
#include "GameObject/AI/GsAIReserveHandler.h"

#include "IdleEmotion/GsIdleEmotionLocalRunner.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsOptionManager.h"

#include "Skill/GsSkill.h"

#include "Data/GsDataContainManager.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifyHitSet.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelInfo.h"
#include "DataSchema/Skill/Level/GsSchemaSkillCostConsumeInfo.h"
#include "DataSchema/Skill/Level/GsSchemaSkillStatInfo.h"
#include "DataSchema/Skill/GsSchemaSkillCommon.h"

#include "Runtime/DataCenter/Public/DataSchema/GsSchemaEnums.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Util/GsTableUtil.h"
#include "Util/GsLevelUtil.h"
#include "Util/GsGameObjectUtil.h"
#include "UTIL/GsDir.h"

#include "Option/GsGameUserSettings.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "Message/MessageParam/GsCoolTimeMessageParam.h"

#include "T1Project.h"

#include "DrawDebugHelpers.h"
#include "LogMacros.h"

FGsSkillHandlerLocalPlayer::InputPressType FGsSkillHandlerLocalPlayer::IsPressMode[] =
{
	FGsSkillHandlerLocalPlayer::InputPressType::InputPressType_MAX,
	FGsSkillHandlerLocalPlayer::InputPressType::InputPressType_MAX,
	FGsSkillHandlerLocalPlayer::InputPressType::InputPressType_MAX
};

void FGsSkillHandlerLocalPlayer::SetPress(InputPressType Type)
{
	// 최종 입력 타입이 같은면 생략
	if (IsPressMode[InputPressType::InputPressType_LAST] == Type)
	{
		return;
	}

	for (int i = InputPressType::InputPressType_LAST - 1; i >= 0; --i)
	{
		// 이전 인덱스에서 정보기록 제거
		if (IsPressMode[i] == Type)
		{
			IsPressMode[i] = InputPressType::InputPressType_MAX;
		}
		// 최종 인덱스에 유효한 타입정보가 있을경우
		// 빈 인덱스에 설정
		else if (IsPressMode[i] == InputPressType::InputPressType_MAX &&
			IsPressMode[InputPressType::InputPressType_LAST] != InputPressType::InputPressType_MAX)
		{
			IsPressMode[i] = IsPressMode[InputPressType::InputPressType_LAST];
		}
	}

	// 최종 입력타입 설정
	IsPressMode[InputPressType::InputPressType_LAST] = Type;
}

void FGsSkillHandlerLocalPlayer::SetRelease(InputPressType Type)
{
	// 이전 정보기록 제거
	for (int i = 0; i < InputPressType::InputPressType_MAX; ++i)
	{
		if (IsPressMode[i] == Type)
		{
			IsPressMode[i] = InputPressType::InputPressType_MAX;
		}
	}
}

FGsSkillHandlerLocalPlayer::InputPressType FGsSkillHandlerLocalPlayer::GetLastPressType()
{
	for (int i = InputPressType::InputPressType_LAST; i >= 0; --i)
	{
		if (IsPressMode[i] != InputPressType::InputPressType_MAX)
		{
			return IsPressMode[i];
		}
	}
	return InputPressType::InputPressType_MAX;
}

bool FGsSkillHandlerLocalPlayer::IsPressType(InputPressType Type)
{
	for (int i = 0 ; i < InputPressType::InputPressType_MAX; ++i)
	{
		if (IsPressMode[i] == Type)
		{
			return true;
		}
	}
	return false;
}

void FGsSkillHandlerLocalPlayer::Initialize(UGsGameObjectBase* Owner)
{
	Super::Initialize(Owner);

	_messageParam = new FGsGameObjectMessageParamSkill();

	// FGsTargetHandlerLocalPlayer* targetHandlerLocal = Owner->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	_skillDataBuilder.Initialize(_local, this);

	_skillDataBuilder.SetRequestDataRestoreTime(GData()->GetGlobalData()->SkillRequestRestoreTime);

	// Buff용 Runner 생성 - 한번에 한개
	_buffSkillRunner = new FGsBuffSkillRunnerLocalPlayer(_local, [this](int skillid) {
			CallbackStartBuffSkill(skillid); }, [this](int skillid) {
			CallbackEndBuffSkill(skillid); });

	// Message Bind
	FGsMessageHolder* msg = GMessage();

	_actionDelegates.Emplace(
		msg->GetInput().AddRaw(MessageInput::PressJoystick, 
			this, &FGsSkillHandlerLocalPlayer::CallbackPressJoystick));

	_actionDelegates.Emplace(
		msg->GetInput().AddRaw(MessageInput::TerrainTouchMove,
			this, &FGsSkillHandlerLocalPlayer::CallbackPressJoystick));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_SKILL_CANCELABLE,
			this, &FGsSkillHandlerLocalPlayer::CallbackSkillCancelable));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_CHANGE_SKILL_SET,
		this, &FGsSkillHandlerLocalPlayer::CallbackChangeSkillSet));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_AUTOSKILL_UPDATE,
			this, &FGsSkillHandlerLocalPlayer::CallbackUpdateBuffSkillAuto));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_COOLTIME_END,
			this, &FGsSkillHandlerLocalPlayer::CallbackCooltimeEnd));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_CHANGE_SKILL_SLOT_AUTO,
			this, &FGsSkillHandlerLocalPlayer::CallbackChangeSkillSlotAuto));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_ABNORMALITY_REMOVE,
			this, &FGsSkillHandlerLocalPlayer::CallbackRemoveAbnormality));
	
	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::AI_MODE_ACTIVE,
			this, &FGsSkillHandlerLocalPlayer::CallbackAIModeActive));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_TARGET_CHANGED,
			this, &FGsSkillHandlerLocalPlayer::CallbackTargetChange));

	_gameobjectDelegates.Emplace(
		msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_TARGET_CLEAR,
			this, &FGsSkillHandlerLocalPlayer::CallbackTargetClear));

	if (IsBranch(EGsGameClientBranchType::ABNORMALTIY_CC_TAUNT_SKILL))
	{
		_gameobjectDelegates.Emplace(
			msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_ABNORMALITY_ADD,
				this, &FGsSkillHandlerLocalPlayer::CallbackAddAbnormality));
	}
	
	_gameobjectDelegates.Emplace(msg->GetGameObject().AddRaw(MessageGameObject::CLEAR_SKILL_DATA,
		this, &FGsSkillHandlerLocalPlayer::CallbackClearOriginSkilldata));

	_gameobjectDelegates.Emplace(msg->GetGameObject().AddRaw(MessageGameObject::LOCAL_REACTION,
		this, &FGsSkillHandlerLocalPlayer::CallbackUpdateReaction));

	// 특정 상태에서 예약 클리어 처리
	_localObjectDelegates.Emplace(
		_owner->GetMessage().AddRaw(MessageGameObject::StateEnter,
			this, &FGsSkillHandlerLocalPlayer::CallbackStateChangeClearData));


	_messageParam->Empty();
	_skillfailedCount.Value = 0;
	_checkNavMeshRaycastCount = 0;

	// 인풋 저장 값 초기화 처리
	for (int i = 0; i < InputPressType::InputPressType_MAX; ++i)
	{
		IsPressMode[i] = InputPressType::InputPressType_MAX;
	}
}

void FGsSkillHandlerLocalPlayer::Update(float Delta)
{
	//OnReserveSkill(false, Delta);
	if (_buffSkillRunner->IsRunning() && false == _buffSkillRunner->Update(Delta))
	{
		_buffSkillRunner->EndRunner();
	}

// 	if (_skillDataBuilder.UpdateRequestData(Delta))
// 	{
// 		ClearRequestSkill(0);
// 	}

	// Skill 종료 Message 전송
	// SkillRunner의 End콜백에서 처리할 경우 많은 문제가 발생되어 한틱 처리후 외부에 메세지를 전송
	if (_messageParam->IsValid())
	{
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_SKILL_END, _messageParam);
		_messageParam->Empty();
	}

	Super::Update(Delta);
}

void FGsSkillHandlerLocalPlayer::Finalize()
{
	SAFE_DELETE(_messageParam);
	SAFE_DELETE(_buffSkillRunner);

	_enableAutoBuffSkill = false;
	_enableDelayBuffSkill = 0.f;	

	_mapSkillUsefulTime.Empty();

	_skillDataBuilder.Finalize();
	
	// 메세지 해제
	FGsMessageHolder* msg = GMessage();
	for (TPair<MessageInput, FDelegateHandle>& el : _actionDelegates)
	{
		msg->GetInput().Remove(el);
	}
	_actionDelegates.Empty();

	for (MsgGameObjHandle& el : _gameobjectDelegates)
	{
		msg->GetGameObject().Remove(el);
	}
	_gameobjectDelegates.Empty();

	for (MsgGameObjHandle& el : _localObjectDelegates)
	{
		_owner->GetMessage().Remove(el);
	}
	_localObjectDelegates.Empty();
	_listComboSkillData.Empty();

	if (_autoSkillTimerHandle.IsValid())
	{
		_owner->GetWorld()->GetTimerManager().ClearTimer(_autoSkillTimerHandle);
	}

	Super::Finalize();
}

// Super 메서드 호출 금지
void FGsSkillHandlerLocalPlayer::CreateSkillRunner()
{
	_skillRunner = new FGsSkillRunnerLocalPlayer([this](int skillid, bool isEnd) { CallbackEndRunner(skillid, isEnd); });
}

void FGsSkillHandlerLocalPlayer::CreateIdleEmotionRunner()
{
	if (auto data = GSkill()->GetIdleEmotionActionData(_owner))
	{
		_idleEmotionRunner = MakeShared<FGsIdleEmotionLocalRunner>(_owner);
		_idleEmotionRunner->InitIdleEmotion(data);

		SetAutoIdleEmotionEnable(true);
	}
}

void FGsSkillHandlerLocalPlayer::LoadSkill()
{
	Super::LoadSkill();

	_local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
	
	LoadSkillCache();
	//LoadCursorSkill();
}

void FGsSkillHandlerLocalPlayer::LoadSocial()
{
	Super::LoadSocial();

	_listSocial.Empty();
	GSkill()->GetPlayerSocialSet(_listSocial, _owner);
}

void FGsSkillHandlerLocalPlayer::LoadAnimation()
{
	Super::LoadAnimation();
}

void FGsSkillHandlerLocalPlayer::LoadSkillCache()
{
	_listComboSkillData.Empty();

	// 현재 활성화된 무기 타입에서 찾기
	TArray<TSharedPtr<const FGsSkill>> OutSkillSet;
	GSkill()->GetCurrentWeaponLeanSkillSet(OutSkillSet);
	for (TSharedPtr<const FGsSkill> el : OutSkillSet)
	{
		// 콤보 스킬 캐싱
		if (el->_tableData->category == SkillCategory::COMBO)
		{
			_listComboSkillData.Emplace(el->_tableData);
		}
	}
	_listComboSkillData.Sort([](const FGsSchemaSkillSet& lhs, const FGsSchemaSkillSet& rhs)
	{
		return lhs.sequenceInfo.comboInfo.currentStage < rhs.sequenceInfo.comboInfo.currentStage;
	});
}

bool FGsSkillHandlerLocalPlayer::UseBuffRequest(const FGsSkill* SkillData)
{
	// 즉발형 검사
	// 상태 변환 하지 않는 스킬 발동 (BUFF 타입)
	if (SkillData->GetSkillCateogrySet() != SkillCategorySet::BUFF)
	{
		return false;
	}

	// 응답 대기중
	if (_buffSkillRunner->IsReady())
	{
#if WITH_EDITOR
		GSLOG(Log, TEXT("[DebugGameObjectLog] _buffSkillRunner IsSend Ready"), SkillData->GetSkillId());
#endif
		return false;
	}

	// 현재 사용중인 버프인가 확인
	if (_buffSkillRunner->IsValid())
	{
		const FGsSchemaSkillSet* buffSkill = _buffSkillRunner->GetData();
		if (buffSkill && buffSkill->id == SkillData->GetSkillId())
		{
#if WITH_EDITOR
			GSLOG(Log, TEXT("[DebugGameObjectLog] _buffSkillRunner IsRuning Id : %d"), SkillData->GetSkillId());
#endif
			return false;
		}
	}

	// 체크할 타겟 설정
	_skillDataBuilder.BuildTargetData(SkillData);

	// 유효성 검사
	EGsSkillCheckResultType checkResult = _skillDataBuilder.BuildValidateData(SkillData);
	if (EGsSkillCheckResultType::Pass != checkResult)
	{
		// 에러 시스템 메세지를 출력
		if (_local && false == _local->IsSpectatorMode())
		{
			FGsUIHelper::TrayMessageTicker(_skillDataBuilder.GetErrorMessage());
		}		
		return false;
	}

	return SendBuffSkill(SkillData);
}

const FGsSkill* FGsSkillHandlerLocalPlayer::ConvertGroupSkillData(const class FGsSkill* SkillData)
{
	// 콤보(평타) 스킬 최종 변환
	if (SkillData->IsComboSkill() && GetNextComboData())
	{
		return GSkill()->FindActiveSkill(GetNextComboData()->id);
	}
	// 체인 스킬 최종 변환
	else if (SkillData->IsChainSkill())
	{
		if (const FGsComboSkillGroup* chainGroup = GSkill()->FindChainSkillGroup(SkillData->GetSkillId()))
		{
			// 현재 단계의 체인 스킬로 치환
			return chainGroup->GetCurrentData();
		}
	}
	return SkillData;
}

bool FGsSkillHandlerLocalPlayer::UseSkillRequest(const FGsSkill* SkillData)
{	
	// 월드 정보가 없는데 간혹 호출되는 타이밍들이 있는것 같다
	if (nullptr == _owner || nullptr == _owner->GetWorld())
	{
		return false;
	}

	// 그룹 스킬들일 경우 변환
	const FGsSkill* realSkillData = ConvertGroupSkillData(SkillData);

	// 아직 사용 진행중인 스킬이 있는가
	if (IsRequestData())
	{
		const FGsUseSkillRequest& requestSkill = _skillDataBuilder.GetRequestData();
#if WITH_EDITOR
		GSLOG(Log, TEXT("[DebugGameObjectLog] SetReserveData(IsRequestData()) Id : %d"),
			_skillDataBuilder.GetRequestData()._skillData->GetSkillId());
#endif

		// 요청 스킬과 다른 스킬은 예약 처리
		if (requestSkill._skillData->GetSkillId() != realSkillData->GetSkillId())
		{
			// 예약 교체
			_skillDataBuilder.SetReserveData(realSkillData,
				FGsSkillHandlerBase::GetSkillAttackRange(_local, realSkillData->_tableData));
		}

		return false;
	}

	// 예약 교체
	_skillDataBuilder.SetReserveData(realSkillData,
		FGsSkillHandlerBase::GetSkillAttackRange(_local, realSkillData->_tableData));
	
	// 체크할 타겟 최종설정 및 유효성 검사
	UGsGameObjectBase* target = _skillDataBuilder.BuildTargetData(realSkillData);
	EGsSkillCheckResultType checkResult = _skillDataBuilder.BuildValidateData(realSkillData);
	if (EGsSkillCheckResultType::Pass != checkResult)
	{
		// 실패시 예약 데이터 제거
		if (EGsSkillCheckResultType::NotCancel != checkResult)
		{
			ClearReserveSkill(realSkillData->GetSkillId());
		}
#if WITH_EDITOR
		// 에러 시스템 메세지를 출력 (에디터 환경에서만 확인용으로 사용)
		if (_local && false == _local->IsSpectatorMode())
		{
			FGsUIHelper::TrayMessageTicker(_skillDataBuilder.GetErrorMessage());
		}		
#endif	
		return false;
	}

	if (target)
	{	
		// 기존 예약 데이터를 클리어 한다.
		_local->ClearReserveDataByReserveWork(this);	

		// 일반 스킬 사용 거리 체크
		AttackRangeInsideType insideType = IsInsideSkillRangeTarget(realSkillData->_tableData, target);
		if (OutSide == insideType)
		{
			// MoveToSkill 이면 예약
#if WITH_EDITOR
			GSLOG(Log, TEXT("[DebugGameObjectLog] SetReserveData(MoveTo) Id : %d"), realSkillData->GetSkillId());
#endif
			OnMoveToSkill(target, FGsSkillHandlerBase::GetSkillApproachRange(_local, realSkillData->_tableData));
			return false;
		}
		else if(false == target->IsSkipTerrainCheck() && InSide == insideType)
		{
			// 공격 가능한 네비 영역인지 체크			
			FVector destDir = _local->GetLocation() - target->GetLocation();
			float targetRadius = target->GetData()->GetScaledCapsuleRadius();
			float localRadius = _local->GetData()->GetScaledCapsuleRadius();
			float totalRadius = targetRadius + localRadius + 50.f; // 오차 범위
			
			// 이미 대상에 맞닿아 있는지 확인
			// 대형 몬스터의경우 고저차가 있어서 맞닿아 있어도 타겟과의 거리가 더 멀 수 있으므로 높이 보정 없앰
			if (destDir.SizeSquared2D() > totalRadius * totalRadius)
			{
				// 타겟의 반경을 고려하여 자신과 방향에서 가장 가까운 위치를 목적 좌표로 설정
				FVector destPos = target->GetLocation() + destDir.GetSafeNormal() * targetRadius;
				// 네비 데이터 검사
				if (false == CheckNavMeshRaycast(_local->GetLocation(), destPos))
				{
					// 10회 이상 실패는 답이 없음.. 네비데이터 및 충돌 오브젝트 확인 필요
					if (_checkNavMeshRaycastCount > 10)
					{
						GSLOG(Log, TEXT("[DebugGameObjectLog] Failed movetoNaviData"));
						ClearTargetAtSkillFailure();
						_checkNavMeshRaycastCount = 0;
						return false;
					}
					// 이미 여러번 시도한 상황 이라면 목적 좌표를 대상으로 변경
					else if (_checkNavMeshRaycastCount > 3)
					{
						destPos = target->GetLocation();
					}
					else
					{
						// 가능 목적좌표 갱신
						destPos = FindNaviPathValidPoint(target);
					}

					float AcceptanceRadius = 0.f;
					// 대상 반경안에 포함되거나, 목적좌표가 대상일 경우 도착 반경 적용
					float targetRadiusSquared = targetRadius * targetRadius;
					if (destDir.SizeSquared() < targetRadiusSquared || 
						(destPos - target->GetLocation()).SizeSquared() < targetRadiusSquared)
					{
						AcceptanceRadius = targetRadius + localRadius;
					}

					OnMoveToSkill(destPos, AcceptanceRadius);
					_checkNavMeshRaycastCount++;
#if WITH_EDITOR
					GSLOG(Log, TEXT("[DebugGameObjectLog] SetReserveData(MoveToNavi) Id : %d"), realSkillData->GetSkillId());
#endif
					return false;
				}

			}
		}
	}
	else
	{
		ClearReserveSkill(realSkillData->_tableData->id);
		return false;
	}

	_checkNavMeshRaycastCount = 0;
	
	// 스킬 요청
	// !! 주의 SendSkill은 한번만 호출 (중간에 다른 메세지를 통해 다시 UseSkillRequest가 호출 될수 있음)
	return SendSkill(target, realSkillData);
}

FVector FGsSkillHandlerLocalPlayer::FindNaviPathValidPoint(UGsGameObjectBase* Target)
{
	FVector destDir = _local->GetLocation() - Target->GetLocation();
	float targetRadius = Target->GetData()->GetScaledCapsuleRadius();

	if (AGsPlayerController* controller = Cast<AGsPlayerController>(_local->GetCharacter()->GetController()))
	{
		FAIMoveRequest moveRequest;
		moveRequest.SetGoalLocation(Target->GetLocation());
		moveRequest.SetAcceptanceRadius(targetRadius);
		moveRequest.SetAllowPartialPath(false);
		FPathFindingQuery Query;
		controller->BuildPathfindingQuery(moveRequest, Query);

		FNavPathSharedPtr Path;
		controller->FindPathForMoveRequest(moveRequest, Query, Path);

		if (Path.IsValid())
		{
			// 2번째 부터 체크
			const TArray<FNavPathPoint>& pathList = Path.Get()->GetPathPoints();
			for (int i = 1; i < pathList.Num(); ++i)
			{
				// 시작 위치가 차이가 없다면 continue
				FVector2D pathDir2D = FVector2D(pathList[i].Location) - _local->GetLocation2D();
				if (pathDir2D.SizeSquared() < 50.f * 50.f)
				{
					continue;
				}

#if UE_BUILD_DEBUG
				DrawDebugSphere(_local->GetWorld(), pathList[i].Location, 10.f, 100, FColor::Red, false, 1.f);
				if (_checkNavMeshRaycastCount > 1)
				{
					FVector debugPoint = pathList[i].Location;
					debugPoint.Z += 200.f;
					DrawDebugSphere(_local->GetWorld(), debugPoint, 10.f, 100, FColor::Green, false, 1.f);
				}
#endif
				FVector destPos = Target->GetLocation() + destDir.GetSafeNormal() * targetRadius;
				if (CheckNavMeshRaycast(pathList[i].Location, destPos))
				{
					return pathList[i].Location;
				}
			}

			// 타겟이 네비데이터 영역 안에 없는듯
			return pathList.Last();
		}
	}
	
	// 실패할 경우 타겟 좌표로 리턴
	return Target->GetLocation();
}

bool FGsSkillHandlerLocalPlayer::OnSkill(int ID)
{
	const FGsSkill* skillData = GSkill()->FindActiveSkill(ID);
	if (nullptr == skillData || nullptr == skillData->_tableData)
	{
		return false;
	}

#if WITH_EDITOR
	GSLOG(Log, TEXT("[DebugGameObjectLog] %s OnSkill Id : %d"), *FGsDateTimeUTIL::GetCurrentTime(), ID);
#endif
	if (skillData->IsBuffSkill())
	{
		return UseBuffRequest(skillData);
	}

#ifdef TEST_AI_RETURN
	if (_local != nullptr)
	{
		FGsAIReserveHandler* aiReserveHandler = _local->GetAIReserve();
		if (aiReserveHandler != nullptr)
		{
			if (true == aiReserveHandler->IsFirstJobAction(EGsAIActionType::MOVE_TO_AUTO_START_POS))
			{
				GSLOG(Error, TEXT("OnSkill :MOVE_TO_AUTO_START_POS fist reserve job !!"));
			}
		}
	}
#endif

	return UseSkillRequest(skillData);
}

bool FGsSkillHandlerLocalPlayer::OnReserveSkill()
{
	if (IsRequestData())
	{
		GSLOG(Log, TEXT("OnReserveSkill() not working "));
		return false;
	}
	// 예약 스킬 확인
	if (IsReserveData())
	{
		const FGsSkillReserve& reserveSkill = _skillDataBuilder.GetReserveData();
		// 타겟 정보가 없다면 재타겟
		FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
		if (nullptr == targetHandler->GetTarget())
		{
			targetHandler->OnTarget(FGsTargetRule::AutoTarget, true,
				FGsTargetHandlerBase::SelectTargetFilter::AutoSkill, reserveSkill._skillData->GetIffType());
		}
#ifdef TEST_AI_RETURN
		if (_local != nullptr)
		{
			FGsAIReserveHandler* aiReserveHandler = _local->GetAIReserve();
			if (aiReserveHandler != nullptr)
			{
				if (true == aiReserveHandler->IsFirstJobAction(EGsAIActionType::MOVE_TO_AUTO_START_POS))
				{
					GSLOG(Error, TEXT("OnSkill :MOVE_TO_AUTO_START_POS fist reserve job !!"));
				}
			}
		}
#endif


		return UseSkillRequest(reserveSkill._skillData);
	}

	return false;
}

void FGsSkillHandlerLocalPlayer::StopSkill(int ID)
{
	Super::StopSkill(ID);

	// 예약 스킬 종료
	ClearReserveSkill(ID);

	// MovetoTarget중이면 중지
	//
	FGsGameObjectStateManager* fsm = _local->GetBaseFSM();
	if (fsm->IsState(EGsStateBase::AutoMove))
	{
		fsm->ProcessEvent(EGsStateBase::AutoMoveStop);
	}

	if (_skillRunner->IsValid())
	{
		if (ID == 0 || _skillRunner->GetSkillData()->id == ID)
		{
			_skillRunner->CancelRunner();
		}
	}

	if (_buffSkillRunner->IsValid())
	{
		_buffSkillRunner->CancelRunner();
	}

	// 콤보 정보 초기화
	SetNextComboData(nullptr);
}

void FGsSkillHandlerLocalPlayer::CompulsionCancelSkill(int ID)
{
	// Stop 스킬 적용
	Super::CompulsionCancelSkill(ID);

	// 상태 전환
	FGsGameObjectStateManager::ProcessEvent(_local, EGsStateBase::AttackEnd);
}


void FGsSkillHandlerLocalPlayer::SetNextComboData(const FGsSchemaSkillSet* ComboSkillData)
{
	_nextComboSkillData = ComboSkillData;
}

const FGsSchemaSkillSet* FGsSkillHandlerLocalPlayer::GetNextComboData() const
{
	return _nextComboSkillData;
}

const FGsSchemaSkillSet* FGsSkillHandlerLocalPlayer::GetFirstComboData() const
{
	if (_listComboSkillData.Num() > 0)
	{
		return _listComboSkillData[0];
	}
	return nullptr;
}

const FGsSkillReserve& FGsSkillHandlerLocalPlayer::GetReserveSkill() const
{
	return _skillDataBuilder.GetReserveData();
}

const FGsCastingSkillRequest& FGsSkillHandlerLocalPlayer::GetCastInfo() const
{
	return _skillDataBuilder.GetCastingData();
}

void FGsSkillHandlerLocalPlayer::ClearRequestSkill(uint32 Id)
{
	if (IsCasting())
	{
		const FGsCastingSkillRequest& castRequest = _skillDataBuilder.GetCastingData();
		if (castRequest._castingSkillId == Id || Id == 0)
		{
			ClearCastingData(Id);
		}
	}

	const FGsUseSkillRequest& skillRequest = _skillDataBuilder.GetRequestData();
	if (skillRequest.IsVaild())
	{
		if (Id != 0 && skillRequest._skillData->GetSkillId() != Id)
		{
			return;
		}
		_skillDataBuilder.ClearRequestData();

#if WITH_EDITOR
		GSLOG(Log, TEXT("[DebugGameObjectLog] ClearRequestSkill Id : %d"), Id);
#endif
	}

	if (_buffSkillRunner->IsValid())
	{
		if (_buffSkillRunner->GetData()->id == Id)
		{
			_buffSkillRunner->CancelRunner();
		}
	}
}

void FGsSkillHandlerLocalPlayer::ClearTargetAtSkillFailure()
{
	if (FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>())
	{
		UGsGameObjectBase* target = targetHandler->GetTarget();
		if (nullptr == target)
		{
			return;
		}

		FGsTargetRule* targetRule = targetHandler->GetTargetRule();
		if (nullptr == targetRule)
		{
			return;
		}
		// 보스가 아닌 NPC경우에만 해당 타겟은 무시 설정을 진행 한다.
		if (target->IsObjectType(EGsGameObjectType::NonPlayer))
		{
			UGsGameObjectNonPlayer* castNonplayer = target->CastGameObject<UGsGameObjectNonPlayer>();
			if (castNonplayer && false == castNonplayer->IsBossMonster())
			{
				targetRule->SetIgnoreTargetId(target->GetGameId());
			}
		}
		targetHandler->ClearTarget();
	}
}

void FGsSkillHandlerLocalPlayer::ClearReserveSkill(uint32 Id)
{
	const FGsSkillReserve& skillReserve = _skillDataBuilder.GetReserveData();
	if (skillReserve.IsVaild())
	{
		// 무조건 제거
		if (Id == 0)
		{
			_skillDataBuilder.ClearReserveData();
		}
		else if (skillReserve._skillData->GetSkillId() == Id)
		{
			_skillDataBuilder.ClearReserveData();
		}
		// 체인 스킬 그룹 찾기
		else if (skillReserve._skillData->IsChainSkill())
		{
			const FGsComboSkillGroup* chainSkillGroup = GSkill()->FindChainSkillGroup(Id);
			if (nullptr == chainSkillGroup)
			{
				return;
			}
			int32 reserveSkillId = skillReserve._skillData->GetSkillId();
			if (false == chainSkillGroup->Contain(reserveSkillId))
			{
				return;
			}
			_skillDataBuilder.ClearReserveData();
		}
		else if (skillReserve._skillData->IsComboSkill())
		{
			if (false == IsComboSkill(Id))
			{
				return;
			}
			_skillDataBuilder.ClearReserveData(); 
		}

		FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
		// 임의 지정 대상이 존재할 경우 정리
		if (targetHandler->GetSkillSelectTarget() != 0)
		{
			targetHandler->ClearSkillSelectTarget();
		}
#if WITH_EDITOR
		GSLOG(Log, TEXT("[DebugGameObjectLog] ClearReserveData Id : %d"), Id);
#endif
	}
}

void FGsSkillHandlerLocalPlayer::OnManualModeTargetAttackAction()
{
	if (nullptr == GSAI())
	{
		return;
	}

	// 평타 버튼 누르고 있음
	if (IsPressType(ComboSkillbutton))
	{
		OnSkill(_listComboSkillData[0]->id);
		return;
	}

	// 수동 모드 확인
	if (IsPressType(Joystick))
	{
		return;
	}

	// 자동 사냥 모드 체크
	if (GSAI()->IsAIOn())
	{
		return;
	}

	// 예약 데이터 확인
	for (IGsReserveWork* el : _local->GetReserveWorks())
	{
		if (el->IsReserveData())
		{
			return;
		}
	}

	// 수동 모드 자동 사용 대상 확인
	FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	if (nullptr == targetHandler)
	{
		return;
	}

	if (nullptr == targetHandler->GetManualModeAutoTarget())
	{
		return;
	}

	const FGsSchemaSkillSet* nextAutoSkill = GetNextAutoSkill(SkillCategorySet::ACTIVE);
	// 자동 스킬 사용 옵션 체크
// 	FGsServerOption* serverOption = GOption()->GetServerOption();
// 	if (serverOption->IsAutoSkillOn())
// 	{
// 		nextAutoSkill = GetNextAutoSkill(SkillCategorySet::ACTIVE);
// 	}

	if (nullptr == nextAutoSkill)
	{
		nextAutoSkill = GetFirstComboData();

		// Error
		if (nullptr == nextAutoSkill)
		{
			return;
		}
	}

	OnSkill(nextAutoSkill->id);
}

void FGsSkillHandlerLocalPlayer::ClearCastingData(uint32 Id)
{
	const FGsCastingSkillRequest& castingInfo = _skillDataBuilder.GetCastingData();
	if (castingInfo.IsVaild() && castingInfo._castingSkillId == Id)
	{
		_skillDataBuilder.ClearCastingData();
	}	
}

void FGsSkillHandlerLocalPlayer::CallbackEndRunner(int inSkillID, bool inIsEnd)
{
	Super::CallbackEndRunner(inSkillID, inIsEnd);

	// 캔슬이 아닌 종료 상황에서 콤보 스킬 확인 및 초기화
	if (inIsEnd && IsComboSkill(inSkillID))
	{
		SetNextComboData(nullptr);
	}

	// 메세지 전송 준비
	_messageParam->_paramOwner =  _local;
	_messageParam->_skillId = inSkillID;	
	
	if (FGsAIManager* aiManager = GSAI())
	{
		if (aiManager->IsAIOn())
		{
			return;
		}
	}

#if WITH_EDITOR
	GSLOG(Log, TEXT("[DebugGameObjectLog] CallbackEndRunner"));
#endif
	OnReserveSkill();
}

void FGsSkillHandlerLocalPlayer::CallbackSkillCancelable(const IGsMessageParam* inParam)
{	
	 const FGsGameObjectMessageParamSkill* castData = inParam->Cast<const FGsGameObjectMessageParamSkill>();

	 // 캔슬 구간에서는 네트워크 싱크 데이터 정리
	 // ClearRequestSkill(castData->_skillId);

	 // 이동 가능 상태 설정
	 if (FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>())
	 {
		 movement->SetMoveable(true);
		 movement->SetMoveState(false);
	 }

	if (FGsAIManager* aiManager = GSAI())
	{
		if (aiManager->IsAIOn())
		{
			return;
		}
	}
	
#if WITH_EDITOR
	GSLOG(Log, TEXT("[DebugGameObjectLog] %s CallbackSkillCancelable Elapsed : %f"), *FGsDateTimeUTIL::GetCurrentTime(), _skillRunner->GetElapsedTime());
#endif
	if (false == OnReserveSkill())
	{
		OnManualModeTargetAttackAction();
	}
}

void FGsSkillHandlerLocalPlayer::CallbackRemoveAbnormality(const IGsMessageParam* Param)
{
	// 제거된 버프형 abnormality 확인
	if (nullptr == Param)
	{
		return;
	}

	const FGsPrimitivePairInt32* paramPair = Param->Cast<const FGsPrimitivePairInt32>();
	if (nullptr == paramPair)
	{
		return;
	}

	AbnormalityInstanceId instanceId = static_cast<AbnormalityInstanceId>(paramPair->_first);

	if (FGsAbnormalityHandlerBase* abnormalityHandler = _local->GetAbnormalityHandler())
	{
		TWeakPtr<FGsAbnormalityData> abnormalityPtr = abnormalityHandler->GetAbnormalityData(instanceId);
		if (abnormalityPtr.IsValid())
		{
			if (abnormalityPtr.Pin()->GetTable() &&
				abnormalityPtr.Pin()->GetTable()->isBuff)
			{
				FTimerManager& timerManager = _owner->GetWorld()->GetTimerManager();
				// 아직 해당 버프가 리스트에서 제거되지 않은 상황이므로 틱처리 해준다.
				if (_autoSkillTimerHandle.IsValid())
				{
					timerManager.ClearTimer(_autoSkillTimerHandle);
				}

				UGsGameObjectBase* owner = _owner;
				_autoSkillTimerHandle = timerManager.SetTimerForNextTick(FTimerDelegate::CreateLambda([owner]() {
					FGsGameObjectMessageParamBuffSkill messageParam(owner, 0, FGsGameObjectMessageParamBuffSkill::BuffSkillAuto::Enable);
					GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_AUTOSKILL_UPDATE, &messageParam);
					}));
			}
		}
	}
}

void FGsSkillHandlerLocalPlayer::CallbackStateChangeClearData(const FGsGameObjectMessageParam* Param)
{
	// 특정 상태를 감지하여 예약된 정보를 클리어
	if (const FGsGameObjectMessageParamState* castParam = Param->Cast<const FGsGameObjectMessageParamState>())
	{
		switch (castParam->_type)
		{
		case EGsStateBase::Looting:
		case EGsStateBase::Interaction:
		case EGsStateBase::Warp:
		case EGsStateBase::Freeze:
			{
				ClearReserveSkill();
			}
			break;
		}
	}
}

void FGsSkillHandlerLocalPlayer::CallbackPressJoystick(const FGsInputEventMsgBase& In_msg)
{
	ClearReserveSkill();
}

void FGsSkillHandlerLocalPlayer::CallbackUpdateBuffSkillAuto(const IGsMessageParam* Param)
{
	if (auto buffSkillParam = Param->Cast<const FGsGameObjectMessageParamBuffSkill>())
	{
		//-1 일때는 _enableAutoBuffSkill 상태를 유지
		auto oldEnable = _enableAutoBuffSkill;
		_enableAutoBuffSkill = 
			buffSkillParam->_autoEnable == FGsGameObjectMessageParamBuffSkill::BuffSkillAuto::Enable ?  true : 
			buffSkillParam->_autoEnable == FGsGameObjectMessageParamBuffSkill::BuffSkillAuto::Disable ? false : oldEnable;

		if(_enableAutoBuffSkill) CallbackUpdateBufSkill();
	}
}

void FGsSkillHandlerLocalPlayer::CallbackCooltimeEnd(const IGsMessageParam* Param)
{
	if (const FGsCoolTimeMessageParam* castParam = Param->Cast<const FGsCoolTimeMessageParam>())
	{
		if (castParam->_type == EGsCoolTime::Skill)
		{
			if (const FGsSkill* findSkill = GSkill()->FindSkill(castParam->_id))
			{
				if (findSkill->IsBuffSkill() && findSkill->IsAuto())
				{
					FGsGameObjectMessageParamBuffSkill messageParam(_owner, 0, FGsGameObjectMessageParamBuffSkill::BuffSkillAuto::Enable);
					GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_AUTOSKILL_UPDATE, &messageParam);
				}
			}
		}
	}
}

void FGsSkillHandlerLocalPlayer::CallbackChangeSkillSlotAuto(const IGsMessageParam* Param)
{
	if (const FGsGameObjectMessageParamSkillSlot* castParam = Param->Cast<const FGsGameObjectMessageParamSkillSlot>())
	{
		if (castParam->_skill->_slotData._bUseAuto)
		{
			FGsGameObjectMessageParamBuffSkill messageParam(_owner, 0, FGsGameObjectMessageParamBuffSkill::BuffSkillAuto::Enable);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_AUTOSKILL_UPDATE, &messageParam);
		}
	}
}

void FGsSkillHandlerLocalPlayer::CallbackTargetChange(const IGsMessageParam* In_param)
{
	CallbackUpdateBufSkill();
}

void FGsSkillHandlerLocalPlayer::CallbackTargetClear(const IGsMessageParam* In_param)
{
	if (FGsAIManager* aiManager = GSAI())
	{
		if (aiManager->IsAIOn())
		{
			return;
		}
	}
	ClearReserveSkill();
}

void FGsSkillHandlerLocalPlayer::CallbackClearOriginSkilldata(const IGsMessageParam* In_param)
{
	if (IsCasting())
	{
		const FGsCastingSkillRequest& castRequest = _skillDataBuilder.GetCastingData();
		ClearCastingData(castRequest._castingSkillId);
	}
	
	ClearRequestSkill();

	StopSkill();
}

void FGsSkillHandlerLocalPlayer::CallbackAddAbnormality(const IGsMessageParam* In_param)
{
	if (const FGsPrimitivePairInt32* castParam = In_param->Cast<const FGsPrimitivePairInt32>())
	{
		AbnormalityEffectType effectType = (AbnormalityEffectType)castParam->_second;
		if (effectType == AbnormalityEffectType::CC_TAUNT)
		{
			FGsTargetHandlerBase* targetHandler = _local->GetTargetHandler();
			if (targetHandler && targetHandler->GetTarget())
			{
				if (const FGsSchemaSkillSet* normalSkillData = GetFirstComboData())
				{
					OnSkill(normalSkillData->id);
				}
			}
		}
	}
}

void FGsSkillHandlerLocalPlayer::CallbackUpdateReaction(const IGsMessageParam* In_param)
{
	if (const FGsGameObjectMessageParamReactionLocalPlayer* castParam =
		In_param->Cast<const FGsGameObjectMessageParamReactionLocalPlayer>())
	{
		if (FGsTargetHandlerLocalPlayer* localTarget = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>())
		{
			if (localTarget->GetManualModeAutoTarget())
			{
				OnManualModeTargetAttackAction();
			}
		}
	}
}

void FGsSkillHandlerLocalPlayer::CallbackStartBuffSkill(int skillid)
{
	if (const FGsSkill* skill = GSkill()->FindSkill(skillid))
	{
		_enableDelayBuffSkill = skill->GetUIEnableDelayTime();
		FGsGameObjectMessageParamSkillSlot messageParam(skill->_tableData->requireWeapon, skill->GetSlotId(), skill);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_CHANGE_BUFFSKILL_SLOT, &messageParam);
	}
}

void FGsSkillHandlerLocalPlayer::CallbackEndBuffSkill(int skillid)
{
	_enableDelayBuffSkill = 0.f;

	FGsGameObjectMessageParamBuffSkill messageParam(_owner, 0, FGsGameObjectMessageParamBuffSkill::BuffSkillAuto::Enable);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_AUTOSKILL_UPDATE, &messageParam);
	// if(_enableAutoBuffSkill) CallbackUpdateBufSkill();
}

void FGsSkillHandlerLocalPlayer::CallbackUpdateBufSkill()
{
	if (_enableDelayBuffSkill <= 0.f)
	{
		// 예약후 다음에 사용요청
		if (const FGsSchemaSkillSet* autoBuffSkill = GetNextAutoSkill(SkillCategorySet::BUFF))
		{
			OnSkill(autoBuffSkill->id);
		}
	}
}

void FGsSkillHandlerLocalPlayer::CallbackStartEmotionMotion(int32 motionID)
{
	OnSocial(motionID);
}

void FGsSkillHandlerLocalPlayer::CallbackEndEmotionMotion()
{
	StopSocial(true);
}

void FGsSkillHandlerLocalPlayer::OnMoveToSkill(FVector DestPos, float AcceptanceRadius)
{
	FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::MoveToSkill);
	movement->ChangeAutoMoveState().StartPos(_local, DestPos, AcceptanceRadius,
		FGsAutoMoveCompletedDelegate::CreateRaw(this, &FGsSkillHandlerLocalPlayer::CallbackMoveToSkillEnd),
		FGsAutoMoveCompletedOneDelegate::CreateRaw(this, &FGsSkillHandlerLocalPlayer::CallbackMoveToSkillFailed));
}

void FGsSkillHandlerLocalPlayer::OnMoveToSkill(UGsGameObjectBase* Target, int ApproachRange)
{
	if (Target)
	{
		// RequsetData가 설정되어있는데 들어왔다면 오류
		check(false == IsRequestData());
		
		AActor* targetActor = Target->GetActor();
		if (targetActor)
		{
			// 대상 방향으로 회전 보간 처리
			_local->LookAtTarget(Target, true);

			float acceptanceRadius = ApproachRange;
			float targetRadius = Target->GetData()->GetScaledCapsuleRadius();
			acceptanceRadius += targetRadius;

			FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
			UGsCharacterMovementComponent* moveComponent = Cast<UGsCharacterMovementComponent>(movement->GetMovementComponent());
			FGsMovementStateAuto& moveStateAuto = movement->GetStateAutoMove();
			const FGsAutoMoveInfo& autoMoveInfo = moveStateAuto.GetData();

			// 1. 자동 이동 중일때
			if (autoMoveInfo.IsValid() && moveComponent->IsMoved())
			{
				// EGsMovementAutoContentsType::None 타입도 처리 한다. (MoveToSkill로 완료되었지만 사거리가 변한경우)
				EGsMovementAutoContentsType autoContentType = moveStateAuto.GetMovementAutoContentsType();
				if (autoContentType == EGsMovementAutoContentsType::MoveToSkill ||
					autoContentType == EGsMovementAutoContentsType::None)
				{
					// 2. 타겟 변경이 되었을때
					if (autoMoveInfo._moveRequest.IsMoveToActorRequest())
					{
						AActor* goalActor = autoMoveInfo._moveRequest.GetGoalActor();
						if (goalActor && goalActor != targetActor)
						{
							movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::MoveToSkill);
							movement->ChangeAutoMoveState().StartActor(_local
								, Target->GetActor(), acceptanceRadius,
								FGsAutoMoveCompletedDelegate::CreateRaw(this, &FGsSkillHandlerLocalPlayer::CallbackMoveToSkillEnd),
								FGsAutoMoveCompletedOneDelegate::CreateRaw(this, &FGsSkillHandlerLocalPlayer::CallbackMoveToSkillFailed));
							return;
						}
					}

					// 3. 스킬을 사용하기위해 이동중 다른 스킬로 교체
					if (false == FMath::IsNearlyEqual(moveStateAuto.GetDataRequest().GetAcceptanceRadius(), acceptanceRadius
					))
					{
						movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::MoveToSkill);
						moveStateAuto.SetAcceptanceRadius(acceptanceRadius);
						movement->GetStateAutoMove().Start(_local, autoMoveInfo);
						return;
					}

					// 이동중이므로 무시
					return;
				}
				else
				{					
					movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::MoveToSkill);
					movement->ChangeAutoMoveState().StartActor(_local
						, targetActor, acceptanceRadius,
						FGsAutoMoveCompletedDelegate::CreateRaw(this, &FGsSkillHandlerLocalPlayer::CallbackMoveToSkillEnd),
						FGsAutoMoveCompletedOneDelegate::CreateRaw(this, &FGsSkillHandlerLocalPlayer::CallbackMoveToSkillFailed));

#if WITH_EDITOR
					GSLOG(Log, TEXT("[DebugGameObjectLog] MoveTo StartActor"));
#endif
					return;
				}
			}
			// 1. 자동 이동중이 아닐때
			// 2. 자동 이동 타입이 EGsMovementAutoContentsType::MoveToSkill 아닐때
			// 3. 같은 대상이 아닐때
			// 
			// 스킬로 인한 자동 이동 중인데 같은 대상을 향해 갈때 아래 처리를 안하기 위함
			// (CallbackMoveToSkillEnd, CallbackMoveToSkillFailed 이 호출되는 상황을 막음)
			else if (moveStateAuto.GetMovementAutoContentsType() != EGsMovementAutoContentsType::MoveToSkill
				|| autoMoveInfo._moveRequest.GetGoalActor() != targetActor)
			{
				movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::MoveToSkill);
				movement->ChangeAutoMoveState().StartActor(_local
					, Target->GetActor(), acceptanceRadius,
					FGsAutoMoveCompletedDelegate::CreateRaw(this, &FGsSkillHandlerLocalPlayer::CallbackMoveToSkillEnd),
					FGsAutoMoveCompletedOneDelegate::CreateRaw(this, &FGsSkillHandlerLocalPlayer::CallbackMoveToSkillFailed));

#if WITH_EDITOR
				GSLOG(Log, TEXT("[DebugGameObjectLog] MoveTo StartActor"));
#endif
				return;
			}

			// [B.Y] 여기에 들어오면 안되는 상황이지만, 
			// 가끔  FGsMovementStateAuto와 FGsStateLocalPlayerAutoMove 상태 동기화가 안맞을때가 있다.
			// 심오한 부분이므로 일단 진행 가능하게 처리해두고, 추후 확인이 필요한 부분
			movement->GetStateAutoMove().Start(_local, autoMoveInfo);
			// movement->ChangeAutoMoveState().Start(_local, autoMoveInfo, false);

#if WITH_EDITOR
			GSLOG(Log, TEXT("[DebugGameObjectLog]  autoMoveInfo.IsValid()"));
#endif
		}
	}
}

void FGsSkillHandlerLocalPlayer::CallbackMoveToSkillFailed(const FPathFollowingResult& Result)
{
	 FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	 if (movement->GetStateAutoMove().GetMovementAutoContentsType() == EGsMovementAutoContentsType::MoveToSkill)
	 {
		 _local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);

		 if (Result.Code == EPathFollowingResult::Type::Invalid)
		 {
			 ClearTargetAtSkillFailure();
		 }

		 GSLOG(Log, TEXT("[DebugGameObjectLog] CallbackMoveToSkillFailed"));
	 }
}

void FGsSkillHandlerLocalPlayer::CallbackMoveToSkillEnd()
{
	FGsMovementHandlerLocalPlayer* movement = _local->GetCastMovement<FGsMovementHandlerLocalPlayer>();
	if (nullptr == movement)
	{
		return;
	}

	if (movement->GetStateAutoMove().GetMovementAutoContentsType() == EGsMovementAutoContentsType::MoveToSkill)
	{
		_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);
		movement->SetMovementAutoContentsType(EGsMovementAutoContentsType::None);

		if (FGsAIManager* aiManager = GSAI())
		{
			if (aiManager->IsAIOn())
			{
				return;
			}
		}

		GSLOG(Log, TEXT("[DebugGameObjectLog] CallbackMoveToSkillEnd"));
		// 예약 스킬 확인 
		OnReserveSkill();
	}
}

void FGsSkillHandlerLocalPlayer::CallbackChangeSkillSet(const IGsMessageParam* Param)
{
	if (_owner)
	{
		LoadSkill();

		// 무기 교체 이후 자동 공격이 필요한 상황 설정
		// 평타 스킬에 0.1초 쿨타임이 있어, Idle 상황에서 처리만으론 불가하여,
		// 평타 쿨타임까지의 딜레이 처리를 한번 더해준다.
		if (_autoSkillTimerHandle.IsValid())
		{
			_owner->GetWorld()->GetTimerManager().ClearTimer(_autoSkillTimerHandle);
		}

		if (_listComboSkillData.Num() > 0)
		{
			const FGsSkill* normalSkill = GSkill()->FindSkill(_listComboSkillData[0]->id);
			_owner->GetWorld()->GetTimerManager().SetTimer(_autoSkillTimerHandle, FTimerDelegate::CreateLambda([this]() {
				OnManualModeTargetAttackAction();
				}), normalSkill->GetCooldownTime(1), false);
		}
	}
	else
	{
		// Initialize 호출보다 먼저 들어 왔거나, 소멸 (Finalize()) 이후 처리된듯...
		checkf(_owner, TEXT("not valid GameObject Pointer"));
	}
}

void FGsSkillHandlerLocalPlayer::CallbackAIModeActive(const IGsMessageParam* Param)
{
	const FGsPrimitiveInt32* castParam = Param->Cast<const FGsPrimitiveInt32>();
	// 퀘스트 자동진행 중 AUTO 켜지면  멈추기 위해서 AUTO 킬때도 STOP 처리 한다(김힘찬)
	//if (castParam->_data == 0)
	//{
		_local->GetBaseFSM()->ProcessEvent(EGsStateBase::AutoMoveStop);
		ClearReserveSkill();
	//}
}

FGsSkillHandlerLocalPlayer::AttackRangeInsideType FGsSkillHandlerLocalPlayer::IsInsideSkillRange(
	const FGsSchemaSkillSet* SkillData) const
{
	FGsTargetHandlerBase* targetMgr = _local->GetTargetHandler();
	return IsInsideSkillRangeTarget(SkillData, targetMgr->GetTarget());
}

FGsSkillHandlerLocalPlayer::AttackRangeInsideType FGsSkillHandlerLocalPlayer::IsInsideSkillRangeTarget(
	const FGsSchemaSkillSet* SkillData, 	UGsGameObjectBase* In_target) const
{
	if (nullptr == SkillData || nullptr == In_target)
	{
		return OutSide;
	}	

	float Dist = (In_target->GetLocation2D() - _local->GetLocation2D()).Size();
	float Distance = Dist - In_target->GetData()->GetScaledCapsuleRadius();
	if (Distance <= FGsSkillHandlerBase::GetSkillAttackRange(_local, SkillData))
	{
		if (Distance < 0.f)
		{
			return OverLap;
		}

		return InSide;
	}
		
	return OutSide;
}

bool FGsSkillHandlerLocalPlayer::CheckNavMeshRaycast(const FVector& SrcPos, const FVector& DesPos)
{
	AGsCharacterBase* character = _local->GetCharacter();
	const UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(_local->GetWorld());
	const ANavigationData* NavData = NavSys->GetNavDataForProps(character->GetNavAgentPropertiesRef());

	const FSharedConstNavQueryFilter& QueryFilter = NavData->GetDefaultQueryFilter();
	const ARecastNavMesh* RecastNavMesh = Cast<const ARecastNavMesh>(NavData);

	FVector outDetectPos;
	ARecastNavMesh::FRaycastResult hitResult;
	if (false == ARecastNavMesh::NavMeshRaycast(RecastNavMesh, SrcPos, DesPos, outDetectPos, QueryFilter,
		character, hitResult))
	{
		return true;
	}

	return false;
}

bool FGsSkillHandlerLocalPlayer::IsComboSkill(int SkillId) const
{
	return (_listComboSkillData.Num() > 0 && _listComboSkillData[0]->id == SkillId);
}

bool FGsSkillHandlerLocalPlayer::IsCasting() const
{
	const FGsCastingSkillRequest& requestData = _skillDataBuilder.GetCastingData();
	return requestData.IsVaild() && false == requestData._castingComplete;
}

bool FGsSkillHandlerLocalPlayer::IsCastingComplete() const
{
	const FGsCastingSkillRequest& requestData = _skillDataBuilder.GetCastingData();
	return requestData.IsVaild() && requestData._castingComplete;
}

//UGsSkillGuideEffectComponent* FGsSkillHandlerLocalPlayer::GetSkillGuideComponent()
//{
//	if (nullptr == _cursorSkillInfo._skillGuideComponent)
//	{
//		_cursorSkillInfo._skillGuideComponent = Cast<UGsSkillGuideEffectComponent>(_local->GetActor()->GetComponentByClass(
//			UGsSkillGuideEffectComponent::StaticClass()));
//	}
//	return _cursorSkillInfo._skillGuideComponent;
//}

bool FGsSkillHandlerLocalPlayer::SendBuffSkill(const FGsSkill* SkillData)
{
	// 버프타입이 아닌게 왔다면 오류
	check(SkillData->IsBuffSkill());

	int32 skillId = SkillData->GetSkillId();
	// 중복 확인
	if (_buffSkillRunner->IsValid())
	{
		if (_buffSkillRunner->GetData()->id == SkillData->GetSkillId())
		{

#if WITH_EDITOR
			GSLOG(Log, TEXT("[DebugGameObjectLog] SendBuffSkill Req failed %d"), skillId);
#endif
			return false;
		}
	}
	
	// 패킷 전송
	FGsNetSendServiceWorld::SendStartSkill(skillId, _local->GetLocation(),
		FGsDir::dirToAngle(_local->GetRotation().Vector()), FVector::ZeroVector, FVector::ZeroVector, 0, false);

	// BuffRunner 발동 대기 상태
	_buffSkillRunner->SetReady(SkillData->_tableData);

#if WITH_EDITOR
  	GSLOG(Log, TEXT("[DebugGameObjectLog] [ %s ]  SendBuffSkill Req %d"), *FGsDateTimeUTIL::GetCurrentTime(), skillId);
#endif

	return true;
}

bool FGsSkillHandlerLocalPlayer::SendSkill(UGsGameObjectBase* TargetObject, const FGsSkill* SkillData)
{
	// Target 설정이 nullptr일수 없다.
	check(TargetObject);

	// RequsetData가 설정되어있는데 들어왔다면 오류
	check(!IsRequestData());
	// 버프타입이 왔다면 오류
	check(!SkillData->IsBuffSkill());
	// RequsetData 설정
	_skillDataBuilder.SetRequestData(SkillData);
	
	FVector dir = _local->GetRotation().Vector();
	if (TargetObject != _local)
	{
		dir = FVector((TargetObject->GetLocation2D() - _local->GetLocation2D()).GetSafeNormal(), 0.f);
	}

	const FGsSchemaSkillSet* skillTable = SkillData->_tableData;
	int32 skillId						= SkillData->GetSkillId();
	const FVector& pos					= _local->GetLocation();
	int16 angle							= FGsDir::dirToAngle(dir);
	const FVector& offset				= dir * skillTable->collisionInfoList.Last().posOffset;
	int64 targetId						= TargetObject->GetGameId();
	FVector targetPos					= FVector::ZeroVector;

	// SkillAreaCenter::TARGET 처리
	if (skillTable->collisionInfoList.Last().centerType == SkillAreaCenter::TARGET)
	{
		// 수동 개입이 없는경우 approachRange(최대 사용 사거리) 판단
		if (false == IsPressType(FGsSkillHandlerLocalPlayer::Joystick))
		{
			float approachRange = FGsSkillHandlerBase::GetSkillApproachRange(_local, skillTable);
			float distance = (TargetObject->GetLocation2D() - _local->GetLocation2D()).SizeSquared();
			if (distance < approachRange * approachRange)
			{
				targetPos = TargetObject->GetLocation();
			}
		}

		// 타겟이 없을경우(타겟정보가 내플레이어) 현재 방향의 최대 사거리로 설정
		if (TargetObject == _local)
		{
			float approachRange = FGsSkillHandlerBase::GetSkillApproachRange(_local, skillTable);
			targetPos = _local->GetRotation().Vector() * approachRange + _local->GetLocation();
			//	// 지형 높이 보정
			FHitResult hit;
			UGsLevelUtil::TryLineTraceToLand(hit, _local->GetWorld(), targetPos);
			targetPos = hit.Location;
		}
		//DrawDebugSphere(_local->GetWorld(), targetPos, 50.f, 100, FColor::Red, false, 2.f);
	}
	else
	{
		targetPos = TargetObject->GetLocation();
	}

	// 패킷 전송
	FGsNetSendServiceWorld::SendStartSkill(skillId, pos, angle, offset, targetPos, targetId, false);		

	// 예약 데이터 제거
	ClearReserveSkill(skillId);

	// 매너 모드 스킬 전송 세팅
	GSAI()->SendStartSkill(targetId);

#if WITH_EDITOR
  	GSLOG(Log, TEXT("[DebugGameObjectLog] [ %s ] SendSkill Req %d"), *FGsDateTimeUTIL::GetCurrentTime(), skillId);
#endif

	return true;
}

void FGsSkillHandlerLocalPlayer::NetActiveSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData)
{

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[DebugGameObjectLog] [ %s ] StartSkill Ack %d, %d"), *FGsDateTimeUTIL::GetCurrentTime(),
		Packet->SkillId(), Packet->SkillStartType());
#endif
	Super::NetActiveSkillStart(Packet, skillData);

	_skillfailedCount.Value = 0;	

	// 기본 콤보정보 초기화
	SetNextComboData(nullptr);
	//

	const FGsSkill* skillTarget = GSkill()->FindActiveSkill(Packet->SkillId());

	FGsTargetHandlerLocalPlayer* targetHandler = _local->GetCastTarget<FGsTargetHandlerLocalPlayer>();
	// 스킬 사용 성공일 경우 임의 지정 대상 정리
	if (targetHandler->GetSkillSelectTarget() != 0)
	{
		if (skillTarget && skillTarget->IsSelectTarget())
		{
			targetHandler->ClearSkillSelectTarget();
		}
	}

	// 캐릭터 회전 처리
	FRotator rot = FGsDir::AngleToRotator(Packet->CurrDir());
	// 루트 모션 스킬 회전 보간 예외처리
	bool isSmoothRotation = false;
	// if (realSkillData->_tableData->rootMotionId.IsNull()) //IsNull 이 동작 안하는것 같다 ㅜ
	const FGsSchemaRootmotionSet* rootmotionSet = skillData->rootMotionId.GetRow();
	if (nullptr == rootmotionSet && skillData->smoothRotaion)
	{
		isSmoothRotation = true;
	}
	_local->LookAt(rot, isSmoothRotation);

	bool bIsCasted = false;

	if (skillData->category == SkillCategory::COMBO)
	{
		// 다음 콤보 정보 설정
		if (skillData->sequenceInfo.comboInfo.currentStage < skillData->sequenceInfo.maxStage)
		{
			int nextComboId = skillData->sequenceInfo.comboInfo.nextComboSkillId;
			if (const FGsSkill* nextCombo = GSkill()->FindActiveSkill(nextComboId))
			{
				SetNextComboData(nextCombo->_tableData);
			}
		}
	}
	// 캐스팅 스킬 
	else if (skillData->category == SkillCategory::CAST)
	{
		if (Packet->SkillStartType() == SkillStartType::SKILL)
		{
			// 해당 위치로 강제 이동
			// 스킬 전체를 적용시킬것인지 추후 고려
// 			if (FGsMovementHandlerBase* movementHandler = _local->GetMovementHandler())
// 			{
// 				FVector pos = Packet->CurPos();
// 				FVector dir = FGsDir::angleToDir(Packet->CurrDir());
// 				movementHandler->DirectMoveUpdate(pos, dir);
// 			}

			// 캐스팅 완료 처리
			ClearCastingData(Packet->SkillId());
			bIsCasted = true;
		}
		else
		{
			// 주의 캐스팅 구간은 Notify처리가 안됨...(Loop 처리로 인해 SkillRunner를 태우지 않음)
			// 캐스팅 정보 설정
			_skillDataBuilder.SetCastingData(skillData->id);

			// 캐스팅 상태 전환
			FGsGameObjectStateManager::ProcessEvent(_local, EGsStateBase::Casting);

			// 자동 전투 시 스킬 버튼 연출을 위한 메시지
			if (skillTarget)
			{
				FGsGameObjectMessageParamSkillSlot param(skillTarget->GetSkillRequireWeaponType(), skillTarget->GetSlotId(), skillTarget);
				GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_SKILL_START_FOR_EFFECT, &param);
			}

			return;
		}
	}

	if (false == bIsCasted)
	{
		// 자동 전투 시 스킬 버튼 연출을 위한 메시지
		if (skillTarget)
		{
			FGsGameObjectMessageParamSkillSlot param(skillTarget->GetSkillRequireWeaponType(), skillTarget->GetSlotId(), skillTarget);
			GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_SKILL_START_FOR_EFFECT, &param);
		}
	}

	GSkill()->NetStartSkill(Packet);

	// 선판정 스킬 체크
	// 선판정의 경우는 NetSkillEnd 에서 상태변경을 진행한다,
	if (false == skillData->noTimeline)
	{
		FGsSkillRunnerBase* skillRunner = GetSkillRunner();
		float fSpeedRate = skillData->isNormal ? GetAttackSpeedRate() : GetCastSpeedRate();

		// 스킬 실행기 Start
		skillRunner->StartRunner(_local, skillData, fSpeedRate, GetNetSkillData());

		FGsGameObjectStateManager::ProcessEvent(_local, EGsStateBase::Attack);

		// 싱크 데이터 정리
		ClearRequestSkill(skillData->id);
	}
}

void FGsSkillHandlerLocalPlayer::NetActiveSkillEnd(PD::SC::PKT_SC_SKILL_FINISHED_READ* Packet, const FGsSchemaSkillSet* skillData)
{
	// 싱크 데이터 정리
	ClearRequestSkill(skillData->id);

	// 캐스팅 스킬의 경우 캐스팅 완료후 StartSkill을 못받을수 있으므로 캐스팅 정보 정리
	ClearCastingData(skillData->id);

	// 스킬 상태로 전환 요청
	if (Packet->SkillFinishReason() == (uchar)SkillFinishReason::NO_TIME_LINE)
	{
		ensure(skillData->noTimeline);

		FGsSkillRunnerBase* skillRunner = GetSkillRunner();
		float fSpeedRate = skillData->isNormal ? GetAttackSpeedRate() : GetCastSpeedRate();

		// 스킬 실행기 Start
		skillRunner->StartRunner(_local, skillData, fSpeedRate, GetNetSkillData());

		FGsGameObjectStateManager::ProcessEvent(_local, EGsStateBase::Attack);
	}
	else
	{
		// 캐스팅 완료 보장이 되지 않고 중간에 SC_SKILL_FINISHED가 내려올수 있음
		// 상태 회복 처리
		if (FGsGameObjectStateManager* fsm = _local->GetBaseFSM())
		{
			fsm->ProcessEvent(EGsStateBase::AttackEnd);
		}
	}

	GSkill()->NetSkillFinished(Packet);

#if WITH_EDITOR
	GSLOG(Warning, TEXT("[DebugGameObjectLog] [ %s ] SC_SKILL_FINISHED SkillId : %d  Reson : %d Reserve Id : %d"),
		*FGsDateTimeUTIL::GetCurrentTime(), Packet->SkillId(), Packet->SkillFinishReason(), skillData->id);
#endif
}

void FGsSkillHandlerLocalPlayer::NetBuffSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData)
{
	Super::NetBuffSkillStart(Packet, skillData);
	// buffRunner 시작
	_buffSkillRunner->StartRunner(skillData);
}

void FGsSkillHandlerLocalPlayer::NetSkillFailure(uint32 Id, PD::Result ErrResult)
{
	Super::NetSkillFailure(Id, ErrResult);

	// 싱크 데이터 정리
	ClearRequestSkill(Id);

	// 이타입의 실패 상황에서는 유효 네비 패스 포인트를 찾고 안되면 타겟 좌표로 이동하는 방법으로 처리한다.
	if (ErrResult == PD::Result::SKILL_ERROR_UNREACHABLE_STRAIGHT)
	{
		FGsTargetHandlerBase* targethandler = _local->GetTargetHandler();
		if (UGsGameObjectBase* target = targethandler->GetTarget())
		{
			float acceptanceRadius = target->GetData()->GetScaledCapsuleRadius() +
				_local->GetData()->GetScaledCapsuleRadius();

			if (++_skillfailedCount.Value > 5)
			{
				// 타겟의 무작위 방향을 잡아 시도해 본다.
				FRotator randomRot = target->GetRotation() + FRotator(0.f, FMath::RandRange(1.f, 350.f), 0.f);
				FVector destPos = randomRot.Vector().GetSafeNormal() * (acceptanceRadius + 1.f) + target->GetLocation();
				OnMoveToSkill(destPos, acceptanceRadius);
			}
			else
			{
				FVector destPos = FindNaviPathValidPoint(target);
				OnMoveToSkill(destPos, acceptanceRadius);
			}
		}
	}
	else
	{
		// 동일한 에러를 여러번 내려주면 더이상 진행하는건 의미 없다고 판단해 정지 시킨다.
		if (ErrResult == _skillfailedCount.Key)
		{
			// 특정 에러 타입 특수화, 추후 데이터 드리븐으로 변경이 필요할수 있음
			int skillfailedCountMax = 10;
			switch (ErrResult)
			{
			case PD::Result::SKILL_ERROR_CHECK_COLLISION_FOR_SKILL:
				skillfailedCountMax = 3;
				break;
			default:
				break;
			}

			if (++_skillfailedCount.Value > skillfailedCountMax)
			{
				ClearTargetAtSkillFailure();
				ClearReserveSkill(Id);
			}
		}
		else
		{
			_skillfailedCount.Key = ErrResult;
			_skillfailedCount.Value = 1;
		}
		// SC_ACK_START_SKILL_FAILURE 패킷은 스킬을 시작하기도 전에만 내려주는 패킷이므로 (서버 확인 완료)
		// 클라이언트에서 현재 진행 중인 스킬에 대한 처리를 하면 안된다.
		// 현재 모든 클라이언트 스킬 연출은 서버 응답을 받은후 처리.
		// 
	// 
	// 	if (_skillRunner->IsValid())
	// 	{
	// 		if (_skillRunner->GetSkillData()->id == Id)
	// 		{
	// 			// 강제 종료
	// 			CompulsionCancelSkill(Id);
	// 
	// 			// 강제 쿨타임 딜레이 설정
	// 			// 동일 스킬에 대한 데이터등 여러 상황들에 의해 계속 스킬이 사용 하는것 처럼 보이는 현상을 완화하기 위한 처리이지만
	// 			// 위험해 보임.. 추후 제거하는 방향으로
	// 			if (false == GCoolTime()->IsCoolTime(EGsCoolTime::Skill, Id))
	// 			{
	// 				GCoolTime()->SetCoolTime(EGsCoolTime::Skill, 0, Id, 1.f);
	// 			}
	// 		}
	// 	}
	}

	// 캐스팅 완료 보장이 되지 않고 중간에 실패가 내려올수 있음
	// 상태 회복 처리
	if (FGsGameObjectStateManager* fsm = _local->GetBaseFSM())
	{
		if (fsm->IsState(EGsStateBase::Casting))
		{
			fsm->ProcessEvent(EGsStateBase::AttackEnd);
		}
	}


	FString errMessage = PD::ResultEnumToString(ErrResult);
#if WITH_EDITOR
#pragma todo("스킬과 평타 메세지 분리가 작업이 필요함")
	// 실패 메세지 처리
	if (false == errMessage.IsEmpty())
	{
		if (_local && false == _local->IsSpectatorMode())
		{
			// [R3] | ejrrb10 | 플레이어의 HP 가 0이 되었을 때는 티커 메시지 출력 X (CHR-16929)
			if (FGsGameObjectDataCreature* creatureData = _local->GetGameObjectDataCreature())
			{
				if (false == creatureData->GetStatBase()->IsZeroHP())
				{
					FText result;
						FText::FindText(TEXT("NetText"), FTextKey(errMessage), result);
						FGsUIHelper::TrayMessageTicker(result);
				}
			}
		}
	}
	

	GSLOG(Warning, TEXT("[DebugGameObjectLog] [ %s ] SkillFailure Ack %d %s"),
		*FGsDateTimeUTIL::GetCurrentTime(), Id, *errMessage);
#endif
}

// 예약 데이터 있는가
bool FGsSkillHandlerLocalPlayer::IsReserveData()
{
	const FGsSkillReserve& reserveData = _skillDataBuilder.GetReserveData();
	return reserveData.IsVaild();
}

bool FGsSkillHandlerLocalPlayer::IsRequestData() const
{
	const FGsUseSkillRequest& requestData = _skillDataBuilder.GetRequestData();
	return requestData.IsVaild();
}

// 가장 적절한 사용 타이밍을 체크
bool FGsSkillHandlerLocalPlayer::IsUsefulSkill(int tId)
{
	if (_mapSkillUsefulTime.Contains(tId))
	{
		// 버프가 발동 중인가 확인
		if (FGsAbnormalityHandlerBase* abnormalityhandler = _local->GetAbnormalityHandler())
		{
			if (abnormalityhandler->IsAbnormalityId(_mapSkillUsefulTime[tId]))
			{
				return false;
			}
		}
		// 쿨타임 중인가 확인
		return false == GCoolTime()->IsCoolTime(EGsCoolTime::Skill, tId);
	}
	else
	{
		// 처음 등록하는 스킬은 Notify를 검색하여 가징 긴 remainTime을 찾는다.
		const FGsSchemaAbnormalitySet* lastData = nullptr;
		const FGsSkill* skillData = GSkill()->FindSkill(tId);
		if (nullptr == skillData)
		{
			return false;
		}

		for (const FGsSchemaSkillNotify& el : skillData->_tableData->notifyInfoList)
		{
			const FGsSchemaSkillNotifySet* notifyTable = el.skillNotifyId.GetRow();
			if (nullptr == notifyTable) 
			{
				continue;
			}

			// [B.Y] 일단 처음 것만 반영
			const FGsSchemaSkillNotifyElement* element = notifyTable->notifyElementList[0].GetRow();
			const FGsSchemaSkillNotifyHitSet* hitSet = element->typeInfo.GetRowByType<FGsSchemaSkillNotifyHitSet>();
			if (nullptr == hitSet)
			{
				continue;
			}

			int abnormalityId = hitSet->abnormalityId1;
			if (0 >= abnormalityId)
			{
				continue;
			}

			abnormalityId = hitSet->abnormalityId2;
			if (0 >= abnormalityId)
			{
				continue;
			}

			const FGsSchemaAbnormalitySet* abnormalityTable = UGsTableUtil::FindRowById<
				UGsTableAbnormalitySet, FGsSchemaAbnormalitySet>(abnormalityId);
			if (nullptr == abnormalityTable)
			{
				continue;
			}

			// 버프형만 따로 검색	
			if (false == abnormalityTable->isBuff)
			{
				continue;
			}

			if (nullptr == lastData || abnormalityTable->durationMax > lastData->durationMax)
			{
				lastData = abnormalityTable;
			}
		}

		_mapSkillUsefulTime.Emplace(tId, lastData ? lastData->id : 0);
		return IsUsefulSkill(tId);
	}
	return false;
}

// 재시도 해라
void FGsSkillHandlerLocalPlayer::DoRetry()
{
	// GSLOG(Error, TEXT("[LocalSkillDebug] DoRetry()"));
	if (false == IsRequestData())
	{
		OnReserveSkill();
	}
}

const FGsSchemaSkillSet* FGsSkillHandlerLocalPlayer::GetNextAutoSkill(SkillCategorySet Type, SkillId tId)
{
	// 1. 대상 스킬 찾기
	const FGsSkill* targetSkill = nullptr;
	if (INVALID_SKILL_ID != tId)
	{
		// 자동 사용 대상인지 체크
		const FGsSkill* skill = GSkill()->FindSkill(tId);
		if (CanAutoUseSkill(skill))
		{
			targetSkill = skill;
		}
	}
	else
	{
		const TMap<SkillSlotId, const FGsSkill*>* slotMap = GSkill()->GetCurrentSkillSlotMap();
		if (slotMap)
		{
			TArray<const FGsSkill*> listSkill;
			slotMap->GenerateValueArray(listSkill);

			listSkill.RemoveAll([Type](const FGsSkill* el) {
				return ((nullptr != el && el->GetSkillCateogrySet() != Type));
				});

			// 액티브 스킬 타입은 정렬조건이 필요하다.
			// 1. 진행중인 체인 스킬
			// 2. 코스튬 스킬
			// 3. 평타 스킬은 가장 후순위
			if (Type != SkillCategorySet::BUFF)
			{
				listSkill.Sort([](const FGsSkill& el1, const FGsSkill& el2)
					{
						//1
						if ((el1.IsChainSkill() && el1.GetChainStep() != 1))
							return true;
						if ((el2.IsChainSkill() && el2.GetChainStep() != 1))
							return false;

						//2
						if (el1.IsCostumeSkill())
							return true;
						if (el2.IsCostumeSkill())
							return false;

						//3
						if (el1.IsNormalSkill())
							return false;
						if (el2.IsNormalSkill())
							return true;

						return el1.GetSlotId() < el2.GetSlotId();
					});
			}

			for (const FGsSkill* skillInfo : listSkill)
			{
				if (CanAutoUseSkill(skillInfo))
				{
					targetSkill = skillInfo;
					break;
				}
			}
		}
	}

	if (nullptr == targetSkill)
	{
		return nullptr;
	}

	return targetSkill->_tableData;
}

bool FGsSkillHandlerLocalPlayer::CanCancel()
{
	if (IsCasting())
	{
		return false;
	}

	if (FGsSkillRunnerBase* skillRunner = GetSkillRunner())
	{
		return false == skillRunner->IsValid() || skillRunner->CanCancel();
	}
	
	return false;
}

bool FGsSkillHandlerLocalPlayer::CanAutoUseSkill(const FGsSkill* InSkill)
{
	if (nullptr == InSkill)
	{
		return false;
	}	

	// 검사 부하가 적어보이는 순서대로 검사

	// 1. 오토 설정된 스킬만 가능 (평타 예외)
	if ((false == InSkill->IsNormalSkill()) && (false == InSkill->IsAuto()))
	{
		return false;
	}
	
	// 2. 스킬 사용 가능여부 판단
	if (false == InSkill->IsEnable())
	{
		return false;
	}

	// 3. 스킬 사용 주기 설정 여부 및 사용 가능 검사
	if (InSkill->IsActiveSkillCycle())
	{
		// 1차 타겟 대상도 확인이 필요하다.
		FGsTargetHandlerBase* targetHandler = _owner->GetTargetHandler();
		if (nullptr == targetHandler)
		{
			return false;
		}

		UGsGameObjectBase* target = targetHandler->GetTarget();
		if (nullptr == target || false == target->IsObjectType(EGsGameObjectType::Player)
			|| true == InSkill->IsActiveSkillCycleInPVP())
		{
			float elapsedTime = InSkill->GetCycleElapsedTime();
			if ((float)InSkill->GetCycleSecond() > elapsedTime)
			{
				return false;
			}
		}
	}

	// 4. 타겟UI 활성화된 스킬
	bool isTargetUiActive = GSkill()->GetIsShowSkillTargetSelect() && InSkill->GetSlotId() == GSkill()->GetCurrentSkillTargetSlotId();
	if (isTargetUiActive)
	{
		UGsGameUserSettings* gameUserSettings = GGameUserSettings();
		if (gameUserSettings == nullptr)
		{
			return true;
		}

		bool autoskillUseAddtargetButton =
			(bool)gameUserSettings->GetCombatSetting(EGsOptionCombat::AUTOSKILL_USE_ADDTARGETBUTTON);
		if (false == autoskillUseAddtargetButton)
		{
			return false;
		}
	}

	// 5. 타겟이 있다면 SkillSet에 설정된 Iff타입 비교
	// 액티브 스킬만 일단 검사
	//if (SharedFunc::GetSkillCategorySet(InSkill->_tableData->category) == SkillCategorySet::ACTIVE)
	//{
	//	FGsTargetHandlerBase* targethandler = _owner->GetTargetHandler();
	//	if (UGsGameObjectBase* target = targethandler->GetTarget())
	//	{
	//		IffApplyType ifftype = InSkill->_tableData->targetFilterInfo.iffType;
	//		switch (ifftype)
	//		{
	//			// case IffApplyType::SELF:
	//		case IffApplyType::ALL_WITHOUT_SELF:
	//			if (target != _local)
	//			{
	//				return false;
	//			}
	//			break;
	//		case IffApplyType::ENEMY:
	//			if (false == _local->IsEnemy(target))
	//			{
	//				return false;
	//			}
	//			break;
	//		case IffApplyType::PARTY:
	//			if (false == _local->IsParty(target))
	//			{
	//				return false;
	//			}
	//			break;
	//		case IffApplyType::ALLY:
	//			if (false == _local->IsAllyGuildIdMember(target))
	//			{
	//				return false;
	//			}
	//			break;
	//		case IffApplyType::GUILD:
	//			if (false == _local->IsGuildMemeber(target))
	//			{
	//				return false;
	//			}
	//			break;
	//		}
	//	}
	//}

	// 5. 버프가 존재하지 않거나 끝난 스킬만 가능
	if (IsExistAbnormality(InSkill))
	{
		return false;
	}

	// 퀘스트 시네마틱


	// 6. 옵션에서 세팅한 mp rate보다 현재 모자른가
	// (버프면서 항상 사용 옵션on이면 ok)
	// 옵션 rate 가져오기
	if (UGsGameUserSettings* gameUserSettings = GGameUserSettings())
	{
		const float mpRate =
			gameUserSettings->GetAutoSetting(EGsOptionAuto::AUTO_SKILL_MANA_LIMIT_RATE);

		const float isBuffAlwaysUseFloat =
			gameUserSettings->GetAutoSetting(EGsOptionAuto::IS_ALWAYS_USE_AUTO_BUFF_SKILL);

		bool isBuffAlwaysUse = (0.0f == isBuffAlwaysUseFloat) ? false : true;
		// 버프 항상 사용 on이면 사용
		if (InSkill->GetSkillCateogrySet() == SkillCategorySet::BUFF &&
			isBuffAlwaysUse == true && _enableDelayBuffSkill <= 0.f && _enableAutoBuffSkill)
		{
			return true;
		}
		// if the mana cost is zero, there is no need to check the mp rate
		if (0.0f == FGsSkillHandlerLocalPlayer::GetManaCost(InSkill))
		{
			return true;
		}

		// 현재 mp rate 체크
		float currentLocalMPRate = UGsGameObjectUtil::GetLocalMPRate();
		// 옵션값 보다 현재가 작으면 안나가게 처리
		if (currentLocalMPRate < mpRate)
		{
			return false;
		}
	}

	return true;
}

bool FGsSkillHandlerLocalPlayer::IsExistAbnormality(const FGsSkill* InSkill) const
{
	check(InSkill);
	// Category BUFF 타입만 확인
	if (InSkill->_tableData->category != SkillCategory::BUFF)
	{
		return false;
	}

	const FGsSchemaSkillLevelInfo* skillLevelInfo = InSkill->GetLevelInfo(InSkill->GetTotalLevel());
	if (nullptr == skillLevelInfo)
	{
		return false;
	}

	FGsAbnormalityHandlerBase* abnormalityHandler = _local->GetAbnormalityHandler();
	if (nullptr == abnormalityHandler)
	{
		return true;
	}

	for (int32 notifyId : skillLevelInfo->notifyInfoList)
	{
		const FGsSchemaSkillNotifySet* notifyTable = UGsTableUtil::FindRowById<
			UGsTableSkillNotifySet, FGsSchemaSkillNotifySet>(notifyId);
		if (nullptr == notifyTable)
		{
			continue;
		}
		// [B.Y] 일단 처음 것만 반영
		const FGsSchemaSkillNotifyElement* element = notifyTable->notifyElementList[0].GetRow();
		const FGsSchemaSkillNotifyHitSet* hitSet = element->typeInfo.GetRowByType<FGsSchemaSkillNotifyHitSet>();
		if (nullptr == hitSet)
		{
			continue;
		}

		int abnormalityId = hitSet->abnormalityId1;
		if (0 >= abnormalityId)
		{
			continue;
		}

		const FGsSchemaAbnormalitySet* abnormalityTable = UGsTableUtil::FindRowById<
			UGsTableAbnormalitySet, FGsSchemaAbnormalitySet>(abnormalityId);
		if (nullptr == abnormalityTable)
		{
			continue;
		}

		if (false == abnormalityTable->isBuff)
		{
			continue;
		}

		// 상태이상이 존재하면 false 리턴
		// groupId가 0이면 Id로 체크
		if (abnormalityTable->stackGroupId == 0)
		{
			return abnormalityHandler->IsAbnormalityId(abnormalityTable->id);
		}
		else
		{
			return abnormalityHandler->IsAbnormalityGroupId(abnormalityTable->stackGroupId);
		}
	}

	return false;
}

bool FGsSkillHandlerLocalPlayer::IsMySelfSkill(const FGsSchemaSkillSet* SkillData)
{
	if (SkillData)
	{
		IffApplyType iffAplly = SkillData->targetFilterInfo.iffType;
		if (iffAplly == IffApplyType::SELF ||
			iffAplly == IffApplyType::PARTY ||
			iffAplly == IffApplyType::ALLY)
		{
			return true;
		}
	}
	return false;
}

float FGsSkillHandlerLocalPlayer::GetManaCost(const FGsSkill* In_skill)
{
	if (nullptr == In_skill)
	{
		return 0.0f;
	}

	const FGsSchemaSkillCostConsumeInfo* consumeInfoData = In_skill->GetCurrLevelCostConsumeInfoData(In_skill->GetTotalLevel());
	if (nullptr == consumeInfoData)
	{
		return 0.0f;
	}
	TArray<FGsSchemaSkillStatInfo> statInfoList = consumeInfoData->statInfoList;
	for (uint8 i = 0; i < statInfoList.Num(); ++i)
	{
		StatType type = statInfoList[i].type;
		int32 value = statInfoList[i].value;

		if(StatType::MP == type)
		{
			return value;
		}		
	}
	return 0.0f;
}

void FGsSkillHandlerLocalPlayer::SetAutoIdleEmotionEnable(bool enbable)
{
	if (auto emotionHandler = StaticCastSharedPtr<FGsIdleEmotionLocalRunner>(_idleEmotionRunner))
		emotionHandler->SetEnableAutoIdleEmotionMotion(enbable);
}
