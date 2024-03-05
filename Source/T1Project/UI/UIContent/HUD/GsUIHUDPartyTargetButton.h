#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsUIHUDPartyTargetButton.generated.h"

class UWidgetSwitcher;

class UGsButton;


UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDPartyTargetButton : public UGsUIHUD
{
	GENERATED_BODY()

protected:
	// 0: active
	// 1: deactive
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherType;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	FText _textTarget;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnTarget;

private:
	bool _isActive = false;
	// 가상 함수
protected:
	// 최초 한번 호출
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
public:
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

	// ui event
public:
	UFUNCTION()
	void OnClickTarget();

	// logic func
public:
	void UpdateSharedIdStateHandler();

	// set
public:
	void SetTargetName(const FString& In_name);
	void SetIsActive(bool In_isActive);	
};