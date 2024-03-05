
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SDockTab.h"
#include "SDockableTab.h"
#include "SDockTabStack.h"
#include "SlateApplication.h"
#include "GsTerritoryLevelManager.h"

class SWindow;

class SGsTerrEdToolLevelViewSettingCompoundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdToolLevelViewSettingCompoundWidget){}
	SLATE_END_ARGS()

private:
	TArray<TSharedPtr<FString>> _showTypeList;

private:
	float _height;
	float _descriptionColumnSize;
	float _excuteColumnSize;	
	float _padding;

public:
	void Construct(const FArguments& inArgs);

public:
	TSharedRef<SDockTab> HandleTabManagerSpawnTab(const FSpawnTabArgs& inArgs
		, const FName inTabIdentifier);
	TSharedPtr<SWidget> CreateUtilWidget(FString inDescription, FOnClicked InOnClicked);	
	TSharedPtr<SWidget> CreateWidgetDistance();
	TSharedPtr<SWidget> CreateWidgetShow();
	TSharedPtr<SWidget> CreateShapeDistance();
	TSharedPtr<SWidget> CreateShapeShow();
	TSharedPtr<SWidget> CreateSightRadiusDistance();
	TSharedPtr<SWidget> CreateSightShow();
	TSharedPtr<SWidget> CreateTransparentFloorWidget();
	TSharedPtr<SWidget> CreateTransparentFloorValueWidget();
	TSharedPtr<SWidget> CreateCheckNavmeshWidget();
	TSharedPtr<SWidget> CreateScalabilityQualityLowWidget();

public:
	TSharedPtr<FString> FindShowTypeString(EGsTerrEdShowType inShowType);
	void WidgetShowTypeChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	void ShapeShowTypeChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	void SightRadiusTypeChanged(TSharedPtr<FString> InItem, ESelectInfo::Type InSeletionInfo);
	TSharedRef<SWidget> GenerateComboWidget(TSharedPtr<FString> InItem);
	TOptional<float> GetTransparentValue() const;
	void OnSetransparentValue(float inValue, ETextCommit::Type inType){ FGsTerritoryLevelManager::_transparentFloorAlpha = inValue; }
};