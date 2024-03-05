// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialSkill.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Message/MessageParam/GsTutorialMessageParam.h"
#include "DataSchema/Tutorial/GsSchemaTutorialTargetId.h"
#include "DataSchema/Reward/GsSchemaRewardItemBagData.h"
#include "Tutorial/Condition/GsTutorialConditionBase.h"
#include "Tutorial/Action/GsTutorialActionBase.h"
#include "Reward/GsRewardHelper.h"
#include "Item/GsItemManager.h"
#include "Item/GsItemConsumable.h"
#include "Skill/GsSkillHelper.h"
#include "Skill/GsSkill.h"


bool FGsTutorialSkill::OnPrevStart()
{
	if (_bIsComplete)
	{
		return false;
	}

	// 스킬북
	if (0 == _targetSkillBookList.Num())
	{
		if (false == SetTargetSkillBookList())
		{
			return false;
		}
	}

	// 인벤토리에 보유하고 있는 책 있는지 검사
	UGsItemManager* itemMgr = GItem();
	if (nullptr == itemMgr)
	{
		return false;
	}

	TWeakPtr<FGsItem> skillBookPtr;
	for (const int32& itemId : _targetSkillBookList)
	{
		TArray<TWeakPtr<FGsItem>> foundList = itemMgr->FindByTID(itemId, ItemType::CONSUME);
		if (0 < foundList.Num())
		{
			if (foundList[0].IsValid())
			{
				skillBookPtr = foundList[0].Pin();
				break;
			}
		}
	}

	// 없으면 종료
	if (false == skillBookPtr.IsValid())
	{
		return false;
	}

	FGsItemConsumable* consumableItem = skillBookPtr.Pin()->GetCastItem<FGsItemConsumable>();
	if (nullptr == consumableItem)
	{
		return false;
	}

	TArray<int32> userSkillSetIdList;
	if (false == consumableItem->GetUserSkillSetIdList(userSkillSetIdList))
	{
		return false;
	}

	TWeakPtr<const FGsSkill> skill = nullptr;
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		// 무기 타입을 알기 위함이므로 0번째 스킬 하나만 체크함
		skill = FGsSkillHelper::GetSkillByUserSkillSetId(userSkillSetIdList[0], userData->mGender);
	}

	if (false == skill.IsValid())
	{
		return false;
	}

	// 무기 아니면 실패
	CreatureWeaponType weaponType = skill.Pin()->GetSkillRequireWeaponType();
	if (CreatureWeaponType::NONE == weaponType ||
		CreatureWeaponType::HAND == weaponType ||
		CreatureWeaponType::ALL == weaponType ||
		CreatureWeaponType::MAX == weaponType)
	{
		return false;
	}

	_skillId = skill.Pin()->GetSkillId();

	return Super::OnPrevStart();
}

bool FGsTutorialSkill::StartActionInternal()
{
	EGsTutorialActionType actionType = _currAction.Pin()->GetActionType();
	if (EGsTutorialActionType::SKILL_SELECT == actionType ||
		EGsTutorialActionType::SKILL_LEARN == actionType)
	{
		// 대상 스킬 정보를 넘겨줌
		FGsTutorialMsgParamSkill param(_skillId);
		_currAction.Pin()->OnPrevStart(&param);
	}

	return Super::StartActionInternal();
}

bool FGsTutorialSkill::SetTargetSkillBookList()
{
	if (const FGsSchemaTutorialTargetId* targetIdRow = _table->conditionRow.GetRowByType<FGsSchemaTutorialTargetId>())
	{
		RewardId rewardId = static_cast<RewardId>(targetIdRow->targetId);

		FGsRewardUnboxingData unboxData;
		FGsRewardHelper::UnboxingRewardData(rewardId, unboxData);

		_targetSkillBookList.Empty();
		for (const FGsSchemaRewardItemBagData* iter : unboxData._itemDataList)
		{
			_targetSkillBookList.Emplace(iter->itemId);
		}

		if (0 < _targetSkillBookList.Num())
		{
			return true;
		}
	}

	return false;
}
