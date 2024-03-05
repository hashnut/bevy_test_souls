
#include "GsTouchInputProcessor.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMessageParam.h"


FGsTouchInputProcessor::FGsTouchInputProcessor()
{
}

void FGsTouchInputProcessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
}

bool FGsTouchInputProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	UGsUIManager* uiMgr = GUI();
	if (nullptr == uiMgr)
	{
		return false;
	}

	UWorld* world = uiMgr->GetWorld();
	if (nullptr == world)
	{
		return false;
	}

	FGeometry viewportGeometry = UWidgetLayoutLibrary::GetViewportWidgetGeometry(world);
	FVector2D viewportLocalSize = viewportGeometry.GetLocalSize();
	FVector2D mouseLocal = viewportGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());

	//GSLOG(Log, TEXT("[TEST_TOUCH] mouseLocal: %.2f, %.2f, viewportLocalSize: %.2f, %.2f"),
	//	mouseLocal.X, mouseLocal.Y, viewportLocalSize.X, viewportLocalSize.Y);

	if (0 > mouseLocal.X ||
		0 > mouseLocal.Y ||
		viewportLocalSize.X < mouseLocal.X ||
		viewportLocalSize.Y < mouseLocal.Y)
	{
		return false;
	}

	// 터치 이펙트 출력

	//const FString touchVfxPath = TEXT("/Game/Launcher/UI/Resource/LauncherFX/WBP_FX_ButtonTouch.WBP_FX_ButtonTouch");
	if (UClass* touchVfxClass = UGsPathManager::GetClassStatic(uiMgr->GetTouchVfxPath()))
	{
		UGsBlueprintFunctionLibraryUI::AddVFXToPointerPos(touchVfxClass, 1000);
	}
	else
	{
		GSLOG(Error, TEXT("[TEST_TOUCH] Touch effect class is null: %s"), *(uiMgr->GetTouchVfxPath().ToString()));
	}

	if (FGsMessageHolder* msg = GMessage())
	{
		FGsPrimitivePairFloat systemParam(mouseLocal.X, mouseLocal.Y);
		msg->GetSystemParam().SendMessage(MessageSystem::TOUCH_SCREEN, &systemParam);
	}

	return false;
}
