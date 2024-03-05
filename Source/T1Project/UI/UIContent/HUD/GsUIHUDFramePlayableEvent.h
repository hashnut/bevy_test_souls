#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUDFrame.h"

#include "GsUIHUDFramePlayableEvent.generated.h"

class UGsUIHUDPlayableEventGuide;
class UGsUIHUDPlayableEventQuest;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDFramePlayableEvent : public UGsUIHUDFrame
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDPlayableEventGuide* _guideUI;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUIHUDPlayableEventQuest* _questUI;

public:
	virtual void NativeOnInitialized() override;

protected:
	// UGsUIWidgetBase override
	virtual bool IsEnableManagerTick() const override { return true; }
	virtual void OnManagerTick(float InDeltaTime) override;

public:
	void InvalidateAllMenu();

	// get
public:
	UGsUIHUDPlayableEventGuide* GetGuide() { return _guideUI; }
	UGsUIHUDPlayableEventQuest* GetQuest() { return _questUI; }
};