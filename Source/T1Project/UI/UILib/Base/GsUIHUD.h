// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsUIHUD.generated.h"

/**
 * HUDFrame을 구성하는 HUD 구성요소
 */
UCLASS(Abstract, meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUD : public UUserWidget, public IGsInvalidateUIInterface
{
	GENERATED_BODY()

protected:
	// 어떤 모드에서 이 UI를 감출 것인지. (비어있으면 어떤 모드에서도 보임)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GsUIHUD")
	TArray<EGsUIHUDMode> _hideHUDModeList;

protected:
	// IGsInvalidateUIInterface override
	virtual void InvalidateAllInternal() override {}

public:
	virtual bool UpdateManagerTick(float InDeltaTime);

public:
	// 기본 동작을 넣고, 예외가 있으면 상속받아서 구현
	virtual void SetHUDMode(EGsUIHUDMode InMode);

	bool IsHideMode(EGsUIHUDMode InMode) const;
};
