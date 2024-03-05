// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/HUD/MainMenu/GsUIMainMenuBtnSimple.h"
#include "GsUIMainMenuBtnNormal.generated.h"

class UImage;
class UGsUIRedDotBase;
class IGsRedDotCheck;

/**
 * HUD 메뉴버튼. New, 레드닷 있음
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIMainMenuBtnNormal : public UGsUIMainMenuBtnSimple
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UGsUIRedDotBase* _redDot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UImage* _imgNew;

protected:
	EGsIconRedDotType _redDotIconType = EGsIconRedDotType::CHECK;
	IGsRedDotCheck* _redDotCheck = nullptr;
	IGsRedDotCheck* _reinforceCheck = nullptr;

public:
	virtual void SetRedDotCheck(IGsRedDotCheck* InLogic, EGsIconRedDotType InType = EGsIconRedDotType::CHECK) override;
	virtual void SetReinforceCheck(IGsRedDotCheck* InLogic) override { _reinforceCheck = InLogic; }
	virtual bool UpdateRedDot(bool bUpdate) override;

	virtual IGsRedDotCheck* GetRedDotCheck() const override { return _redDotCheck; }
	virtual EGsIconRedDotType GetRedDotType() const override { return _redDotIconType; }

	virtual void SetContentIsNew(bool bIsNew) override;

	virtual void SetMenuOpen(bool bIsOpen) override;

protected:
	// 블루프린트 연출을 위해 전달
	UFUNCTION(BlueprintImplementableEvent)
	void OnSubMenuOpen(bool bIsOpen);
};
