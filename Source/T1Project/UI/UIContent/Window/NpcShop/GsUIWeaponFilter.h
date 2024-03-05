// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "GameObject/Define/GsGameObjectDefine.h"

#include "Shared/Client/SharedEnums/SharedCreatureEnum.h"

#include "GsUIWeaponFilter.generated.h"

/**
 * Ω∫≈≥∫œ ªÛ¡° ≈« π´±‚ « ≈Õ UI
 */

class UGsButton;
class UGsSwitcherButton;
UCLASS()
class T1PROJECT_API UGsUIWeaponFilter : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE(FOnNpcShopWeaponFilterSave);

public:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnSword;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnDagger;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnBow;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnShotgun;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnWand;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnOrb;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnSave;

protected:
	TArray<TPair<ENpcShopProductListTab, UGsSwitcherButton*>> _buttonTabTypeList;

public:
	FOnNpcShopWeaponFilterSave OnWeaponFilterSave;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void ShowOrHide();
	void Show();
	void Hide();

protected:
	UFUNCTION()
	void OnClickSave();

	void OnClickWeapon(int32 InIndex, bool InIsSelected);

	CreatureWeaponType GetWeaponType(ENpcShopProductListTab InTabType);

};
