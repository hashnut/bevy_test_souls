//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "UI/UIContent/Tray/Guild/GsUIGuildStoragePageBase.h"
//#include "GsUIGuildStoragePageAcquireDetail.generated.h"
//
//
//class UWidgetSwitcher;
//class UGsGuildStorageAcquireDetailListItem;
///**
// * 기사단 창고 획득 내역 페이지
// */
//UCLASS()
//class T1PROJECT_API UGsUIGuildStoragePageAcquireDetail : public UGsUIGuildStoragePageBase
//{
//	GENERATED_BODY()
//
//protected:
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UWidgetSwitcher* _switcherList;
//
//protected:
//	TArray<UGsGuildStorageAcquireDetailListItem*> _dataList;
//
//protected:
//	virtual void NativeConstruct() override;
//	virtual void NativeDestruct() override;
//
//protected:
//	// IGsInvalidateUIInterface override
//	virtual void InvalidateAllInternal() override;
//
//public:
//	virtual void Show() override;
//	virtual void Hide() override;
//};
