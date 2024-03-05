#pragma once
#include "CoreMinimal.h"

#include "UMG/Public/Blueprint/UserWidget.h"

#include "GsUISummonCardOpenHighEffect.generated.h"

UCLASS()
class T1PROJECT_API UGsUISummonCardOpenHighEffect : public UUserWidget
{
	GENERATED_BODY()
protected:

	// 연출 애니매이션
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animation;

public:
	void SetAnimationFirstPos();
	void PlayEffectAnimation();
};