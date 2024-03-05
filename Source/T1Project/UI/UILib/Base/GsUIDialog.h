// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#define TEST_QUEST

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "GsUIDialog.generated.h"

/**
 * 다이얼로그 창 기본
 */
UCLASS()
class T1PROJECT_API UGsUIDialog : public UGsUITray
{
	GENERATED_BODY()

public:
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_DIALOGUE; }
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_DIALOG; }

	virtual TrayCondition GetTrayCondition() const override { return TrayCondition::TC_Private; }
};
