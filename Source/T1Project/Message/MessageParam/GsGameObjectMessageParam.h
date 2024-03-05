// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIModule/Classes/Navigation/PathFollowingComponent.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Client/SharedEnums/SharedSpawnEnum.h"

#include "GameObject/Define/GsGameObjectDefine.h"
#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"
#include "GameObject/Abnormality/GsAbnormalityDataDefine.h"
#include "GameObject/Trigger/GsTriggerEventBase.h"
#include "GameObject/Fence/GsFence.h"
#include "GameObject/Target/GsTargetHandlerBase.h"

#include "DataSchema/Sound/GsSchemaSoundResData.h"

#include "Message/MessageParam/GsMessageParam.h"

#include "Map/GsMapIconType.h"
#include "ActorEx/GsCampBase.h"


struct FGsSchemaSkillSet;
struct FGsSpawnParam;
class UGsGameObjectBase;

/**
 * GameObject Message 처리관련 파라미터 구조체 정의
 * 필요에 따라 상속, 또는 개별 구현을 한다.
 */
struct FGsGameObjectMessageParam : public IGsMessageParam
{
	UGsGameObjectBase* _paramOwner;
	
	FGsGameObjectMessageParam() = default;
	explicit FGsGameObjectMessageParam(UGsGameObjectBase* Owner)
		: 
		_paramOwner(Owner)
	{
	}
	virtual ~FGsGameObjectMessageParam() = default;

	// 연산자 오버로딩
	FGsGameObjectMessageParam operator = (const FGsGameObjectMessageParam* param)
	{
		_paramOwner = param->_paramOwner;
		return *this;
	}

	virtual bool IsValid() const { return nullptr != _paramOwner; }
	virtual void Empty() { _paramOwner = nullptr; }
};

// 타겟 정보
struct FGsGameObjectMessageParamTarget final : public FGsGameObjectMessageParam
{
	UGsGameObjectBase* _paramTarget;
	// _paramTarget정보가 주타겟인지 확인 
	bool _isFirstTarget = true;
	// 타겟 설정 입력 타입
	FGsTargetHandlerBase::SelectTargetFilter _targetFilterType = FGsTargetHandlerBase::SelectTargetFilter::None;

	FGsGameObjectMessageParamTarget() = default;
	explicit FGsGameObjectMessageParamTarget(UGsGameObjectBase* Owner, UGsGameObjectBase* Target,
		bool IsFirstTarget = true, 
		FGsTargetHandlerBase::SelectTargetFilter TargetFilterType = FGsTargetHandlerBase::None)
		: 
		FGsGameObjectMessageParam(Owner),
		_paramTarget(Target),
		_isFirstTarget(IsFirstTarget),
		_targetFilterType(TargetFilterType)
	{
	}

	FGsGameObjectMessageParamTarget operator = (const FGsGameObjectMessageParamTarget* param)
	{
		_paramOwner = param->_paramOwner;
		_paramTarget = param->_paramTarget;
		_isFirstTarget = param->_isFirstTarget;
		_targetFilterType = param->_targetFilterType;
		return *this;
	}

	virtual bool IsValid() const override
	{  
		return FGsGameObjectMessageParam::IsValid() && nullptr != _paramTarget;
	}
	virtual void Empty() override
	{ 
		FGsGameObjectMessageParam::Empty();
		_paramTarget = nullptr;
	}
};

// 현재 상태 정보
struct FGsGameObjectMessageParamState final : public FGsGameObjectMessageParam
{
	EGsStateBase _type;

	FGsGameObjectMessageParamState() = default;
	explicit FGsGameObjectMessageParamState(UGsGameObjectBase* Owner, EGsStateBase Type)
		:
		FGsGameObjectMessageParam(Owner),
		_type(Type)
	{
	}

	virtual ~FGsGameObjectMessageParamState() = default;
};

// 스킬 정보
struct FGsGameObjectMessageParamSkill final : public FGsGameObjectMessageParam
{
	int _skillId;

	FGsGameObjectMessageParamSkill() = default;
	explicit FGsGameObjectMessageParamSkill(UGsGameObjectBase* Owner, int SkillId)
		:
		FGsGameObjectMessageParam(Owner),
		_skillId(SkillId)
	{
	}

	virtual ~FGsGameObjectMessageParamSkill() = default;

	virtual bool IsValid() const override
	{
		return  FGsGameObjectMessageParam::IsValid() && 0 != _skillId;
	}
	virtual void Empty() override
	{
		FGsGameObjectMessageParam::Empty();
		_skillId = 0;
	}
};

// 스킬 정보
struct FGsGameObjectMessageParamPlayerStat final : public FGsGameObjectMessageParam
{
	TArray<TPair<StatType, int32>> StatMap;

	FGsGameObjectMessageParamPlayerStat() = default;
	explicit FGsGameObjectMessageParamPlayerStat(UGsGameObjectBase * Owner, TArray<TPair<StatType, int32>>StatMap)
		:
		FGsGameObjectMessageParam(Owner),
		StatMap(StatMap)
	{
	}
	virtual ~FGsGameObjectMessageParamPlayerStat() = default;

	bool FindStatType(StatType inType) const
	{
		if(nullptr != StatMap.FindByPredicate([inType](TPair<StatType, int32>& elem)->bool { return inType == elem.Key; }))
			return true;
		return false;
	}

	virtual bool IsValid() const override
	{
		return  FGsGameObjectMessageParam::IsValid() && 0 != StatMap.Num();
	}
	virtual void Empty() override
	{
		FGsGameObjectMessageParam::Empty();
		StatMap.Empty();
	}
};

//BuffSkillAuto
struct FGsGameObjectMessageParamBuffSkill final : public FGsGameObjectMessageParam
{
	enum class BuffSkillAuto : uint8 { Disable = 0, Enable = 1, Keep = 2};

	int _skillId;
	BuffSkillAuto _autoEnable;

	FGsGameObjectMessageParamBuffSkill() = default;
	virtual ~FGsGameObjectMessageParamBuffSkill() = default;
	explicit FGsGameObjectMessageParamBuffSkill(UGsGameObjectBase * Owner, int SkillId, BuffSkillAuto autoSkill = BuffSkillAuto::Keep)
		:
		FGsGameObjectMessageParam(Owner),
		_skillId(SkillId),
		_autoEnable(autoSkill)
	{
	}

	virtual bool IsValid() const override
	{
		return  FGsGameObjectMessageParam::IsValid() && 0 != _skillId;
	}
	virtual void Empty() override
	{
		FGsGameObjectMessageParam::Empty();
		_skillId = 0;
		_autoEnable = BuffSkillAuto::Keep;
	}
};

// 맵 아이콘 정보
// 스폰시 : UGsGameObjectBase* 사용
// 디스폰시: int64 게임 아이디 사용
struct FGsGameObjectMessageParamMapIcon final : public FGsGameObjectMessageParam
{
	int64 _gameId;
	EGsMapIconType _iconType;
	bool _isPartyMember;
	SPAWN_REASON _despawnReason;

	FGsGameObjectMessageParamMapIcon() = default;
	virtual ~FGsGameObjectMessageParamMapIcon() = default;
	explicit FGsGameObjectMessageParamMapIcon(int64 GameID)
		: 
		_gameId(GameID)
	{
	}
	explicit FGsGameObjectMessageParamMapIcon(int64 GameID, SPAWN_REASON DespawnReason)
		:
		_gameId(GameID), 
		_despawnReason(DespawnReason)
	{
	}
	explicit FGsGameObjectMessageParamMapIcon(UGsGameObjectBase* Owner, EGsMapIconType IconType,
		bool In_isPartyMember = false)
		:
		FGsGameObjectMessageParam(Owner),
		_iconType(IconType),
		_isPartyMember(In_isPartyMember)
	{
	}
};

// 지역 맵 아이콘 정보
// 스폰시 : UGsGameObjectBase* 사용
// 디스폰시: int64 게임 아이디 사용
struct FGsGameObjectMessageParamRegionMapIcon final : public FGsGameObjectMessageParam
{
	int64 _gameId;
	EGsRegionMapIconType _iconType;
	SPAWN_REASON _despawnReason;

	FGsGameObjectMessageParamRegionMapIcon() = default;
	virtual ~FGsGameObjectMessageParamRegionMapIcon() = default;
	explicit FGsGameObjectMessageParamRegionMapIcon(int64 GameID)
		:
		_gameId(GameID)
	{
	}
	explicit FGsGameObjectMessageParamRegionMapIcon(int64 GameID, SPAWN_REASON DespawnReason)
		:
		_gameId(GameID),
		_despawnReason(DespawnReason)
	{
	}

	explicit FGsGameObjectMessageParamRegionMapIcon(UGsGameObjectBase * Owner, EGsRegionMapIconType IconType)
		:
		FGsGameObjectMessageParam(Owner),
		_iconType(IconType)		
	{
	}
};


// 행동 제약 타입 정보
struct FGsGameObjectMessageParamReactionLocalPlayer final : public FGsGameObjectMessageParam
{
	const TMap<CreatureActionType, int>& _refMapCreatureAction;

	// FGsGameObjectMessageParamReactionLocalPlayer() = default;
	virtual ~FGsGameObjectMessageParamReactionLocalPlayer() = default;
	explicit FGsGameObjectMessageParamReactionLocalPlayer(const TMap<CreatureActionType, int>& MapCreatureAction) :
		_refMapCreatureAction(MapCreatureAction)
	{
	}

	bool IsLock(CreatureActionType creatureActionType) const
	{ 
		if (_refMapCreatureAction.Contains(creatureActionType))
		{
			return _refMapCreatureAction[creatureActionType] > 0;
		}
		return false;
	}
};

struct FGsLocalGameObjectMessageParamAbnormality final : public FGsGameObjectMessageParam
{
	const TMap<AbnormalityInstanceId, TSharedPtr<FGsAbnormalityData>>& _refMapAbnormality;

	explicit FGsLocalGameObjectMessageParamAbnormality(const TMap<AbnormalityInstanceId, TSharedPtr<FGsAbnormalityData>>& MapAbnormality) :
		_refMapAbnormality(MapAbnormality)
	{
	}
	virtual ~FGsLocalGameObjectMessageParamAbnormality() = default;

	bool IsAbnormality(AbnormalityInstanceId Id)
	{
		if (_refMapAbnormality.Contains(Id))
		{
			return true;
		}
		return false;
	}

	bool IsAbnormalityEffectType(AbnormalityEffectType EffectType) const
	{
		for (const TPair< AbnormalityInstanceId, TSharedPtr<FGsAbnormalityData>>& pair : _refMapAbnormality)
		{
			const TSharedPtr<FGsAbnormalityData>& el = pair.Value;
			if (el.IsValid() && el->GetEffectType() == EffectType)
			{
				return true;
			}
		}
		return false;
	}
};

struct FGsGameObjectMessageParamAbnormality final : public FGsGameObjectMessageParam
{
	UGsGameObjectBase* _paramOwner;
	AbnormalityInstanceId _abnormalityInstId;
	bool _bIsRemove;
	AbnormalityEffectType _effectType;

	FGsGameObjectMessageParamAbnormality() = default;
	explicit FGsGameObjectMessageParamAbnormality(UGsGameObjectBase* InOwner, AbnormalityInstanceId InId, bool bInIsRemove) :
		_paramOwner(InOwner), 
		_abnormalityInstId(InId),
		_bIsRemove(bInIsRemove)
	{
	}
	explicit FGsGameObjectMessageParamAbnormality(UGsGameObjectBase* InOwner, 
		AbnormalityInstanceId InId, 
		bool bInIsRemove, 
		AbnormalityEffectType InEffType) :
		_paramOwner(InOwner),
		_abnormalityInstId(InId),
		_bIsRemove(bInIsRemove),
		_effectType(InEffType)

	{
	}
	virtual ~FGsGameObjectMessageParamAbnormality() = default;
};

// 트리거 이벤트용 Param구조체
struct FGsTriggerMessageParam : public FGsGameObjectMessageParam
{
	int64 _objectGameId;
	EGsTriggerType _type;
	bool _isActive;

	FGsTriggerMessageParam() = default;
	explicit FGsTriggerMessageParam(int64 In_objectGameId, EGsTriggerType In_type, bool In_isActive) :
		_objectGameId(In_objectGameId), _type(In_type), _isActive(In_isActive)
	{
	}
	virtual ~FGsTriggerMessageParam() = default;
};

/**
 * 스킬 슬롯 정보 변경 시.
 */
struct FGsGameObjectMessageParamSkillSlot final : public IGsMessageParam
{
	// LocalPlayer의 정보라 owner가 딱히 필요없어서 FGsGameObjectMessageParam 를 상속받지 않았음
	CreatureWeaponType _weaponType;
	SkillSlotId _slotId;
	const class FGsSkill* _skill;

	FGsGameObjectMessageParamSkillSlot() = default;
	explicit FGsGameObjectMessageParamSkillSlot(CreatureWeaponType InWeapon, 
		SkillSlotId InSlotId, const class FGsSkill* InSkill)
		: _weaponType(InWeapon), _slotId(InSlotId), _skill(InSkill)
	{
	}

	virtual ~FGsGameObjectMessageParamSkillSlot() = default;
};

/**
 * 체인 스킬 슬롯 정보 변경 시.
 */
struct FGsGameObjectMessageParamSkillSlotChain final : public IGsMessageParam
{
	// LocalPlayer의 정보라 owner가 딱히 필요없어서 FGsGameObjectMessageParam 를 상속받지 않았음
	CreatureWeaponType _weaponType;
	SkillSlotId _slotId;
	const class FGsSkill* _prevSkill;
	const class FGsSkill* _skill;

	FGsGameObjectMessageParamSkillSlotChain() = default;
	explicit FGsGameObjectMessageParamSkillSlotChain(CreatureWeaponType InWeapon,
		SkillSlotId InSlotId, const class FGsSkill* InPrevSkill, const class FGsSkill* InSkill)
		: _weaponType(InWeapon), _slotId(InSlotId), _prevSkill(InPrevSkill), _skill(InSkill)
	{
	}

	virtual ~FGsGameObjectMessageParamSkillSlotChain() = default;
};

/**
 * 패시브 정보 변경 시
 */
struct FGsGameObjectMessageParamPassivity final : public FGsGameObjectMessageParam
{	
	PassivityId _passivityId;
	PassivityOwnerType _ownerType;
	PassivityOwnerId _ownerId;
	bool _bIsBuff;

	FGsGameObjectMessageParamPassivity() = default;
	explicit FGsGameObjectMessageParamPassivity(UGsGameObjectBase* InOwner, PassivityId InId, 
		PassivityOwnerType InOwnerType, PassivityOwnerId InOwnerId, bool bInIsBuff) :
		FGsGameObjectMessageParam(InOwner),
		_passivityId(InId),
		_ownerType(InOwnerType),
		_ownerId(InOwnerId),
		_bIsBuff(bInIsBuff)
	{
	}

	virtual ~FGsGameObjectMessageParamPassivity() = default;
};

/*
	클라이언트 스폰 끝 파람
*/
struct FGsClientSpawnListLoadFinishMessageParam : public IGsMessageParam
{
	TArray<TSharedPtr<FGsSpawnParam>> _arrayNonPlayer;

	FGsClientSpawnListLoadFinishMessageParam() = default;
	explicit FGsClientSpawnListLoadFinishMessageParam(TArray<TSharedPtr<FGsSpawnParam>> In_nonPlayers) :
		_arrayNonPlayer(In_nonPlayers)
	{
	}

	virtual ~FGsClientSpawnListLoadFinishMessageParam() = default;
};

/*
	이동 실패
	사용처: 
	1. 퀘스트 이동(1. 타겟 없음, 2. 이동 실패(경로 이상함))
	2. npc list(1. 타겟 없음)
*/

struct FGsMoveFailedMessageParam : public IGsMessageParam
{
	// 이동 실패 이유
	EGsMoveFailedReason _moveFailedReason;
	int _npcTblId;
	EPathFollowingResult::Type _pathResult;

	FGsMoveFailedMessageParam() = default;
	explicit FGsMoveFailedMessageParam(EGsMoveFailedReason In_reason) :
		_moveFailedReason(In_reason)
	{
	}
	explicit FGsMoveFailedMessageParam(EGsMoveFailedReason In_reason, int In_npcTbId) :
		_moveFailedReason(In_reason),_npcTblId(In_npcTbId)
	{
	}
	explicit FGsMoveFailedMessageParam(EGsMoveFailedReason In_reason, 
		EPathFollowingResult::Type In_pathResult) :
		_moveFailedReason(In_reason), _pathResult(In_pathResult)
	{
	}

	virtual ~FGsMoveFailedMessageParam() = default;
};

struct FGsTalkBalloonRankMessageParam : public IGsMessageParam
{
	FString _localGuildName;
	FString _localUserName;
	FString _worldGuildName;
	FString _worldUserName;
	
	FGsTalkBalloonRankMessageParam() = default;
	explicit FGsTalkBalloonRankMessageParam(const FString& InLocalGuildName, const FString& InLocalUserName,
		const FString& InWorldGuildName, const FString& InWorldUserName) :
		_localGuildName(InLocalGuildName), _localUserName(InLocalUserName), 
		_worldGuildName(InWorldGuildName), _worldUserName(InWorldUserName)
	{
	}

	virtual ~FGsTalkBalloonRankMessageParam() = default;
};

struct FGsTalkBalloonMarketMessageParam : public IGsMessageParam
{
	ItemId _highestPriceItemId;
	ItemId _highestGradeItemId;

	FGsTalkBalloonMarketMessageParam() = default;
	explicit FGsTalkBalloonMarketMessageParam(ItemId InPriceItemId, ItemId InGradeItemId) :
		_highestPriceItemId(InPriceItemId), _highestGradeItemId(InGradeItemId)
	{
	}
	virtual ~FGsTalkBalloonMarketMessageParam() = default;
};

//AUTO Idle Motion 출력
struct FGsGameObjectMessageParamIdleMotion final : public FGsGameObjectMessageParam
{
	enum class IdleMotionAuto : uint8 { Disable = 0, Enable = 1};
	IdleMotionAuto _autoEnable{ IdleMotionAuto::Disable };

	FGsGameObjectMessageParamIdleMotion() = default;
	virtual ~FGsGameObjectMessageParamIdleMotion() = default;
	explicit FGsGameObjectMessageParamIdleMotion(UGsGameObjectBase * Owner, IdleMotionAuto autoMotion = IdleMotionAuto::Disable)
		: FGsGameObjectMessageParam(Owner),
		_autoEnable(autoMotion)
	{
	}
	virtual void Empty() override
	{
		FGsGameObjectMessageParam::Empty();
		_autoEnable = IdleMotionAuto::Disable;
	}
};

//fence param
struct FGsGameObjectFenceParam final : public FGsGameObjectMessageParam
{
	const struct FGsFence* _fence;
	EGsEnvEvent _envEvent;
	FGsGameObjectFenceParam() = default;
	virtual ~FGsGameObjectFenceParam() = default;
	explicit FGsGameObjectFenceParam(const struct FGsFence* inFence, EGsEnvEvent inEnv)
		: _fence(inFence), _envEvent(inEnv)
	{
	}
	virtual void Empty() override
	{
		_fence = nullptr;
	}
};

//fence param
struct FGsGameObjectSoundParam final : public FGsGameObjectMessageParam
{
	struct FGsFence* _fence;
	FGsSchemaSoundResDataRow _sound;
	FGsGameObjectSoundParam() = default;
	explicit FGsGameObjectSoundParam(struct FGsFence* inFence, FGsSchemaSoundResDataRow inSound)
		: _fence(inFence), _sound(inSound)
	{
	}
	virtual void Empty() override
	{
		_fence = nullptr;
	}

	virtual ~FGsGameObjectSoundParam() = default;
};

struct FGsActorMessageParam final : public IGsMessageParam
{
	AActor* _paramActor;

	FGsActorMessageParam() = default;
	explicit FGsActorMessageParam(AActor* In_actor)
		:
		_paramActor(In_actor)
	{
	}
	virtual ~FGsActorMessageParam() = default;

};

struct FGsCampActorMessageParam final : public IGsMessageParam
{
	const AGsCampBase* _campActor;

	FGsCampActorMessageParam() = default;
	explicit FGsCampActorMessageParam(const AGsCampBase* In_actor)
		:
		_campActor(In_actor)
	{
	}
	virtual ~FGsCampActorMessageParam() = default;
};
