// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopup.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "UI/UILib/Helper/GsUIBaseHelper.h"
#include "Components/InvalidationBox.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageInput.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Input/GsInputEventMsgBase.h"
#include "Option/GsOptionEnumType.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"


void UGsUIPopup::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	IGsInvalidateUIInterface::CheckInvalidate();

	for (FGsUICurveAnimation& curveAnim : _curveAnimationList)
	{
		if (curveAnim.IsPlaying())
		{
			curveAnim.Update(InDeltaTime);
		}
	}

#if PLATFORM_WINDOWS || WITH_EDITOR
	if (_useEditableFocusControl && _editableWidget.Num() > 0)
	{
		bool focus = _editableWidget.FindByPredicate([](UWidget* e) { return e->GetCachedWidget()->HasKeyboardFocus(); }) != nullptr;
		if (_preKeyboardFocused != focus)
		{
			_preKeyboardFocused = focus;
			if (focus) 
			{
				OnKeyboardFocusReceived();
			}
			else
			{
				OnKeyboardFocusLost();
			}
		}
	}
#endif
}

// 터치 이벤트 무조건 막기(카메라 회전 막기)
FReply UGsUIPopup::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUIPopup::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUIPopup::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

void UGsUIPopup::NativeConstruct()
{
	Super::NativeConstruct();
	IGsEditableInterface::InputInitialize(this);

#if PLATFORM_WINDOWS || WITH_EDITOR
	if (IsEditableWidgetInclude())
	{
		if (_useEditableFocusControl)
		{
			_editableWidget = LoadEditableWidgetRecursive(this);
			_preKeyboardFocused = _editableWidget.FindByPredicate([](UWidget* e) { return e->GetCachedWidget()->HasKeyboardFocus(); }) != nullptr;

			if (_preKeyboardFocused)
			{
				OnKeyboardFocusReceived();
			}
			else
			{
				OnKeyboardFocusLost();
			}
		}
		else
		{
			EnableEditableInput();
		}
	}
#endif
}

void UGsUIPopup::NativeDestruct()
{		
	_editableWidget.Reset();
	IGsEditableInterface::InputFinalize(this);
	Super::NativeDestruct();
}

void UGsUIPopup::OnKeyboardFocusLost()
{
	if (_useEditableFocusControl)
	{
		DisableEditableInput();
	}
}

void UGsUIPopup::OnKeyboardFocusReceived()
{
	if (_useEditableFocusControl)
	{
		EnableEditableInput();
	}
}

bool UGsUIPopup::OnBack()
{
	Close();

	return true;
}

void UGsUIPopup::OnClickBack()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->Back();
	}
}

void UGsUIPopup::SetInputComponent(UInputComponent* inputComponent)
{
	IGsEditableInterface::SetInputComponent(inputComponent);

	//InputComponent->BindAction<TDelegate<void(EGsKeyMappingType, EInputEvent)>>(TEXT("KeyboardMappingName_ok"), IE_Released,
	//	this, &UGsUIPopup::InputMapKeyAction, EGsKeyMappingType::KEY_MAPPING_OK, IE_Released);

	//InputComponent->BindAction<TDelegate<void(EGsKeyMappingType, EInputEvent)>>(TEXT("KeyboardMappingName_no"), IE_Released,
	//	this, &UGsUIPopup::InputMapKeyAction, EGsKeyMappingType::KEY_MAPPING_NO, IE_Released);
}

void UGsUIPopup::CommandInputMapKeyAction(EGsKeyMappingType keyMapType, EInputEvent inputEvent)
{
	switch(keyMapType)
	{
	case EGsKeyMappingType::KEY_MAPPING_OK:
		// 
		/**
		 * None
		 */
		break;
	case EGsKeyMappingType::KEY_MAPPING_NO:
		OnInputCancel();
		break;
	default:
		break;
	}
}