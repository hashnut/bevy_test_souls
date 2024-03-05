// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIPopup.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Client/SharedEnums/SharedStatEnum.h"
#include "GsUIPopupItemRefineStatList.generated.h"

class UVerticalBox;
class UGsButton;
class UGsDynamicPanelSlotHelper;
/**
 * 
 */
UCLASS()
class T1PROJECT_API UGsUIPopupItemRefineStatList : public UGsUIPopup
{
	GENERATED_BODY()

protected:
	// 제련 스텟 및 보너스 스텟 리스트 동적 생성 및 관리
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _refineOptionListEntryWidgetClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GsUI")
	TSubclassOf<UUserWidget> _bonusStatListEntryWidgetClass;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _refineOptionListPanelSlotHelper;
	UPROPERTY()
	UGsDynamicPanelSlotHelper* _bonusStatListPanelSlotHelper;
	

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _refineOptionListRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _bonusStatRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UVerticalBox* _bonusStatListRootPanel;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	UGsButton* _btnClose;


private:
	ItemId _targetItemTid = 0;
	uint64 _targetItemDBID = 0;
	ItemCategorySub _materialCategorySub;
	TArray<int32> _refineOptionIdList;
	TMap<StatType, TPair<int32, int32>> _bonusStatDataMap;

public:
	void OpenExpectStatList(const uint64 InEquipItemDBID, const ItemCategorySub InRefineOptionType);
	//void OpenExpectStatList(const uint64 InEquipItemDBID, const uint64 InMaterialItemDBID);


protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;

protected:
	virtual void OnInputOk() override;
	virtual void OnInputCancel() override;

public:
	ItemCategorySub GetIngredientCategorySub() { return _materialCategorySub; }
	ItemId GetTargetItemTid() { return _targetItemTid; }

protected:
	UFUNCTION()
	void OnClickClose();
	UFUNCTION()
	void ItemListRefreshEntry(int32 InIndex, UWidget* InEntry);
	UFUNCTION()
	void BonusItemListRefreshEntry(int32 InIndex, UWidget* InEntry);
	
};
