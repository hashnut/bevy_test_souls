// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/HUD/MainMenu/GsUIMainMenuBtnBase.h"
#include "GsUIMainMenuBtnHamberger.generated.h"

class UGsUIRedDotBase;
class IGsRedDotCheck;

/**
 * HUD 메뉴버튼, 햄버거 메뉴
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIMainMenuBtnHamberger : public UGsUIMainMenuBtnBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UGsUIRedDotBase* _redDot;

protected:
	EGsIconRedDotType _redDotIconType = EGsIconRedDotType::NUMBER;
	IGsRedDotCheck* _redDotCheck = nullptr;

protected:
	virtual void NativeConstruct() override;

public:
	virtual void SetRedDotCheck(IGsRedDotCheck* InLogic, EGsIconRedDotType InType = EGsIconRedDotType::CHECK) override;
	virtual bool UpdateRedDot(bool bUpdate) override;

	virtual IGsRedDotCheck* GetRedDotCheck() const override { return _redDotCheck; }
	virtual EGsIconRedDotType GetRedDotType() const override { return _redDotIconType; }

	virtual void SetMenuOpen(bool bIsOpen) override;
};
