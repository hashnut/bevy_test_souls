// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SKillNotify/SkillNotifyBase.h"
#include "GroupCollisionSkillNotify.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class SKILLEDITOR_API UGroupCollisionSkillNotify : public USkillNotifyBase
{
	GENERATED_BODY()
	
public:
	UGroupCollisionSkillNotify();

#if WITH_EDITOR
	FSimpleDelegate OnChangeGroupCollisionSkillNotify;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void ValidateAssociatedAssets() override;
#endif
	
};
