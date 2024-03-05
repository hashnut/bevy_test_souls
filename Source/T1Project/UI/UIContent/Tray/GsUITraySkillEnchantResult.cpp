// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITraySkillEnchantResult.h"
#include "PaperSprite.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Common/Skill/GsUISkillIcon.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Skill/GsSkill.h"
#include "Management/ScopeGlobal/GsSoundManager.h"
#include "Sound/GsSoundPlayer.h"


void UGsUITraySkillEnchantResult::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnConfirm->OnClicked.AddDynamic(this, &UGsUITraySkillEnchantResult::OnClickConfirm);
	_checkerCloseLock.SetIntervalSecond(_closeLockTime);
}

void UGsUITraySkillEnchantResult::SetData(const FGsSkill* InSkill, bool bIsUpgrade, bool bInIsSuccess)
{
	if (nullptr == InSkill)
	{
		Close();
		return;
	}

	FText textTitle;
	if (bInIsSuccess)
	{
		if (bIsUpgrade)
		{
			// TEXT: 업그레이드!
			FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Upgrade_Success"), textTitle);
		}
		else
		{
			// TEXT: 강화 성공!
			FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Enchant_Success"), textTitle);
		}
	}
	else
	{
		// TEXT: 강화 실패
		FText::FindText(TEXT("SkillUIText"), TEXT("SkillUI_Enchant_Fail"), textTitle);
	}

	_textBlockTitle->SetText(textTitle);

	_uiIcon->SetData(InSkill);

	FString strName = FString::Format(TEXT("Lv.{0} {1}"), 
		{ InSkill->GetLevel(), *InSkill->_userSkillSetData->nameText.ToString() });

	_textBlockName->SetText(FText::FromString(strName));
	_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(InSkill->_tableData->grade));

	// 닫기 버튼에 락걸기
	_checkerCloseLock.UpdateTime();
}

void UGsUITraySkillEnchantResult::OnClickConfirm()
{
	if (_checkerCloseLock.IsTimeOver())
	{
		Close();
	}
}
