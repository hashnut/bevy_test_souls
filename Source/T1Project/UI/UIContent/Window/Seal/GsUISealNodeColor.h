#pragma once

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "GsUISealNodeColor.generated.h"

class UWidgetAnimation;

/** 봉인노드 색상 이펙트 */
UCLASS()
class UGsUISealNodeColor final : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _color0;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _color1;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _color2;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* _color3;

	UPROPERTY(EditInstanceOnly, Category = "GsUI")
	SealSlotColor _color;

public:
	void SynchronizeProperties() final;

private:
	void InvalidateColor();
};
