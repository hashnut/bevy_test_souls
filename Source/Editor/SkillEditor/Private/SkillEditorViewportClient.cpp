
#include "SkillEditorViewportClient.h"
#include "SkillEditorViewport.h"
#include "SkillEditor.h"
#include "AdvancedPreviewScene.h"

#include "EditorStyleSet.h"
#include "Editor/EditorPerProjectUserSettings.h"
#include "AssetViewerSettings.h"
#include "Editor/UnrealEd/Classes/Animation/DebugSkelMeshComponent.h"
#include "Runtime/Engine/Classes/Animation/AnimSingleNodeInstance.h"
#include "Editor/Persona/Public/IPersonaPreviewScene.h"

#include "SkillNotify/CollisionSkillNotify.h"
#include "SkillEditorTools.h"
#include "DrawTools.h"

#include "Engine/Engine.h"


DrawTools::FDrawManager FSkillEditorViewportClient::DrawManager;
DrawTools::FDrawManager FSkillEditorViewportClient::NotifyDrawManager;

FSkillEditorViewportClient::FSkillEditorViewportClient(TWeakPtr<class FSkillEditor> ParentSkillEditor, const TSharedRef<class IPersonaPreviewScene>& AdvPreviewScene, const TSharedRef<class SSkillEditorViewport>& Viewport)//, UMyUI* ObjectToEdit)
	: FEditorViewportClient(nullptr, &AdvPreviewScene.Get(), StaticCastSharedRef<SEditorViewport>(Viewport))
	, SkillEditorPtr(ParentSkillEditor)
	, SkillEditorViewportPtr(Viewport)
{
	SetViewMode(VMI_Max);

	PersonaPreviewScene = static_cast<IPersonaPreviewScene*>(PreviewScene);

	SetViewLocation(FVector(0.0f, 3.0f, 2.0f));
	SetViewRotation(FRotator(-45.0f, -90.0f, 0.0f));
	SetViewLocationForOrbiting(FVector::ZeroVector, 500.0f);

	SetRealtime(true);
}

FSkillEditorViewportClient::~FSkillEditorViewportClient()
{
}

void FSkillEditorViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);	
}

void FSkillEditorViewportClient::Draw(const FSceneView * View, FPrimitiveDrawInterface * PDI)
{
	if (PersonaPreviewScene)
	{
		if (PersonaPreviewScene->GetPreviewMeshComponent())
		{
			if (PersonaPreviewScene->GetPreviewMeshComponent()->GetSingleNodeInstance())
			{
				DrawManager.SetCurrentTime(PersonaPreviewScene->GetPreviewMeshComponent()->GetSingleNodeInstance()->GetCurrentTime());
			}
		}
	}
	DrawManager.Draw(View, PDI);

	if (PersonaPreviewScene)
	{
		if (PersonaPreviewScene->GetPreviewMeshComponent())
		{
			if (PersonaPreviewScene->GetPreviewMeshComponent()->GetSingleNodeInstance())
			{
				NotifyDrawManager.SetCurrentTime(PersonaPreviewScene->GetPreviewMeshComponent()->GetSingleNodeInstance()->GetCurrentTime());
			}
		}
	}
	NotifyDrawManager.Draw(View, PDI);

	//Client->GetWorld()->ChangeFeatureLevel(ERHIFeatureLevel::ES3_1);
	FEditorViewportClient::Draw(View, PDI);

	//Client->GetWorld()->ChangeFeatureLevel(ERHIFeatureLevel::SM5);
	
}