// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"

#include "Management/ScopeGame/GsItemEnchantManager.h"

#include "GsUIEnchantRateListEntry.generated.h"


class UTextBlock;
/**
 * 장비 강화 확률 표시 툴팁 슬롯
 */
UCLASS()
class T1PROJECT_API UGsUIEnchantRateListEntry : public UUserWidget,
	public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockTitle;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockCurrent;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockExpect;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _textBlockPercent;

public:
	void SetData(FGsItemEnchantManager::DisplayRateInfo InDisplayInfo);

	void SetTitleText(ItemEnchantResult InResult);
	void SetCurrentText(const FText& InText);
	void SetExpectText(const FText& InText);
	void SetPercentText(int32 InProbability); // InProbability: 만분률 값
};
