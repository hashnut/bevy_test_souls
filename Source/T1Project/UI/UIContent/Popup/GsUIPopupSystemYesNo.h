// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Popup/GsUIPopupYesNo.h"
#include "GsUIPopupSystemYesNo.generated.h"

/**
 * 시스템 YesNo팝업. 게임 종료 팝업을 표시
 * - 기본 ZOrder 가 높음
 * - 로딩 중 파괴되지 않음
 * - 중복 생성 가능
 */
UCLASS()
class T1PROJECT_API UGsUIPopupSystemYesNo : public UGsUIPopupYesNo
{
	GENERATED_BODY()	

public:
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_SYSTEM_POPUP; }
	virtual bool IsNotDestroyedDuringLoading() const override { return true; }
	virtual bool IsManagedBySystemStack() const override { return true; }

	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_SYSTEM_POPUP; }
};
