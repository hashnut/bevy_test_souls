// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISkillDetailInfoPassivity.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "UI/UIContent/Common/Skill/GsUIPassivityDetailInfo.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "Skill/GsSkill.h"
#include "Skill/GsSkillUIData.h"
#include "Skill/GsSkillHelper.h"
#include "UTIL/GsTableUtil.h"
#include "T1Project.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Management/ScopeGame/GsSupplyManager.h"
#include "Management/GsScopeHolder.h"
#include "Management/GsScopeGame.h"


void UGsUISkillDetailInfoPassivity::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnLearn->OnClicked.AddDynamic(this, &UGsUISkillDetailInfoPassivity::OnClickLearnButtton);

	_checkerButton.SetIntervalSecond(0.5f);
}

void UGsUISkillDetailInfoPassivity::SetDataAll(FGsSkillUIData* InData)
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

static FText MakeSkillSupplyText(const FGsSkill* InData)
{
	if (nullptr == InData)
	{
		GSLOG(Error, TEXT("nullptr == InData"));
	}
	else
	{
		const SkillId CurrentSkillId = InData->GetUserSkillSetId();
		if (INVALID_SKILL_ID == CurrentSkillId)
		{
			GSLOG(Error, TEXT("INVALID_SKILL_ID == CurrentSkillId"));
		}
		else
		{
			const UGsSupplyManager* SupplyManager = GSSupply();
			if (nullptr != SupplyManager)
			{
				return SupplyManager->MakeRichTextSkillId(CurrentSkillId);
			}
		}
	}

	return FText::GetEmpty();
}

void UGsUISkillDetailInfoPassivity::SetData(const FGsSkill* InData, int32 InOverrideLevel)
{
	if (nullptr == InData ||
		nullptr == InData->_tableData ||
		nullptr == InData->_userSkillSetData)
	{
		_panelRoot->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	_panelRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 스킬 정보로 들어올 경우, 스킬이름으로 출력함
	_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InData->_tableData->grade));
	_textBlockName->SetText(InData->_userSkillSetData->nameText);

	if (0 == InData->_tableData->passivityIdList.Num())
	{
		_detailInfo->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		// 무조건 0번째로 찍는다
		if (const FGsSchemaPassivitySet* passivity = InData->_tableData->passivityIdList[0].GetRow())
		{
			_detailInfo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_detailInfo->SetPassivityData(passivity, MakeSkillSupplyText(InData));
		}
		else
		{
			_detailInfo->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	if (InData->_isLearn)
	{
		_btnLearn->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		_btnLearn->SetVisibility(ESlateVisibility::Visible);

		bool bCanLearn = FGsSkillHelper::CanLearn(InData);
		_btnLearn->SetIsEnabled(bCanLearn);
	}	
}

void UGsUISkillDetailInfoPassivity::SetPassivityData(PassivityId InPassivityId)
{
	const FGsSchemaPassivitySet* tableRow =
		UGsTableUtil::FindRowById<UGsTablePassivitySet, FGsSchemaPassivitySet>(InPassivityId);
	if (nullptr == tableRow)
	{
		_panelRoot->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	_panelRoot->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 패시비티 정보로 들어올 경우 패시비티 이름으로 출력함
	// 스킬 정보로 들어올 경우, 스킬이름으로 출력함
	_textBlockName->SetColorAndOpacity(FLinearColor::White);
	_textBlockName->SetText(tableRow->nameText);

	_detailInfo->SetPassivityData(tableRow);

	_btnLearn->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUISkillDetailInfoPassivity::OnClickLearnButtton()
{
	// 광클방지
	if (_checkerButton.IsTimeOver())
	{
		OnClickLearn.ExecuteIfBound(false);

		_checkerButton.UpdateTime();
	}
}
