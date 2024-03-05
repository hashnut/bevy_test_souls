// Fill out your copyright notice in the Description page of Project Settings.


#include "GsSkillHelper.h"
#include "DataSchema/User/GsSchemaUserSkillSet.h"
#include "DataSchema/Skill/GsSchemaSkillSet.h"
#include "DataSchema/Skill/Cost/GsSchemaSkillUpCostSet.h"
#include "DataSchema/Skill/Level/GsSchemaSkillLevelSet.h"
#include "DataSchema/Skill/Upgrade/GsSchemaSkillUpgradeSet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivityEffectSkillUpgradeSet.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"
#include "GameObject/Abnormality/GsAbnormalityHandlerLocalPlayer.h"
#include "Skill/GsSkill.h"
#include "Item/GsItemManager.h"
#include "Currency/GsCurrencyHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "T1Project.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "UTIL/GsTableUtil.h"
#include "Costume/GsCostumeData.h"


TWeakPtr<const FGsSkill> FGsSkillHelper::GetSkillByUserSkillSetId(int32 InUserSkillSetId, CreatureGenderType InGender)
{
	const UGsTableUserSkillSet* table = Cast<UGsTableUserSkillSet>(FGsSchemaUserSkillSet::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}

	const FGsSchemaUserSkillSet* row;
	if (false == table->FindRowById(InUserSkillSetId, row))
	{
		return nullptr;
	}

	const FGsSchemaSkillSet* skillSet = nullptr;
	if (CreatureGenderType::FEMALE == InGender)
	{
		skillSet = row->femaleSkillId.GetRow();
	}
	else if (CreatureGenderType::MALE == InGender)
	{
		skillSet = row->maleSkillId.GetRow();
	}

	if (nullptr == skillSet)
	{
		return nullptr;
	}

	return GSkill()->FindSkillWeakPtr(skillSet->id, skillSet->requireWeapon);
}

TWeakPtr<const FGsSkill> FGsSkillHelper::GetSkillByUserSkillSet(const FGsSchemaUserSkillSet* InRow, CreatureGenderType InGender)
{
	if (InRow)
	{
		const FGsSchemaSkillSet* skillSet = nullptr;
		if (CreatureGenderType::FEMALE == InGender)
		{
			skillSet = InRow->femaleSkillId.GetRow();
		}
		else if (CreatureGenderType::MALE == InGender)
		{
			skillSet = InRow->maleSkillId.GetRow();
		}

		if (skillSet)
		{
			return GSkill()->FindSkillWeakPtr(skillSet->id, skillSet->requireWeapon);
		}

		GSLOG(Warning, TEXT("[Skill] Fail to get skill by userSkillSet. userSkillSet: %d, gender: %d"), InRow->id, InGender);
	}

	return nullptr;
}

SkillId FGsSkillHelper::GetSkillIdByUserSkillSetId(int32 InUserSkillSetId, CreatureGenderType InGender)
{
	const UGsTableUserSkillSet* table = Cast<UGsTableUserSkillSet>(FGsSchemaUserSkillSet::GetStaticTable());
	if (nullptr == table)
	{
		return INVALID_SKILL_ID;
	}

	const FGsSchemaUserSkillSet* row;
	if (false == table->FindRowById(InUserSkillSetId, row))
	{
		return INVALID_SKILL_ID;
	}

	const FGsSchemaSkillSet* skillSet = nullptr;
	if (CreatureGenderType::FEMALE == InGender)
	{
		skillSet = row->femaleSkillId.GetRow();
	}
	else if (CreatureGenderType::MALE == InGender)
	{
		skillSet = row->maleSkillId.GetRow();
	}

	if (nullptr == skillSet)
	{
		return INVALID_SKILL_ID;
	}

	return skillSet->id;
}

bool FGsSkillHelper::IsLearnedSkill(int32 InUserSkillSetId)
{
	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		TWeakPtr<const FGsSkill> skill = GetSkillByUserSkillSetId(InUserSkillSetId, userData->mGender);
		if (skill.IsValid())
		{
			return IsLearnedSkill(skill.Pin().Get());
		}
	}

	return false;
}

bool FGsSkillHelper::IsLearnedSkill(const FGsSkill* InSkill)
{
	// 배웠으면 바로 True 리턴
	if (InSkill->_isLearn)
	{
		return true;
	}

	// 업그레이드 스킬의 루트스킬 검사 로직
	// - 업그레이드 스킬일 경우 현재 단계의 스킬만 받는데, 루트 스킬은 받은 정보가 없어서 배웠는지 여부를 알수 없음.
	// - 따라서 업그레이드 스킬 중 배운게 있는지를 검사하여 배웠는지 판정.
	// - 루트스킬 외에는 스킬북을 만들지 않기로 논의되었으므로 검사하지 않아도 됨
	if (const FGsSchemaSkillLevelSet* levelSet = InSkill->GetCurrLevelSet())
	{
		// max값은 있는데 현재 스탭이 0일 것은 루트스킬
		if (0 < levelSet->maxUpgradeStep &&
			0 == levelSet->upgradeInfo.currentStep)
		{
			const FGsSchemaSkillUpgradeSet* upgradeData = levelSet->upgradeInfo.skillUpgradeId.GetRow();
			if (nullptr == upgradeData)
			{
				return false;
			}

			const FGsNetUserData* userData = GGameData()->GetUserData();
			if (nullptr == userData)
			{
				return false;
			}

			for (int32 i = 0; i < upgradeData->upgradeInfoList.Num(); ++i)
			{
				TWeakPtr<const FGsSkill> skillUpgrade = GetSkillByUserSkillSet(
					upgradeData->upgradeInfoList[i].upgradeSkillId.GetRow(), userData->mGender);

				if (skillUpgrade.IsValid())
				{
					// 하나라도 배운게 있으면 루트스킬을 배운것으로 판정
					if (skillUpgrade.Pin()->_isLearn)
					{
						return true;
					}
				}
			}
		}
	}

	return false;
}

const FGsSchemaSkillUpCostSet* FGsSkillHelper::GetSkillUpCostSet(int InSkillUpCostId)
{
	const UGsTableSkillUpCostSet* table = Cast<const UGsTableSkillUpCostSet>(FGsSchemaSkillUpCostSet::GetStaticTable());
	if (table)
	{
		const FGsSchemaSkillUpCostSet* row = nullptr;
		if (table->FindRowById(InSkillUpCostId, row))
		{
			return row;
		}
	}

	return nullptr;
}

bool FGsSkillHelper::CheckSkillUpCost(int32 InSkillUpCostId, bool bCheckCurrency, bool bShowMsg)
{
	const FGsSchemaSkillUpCostSet* costSet = GetSkillUpCostSet(InSkillUpCostId);
	if (nullptr == costSet)
	{
		return false;
	}

	// 소모 재화 체크. 기획팀과 논의 후 무조건 0번째만 사용하도록 하였으나, 혹시 몰라 일단 리스트 검사로 구현
	if (bCheckCurrency)
	{
		for (const FGsSchemaSkillUpCurrencyInfo& skillUpCurrency : costSet->currencyList)
		{
			if (false == FGsCurrencyHelper::CheckCurrency(skillUpCurrency.currencyType, skillUpCurrency.currencyAmount, bShowMsg))
			{
				return false;
			}
		}
	}

	// 소모 아이템 체크
	bool bIsEnough = true;
	UGsItemManager* itemMgr = GItem();
	for (const FGsSchemaSkillUpItemInfo& skillUpInfo : costSet->itemList)
	{
		ItemAmount count = itemMgr->GetAllItemCountByTID(skillUpInfo.itemId, true);
		if (skillUpInfo.itemCount > count)
		{
			bIsEnough = false;
			break;
		}
	}

	if (bShowMsg)
	{
		if (false == bIsEnough)
		{
			// TEXT: 재료 아이템이 부족합니다.
			FText textMsg;
			FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Enchant_Notice_NotEnoughCostItem"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}
	}

	return bIsEnough;
}

bool FGsSkillHelper::CanLearn(const FGsSkill* InSkill, bool bShowMsg)
{
	if (nullptr == InSkill ||
		nullptr == InSkill->_tableData)
	{
		return false;
	}

	// 이미 배웠으면 배울 수 없음
	if (IsLearnedSkill(InSkill))
	{
		if (bShowMsg)
		{
			// TEXT: 이미 습득한 스킬입니다.
			FText textMsg;
			FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Notice_AlreadyLearn"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		return false;
	}

	// 업그레이드 패시브 스킬 체크
	if (SkillCategorySet::PASSIVE == InSkill->GetSkillCateogrySet())
	{
		// 업그레이드 스킬일 경우, Root스킬을 가지고있는지, 필수 패시비티를 가지고 있는지 체크 필요
		// 무기가 다를 경우 서버가 패시비티를 주지 않으므로, 클라는 필수 패시비티는 체크를 하지 않음
		TWeakPtr<const FGsSkill> rootSkill = GetUpgradeRootSkillByPassivitySkill(InSkill);
		if (rootSkill.IsValid())
		{
			if (false == rootSkill.Pin()->_isLearn)
			{
				int32 upgradeStep = GetUpgradeSkillStepByPassivitySkill(InSkill);

				bool bHasRootSkill = false;
				if (-1 != upgradeStep)
				{
					if (HasRootSkill(rootSkill.Pin().Get(), upgradeStep))
					{
						bHasRootSkill = true;
					}
				}
				
				if (false == bHasRootSkill)
				{
					if (bShowMsg)
					{
						// TEXT: 필수 패시브 및 기본 스킬을 보유해야 해당 스킬을 배울 수 있습니다
						FText textMsg;
						FText::FindText(TEXT("SkillUIText"), TEXT("Notice_UseSkillBook_NoPreviousSkill"), textMsg);
						FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
					}

					return false;
				}
			}
		}
	}
	else
	{
		// 업그레이드 스킬은 루트 스킬을 제외하고는 패시브 스킬북만 존재한다
		if (0 < InSkill->GetUpgradeStep())
		{
			return false;
		}
	}

	// 코스튬 스킬일 경우 
	CostumeId costumeId = GetCostumeIdBySkillId(InSkill->GetSkillId());
	if (INVALID_COSTUME_ID != costumeId)
	{
		// 코스튬이 있는데 스킬 습득을 안했다
		TWeakPtr<FGsCostumeData> costumeData = GSCostume()->GetCostumeData(costumeId);
		if (costumeData.IsValid())
		{
			if (costumeData.Pin()->isCollectedCostume())
			{
				return true;
			}
		}

		return false;
	}

	// 스킬북이 존재하는지 체크
	FGsSkillUIDataHandler* skillDataHandler = GSkill()->GetSkillUIDataHandler();
	if (nullptr == skillDataHandler)
	{
		return false;
	}

	const TArray<ItemId>* skillBookListPtr = skillDataHandler->GetExistSkillBookList(InSkill->GetSkillId());
	if (nullptr == skillBookListPtr ||
		0 == skillBookListPtr->Num())
	{
		return false;
	}

	for (const ItemId& skillBookItemId : *skillBookListPtr)
	{
		TArray<TWeakPtr<FGsItem>> skillBookList = GItem()->FindByTID(skillBookItemId, ItemType::CONSUME);
		for (TWeakPtr<const FGsItem> item : skillBookList)
		{
			if (item.IsValid())
			{
				// 스킬북 잠겼는지 체크
				if (false == item.Pin()->GetLock())
				{
					return true;
				}
			}
		}
	}

	return false;
}

bool FGsSkillHelper::CanReinforce(const FGsSkill* InSkill, bool bCheckCurrency)
{
#if USE_SKILL_ENCHANT
	if (nullptr == InSkill ||
		nullptr == InSkill->_tableData)
	{
		return false;
	}

	// 안배웠으면 불가
	if (false == InSkill->_isLearn)
	{
		return false;
	}

	// 최대 강화 레벨이면 불가
	if (InSkill->GetLevel() >= InSkill->GetMaxEnchantLevel())
	{
		return false;
	}

	const FGsSchemaSkillLevelInfo* levelInfo = InSkill->GetLevelInfo(InSkill->GetLevel());
	if (nullptr == levelInfo)
	{
		return false;
	}

	// 재료 부족하면 불가
	if (false == FGsSkillHelper::CheckSkillUpCost(levelInfo->enchantSkillUpCostId, bCheckCurrency))
	{
		return false;
	}

	return true;
#else
	return false;
#endif
}

bool FGsSkillHelper::CheckSkillRedDot(const FGsSkill* InSkill)
{
	if (InSkill && InSkill->_isNew)
	{
		return true;
	}

	if (CanLearn(InSkill))
	{
		return true;
	}

	return false;
}

bool FGsSkillHelper::CanUseSkillBook(int32 InUserSkillSetId, bool bShowMsg)
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
	{
		return false;
	}

	TWeakPtr<const FGsSkill> skill = GetSkillByUserSkillSetId(InUserSkillSetId, userData->mGender);
	if (false == skill.IsValid() ||
		nullptr == skill.Pin()->_tableData)
	{
		return false;
	}

	// 이미 배운 스킬이면 사용할 수 없음
	if (IsLearnedSkill(skill.Pin().Get()))
	{
		if (bShowMsg)
		{
			// TEXT: 이미 습득한 스킬입니다.
			FText textMsg;
			FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Notice_AlreadyLearn"), textMsg);
			FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
		}

		return false;
	}

	if (SkillCategorySet::PASSIVE == skill.Pin()->GetSkillCateogrySet())
	{
		// 업그레이드 스킬일 경우, Root스킬을 가지고있는지, 필수 패시비티를 가지고 있는지 체크 필요
		// 무기가 다를 경우 서버가 패시비티를 주지 않으므로, 클라는 필수 패시비티는 체크를 하지 않음
		TWeakPtr<const FGsSkill> rootSkill = GetUpgradeRootSkillByPassivitySkill(skill.Pin().Get());
		if (rootSkill.IsValid())
		{
			bool bIsLearn = false;
			if (rootSkill.Pin()->_isLearn)
			{
				bIsLearn = true;
			}
			else
			{
				TArray<TWeakPtr<const FGsSkill>> upgradeSkillList;
				GetSkillListByUpgradeStep(rootSkill.Pin().Get(), upgradeSkillList);

				for (TWeakPtr<const FGsSkill> iter : upgradeSkillList)
				{
					if (iter.Pin()->_isLearn)
					{
						bIsLearn = true;
						break;
					}
				}
			}			

			if (false == bIsLearn)
			{
				if (bShowMsg)
				{
					// TEXT: 필수 패시브 및 기본 스킬을 보유해야 해당 스킬을 배울 수 있습니다
					FText textMsg;
					FText::FindText(TEXT("SkillUIText"), TEXT("Notice_UseSkillBook_NoPreviousSkill"), textMsg);
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textMsg);
				}

				return false;
			}
		}
	}
	else
	{
		// 업그레이드 스킬은 루트 스킬을 제외하고는 패시브 스킬북만 존재한다
		if (0 < skill.Pin()->GetUpgradeStep())
		{
			return false;
		}
	}

	return true;
}

void FGsSkillHelper::GetSkillListByUpgradeStep(const FGsSkill* InSkill, OUT TArray<TWeakPtr<const FGsSkill>>& OutList)
{
	if (nullptr == InSkill ||
		nullptr == InSkill->_tableData)
	{
		return;
	}

	const FGsSchemaSkillLevelSet* levelSet = InSkill->GetCurrLevelSet();
	if (nullptr == levelSet)
	{
		return;
	}

	// 업그레이드 스킬이 아닐 경우
	if (0 == levelSet->maxUpgradeStep)
	{
		return;
	}

	const FGsSchemaSkillUpgradeSet* upgradeData = levelSet->upgradeInfo.skillUpgradeId.GetRow();
	if (nullptr == upgradeData)
	{
		return;
	}

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
	{
		return;
	}

	const FGsSchemaUserSkillSet* userSkillSetRoot = upgradeData->rootSkillId.GetRow();
	if (nullptr == userSkillSetRoot)
	{
		return;
	}

	TWeakPtr<const FGsSkill> skillRoot = GetSkillByUserSkillSet(upgradeData->rootSkillId.GetRow(), userData->mGender);
	if (false == skillRoot.IsValid())
	{
		return;
	}

	OutList.Emplace(skillRoot);

	for (int32 i = 0; i < upgradeData->upgradeInfoList.Num(); ++i)
	{
		TWeakPtr<const FGsSkill> skillUpgrade = GetSkillByUserSkillSet(
			upgradeData->upgradeInfoList[i].upgradeSkillId.GetRow(), userData->mGender);

		if (skillUpgrade.IsValid())
		{
			OutList.Emplace(skillUpgrade);
		}
	}
}

const FGsSchemaPassivitySet* FGsSkillHelper::GetUpgradeSkillRequirePassivity(const FGsSchemaSkillLevelSet* InLevelSet)
{
	if (nullptr == InLevelSet)
	{
		return nullptr;
	}

	// 업그레이드 스킬이 아니면 불가
	if (0 == InLevelSet->maxUpgradeStep)
	{
		return nullptr;
	}

	// 필요 패시비티 없으면 불가
	const FGsSchemaSkillUpgradeSet* upgradeData = InLevelSet->upgradeInfo.skillUpgradeId.GetRow();
	if (nullptr == upgradeData)
	{
		return nullptr;
	}

	// 0 스텝은 뿌리스킬로 필요 패시비티 없음
	if (0 == InLevelSet->upgradeInfo.currentStep)
	{
		return nullptr;
	}

	// step은 1부터 시작하는 수 이고, 인덱스는 0부터 시작함
	int32 infoIndex = InLevelSet->upgradeInfo.currentStep - 1;
	if (false == upgradeData->upgradeInfoList.IsValidIndex(infoIndex))
	{
		return nullptr;
	}

	return upgradeData->upgradeInfoList[infoIndex].requirePassivity.GetRow();
}

TWeakPtr<const FGsSkill> FGsSkillHelper::GetUpgradeRootSkill(const FGsSkill* InSkill)
{
	if (nullptr == InSkill ||
		nullptr == InSkill->_tableData)
	{
		return nullptr;
	}

	if (SkillCategorySet::PASSIVE == InSkill->GetSkillCateogrySet())
	{
		return nullptr;
	}

	const FGsSchemaSkillLevelSet* levelSet = InSkill->GetCurrLevelSet();
	if (nullptr == levelSet)
	{
		return nullptr;
	}

	// 업그레이드 스킬이 아닐 경우
	if (0 == levelSet->maxUpgradeStep)
	{
		return nullptr;
	}

	const FGsSchemaSkillUpgradeSet* upgradeData = levelSet->upgradeInfo.skillUpgradeId.GetRow();
	if (nullptr == upgradeData)
	{
		return nullptr;
	}

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
	{
		return nullptr;
	}

	const FGsSchemaUserSkillSet* userSkillSetRoot = upgradeData->rootSkillId.GetRow();
	if (nullptr == userSkillSetRoot)
	{
		return nullptr;
	}

	return GetSkillByUserSkillSet(upgradeData->rootSkillId.GetRow(), userData->mGender);
}

SkillId FGsSkillHelper::GetUpgradeRootSkillId(const FGsSkill* InSkill)
{
	if (nullptr == InSkill ||
		nullptr == InSkill->_tableData)
	{
		return INVALID_SKILL_ID;
	}

	const FGsSchemaSkillLevelSet* levelSet = InSkill->GetCurrLevelSet();
	if (nullptr == levelSet)
	{
		return INVALID_SKILL_ID;
	}

	// 업그레이드 스킬이 아닐 경우
	if (0 == levelSet->maxUpgradeStep)
	{
		return INVALID_SKILL_ID;
	}

	const FGsSchemaSkillUpgradeSet* upgradeData = levelSet->upgradeInfo.skillUpgradeId.GetRow();
	if (nullptr == upgradeData)
	{
		return INVALID_SKILL_ID;
	}

	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
	{
		return INVALID_SKILL_ID;
	}

	const FGsSchemaUserSkillSet* userSkillSetRoot = upgradeData->rootSkillId.GetRow();
	if (nullptr == userSkillSetRoot)
	{
		return INVALID_SKILL_ID;
	}

	return GetSkillIdByUserSkillSetId(userSkillSetRoot->id, userData->mGender);
}

bool FGsSkillHelper::GetSkillSimpleInfoByUserSkillSetId(int32 InUserSkillSetId,
	OUT FText& OutSkillName, OUT FText& OutSkillDesc, OUT ItemGrade& OutGrade)
{
	if (0 < InUserSkillSetId)
	{
		const FGsSchemaUserSkillSet* userSkillSet =
			UGsTableUtil::FindRowById<UGsTableUserSkillSet, FGsSchemaUserSkillSet>(InUserSkillSetId);
		if (userSkillSet)
		{
			if (const FGsSchemaSkillSet* skillSet = userSkillSet->maleSkillId.GetRow())
			{
				OutSkillName = userSkillSet->nameText;
				OutSkillDesc = userSkillSet->descText;
				OutGrade = skillSet->grade;

				return true;
			}
		}
	}	

	return false;
}

CostumeId FGsSkillHelper::GetCostumeIdBySkillId(SkillId InSkillId)
{
	if (FGsSkillUIDataHandler* skillDataHandler = GSkill()->GetSkillUIDataHandler())
	{
		return skillDataHandler->GetCostumeIdBySkillId(InSkillId);
	}

	return INVALID_COSTUME_ID;
}

bool FGsSkillHelper::HasRootSkill(const FGsSkill* InRootSkill, int32 InCurrStep)
{
	if (nullptr == InRootSkill)
	{
		return false;
	}

	// 현재 스킬이면 리턴
	if (0 == InCurrStep)
	{
		return InRootSkill->_isLearn;
	}

	// 단계 순서대로 가져와짐
	TArray<TWeakPtr<const FGsSkill>> skillList;
	GetSkillListByUpgradeStep(InRootSkill, skillList);

	// 현 단계 이하에 배운 스킬이 있으면 true
	for (int i = 0; i < skillList.Num(); ++i)
	{
		if (InCurrStep < i)
		{
			break;
		}
		else
		{
			if (skillList[i].Pin()->_isLearn)
			{
				return true;
			}
		}
	}

	return false;
}

TWeakPtr<const FGsSkill> FGsSkillHelper::GetUpgradeSkillByPassivitySkill(const FGsSkill* InSkill)
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
	{
		return nullptr;
	}

	for (const FGsSchemaPassivitySetRow& itPassivityRow : InSkill->_tableData->passivityIdList)
	{
		const FGsSchemaPassivitySet* passivitySet = itPassivityRow.GetRow();
		if (nullptr == passivitySet)
		{
			continue;
		}

		// 업그레이드 스킬의 스킬북일 경우, 필수 패시비티 체크
		if (PassivityType::SKILL_UPGRADE == passivitySet->type)
		{
			bool bIsRootSkill = false;
			for (const FGsSchemaPassivityEffectInfo& itEffectInfo : passivitySet->effectInfoList)
			{
				const FGsSchemaPassivityEffectSkillUpgradeSet* effectUpgrade =
					UGsTableUtil::FindRowById<UGsTablePassivityEffectSkillUpgradeSet, FGsSchemaPassivityEffectSkillUpgradeSet>(itEffectInfo.effectId);
				if (nullptr == effectUpgrade)
				{
					continue;
				}

				const FGsSchemaSkillUpgradeSet* upgradeSet = effectUpgrade->param.skillUpgradeId.GetRow();
				if (nullptr == upgradeSet)
				{
					continue;
				}

				for (const FGsSchemaSkillUpgradeStepInfo& stepInfo : upgradeSet->upgradeInfoList)
				{
					if (stepInfo.step == effectUpgrade->param.currentStep)
					{
						const FGsSchemaUserSkillSet* skillUserSkillSet = stepInfo.upgradeSkillId.GetRow();
						if (nullptr == upgradeSet)
						{
							continue;
						}

						return GetSkillByUserSkillSet(skillUserSkillSet, userData->mGender);
					}
				}
			}
		}
	}

	return nullptr;
}

TWeakPtr<const FGsSkill> FGsSkillHelper::GetUpgradeRootSkillByPassivitySkill(const FGsSkill* InSkill)
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
	{
		return nullptr;
	}

	for (const FGsSchemaPassivitySetRow& itPassivityRow : InSkill->_tableData->passivityIdList)
	{
		const FGsSchemaPassivitySet* passivitySet = itPassivityRow.GetRow();
		if (nullptr == passivitySet)
		{
			continue;
		}

		// 업그레이드 스킬의 스킬북일 경우, 필수 패시비티 체크
		if (PassivityType::SKILL_UPGRADE == passivitySet->type)
		{
			bool bIsRootSkill = false;
			for (const FGsSchemaPassivityEffectInfo& itEffectInfo : passivitySet->effectInfoList)
			{
				const FGsSchemaPassivityEffectSkillUpgradeSet* effectUpgrade =
					UGsTableUtil::FindRowById<UGsTablePassivityEffectSkillUpgradeSet, FGsSchemaPassivityEffectSkillUpgradeSet>(itEffectInfo.effectId);
				if (nullptr == effectUpgrade)
				{
					continue;
				}

				const FGsSchemaSkillUpgradeSet* upgradeSet = effectUpgrade->param.skillUpgradeId.GetRow();
				if (nullptr == upgradeSet)
				{
					continue;
				}

				const FGsSchemaUserSkillSet* rootSkillUserSkillSet = upgradeSet->rootSkillId.GetRow();
				if (nullptr == upgradeSet)
				{
					continue;
				}

				return GetSkillByUserSkillSet(rootSkillUserSkillSet, userData->mGender);
			}
		}
	}

	return nullptr;
}

int32 FGsSkillHelper::GetUpgradeSkillStepByPassivitySkill(const FGsSkill* InSkill)
{
	const FGsNetUserData* userData = GGameData()->GetUserData();
	if (nullptr == userData)
	{
		return -1;
	}

	for (const FGsSchemaPassivitySetRow& itPassivityRow : InSkill->_tableData->passivityIdList)
	{
		const FGsSchemaPassivitySet* passivitySet = itPassivityRow.GetRow();
		if (nullptr == passivitySet)
		{
			continue;
		}

		// 업그레이드 스킬의 스킬북일 경우, 필수 패시비티 체크
		if (PassivityType::SKILL_UPGRADE == passivitySet->type)
		{
			bool bIsRootSkill = false;
			for (const FGsSchemaPassivityEffectInfo& itEffectInfo : passivitySet->effectInfoList)
			{
				const FGsSchemaPassivityEffectSkillUpgradeSet* effectUpgrade =
					UGsTableUtil::FindRowById<UGsTablePassivityEffectSkillUpgradeSet, FGsSchemaPassivityEffectSkillUpgradeSet>(itEffectInfo.effectId);
				if (nullptr == effectUpgrade)
				{
					continue;
				}

				return effectUpgrade->param.currentStep;
			}
		}
	}

	return -1;
}
