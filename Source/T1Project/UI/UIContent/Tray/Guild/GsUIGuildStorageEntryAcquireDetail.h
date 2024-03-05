//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "UI/UIContent/Tray/Guild/GsUIGuildStorageEntryBase.h"
//#include "GsUIGuildStorageEntryAcquireDetail.generated.h"
//
//
//class UGsHorizontalBoxIconSelector;
//class UTextBlock;
//class UGsUIIconItem;
//class UGsUIIconItemInventory;
//class FGsItem;
///**
// * 기사단 창고 획득 내역 리스트뷰 UI 아이템
// */
//UCLASS()
//class T1PROJECT_API UGsUIGuildStorageEntryAcquireDetail : public UGsUIGuildStorageEntryBase
//{
//	GENERATED_BODY()
//
//protected:
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UGsHorizontalBoxIconSelector* _iconSelector;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockHistory;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockTime;
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
