#include "GsPlayableEventNonPlayerComponent.h"

#include "ActorEx/GsCharacterBase.h"

#include "Animation/GsAnimInstanceState.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Skill/GsSkillHandlerNonPlayer.h"
#include "GameObject/Skill/GsSkillHandlerPlayableEventNonPlayer.h"
#include "GameObject/Skill/Runner/GsSkillRunnerBase.h"
#include "GameObject/ObjectClass/GsGameObjectPlayableEventNonPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"

#include "UTIL/GsTableUtil.h"

#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/GameObject/GsSchemaCreatureCommonInfo.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"

#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Sequence/GsSequenceManager.h"
#include "Sequence/GsSequencePlayer.h"

#include "Shared/Client/SharedEnums/SharedMoveEnum.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"

#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Engine/Classes/Animation/AnimMontage.h"

UGsPlayableEventNonPlayerComponent::UGsPlayableEventNonPlayerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGsPlayableEventNonPlayerComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AGsCharacterBase* charBase = Cast<AGsCharacterBase>(GetOwner()))
	{
		USkeletalMeshComponent* meshPtr = charBase->GetMesh();

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

		anim->SetMoveSpeed(MotionType::NORMAL);
		anim->SetBattleIdleType(true);
	}

}

// 스킬 사용
void UGsPlayableEventNonPlayerComponent::UseSkill(TFunction<void(bool)> In_callbackSkillEnd)
{
	if (_playableEventNonPlayer == nullptr)
	{
		return;
	}
	const FGsSchemaSkillSet* skillTableData =
		UGsTableUtil::FindRowById<UGsTableSkillSet, FGsSchemaSkillSet>(_useSkillId);

	if (FGsSkillHandlerBase* skillhandler = _playableEventNonPlayer->GetSkillHandler())
	{	
		FGsSkillHandlerPlayableEventNonPlayer* nonPlayerSkillHandler =
			static_cast<FGsSkillHandlerPlayableEventNonPlayer*>(skillhandler);
		// net skill 정보 넣기
		nonPlayerSkillHandler->SetNetSkillData(_useSkillId);

		FGsSkillRunnerBase* runner = skillhandler->GetSkillRunner();

		float fSpeedRate = skillTableData->isNormal ? skillhandler->GetAttackSpeedRate() :
			skillhandler->GetCastSpeedRate();

		const FGsSkillNetData* netData = skillhandler->GetNetSkillData();
		runner->StartRunner(_playableEventNonPlayer, skillTableData, fSpeedRate, netData);
		
		if (FGsSkillHandlerPlayableEventNonPlayer* playableEventSkillHandler =
			static_cast<FGsSkillHandlerPlayableEventNonPlayer*>(skillhandler))
		{
			playableEventSkillHandler->SetCallbackSkillEnd(In_callbackSkillEnd);
		}
		
		if (UAnimMontage* resAni = runner->GetAni())
		{
			AGsCharacterBase* characterActor = _playableEventNonPlayer->GetCharacter();
			if (skillTableData->category == SkillCategory::COMBO)
			{
				characterActor->PlayAnimMontage(resAni, runner->GetSpeedScale(), skillTableData->sequenceInfo.comboInfo.sectionName);
			}
			else
			{
				characterActor->PlayAnimMontage(resAni, runner->GetSpeedScale());
			}
		}
		
	}
}

void UGsPlayableEventNonPlayerComponent::EndPlay(EEndPlayReason::Type inType)
{

	Super::EndPlay(inType);
}



void UGsPlayableEventNonPlayerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (_isAlive == true && nullptr != _playableEventNonPlayer)
	{
		_playableEventNonPlayer->Update(DeltaTime);
	}
}

bool UGsPlayableEventNonPlayerComponent::IsPlaySequence()
{
	bool isPlaying = false;
	if (UGsSequencePlayer* sequencePlayer = GSequencePlayer())
	{
		isPlaying =  sequencePlayer->IsPlaying();
	}

	return isPlaying;
}