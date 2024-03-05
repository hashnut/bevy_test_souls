//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "UI/UIContent/Tray/Guild/GsUIGuildStorageEntryBase.h"
//#include "GsUIGuildStorageEntryDistributionDetail.generated.h"
//
//
//class UGsHorizontalBoxIconSelector;
//class UTextBlock;
//class UWidgetSwitcher;
//class UGsUIIconItem;
//class UGsUIIconItemInventory;
//class FGsItem;
///**
// * 기사단 창고 분배 내역 리스트뷰 UI 아이템
// */
//UCLASS()
//class T1PROJECT_API UGsUIGuildStorageEntryDistributionDetail : public UGsUIGuildStorageEntryBase
//{
//	GENERATED_BODY()
//
//protected:
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsHorizontalBoxIconSelector* _iconSelector;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockGiver;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockReceiver;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockTime;
//
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UWidgetSwitcher* _switcherGiver;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UWidgetSwitcher* _switcherReceiver;
//
//protected:
//	UPROPERTY()
//	UGsUIIconItemInventory* _uiIcon;
//
//	FGsItem* _item = nullptr;
//
//protected:
//	virtual void NativeDestruct() override;
//	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
//
//protected:
//	void InvalidateIcon(FGsItem* InItem);
//	void OnClickItemIcon(UGsUIIconItem& InIcon);
//	void OnLongPressIcon(UGsUIIconItem& InIcon);
//
//public:
//	void InvalidateIcon();
//};
