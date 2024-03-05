#pragma once

/**
* file		GsNormalDungeonState.h
* @brief	일반 던전
* @author	PKT
* @date		2022/12/13
**/
#include "CoreMinimal.h"
#include "GsDungeonStateBase.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

class FGsNormalDungeonState : public FGsDungeonStateBase
{
public:
	explicit FGsNormalDungeonState() : FGsDungeonStateBase(DungeonCategory::NORMAL) {/*None*/ }

	virtual void HUDTimeUpdate(DungeonGroupId InGoupId = INVALID_DUNGEON_GROUP_ID) override;

	virtual UnlockCategory GetUnlockCategory() const override { return UnlockCategory::NORMAL_DUNGEON; }
};