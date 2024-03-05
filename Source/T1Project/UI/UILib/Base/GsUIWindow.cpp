// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindow.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "UI/UILib/Helper/GsUIBaseHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Components/InvalidationBox.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageInput.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Input/GsInputEventMsgBase.h"
#include "Option/GsOptionEnumType.h"
#include "DataSchema/KeyMapping/GsSchemaKeyMapping.h"


void UGsUIWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
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
}

void UGsUIWindow::NativeConstruct()
{
	MWindowInput& input = GMessage()->GetWindowInput();

	Super::NativeConstruct();
}

void UGsUIWindow::NativeDestruct()
{
	UnregisterInputComponent();

	Super::NativeDestruct();
}

FReply UGsUIWindow::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUIWindow::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Handled();
}

FReply UGsUIWindow::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return FReply::Unhandled();
}

bool UGsUIWindow::OnBack()
{
	Close();

	return true;
}

void UGsUIWindow::OnClickBack()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->Back();
	}
}

void UGsUIWindow::OnClickCloseAllStack()
{
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->CloseAllStack(this);
	}
}

void UGsUIWindow::OnClickContentsHelp()
{
	//if (_helpContentsKey == EGsHelpContents::None) return;
	if (UGsUIManager* uiMgr = GUI())
	{
		FGsUIHelper::OpenHelpPage(_helpContentsKey);
	}
}

bool UGsUIWindow::IsSameContentState(int32 InStateType) const
{
	if (GMode()->GetCurrentContentStateIndex() == InStateType)
	{
		return true;
	}

	return false;
}

void UGsUIWindow::OnKeyboardInput(const IGsMessageParam* InParam)
{
	const FGsInputEventMsgWindowKey* inputEventMsgWindowKey = InParam->Cast<const FGsInputEventMsgWindowKey>();
	if (nullptr == inputEventMsgWindowKey)
		return;

	switch (static_cast<EGsKeyMappingType>(inputEventMsgWindowKey->WindowKeyType))
	{
	case EGsKeyMappingType::KEY_MAPPING_NO:
		OnClickBack();
		break;
	default:
		break;
	}
}

void UGsUIWindow::InputMapKeyAction(EGsKeyMappingType keyMapType, EInputEvent inputEvent)
{
	switch (keyMapType)
	{
	case EGsKeyMappingType::KEY_MAPPING_NO:
		OnClickBack();
		break;
	default:
		break;
	}
}


void UGsUIWindow::CommandInputMapKeyAction(EGsKeyMappingType keyMapType, EInputEvent inputEvent)
{
	switch (keyMapType)
	{
	case EGsKeyMappingType::KEY_MAPPING_OK:
		// 
		/**
		 * None
		 */
		break;
	case EGsKeyMappingType::KEY_MAPPING_NO:
		OnClickBack();
		break;
	default:
		break;
	}
}