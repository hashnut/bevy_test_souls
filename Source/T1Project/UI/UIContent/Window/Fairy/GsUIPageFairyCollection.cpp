// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPageFairyCollection.h"
#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsFairyManager.h"

#include "GsUIFairyCollectionEntry.h"
#include "GsUIListItemFairyCollectionData.h"
#include "UTIL/GsTableUtil.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UI/UIContent/Popup/GsUIPopupFairyCollectionImpact.h"
#include "UI/UIContent/Popup/GsUIPopupFairyCollection.h"
#include "UI/UIContent/Popup/GsUIPopupCollectionStatFilter.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Tray/GsUITrayCommonStatFilterInCategory.h"
#include "TextBlock.h"
#include "ListView.h"
#include "PanelWidget.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../StatFilterSystem/GsStatFilterHandler.h"



void UGsUIPageFairyCollection::BeginDestroy()
{
	_selectedStatType.Empty();

	Super::BeginDestroy();
}

void UGsUIPageFairyCollection::NativeOnInitialized()
{
	_btnPrev->OnClicked.AddDynamic(this, &UGsUIPageFairyCollection::OnPrev);
	_btnNext->OnClicked.AddDynamic(this, &UGsUIPageFairyCollection::OnNext);
	_btnCollectionEffect->OnClicked.AddDynamic(this, &UGsUIPageFairyCollection::OnClickProgressDetail);
	_btnStat->OnClicked.AddDynamic(this, &UGsUIPageFairyCollection::OnClickStatFilter);

	_toggleGroupType.AddToggleWidget(_toggleAll);
	_toggleGroupType.AddToggleWidget(_toggleComplete);
	_toggleGroupType.AddToggleWidget(_toggleIncomplete);

	_toggleGroupType.OnSelectChanged.BindUObject(this, &UGsUIPageFairyCollection::OnSelectedTypeChanged);

	_emptyPanel->SetVisibility(ESlateVisibility::Collapsed);
	_listViewCollections->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	_listViewCollections->OnEntryWidgetGenerated().AddLambda([this](UUserWidget& entry) {

		if (UGsUIFairyCollectionEntry* collectionEntry = Cast<UGsUIFairyCollectionEntry>(&entry))
		{
			collectionEntry->OnCollectionPopup.BindUObject(this, &UGsUIPageFairyCollection::OnPopupFairyEachCollection);

			if (!collectionEntry->IsValideCallback())
			{
				collectionEntry->SetSelectionCallback([this](UGsUIListItemFairyCollectionData* data) {
						_selectedCollection = data != nullptr ? data->GetData() : nullptr;
					});
			}
		}
		});

	_listViewCollections->OnEntryWidgetReleased().AddLambda([this](UUserWidget& entry) {

		if (UGsUIFairyCollectionEntry* collectionEntry = Cast<UGsUIFairyCollectionEntry>(&entry))
		{
			if (collectionEntry->IsValideCallback())
			{
				collectionEntry->SetSelectionCallback(nullptr);
			}
		}
		});

	_listViewCollections->OnListViewScrolled().AddLambda([this](float InOffset, float InDistanceRemaining)
		{
			SetListCountDisplay(StaticCast<int32>(InOffset));
		}
	);

	Super::NativeOnInitialized();
}

void UGsUIPageFairyCollection::NativeConstruct()
{
	Super::NativeConstruct();

	_selectedStatType.Empty();
}

void UGsUIPageFairyCollection::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIPageFairyCollection::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_selectedType == -1)
		_toggleGroupType.SetSelectedIndex(0, true);
}

#pragma region IGsSelectedPanelInterface
void UGsUIPageFairyCollection::InitializeTab(int32 tabIndex)
{
	IGsSelectedPanelInterface::InitializeTab(tabIndex);

	_selectedCollection.Reset();
	_selectedType = toUnderSearchType(FairySearchType::NONE);
	_toggleGroupType.SetSelectedIndex(0);

	SetRefreshFairyItems();
}

void UGsUIPageFairyCollection::ReleasedTab()
{
	CloseChildWidget();

	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupFairyCollectionProgress")).Get())
		popup->Close();
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupFairyCollection")).Get())
		popup->Close();
}

void UGsUIPageFairyCollection::OnSelected()
{
	SetRefreshFairyItems();
	UpdateRedDot();
}

void UGsUIPageFairyCollection::OnUnselected()
{
	const auto& collection = GSFairy()->GetFairyCollection();
	for(auto e : collection)
	{
		e->RemoveNewMark();
	}

	GSFairy()->DisableFairyCollectionRedDot();
}

void UGsUIPageFairyCollection::OnUpdateItemList(TArray<FairyId>& arr)
{
	if (!_isSelectedTab) return;
	SetRefreshFairyItems();

}

bool UGsUIPageFairyCollection::CloseChildWidget()
{
	if (GUI()->IsActiveWidget(TEXT("TrayCommonStatFilterInCateogry")))
	{
		TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->GetWidgetByKey(TEXT("TrayCommonStatFilterInCateogry"), true);
		if (widget.IsValid())
		{
			widget->Close();

			return true;
		}
	}

	return false;
}

#pragma endregion
void UGsUIPageFairyCollection::OnSelectedTypeChanged(int32 index)
{
	_selectedType = index;
	SetRefreshFairyItems();
}

void UGsUIPageFairyCollection::SetIntoView(int32 InIndex)
{
	int32 maxCount = _listViewCollections->GetNumItems();

	InIndex = FMath::Clamp(InIndex, 0, maxCount);
	_listViewCollections->SetScrollOffset(StaticCast<float>(InIndex));

	SetListCountDisplay(InIndex);
}

void UGsUIPageFairyCollection::SetListCountDisplay(int32 InIndex)
{
	int32 maxCount = _listViewCollections->GetNumItems();
	_currentCount = FMath::Min(InIndex, maxCount);

	const static int32 MIN_COUNT = 1;
	_btnPrev->SetIsEnabled(MIN_COUNT <= _currentCount);
	_btnNext->SetIsEnabled((PAGE_OVER_COUNT + _currentCount) < maxCount);

	_textPage->SetText(FText::FromString(FString::Format(TEXT("{0} / {1}"), { maxCount > 0 ? _currentCount + 1 : 0, maxCount })));
}

void UGsUIPageFairyCollection::OnClickStatFilter()
{
	FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
	FGsContentsManagerGame* contents = (gameFlow) ? gameFlow->GetContentsManagerGame() : nullptr;
	if (nullptr == contents)
	{
		return;
	}
	FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
	FGsStatFilterHandler* statfilterHandler = (hud) ? hud->GetStatFilterHandler() : nullptr;

	if (statfilterHandler)
	{
		statfilterHandler->OpenWidget(GSFairy()->ContentsStatAll().Array(), _selectedStatType, [this](const TArray<StatType>& InData)
			{
				this->OnCallbackStatFilterApply(InData);
			}
		);
	}
}

void UGsUIPageFairyCollection::OnCallbackStatFilterApply(const TArray<StatType>& InSelectedType)
{
	_selectedStatType = InSelectedType;

	SetRefreshFairyItems();
}

void UGsUIPageFairyCollection::OnPrev()
{
	SetIntoView(_currentCount - PAGE_OVER_COUNT);
}

void UGsUIPageFairyCollection::OnNext()
{
	SetIntoView(_currentCount + PAGE_OVER_COUNT);
}

void UGsUIPageFairyCollection::OnClickProgressDetail()
{
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIPopupFairyCollectionImpact> weakPopup = 
			Cast<UGsUIPopupFairyCollectionImpact>(uiManager->OpenAndGetWidget("PopupFairyCollectionProgress"));
		if (auto popup = weakPopup.Get())
		{
			TArray<TWeakPtr<FGsFairyCollection>> completecCollections;
			GSFairy()->GetCollectionSearchItems(FairySearchType::Complete, FText::GetEmpty(), completecCollections);
			auto costumeCollections = GSFairy()->GetFairyCollection();

			UGsUIPopupFairyCollectionImpact::PopupInitData initParam(
				PassivityOwnerType::FAIRY_COLLECTION, 
				completecCollections.Num(), 
				costumeCollections.Num());

			popup->InitializeData(&initParam);
		}
	}
}

void UGsUIPageFairyCollection::SetRefreshFairyItems()
{
	TArray<TWeakPtr<FGsFairyCollection>> collections;
	GSFairy()->GetCollectionSearchItems(static_cast<FairySearchType>(_selectedType), _selectedStatType, collections);

	TArray<UGsUIListItemFairyCollectionData*> listItems;
	for (auto data : collections)
	{
		if (UGsUIListItemFairyCollectionData* listItem = NewObject<UGsUIListItemFairyCollectionData>())
		{
			listItem->SetItemData(data);
			listItems.Add(listItem);
		}
	}

	if (_listViewCollections)
	{
		_listViewCollections->ClearListItems();
		_listViewCollections->SetListItems(listItems);
		_listViewCollections->ScrollToTop();
		_listViewCollections->SetScrollbarVisibility(ESlateVisibility::Collapsed);

		_emptyPanel->SetVisibility(listItems.Num() > 0 ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	}

	const static int32 DEFAULT_INDEX = 0;
	SetListCountDisplay(DEFAULT_INDEX);
}


void UGsUIPageFairyCollection::UpdateRedDot()
{

}


void UGsUIPageFairyCollection::OnPopupFairyEachCollection(FairyId id)
{
	if (auto PopupCollection = Cast<UGsUIPopupFairyCollection>(GUI()->OpenAndGetWidget(TEXT("PopupFairyCollection"))))
	{
		UGsUIPopupFairyCollection::PopupInitData param = { id };
		PopupCollection->InitializeData(&param);
	}
}