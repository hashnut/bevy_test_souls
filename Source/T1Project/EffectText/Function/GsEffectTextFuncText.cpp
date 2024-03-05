// Fill out your copyright notice in the Description page of Project Settings.


#include "GsEffectTextFuncText.h"
#include "EffectText/GsEffectTextDefine.h"
#include "DataSchema/Abnormality/GsSchemaAbnormalitySet.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/UI/GsSchemaItemCategoryDisPlayName.h"
#include "GameObject/Stat/GsStatHelper.h"
#include "UTIL/GsTableUtil.h"


void FGsEffectTextFuncText::GetAbnormalityNameText(AbnormalityEffectType InType, OUT FText& OutData, int32 InOption)
{
	if (FGsEffectTextOptionFunc::IsOption(InOption, EGsEffectTextOption::ABNORAMLITY_APPLY_TYPE_ADD_GROUP))
	{
		// TEXT: 특정 상태이상
		FText::FindText(TEXT("TooltipText"), TEXT("Abnormality_addGroup"), OutData);
		return;
	}

	switch (InType)
 	{
 	case AbnormalityEffectType::OVERTIME_DAMAGE:
		// TEXT: 지속 피해
		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Overtime_Damage"), OutData);
		break;
 	case AbnormalityEffectType::OVERTIME_HOT:
 		// TEXT: 지속 회복
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Overtime_HOT"), OutData);
 		break;
 	case AbnormalityEffectType::OVERTIME_SENSITIVE:
 		// TEXT: 살기 획득
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Overtime_Sensitive"), OutData);
 		break;
 	case AbnormalityEffectType::CC_STUN:
 		// TEXT: 기절
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_CC_Stun"), OutData);
 		break;
 	case AbnormalityEffectType::CC_SILENCE:
 		// TEXT: 침묵
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_CC_Silence"), OutData);
 		break;
 	case AbnormalityEffectType::CC_BIND:
 		// TEXT: 속박
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_CC_Bind"), OutData);
 		break;
 	case AbnormalityEffectType::CC_CANNOT_WARP:
 		// TEXT: 워프할 수 없음
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_CC_CannotWarp"), OutData);
 		break;
 	case AbnormalityEffectType::CC_BURY:
 		// TEXT: 매몰
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_CC_Bury"), OutData);
 		break;
 	case AbnormalityEffectType::CC_KNOCK_BACK:
 		// TEXT: 넉백
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_CC_KnockBack"), OutData);
 		break;
 	case AbnormalityEffectType::CC_FEAR:
 		// TEXT: 공포
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_CC_Fear"), OutData);
 		break;
 	case AbnormalityEffectType::CC_TAUNT:
 		// TEXT: 도발
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_CC_Taunt"), OutData);
 		break;
 	case AbnormalityEffectType::CC_MOVE_LOCATION:
 		// TEXT: 위치 이동
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_CC_MoveLocation"), OutData);
 		break;
 	case AbnormalityEffectType::CC_POLYMORP:
 		// TEXT: 변이
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_CC_Polymorp"), OutData);
 		break;
	case AbnormalityEffectType::CC_BLOCKADE:
		// TEXT: 봉쇄
		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_CC_Blockade"), OutData);
		break;
 	case AbnormalityEffectType::STAT_DASH:
 		// TEXT: 대쉬
 		FText::FindText(TEXT("AbnormalityText"), TEXT("Buff_Dash"), OutData);
 		break;	
 	case AbnormalityEffectType::STAT_IMMUNE:
 		// TEXT: 무적
 		FText::FindText(TEXT("AbnormalityText"), TEXT("Buff_Immune"), OutData);
 		break;	
 	case AbnormalityEffectType::STAT_VENT:
 		// TEXT: 살기 폭발
 		FText::FindText(TEXT("AbnormalityText"), TEXT("Buff_Vent"), OutData);
 		break;
 #ifdef R3_ARCAS_NOT_USE
 	case AbnormalityEffectType::UTIL_ARCAS_FAIRY_BUFF:
 		// TEXT: 아르카스의 가호
 		FText::FindText(TEXT("AbnormalityText"), TEXT("Buff_Arcas"), OutData);
 		break;
 #endif // R3_ARCAS_NOT_USE
 	//case AbnormalityEffectType::UTIL_ON_REMOVE_EXE_SKILL_NOTIFY:
 	//case AbnormalityEffectType::UTIL_NOT_AI_CHANGE_PHASE:
 	case AbnormalityEffectType::UTIL_IMMORTAL:
 		// TEXT: 불사
 		FText::FindText(TEXT("AbnormalityText"), TEXT("Util_Immortal"), OutData);
 		break;
 
 	case AbnormalityEffectType::UTIL_HIDING:
 		// TEXT: 은신
 		FText::FindText(TEXT("AbnormalityText"), TEXT("Util_Hiding"), OutData);
 		break;
  	case AbnormalityEffectType::UTIL_BERSERKER_RAGE:
  		// TEXT: 폭주
  		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_BerserkerRage"), OutData);
  		break;
 	case AbnormalityEffectType::UTIL_POINTED_ARROW:
 		// TEXT: 뾰족 화살
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_PointedArrow"), OutData);
 		break;
 	case AbnormalityEffectType::UTIL_WEDGE_ARROW:
 		// TEXT: 쐐기 화살
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_WedgeArrow"), OutData);
 		break;
 	case AbnormalityEffectType::UTIL_JAGGED_ARROW:
 		// TEXT: 톱니 화살
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_JaggedArrow"), OutData);
 		break;
 	case AbnormalityEffectType::UTIL_FINAL_HOWL:
 		// TEXT: 파이널 하울
 		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_FinalHowl"), OutData);
 		break;
	case AbnormalityEffectType::UTIL_BONUS_DAMAGE:
		// TEXT: 추가피해
		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_BonusDamage"), OutData);
		break;
	case AbnormalityEffectType::UTIL_BUFF_COUNTER:
		// TEXT: 반격
		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_Counter"), OutData);
		break;
	case AbnormalityEffectType::UTIL_BURY_KEEP_TARGET:
		// TEXT: 매몰
		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_BuryKeepTarget"), OutData);
		break;
	case AbnormalityEffectType::UTIL_CONFUSE:
		// TEXT: 혼란
		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_Confuse"), OutData);
		break;
	case AbnormalityEffectType::UTIL_DAMAGE_SHARE:
		// TEXT: 대미지 공유
		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_DamageShare"), OutData);
		break;
	case AbnormalityEffectType::UTIL_CREATE_DEPON_NPC:
		// TEXT: 디폰화
		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_Depon"), OutData);
		break;
	case AbnormalityEffectType::UTIL_HP_ABSORPTION:
		// TEXT: 흡혈
		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_HpAbsorption"), OutData);
		break;
	case AbnormalityEffectType::UTIL_MOUNT_VEHICLE:
		// TEXT: 페어리 가속 효과
		FText::FindText(TEXT("AbnormalityText"), TEXT("EffectType_Util_Vehicle"), OutData);
		break;
 	default:
 		OutData = FText::GetEmpty();
 		break;
 	}
}

void FGsEffectTextFuncText::GetDamageStatTypeText(DamageStatType InType, OUT FText& OutText)
{
	switch (InType)
	{
	case DamageStatType::HP:
		// TEXT: HP
		FText::FindText(TEXT("TooltipText"), TEXT("Abnormality_targetStat_HP"), OutText);
		break;
	case DamageStatType::HP_SHIELD:
		// TEXT: 보호막
		FText::FindText(TEXT("TooltipText"), TEXT("Abnormality_targetStat_HPSHIELD"), OutText);
		break;
	case DamageStatType::MP:
		// TEXT: MP
		FText::FindText(TEXT("TooltipText"), TEXT("Abnormality_targetStat_MP"), OutText);
		break;
	case DamageStatType::SENSITIVE:
		// TEXT: 살기
		FText::FindText(TEXT("StatText"), TEXT("SENSITIVE"), OutText);
		break;
	}
}

void FGsEffectTextFuncText::GetElementalTypeText(DamageElementalType InType, OUT FText& OutText)
{
	switch (InType)
	{
	case DamageElementalType::DARK:
		// TEXT: 암흑 속성 피해 {0}
		FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_elementalDamage_DARK"), OutText);
		break;
	case DamageElementalType::HOLY:
		// TEXT: 신성 속성 피해 {0}
		FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_elementalDamage_HOLY"), OutText);
		break;
	case DamageElementalType::FLAME:
		// TEXT: 화염 속성 피해 {0}
		FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_elementalDamage_FLAME"), OutText);
		break;
	case DamageElementalType::FROST:
		// TEXT: 냉기 속성 피해 {0}
		FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_elementalDamage_FROST"), OutText);
		break;
	}
}

void FGsEffectTextFuncText::GetSkillTargetTypeText(SkillTargetType InType, OUT FText& OutText)
{
	switch (InType)
	{
	case SkillTargetType::CASTER:
		// TEXT: 시전자
		FText::FindText(TEXT("TooltipText"), TEXT("SkillTargetType_Caster"), OutText);
		break;
	case SkillTargetType::TARGET:
		// TEXT: 대상
		FText::FindText(TEXT("TooltipText"), TEXT("SkillTargetType_Target"), OutText);
		break;
	}
}

void FGsEffectTextFuncText::GetHealRatioValueTypeText(HealRatioValueType InType, OUT FText& OutText)
{
	switch (InType)
	{
	case HealRatioValueType::ATTACK:
		// TEXT: 공격력
		FText::FindText(TEXT("TooltipText"), TEXT("HealRatioValueType_Attack"), OutText);
		break;
	case HealRatioValueType::MAX_HP:
		// TEXT: 최대 체력
		FText::FindText(TEXT("TooltipText"), TEXT("HealRatioValueType_MaxHP"), OutText);
		break;
	case HealRatioValueType::CURRENT_HP:
		// TEXT: 현재 체력
		FText::FindText(TEXT("TooltipText"), TEXT("HealRatioValueType_CurrentHP"), OutText);
		break;
	case HealRatioValueType::MAX_MP:
		// TEXT: 최대 마력
		FText::FindText(TEXT("TooltipText"), TEXT("HealRatioValueType_MaxMP"), OutText);
		break;
	case HealRatioValueType::CURRENT_MP:
		// TEXT: 현재 마력
		FText::FindText(TEXT("TooltipText"), TEXT("HealRatioValueType_CurrentMP"), OutText);
		break;
	}
}

bool FGsEffectTextFuncText::GetAbnormalityApplyType(AbnormalityApplyType InType, OUT FText& OutText)
{
	switch (InType)
	{
	// ADD 항목들은 별도 출력 규칙이 있음
	case AbnormalityApplyType::ADD:
	case AbnormalityApplyType::ADD_GROUP:
	case AbnormalityApplyType::REMOVE:
	{
		return false; // 별도 처리 규칙이 있는 항목들
	}	
	case AbnormalityApplyType::REMOVE_ALL:
		// TEXT: 모든 상태이상 해제 
		return FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_abnormalityApply_REMOVEALL"), OutText);
	case AbnormalityApplyType::REMOVE_BUFF:
		// TEXT: 모든 버프 해제 
		return FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_abnormalityApply_REMOVEBUFF"), OutText);
	case AbnormalityApplyType::REMOVE_DEBUFF:
		// TEXT: 모든 디버프 해제
		return FText::FindText(TEXT("TooltipText"), TEXT("SkillNotify_abnormalityApply_REMOVEDEBUFF"), OutText);
	case AbnormalityApplyType::REMOVE_GROUP:
		// TEXT: 특정 상태이상 해제
		return FText::FindText(TEXT("TooltipText"), TEXT("Abnormality_removeGroup"), OutText);
	}

	return false;
}

bool FGsEffectTextFuncText::GetPassivityCauseText(PassivityCauseType InType, OUT FText& OutText)
{
	switch (InType)
	{
	case PassivityCauseType::HIT_TARGET:
		// TEXT: 공격 적중 시
		return FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_HITTARGET"), OutText);
	case PassivityCauseType::HIT_TARGET_NORMAL:
		// TEXT: 평타 적중 시
		return FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_HITTARGET_NORMAL"), OutText);
	case PassivityCauseType::HIT_TARGET_SKILL:
		// TEXT: 스킬 적중 시
		return FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_HITTARGET_SKILL"), OutText);
	case PassivityCauseType::HIT_TARGET_CRITICAL:
		// TEXT: 치명타 적중 시
		return FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_HITTARGET_CRITICAL"), OutText);
	case PassivityCauseType::HIT_SELF:
		// TEXT: 피격 시
		return FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_HITSELF"), OutText);
	case PassivityCauseType::HIT_SELF_NORMAL:
		// TEXT: 평타 피격 시
		return FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_HITSELF_NORMAL"), OutText);
	case PassivityCauseType::HIT_SELF_SKILL:
		// TEXT: 스킬 피격 시
		return FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_HITSELF_SKILL"), OutText);
	case PassivityCauseType::HIT_SELF_CRITICAL:
		// TEXT: 치명타 피격 시
		return FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_HITSELF_CRITICAL"), OutText);
	case PassivityCauseType::DIE_SELF:
		// TEXT: 사망 시
		return FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_DIESELF"), OutText);
	case PassivityCauseType::DIE_TARGET:
		// TEXT: 대상 사망 시
		return FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_DIETARGET"), OutText);
	case PassivityCauseType::START_SKILL_SELF:
	case PassivityCauseType::EQUIP_ITEM_SELF:	
	case PassivityCauseType::UPSERT_ABNORMALITY_TARGET:
	{
		// 별도 출력규칙 있음. FGsEffectTextParsePassivityBase::GetTextPassivityCause 참고
		return false;
	}
	break;
	}
	return false;
}

void FGsEffectTextFuncText::GetItemCategoryText(ItemCategorySub InType, OUT FText& OutText)
{
	if (const FGsSchemaItemCategoryDisPlayName* row =
		UGsTableUtil::FindRowById<UGsTableItemCategoryDisPlayName, FGsSchemaItemCategoryDisPlayName>(InType))
	{
		OutText = row->displayName;
	}

	// FIX: 기획팀 삭제 요청
	/*
	switch (InType)
	{
	case ItemCategorySub::SWORD:
		// TEXT: 대검
		FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_EQUIPITEMSELF_itemCategorySub_SWORD"), OutText);
		break;
	case ItemCategorySub::DAGGER:
		// TEXT: 단검
		FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_EQUIPITEMSELF_itemCategorySub_DAGGER"), OutText);
		break;
	case ItemCategorySub::BOW:
		// TEXT: 활
		FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_EQUIPITEMSELF_itemCategorySub_BOW"), OutText);
		break;
	case ItemCategorySub::WAND:
		// TEXT: 지팡이
		FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_EQUIPITEMSELF_itemCategorySub_WAND"), OutText);
		break;
	case ItemCategorySub::WAEPON_ORB:
		// TEXT: 오브
		FText::FindText(TEXT("TooltipText"), TEXT("Passivity_invoke_Cause_EQUIPITEMSELF_itemCategorySub_ORB"), OutText);
		break;
	}
	*/
}

void FGsEffectTextFuncText::GetCreatureWeaponText(CreatureWeaponType InType, OUT FText& OutText)
{
	switch (InType)
	{
	case CreatureWeaponType::SWORD:
		GetItemCategoryText(ItemCategorySub::SWORD, OutText);
		break;
	case CreatureWeaponType::DAGGER:
		GetItemCategoryText(ItemCategorySub::DAGGER, OutText);
		break;
	case CreatureWeaponType::BOW:
		GetItemCategoryText(ItemCategorySub::BOW, OutText);
		break;
	case CreatureWeaponType::WAND:
		GetItemCategoryText(ItemCategorySub::WAND, OutText);
		break;
	case CreatureWeaponType::ORB:
		GetItemCategoryText(ItemCategorySub::WAEPON_ORB, OutText);
		break;
 	case CreatureWeaponType::SHOTGUN:
 		GetItemCategoryText(ItemCategorySub::SHOTGUN, OutText);
 		break;
	}
}

void FGsEffectTextFuncText::GetValueRatioText(int32 InValue, bool bIsBasisPoint, OUT FText& OutText)
{
	// TEXT: {0}%
	FText textFormat;
	if (false == FText::FindText(TEXT("UICommonText"), TEXT("NPercent"), textFormat))
	{
		return;
	}

	if (bIsBasisPoint)
	{
		// 만분율에서 퍼센트로 변환
		float percent = InValue * 0.01f;
		OutText = FText::Format(textFormat, percent);

		return;
	}

	OutText = FText::Format(textFormat, InValue);
}

void FGsEffectTextFuncText::GetAbnormalityIdNameText(int32 InAbnormalityId, OUT FText& OutText)
{
	if (const FGsSchemaAbnormalitySet* row =
		UGsTableUtil::FindRowById<UGsTableAbnormalitySet, FGsSchemaAbnormalitySet>(InAbnormalityId))
	{
		OutText = row->ingameName;
	}
}

void FGsEffectTextFuncText::GetPassivityIdNameText(int32 InPassivityId, OUT FText& OutText)
{
	if (const FGsSchemaPassivitySet* row =
		UGsTableUtil::FindRowById<UGsTablePassivitySet, FGsSchemaPassivitySet>(InPassivityId))
	{
		OutText = row->nameText;
	}
}

void FGsEffectTextFuncText::GetStatNameText(StatType InStatType, OUT FText& OutText)
{
	if (StatType::SENSITIVE == InStatType)
	{
		FText::FindText(TEXT("StatText"), TEXT("SENSITIVE"), OutText);
	}
	else
	{
		FGsStatHelper::GetDisPlayStatName(InStatType, OutText); // MAX_HP, HP, ATTACK 만 사용됨
	}
}