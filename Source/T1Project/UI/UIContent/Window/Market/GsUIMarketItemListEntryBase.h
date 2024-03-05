// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "GsUIMarketItemListEntryBase.generated.h"

class UTextBlock;

/**
 * 거래소 아이템 리스트 기본 클래스. 아이콘, 이름(등급별 색상)등 공통기능 정의
 * - 주의: ListView의 OnItemClicked 이벤트를 받기 위해서는 버튼으로 이벤트를 막으면 안된다...
 */
UCLASS(Abstract)
class UGsUIMarketItemListEntryBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockName;
	UPROPERTY(BlueprintReadOnly, Category = "ItemIconSelector", meta = (BindWidget))
	class UGsHorizontalBoxIconSelector* _iconSelector;

protected:
	TWeakObjectPtr<UGsUIIconItem> _iconPtr;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	virtual void OnLongPressIcon(class UGsUIIconItem& InIcon);	
	virtual class FGsItem* GetItem();

public:
	virtual void RefreshAll() {}
	virtual bool IsSlotLongPressed() const { return false; }

protected:
	void OpenItemDetailInfo(class FGsItem* InItem);
	void SetUIItemName(const FText& InNameText, Level InEnchantLevel = 0, ItemGrade InGrade = ItemGrade::NONE);	
	void SetUIItemStat(OUT UTextBlock* OutTextBlock);	

protected:	
	bool GetItemStatText(OUT FText& OutText);

private:
	UGsUIIconItem* GetIcon();	
};
