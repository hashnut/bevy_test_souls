

#include "GsUITitleStatItemEntry.h"
#include "UMG/Public/Components/TextBlock.h"
#include "T1Project/T1Project.h"

void UGsUITitleStatItemEntry::SetData(const FText& InStatNmae, const FText& InStatValue)
{
	_statName->SetText(InStatNmae);
	
	_statValue->SetText(InStatValue);
}