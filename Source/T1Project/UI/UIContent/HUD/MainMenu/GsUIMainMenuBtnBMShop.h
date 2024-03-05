// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UIContent/HUD/MainMenu/GsUIMainMenuBtnBase.h"
#include "GsUIMainMenuBtnBMShop.generated.h"

class UTextBlock;
class UGsUIRedDotBase;
class IGsRedDotCheck;
struct FGsSharedParam;

/**
 * HUD 메뉴버튼. BMShop 전용로직 넣을 것
 */
UCLASS(meta = (DisableNativeTick))
class T1PROJECT_API UGsUIMainMenuBtnBMShop : public UGsUIMainMenuBtnBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUIMainMenu", meta = (BindWidget))
	UGsUIRedDotBase* _bmShopRootRedDot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UUserWidget* _uiEffect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockLimitTime;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _displayProductCountRoot;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _countText;

private:
	FDateTime _bmShopEffectTimer;

protected:
	EGsIconRedDotType _redDotIconType = EGsIconRedDotType::CHECK;
	IGsRedDotCheck* _redDotCheck = nullptr;

public:
	virtual void SetRedDotCheck(IGsRedDotCheck* InLogic, EGsIconRedDotType InType = EGsIconRedDotType::CHECK) override;
	virtual bool UpdateRedDot(bool bUpdate) override;

	virtual IGsRedDotCheck* GetRedDotCheck() const override { return _redDotCheck; }
	virtual EGsIconRedDotType GetRedDotType() const override { return _redDotIconType; }

protected:
	virtual void NativeConstruct() override;

	virtual void SetMenuOpen(bool bIsOpen) override;

public:
	void UpdateManagerTick(float InDeltaTime);

protected:
	// 블루프린트 연출을 위해 전달
	UFUNCTION(BlueprintImplementableEvent)
	void OnSubMenuOpen(bool bIsOpen);

public:
	void UpdateBMShopEffect(FGsSharedParam& InMessageParam);

private:
	void UpdateBMShopIconEffect(bool InState);
	void UpdateBMShopRemainTime(bool InState);
	void UpdateBMShopOpenProductCount(bool InState);
};
