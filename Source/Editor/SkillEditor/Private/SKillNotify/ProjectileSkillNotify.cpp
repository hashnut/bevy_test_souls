// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSkillNotify.h"

#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "SkillEditorViewportClient.h"

UProjectileSkillNotify::UProjectileSkillNotify()
	: Super()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor(192, 255, 99, 255);
#endif // WITH_EDITORONLY_DATA		
}

#if WITH_EDITOR
void UProjectileSkillNotify::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	if (PropertyChangedEvent.Property != nullptr)
	{
		OnChangeSkillNotify.ExecuteIfBound();
	}

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void UProjectileSkillNotify::Notify(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation)
{
	FSkillEditorViewportClient::NotifyDrawManager.ClearDrawData(SectionIndex);

	if (UseSkillNotifyCollision)
	{
		if (SkillNotifySet.GetRow())
		{
			const FGsSchemaSkillNotifySet* skillNotifySet = SkillNotifySet.GetRow();
			for (const FGsSchemaSkillCollisionRow& el : skillNotifySet->collisionInfoList)
			{
				const FGsSchemaSkillCollision* skillCollision = el.GetRow();
				DrawTools::FDrawData DrawData;
				DrawData.DrawType = DrawTools::EDrawType::None;
				if (skillCollision->type == SkillCollisionType::CIRCLE)
					DrawData.DrawType = DrawTools::EDrawType::Circle;
				else if (skillCollision->type == SkillCollisionType::FAN)
					DrawData.DrawType = DrawTools::EDrawType::Cone;
				else if (skillCollision->type == SkillCollisionType::RECT)
					DrawData.DrawType = DrawTools::EDrawType::Rect;

				float EndTime = SkillSectionData.Get() != nullptr ? SkillSectionData.Get()->EndTime : FSkillEditorViewportClient::NotifyDrawManager.GetCurrentTime() + 0.5f;

				DrawData.TM = FTransform::Identity;
				DrawData.DegAngle = skillCollision->angle;
				DrawData.LifeTime = EndTime - FSkillEditorViewportClient::NotifyDrawManager.GetCurrentTime();
				DrawData.StartTime = FSkillEditorViewportClient::NotifyDrawManager.GetCurrentTime();
				DrawData.EndTime = EndTime;
				DrawData.Radius = skillCollision->maxRadius;
				DrawData.Width = skillCollision->width;
				DrawData.Height = skillCollision->height;
				DrawData.Color = NotifyColor;// FColor(192, 128, 128, 128); //NotifyColor;
				DrawData.FillMode = true;
				DrawData.PosOffset = skillCollision->posOffset;
				DrawData.OffsetX = skillCollision->offsetX;
				DrawData.OffsetY = skillCollision->offsetY;
				DrawData.AngleOffset = skillCollision->angleOffset;
				DrawData.AngleOffset2 = skillCollision->angleOffset2;
				DrawData.CollisionDraw = true;// el->CollisionDraw;
				DrawData.MaxRadius = skillCollision->maxRadius;
				DrawData.MinRadius = skillCollision->minRadius;

				FSkillEditorViewportClient::NotifyDrawManager.PushDrawData(SectionIndex, DrawData);
				//FSkillEditorViewportClient::NotifyDrawManager.PushDrawData(DrawData);
			}
		}
	}
}