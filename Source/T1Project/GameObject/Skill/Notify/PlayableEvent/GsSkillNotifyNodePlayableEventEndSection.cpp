#include "GsSkillNotifyNodePlayableEventEndSection.h"

#include "Management/ScopeLobby/GsLobbyCharacterManager.h"

#include "GameObject/ObjectClass/GsGameObjectPlayableEventLocalPlayer.h"
#include "GameObject/ObjectClass/GsGameObjectPlayableEventNonPlayer.h"
#include "GameObject/Skill/GsSkillHandlerBase.h"

#include "GameObject/Target/GsTargetHandlerBase.h"

#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"

#include "UTIL/GsDir.h"

#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"

FGsSkillNotifyNodePlayableEventEndSection::FGsSkillNotifyNodePlayableEventEndSection(IffApplyType SkillIffType)
	: Super(SkillIffType)
{
}

void FGsSkillNotifyNodePlayableEventEndSection::Action(UGsGameObjectBase* Owner)
{
	UGsGameObjectPlayableEventLocalPlayer* local = LSLobbyCharacter()->GetPlayableEventLocalPlayer();
	if (local == nullptr)
	{
		return;
	}

	bool isLocal = (Owner == local) ? true : false;

	if (isLocal == false)
	{
		return;
	}

	FGsSkillNotifyNodeBase::Action(Owner);
	
	IffApplyType iffType = _skillIffType;
	if (const FGsSchemaSkillTargetFilter* targetFilterSet = _currNotifyData->targetFilterInfo.GetRow())
	{
		iffType = targetFilterSet->iffType;
	}

	PD::SC::PKT_SC_START_SKILL_READ packet;
	packet.mCasterId = Owner->GetGameId();
	packet.mSkillId = _netData->_skillId;
	packet.mSkillLevel = 1;
	packet.mCurPos = Owner->GetLocation();
	packet.mCurrDir = FGsDir::dirToAngle(Owner->GetRotation().Vector());
	packet.mTargetId = 0;
	packet.mSkillStartType = SkillStartType::SKILL;
	
	if (FGsSkillHandlerBase* skillHandler = Owner->GetSkillHandler())
	{
		skillHandler->NetSkillStart(&packet);
	}
}