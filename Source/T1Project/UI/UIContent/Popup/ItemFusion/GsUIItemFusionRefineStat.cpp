#include "GsUIItemFusionRefineStat.h"
#include "UMG/Public/Components/TextBlock.h"

void UGsUIItemFusionRefineStat::SetData(const FText& InName, const FText& InValue, const FLinearColor& InColor)
{
	_name->SetText(InName);
	_name->SetColorAndOpacity(InColor);

	_value->SetText(InValue);
	_value->SetColorAndOpacity(InColor);

	this->SetVisibility(ESlateVisibility::HitTestInvisible);
}