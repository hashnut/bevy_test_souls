// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITooltipTargetInfo.h"
#include "PaperSprite.h"
#include "Components/PanelWidget.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/GsSchemaCreatureRaceData.h"
#include "DataSchema/item/GsSchemaItemMaterialData.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UTIL/GsGameObjectUtil.h"
#include "T1Project.h"


void UGsUITooltipTargetInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// [M2]에서 속성UI는 보여주지 않음
	_panelRootAttribute->SetVisibility(ESlateVisibility::Collapsed);
	_panelRootPassive->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUITooltipTargetInfo::SetData(const FGsSchemaNpcData* InData)
{
	SetUIRace(InData->race);
	SetUIWeaknessMaterial(InData->race);
	SetUIElement(InData->element);
}

void UGsUITooltipTargetInfo::SetUIRace(CreatureRaceType InType)
{
	_panelRootRace->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_textRace = UGsGameObjectUtil::GetRaceText(InType);
}

void UGsUITooltipTargetInfo::SetUIWeaknessMaterial(CreatureRaceType InType)
{
	_panelRootMaterial->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_textMaterial = UGsGameObjectUtil::GetRaceWeaknessItemMaterialText(InType);
}

// 속성
void UGsUITooltipTargetInfo::SetUIElement(MonsterElement InType)
{
	_panelRootAttribute->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_textAttribute = UGsGameObjectUtil::GetMonsterElementalText(InType);
}