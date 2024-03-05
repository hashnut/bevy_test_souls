// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SKillNotify/SkillNotifyBase.h"
#include "RandomCollisionSkillNotify.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class SKILLEDITOR_API URandomCollisionSkillNotify : public USkillNotifyBase
{
	GENERATED_BODY()
	
public:
	URandomCollisionSkillNotify();

#if WITH_EDITOR
	FSimpleDelegate OnChangeRandomCollisionSkillNotify;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void ValidateAssociatedAssets() override;
#endif

};
