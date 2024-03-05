// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "GsUIItemDefaultInfo.generated.h"

class FGsItem;
class UTextBlock;
class UPanelWidget;
class UGsSwitcherButton;
class UGsUIIconItemInventory;
/**
 * 아이콘, 이름, 타입, 안전강화횟수, 실드 등 표시
 */
UCLASS()
class T1PROJECT_API UGsUIItemDefaultInfo: public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelector;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UTextBlock* _tbName;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UTextBlock* _tbCategoryName;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UPanelWidget* _panelCountRoot;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UTextBlock* _tbSafeEnchantCount;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UTextBlock* _tbShieldCount;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	UGsSwitcherButton* _sBtnLock;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _vcItemPanel;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class USpacer* _spacerMagicalForge;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UImage* _imageMagicalForge;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockMagicalForge;

private:
	uint64 _ItemDBID = 0;
	bool _isUseLock = true;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public:
	void SetData(const FGsItem* InItem, bool bInHideButtons, bool InUseLock = true, bool bInItemCollectionMarking = false);
	void SetData(const CurrencyType InCurrencyType, const Currency InAmount, bool bInItemCollectionMarking = false);
	
	// 아이템 갯수 표시를 바꾸고 싶을 경우
	void SetItemAmount(int64 InAmount);

protected:
	UFUNCTION()
	void OnClickLock();

protected:
	void SetUIItemName(const FText& InNameText, Level InEnchantLevel, ItemGrade InGrade);

	UGsUIIconItemInventory* GetIcon();
};
