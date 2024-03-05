// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITrayTickerCommon.h"
#include "Components/TextBlock.h"


void UGsUITrayTickerCommon::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_originalTitle = _textTitle->GetText();
	_originalDesc = _textDesc->GetText();
}

void UGsUITrayTickerCommon::SetData(TFunction<void()> InCallback /*= nullptr*/)
{
	SetData(_originalTitle, _originalDesc, InCallback);
}

void UGsUITrayTickerCommon::SetData(const FText& InMainText, const FText& InSubText, TFunction<void()> InCallback /* = nullptr */)
{
	_textTitle->SetText(InMainText);
	_textDesc->SetText(InSubText);

	UGsUITrayTickerAnimation::SetData(InCallback);
}