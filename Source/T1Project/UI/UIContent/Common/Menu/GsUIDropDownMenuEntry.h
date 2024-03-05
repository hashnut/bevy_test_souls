// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GsUIDropDownMenuEntry.generated.h"

struct IMenuData {};

class UGsSwitcherButton;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIDropDownMenuEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnSelectedMenuEntry, const class UGsUIDropDownMenuEntry*);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsSwitcherButton* _btnEntry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI", meta = (BindWidget))
	FText _menuName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GsUI", meta = (BindWidget))
	FLinearColor _menuTextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (DisplayName = "MenuTextEnableColor"))
	FLinearColor _menuEnableColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (DisplayName = "MenuTextSelectColor"))
	FLinearColor _menuSelectColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Appearance, meta = (DisplayName = "MenuTextDisableColor"))
	FLinearColor _menuDisableColor;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FSlateBrush _iconBrush;

private:
	FLinearColor _backupMenuTextColor{ FLinearColor::White };
	FVector2D	 _brushImageSize{ 64, 64 };
	FLinearColor _brushTintColor{FLinearColor::White};

	int32 _disableIndex{ 0 };
	bool _isUseUnselecdNoti{ false };

protected:
	TWeakPtr<IMenuData> _data;

public:
	FOnSelectedMenuEntry OnSelectedMenu;

	virtual void BeginDestroy() override;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//virtual void SynchronizeProperties() override;

protected:
	UFUNCTION()
	void OnClickMenu();

	UFUNCTION()
	virtual void OnSelectedEntry(bool isSelected);

	virtual void InitEntry();
	virtual void RefreshEntry();

protected:
	void SetMenuName(FText name) { _menuName = name; }
	void SetMenuNormalColor(FLinearColor color) { _menuEnableColor = color; }
	void SetMenuSelectColor(FLinearColor color) { _menuSelectColor = color; }
	void SetMenuDisableColor(FLinearColor color) { _menuDisableColor = color; }
	void SetMenuIconPaths(FSoftObjectPath path, FVector2D imgSize = FVector2D::ZeroVector, FLinearColor tintColor = FLinearColor::Transparent );
	void SetMenuData(TWeakPtr<IMenuData> data) { _data = data; }

	void SetMenuEnabled(bool set);
	void SetIsSelected(bool set);
	void SetUseUnselectEnty(bool set) { _isUseUnselecdNoti = set; }

public:
	FText GetMenuName() const { return _menuName; }
	FLinearColor GetMenuColor() const { return _menuTextColor; }
	const FSlateBrush& GetMenuIcon() const { return _iconBrush;  }
	FVector2D GetMenuIconImageSize() const;
	FLinearColor GetMenuIconTintColor() const;

	friend class UGsUIDropDownMenuCommon;
};
