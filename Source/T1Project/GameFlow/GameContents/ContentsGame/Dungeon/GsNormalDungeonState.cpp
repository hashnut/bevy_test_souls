
#include "GsNormalDungeonState.h"

#include "../Dungeon/GsDungeonGroupBase.h"
#include "../Dungeon/GsDungeonGroupNormal.h"

#include "../Management/ScopeGame/GsDungeonManager.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"



void FGsNormalDungeonState::HUDTimeUpdate(DungeonGroupId InGoupId /* = INVALID_DUNGEON_GROUP_ID */)
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