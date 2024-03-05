#pragma once

/**
* file		GsBattleArenaDungeonState.h
* @brief	일반 던전
* @author	PKT
* @date		2022/12/13
**/
#include "CoreMinimal.h"
#include "GsDungeonStateBase.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedUnlockEnum.h"

class FGsBattleArenaDungeonState : public FGsDungeonStateBase
{
public:
	using Super = FGsDungeonStateBase;

public:
	explicit FGsBattleArenaDungeonState() : FGsDungeonStateBase(DungeonCategory::ARENA) {/*None*/ }
	virtual ~FGsBattleArenaDungeonState() {}

	virtual bool Enter(MapId InMapId);
	virtual void Exit();

	virtual void HUDTimeUpdate(DungeonGroupId InGoupId = INVALID_DUNGEON_GROUP_ID) override {	/*Battle Arena HUD Time 없음.*/ }

	virtual UnlockCategory GetUnlockCategory() const override { return UnlockCategory::GUILD_WAR; }

protected:
	virtual HUDTimeMode GetHUDTimeMode() const override { return HUDTimeMode::Hidden; }	
	// 2022/11/01 PKT - 각 던전에서 나갈 때 확인 창이 필요 한가를 묻는다.( Default - 확인창 없음. )
	virtual bool IsExitNotifyMessage(FText& InMessage1, FText& InMessage2) override;
};