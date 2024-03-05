
#include "GsSpecialDungeonState.h"

#include "../Dungeon/GsDungeonGroupBase.h"
#include "../Dungeon/GsDungeonGroupNormal.h"

#include "../Management/ScopeGame/GsDungeonManager.h"

#include "../Management/GsMessageHolder.h"

#include "../Message/GsMessageGameObject.h"
#include "../Message/MessageParam/GsMessageParam.h"

#include "DataSchema/GsSchemaEnums.h"


bool FGsSpecialDungeonState::Enter(MapId InMapId)
{
	if (false == FGsDungeonStateBase::Enter(InMapId))
	{
		return false;
	}

	SetEnvEffect(true);

	return true;
}

void FGsSpecialDungeonState::Exit()
{
	SetEnvEffect(false);

	FGsDungeonStateBase::Exit();
}

void FGsSpecialDungeonState::HUDTimeUpdate(DungeonGroupId InGoupId /* = INVALID_DUNGEON_GROUP_ID */)
{
	const DungeonGroupId id = GetGroupId();
	const DungeonCategory category = GetCategory();

	if (INVALID_DUNGEON_GROUP_ID != InGoupId && id != InGoupId)
	{	// 2023/12/11 PKT - 
		return;
	}

	const FGsDungeonGroupBase* dungeonGroup = GSDungeon()->FindGroupDataById(id);
	if (dungeonGroup && category == dungeonGroup->GetCategory())
	{
		const FGsDungeonGroupNormal* dungoneGroupData = StaticCast<const FGsDungeonGroupNormal*>(dungeonGroup);
		if (dungoneGroupData)
		{
			SendMessageHUDTimeUpdate(dungoneGroupData->GetTime(), dungoneGroupData->IsTimeLimit());
		}
	}
}

void FGsSpecialDungeonState::SetEnvEffect(bool In_isStart)
{
	if (FGsMessageHolder* messageMgr = GMessage())
	{
		FGsPrimitiveInt32 param(static_cast<int32>(EGsEnvEvent::SPECIAL_DUNGEON));

		if (In_isStart)
		{
			messageMgr->GetGameObject().SendMessage(MessageGameObject::ENV_START_EVENT, &param);
		}
		else
		{
			messageMgr->GetGameObject().SendMessage(MessageGameObject::ENV_END_EVENT, &param);
		}
	}
}