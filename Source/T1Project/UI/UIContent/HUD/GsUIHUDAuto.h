// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIHUD.h"
#include "GsUIHUDAuto.generated.h"



/**
 * HUD 자동진행 
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIHUDAuto : public UGsUIHUD
{
	GENERATED_BODY()
	
	// 멤버 변수
protected:
	
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsButton* _btnAuto;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcher;
	// 가상 함수
protected:
	// 최초 한번 호출
	virtual void NativeOnInitialized() override;	
	virtual void InvalidateAllInternal() override;

	// 로직 함수
public:
	
	// 버튼 클릭
	UFUNCTION(BlueprintCallable, Category = "GsUI")
	void OnClickAutoButton();
	UFUNCTION(BlueprintImplementableEvent, Category = "GsUI")
	void OnPlayAni();
	// 클릭 테스트
	void TestClick();

public:
	void ClickedAutoButton();

	// get, set
public:
	//  상태 바꾸기
	void SetOnOffState(bool In_isOn);

	void SetQuestAutoState(bool In_isOn);
};
