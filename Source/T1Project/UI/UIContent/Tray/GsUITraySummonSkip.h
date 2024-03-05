#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITraySummonSkip.generated.h"

class UGsButton;
class UGsUISummonWidgetSwitcherEx;

class UWidget;

UCLASS()
class T1PROJECT_API UGsUITraySummonSkip : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSkip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	TArray<UGsUISummonWidgetSwitcherEx*> _arrayProgressSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	TArray<UWidget*> _arrayWidgetPos;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _moveProgressWidget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidget* _dragCanvas;

private:
	bool _dirtySetProgress = false;
	float _currentProgressRate = 0.0f;

	// virtual function
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// bp call function
public:
	UFUNCTION()
	void OnClickSkip();

	// logic function
public:
	void OnChangedProgressRate(float In_progressRate);
	// switcher
	void ChangeProgressRateSwitchers(float In_progressRate);
	// move widget
	void ChangeProgressRateMoveWidget(float In_progressRate);
};