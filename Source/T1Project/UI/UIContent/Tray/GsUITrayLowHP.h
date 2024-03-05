// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "UI/UILib/Define/GsUIDefine.h"
#include "GsUITrayLowHP.generated.h"

/**
 *  Tray Loading 
 */
UCLASS()
class T1PROJECT_API UGsUITrayLowHP : public UGsUITray
{
	GENERATED_BODY()
public:
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_LOW_HP; }
};
