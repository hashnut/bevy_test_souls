#include "GsUIMonsterCollectionSlateProgressEntry.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "MonsterKnowledgeCollection/EMonsterKnowledgeSlateCategory.h"
#include "UMG/Public/Components/ProgressBar.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/TextBlock.h"


void UGsUIMonsterCollectionSlateProgressEntry::SetData(EMonsterKnowledgeSlateCategory InCategory, float InPercent)
{
	_progressBar->SetPercent(InPercent);

	const TPair<EMonsterKnowledgeSlateCategory, FText>* categoryData = GsMonsterCollection()->SlateCategoryData(StaticCast<int32>(InCategory));
	if (categoryData)
	{
		_textCategoryName->SetText(categoryData->Value);
	}

	int32 value = FMath::Clamp<float>(InPercent, 0.f, 1.f) * 10000.f;
	FText textPercent = FText::FromString(FString::Printf(TEXT("%0.2f%%"), (float)value * 0.01f));
	_textPercent->SetText(textPercent);

	_switcherImage->SetActiveWidgetIndex(StaticCast<int32>(InCategory));
}