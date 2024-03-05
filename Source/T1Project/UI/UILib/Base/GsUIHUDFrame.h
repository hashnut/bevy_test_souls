// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsUIHUDFrame.generated.h"

/**
 * HUD 기본 클래스
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIHUDFrame : public UGsUIWidgetBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(Transient)
	TArray<class UGsUIHUD*> _hudList;

public:
	// UGsWidgetbase override
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_HUD; }
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_HUD; }

public:
	virtual void SetHUDMode(EGsUIHUDMode InMode);
};
