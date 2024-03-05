// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUISummonDetailGradeContents.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Fairy/GsFairyData.h"
#include "Costume/GsCostumeData.h"

#include "UI/UIContent/Popup/SummonDetailInfo/GsUISummonItemPortrait.h"
#include "UI/UIContent/Popup/GsUIPopupFairyCollection.h"
#include "UI/UIContent/Popup/GsUIPopupCostumeCollection.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Components/WidgetSwitcher.h"

void UGsUISummonDetailGradeContents::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUISummonDetailGradeContents::OnCreateSummonItemEntry);
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUISummonDetailGradeContents::OnRefreshSummonItemEntry);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}
void UGsUISummonDetailGradeContents::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<UWidget*> childList = _candidatePortraits->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUISummonItemPortrait>())
		{
			child->RemoveFromParent();
		}
	}

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryPortrait, _candidatePortraits);
	_scrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUISummonDetailGradeContents::OnCreateSummonItemEntry);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUISummonDetailGradeContents::OnRefreshSummonItemEntry);
}

void UGsUISummonDetailGradeContents::NativeDestruct()
{
	_data.Reset();

	Super::NativeDestruct();
}

void UGsUISummonDetailGradeContents::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto userData = GGameData()->GetUserData())
		_genderType = userData->mGender;
}

void UGsUISummonDetailGradeContents::SetData(ItemGrade grade, TArray<FGsSummonChangeData>& data)
{
	_data.Reset();
	_data.Append(data);

	_gradeColor = FGsUIColorHelper::GetColorItemGrade(grade);
	_gradeName = FText::GetEmpty();
	switch (grade)
	{
	case ItemGrade::NORMAL:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Normal"), _gradeName);	break;
	case ItemGrade::MAGIC:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Magic"), _gradeName);	break;
	case ItemGrade::RARE:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Rare"), _gradeName);	break;
	case ItemGrade::EPIC:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Epic"), _gradeName);	break;
	case ItemGrade::UNIQUE:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Unique"), _gradeName);	break;
	case ItemGrade::LEGEND:	FText::FindText(TEXT("ItemUIText"), TEXT("Grade_Legend"), _gradeName);	break;
	default: break;
	}
	_selectGrade->SetActiveWidgetIndex(static_cast<int32>(grade) - 1);
	_scrollBoxHelper->RefreshAll(data.Num());
}

void UGsUISummonDetailGradeContents::OnCreateSummonItemEntry(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUISummonItemPortrait>(InEntry))
	{
		entry->OnItemSelected.BindUObject(this, &UGsUISummonDetailGradeContents::OnPopupCollection);
	}
}

void UGsUISummonDetailGradeContents::OnRefreshSummonItemEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUISummonItemPortrait>(InEntry))
	{
		if (_data.IsValidIndex(index))
		{
			entry->SetData(_data[index].data, _data[index].perProbability, _genderType);
			entry->RefreshUI();
		}
	}
}

void UGsUISummonDetailGradeContents::OnPopupCollection(TWeakPtr<IGsBasePortrait> data)
{
	if (auto useData = data.Pin())
	{
		auto type = useData->GetType();
		if (type == IGsBasePortrait::PortraitType::Fairy)
		{
			if (auto fairyData = StaticCastSharedPtr<FGsFairyData>(useData))
			{
				if (auto PopupCollection = Cast<UGsUIPopupFairyCollection>(GUI()->OpenAndGetWidget(TEXT("PopupFairyCollection"))))
				{
					UGsUIPopupFairyCollection::PopupInitData param = { fairyData->_id, fairyData->GetCurrentLevel(), false };
					PopupCollection->InitializeData(&param);
				}
			}
		}
		else if (type == IGsBasePortrait::PortraitType::Costume)
		{
			if (auto costumeData = StaticCastSharedPtr<FGsCostumeData>(useData))
			{
				if (auto PopupCollection = Cast<UGsUIPopupCostumeCollection>(GUI()->OpenAndGetWidget(TEXT("PopupCostumeCollection"))))
				{
					UGsUIPopupCostumeCollection::PopupInitData param = { costumeData->_id, _genderType, false };
					PopupCollection->InitializeData(&param);
				}
			}
		}
	}
}
