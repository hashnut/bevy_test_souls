//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "GsUIGuildStorageEntryDistribution.h"
//#include "Components/TextBlock.h"
//#include "Components/Image.h"
//#include "Guild/GsGuildStorageData.h"
//#include "Guild/GsGuildHelper.h"
//#include "UI/UIControlLib/ContentWidget/GsButton.h"
//#include "UI/UIContent/Tray/Guild/GsGuildStorageDistributionListItem.h"
//
//
//void UGsUIGuildStorageEntryDistribution::NativeOnListItemObjectSet(UObject* ListItemObject)
//{
//	_item = Cast<UGsGuildStorageDistributionListItem>(ListItemObject);
//	if (nullptr == _item)
//		return;
//	
//	FGsGuildStorageMemberData* data = _item->GetData();
//	if (nullptr == data)
//		return;
//
//	_textBlockLevel->SetText(FText::FromString(FString::Format(TEXT("Lv.{0}"), { data->_level })));
//	_textBlockName->SetText(FText::FromString(data->_userName));
//	FText textGrade;
//	if (false == FGsGuildHelper::GetMemberGradeText(data->_memberGrade, textGrade))
//	{
//		textGrade = FText::GetEmpty();
//	}
//	_textBlockGrade->SetText(textGrade);
//	_textBlockWeekly->SetText(FText::AsNumber(data->_weeklyContribution));
//	_textBlockAccumulation->SetText(FText::AsNumber(data->_accumContribution));
//
//	SetIsSelected(_item->GetIsSelected());
//}
//
//void UGsUIGuildStorageEntryDistribution::NativeOnItemSelectionChanged(bool bIsSelected)
//{
//	if (nullptr == _item)
//	{
//		SetIsSelected(bIsSelected);
//	}
//	else
//	{
//		SetIsSelected(_item->GetIsSelected());
//	}
//}
//
//void UGsUIGuildStorageEntryDistribution::SetIsSelected(bool bInIsSelected)
//{
//	_imageSelected->SetVisibility(bInIsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
//}
