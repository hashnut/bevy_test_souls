// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTutorialFactory.h"
#include "Tutorial/Tutorial/GsTutorialDefault.h"
#include "Tutorial/Tutorial/GsTutorialGuildJoin.h"
#include "Tutorial/Tutorial/GsTutorialGuildContents.h"
#include "Tutorial/Tutorial/GsTutorialTitleSystem.h"
#include "Tutorial/Tutorial/GsTutorialPlayerStat.h"
#include "Tutorial/Tutorial/GsTutorialFairy.h"
#include "Tutorial/Tutorial/GsTutorialCostume.h"
#include "Tutorial/Tutorial/GsTutorialSkill.h"
#include "Tutorial/Tutorial/GsTutorialWeaponEquip.h"
#include "Tutorial/Tutorial/GsTutorialSpiritShot.h"
#include "Tutorial/Condition/GsTutorialConditionQuest.h"
#include "Tutorial/Condition/GsTutorialConditionQuestId.h"
#include "Tutorial/Condition/GsTutorialConditionQuestStoryId.h"
#include "Tutorial/Condition/GsTutorialConditionOpenUI.h"
#include "Tutorial/Condition/GsTutorialConditionOpenUICheckParam.h"
#include "Tutorial/Condition/GsTutorialConditionUnlockContent.h"
#include "Tutorial/Condition/GsTutorialConditionFirstTutorial.h"
#include "Tutorial/Action/GsTutorialActionDefault.h"
#include "Tutorial/Action/GsTutorialActionUIInteraction.h"
#include "Tutorial/Action/GsTutorialActionNpcShopSelectPotionIcon.h"
#include "Tutorial/Action/GsTutorialActionNpcShopSelectPotionList.h"
#include "Tutorial/Action/GsTutorialActionSkillSelect.h"
#include "Tutorial/Action/GsTutorialActionSkillLearn.h"
#include "Tutorial/Action/GsTutorialActionSkillEquip.h"
#include "Tutorial/Action/GsTutorialActionInventorySelectItem.h"
#include "Tutorial/Action/GsTutorialActionSummonShowImmediately.h"
#include "Tutorial/Action/GsTutorialActionFairySelect.h"
#include "Tutorial/Action/GsTutorialActionCostumeSelect.h"
#include "Tutorial/Action/GsTutorialActionCraftSelect.h"
#include "Tutorial/Action/GsTutorialActionMonsterKnowledgeSelect.h"
#include "Tutorial/Action/GsTutorialActionHUDSkillBtnSelect.h"
#include "Tutorial/Action/GsTutorialActionHUDNpcBtnSelect.h"
#include "Tutorial/Action/GsTutorialActionHUDQuickSlotSelect.h"
#include "Tutorial/Action/GsTutorialActionItemEnchantSelectItem.h"
#include "Tutorial/Action/GsTutorialActionHUDQuestSelect.h"
#include "Tutorial/Action/GsTutorialActionHUDScanSlotSelect.h"
#include "Tutorial/Action/GsTutorialActionHUDPotionIconSelect.h"
#include "Tutorial/Action/GsTutorialActionMultiLevelRankSelect.h"
#include "Tutorial/Action/GsTutorialActionBMShopTabSelect.h"
#include "Tutorial/Action/GsTutorialActionBMShopProductSelect.h"
#include "Tutorial/Action/GsTutorialActionHUDQuestTeleport.h"
#include "DataSchema/Tutorial/GsSchemaTutorial.h"
#include "DataSchema/Tutorial/GsSchemaTutorialAction.h"
#include "T1Project.h"


TSharedPtr<FGsTutorialBase> FGsTutorialFactory::CreateTutorial(const FGsSchemaTutorial* InTable)
{
	// NONE은 에러 상황 아님. 포함 안할 튜토리얼을 NONE으로 지정
	if (EGsTutorialType::NONE == InTable->tutorialType)
	{
		return nullptr;
	}

	TSharedPtr<FGsTutorialBase> outTutorial = nullptr;
	switch (InTable->tutorialType)
	{
	case EGsTutorialType::DEFAULT:
	case EGsTutorialType::TEST_TUTORIAL:
		outTutorial = MakeShared<FGsTutorialDefault>();
		break;
	case EGsTutorialType::GUILD_JOIN:
		outTutorial = MakeShared<FGsTutorialGuildJoin>();
		break;
	case EGsTutorialType::GUILD_CONTENTS:
		outTutorial = MakeShared<FGsTutorialGuildContents>();
		break;
	case EGsTutorialType::TITLE_SYSTEM:
		outTutorial = MakeShared<FGsTutorialTitleSystem>();
		break;
	case EGsTutorialType::PLAYER_STAT:
		outTutorial = MakeShared<FGsTutorialPlayerStat>();
		break;
	case EGsTutorialType::FAIRY:
		outTutorial = MakeShared<FGsTutorialFairy>();
		break;
	case EGsTutorialType::COSTUME:
		outTutorial = MakeShared<FGsTutorialCostume>();
		break;
	case EGsTutorialType::SKILL:
		outTutorial = MakeShared<FGsTutorialSkill>();
		break;
	case EGsTutorialType::WEAPON_EQUIP:
		outTutorial = MakeShared<FGsTutorialWeaponEquip>();
		break;
	case EGsTutorialType::SPIRIT_SHOT:
		outTutorial = MakeShared<FGsTutorialSpiritShot>();
		break;
	}

	if (outTutorial.IsValid())
	{
		outTutorial->Initialize(InTable);
		return outTutorial;
	}

	GSLOG(Error, TEXT("[Tutorial] Fail to create tutorial. id: %d, type: %d"), InTable->id, InTable->tutorialType);
	return nullptr;
}

TSharedPtr<FGsTutorialConditionBase> FGsTutorialFactory::CreateCondition(const FGsSchemaTutorial* InTable)
{
	TSharedPtr<FGsTutorialConditionBase> outCondition = nullptr;
	switch (InTable->conditionType)
	{
	case EGsTutorialConditionType::QUEST:
		outCondition = MakeShared<FGsTutorialConditionQuest>();
		break;
	case EGsTutorialConditionType::QUEST_ID:
		outCondition = MakeShared<FGsTutorialConditionQuestId>();
		break;
	case EGsTutorialConditionType::QUEST_STORY_ID:
		outCondition = MakeShared<FGsTutorialConditionQuestStoryId>();
		break;
	case EGsTutorialConditionType::OPEN_UI:
		outCondition = MakeShared<FGsTutorialConditionOpenUI>();
		break;
	case EGsTutorialConditionType::OPEN_UI_CHECK_PARAM:
		outCondition = MakeShared<FGsTutorialConditionOpenUICheckParam>();
		break;
	case EGsTutorialConditionType::UNLOCK_CONTENT:
		outCondition = MakeShared<FGsTutorialConditionUnlockContent>();
		break;
	case EGsTutorialConditionType::FIRST_TUTORIAL:
		outCondition = MakeShared<FGsTutorialConditionFirstTutorial>();
		break;
	}

	if (outCondition)
	{
		outCondition->Initialize(InTable);
		return outCondition;
	}

	GSLOG(Error, TEXT("[Tutorial] Fail to create condition. id: %d, type: %d"), InTable->id, InTable->conditionType);
	return nullptr;
}

TSharedPtr<FGsTutorialActionBase> FGsTutorialFactory::CreateAction(const FGsSchemaTutorialAction* InTable)
{
	TSharedPtr<FGsTutorialActionBase> outAction = nullptr;
	switch (InTable->actionType)
	{
	case EGsTutorialActionType::UI_INTERACTION:
		outAction = MakeShared<FGsTutorialActionUIInteraction>();
		break;	
	case EGsTutorialActionType::NPC_SHOP_SELECT_POTION_ICON:
		outAction = MakeShared<FGsTutorialActionNpcShopSelectPotionIcon>();
		break;
	case EGsTutorialActionType::NPC_SHOP_SELECT_POTION_LIST:
		outAction = MakeShared<FGsTutorialActionNpcShopSelectPotionList>();
		break;
	case EGsTutorialActionType::SKILL_SELECT:
		outAction = MakeShared<FGsTutorialActionSkillSelect>();
		break;
	case EGsTutorialActionType::SKILL_LEARN:
		outAction = MakeShared<FGsTutorialActionSkillLearn>();
		break;
	case EGsTutorialActionType::SKILL_EQUIP:
		outAction = MakeShared<FGsTutorialActionSkillEquip>();
		break;
	case EGsTutorialActionType::INVENTORY_SELECT_ITEM:
		outAction = MakeShared<FGsTutorialActionInventorySelectItem>();
		break;
	case EGsTutorialActionType::SUMMON_SHOW_IMMEDIATELY:
		outAction = MakeShared<FGsTutorialActionSummonShowImmediately>();
		break;
	case EGsTutorialActionType::FAIRY_SELECT:
		outAction = MakeShared<FGsTutorialActionFairySelect>();
		break;
	case EGsTutorialActionType::COSTUME_SELECT:
		outAction = MakeShared<FGsTutorialActionCostumeSelect>();
		break;
	case EGsTutorialActionType::CRAFT_SELECT:
		outAction = MakeShared<FGsTutorialActionCraftSelect>();
		break;
	case EGsTutorialActionType::MONSTER_KNOWLEDGE_SELECT:
		outAction = MakeShared<FGsTutorialActionMonsterKnowledgeSelect>();
		break;
	case EGsTutorialActionType::HUD_SKILL_BTN_SELECT:
		outAction = MakeShared<FGsTutorialActionHUDSkillBtnSelect>();
		break;
	case EGsTutorialActionType::HUD_NPC_BTN_SELECT:
		outAction = MakeShared<FGsTutorialActionHUDNpcBtnSelect>();
		break;
	case EGsTutorialActionType::HUD_QUICK_SLOT_SELECT:
		outAction = MakeShared<FGsTutorialActionHUDQuickSlotSelect>();
		break;
	case EGsTutorialActionType::HUD_QUEST_SELECT:
		outAction = MakeShared<FGsTutorialActionHUDQuestSelect>();
		break;
	case EGsTutorialActionType::HUD_SCAN_SLOT_SELECT:
		outAction = MakeShared<FGsTutorialActionHUDScanSlotSelect>();
		break;
	case EGsTutorialActionType::HUD_POTION_ICON_SELECT:
		outAction = MakeShared<FGsTutorialActionHUDPotionIconSelect>();
		break;
	case EGsTutorialActionType::ITEM_ENCHANT_SELECT_ITEM:
		outAction = MakeShared<FGsTutorialActionItemEnchantSelectItem>();
		break;
	case EGsTutorialActionType::MULTI_LEVEL_RANK_SELECT:
		outAction = MakeShared<FGsTutorialActionMultiLevelRankSelect>();
		break;
	case EGsTutorialActionType::BMSHOP_TAB_SELECT:
		outAction = MakeShared<FGsTutorialActionBMShopTabSelect>();
		break;
	case EGsTutorialActionType::BMSHOP_PRODUCT_SELECT:
		outAction = MakeShared<FGsTutorialActionBMShopProductSelect>();
		break;
	case EGsTutorialActionType::HUD_QUEST_TELEPORT:
		outAction = MakeShared<FGsTutorialActionHUDQuestTeleport>();
		break;
	default:
		outAction = MakeShared<FGsTutorialActionDefault>();
		break;
	}

	outAction->Initialize(InTable);

	return outAction;
}
