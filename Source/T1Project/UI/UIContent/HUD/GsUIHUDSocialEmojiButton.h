// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Message/GsMessageUI.h"
#include "GsUIHUDSocialEmojiButton.generated.h"

class UGsButton;
/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDSocialEmojiButton : public UGsUIHUD
{
	GENERATED_BODY()
protected:
	// 카메라 타입 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnEmoticon;

protected:
//	TArray<MsgUIHandle> _messageHandlers;
	bool _isEnableSocialMotion{ false };

protected:
	UFUNCTION()
	void OnClickOpenPopupEmoticon();

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

//	void OnMessageEnableMotion(const IGsMessageParam* param);
//private:
//	bool IsEnableSocialMotion();
};
