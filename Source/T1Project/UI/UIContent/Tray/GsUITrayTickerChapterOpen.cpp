// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTickerChapterOpen.h"

void UGsUITrayTickerChapterOpen::SetData(int32 inIndex, const FText& InText)
{
	_chapterIndexText = FText::AsNumber(inIndex + 1);
	_chapterTitleText = InText;
}

void UGsUITrayTickerChapterOpen::OnExitAnimation()
{
	if (_delegateOnFinished.IsBound())
	{
		_delegateOnFinished.ExecuteIfBound();
	}

	Close();
}