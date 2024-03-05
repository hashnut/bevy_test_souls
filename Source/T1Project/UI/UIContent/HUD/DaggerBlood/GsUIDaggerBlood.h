#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIBloodDot.h"
#include "GsUIDaggerBlood.generated.h"

class UWidgetSwitcher;
class UImage;
using DotState = UGsUIBloodDot::DotState;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIDaggerBlood : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_EVENT(UGsUIDaggerBlood, FGsBloodDotUsed);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _bloodDropSwitcher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	UGsUIBloodDot* _getBloodDot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	UGsUIBloodDot* _getLoopBloodDot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	UGsUIBloodDot* _completeBloodDot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	UGsUIBloodDot* _useBloodDot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUI")
	UImage* _baseBloodDot;

protected:

	TMap<DotState, UWidget*> _bloodState;
	DotState _curState{ DotState::None };
	DotState _nxtState{ DotState::None };

public:
	FGsBloodDotUsed OnUsedDaggerBlood;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetDaggerBloodStat(DotState state, bool animEnd = false);
	DotState GetDaggerBloodStat() { return _curState; }

};