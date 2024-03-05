// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DataSchema/Fairy/GsSchemaCeilingItemData.h"
#include "GsUIListEntryAcqItemPanel.generated.h"

class UPanelWidget;
class UGsDynamicPanelSlotHelper;

/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIListEntryAcqItemPanel : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI")
	FText _textResCountTitle;

	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UPanelWidget* _itemPanel;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _entryWidgetClass;

	UPROPERTY()
	UGsDynamicPanelSlotHelper* _panelWidgetHelper;

protected:
	TArray<FGsSchemaCeilingItemData> _dataList;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnRefreshEntry(int32 index, UWidget* entry);

public:
	void ClearData();
	void SetData(int index, int composeResCount, FGsSchemaCeilingItemData& data);
	void RefreshUI();
};
