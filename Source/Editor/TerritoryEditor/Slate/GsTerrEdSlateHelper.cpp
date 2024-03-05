// Fill out your copyright notice in the Description page of Project Settings.

#include "GsTerrEdSlateHelper.h"

TSharedPtr<SWidget> FGsTerrEdSlateHelper::CreateNumericWidget(FString inDescription, TFunction<float(void)> inReturnVlaue, TFunction<void(float, ETextCommit::Type)> inSetVlaue, float inFieldColumSize, float inValueColumSize)
{
	FMargin padding;

	return SNew(SSplitter);

	//return SNew(SSplitter)
	//	.Style(FEditorStyle::Get(), "DetailsView.Splitter")
	//	.PhysicalSplitterHandleSize(1.0f)
	//	.HitDetectionSplitterHandleSize(5.0f)
	//	+ SSplitter::Slot()
	//	.Value(inFieldColumSize)
	//	[
	//		SNew(SHorizontalBox)
	//		+ SHorizontalBox::Slot()
	//	.HAlign(HAlign_Left)
	//	.VAlign(VAlign_Center)
	//	.AutoWidth()
	//	.Padding(padding)
	//	[
	//		SNew(STextBlock).Text(FText::FromString(inDescription))
	//	]

	//	]
	//+ SSplitter::Slot()
	//	.Value(inValueColumSize)
	//	[
	//		SNew(SHorizontalBox)
	//		+ SHorizontalBox::Slot()
	//	.HAlign(HAlign_Center)
	//	.VAlign(VAlign_Center)
	//	.AutoWidth()
	//	.MaxWidth(400)
	//	[
	//		SNew(SSpinBox<float>)
	//		.Value_Lambda(inReturnVlaue)
	//		.OnValueCommitted_Lambda(inSetVlaue)
	//		.MinValue(0.001f)
	//		.MaxValue(100.0f)
	//		//.Style(&FGameplayInsightsStyle::Get().GetWidgetStyle<FSpinBoxStyle>("TransportControls.HyperlinkSpinBox"))
	//		.ClearKeyboardFocusOnCommit(true)
	//		.Delta(0.01f)
	//		.LinearDeltaSensitivity(25)
	//		.TypeInterface(MakeShared<TNumericUnitTypeInterface<T>>(EUnit::Multiplier))
	//	]
	//	];
}
