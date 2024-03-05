#include "GsUIPresetFairy.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

#include "Management/ScopeGame/GsFairyManager.h"

#include "Fairy/GsFairyData.h"

#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Paper2D/Classes/PaperSprite.h"


void UGsUIPresetFairy::SetFairy(FairyId InFairyId)
{
	UGsFairyManager* fairyManager = GSFairy();
	if (nullptr == fairyManager)
	{
		return;
	}

	const TWeakPtr<FGsFairyData> fairyData = fairyManager->GetFairyData(InFairyId);
	if (fairyData.IsValid())
	{
		const FSoftObjectPath path = fairyData.Pin()->GetFairyPortraitPath();
		_imagePortrait->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(path));

		FairyGrade grade = fairyData.Pin()->GetFairyGrade();
		_switcherGrade->SetActiveWidgetIndex(GetGradeIndex(grade));

		const TWeakPtr<FGsFairyItem> fairyDataEquip = fairyManager->GetEquipFairyData();
		_panelEquip->SetVisibility((fairyDataEquip.Pin()->GetFaryID() == InFairyId) ? 
			ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

		FText levelFormat;
		FText::FindText(TEXT("FairyText"), TEXT("FairyUI_LevelFormat"), levelFormat);

		FairyLevelId fairyLevel = fairyData.Pin()->GetCurrentLevel();
		if (INVALID_LEVEL == fairyLevel)
		{
			_textBlockEnchantCount->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			_textBlockEnchantCount->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			_textBlockEnchantCount->SetText(FText::Format(levelFormat, fairyLevel));
		}
	}
}

int32 UGsUIPresetFairy::GetGradeIndex(FairyGrade InGrade)
{
	switch (InGrade)
	{
	case FairyGrade::NONE:		return 0;
	case FairyGrade::NORMAL:	return 0;
	case FairyGrade::MAGIC:		return 1;
	case FairyGrade::RARE:		return 2;
	case FairyGrade::EPIC:		return 3;
	case FairyGrade::UNIQUE:	return 4;
	case FairyGrade::LEGEND:	return 5;
	}
	return 0;
}
