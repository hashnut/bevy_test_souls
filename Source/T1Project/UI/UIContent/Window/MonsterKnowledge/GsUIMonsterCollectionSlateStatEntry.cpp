#include "GsUIMonsterCollectionSlateStatEntry.h"
#include "UMG/Public/Components/TextBlock.h"
#include "../UI/UILib/Base/GsUIVFX.h"

void UGsUIMonsterCollectionSlateStatEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_vfxEffect->HideVFX();
}

void UGsUIMonsterCollectionSlateStatEntry::SetData(const FText& InName, const FText& InIncreaseValue, const FText& InTotalValue)
{
	_textName->SetText(InName);
	_textIncreaseValue->SetText(InIncreaseValue);
	_textTotalValue->SetText(InTotalValue);
}

void UGsUIMonsterCollectionSlateStatEntry::PlayEffect()
{
	_vfxEffect->ShowVFX();
}