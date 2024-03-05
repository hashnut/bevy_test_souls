// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUITalkBubble.h"
#include "SlateApplication.h"
#include "Rendering/SlateRenderer.h"
#include "Fonts/FontMeasure.h"
#include "TextBlock.h"
#include "Image.h"
#include "DataSchema/SocialEmotion/GsSchemaEmoji.h"
#include "CanvasPanel.h"
#include "GameFramework/HUD.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

void UGsUITalkBubble::NativeOnInitialized()
{
	_talkLimitWidth = _talkText->Font.Size * 11;
}

void UGsUITalkBubble::SetText(FText txt) //size fill은 size 설정을 가끔 무시한다. 
{
	auto MinDesiredWidth = _talkText->GetDesiredSize();

	FString str = txt.ToString(); 
	float textWidth = ComputeDesiredSize(FText::FromString(str)) + 40.f;

	if (textWidth > 270.f)
	{
		str = str.Left(10);
		textWidth = ComputeDesiredSize(FText::FromString(str)) + 40.f;
	}

	_talkText->SetMinDesiredWidth(textWidth);

	_talkText->SetText(txt);

	_talkbox->SetVisibility(ESlateVisibility::Visible);
	_emoticon->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUITalkBubble::SetEmoticon(int32 emojiID)
{
	_emoticon->SetVisibility(ESlateVisibility::Visible);
	_talkbox->SetVisibility(ESlateVisibility::Collapsed);

	if (const UGsTableEmoji* dataTable = Cast<UGsTableEmoji>(FGsSchemaEmoji::GetStaticTable()))
	{
		const FGsSchemaEmoji* row = nullptr;
		if (dataTable->FindRowById(emojiID, row)) //<FGsSchemaEmoji>(*FString::FromInt(
		{
			_emoticon->SetBrushFromAtlasInterface(FGsUIHelper::LoadSpriteFromPath(row->emoticon), true);
			//if (UObject* loadObject = row->pathEmoticon.TryLoad())
			//{
			//	_emoticon->SetBrushResourceObject(loadObject);
			//}
		}
	}
}

float UGsUITalkBubble::ComputeDesiredSize(FText text) const
{
	auto MinDesiredWidth = _talkText->GetDesiredSize();
	const FVector2D LocalShadowOffset = _talkText->ShadowOffset;
	const float LocalOutlineSize = _talkText->Font.OutlineSettings.OutlineSize;

	const FVector2D ComputedOutlineSize(LocalOutlineSize * 2, LocalOutlineSize);
	if (FSlateRenderer* renderer = FSlateApplication::Get().GetRenderer())
	{
		const FVector2D TextSize = renderer->GetFontMeasureService()->Measure(text, _talkText->Font) + ComputedOutlineSize + LocalShadowOffset;
		return TextSize.X;
	}
	return MinDesiredWidth.X;
}