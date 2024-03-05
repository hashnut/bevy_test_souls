// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SkillNotifyBase.h"
//#include "GameObject/Skill/Data/GsSkillNotifyDataBase.h"

#include "CancleSkillNotify.generated.h"

UCLASS(HideDropdown)
class SKILLEDITOR_API UCancleSkillNotify : public USkillNotifyBase
{
	GENERATED_BODY()
	
public:
	UCancleSkillNotify();

#if WITH_EDITOR
	FSimpleDelegate OnChangeCancleSkillNotify;
#endif

	// Begin UObject interface
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
