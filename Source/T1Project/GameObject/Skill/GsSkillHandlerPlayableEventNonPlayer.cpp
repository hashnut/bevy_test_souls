#include "GsSkillHandlerPlayableEventNonPlayer.h"

#include "GameObject/ObjectClass/GsGameObjectBase.h"
#include "GameObject/Define/GsGameObjectDefine.h"

#include "ActorEx/GsCharacterBase.h"

#include "DataSchema/Skill/GsSchemaSkillSet.h"

#include "GameObject/Skill/Runner/GsSkillRunnerPlayableEventLocalPlayer.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"

#include "Animation/GsAnimInstanceState.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "../../UTIL/GsDir.h"



void FGsSkillHandlerPlayableEventNonPlayer::CallbackEndRunner(int inSkillID, bool inIsEnd)
{
	Super::CallbackEndRunner(inSkillID, inIsEnd);

	// ani idle로 돌리자
	SetAniIdle();

	if (_callbackSkillEnd != nullptr)
	{
		_callbackSkillEnd(true);
	}

}

void FGsSkillHandlerPlayableEventNonPlayer::SetAniIdle()
{
	if (nullptr == _owner)
	{
		return;
	}

	AGsCharacterBase* characterActor = _owner->GetCharacter();
	
	if (nullptr == characterActor)
	{
		return;
	}

	USkeletalMeshComponent* meshPtr = characterActor->GetMesh();

	if (nullptr == meshPtr)
	{
		return;
	}

	UGsAnimInstanceState* anim = Cast<UGsAnimInstanceState>(meshPtr->GetAnimInstance());

	if (anim == nullptr)
	{
		return;
	}

	anim->ChangeState((int)(EGsStateBase::Idle), 0);
}

void FGsSkillHandlerPlayableEventNonPlayer::CreateSkillRunner()
{
	_skillRunner = new FGsSkillRunnerPlayableEventLocalPlayer([this](int skillid, bool isEnd) { CallbackEndRunner(skillid, isEnd); });
}

// net skill 데이터 채워 넣기
void FGsSkillHandlerPlayableEventNonPlayer::SetNetSkillData(int In_skillId)
{
	const FGsSchemaSkillSet* skillData = UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(In_skillId);
	if (nullptr == skillData)
	{
		return;
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

	// base 함수 호출로 저장만 한다
	FGsSkillHandlerBase::NetSkillStart(&packet);
}
