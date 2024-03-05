#include "GsUIHUDFramePlayableEvent.h"

#include "UI/UIContent/HUD/GsUIHUDPlayableEventGuide.h"
#include "UI/UIContent/HUD/GsUIHUDPlayableEventQuest.h"

#include "UI/UILib/Base/GsUIHUD.h"

void UGsUIHUDFramePlayableEvent::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_hudList.Emplace(_guideUI);
	_hudList.Emplace(_questUI);
}

void UGsUIHUDFramePlayableEvent::OnManagerTick(float InDeltaTime)
{
	// 보일 때만 틱이 돌도록
	if (false == IsVisible())
	{
		return;
	}

	for (UGsUIHUD* iter : _hudList)
	{
		iter->UpdateManagerTick(InDeltaTime);
	}
}

void UGsUIHUDFramePlayableEvent::InvalidateAllMenu()
{
	for (UGsUIHUD* hud : _hudList)
	{
		hud->InvalidateAll();
	}
}
