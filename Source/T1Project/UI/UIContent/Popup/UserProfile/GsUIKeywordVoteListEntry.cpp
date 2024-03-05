// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIKeywordVoteListEntry.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Common/GsUIRadioButton.h"

#include "PaperSprite.h"

void UGsUIKeywordVoteListEntry::SetSwitcherIndex(int32 InIndex)
{
	_switcherIndex = InIndex;
	_switcherKeyword->SetActiveWidgetIndex(InIndex);
}

void UGsUIKeywordVoteListEntry::SetData(bool InIsMine, const FSoftObjectPath& InIconPath, const FText& InKeywordName, uint64 InKeywordCount)
{
	_radioButton->SetVisibility(InIsMine ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

	if (!_isLoaded || _switcherIndex == 0)
	{
		SetIcon(InIconPath, _imageIconMain);
		SetName(InKeywordName, _textBlockNameMain);
		SetCount(InKeywordCount, _textBlockCountMain);
	}

	if (!_isLoaded || _switcherIndex == 1)
	{
		SetIcon(InIconPath, _imageIconNormal);
		SetName(InKeywordName, _textBlockNameNormal);
		SetCount(InKeywordCount, _textBlockCountNormal);
	}
	_isLoaded = true;
}

void UGsUIKeywordVoteListEntry::SetIsSelected(bool InIsSelected)
{
	_radioButton->SetIsSelected(InIsSelected);
}

void UGsUIKeywordVoteListEntry::SetToggleGroup(FGsToggleGroup* InToggleGroup)
{
	_toggleGroup = InToggleGroup;
}

void UGsUIKeywordVoteListEntry::SetIcon(const FSoftObjectPath& InIconPath, UImage* InImage)
{
	TWeakObjectPtr<UImage> weakThis(InImage);
	FGsUIHelper::AsyncLoadSpriteFromPath(InIconPath, FStreamableDelegateParam::CreateWeakLambda(this, [weakThis](UObject* inRes)
		{
			if (UImage* strongThis = weakThis.Get())
			{
				strongThis->SetBrushFromAtlasInterface(Cast<UPaperSprite>(inRes));
			}
		}));
}

void UGsUIKeywordVoteListEntry::SetName(const FText& InKeywordName, UTextBlock* InTextBlock)
{
	InTextBlock->SetText(InKeywordName);
}

void UGsUIKeywordVoteListEntry::SetCount(uint64 InKeywordCount, UTextBlock* InTextBlock)
{
	InTextBlock->SetText(FText::AsNumber(InKeywordCount));
}

FReply UGsUIKeywordVoteListEntry::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	if (_toggleGroup)
	{
		IGsToggleGroupEntry* entry = _radioButton->GetToggleGroupEntry();
		_toggleGroup->SetSelectedIndex(entry->GetIndexInGroup());
	}

	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

IGsToggleGroupEntry* UGsUIKeywordVoteListEntry::GetToggleGroupEntry()
{
	return _radioButton->GetToggleGroupEntry();
}
