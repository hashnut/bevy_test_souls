// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillNotifyBase.h"
#include "DataSchema/Skill/GsSchemaSkillCutRemain.h"
#include "DataSchema/Skill/GsSchemaSkillNotifySet.h"

#include "CutremineSkillNotify.generated.h"

/**
 * 
 */
UCLASS(const, hidecategories = Object, collapsecategories, meta = (DisplayName = "Cutremine Skill Notify"))
class SKILLEDITOR_API UCutremineSkillNotify : public USkillNotify//USkillNotifyBase
{
	GENERATED_BODY()
	
public:
	UCutremineSkillNotify();

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DC SKillNotifyData")
	FGsSchemaSkillCutRemain NotifyCutremineInfo;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DC SKillNotifyData")
	FGsSchemaSkillNotifySetRow SkillNotifySet;*/

#if WITH_EDITOR
	///FSimpleDelegate OnChangeCutremineSkillNotify;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
