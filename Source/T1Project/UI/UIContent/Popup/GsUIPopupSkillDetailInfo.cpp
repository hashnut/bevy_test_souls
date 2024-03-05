// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupSkillDetailInfo.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Skill/GsUISkillDetailInfoDefault.h"
#include "UI/UIContent/Window/Skill/GsUISkillDetailInfoPassivity.h"
#include "Skill/GsSkill.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsSkillManager.h"
#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"


void UGsUIPopupSkillDetailInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupSkillDetailInfo::OnClickClose);
}

void UGsUIPopupSkillDetailInfo::OnInputCancel()
{
	Close();
}

void UGsUIPopupSkillDetailInfo::SetData(SkillId InSkillId, int32 InOverrideLevel)
{
	_switcherInfo->SetActiveWidgetIndex(0);

	// TEXT: 스킬 정보
	FText textTitle;
	FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Popup_Title_SkillInfo"), textTitle);
	_textBlockTitle->SetText(textTitle);

	if (const FGsSkill* skill = GSkill()->FindSkill(InSkillId, CreatureWeaponType::MAX, true))
	{
		SetData(skill, InOverrideLevel);
	}
}

void UGsUIPopupSkillDetailInfo::SetData(const FGsSkill* InData, int32 InOverrideLevel)
{
	_switcherInfo->SetActiveWidgetIndex(0);

	// TEXT: 스킬 정보
	FText textTitle;
	FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Popup_Title_SkillInfo"), textTitle);
	_textBlockTitle->SetText(textTitle);

	if (InData)
	{
		_uiDetailInfo->SetData(InData, InOverrideLevel);
	}
}

void UGsUIPopupSkillDetailInfo::SetPassivityData(PassivityId InPassivityId)
{
	_switcherInfo->SetActiveWidgetIndex(1);

	// TEXT: 패시브 정보
	FText textTitle;
	FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Popup_Title_PassiveInfo"), textTitle);
	_textBlockTitle->SetText(textTitle);

	_uiDetailInfoPassivity->SetPassivityData(InPassivityId);
}

void UGsUIPopupSkillDetailInfo::OnClickClose()
{
	OnInputCancel();
}
