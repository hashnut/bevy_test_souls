
#include "SkillEditorViewport.h"
#include "SkillEditorViewportClient.h"
#include "SkillEditorViewportToolBar.h"

#include "AdvancedPreviewScene.h"
#include "EditorStyleSet.h"
#include "SBoxPanel.h"

#define LOCTEXT_NAMESPACE "SkillEditorViewport"

SSkillEditorViewport::SSkillEditorViewport()
	//: PreviewScene(MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues())))
{
}

SSkillEditorViewport::~SSkillEditorViewport()
{
}

void SSkillEditorViewport::AddReferencedObjects(FReferenceCollector & Collector)
{
}

TSharedRef<FEditorViewportClient> SSkillEditorViewport::MakeEditorViewportClient()
{
	SkillEditorViewportClient = MakeShareable(new FSkillEditorViewportClient(SkillEditorPtr, PreviewScene.Pin().ToSharedRef(), SharedThis(this)));

	return SkillEditorViewportClient.ToSharedRef();
}

TSharedPtr<SWidget> SSkillEditorViewport::MakeViewportToolbar()
{
	return SAssignNew(ViewportToolbar, SSkillEditorViewportToolBar, SharedThis(this))
		.Cursor(EMouseCursor::Default)
		.Extenders(Extenders);
}

void SSkillEditorViewport::Construct(const FArguments& InArgs)
{
	SkillEditorPtr = InArgs._ParentMyEditor;
	PreviewScene = InArgs._PreviewScene;

	SEditorViewport::Construct(SEditorViewport::FArguments());

	ViewportOverlay->AddSlot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Right)
		.Padding(FMargin(10.0f, 10.0f, 10.0f, 10.0f))
		[
			SAssignNew(OverlayTextVerticalBox, SVerticalBox)
		];
	
	OverlayTextVerticalBox->ClearChildren();
	OverlayTextVerticalBox->AddSlot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("SkillEditorText", "Skill Editor Viewport by BSAM"))
			.TextStyle(FEditorStyle::Get(), TEXT("TextBlock.ShadowedText"))
			.ColorAndOpacity(FLinearColor::Red)
		];
	OverlayTextVerticalBox->AddSlot()
		.AutoHeight()
		[
			SAssignNew(_textBlockFoundResult, STextBlock)
			.Text(_textFoudList)
			.TextStyle(FEditorStyle::Get(), TEXT("TextBlock.ShadowedText"))
			.ColorAndOpacity(FLinearColor::White)
		];	
}

void SSkillEditorViewport::PrintText(const FString& InPrintText)
{
	//FString strResult = TEXT("dddaeaea\ndaseae");
	if (_textBlockFoundResult.IsValid())
	{
		_textFoudList = FText::FromString(InPrintText);
		_textBlockFoundResult->SetText(_textFoudList);
	}
}

#undef LOCTEXT_NAMESPACE