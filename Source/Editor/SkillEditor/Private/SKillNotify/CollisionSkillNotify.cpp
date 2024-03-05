// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionSkillNotify.h"

#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Animation/AnimSequenceBase.h"

#include "ActorComponentEx/GsSkillCollisionComponent.h"
#include "DrawPrimitiveComponent.h"
#include "SkillEditorViewportClient.h"
#include "DrawTools.h"
#include "Shared/Client/SharedEnums/SharedSkillEnum.h"

#if WITH_EDITOR
#include "Logging/MessageLog.h"
#include "Misc/UObjectToken.h"
#endif

UCollisionSkillNotify::UCollisionSkillNotify()
	: Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(192, 255, 99, 255);
	LifeTime = 0.3f;
	FillMode = true;
#endif // WITH_EDITORONLY_DATA
}

void UCollisionSkillNotify::PostLoad()
{
	Super::PostLoad();

	//RotationOffsetQuat = FQuat(RotationOffset);
}

#if WITH_EDITOR
void UCollisionSkillNotify::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if (PropertyChangedEvent.Property != nullptr)
	{
		OnChangeSkillNotify.ExecuteIfBound();
	}	
}

void UCollisionSkillNotify::ValidateAssociatedAssets()
{	
}
#endif

void UCollisionSkillNotify::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{	
	/*if (NotifyCollisionInfo.type == SkillCollisionType::NONE)
		return;

	DrawTools::FDrawData DrawData;
	if (SkillCollisionType::CIRCLE == NotifyCollisionInfo.type)
		DrawData.DrawType = DrawTools::EDrawType::Circle;	
	else
	if (SkillCollisionType::RECT == NotifyCollisionInfo.type)
		DrawData.DrawType = DrawTools::EDrawType::Rect;
	else
	if (SkillCollisionType::FAN == NotifyCollisionInfo.type)
		DrawData.DrawType = DrawTools::EDrawType::Cone;

	DrawData.TM = Tm;
	DrawData.DegAngle = NotifyCollisionInfo.angle;
	DrawData.LifeTime = LifeTime;
	DrawData.StartTime = 0.0f;
	DrawData.Radius = NotifyCollisionInfo.radius;
	DrawData.Width = NotifyCollisionInfo.width;
	DrawData.Height = NotifyCollisionInfo.height;
	DrawData.Color = NotifyColor;
	DrawData.FillMode = FillMode;

	FSkillEditorViewportClient::DrawManager.PushDrawData(DrawData);*/
}

FString UCollisionSkillNotify::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}