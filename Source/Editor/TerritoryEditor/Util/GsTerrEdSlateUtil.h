
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SBoxPanel.h"

class FGsTerrEdSlateUtil
{
public:
	static TSharedPtr<SHorizontalBox> CreateFieldText(const FString& inFieldName, FMargin inPadding=FMargin(0.0f, 2.5f, 2.0f, 2.5f));
	static void SetShowOnlySelectActor(bool isActive);
	static bool IsShowOnlySelectActor();
};

