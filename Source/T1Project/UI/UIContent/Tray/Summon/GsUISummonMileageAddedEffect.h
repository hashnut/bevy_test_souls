#pragma once
#include "CoreMinimal.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "GsUISummonMileageAddedEffect.generated.h"

class UWidgetAnimation;

UCLASS()
class T1PROJECT_API UGsUISummonMileageAddedEffect : public UUserWidget
{
	GENERATED_BODY()
protected:
	// 연출 애니매이션
	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _animation;

protected:
	virtual void NativeConstruct() override;

public:
	// 애니 시작
	void StartAnimation();
};