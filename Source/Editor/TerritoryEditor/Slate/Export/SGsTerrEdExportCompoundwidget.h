
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SDockTab.h"
#include "SDockableTab.h"
#include "SDockTabStack.h"
#include "SlateApplication.h"

class SWindow;

class SGsTerrEdExportCompoundWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SGsTerrEdExportCompoundWidget){}
	SLATE_END_ARGS()

private:
	float _height;
	float _descriptionColumnSize;
	float _excuteColumnSize;		

public:
	void Construct(const FArguments& inArgs);

public:
	TSharedRef<SDockTab> HandleTabManagerSpawnTab(const FSpawnTabArgs& inArgs
		, const FName inTabIdentifier);
	TSharedPtr<SWidget> CreateUtilWidget(FString inDescription, FOnClicked InOnClicked);
};