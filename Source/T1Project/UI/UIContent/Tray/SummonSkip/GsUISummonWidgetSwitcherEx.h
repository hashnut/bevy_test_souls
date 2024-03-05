#pragma once

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "GsUISummonWidgetSwitcherEx.generated.h"

UCLASS()
class UGsUISummonWidgetSwitcherEx : public UWidgetSwitcher
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI")
	float _progressRate;

public:
	void SetProgressRate(float In_rate);
};