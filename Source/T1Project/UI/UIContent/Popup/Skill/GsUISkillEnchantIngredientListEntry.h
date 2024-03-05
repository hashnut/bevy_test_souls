// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUISkillEnchantIngredientListEntry.generated.h"

class UTextBlock;
class UGsHorizontalBoxIconSelector;
class UGsUIIconItemInventory;

/**
 * 스킬 강화 팝업의 재료 리스트 아이템
 */
UCLASS()
class T1PROJECT_API UGsUISkillEnchantIngredientListEntry : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsHorizontalBoxIconSelector* _iconSelector;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCurrCount;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockNeedCount;

protected:
	virtual void NativeDestruct() override;

public:
	void SetData(ItemId InItemId, ItemAmount InNeedAmount);

private:
	void OnLongPressIcon(class UGsUIIconItem& InIcon);

	UGsUIIconItemInventory* GetIcon();
};
