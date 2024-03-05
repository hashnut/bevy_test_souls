
#include "GSUIMonsterCollectionBasePage.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeData.h"

void UGSUIMonsterCollectionBasePage::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGSUIMonsterCollectionBasePage::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UGSUIMonsterCollectionBasePage::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGSUIMonsterCollectionBasePage::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGSUIMonsterCollectionBasePage::Enter(struct FGsMonsterKnowledgeUICaptureData* InData)
{
	
}

void UGSUIMonsterCollectionBasePage::InvalidateBook(uint32 InData)
{
	InvalidateRedDot();
}

void UGSUIMonsterCollectionBasePage::InvalidateBookSet(const TArray<FGsMonsterCollectionBookResult>* InData)
{
	InvalidateRedDot();
}

void UGSUIMonsterCollectionBasePage::InvalidateCodex()
{
	InvalidateRedDot();
}

void UGSUIMonsterCollectionBasePage::InvalidateKnowledgeGain()
{
	InvalidateRedDot();
}

void UGSUIMonsterCollectionBasePage::InvalidateMapGroupText()
{
	InvalidateRedDot();
}

void UGSUIMonsterCollectionBasePage::InvalidateCategoryExplore()
{
	InvalidateRedDot();
}

void UGSUIMonsterCollectionBasePage::InvalidateMapGroupList(EGsMapGroupRefreshType InScrollType /*= EGsMapGroupRefreshType::NO_SCROLL*/)
{
	InvalidateRedDot();
}

void UGSUIMonsterCollectionBasePage::InvalidateSlate(MonsterKnowledgeSlateId InId)
{
	InvalidateRedDot();
}

void UGSUIMonsterCollectionBasePage::InvalidateSlateCurrency()
{
	InvalidateRedDot();
}