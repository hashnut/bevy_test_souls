// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Message/GsMessageUI.h"
#include "Message/GsMessageContents.h"

#include "DataSchema/GsSchemaEnums.h"

#include "UI/UILib/Base/GsUIHUD.h"

#include "GsUIHUDCamera.generated.h"

/**
 * HUD 카메라버튼 영역
 */

class UTextBlock;
class UButton;
class UGsButton;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDCamera : public UGsUIHUD
{
	GENERATED_BODY()
	
protected:
	// 카메라 타입 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _cameraTypeText;
	// 카메라 넘버 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _cameraNumberText;
	// fixed quarter mode yaw type(n,e,s,w)
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _cameraFixedQuarterYawTypeText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCamera;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnFixedQuarterYawChange;

	MsgUIHandle _enableBtnHandle;
	TArray<TPair<MessageContentCamera, FDelegateHandle>> _msgContentCameraDelegates;

	// virtual func
protected:
	// UUserWidget override
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// GsInvalidateUIInterface override
	virtual void InvalidateAllInternal();

	// logic func
public:
	void Reset();
	void ClickedCameraViewChange();
	
	// ui binding function
protected:
	UFUNCTION()
	void OnClickCamera();
	UFUNCTION()
	void OnClickFixedQuraterYawChange();

	// msg binding func
public:
	void OnChangedFixedQuarterYaw();

	// get, set
public:
	void SetCameraTypeText(EGsCameraMode In_mode);
	void SetEnableCameraButton(const struct IGsMessageParam* InParam);
	void SetCameraVisibility(bool InShow);
	void SetFixedQuarterYawMode(bool In_isShow);
};
