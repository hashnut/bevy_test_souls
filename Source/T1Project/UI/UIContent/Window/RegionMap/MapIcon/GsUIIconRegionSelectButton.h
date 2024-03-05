#pragma once
#include "CoreMinimal.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsUIIconRegionSelect.h"
#include "GsUIIconRegionSelectButton.generated.h"

class UGsButton;

UCLASS()
class T1PROJECT_API UGsUIIconRegionSelectButton : public UGsUIIconRegionSelect
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnIcon;

private:
	TFunction<void()> _callbackClick;

protected:
	virtual void NativeOnInitialized() override;

	// logic function
public:
	UFUNCTION()
	void OnClickButton();

	// set
public:
	void SetClickCallback(TFunction<void()> In_callback)
	{
		_callbackClick = In_callback;
	}
};