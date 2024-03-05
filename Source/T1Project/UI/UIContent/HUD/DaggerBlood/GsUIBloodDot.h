#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIBloodDot.generated.h"

class UImage;
/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIBloodDot : public UUserWidget
{
	GENERATED_BODY()

public:
	enum class DotState { None, Get, GetLoop, CompleteLoop, UsePlay };

	DECLARE_EVENT(UGsUIBloodDot, FGsAnimationFinished);
	FGsAnimationFinished& OnCompleteAnimation() { return _OnCompleteAnimation; }

//protected:
//	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
//	UImage* _image;
//
//	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GsUI")
//	float _aniTime = 0.8f;

public:
	FGsAnimationFinished _OnCompleteAnimation;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void StartEffectAnimation();

	// Get, UsePlay일 경우 블루프린트의 OnAnimationFinished 이벤트에서 불러줍니다
	UFUNCTION(BlueprintCallable)
	void FinishEffectAnimation();

public:
	void UpdateWidget();

};
