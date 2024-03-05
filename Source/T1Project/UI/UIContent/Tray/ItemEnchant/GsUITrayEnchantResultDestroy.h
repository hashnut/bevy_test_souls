// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUITray.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUITrayEnchantResultDestroy.generated.h"

class UGsButton;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUITrayEnchantResultDestroy : public UGsUITray
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnConfirm;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _targetItemIconSelector;
	UPROPERTY()
	UGsDynamicIconSlotHelper* _targetIconSlotHelper;

private:
	float _currentTime = 0.f;
	float _lockMaxTime = 0.f;
	ItemId _itemTid = 0;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void SetDisplayItem(ItemId InItemTid, float InLockMaxTime);

private:
	bool IsPossibleOnClick() const;

private:
	UFUNCTION()
	void OnclickConfirm();
	UFUNCTION(BlueprintCallable)
	void OnDissolveEffect();
	UFUNCTION()
	void OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon);
	UFUNCTION()
	void OnCreateTargetIcon(UGsUIIconBase* InIcon);
	
};
