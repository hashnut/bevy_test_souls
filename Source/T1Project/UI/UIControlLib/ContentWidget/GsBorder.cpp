// Fill out your copyright notice in the Description page of Project Settings.


#include "GsBorder.h"

#include "Slate/SlateBrushAsset.h"
#include "Materials/MaterialInterface.h"
#include "Engine/Texture2D.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Layout/SBorder.h"
#include "Components/BorderSlot.h"
#include "ObjectEditorUtils.h"

#include "Components/ContentWidget.h"
#include "Public/TimerManager.h"
#include "UI/UIContent/Tray/GsUITrayLongPressBar.h"
#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"
#include "Public/Blueprint/SlateBlueprintLibrary.h"
#include "Public/Blueprint/WidgetLayoutLibrary.h"
#include "Management/ScopeGlobal/GsPathManager.h"

#define LOCTEXT_NAMESPACE "UMG"


void UGsBorder::SynchronizeProperties()
{
	//Super::SynchronizeProperties();
	UContentWidget::SynchronizeProperties();

	//---
	TAttribute<FLinearColor> ContentColorAndOpacityBinding = PROPERTY_BINDING(FLinearColor, ContentColorAndOpacity);
	TAttribute<FSlateColor> BrushColorBinding = OPTIONAL_BINDING_CONVERT(FLinearColor, BrushColor, FSlateColor, ConvertLinearColorToSlateColor);
	TAttribute<const FSlateBrush*> ImageBinding = OPTIONAL_BINDING_CONVERT(FSlateBrush, Background, const FSlateBrush*, ConvertImage);

	MyBorder->SetPadding(Padding);
	MyBorder->SetBorderBackgroundColor(BrushColorBinding);
	MyBorder->SetColorAndOpacity(ContentColorAndOpacityBinding);

	MyBorder->SetBorderImage(ImageBinding);

	MyBorder->SetDesiredSizeScale(DesiredSizeScale);
	MyBorder->SetShowEffectWhenDisabled(bShowEffectWhenDisabled != 0);

	MyBorder->SetOnMouseButtonDown(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonDownEx)); // Ex로 바꿈
	MyBorder->SetOnMouseButtonUp(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseButtonUpEx)); // Ex로 바꿈
	MyBorder->SetOnMouseMove(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseMoveEx)); // Ex로 바꿈
	MyBorder->SetOnMouseDoubleClick(BIND_UOBJECT_DELEGATE(FPointerEventHandler, HandleMouseDoubleClick));
	//---

	_bIsPressed = false;
	_bIsLongPressed = false;
}

void UGsBorder::ReleaseSlateResources(bool bReleaseChildren)
{
	ResetLongPress();

	Super::ReleaseSlateResources(bReleaseChildren);
}

FReply UGsBorder::HandleMouseButtonDownEx(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (_useLongPress)
	{
		if (false == _bIsPressed)
		{
			_bIsPressed = true;
			_bIsLongPressed = false;
			_pressPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
			
			if (UWorld* world = GetWorld())
			{
				// onClick 과 OnPress 의 차이를 두기위해 임시로 0.3 초 동안 Press 일때로 가정
				FTimerManager* timerMgr = &world->GetTimerManager();
				timerMgr->ClearTimer(_timerHandle);
				timerMgr->SetTimer(_timerHandle, this,
					&UGsBorder::OnEndTimer, 0.3f, false);
			}
		}
	}

	if (OnMouseButtonDownEvent.IsBound())
	{
		return OnMouseButtonDownEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	// MouseCapture를 사용하려고 했는데, 그러면 ListView의 OnItemClicked 이벤트를 막아버려 문제가 됨.
	return FReply::Unhandled();
}

FReply UGsBorder::HandleMouseButtonUpEx(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (_useLongPress)
	{
		ResetLongPress();
	}

	if (OnMouseButtonUpEvent.IsBound())
	{
		return OnMouseButtonUpEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	return FReply::Unhandled();
}

FReply UGsBorder::HandleMouseMoveEx(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	if (_useLongPress)
	{
		if (_bIsPressed)
		{
			// 움직이면 롱프레스를 취소하기 위한 처리.
			// 이동 안해도 들어오는 경우가 있어 Press 지점에서 얼마나 이동했는지를 검사한다 
			FVector2D currPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
			float distSquared = FVector2D::DistSquared(
				_pressPos, currPos);
			if (distSquared >= _moveCheckValue)
			{
				ResetLongPress();
			}
		}
	}
	
	if (OnMouseMoveEvent.IsBound())
	{
		return OnMouseMoveEvent.Execute(Geometry, MouseEvent).NativeReply;
	}

	return FReply::Unhandled();
}

void UGsBorder::ResetLongPress()
{
	if (_timerHandle.IsValid())
	{
		if (UWorld* world = GetWorld())
		{
			FTimerManager& timerMgr = world->GetTimerManager();
			timerMgr.ClearTimer(_timerHandle);
		}
	}

	if (_longPressObjPtr.IsValid())
	{
		UGsBlueprintFunctionLibraryUI::RemoveWidget(_longPressObjPtr.Get());
		_longPressObjPtr.Reset();
	}

	_bIsPressed = false;
}

void UGsBorder::OnEndTimer()
{
	if (false == _bIsPressed)
	{
		return;
	}

	// 타겟이 있다면 타겟에 붙인다
	UWidget* targetWidget = (_logPressBarTarget.IsValid()) ? _logPressBarTarget.Get() : this;
	UGsUITrayLongPressBar* trayLongPress = UGsBlueprintFunctionLibraryUI::AddLongPressBar(
		targetWidget, _longPressGaugeBarZOrder);

	_longPressObjPtr = trayLongPress;

	if (trayLongPress)
	{
		if (false == trayLongPress->_onFinishedLongPress.IsBoundToObject(this))
		{
			trayLongPress->_onFinishedLongPress.AddUObject(this, &UGsBorder::OnFinishedLongPressTrayMessage);
		}
	}
}

void UGsBorder::OnFinishedLongPressTrayMessage(bool InIsFullGauge)
{
	if (InIsFullGauge)
	{
		if (_bIsPressed)
		{
			// 누른 위치와 동일할 때만 성공
			FVector2D currPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
			float distSquared = FVector2D::DistSquared(_pressPos, currPos);
			if (distSquared < _moveCheckValue)
			{
				_bIsLongPressed = true;
				OnFinishedLongPress.ExecuteIfBound();
			}
		}

		ResetLongPress();
	}
}

#undef LOCTEXT_NAMESPACE
