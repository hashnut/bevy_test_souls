// Copyright Epic Games, Inc. All Rights Reserved.


#include "SkillEditorViewportToolBar.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "ToolMenus.h"
#include "EngineGlobals.h"
#include "AssetData.h"
#include "Engine/Engine.h"
#include "EditorStyleSet.h"
#include "PropertyEditorModule.h"
#include "IDetailsView.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Editor/EditorPerProjectUserSettings.h"
#include "Preferences/PersonaOptions.h"
#include "EditorViewportCommands.h"
#include "SEditorViewportToolBarMenu.h"
#include "STransformViewportToolbar.h"
#include "SEditorViewportViewMenu.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/Colors/SColorPicker.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/STextComboBox.h"
#include "AssetViewerSettings.h"
#include "Engine/PreviewMeshCollection.h"
#include "Widgets/SWidget.h"
#include "Types/ISlateMetaData.h"
#include "Textures/SlateIcon.h"
#include "ShowFlagMenuCommands.h"
#include "BufferVisualizationMenuCommands.h"
#include "BoneSelectionWidget.h"
#include "Widgets/Text/SRichTextBlock.h"

#include "SkillEditorCommands.h"
#include "SkillEditorViewportClient.h"

#define LOCTEXT_NAMESPACE "SkillEditorViewportToolBar"


///////////////////////////////////////////////////////////
// SSkillEditorViewportToolBar

TSharedRef<SWidget> SSkillEditorViewportToolBar::GenerateViewMenu() const
{
	const FSkillEditorCommands& Actions = FSkillEditorCommands::Get();

	TSharedPtr<FExtender> MenuExtender = FExtender::Combine(Extenders);

	const bool bInShouldCloseWindowAfterMenuSelection = true;
	TSharedPtr<FUICommandList> CommandListEmpty;
	CommandListEmpty = MakeShareable(new FUICommandList());
	FMenuBuilder InMenuBuilder(bInShouldCloseWindowAfterMenuSelection, CommandListEmpty, MenuExtender);

	InMenuBuilder.PushCommandList(CommandListEmpty.ToSharedRef());
	InMenuBuilder.PushExtender(MenuExtender.ToSharedRef());

	InMenuBuilder.BeginSection("AnimViewportCamera", LOCTEXT("ViewMenu_CameraLabel", "Camera"));
	{
		InMenuBuilder.AddMenuEntry(FEditorViewportCommands::Get().FocusViewportToSelection);

		InMenuBuilder.AddWidget(MakeFOVWidget(), LOCTEXT("Viewport_FOVLabel", "Field Of View"));
	}
	InMenuBuilder.EndSection();

	return InMenuBuilder.MakeWidget();
}

TSharedRef<FExtender> SSkillEditorViewportToolBar::GetViewMenuExtender(TSharedPtr<class SEditorViewport> InRealViewport)
{
	TSharedRef<FExtender> Extender(new FExtender());
	Extender->AddMenuExtension(
		TEXT("ViewMode"),
		EExtensionHook::After,
		InRealViewport->GetCommandList(),
		FMenuExtensionDelegate::CreateLambda([](FMenuBuilder& InMenuBuilder)
			{
			}));

	return Extender;
}

TSharedRef<SWidget> SSkillEditorViewportToolBar::MakeFOVWidget() const
{
	const float FOVMin = 5.f;
	const float FOVMax = 170.f;

	return
		SNew(SBox)
		.HAlign(HAlign_Right)
		[
			SNew(SBox)
			.Padding(FMargin(4.0f, 0.0f, 0.0f, 0.0f))
			.WidthOverride(100.0f)
			[
				SNew(SNumericEntryBox<float>)
				.Font(FEditorStyle::GetFontStyle(TEXT("MenuItem.Font")))
				.AllowSpin(true)
				.MinValue(FOVMin)
				.MaxValue(FOVMax)
				.MinSliderValue(FOVMin)
				.MaxSliderValue(FOVMax)
				.Value(this, &SSkillEditorViewportToolBar::OnGetFOVValue)
				.OnValueChanged(const_cast<SSkillEditorViewportToolBar*>(this), &SSkillEditorViewportToolBar::OnFOVValueChanged)
				.OnValueCommitted(const_cast<SSkillEditorViewportToolBar*>(this), &SSkillEditorViewportToolBar::OnFOVValueCommitted)
			]
		];
}

TOptional<float> SSkillEditorViewportToolBar::OnGetFOVValue() const
{
	if (SkillEditorViewport.IsValid())
	{
		return SkillEditorViewport.Pin()->GetViewportClient()->ViewFOV;

	}
	return 0.0f;
}

void SSkillEditorViewportToolBar::OnFOVValueChanged(float NewValue)
{
	SkillEditorViewport.Pin()->GetViewportClient()->FOVAngle = NewValue;
	SkillEditorViewport.Pin()->GetViewportClient()->ViewFOV = NewValue;
}

void SSkillEditorViewportToolBar::OnFOVValueCommitted(float NewValue, ETextCommit::Type CommitInfo)
{
	//OnFOVValueChanged will be called... nothing needed here.
}

void SSkillEditorViewportToolBar::Construct(const FArguments& InArgs, TSharedPtr<class SSkillEditorViewport> InRealViewport)
{
	Extenders = InArgs._Extenders;
	Extenders.Add(GetViewMenuExtender(InRealViewport));	

	// If we have no extender, make an empty one
	if (Extenders.Num() == 0)
	{
		Extenders.Add(MakeShared<FExtender>());
	}

	const FMargin ToolbarSlotPadding(2.0f, 2.0f);
	const FMargin ToolbarButtonPadding(2.0f, 0.0f);

	static const FName DefaultForegroundName("DefaultForeground");

	TSharedRef<SHorizontalBox> LeftToolbar = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(ToolbarSlotPadding)
		[
			SNew(SEditorViewportToolbarMenu)
			.ToolTipText(LOCTEXT("ViewMenuTooltip", "View Options.\nShift-clicking items will 'pin' them to the toolbar."))
			.ParentToolBar(SharedThis(this))
			.Cursor(EMouseCursor::Default)
			.Image("EditorViewportToolBar.MenuDropdown")
			.AddMetaData<FTagMetaData>(FTagMetaData(TEXT("EditorViewportToolBar.MenuDropdown")))
			.OnGetMenuContent(this, &SSkillEditorViewportToolBar::GenerateViewMenu)
		];

	ChildSlot
	[
		SNew( SBorder )
		.BorderImage( FEditorStyle::GetBrush("NoBorder") )
		// Color and opacity is changed based on whether or not the mouse cursor is hovering over the toolbar area
		.ColorAndOpacity( this, &SViewportToolBar::OnGetColorAndOpacity )
		.ForegroundColor( FEditorStyle::GetSlateColor(DefaultForegroundName) )
		[
			SNew( SVerticalBox )
			+ SVerticalBox::Slot()
			.AutoHeight()
			[
				LeftToolbar
			]
			+SVerticalBox::Slot()
			.Padding(FMargin(4.0f, 3.0f, 0.0f, 0.0f))
			[
				// Display text (e.g., item being previewed)
				SNew(SRichTextBlock)
				.DecoratorStyleSet(&FEditorStyle::Get())
				.TextStyle(&FEditorStyle::Get().GetWidgetStyle<FTextBlockStyle>("AnimViewport.MessageText"))
			]
		]
	];
	
	SViewportToolBar::Construct(SViewportToolBar::FArguments());

	SkillEditorViewport = InRealViewport;

	//FSkillEditorViewportClient& SkillEditorViewportClient = SkillEditorViewport.Pin()->GetSkillEditorViewportClient();
}

#undef LOCTEXT_NAMESPACE
