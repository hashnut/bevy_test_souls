//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
//#include "UI/UILib/Interface/GsInvalidateUIInterface.h"
//#include "GsUIGuildStoragePageBase.generated.h"
//
//
//class UListView;
///**
// * 기사단 창고 페이지 기본 클래스
// */
//UCLASS()
//class T1PROJECT_API UGsUIGuildStoragePageBase : public UUserWidget, public IGsInvalidateUIInterface
//{
//	GENERATED_BODY()
//
//protected:
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UListView* _listView;
//
//protected:
//	bool _isOpened = false;
//
//protected:
//	// UUserWidget override
//	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
//
//protected:
//	// IGsInvalidateUIInterface override
//	virtual void InvalidateAllInternal() override {}
//
//public:
//	virtual void Show() { _isOpened = true; }
//	virtual void Hide() { _isOpened = false; }
//
//};
