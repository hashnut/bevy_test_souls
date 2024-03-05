// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUITrayEnchantResultSuccess.generated.h"

class UGsButton;
class UTextBlock;
class UGsUIIconBase;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayEnchantResultSuccess : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UGsButton* _btnConfirm;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _itemNameText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _itemShieldCountText;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
		UTextBlock* _itemSuccessText;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
		UGsHorizontalBoxIconSelector* _targetItemIconSelector;
	UPROPERTY()
		UGsDynamicIconSlotHelper* _targetIconSlotHelper;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherIcon;

private:
	float _currentTime = 0.f;
	float _lockMaxTime = 0.f;
	ItemDBId _targetItemDbid = 0;
	TFunction<void()> _destroyCallback;

	const int32 SHOW_SHIELD = 0;
	const int32 SHOW_MAGICAL_FORGE = 1;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetDisplayItem(const ItemDBId InItemDbid, float InLockMaxTime, bool InIsEnchant = true);
	void SetDestroyCallback(TFunction<void()> InDestroyCallback);
	void SetSuccessText(const FText& InText);

private:
	bool IsPossibleOnClick() const;

	void SetDisplayEnchant();
	void SetDisplayMagicalForge();

	void GetItemNameText(const FText& InNameText, const Level InLevel, OUT FText& OutNameText);

private:
	UFUNCTION()
	void OnclickConfirm();
	UFUNCTION()
	void OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateTargetIcon(UGsUIIconBase* InIcon);
};
