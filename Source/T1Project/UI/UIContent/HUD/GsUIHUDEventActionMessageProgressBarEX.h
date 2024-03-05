#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "../UI/UILib/Define/GsUIDefine.h"
#include "GsUIHUDEventActionMessageProgressBarEX.generated.h"

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDEventActionMessageProgressBarEX : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UCanvasPanel* _panelProgressBar;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UProgressBar* _timeBar;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _timeText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		class UTextBlock* _message;

	FDateTime _beingTime = 0;

	FDateTime _endTime = 0;
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

public:
	virtual bool UpdateManagerTick(float InDeltaTime) override;

protected:
	void SetTimerVisibility(bool InIsVisibility);
	bool IsTimerOn() const;

	void UpdateTime(float InDeltaTime = 0.f);
	void PlayAnimation();
	void Stop();
	bool IsPlayAnimation() const;

public:
	void SetData(const FText& InMessage, int64 InElapsedTime, int64 InTotalTime);
};