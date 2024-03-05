// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/ItemCollection/GsUIItemCollectionApplyStatus.h"

#include "UMG/Public/Components/TextBlock.h"

void UGsUIItemCollectionApplyStatus::SetData(const FText& InText)
{
	_textApplyState->SetText(InText);
}

void UGsUIItemCollectionApplyStatus::SetData(const StatType& InStatType)
{
	//FGsStatHelper
}