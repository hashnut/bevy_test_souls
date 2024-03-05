// Copyright/2019/04/19 - BSAM
// Viewport to show in editor (manage rendering state and related components)

#pragma once

#include "EngineMinimal.h"
#include "SlateFwd.h"
#include "UObject/GCObject.h"
#include "SEditorViewport.h"
#include "SCommonEditorViewportToolbarBase.h"

class FSkillEditor;
class IPersonaPreviewScene;
class SSkillEditorViewportToolBar;

class SSkillEditorViewport : public SEditorViewport, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SSkillEditorViewport) {}
		SLATE_ARGUMENT(TArray<TSharedPtr<FExtender>>, Extenders)
		SLATE_ARGUMENT(TWeakPtr<FSkillEditor>, ParentMyEditor)
		SLATE_ARGUMENT(TWeakPtr<IPersonaPreviewScene>, PreviewScene)
	SLATE_END_ARGS()

private:
	// Parent editor, preview scene, viewport client, and passed in objects.
	TWeakPtr<FSkillEditor> SkillEditorPtr;

	// Perform rendering of the independent space(reflected in actual output if you apply various applications)
	//TWeakPtr<class FAdvancedPreviewScene> PreviewScene; 
	TWeakPtr<class IPersonaPreviewScene> PreviewScene;
	TSharedPtr<class FSkillEditorViewportClient> SkillEditorViewportClient;

	// Vertical box widget for overlay.
	TSharedPtr<SVerticalBox> OverlayTextVerticalBox;

	// A static mesh component for previewing.
	class UStaticMeshComponent* PreviewMeshComponent;

	FText _textFoudList;
	TSharedPtr<STextBlock> _textBlockFoundResult;

protected:
	/** The viewport toolbar */
	TSharedPtr<SSkillEditorViewportToolBar> ViewportToolbar;

	/** Menu extenders */
	TArray<TSharedPtr<FExtender>> Extenders;

public:	
	SSkillEditorViewport();
	~SSkillEditorViewport();

	FSkillEditorViewportClient& GetSkillEditorViewportClient()
	{
		return *SkillEditorViewportClient;
	}

	void PrintText(const FString& InPrintText);

protected:
	// You need to create the viewport client by implementing the following function.
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;
	virtual TSharedPtr<SWidget> MakeViewportToolbar() override;

	// Garbage collection is possible even if it is not a U object.
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;

public:
	void Construct(const FArguments& InArgs);	
};