// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "GsUIAutoDepositItemList.generated.h"


class UGsUIIconItem;
/**
 * 창고 자동 보관 가능한 아이템 목록
 */
UCLASS()
class T1PROJECT_API UGsUIAutoDepositItemList : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UWidgetSwitcher* _switcherItemList;
	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
	class UGsWrapBoxIconSelector* _iconSelector;
	UPROPERTY()
	class UGsDynamicIconSlotHelper* _iconSlotHelper;

private:
	TArray<ItemId> _itemIdList;
	TMap<ItemId, bool> _itemSelectMap;
	bool _isSelectRefresh = false;

public:
	DECLARE_DELEGATE_OneParam(FOnClickedIcon, UGsUIIconItem&);
	FOnClickedIcon OnClickedIcon;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetItemList(const TArray<ItemId>& InItemIdList, const TMap<ItemId, bool>& InItemSelectMap);
	void SelectItem(ItemId InItemId, bool InIsSelect);

	void InvalidateItemListSelect(const TMap<ItemId, bool>& InItemSelectMap);

	void InvalidateAll();

private:
	void OnClickIcon(UGsUIIconItem& InIcon);
	void OnLongPressIcon(UGsUIIconItem& InIcon);

	UFUNCTION()
	void OnRefreshIcon(int32 InIndex, class UGsUIIconBase* InIcon);

};
