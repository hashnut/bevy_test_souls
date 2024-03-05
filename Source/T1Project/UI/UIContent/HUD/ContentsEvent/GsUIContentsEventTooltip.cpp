#include "GsUIContentsEventTooltip.h"

#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/RichTextBlock.h"

void UGsUIContentsEventTooltip::SetParameters(const Parameters& InParameters)
{
	_titleTxt->SetText(InParameters._title);
	_descTxt->SetText(InParameters._desc);
	_subDescTxt->SetText(InParameters._subDesc);
}

void UGsUIContentsEventTooltip::Open()
{
	Super::Open();

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}
