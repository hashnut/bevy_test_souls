// Copyright/2019/04/19 - BSAM
// Viewport client to show in editor (rendering settings and data processing to show in Viewport)

#pragma once

#include "CoreMinimal.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"
#include "EditorViewportClient.h"
#include "DrawTools.h"

class FSkillEditorViewportClient : public FEditorViewportClient, public TSharedFromThis<FSkillEditorViewportClient>
{
private:
	TWeakPtr<class FSkillEditor> SkillEditorPtr;
	TWeakPtr<class SSkillEditorViewport> SkillEditorViewportPtr;
	//class FAdvancedPreviewScene* AdvancedPreviewScene;	
	class IPersonaPreviewScene* PersonaPreviewScene;

public:
	//float CurrentTime;
	static DrawTools::FDrawManager DrawManager;
	static DrawTools::FDrawManager NotifyDrawManager;
	//UDebugSkelMeshComponent* DebugSkelMeshComponent;

public:
	// Set all necessary default variables in the constructor.
	FSkillEditorViewportClient(TWeakPtr<class FSkillEditor> ParentSkillEditor, const TSharedRef<class IPersonaPreviewScene>& AdvPreviewScene, const TSharedRef<class SSkillEditorViewport>& Viewport);
	~FSkillEditorViewportClient();

	// Inputs used in the editor
	virtual void Tick(float DeltaSeconds) override;
	virtual void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
};