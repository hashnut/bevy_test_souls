// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillDetailInfoDefault.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "UI/UIContent/Window/Skill/GsUISkillDetailInfoEntry.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Skill/GsSkillUIData.h"
#include "UTIL/GsTableUtil.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "UI/UIContent/Window/Skill/GsUISkillDetailInfoRichEntry.h"


void UGsUISkillDetailInfoDefault::SetDataAll(FGsSkillUIData* InData)
{
	if (InData)
	{
		SetData(InData->GetSkill(), 0);
	}
	else
	{
		SetData(nullptr, 0);
	}
}

void UGsUISkillDetailInfoDefault::SetData(const FGsSkill* InData, int32 InOverrideLevel)
{
	Super::SetData(InData, InOverrideLevel);

	if (nullptr == InData)
	{
		_panelRoot->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	_panelRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	_scroll->ScrollToStart();
}

void UGsUISkillDetailInfoDefault::SetPassivityData(PassivityId InPassivityId)
{
	const FGsSchemaPassivitySet* table = 
		UGsTableUtil::FindRowById<UGsTablePassivitySet, FGsSchemaPassivitySet>(InPassivityId);
	if (table)
	{
		_textBlockName->SetText(table->nameText);
		_textBlockName->SetColorAndOpacity(
			FGsUIColorHelper::GetColorNameTextPassivity(table->isBuff, PassivityOwnerType::MAX));

		_textBlockDesc->SetText(table->descText);
	}

	// 효과글 찍기
	FString strEffect;
	GSEffectText()->GetEffectTextPassivity(InPassivityId, strEffect);	
	_infoEffect->SetVisibility((false == strEffect.IsEmpty()) ? 
		ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	_infoEffect->SetDesc(FText::FromString(strEffect));

	// 나머지 항목 숨김
	_infoCoolTime->SetVisibility(ESlateVisibility::Collapsed);
	_infoCost->SetVisibility(ESlateVisibility::Collapsed);
	_infoAcquisition->SetVisibility(ESlateVisibility::Collapsed);
}
