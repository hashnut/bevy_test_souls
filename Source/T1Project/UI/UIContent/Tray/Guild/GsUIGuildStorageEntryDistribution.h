//// Fill out your copyright notice in the Description page of Project Settings.
//
//#pragma once
//
//#include "CoreMinimal.h"
//#include "UI/UIContent/Tray/Guild/GsUIGuildStorageEntryBase.h"
//#include "GsUIGuildStorageEntryDistribution.generated.h"
//
//
//class UTextBlock;
//class UImage;
//class UGsGuildStorageDistributionListItem;
///**
// * 기사단 창고 기사단원 리스트뷰 UI 아이템
// */
//UCLASS()
//class T1PROJECT_API UGsUIGuildStorageEntryDistribution : public UGsUIGuildStorageEntryBase
//{
//	GENERATED_BODY()
//
//protected:
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockLevel;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockName;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockGrade;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockWeekly;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UTextBlock* _textBlockAccumulation;
//	UPROPERTY(BlueprintReadOnly, Category = "GsUI", meta = (BindWidget))
//	UImage* _imageSelected;
//
//protected:
//	UGsGuildStorageDistributionListItem* _item = nullptr;
//	bool _bIsSelected = false;
//
//protected:
//	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
//	virtual void NativeOnItemSelectionChanged(bool bIsSelected);
//
//public:
//	void SetIsSelected(bool bInIsSelected);
//	bool GetIsSelected() const { return _bIsSelected; }
//};
