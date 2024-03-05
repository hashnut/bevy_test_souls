// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIHUDCamera.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsSkillManager.h"

#include "Management/ScopeGame/GsGameObjectmanager.h"
#include "Management/ScopeGame/GsCameraModeManager.h"

#include "Message/MessageParam/GsUIMessageParam.h"


#include "GameObject/ObjectClass/GsGameObjectLocalPlayer.h"

#include "Camera/Define/GsCameraGameDefine.h"

#include "Input/GsInputEventMsgBase.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Engine/GameViewportClient.h"

#include "Components/TextBlock.h"


void UGsUIHUDCamera::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnCamera->OnClicked.AddDynamic(this, &UGsUIHUDCamera::OnClickCamera);
	_btnFixedQuarterYawChange->OnClicked.AddDynamic(this, &UGsUIHUDCamera::OnClickFixedQuraterYawChange);
}

void UGsUIHUDCamera::NativeConstruct()
{
	FGsMessageHolder* msg = GMessage();

	_enableBtnHandle = msg->GetUI().AddUObject(MessageUI::ENABLE_CAMERA_BUTTON, this,
		&UGsUIHUDCamera::SetEnableCameraButton);


	_msgContentCameraDelegates.Emplace(
		msg->GetCamera().AddUObject(MessageContentCamera::CHANGED_FIXED_QUARTER_YAW, this,
			&UGsUIHUDCamera::OnChangedFixedQuarterYaw));

	Super::NativeConstruct();
}

void UGsUIHUDCamera::NativeDestruct()
{
	FGsMessageHolder* msg = GMessage();

	msg->GetUI().Remove(_enableBtnHandle);

	if (_msgContentCameraDelegates.Num() != 0)
	{
		for (auto iter : _msgContentCameraDelegates)
		{
			msg->GetCamera().Remove(iter);
		}
		_msgContentCameraDelegates.Empty();
	}

	Super::NativeDestruct();
}

void UGsUIHUDCamera::Reset()
{
	SetCameraVisibility(true);
}

void UGsUIHUDCamera::OnClickCamera()
{
	ClickedCameraViewChange();
}

void UGsUIHUDCamera::ClickedCameraViewChange()
{
	FGsInputEventMsgBase msg;
	GMessage()->GetInput().SendMessage(MessageInput::ViewChange, msg);

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDCamera::SetCameraTypeText(EGsCameraMode In_mode)
{
	FText textCam;
	FText textNumber;
	if (EGsCameraMode::BackView == In_mode)
	{
		FText::FindText(TEXT("UICommonText"), TEXT("HUDCameraBack"), textCam);

		textNumber = FText::AsNumber(1);
	}
	else if (EGsCameraMode::Quarter == In_mode)
	{
		FText::FindText(TEXT("UICommonText"), TEXT("HUDCameraQuater"), textCam);

		textNumber = FText::AsNumber(2);
	}
	else if (EGsCameraMode::Boss == In_mode)
	{
		FText::FindText(TEXT("UICommonText"), TEXT("HUDCameraBoss"), textCam);
	}
	else if (EGsCameraMode::FixedQuarter == In_mode)
	{
		FText::FindText(TEXT("UICommonText"), TEXT("HUDCameraFixed"), textCam);

		textNumber = FText::AsNumber(3);
	}

	if (_cameraTypeText)
	{
		_cameraTypeText->SetText(textCam);
	}

	if (_cameraNumberText)
	{
		_cameraNumberText->SetText(textNumber);
	}

	bool isFixedQuarterBtnShow = (In_mode == EGsCameraMode::FixedQuarter) ? true : false;
	SetFixedQuarterYawMode(isFixedQuarterBtnShow);
}


void UGsUIHUDCamera::SetEnableCameraButton(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamBool* param = InParam->Cast<const FGsUIMsgParamBool>();
	if (param)
	{
		// 카메라 막기 대신 감추기로 기획변경
		SetCameraVisibility(param->_data);
	}
}

void UGsUIHUDCamera::SetCameraVisibility(bool InShow)
{
	if (_btnCamera)
	{
		_btnCamera->SetVisibility(InShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UGsUIHUDCamera::InvalidateAllInternal()
{
	if (FGsCameraModeManager* camMgr = GSCameraMode())
	{
		EGsCameraMode currentMode =  camMgr->GetCurrentMode();
		SetCameraTypeText(currentMode);

		bool isFixedQuarterBtnShow = (currentMode == EGsCameraMode::FixedQuarter) ? true : false;		
		SetFixedQuarterYawMode(isFixedQuarterBtnShow);
	}
}
void UGsUIHUDCamera::SetFixedQuarterYawMode(bool In_isShow)
{
	_btnFixedQuarterYawChange->SetVisibility(In_isShow ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (In_isShow == false)
	{
		return;
	}

	if (FGsCameraModeManager* camMgr = GSCameraMode())
	{
		EGsCameraFixedQuarterYawType yawType = camMgr->GetFixedQuarterYawType();
		FString strType;
		switch (yawType)
		{
		case EGsCameraFixedQuarterYawType::North:
			strType = "N";
			break;
		case EGsCameraFixedQuarterYawType::East:
			strType = "E";
			break;
		case EGsCameraFixedQuarterYawType::South:
			strType = "S";
			break;
		case EGsCameraFixedQuarterYawType::West:
			strType = "W";
			break;
		}
		FText textType = FText::FromString(strType);
		_cameraFixedQuarterYawTypeText->SetText(textType);

	}
}

void UGsUIHUDCamera::OnClickFixedQuraterYawChange()
{
	GMessage()->GetCamera().SendMessage(MessageContentCamera::CLICK_FIXED_QUARTER_YAW);

	// close skill target select
	GSkill()->SetIsShowSkillTargetSelect(false);
}

void UGsUIHUDCamera::OnChangedFixedQuarterYaw()
{
	SetFixedQuarterYawMode(true);
}