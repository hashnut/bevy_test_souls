// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUISealMaterialListitem.generated.h"

class UGsButton;
class UGsDynamicIconSlotHelper;
class UGsHorizontalBoxIconSelector;
class UGsUIIconBase;
class UGsUIIconItem;
class UImage;
class UTextBlock;
class UWidget;
class UGsUITooltipDesc;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUISealMaterialListitem : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

public:

	struct Parameters
	{
		ItemId _itemId;
		Currency _requireAmount;
		Currency _ownedAmount;
		bool _isEnoughCostElement;
	};

protected:
	/** 좌측 아이템 아이콘 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;

	/** 아이콘 슬롯 헬퍼 */
	UPROPERTY()
	UGsDynamicIconSlotHelper* _slotHelperIcon;
	
	/** 아이템 이름 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemName;

	/** "현재 수량/요구 수량" 텍스트 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _itemAmount;

	/** 툴팁 버튼 */
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _materialItemInfoBtn;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsUITooltipDesc* _tooltip;

	/** 세팅 값 파라미터 */
	Parameters _parameters;

	/************************************************************************/
	/* Overrides                                                            */
	/************************************************************************/
protected:
	void NativeOnInitialized() override;

	void NativeDestruct() override;

	/************************************************************************/
	/* Setter                                                               */
	/************************************************************************/
public:
	void SetParameters(Parameters InParameters);


	/************************************************************************/
	/* Event                                                                */
	/************************************************************************/
private:
	UFUNCTION()
	void OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon);

	void OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon);

	UFUNCTION()
	void OnPressedMaterialTooltipButton();
};
