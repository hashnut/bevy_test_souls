// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillNotifyBase.h"
#include "DataSchema/Skill/NotifyInfo/GsSchemaSkillNotifySkillGuide.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"

#include "GuideEffectSkillNotify.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Guide Effect Skill Notify"))
class SKILLEDITOR_API UGuideEffectSkillNotify : public USkillNotify//USkillNotifyBase
{
	GENERATED_BODY()
	
public:
	UGuideEffectSkillNotify();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DC SKillNotifyData")
	FGsSchemaSkillNotifySkillGuide NotifySkillGuideInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DC SKillNotifyData")
	FGsSchemaSkillNotifySetRow SkillNotifySet;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SKillNotify")
	int SkillGuideEffectId;*/
	// 예시선 이펙트 지속 시간
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuideEffectSkillNotify")
	//float Duration = 0.5f;

	////UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuideEffectSkillNotify", meta = (AllowedClasses = "AGsActorSkillGuideEffect"))
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GuideEffectSkillNotify")
	//FSoftObjectPath SkillGuideEffect;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
	FTransform Tm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
	float DegAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
	float Width;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawData")
	float Heigt;*/
};
