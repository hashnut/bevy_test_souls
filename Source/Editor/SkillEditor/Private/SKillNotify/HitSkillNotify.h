// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillNotifyBase.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"
#include "SkillSection/SkillSectionData.h"

#include "HitSkillNotify.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Hit Skill Notify"))
class UHitSkillNotify : public USkillNotify//USkillNotifyBase
{
	GENERATED_BODY()
	
public:
	UHitSkillNotify();

	UPROPERTY(VisibleAnywhere, Category = "DC SKillNotifyData")
	bool UseSkillNotifyCollision;

	UPROPERTY(VisibleAnywhere, Category = "DC SKillNotifyData")
	int32 SectionIndex;

	UPROPERTY(VisibleAnywhere, Category = "DC SKillNotifyData")
	TWeakObjectPtr<USkillSectionData> SkillSectionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DC SKillNotifyData")
	FGsSchemaSkillNotifySetRow SkillNotifySet;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

#if WITH_EDITOR
	//FSimpleDelegate OnChangeHitSkillNotify;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
