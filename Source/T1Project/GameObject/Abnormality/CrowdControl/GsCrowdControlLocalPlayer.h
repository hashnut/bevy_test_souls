// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GsCrowdControlBase.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

namespace PD
{
	namespace SC
	{
		struct PKT_SC_BEHAVIOR_STATUS_READ;
	}
}

/**
 * 
 */
class T1PROJECT_API FGsCrowdControlLocalPlayer final : public FGsCrowdControlBase
{
	using Super = FGsCrowdControlBase;

public:
	FGsCrowdControlLocalPlayer() = default;
	virtual ~FGsCrowdControlLocalPlayer() = default;

protected:
	// 나의 행동 제약 정보를 서버와 동기화
	TMap<CreatureActionType, int> _mapCreatureAction;
	// 행동 제약 타입에 따른 함수 펑터 연결
	TArray<TFunction<void(bool)>> _executeCreatureAction;
	// for문을 돌아야해서 캐싱
	bool _isCreatureAction = false;

	// true == creatureActionTypeItem 일때, 사용 제한되는 아이템 GroupId
	TArray<int32> _reactionAffectItemIdList;

public:
	virtual void Initialize(class UGsGameObjectBase* Owner) override;
	
	void NetBehaviorStatusAck(PD::SC::PKT_SC_BEHAVIOR_STATUS_READ* Packet);

protected:
	virtual void UpdateStun() override;
	virtual void UpdateKnockDown() override;
	virtual void UpdateKnockBack() override;
	virtual void UpdateFear() override;
	virtual void UpdatePolymorp() override;
	virtual void UpdateFrameStop() override;
	virtual void UpdateTaunt() override;
	//virtual void UpdateCocoon() override;
	//virtual void UpdateRootWinding() override;

	virtual void EndCurrentReactionState(EGsAbnormalityAnimProcessType endReaction) override;

protected:
	void EventMoveLock(bool IsLock);
	void EventSkillLock(bool IsLock);
	void EventItemLock(bool IsLock);
	void EventMoveLocation(bool IsLock);
	void EventRootmotionLock(bool IsLock);
	void EventTargetLock(bool IsLock);
	void EventVehicleLock(bool IsLock);

public:
	virtual bool IsCreatureAction() const override;
	virtual bool IsCreatureActionType(CreatureActionType actionType) const override;

	const TMap<CreatureActionType, int>& GetCreatureActionMap() const;
};
