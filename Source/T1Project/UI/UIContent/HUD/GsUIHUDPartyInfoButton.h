#pragma once
#include "CoreMinimal.h"

#include "UI/UILib/Base/GsUIHUD.h"

#include "Message/GsMessageGameObject.h"

#include "GsUIHUDPartyInfoButton.generated.h"

class UGsButton;

class UWidgetSwitcher;

struct IGsMessageParam;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDPartyInfoButton : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	// switcher index
	// 0: not party
	// 1: in party
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherPartyState;

	// 1: root rule
	// 2. party member count
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textPartyInfo;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnInfo;

	// 메시지 관리
	MsgGameObjHandleArray _arrDelegateGameObjectMsg;



	// 가상 함수
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// message
protected:
	void OnPartyHUDSideTabUpdate(const IGsMessageParam*);

	// ui 이벤트
protected:
	// 파티 정보 클릭 
	UFUNCTION()
	void OnClickPartyInfoButton();


};