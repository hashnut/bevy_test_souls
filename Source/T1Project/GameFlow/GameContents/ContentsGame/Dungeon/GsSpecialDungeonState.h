#pragma once

/**
* file		GsSpecialDungeonStatus.h
* @brief	특수 던전
* @author	PKT
* @date		2022/12/13
**/

#include "CoreMinimal.h"
#include "GsDungeonStateBase.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

class FGsSpecialDungeonState : public FGsDungeonStateBase
{
public:
	explicit FGsSpecialDungeonState() : FGsDungeonStateBase(DungeonCategory::SPECIAL) {/*None*/ }

	virtual bool Enter(MapId InMapId) override;
	virtual void Exit() override;

	virtual void HUDTimeUpdate(DungeonGroupId InGoupId = INVALID_DUNGEON_GROUP_ID) override;

	virtual UnlockCategory GetUnlockCategory() const override { return UnlockCategory::SPECIAL_DUNGEON; }

private:

	void SetEnvEffect(bool In_isStart);
};