#pragma once
#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsUIHUDCounterAttackButton.generated.h"

class UGsButton;

UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDCounterAttackButton : public UGsUIHUD
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnCounterAttack;

	// 가상 함수
protected:
	// 최초 한번 호출
	virtual void NativeOnInitialized() override;

public:
	virtual void SetHUDMode(EGsUIHUDMode InMode) override;

	// ui event
public:
	UFUNCTION()
	void OnClickCounterAttack();

public:
	void UpdateVisible();
	void UpdateHudMode();

};