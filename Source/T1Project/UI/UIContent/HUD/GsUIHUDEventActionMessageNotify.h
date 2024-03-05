#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "../UI/UILib/Define/GsUIDefine.h"
#include "GsUIHUDEventActionMessageNotify.generated.h"

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDEventActionMessageNotify : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UTextBlock* _message;

protected:
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

public:
	void SetData(const FText& InMessage);
};