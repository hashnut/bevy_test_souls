// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "ActorEx/GsActorFieldFairy.h"
#include "GsUIBillboardFairyTalkBalloon.generated.h"

class UWidgetAnimation;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIBillboardFairyTalkBalloon : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _fairyName;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FSlateColor _nameColor;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _fairyTalk;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", Meta = (BindWidgetAnim, AllowPrivateAccess = true), Transient)
	UWidgetAnimation* _animationPopup;

	AGsActorFieldFairy* _ownFairy;

	float _endTime{ 0.f };

protected:
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetName(FText fairyName, FairyGrade grade);
	void SetTalk(FText talk);

	void SetFairyActor(AActor* fairy) { _ownFairy = Cast<AGsActorFieldFairy>(fairy); }

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "GsUI")
	void StartAnimation();

};
