#include "GsUIMonsterCollectionSlateAllStat.h"
#include "UMG/Public/Components/TextBlock.h"
#include "../UI/UILib/Base/GsUIVFX.h"

void UGsUIMonsterCollectionSlateAllStat::NativeConstruct()
{
	Super::NativeConstruct();
	
	_vfxEffect->HideVFX();
}


void UGsUIMonsterCollectionSlateAllStat::SetData(const FText& InName, const FText& InExpect, const FText& InTotal)
{
	_textName->SetText(InName);
	_textExpect->SetText(InExpect);
	_textTotal->SetText(InTotal);
}

void UGsUIMonsterCollectionSlateAllStat::PlayEffect()
{
	_vfxEffect->ShowVFX();
}