// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Tray/Sequence/GsUISequenceTextEffect.h"
#include "UMG/Public/Components/TextBlock.h"


void UGsUISequenceTextEffect::PlayEffect(const FText In_Text1, const FText In_Text2)
{
	bool isText1Empty = In_Text1.IsEmpty();
	if (_text1)
	{
		_text1->SetVisibility((isText1Empty) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

		if (!isText1Empty)
		{
			_text1->SetText(In_Text1);
		}
	}

	bool isText2Empty = In_Text1.IsEmpty();
	if (_text2)
	{
		_text2->SetVisibility((isText2Empty) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);

		if (!isText2Empty)
		{
			_text2->SetText(In_Text2);
		}
	}
}