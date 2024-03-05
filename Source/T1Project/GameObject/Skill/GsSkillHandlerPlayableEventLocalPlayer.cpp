#include "GsSkillHandlerPlayableEventLocalPlayer.h"

#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeLobby/GsLobbyPlayableEventManager.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/Movement/LocalPlayer/GsMovementStateAuto.h"
#include "GameObject/Data/GsGameObjectData.h"
#include "GameObject/Skill/Runner/GsSkillRunnerPlayableEventLocalPlayer.h"
#include "GameObject/Skill/Runner/GsBuffSkillRunnerLocalPlayer.h"
#include "GameObject/State/GsGameObjectStateManager.h"

#include "Skill/GsSkill.h"

#include "ActorEx/GsCharacterBase.h"
#include "ActorComponentEx/GsHommingProjectileComponent.h"

#include "DataSchema/Skill/GsSchemaSkillSet.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsDir.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Core/Public/Misc/DateTime.h"

void FGsSkillHandlerPlayableEventLocalPlayer::Initialize(UGsGameObjectBase* Owner)
{
	Super::Initialize(Owner);

	_maxTimeTick =
		GData()->GetGlobalData()->_playableEventSkillGlobalCoolTime * ETimespan::TicksPerSecond;
}

bool FGsSkillHandlerPlayableEventLocalPlayer::OnSkill(int ID)
{
	if (nullptr == _owner)
	{
		return false;
	}

	if (true == _isGlobalCoolTime)
	{
		// _isReserveSkill = true;
		// _reserveSkillId = ID;
		return false;
	}

	FGsSkillRunnerBase* skillRunner = GetSkillRunner();
	if (false == skillRunner->CanCancel())
	{
		return false;
	}

	StartGlobalCooltime();
	const FGsSchemaSkillSet* skillData = UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(ID);
	if (nullptr == skillData)
	{
		return false;
	}

	UGsGameObjectPlayableEventLocalPlayer* playableLocal =
		Cast<UGsGameObjectPlayableEventLocalPlayer>(_owner);

	// 스킬 사용 가능한지 체크
	if (UGsGameObjectBase* target = LSLobbyPlayableEvent()->FindNearTarget())
	{
		float approachRange = skillData->approachRange;
		if (approachRange > 0.f)
		{
			// 타겟 설정
			FGsTargetHandlerLocalPlayer* targetHandler = playableLocal->GetCastTarget<FGsTargetHandlerLocalPlayer>();
			targetHandler->SetTarget(target);

			// 일반 스킬 사용 체크
			if (false == IsInsideSkillRange(skillData))
			{	
				// MoveToSkill 이면 예약후 종료
				_isReserveSkill = true;
				_reserveSkillId = ID;
				OnMoveToSkill(target, approachRange);
				return false;
			}
			else
			{
				// 가능 거리일 경우 대상방향으로 회전
				_owner->LookAtTarget(target, true);
			}
		}
	}

	PD::SC::PKT_SC_START_SKILL_READ packet;
	packet.mCasterId = _owner->GetGameId();
	packet.mSkillId = skillData->id;
	packet.mSkillLevel = 1;
	packet.mCurPos = _owner->GetLocation();
	packet.mCurrDir = FGsDir::dirToAngle(_owner->GetRotation().Vector());
	packet.mAimStartPos = FVector::ZeroVector;
	packet.mAimTargetPos = FVector::ZeroVector;
	packet.mTargetId = 0;
	packet.mReactionId = 0;

	SkillStartType startType = (skillData->category == SkillCategory::CAST) ?
		SkillStartType::CAST : SkillStartType::SKILL;

	packet.mSkillStartType = startType;

	NetSkillStart(&packet);

	ClearReserveSkill(skillData->id);

	return true;
}

bool FGsSkillHandlerPlayableEventLocalPlayer::IsReserveSkill() const
{
	return _isReserveSkill;
}

void FGsSkillHandlerPlayableEventLocalPlayer::ClearReserveSkill(uint32 Id)
{
	_isReserveSkill = false;
	_reserveSkillId = 0;
}

bool FGsSkillHandlerPlayableEventLocalPlayer::OnReserveSkill()
{
	if (IsReserveSkill())
	{
		return OnSkill(_reserveSkillId);
	}
	return false;
}

void FGsSkillHandlerPlayableEventLocalPlayer::CreateSkillRunner()
{
	_skillRunner = new FGsSkillRunnerPlayableEventLocalPlayer([this](int skillid, bool isEnd) { CallbackEndRunner(skillid, isEnd); });
}

void FGsSkillHandlerPlayableEventLocalPlayer::Update(float Delta)
{
	Super::Update(Delta);

	if (true == _isGlobalCoolTime)
	{
		int64 nowTick = FDateTime::UtcNow().GetTicks();
		int64 diffTick = nowTick - _startTimeTick;
		
		// 시간 종료
		if (diffTick >= _maxTimeTick)
		{
			_isGlobalCoolTime = false;
		}
	}
}

// 글로벌 쿨타임 시작
void FGsSkillHandlerPlayableEventLocalPlayer::StartGlobalCooltime()
{
	_isGlobalCoolTime = true;
	_startTimeTick = FDateTime::UtcNow().GetTicks();
}

void FGsSkillHandlerPlayableEventLocalPlayer::NetActiveSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData)
{
	Super::NetActiveSkillStart(Packet, skillData);

	// 기본 콤보정보 초기화
	SetNextComboData(nullptr);
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

	FGsSkillRunnerBase* skillRunner = GetSkillRunner();

	// 애님 몽타주의 재생 속도를 참조 한다.
	// 스킬 실행기 Start
	skillRunner->StartRunner(_local, skillData, 1.f, GetNetSkillData());

	FGsGameObjectStateManager::ProcessEvent(_local, EGsStateBase::Attack);
}

void FGsSkillHandlerPlayableEventLocalPlayer::NetBuffSkillStart(PD::SC::PKT_SC_START_SKILL_READ* Packet, const FGsSchemaSkillSet* skillData)
{
	Super::NetBuffSkillStart(Packet, skillData);

	_buffSkillRunner->StartRunner(skillData);
}

// 예약 데이터 있는가
bool FGsSkillHandlerPlayableEventLocalPlayer::IsReserveData()
{
	return IsReserveSkill();
}