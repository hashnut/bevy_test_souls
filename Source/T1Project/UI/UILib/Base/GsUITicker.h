#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUITicker.generated.h"

UCLASS()
class T1PROJECT_API UGsUITicker : public UGsUITray
{
	GENERATED_BODY()

private:
	TMap<UUMGSequencePlayer*, float> _timeSet;

public:
	virtual TrayCondition GetTrayCondition() const override { return TrayCondition::TC_Active; }
	
protected:
	virtual void PauseAllAnimations() override;
	virtual void RestartAllAnimations() override;
	virtual void ClearAllPauseAnimations() override;

protected:
	virtual void OnAnimationStartedPlaying(UUMGSequencePlayer& Player) override;
};

