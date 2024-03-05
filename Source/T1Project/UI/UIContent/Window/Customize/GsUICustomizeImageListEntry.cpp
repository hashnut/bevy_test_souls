// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUICustomizeImageListEntry.h"
#include "PaperSprite.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIHelper.h"


void UGsUICustomizeImageListEntry::NativeOnInitialized()
{
	_btnSlot->OnClicked.AddDynamic(this, &UGsUICustomizeImageListEntry::OnClickSlot);

	Super::NativeOnInitialized();
}

void UGsUICustomizeImageListEntry::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	_imgSelected->SetVisibility((bInIsSelected) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

bool UGsUICustomizeImageListEntry::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUICustomizeImageListEntry::SetData(const FSoftObjectPath& InImagePath)
{
	_imgIcon->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(InImagePath));
}

void UGsUICustomizeImageListEntry::OnClickSlot()
{
	// ToggleGroup에 콜백 전달. 켜기 위해 true 전달.
	if (_toggleCallback)
	{
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}
}
