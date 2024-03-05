#pragma once

/**
* file		GsPartyDungeonState.h
* @brief	파티 던전
* @author	PKT
* @date		2022/12/13
**/

#include "CoreMinimal.h"
#include "GsDungeonStateBase.h"
#include "../Dungeon/GsDungeonCommonEnum.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedPartyDungeonEnum.h"

class FGsPartyDungeonState : public FGsDungeonStateBase
{
	// 2022/11/01 PKT - Help 요청 카운트
	int32 _helpseekingCount = 0;
	// 2022/06/20 PKT - Help 가능 상태
	EDungeonHelpseekingState _helpseekingState = EDungeonHelpseekingState::ENone;

	//bool _helpseekingEnable = false;
	// 2022/11/02 PKT - 던전의 진행에 따라 더 이상 도움 요청이 불가능한 상황
	//bool _helpseekingLock = false;

	DungeonGroupId _applyGroupId = INVALID_DUNGEON_GROUP_ID;
	DungeonDifficulty _applyDifficulty = DungeonDifficulty::NONE;

public:
	explicit FGsPartyDungeonState() : FGsDungeonStateBase(DungeonCategory::PARTY), _helpseekingCount(0), _helpseekingState(EDungeonHelpseekingState::ENone)
		, _applyGroupId(INVALID_DUNGEON_GROUP_ID), _applyDifficulty(DungeonDifficulty::NONE)
	{/*None*/ }

	virtual void Initialize() override;
	virtual void Finalize() override;

	virtual bool Enter(MapId InMapId) override;
	virtual void Exit() override;

	virtual void HUDTimeUpdate(DungeonGroupId InGoupId = INVALID_DUNGEON_GROUP_ID) override {	/*파티 던전 HUD Time 없음.*/ }

	virtual void LocalPlayerDie() override;

protected:
	virtual HUDTimeMode GetHUDTimeMode() const override;
	virtual bool IsExitNotifyMessage(FText& InMessage1, FText& InMessage2) override;

public:
	void RequestHelpseeking();

public:
	// 2022/11/02 PKT - 파티 나가기(본인) / 매칭 취소 / 실패 등의 이유로 매칭 정보 초기화
	void ClearData();
	// 2023/3/3 PKT - 던전 파티가 시작 되었다.
	void SetStartPlay(const FDateTime& InStartTime, const FDateTime& InEndTime);
	// 2022/11/02 PKT - 파티 던전을 들어가기 위해 매칭 / 파티 결성 등의 패킷
	void SetJoin(EDungeonProgressState InProgressState, DungeonGroupId InDungeonGroupId, DungeonDifficulty InDifficulty);
	// 2022/11/02 PKT - 도움 요청 카운트
	void SetHelpseekingInfo(int32 InCount, bool InIsEnable = true);	
	// 2022/11/02 PKT - 도움 요청 활성화 여부
	void SetHelpseekingEnable(bool InIsEnable);
	// 2022/11/02 PKT - 도움 요청 Lock
	void SetHelpseekingLock(bool InIsLock);
	// 2022/11/02 PKT - 파티에서 추방( 본인 ) 당했을 시 추방 사유를 안내( 파티장일 경우 )
	void NotifyPartyKickReason();
	// 2023/2/15 PKT - 매칭 취소 / 실패
	void PartyMathcingBroken();

	/**
	 * Geter
	 */
public:
	virtual UnlockCategory GetUnlockCategory() const override { return UnlockCategory::PARTY_DUNGEON; }
	DungeonGroupId GetApplyGroupId() const { return _applyGroupId;	}
	DungeonDifficulty GetApplyDifficulty() const { return _applyDifficulty; }

	int32 GetHelpseekingCount() const { return _helpseekingCount; }
	bool GetHelpseekingEnable() const { return _helpseekingState == EDungeonHelpseekingState::EActive; }
};