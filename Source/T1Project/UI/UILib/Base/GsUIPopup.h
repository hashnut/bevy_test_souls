// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Runtime/DataCenter/Public/KeyMapping/GsKeyMappingEnum.h"

#include "GsUIPopup.generated.h"

/**
 * 스택관리되고, 화면을 덮지않는 UI의 기본 클래스.
 */
UCLASS()
class T1PROJECT_API UGsUIPopup : public UGsUIWidgetBase, public IGsInvalidateUIInterface, public IGsEditableInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "GsUI")
	bool _bCanMultipleInstance = false;

	UPROPERTY(EditDefaultsOnly, Category = "GsUI")
	bool _useEditableFocusControl{ false };

protected:
	bool _bStackRecoverFlag = false;
	bool _bRemainStack = false;

	UPROPERTY(Transient)
	TArray<UWidget*> _editableWidget;

	bool _preKeyboardFocused{ false };

protected:
	// UUserWidget override	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;

protected:
	// UUserWidget override.
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void OnKeyboardFocusLost();
	virtual void OnKeyboardFocusReceived();

protected:
	// UGsUIWidgetBase override
	virtual bool OnBack() override;
	virtual void SetStackRecoverFlag(bool bIsOn) override { _bStackRecoverFlag = bIsOn; }

protected:
	// IGsInvalidateUIInterface override
	virtual void InvalidateAllInternal() override {}

public:
	// UGsUIWidgetBase override
	virtual bool IsStackable() const final { return true; }
	virtual bool IsWindow() const final { return false; }
	virtual bool CanMultipleInstance() const override { return _bCanMultipleInstance; }
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_POPUP; }
	virtual bool IsRemainStack() const override { return _bRemainStack; }
	virtual void SetRemainStack(bool bIsRemain) { _bRemainStack = bIsRemain; }

	virtual void CommandInputMapKeyAction(EGsKeyMappingType keyMapType, EInputEvent inputEvent) override;

	// IGsEditableInterface override
protected:
	virtual void SetInputComponent(UInputComponent* inputComponent) override;


protected:
	// 뒤로가기 버튼 클릭 시
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	virtual void OnClickBack();

	// 주의: 하위 팝업에서 상속받아 구현. 키보드와 버튼에서의 동작을 통일하기 위함
	virtual void OnInputOk() {}
	virtual void OnInputCancel() {}

public:
	bool IsStackRecoverFlag() const { return _bStackRecoverFlag; }
};
