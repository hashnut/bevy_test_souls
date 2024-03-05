// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/UIControlLib/Interface/GsEntryInterface.h"
#include "GsUIHUDQuestObjectiveSlot.generated.h"

class UWidgetSwitcher;
class USizeBox;
class UTextBlock;
/**
 *  Äù½ºÆ® ¿ÀºêÁ§Æ¼ºê ½½·Ô
 */
UCLASS()
class T1PROJECT_API UGsUIHUDQuestObjectiveSlot : public UUserWidget, public IGsEntryInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _stateTypeSwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _readySwicher;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _descText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _countText;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class USizeBox* _progressingSizeBox;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class USizeBox* _completeSizeBox;

public:
	void Update(bool inComplete, bool inIsReady, FText& inDescText, FText& inCountText);
};