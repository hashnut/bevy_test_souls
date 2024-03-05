// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFlow/GameContents/ContentsGame/Hud/BMShop/ServerMigrate/GsServerMigrateHandler.h"
#include "GsUIServerMigrateServerListSortComboBox.generated.h"

class UPanelWidget;
class UWidgetSwitcher;
class UGsSwitcherButton;
/**
 *
 */
UCLASS()
class T1PROJECT_API UGsUIServerMigrateServerListSortComboBox : public UUserWidget
{
	GENERATED_BODY()

public:
	enum class MenuType { Normal, Recommended, Spacious };

	DECLARE_DELEGATE_OneParam(FOnSelectedMenu, MenuType);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnSortMenu;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _panelMenu;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnNormal;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnRecommended;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnSpacious;

public:
	FOnSelectedMenu OnSelectedMenu;

protected:
	float _mouseLeaveTime{ 0.f };
	TMap<MenuType, UGsSwitcherButton*> _menuItems;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

protected:
	UFUNCTION()
	void OnClickedMenu();

	UFUNCTION()
	void OnClickedNormal();

	UFUNCTION()
	void OnClickedRecommended();

	UFUNCTION()
	void OnClickedSpacious();

private:
	void OpenMenuPopup();
	void CloseMenuPopup();
	bool IsOpenPopup();
	void OnMenuSelected(MenuType type, UGsSwitcherButton* btn, bool noti = true);

public:
	void SetMenuSelect(MenuType type);
};
