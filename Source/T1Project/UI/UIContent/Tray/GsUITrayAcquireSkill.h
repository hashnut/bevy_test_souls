// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUITrayAcquireSkill.generated.h"

/**
 * 스킬 획득 티커
 */
UCLASS()
class T1PROJECT_API UGsUITrayAcquireSkill : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	float _lifeTimeS = 1.8f;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsUISkillIcon* _uiIcon;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _textBlockName;	

protected:
	float _remainTime = 0.f;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
public:
	void SetSkill(SkillId InSkillId);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayAcquireAnimation();
};
