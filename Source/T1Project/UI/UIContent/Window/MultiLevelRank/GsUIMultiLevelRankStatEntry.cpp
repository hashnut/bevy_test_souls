

#include "GsUIMultiLevelRankStatEntry.h"

#include "Components/TextBlock.h"

#include "UI/UIContent/Helper/GsUIColorHelper.h"


void UGsUIMultiLevelRankStatEntry::SetData(const FText& InName, const FText& InValue)
{
	_textBlockName->SetText(InName);
	_textBlockValue->SetText(InValue);
}