

#include "GsUIMultiLevelRankQuestEntry.h"

#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"

#include "UI/UIContent/Common/GsUIProgressBar.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "PaperSprite.h"


void UGsUIMultiLevelRankQuestEntry::InitializeData(const FText& InTitle, const FText& InDesc, bool InOnlyTitle, const FSoftObjectPath* InIconPath)
{
	_textName->SetText(InTitle);
	_textDesc->SetText(InDesc);

	_textParm1->SetVisibility(InOnlyTitle ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	_textParm1->SetVisibility(InOnlyTitle ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	_widgetSign->SetVisibility(InOnlyTitle ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);

	_progressBar->SetProgressText(FText::GetEmpty());
	_progressBar->SetPercent(0);

	_switcherCheck->SetActiveWidgetIndex(1);

	if (nullptr == InIconPath)
		return;

	TWeakObjectPtr<UImage> weakThis(_imageType);
	FGsUIHelper::AsyncLoadSpriteFromPath(*InIconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
		{
			if (UImage* strongThis = weakThis.Get())
			{
				strongThis->SetBrushFromAtlasInterface(Cast<UPaperSprite>(inRes));
			}
		}));
}

void UGsUIMultiLevelRankQuestEntry::SetProgressData(int32 InDynamicValue, int32 InObjectiveValue)
{
	_progressBar->SetProgress((float)InDynamicValue/(float)InObjectiveValue);

	_textParm1->SetText(FText::AsNumber(InDynamicValue));
	_textParm2->SetText(FText::AsNumber(InObjectiveValue));

	if (InDynamicValue == InObjectiveValue)
	{
		_switcherCheck->SetActiveWidgetIndex(0);
	}
}

void UGsUIMultiLevelRankQuestEntry::SetColor(const FLinearColor& InColor)
{
	_textName->SetColorAndOpacity(InColor);
	_textParm1->SetColorAndOpacity(InColor);
	_textParm2->SetColorAndOpacity(InColor);
	_widgetSign->SetColorAndOpacity(InColor);
}
