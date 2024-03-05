#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Message/GsMessageContentHud.h"

#include "GsHUDPlayableEvent.generated.h"

class UGsUIHUDFramePlayableEvent;

UCLASS()
class T1PROJECT_API AGsHUDPlayableEvent : public AHUD
{
	GENERATED_BODY()

private:

	MsgPlayableEventHudHandleArray _hudHandlerList;

	TWeakObjectPtr<UGsUIHUDFramePlayableEvent> _hudWidget;


public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type in_type) override;


public:
	void InitMessageHandler();
	void RemoveMessageHandler();

	// ¸Þ½ÃÁö
public:
	void InvalidateAll();
	void InvalidateQuest();
	void InvalidateGuide();

	// get
public:
	TWeakObjectPtr<UGsUIHUDFramePlayableEvent> GetHudWidget();
};