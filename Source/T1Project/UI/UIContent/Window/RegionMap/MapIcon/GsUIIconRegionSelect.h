#pragma once
#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconDefault.h"
#include "GsUIIconRegionSelect.generated.h"

/*
	 아이콘: 선택 표시 처리
*/
class UWidgetAnimation;
class UWidgetSwitcher;

UCLASS()
class T1PROJECT_API UGsUIIconRegionSelect : public UGsUIIconDefault
{
	GENERATED_BODY()

protected:

	// 선택 스위치(0: 선택안함, 1: 선택)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherSelect;

	// spot name
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textSpotName;

	UPROPERTY(BlueprintReadOnly, Category = Animations, meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _ani_blink;

protected:
	virtual void NativeDestruct() override;

public:
	// 선택 처리
	virtual void SetSelect(bool In_select);
	// 선택 애니 플레이
	void PlaySelectAnimation(bool In_select);

	// name setting
	void SetSpotName(FText In_name);
};