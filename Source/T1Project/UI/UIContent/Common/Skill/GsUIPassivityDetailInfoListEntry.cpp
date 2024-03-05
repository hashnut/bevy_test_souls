// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPassivityDetailInfoListEntry.h"

#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIContent/Window/Skill/GsUISkillDetailInfoEntry.h"
#include "DataSchema/EffectText/GsSchemaEffectTextPassivityEffect.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Skill/GsSkill.h"
#include "UI/UIContent/Window/Skill/GsUISkillDetailInfoRichEntry.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"


void UGsUIPassivityDetailInfoListEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_sBtnOpen->OnClicked.AddDynamic(this, &UGsUIPassivityDetailInfoListEntry::OnClickOpenPanel);
}

void UGsUIPassivityDetailInfoListEntry::NativeConstruct()
{
	Super::NativeConstruct();

	// TEXT: 조건 및 확률
	FText Skill_Condition_Rate;
	FText::FindText(TEXT("FairyText"), TEXT("Skill_Condition_Rate"), Skill_Condition_Rate);
	_infoCause->SetTitle(Skill_Condition_Rate);

	// TEXT: 효과
	FText Skill_InvokeEffect;
	FText::FindText(TEXT("FairyText"), TEXT("Skill_InvokeEffect"), Skill_InvokeEffect);
	_infoEffect->SetTitle(Skill_InvokeEffect);

	// TEXT: 대상
	FText Skill_Target;
	FText::FindText(TEXT("FairyText"), TEXT("Skill_Target"), Skill_Target);
	_infoTarget->SetTitle(Skill_Target);

	// TEXT: 재사용 대기 시간
	FText Skill_Cooltime;
	FText::FindText(TEXT("FairyText"), TEXT("Skill_Cooltime"), Skill_Cooltime);
	_infoCoolTime->SetTitle(Skill_Cooltime);

	SetOpenInfo(false);
}

void UGsUIPassivityDetailInfoListEntry::OnClickOpenPanel()
{
	SetOpenInfo(!_sBtnOpen->GetIsSelected());
}

void UGsUIPassivityDetailInfoListEntry::SetHideEmptyCondition(bool bInHide)
{
	_bHideEmptyCondition = bInHide;
}

void UGsUIPassivityDetailInfoListEntry::SetData(const FGsSchemaEffectTextPassivityEffect& InData)
{
	FGsEffectTextManager* effectTextMgr = GSEffectText();
	if (nullptr == effectTextMgr)
	{
		return;
	}

	// 조건 텍스트 얻어오기
	FString strCondition;
	effectTextMgr->GetStringPassivityCondition(&InData, strCondition);

	// 조건이 비어있고, 가려주기 옵션이 켜있는경우 버튼 가림
	if (strCondition.IsEmpty() &&
		_bHideEmptyCondition)
	{
		_sBtnOpen->SetVisibility(ESlateVisibility::Collapsed);
		SetOpenInfo(true);
	}
	else
	{
		_sBtnOpen->SetVisibility(ESlateVisibility::Visible);

		FText textCondition = FText::FromString(strCondition);
		_textBlockConditionOn->SetText(textCondition);
		_textBlockConditionOff->SetText(textCondition);
	}

	// 조건 및 확률
	FString strCauseAndRate;
	GSEffectText()->GetStringPassivityCauseAndRate(&InData, strCauseAndRate);
	if (strCauseAndRate.IsEmpty())
	{
		_infoCause->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_infoCause->SetVisibility(ESlateVisibility::HitTestInvisible);
		_infoCause->SetDesc(FText::FromString(strCauseAndRate));
	}

	// 발동 시 효과
	FString strEffect;
	effectTextMgr->GetStringPassivityEffect(&InData, strEffect);
	if (strEffect.IsEmpty())
	{
		_infoEffect->SetVisibility(ESlateVisibility::Collapsed);	
	}
	else
	{
		_infoEffect->SetVisibility(ESlateVisibility::HitTestInvisible);
		_infoEffect->SetDesc(FText::FromString(strEffect));
	}

	// 대상
	int32 effectTextOption = 0;
	FGsEffectTextOptionFunc::AddOption(EGsEffectTextOption::PASSIVITY_INVOKE_TARGET_SHORT, effectTextOption);

	FString strTarget;
	effectTextMgr->GetStringPassivityTarget(&InData, strTarget, effectTextOption);
	if (strTarget.IsEmpty())
	{
		_infoTarget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_infoTarget->SetVisibility(ESlateVisibility::HitTestInvisible);
		
		_infoTarget->SetDesc(FText::FromString(strTarget));
	}	

	// 재사용 대기 시간
	FString strCoolTime;
	effectTextMgr->GetStringPassivityCoolTime(&InData, strCoolTime);
	
	if (strCoolTime.IsEmpty())
	{
		_infoCoolTime->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_infoCoolTime->SetVisibility(ESlateVisibility::HitTestInvisible);
		_infoCoolTime->SetDesc(FText::FromString(strCoolTime));
	}
}

void UGsUIPassivityDetailInfoListEntry::SetData(const FString& InStrEffect)
{
	_sBtnOpen->SetVisibility(ESlateVisibility::Collapsed);
	SetOpenInfo(true);

	_infoEffect->SetVisibility(ESlateVisibility::HitTestInvisible);
	_infoEffect->SetDesc(FText::FromString(InStrEffect));

	_infoCause->SetVisibility(ESlateVisibility::Collapsed);
	_infoTarget->SetVisibility(ESlateVisibility::Collapsed);
	_infoCoolTime->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIPassivityDetailInfoListEntry::SetOpenInfo(bool bIsOpen)
{
	_sBtnOpen->SetIsSelected(bIsOpen);
	_panelRootInfo->SetVisibility(bIsOpen ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}
