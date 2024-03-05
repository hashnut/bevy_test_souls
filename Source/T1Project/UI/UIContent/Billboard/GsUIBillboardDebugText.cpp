// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIContent/Billboard/GsUIBillboardDebugText.h"

#include "Management/ScopeGame/GsGameObjectManager.h"
#include "GameObject/Define/GsGameObjectStruct.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/ScaleBox.h"
#include "Runtime/UMG/Public/Components/VerticalBox.h"
#include "Runtime/UMG/Public/Components/VerticalBoxSlot.h"
#include "Runtime/UMG/Public/Components/CanvasPanel.h"
#include "Runtime/UMG/Public/Components/Spacer.h"
#include "Blueprint/WidgetTree.h"
#include "Image.h"

static constexpr TCHAR _fontPath[] = TEXT("/Game/UI/Font/Chat");
static constexpr uint16 _fontSize = 14;

#if	!UE_BUILD_SHIPPING
void UGsUIBillboardDebugText::InitializeDebugTextInfo(TArray<FString>&& InDebugTextArray)
{
	if (InDebugTextArray.Num() <= 0)
	{
		GSLOG(Error, TEXT("[DebugText] Length of Log is Empty!"));
		return;
	}
		
	_debugInfoList.Empty();
	_textBlockList.Empty();
	_verticalBox->ClearChildren();

	USpacer* spacer = WidgetTree->ConstructWidget<USpacer>(USpacer::StaticClass());
	spacer->SetSize(FVector2D(1.0f, 5.f));

	_verticalBox->AddChild(spacer);

	// 폰트 로드
	if (nullptr == _fontFamily)
	{
		_fontFamily = LoadObject<UObject>(this, _fontPath);
	}

	for (const auto& str : InDebugTextArray)
	{
		_debugInfoList.Add(str);

		// TextBlock 만들어서 저장해놓기
		UTextBlock* textBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		textBlock->SetText(FText::FromString(str));
		textBlock->SetJustification(ETextJustify::Center);
		textBlock->SetFont(FSlateFontInfo(_fontFamily, _fontSize));
		_textBlockList.Emplace(textBlock);
		_verticalBox->AddChild(textBlock);
	}

	// VerticalBox 아래 슬롯들 채우기 레이아웃 설정
	for (auto& slot : _verticalBox->GetSlots())
	{
		UVerticalBoxSlot* vSlot = Cast<UVerticalBoxSlot>(slot);
		vSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		vSlot->SetPadding(FMargin(7.5f, 0.05f, 0.05f, 7.5f));
	}

	_isInitialized = true;
}

void UGsUIBillboardDebugText::SetDebugTextInfo(TArray<FString>&& InDebugTextArray)
{
	// 1번째 방법 : 텍스트 분리해서 위젯 만들기 -> 폐기...
	// 2번째 방법 : 그냥 한 텍스트에 때려박기

	// 3번째 방법 : 각 UTextBlock 을 동적으로 생성해서 색깔 변경
	if (!_isInitialized || InDebugTextArray.Num() != _debugInfoList.Num())
	{
		InitializeDebugTextInfo(MoveTemp(InDebugTextArray));
		return;
	}

	for (int i = 0; i < InDebugTextArray.Num(); ++i)
	{
		FString result(InDebugTextArray[i]);

		if (result != _debugInfoList[i])
		{
			_debugInfoList[i] = result;

			_textBlockList[i]->SetColorAndOpacity(RED);
			_textBlockList[i]->SetText(FText::FromString(result));
		}
		else
		{
			_textBlockList[i]->SetColorAndOpacity(WHITE);
		}
	}
}
#endif