#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Common/Icon/GsUIIconDefault.h"
#include "GsUIIconMinimap.generated.h"

/*
	미니맵 아이콘
*/
class UCanvasPanel;
class UUserWidget;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIIconMinimap : public UGsUIIconDefault
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIIcon", meta = (BindWidget))
	UCanvasPanel* _iconRoot;

public:
	// 캔버스에 붙임
	void AddCanvas(UUserWidget* In_widget);
};