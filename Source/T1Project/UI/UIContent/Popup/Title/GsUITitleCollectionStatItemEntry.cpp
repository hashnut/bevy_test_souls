

#include "GsUITitleCollectionStatItemEntry.h"
#include "UMG/Public/Components/TextBlock.h"
#include "T1Project/T1Project.h"

void UGsUITitleCollectionStatItemEntry::SetData(const FText& InStatNmae, const FText& InStatValue)
{
	_textStatName->SetText(InStatNmae);

	_textStatValue->SetText(InStatValue);
}