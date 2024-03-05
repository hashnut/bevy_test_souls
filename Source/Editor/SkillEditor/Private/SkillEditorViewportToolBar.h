// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "Styling/SlateColor.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "SEditorViewport.h"
#include "SkillEditorViewport.h"
#include "Editor/UnrealEd/Public/SViewportToolBar.h"

class FMenuBuilder;
class SComboButton;

/**
 * A level viewport toolbar widget that is placed in a viewport
 */
class SSkillEditorViewportToolBar : public SViewportToolBar
{
public:
	SLATE_BEGIN_ARGS(SSkillEditorViewportToolBar)
	{}

	SLATE_ARGUMENT(TArray<TSharedPtr<FExtender>>, Extenders)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<class SSkillEditorViewport> InRealViewport);

private:
	/**
	 * Generates the toolbar view menu content 
	 */
	TSharedRef<SWidget> GenerateViewMenu() const;

	/** Called by the FOV slider in the perspective viewport to get the FOV value */
	TOptional<float> OnGetFOVValue() const;
	/** Called when the FOV slider is adjusted in the perspective viewport */
	void OnFOVValueChanged( float NewValue );
	/** Called when a value is entered into the FOV slider/box in the perspective viewport */
	void OnFOVValueCommitted( float NewValue, ETextCommit::Type CommitInfo );

private:
	/** Extend the view menu */
	TSharedRef<FExtender> GetViewMenuExtender(TSharedPtr<class SEditorViewport> InRealViewport);

	/** Build the FOV widget */
	TSharedRef<SWidget> MakeFOVWidget() const;

private:
	TWeakPtr<class SSkillEditorViewport> SkillEditorViewport;

	/** Extenders */
	TArray<TSharedPtr<FExtender>> Extenders;
};