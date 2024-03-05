#pragma once

#include "GameObject/GsGameObjectHandler.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


class UGsGameObjectBase;

class UParticleSystem;

struct FGsSchemaSoundResData;

//----------------------------------------------------
// 아이템 사용 이펙트 핸들러(정탄)
//----------------------------------------------------

class FGsUseItemEffectHandler : public IGsGameObjectHandler
{
protected:
	// 핸들러 소유자
	UGsGameObjectBase* _owner = nullptr;

	// 생성자, 소멸자
public:
	FGsUseItemEffectHandler() : IGsGameObjectHandler(GameObjectHandlerType::USE_ITEM_EFFECT) {};
	virtual ~FGsUseItemEffectHandler() = default;

	// 가상 함수
public:
	// 초기화 
	virtual void Initialize(UGsGameObjectBase* In_owner) override;
	virtual void Finalize() override {}

	// 로직 함수
public:
	// 카테고리별 이펙트 출력 처리
	void OnUseItemCategory(ItemCategorySub In_category);

	/** 천마석 사용 이펙트 (합산 레벨별로 다른 이펙트 출력) */
	void OnUseItemEffect(ItemCategorySub In_category,CreatureWeaponType In_weaponType, Level InSumLevel = 0);

	/** 천마석 Hit 이펙트 (합산 레벨별로 다른 이펙트 출력) */
	void OnUseItemHitEffect(ItemCategorySub In_category, Level InSumLevel = 0);

	/** 장착 무기 아이템 천마석(정령탄) 이펙트 접근 (합산 레벨별로) */
	UParticleSystem* FindSpiritShotEffect(bool In_isHit, Level InSumLevel = 0);

	// 천마석(정령탄) 사운드 테이블 접근(아이템 등급에 따라)
	const FGsSchemaSoundResData* FindSpiritShotSoundRes(bool In_isHit);

	// 연출가능여부 체크
	bool IsShowEffectPlay();
};