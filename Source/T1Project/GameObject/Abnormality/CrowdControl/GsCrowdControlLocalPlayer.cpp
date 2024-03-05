// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCrowdControlLocalPlayer.h"
#include "Shared/Client/SharedEnums/SharedAbnormalityEnum.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Message/MessageParam/GsGameObjectMessageParam.h"
#include "ActorEx/GsCharacterLocalPlayer.h"
#include "ControllerEx/GsPlayerController.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"
#include "GameObject/Reserve/GsReserveWork.h"
#include "GameObject/Target/GsTargetHandlerLocalPlayer.h"
#include "Shared/Shared/SharedPackets/PD_World_Client_Bypass.h"
#include "Item/GsItemManager.h"
#include "Copy.h"
#include "ForEach.h"

//#include "Util/GsEnum.h"

//ENABLE_ENUM_OPERATORS(CreatureActionType)
void FGsCrowdControlLocalPlayer::Initialize(class UGsGameObjectBase* Owner)
{
	Super::Initialize(Owner);

	_isCreatureAction = false;
	_reactionAffectItemIdList.Empty();

	using ITEM = TPair<CreatureActionType, TFunction<void(bool)>>;
	
	auto idx = static_cast<int>(CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT);
	for (; idx < static_cast<int>(CreatureActionType::MAX); idx++)
	{
		auto key = static_cast<CreatureActionType>(idx);
		_mapCreatureAction.Emplace(key, 0);
		_executeCreatureAction.EmplaceAt(int(key), [](bool IsEnable){}); //size + 1 이상은 못늘림
	}
	
	for (auto& e : {
		ITEM{CreatureActionType::CREATURE_ACTION_TYPE_MOVE_DEFAULT,  [this](bool IsEnable) { EventMoveLock(IsEnable);}},
		ITEM{CreatureActionType::CREATURE_ACTION_TYPE_MOVE_ROOT_MOTION,  [this](bool IsEnable) { EventRootmotionLock(IsEnable);}},
		ITEM{CreatureActionType::CREATURE_ACTION_TYPE_SKILL_BASIC,  [this](bool IsEnable) { EventSkillLock(IsEnable);}},
		ITEM{CreatureActionType::CREATURE_ACTION_TYPE_SKILL,  [this](bool IsEnable) { EventSkillLock(IsEnable);}},
		ITEM{CreatureActionType::CREATURE_ACTION_TYPE_ITEM,  [this](bool IsEnable) { EventItemLock(IsEnable);}},
		ITEM{CreatureActionType::CREATURE_ACTION_TYPE_MOVE_LOCATON,  [this](bool IsEnable) { EventMoveLocation(IsEnable);}},
		ITEM{CreatureActionType::CREATURE_ACTION_TYPE_MOUNT_VEHICLE,  [this](bool IsEnable) { EventVehicleLock(IsEnable); }}
		})
	{
		_executeCreatureAction[static_cast<int>(e.Key)] = e.Value;
	};
}

void FGsCrowdControlLocalPlayer::NetBehaviorStatusAck(PD::SC::PKT_SC_BEHAVIOR_STATUS_READ* Packet)
{
	TMap<CreatureActionType, int> totalCreatureAction;
	TMap<CreatureActionType, int> UpdateCreatureAction;
	_reactionAffectItemIdList.Empty();

	if (Packet)
	{
		//패킷 읽음
		for (PD::SC::PKT_SC_BEHAVIOR_STATUS_READ::BehaviorStatusArrayIterator iter = Packet->GetFirstBehaviorStatusArrayIterator();
			iter != Packet->GetLastBehaviorStatusArrayIterator(); ++iter)
		{
			totalCreatureAction.Emplace(iter->CreatureActionType(), iter->Count());	
		}
		for (PD::SC::PKT_SC_BEHAVIOR_STATUS_READ::DisabledItemGroupIdArrayIterator disableItemiter = Packet->GetFirstDisabledItemGroupIdArrayIterator();
			disableItemiter != Packet->GetLastDisabledItemGroupIdArrayIterator(); ++disableItemiter)
		{
			_reactionAffectItemIdList.Emplace(disableItemiter->DisabledItemGroupId());
		}

		//처리하는 action중에 active 상태의 변화가 있는지 체크
		Algo::CopyIf(totalCreatureAction, UpdateCreatureAction, [this](auto& e) {
			auto iter = _mapCreatureAction.Find(e.Key);
			return iter && ((e.Value <= 0 && *iter > 0) || (e.Value > 0 && *iter <= 0));});

		//활성화 또는 비활성화 실행
		Algo::ForEach(UpdateCreatureAction, [this](auto& e){ _executeCreatureAction[static_cast<int>(e.Key)](e.Value > 0); });

		//값 갱신
		Algo::ForEachIf(_mapCreatureAction, [&totalCreatureAction](auto& e) {
			return totalCreatureAction.Find(e.Key) != nullptr; }, [&totalCreatureAction](auto& e) {
				e.Value = *totalCreatureAction.Find(e.Key); });

		//현재 활성화 된 액션이 있는가 체크
		_isCreatureAction = Algo::AnyOf(_mapCreatureAction, [](auto& e) {return e.Value > 0;});

		FGsGameObjectMessageParamReactionLocalPlayer param(_mapCreatureAction);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_REACTION, &param);
	}

	//if (Packet)
	//{
	//	bool isUpdate = false;
	//	_isCreatureAction = false;
	//	for (PD::SC::PKT_SC_BEHAVIOR_STATUS_READ::BehaviorStatusArrayIterator iter = Packet->GetFirstBehaviorStatusArrayIterator();
	//		iter != Packet->GetLastBehaviorStatusArrayIterator(); ++iter)
	//	{
	//		CreatureActionType type = iter->CreatureActionType();
	//		if (_mapCreatureAction.Contains(type))
	//		{
	//			int stackCount = iter->Count();
	//			bool srcCampare = _mapCreatureAction[type] != 0;
	//			bool destCampare = stackCount != 0;
	//			bool isReaction = stackCount > 0;
	//			if (isReaction)
	//			{
	//				_isCreatureAction = true;
	//			}
	//			// 둘중 하나만 0이거나 0이상이거나
	//			if (srcCampare != destCampare)
	//			{
	//				// 제어 기능 실행
	//				if (_executeCreatureAction[(int)type])
	//				{
	//					_executeCreatureAction[(int)type](isReaction);
	//				}
	//			}
	//			_mapCreatureAction[type] = stackCount;
	//		}
	//	}
	//	// 리액션 관련 메세지 Send
	//	FGsGameObjectMessageParamReactionLocalPlayer param(_mapCreatureAction);
	//	GMessage()->GetGameObject().SendMessage(MessageGameObject::LOCAL_REACTION, &param);
	//}
}

void FGsCrowdControlLocalPlayer::UpdateStun()
{
	Super::UpdateStun();
}

void FGsCrowdControlLocalPlayer::UpdateKnockDown()
{
	Super::UpdateKnockDown();
}

void FGsCrowdControlLocalPlayer::UpdateKnockBack()
{
	Super::UpdateKnockBack();
}

void FGsCrowdControlLocalPlayer::UpdateFear()
{
	Super::UpdateFear();

	auto local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
	if (_reactionInfo.GetCurrentReactionProcessType() == EGsAbnormalityAnimProcessType::FEARACT)
	{
		if (local && local->IsSpectatorMode() == false)
		{
			local->SwitchSpectatorMode(true);
		}
	}
	//else
	//{
	//	if (local && local->IsSpectatorMode() == true)
	//	{
	//		local->SwitchSpectatorMode(false);
	//	}
	//}
}

void FGsCrowdControlLocalPlayer::UpdatePolymorp()
{
	Super::UpdatePolymorp();
}

void FGsCrowdControlLocalPlayer::UpdateFrameStop()
{
	Super::UpdateFrameStop();
}

void FGsCrowdControlLocalPlayer::UpdateTaunt()
{
	if (FGsTargetHandlerLocalPlayer* tagetHandler = _owner->GetCastTarget<FGsTargetHandlerLocalPlayer>())
	{
		if (UGsGameObjectBase* target = tagetHandler->GetTarget())
		{
			tagetHandler->OnTarget(FGsTargetRule::SemiAutoTarget);
		}
	}

	Super::UpdateTaunt();
}

void FGsCrowdControlLocalPlayer::EndCurrentReactionState(EGsAbnormalityAnimProcessType endReaction)
{
	if (endReaction == EGsAbnormalityAnimProcessType::FEARACT)
	{
		if (auto local = _owner->CastGameObject<UGsGameObjectLocalPlayer>())
		{
			local->SwitchSpectatorMode(false);
		}
	}

	Super::EndCurrentReactionState(endReaction);
}

void FGsCrowdControlLocalPlayer::EventMoveLock(bool IsLock)
{
	_owner->CastGameObject<UGsGameObjectLocalPlayer>()->MoveLock(UGsGameObjectLocalPlayer::EMoveLockType::Abnormality, IsLock);
}

void FGsCrowdControlLocalPlayer::EventSkillLock(bool IsLock)
{
}

void FGsCrowdControlLocalPlayer::EventItemLock(bool IsLock)
{
	if (UGsItemManager* itemMgr = GItem())
	{
		itemMgr->UpdateReactionItemLock(_reactionAffectItemIdList);
	}
}

void FGsCrowdControlLocalPlayer::EventMoveLocation(bool IsLock)
{
}

void FGsCrowdControlLocalPlayer::EventRootmotionLock(bool IsLock)
{
	// SC_BEHAVIOR_STATUS 패킷이 브로드 캐스팅이 아닌 관계로
	// 타 유저 및 몬스터 처리를 할수가 없다.	
	// @See : void FGsCrowdControlBase::UpdateBind()
// 	if (UGsAnimInstanceState* anim = _owner->GetAnimInstance())
// 	{
// 		ERootMotionMode::Type rootmotionType = (IsEnable) ? ERootMotionMode::Type::IgnoreRootMotion :
// 			ERootMotionMode::Type::RootMotionFromMontagesOnly;
// 		
// 		anim->SetRootMotionMode(rootmotionType);
// 	}
}

void FGsCrowdControlLocalPlayer::EventTargetLock(bool IsLock)
{
	// 현재 타겟 고정
}

void FGsCrowdControlLocalPlayer::EventVehicleLock(bool IsLock)
{
	bool IsRemove = IsLock;
	UGsGameObjectLocalPlayer* local = _owner->CastGameObject<UGsGameObjectLocalPlayer>();
	local->SetFairyVehicleCondition(EGsFairyVehicleCondition::VEHICLE_CREATURE_ACTION, IsRemove); 
}

bool FGsCrowdControlLocalPlayer::IsCreatureAction() const
{
	return _isCreatureAction;
}

bool FGsCrowdControlLocalPlayer::IsCreatureActionType(CreatureActionType actionType) const
{
	if (_mapCreatureAction.Contains(actionType))
	{
		return _mapCreatureAction[actionType] > 0;
	}
	return false;
}

const TMap<CreatureActionType, int>& FGsCrowdControlLocalPlayer::GetCreatureActionMap() const
{
	return _mapCreatureAction;
}

