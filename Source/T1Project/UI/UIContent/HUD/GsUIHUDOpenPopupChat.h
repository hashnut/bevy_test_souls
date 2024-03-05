#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Input/GsInputEventMsgBase.h"
#include "GsUIHUDOpenPopupChat.generated.h"

class UGsButton;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDOpenPopupChat : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	// 카메라 타입 텍스트
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnChat;

	TPair<MessageInput, FDelegateHandle> _inputMessageHandler;

protected:
	UFUNCTION()
	void OnClickOpenPopupChat();

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeOnInitialized() override;
	void OnMessageOpenPopup(const IGsMessageParam* param);
};

