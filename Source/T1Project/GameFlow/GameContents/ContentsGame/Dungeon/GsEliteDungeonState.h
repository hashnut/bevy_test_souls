#pragma once

/**
* file		GsEliteDungeonState.h
* @brief	정예 던전
* @author	PKT
* @date		2022/12/13
**/

#include "CoreMinimal.h"
#include "GsDungeonStateBase.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"

class FGsEliteDungeonState : public FGsDungeonStateBase
{
public:
	using Super = FGsDungeonStateBase;

public:
	explicit FGsEliteDungeonState() : FGsDungeonStateBase(DungeonCategory::ELITE)	{/*None*/}

	virtual bool Enter(MapId InMapId) override;
	virtual void Exit() override;

	virtual void HUDTimeUpdate(DungeonGroupId InGoupId = INVALID_DUNGEON_GROUP_ID) override;
	// 2022/11/01 PKT - Elite Dungeon Keeper NPC
	virtual void InteractionNpcKeeperContents(int32 InSchemaTableId) override;
	// 2022/11/01 PKT - Elite Dungeon Prop Potal
	virtual void InteractionPropPortalStart(int32 InSchemaTableId, int64 InGameId) override;
	virtual void InteractionPropPortalEnd(int64 InGameId) override;

	virtual UnlockCategory GetUnlockCategory() const override { return UnlockCategory::ELITE_DUNGEON; }
	
protected:
	virtual HUDTimeMode GetHUDTimeMode() const override;
	virtual bool IsExitNotifyMessage(FText& InMessage1, FText& InMessage2);	

public:
	bool InIsLobbyFloor() const;
};