// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/HUD/MainMenu/GsUIMainMenuBtnBase.h"
#include "GsUIMainMenuBtnInventory.generated.h"

class UWidgetSwitcher;
class UTextBlock;
class UImage;
class UGsUIRedDotBase;
class IGsRedDotCheck;
struct IGsMessageParam;

/**
 * HUD 메뉴버튼. 인벤토리 전용로직 넣을 것
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIMainMenuBtnInventory : public UGsUIMainMenuBtnBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UGsUIRedDotBase* _redDot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UImage* _imgNew;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UWidgetSwitcher* _switcherWeightPenalty;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLow;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMid;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockHigh;

protected:
	EGsIconRedDotType _redDotIconType = EGsIconRedDotType::CHECK;
	IGsRedDotCheck* _redDotCheck = nullptr;

public:
	virtual void SetRedDotCheck(IGsRedDotCheck* InLogic, EGsIconRedDotType InType = EGsIconRedDotType::CHECK) override;
	virtual bool UpdateRedDot(bool bUpdate) override;

	virtual IGsRedDotCheck* GetRedDotCheck() const override { return _redDotCheck; }
	virtual EGsIconRedDotType GetRedDotType() const override { return _redDotIconType; }

	virtual void SetContentIsNew(bool bIsNew) override;

	virtual void SetMenuOpen(bool bIsOpen) override;

protected:
	// 블루프린트 연출을 위해 전달
	UFUNCTION(BlueprintImplementableEvent)
	void OnSubMenuOpen(bool bIsOpen);

public:
	void UpdateInvenWeight(const IGsMessageParam* InParam);
	void UpdateWeightPenaltyType(const IGsMessageParam* InParam);
};
