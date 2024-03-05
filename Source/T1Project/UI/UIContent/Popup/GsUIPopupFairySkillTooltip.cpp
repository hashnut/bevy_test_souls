// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Popup/GsUIPopupFairySkillTooltip.h"
#include "TextBlock.h"
#include "ScrollBox.h"
#include "PanelWidget.h"
#include "Fairy/GsFairyData.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UI/UIContent/Common/Skill/GsUIPassivityDetailInfo.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "WidgetSwitcher.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/Fairy/GsUISummonPortraitBase.h"

#include "Engine/AssetManager.h"

void UGsUIPopupFairySkillTooltip::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnPrev->OnClicked.AddDynamic(this, &UGsUIPopupFairySkillTooltip::OnClickPrevLevel);
	_btnNext->OnClicked.AddDynamic(this, &UGsUIPopupFairySkillTooltip::OnClickNextLevel);
}

void UGsUIPopupFairySkillTooltip::InitializeData(PopupInitData* initParam /*= nullptr*/)
{
	if (initParam)
	{
		_skillPassivityList.Reset();
		_skillPassivityList.Append(initParam->skillPassivityList);
		SetFairyInfo(initParam->data);

		if (auto data = initParam->data.Pin())
		{
			_maxLevel = 0;
			_maxLevel = 0;
			if (_skillPassivityList.IsValidIndex(0))
			{
				_maxLevel = _skillPassivityList.Last().Key;
				_minLevel = _skillPassivityList[0].Key;
			}

			if (initParam->viewLevel == INVALID_FAIRY_LEVEL)
			{
				_btnNext->SetIsEnabled(false);
				_btnPrev->SetIsEnabled(false);

				_btnNext->SetVisibility(ESlateVisibility::Hidden);
				_btnPrev->SetVisibility(ESlateVisibility::Hidden);

				SetData(data->GetCurrentLevel(), initParam->isNoneLevelSkill);
			}
			else
			{
				_btnNext->SetVisibility(ESlateVisibility::Visible);
				_btnPrev->SetVisibility(ESlateVisibility::Visible);

				_btnNext->SetIsEnabled(true);
				_btnPrev->SetIsEnabled(true);

				if (initParam->viewLevel == INVALID_FAIRY_LEVEL_ID)
					 SetData(data->GetCurrentLevel());
				else SetData(initParam->viewLevel);
			}
		}
	}
}

void UGsUIPopupFairySkillTooltip::SetFairyInfo(TWeakPtr<FGsFairyData> data)
{
	if (auto fairyData = data.Pin())
	{
		FLinearColor color = FGsUIColorHelper::GetColorFairyGrade(fairyData->GetFairyGrade());

		_fairyName->SetText(fairyData->GetFairyName());
		_fairyName->SetColorAndOpacity(color);

		_portrait->SetData(data);
		_portrait->RefreshUI();
	}
}

void UGsUIPopupFairySkillTooltip::SetData(FairyLevelId level, bool noneLevelSkill /*=false*/)
{
	_btnNext->SetIsEnabled(level >= _maxLevel ? false : true);
	_btnPrev->SetIsEnabled(level <= _minLevel ? false : true);

	_curLevel = level;
	if(auto iter = _skillPassivityList.FindByPredicate([level](auto& e) { return e.Key == level; }))
	{ 
		if (auto skillPassivity = iter->Value)
		{
			FText levelFormat;
			FText::FindText(TEXT("FairyText"), TEXT("FairyUI_LevelFormat"), levelFormat);

			_textSkillName = skillPassivity->nameText;
			_textSkillLevel = noneLevelSkill ? FText::GetEmpty() : FText::Format(levelFormat, iter->Key);
			_uiDetailInfo->SetPassivityData(skillPassivity);
		}
	}
}

void UGsUIPopupFairySkillTooltip::OnClickClose()
{
	Close();
}

void UGsUIPopupFairySkillTooltip::OnClickNextLevel()
{
	SetData(_curLevel + 1);
}

void UGsUIPopupFairySkillTooltip::OnClickPrevLevel()
{
	SetData(_curLevel - 1);
}

void UGsUIPopupFairySkillTooltip::OnInputCancel()
{
	OnClickClose();
}