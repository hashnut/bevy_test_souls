#include "GsUIMonsterCollectionBookDetailRewardEntry.h"
#include "UMG/Public/Components/TextBlock.h"

void UGsUIMonsterCollectionBookDetailRewardEntry::SetData(const int32 InLevel, const int32 InKnowledge, const int32 InPoint)
{
	_textKnowledgeLevel->SetText(FText::AsNumber(InLevel));
	_textRequredKnowledge->SetText(FText::AsNumber(InKnowledge));
	_textRewardPoint->SetText(FText::AsNumber(InPoint));
}