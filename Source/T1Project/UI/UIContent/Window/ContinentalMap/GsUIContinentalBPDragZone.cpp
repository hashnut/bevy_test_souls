#include "GsUIContinentalBPDragZone.h"

#include "Core/Public/Delegates/Delegate.h"

#include "Engine/Classes/Engine/GameViewportClient.h"

#include "T1Project.h"

// GetMousePositionOnViewport -> 는 모바일에서 사용 못함
//https://answers.unrealengine.com/questions/818492/get-touch-pos-in-widget-umg.html?childToView=818487#answer-818487
FReply UGsUIContinentalBPDragZone::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	const FVector2D touchPos = InGestureEvent.GetScreenSpacePosition();

	/*GSLOG(Warning, TEXT("touchPos x: %f, y: %f"),
		touchPos.X, touchPos.Y);*/

	OnBPTouchMoved.ExecuteIfBound(touchPos);

	return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
}
FReply UGsUIContinentalBPDragZone::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	const FVector2D touchPos = InGestureEvent.GetScreenSpacePosition();

	OnBPTouchStarted.ExecuteIfBound(touchPos);
	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}
FReply UGsUIContinentalBPDragZone::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	const FVector2D touchPos = InGestureEvent.GetScreenSpacePosition();

	OnBPTouchEnd.ExecuteIfBound(touchPos);
	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
}