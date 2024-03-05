// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/Tray/GsUITrayTickerMessage.h"
#include "GsUITrayTickerSystem.generated.h"

/**
 * 시스템 메시지 출력
 * - Depth 높음, 로딩중 사라지지 않음
 */
UCLASS()
class T1PROJECT_API UGsUITrayTickerSystem : public UGsUITrayTickerMessage
{
	GENERATED_BODY()

protected:
	// System 메시지는 위에 출력될 수 있도록 기본 Depth를 높게 설정
	virtual int32 GetManagedDefaultZOrder() const override { return UI_DEPTH_SYSTEM_TRAY; }

	// 로딩 중 사라지지 않음
	virtual bool IsNotDestroyedDuringLoading() const { return true; }

	// UI_HIDE_SYSTEM_TRAY Flag에 의해 Hide
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_SYSTEM_TRAY; }
};
