
#include "GsHerculesInputProcess.h"
#include "SlateApplication.h"
#include "Widgets/SViewport.h"
#include "Events.h"
#include "Management/ScopeGlobal/GsHiveManager.h"
#include "Hercules_App.h"
#include "T1Project.h"
#include "Engine/Engine.h"

void FGsHerculesInputProcess::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{

}

bool FGsHerculesInputProcess::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{	
	if (GHive()->IsInitializedHercules())
	{
		const FGeometry& viewportGeometry = SlateApp.GetGameViewport()->GetCachedGeometry();
		FVector2D localPos = viewportGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());

		CHerculesPlugin::TouchEvent(localPos.X, localPos.Y, 1);
	}

	return IInputProcessor::HandleMouseMoveEvent(SlateApp, MouseEvent);
}

bool FGsHerculesInputProcess::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	if (GHive()->IsInitializedHercules())
	{
		const FGeometry& viewportGeometry = SlateApp.GetGameViewport()->GetCachedGeometry();
		FVector2D localPos = viewportGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		FVector2D viewPortSize = viewportGeometry.GetLocalSize();
		
		SCREEN_MESSAGE(1000, 100.0f, FColor::Orange, "Press : [HandleMouseButtonDownEvent %.2f, %.2f]", localPos.X, localPos.Y);

		CHerculesPlugin::SetScreenSize(viewPortSize.X, viewPortSize.Y);
		CHerculesPlugin::TouchEvent(localPos.X, localPos.Y, 0);
	}

	return IInputProcessor::HandleMouseButtonDownEvent(SlateApp, MouseEvent);
}

bool FGsHerculesInputProcess::HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	if (GHive()->IsInitializedHercules())
	{
		const FGeometry& viewportGeometry = SlateApp.GetGameViewport()->GetCachedGeometry();
		FVector2D localPos = viewportGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());

		SCREEN_MESSAGE(1000, 100.0f, FColor::Orange, "Release : [HandleMouseButtonUpEvent %.2f, %.2f]", localPos.X, localPos.Y);

		CHerculesPlugin::TouchEvent(localPos.X, localPos.Y, 2);
	}

	return IInputProcessor::HandleMouseButtonUpEvent(SlateApp, MouseEvent);

}