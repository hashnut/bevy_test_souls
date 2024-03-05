#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"


#include "GsUIHUDPlayableEventGuide.generated.h"

class UWidgetSwitcher;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDPlayableEventGuide : public UGsUIHUD
{
	GENERATED_BODY()


protected:
	// 0: 조이스틱 이동
	// 1: 스킬 사용
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherType;
protected:
	// IGsInvalidateUIInterface overrde.
	virtual void InvalidateAllInternal() override;
};