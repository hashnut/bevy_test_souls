// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
#include "GameFlow/GameContents/GsContentsMode.h"
#include "DataSchema/GsSchemaEnums.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Message/GsMessageInput.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"
#include "GsUIWindow.generated.h"

/**
 * 스택 관리되고, 화면을 덮는 UI 들의 기본 클래스.
 * - 주의: State와 Sync를 이루는 윈도우의 경우, SyncContentState 반드시 오버라이딩 하여 구현할 것
 * - 주의: _bStackRecoverFlag 값을 체크하여 NativeConstruct / NativeDestruct에서 처리할 것 
 */
UCLASS()
class T1PROJECT_API UGsUIWindow : public UGsUIWidgetBase, public IGsInvalidateUIInterface
{
	GENERATED_BODY()

	friend class UGsUIController;

protected:
	UPROPERTY(EditAnywhere, Category = "GsUI", meta = (DisplayName = "Help Contents Key"))
	EGsHelpContents _helpContentsKey{ EGsHelpContents::None };

protected:
	// 다른 윈도우가 열리며 창이 강제로 닫힐 경우에 true (UGsUIController::PushStack 참고)
	// 일반 Close 상황, CloseAllStack, 맵이동등에 의한 강제 제거 시 강제로 false 처리
	bool _bStackRecoverFlag = false;
	TArray<MsgInputHandle> _inputHandlerList;
	
protected:
	// UUserWidget override
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// 카메라 회전 막기 처리
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual void CommandInputMapKeyAction(EGsKeyMappingType keyMapType, EInputEvent inputEvent) override;

protected:
	// UGsUIWidgetBase override
	virtual bool OnBack() override;

protected:
	// IGsInvalidateUIInterface override
	virtual void InvalidateAllInternal() override {}

public:
	// UGsUIWidgetBase override
	virtual bool IsStackable() const final { return true; }
	virtual bool IsWindow() const final { return true; }
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_WINDOW; }

	virtual void InputMapKeyAction(EGsKeyMappingType keyMapType, EInputEvent inputEvent) override;

protected:
	// 일반 Close 상황, 
	virtual void SetStackRecoverFlag(bool bIsOn) override { _bStackRecoverFlag = bIsOn; }

protected:
	// 뒤로가기 버튼 클릭 시
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual void OnClickBack();

	// HUD로 가는 버튼 눌렀을 때
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual void OnClickCloseAllStack();

	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual void OnClickContentsHelp();
protected:
	// 주의: State와 Sync를 이루는 윈도우의 경우 반드시 구현할 것
	virtual bool SyncContentState() { return false; }

protected:
	bool IsSameContentState(int32 InStateType) const;
	void OnKeyboardInput(const IGsMessageParam* InParam);

public:
	bool IsStackRecoverFlag() const { return _bStackRecoverFlag; }
};
