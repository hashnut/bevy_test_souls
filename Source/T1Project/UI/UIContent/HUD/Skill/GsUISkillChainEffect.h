// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUISkillChainEffect.generated.h"

class URetainerBox;
class UImage;
class FGsSkill;
class UPaperSprite;

/**
 * 스킬버튼 체인 스킬 연출
 */
UCLASS()
class T1PROJECT_API UGsUISkillChainEffect : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnFinishChangeAnimation);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	URetainerBox* _retainerBox;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIcon1;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UImage* _imgIcon2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	float _animTime = 0.35f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _grayDarkness = 1.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	float _grayOpacity = 1.f;

protected:
	float _remainTime = 0.f;

public:
	FOnFinishChangeAnimation OnFinishAnimation;

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetData(const FGsSkill* InSkill, const FGsSkill* InNextSkill, bool bInIsEnabled);
	bool IsPlayingEffect() const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayChainAnimation();

	void OnFinishChainAnimation();

	void SetIconImageSprite(UPaperSprite* InSprite, OUT UImage* OutImage);
	void SetGrayScale(bool bInIsGray, OUT UImage* OutImage);
};
