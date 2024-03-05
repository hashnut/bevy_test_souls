#include "GsUIScanDragObject.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "Engine/Classes/Engine/GameViewportClient.h"
#include "Engine/Public/Slate/SceneViewport.h"
#include "Engine/Classes/GameFramework/PlayerController.h"

#include "Engine.h"

#include "T1Project.h"

void UGsUIScanDragObject::NativeConstruct()
{
	Super::NativeConstruct();

	_squaredDragDiffVal = GData()->GetGlobalData()->_squaredDragDiffVal;
}

void UGsUIScanDragObject::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}
void UGsUIScanDragObject::SetDragWidgetPos(FVector2D In_localPos)
{
	FVector2D localHalfSize = _dragWidget->GetCachedGeometry().GetLocalSize() * 0.5f;

	FVector2D calcPos = In_localPos - localHalfSize;
	_dragWidget->SetRenderTranslation(calcPos);
}

FReply UGsUIScanDragObject::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchMoved(InGeometry, InGestureEvent);

	const FVector2D touchPos = InGestureEvent.GetScreenSpacePosition();
	_currentTouchPos = touchPos;
	
	//GSLOG(Warning, TEXT("NativeOnTouchMoved touchPos x: %f, y: %f"),
	//	touchPos.X, touchPos.Y);
	
	FVector2D posLocal = InGeometry.AbsoluteToLocal(touchPos);
	SetDragWidgetPos(posLocal);

	if (_isTouchStart == true)
	{
		if (_isDragOn == false)
		{
			// check In ui
			if (FVector2D::DistSquared(_dragStartPos, _currentTouchPos) > _squaredDragDiffVal)
			{
				SetIsDragOn(true);
			}
		}

		return FReply::Handled();
	}
	// drag out-> in ui
	// use camera move
	// https://jira.com2us.com/jira/browse/C2URWQ-688
	return FReply::Unhandled();
}
FReply UGsUIScanDragObject::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchStarted(InGeometry, InGestureEvent);

	const FVector2D touchPos = InGestureEvent.GetScreenSpacePosition();
	_currentTouchPos = touchPos;
	_dragStartPos = touchPos;
	_isTouchStart = true;

	FVector2D posLocal = InGeometry.AbsoluteToLocal(touchPos);
	SetDragWidgetPos(posLocal);

	//GSLOG(Warning, TEXT("NativeOnTouchStarted touchPos x: %f, y: %f"),
	//	touchPos.X, touchPos.Y);

	if (nullptr != CallbackTouchStart)
	{
		CallbackTouchStart();
	}
	
	return FReply::Handled().CaptureMouse(TakeWidget());
}
FReply UGsUIScanDragObject::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	Super::NativeOnTouchEnded(InGeometry, InGestureEvent);

	const FVector2D touchPos = InGestureEvent.GetScreenSpacePosition();
	_currentTouchPos = touchPos;
	

	//GSLOG(Warning, TEXT("NativeOnTouchEnded touchPos x: %f, y: %f"),
	//	touchPos.X, touchPos.Y);


	// scan start

	// 터치 시작하여(영역안에서 시작) 터치를 놓을때만 처리
	// 외부에서 드래그후 스캔버튼에서 놓았을때 스캔안되게 처리
	// https://jira.com2us.com/jira/browse/C2URWQ-1224
	if(_isTouchStart == true)
	{ 
		if (nullptr != CallbackScanStart)
		{
			CallbackScanStart(_isDragOn);
		}
	}

	_isTouchStart = false;

	SetIsDragOn(false);

	if (nullptr != CallbackTouchEnd)
	{
		CallbackTouchEnd();
	}

	//FVector2D posLocal = InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition());
	return FReply::Handled().ReleaseMouseCapture();
}

void UGsUIScanDragObject::SetIsDragOn(bool In_isDragOn)
{
	_isDragOn = In_isDragOn;

	ESlateVisibility dragVisibility = (_isDragOn == true) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;
	_dragWidget->SetVisibility(dragVisibility);
}