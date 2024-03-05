
#include "GsUIMonsterKnowledgeSlateStatListEntry.h"
#include "../Source/Runtime/UMG/Public/Components/TextBlock.h"


void UGsUIMonsterKnowledgeSlateStatListEntry::SetData(const FText& InName, const FText& InValue)
{
	_textName->SetText(InName);
	_textValue->SetText(InValue);
}