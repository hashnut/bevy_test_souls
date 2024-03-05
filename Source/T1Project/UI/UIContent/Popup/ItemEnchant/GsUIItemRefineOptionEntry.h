// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
//#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "GsUIItemRefineOptionEntry.generated.h"

class UTextBlock;
class UVerticalBox;
class UGsDynamicPanelSlotHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIItemRefineOptionEntry : public UUserWidget
{
	GENERATED_BODY()

protected:
	// 胶泡 府胶飘 悼利 积己 棺 包府
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _refineStatListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _refineStatListPanelSlotHelper;
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _defaultStatListRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UTextBlock* _titleText;

private:
	TMap<StatType, TPair<int32, int32>> _defaultStatPairDataMap;

public:
	void SetStatData(uint64 InTargetItemDBID, const int32 InOptionID, const ItemCategorySub InCategorySub, int32 inIndex);


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;


	UFUNCTION()
	void ItemListRefreshEntry(int32 InIndex, UWidget* InEntry);
};
