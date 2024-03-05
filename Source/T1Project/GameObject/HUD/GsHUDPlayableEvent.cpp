#include "GsHUDPlayableEvent.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/GsMessageHolder.h"

#include "UI/UIContent/HUD/GsUIHUDFramePlayableEvent.h"

#include "UI/UIContent/HUD/GsUIHUDPlayableEventGuide.h"
#include "UI/UIContent/HUD/GsUIHUDPlayableEventQuest.h"
#include "../Management/GsMessageHolder.h"
#include "../Message/GsMessageContents.h"

void AGsHUDPlayableEvent::BeginPlay()
{
	Super::BeginPlay();
	InitMessageHandler();


}
void AGsHUDPlayableEvent::EndPlay(const EEndPlayReason::Type in_type)
{
	RemoveMessageHandler();
	Super::EndPlay(in_type);
}

void AGsHUDPlayableEvent::InitMessageHandler()
{
	FGsMessageHolder* msg = GMessage();

	MPlayableEventHud& contentsHUD = msg->GetContentsPlayableEventHud();

	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentPlayableEventHud::INVALIDATE_ALL,
		this, &AGsHUDPlayableEvent::InvalidateAll));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentPlayableEventHud::INVALIDATE_QUEST,
		this, &AGsHUDPlayableEvent::InvalidateQuest));
	_hudHandlerList.Emplace(contentsHUD.AddUObject(MessageContentPlayableEventHud::INVALIDATE_GUIDE,
		this, &AGsHUDPlayableEvent::InvalidateGuide));
}
void AGsHUDPlayableEvent::RemoveMessageHandler()
{
	FGsMessageHolder* message = GMessage();

	for (MsgPlayableEventHudHandle& msgHandler : _hudHandlerList)
	{
		message->GetContentsPlayableEventHud().Remove(msgHandler);
	}
	_hudHandlerList.Empty();
}
void AGsHUDPlayableEvent::InvalidateAll()
{
	TWeakObjectPtr<UGsUIHUDFramePlayableEvent> hudWidget = GetHudWidget();
	if (hudWidget.IsValid())
	{
		hudWidget->InvalidateAllMenu();
	}

}
void AGsHUDPlayableEvent::InvalidateQuest()
{
	TWeakObjectPtr<UGsUIHUDFramePlayableEvent> hudWidget = GetHudWidget();
	if (hudWidget.IsValid())
	{
		if (UGsUIHUDPlayableEventQuest* questUI = hudWidget->GetQuest())
		{
			questUI->InvalidateAll();
		}
	}
}
void AGsHUDPlayableEvent::InvalidateGuide()
{
	TWeakObjectPtr<UGsUIHUDFramePlayableEvent> hudWidget = GetHudWidget();
	if (hudWidget.IsValid())
	{
		if (UGsUIHUDPlayableEventGuide* guideUI = hudWidget->GetGuide())
		{
			guideUI->InvalidateAll();
		}
	}
}

TWeakObjectPtr<UGsUIHUDFramePlayableEvent> AGsHUDPlayableEvent::GetHudWidget()
{
	if (_hudWidget.IsValid())
	{
		return _hudWidget;
	}

	UGsUIManager* uiManager = GUI();
	if (nullptr != uiManager)
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = uiManager->GetWidgetByKey(TEXT("HUDPlayableEvent"), false);
		if (widget.IsValid())
		{
			_hudWidget = Cast<UGsUIHUDFramePlayableEvent>(widget.Get());
		}
	}

	return _hudWidget;
}