#pragma once

#include "CoreMinimal.h"
#include "../Dungeon/GsDungeonCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

/**
* file		GsDungeonStateBase.h
* @brief	진행 중인 던전 상태에 대한 처리
* @author	PKT
* @date		2022/12/13
**/

class FGsDungeonStateBase
{
protected:
	// 2022/12/13 PKT - 카테고리
	DungeonCategory _category = DungeonCategory::MAX;
	// 2022/12/13 PKT - Group Id
	DungeonGroupId _groupId = INVALID_DUNGEON_GROUP_ID;
	// 2022/12/13 PKT - mapId
	MapId _mapId = INVALID_MAP_ID;
	// 2022/12/13 PKT - 던전 진행 상태
	EDungeonProgressState _progressState = EDungeonProgressState::ENone;

public:
	explicit FGsDungeonStateBase(DungeonCategory InCategory)
		: _category(InCategory), _groupId(INVALID_DUNGEON_GROUP_ID), _mapId(INVALID_MAP_ID), _progressState(EDungeonProgressState::ENone)
	{
		/**/
	}

	virtual void Initialize();
	virtual void Finalize();

	virtual bool Enter(MapId InMapId);
	virtual void Exit();

	// 2022/11/01 PKT - 나가기/귀환 아이템 등을 통해 던전에서 탈출 할 때 호출..
	virtual void TryExit(ItemDBId InUseItemDBId);

	// 2022/10/31 PKT - HUD Time Update
	virtual void HUDTimeUpdate(DungeonGroupId InGoupId = INVALID_DUNGEON_GROUP_ID) {/*None*/ }
	// 2022/10/31 PKT - 던전 내 로컬 플레이어가 죽었을 시 처리해야 할 것들을 처리 한다.
	virtual void LocalPlayerDie() {/*None*/ }	
	// 2022/11/01 PKT - Elite Dungeon Keeper NPC
	virtual void InteractionNpcKeeperContents(int32 InSchemaTableId) {/*고레벨 던전에서 사용*/ }
	// 2022/11/01 PKT - Elite Dungeon Prop Potal
	virtual void InteractionPropPortalStart(int32 InSchemaTableId, int64 InGameId) {/*고레벨 던전에서 사용*/ }
	virtual void InteractionPropPortalEnd(int64 InGameId) {/*고레벨 던전에서 사용*/ }

protected:
	enum class HUDTimeMode { Play, Pause, Hidden };
	void SendMessageHUDTimeMode(HUDTimeMode InHUDTimeMode);
	// 2022/12/13 PKT - 각 던전 타입에 따라 혹은 던전 입장 퇴장등에 따라 HUD Time Show / Hidden / Pause	
	virtual HUDTimeMode GetHUDTimeMode() const;
	// 2022/11/01 PKT - 각 던전에서 나갈 때 확인 창이 필요 한가를 묻는다.( Default - 확인창 없음. )
	virtual bool IsExitNotifyMessage(FText& InMessage1, FText& InMessage2);
	// 2022/12/07 PKT - 던전 남은 시간 HUD에 출력
	virtual void SendMessageHUDTimeUpdate(FDateTime InDateTime, bool InIsLimitTime, bool InIsPause = false);

	/**
	 * Getter
	 */
public:
	DungeonCategory GetCategory() const { return _category; }
	DungeonGroupId GetGroupId() const { return _groupId; }
	MapId GetMapId() const { return _mapId; }
	EDungeonProgressState GetProgressState() const { return _progressState; }
	virtual UnlockCategory GetUnlockCategory() const { return UnlockCategory::NONE; }
};