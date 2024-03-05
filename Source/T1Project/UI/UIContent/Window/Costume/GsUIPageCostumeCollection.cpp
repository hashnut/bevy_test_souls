// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPageCostumeCollection.h"
#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"
#include "Shared/Client/SharedEnums/SharedPassivityEnum.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"

#include "GsUICostumeCollectionEntry.h"
#include "GsUIListCostumeCollectionData.h"
#include "UTIL/GsTableUtil.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UI/UIContent/Popup/GsUIPopupFairyCollectionImpact.h"
#include "UI/UIContent/Popup/GsUIPopupCostumeCollection.h"
#include "UI/UIContent/Tray/GsUITrayCommonStatFilterInCategory.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "TextBlock.h"
#include "ListView.h"
#include "PanelWidget.h"
#include "../Management/ScopeGlobal/GsGameFlowManager.h"
#include "../GameFlow/GsGameFlowGame.h"
#include "../GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "../GameFlow/GameContents/GsContentsMode.h"
#include "../GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "../StatFilterSystem/GsStatFilterHandler.h"

void UGsUIPageCostumeCollection::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIPageCostumeCollection::NativeOnInitialized()
{
	_btnPrev->OnClicked.AddDynamic(this, &UGsUIPageCostumeCollection::OnPrev);
	_btnNext->OnClicked.AddDynamic(this, &UGsUIPageCostumeCollection::OnNext);
	_btnCollectionEffect->OnClicked.AddDynamic(this, &UGsUIPageCostumeCollection::OnClickProgressDetail);
	_btnStat->OnClicked.AddDynamic(this, &UGsUIPageCostumeCollection::OnClickStatFilter);

	_toggleGroupType.AddToggleWidget(_toggleAll);
	_toggleGroupType.AddToggleWidget(_toggleComplete);
	_toggleGroupType.AddToggleWidget(_toggleIncomplete);

	_toggleGroupType.OnSelectChanged.BindUObject(this, &UGsUIPageCostumeCollection::OnSelectedTypeChanged);

	_emptyPanel->SetVisibility(ESlateVisibility::Collapsed);
	_listViewCollections->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	_listViewCollections->OnEntryWidgetGenerated().AddLambda([this](UUserWidget& entry) {

		if (UGsUICostumeCollectionEntry* collectionEntry = Cast<UGsUICostumeCollectionEntry>(&entry))
		{
			collectionEntry->OnCollectionPopup.BindUObject(this, &UGsUIPageCostumeCollection::OnPopupCostumeEachCollection);
			if (!collectionEntry->IsValideCallback())
			{
				collectionEntry->SetSelectionCallback([this](UGsUIListCostumeCollectionData* data) {
					_selectedCollection = data != nullptr ? data->GetData() : nullptr;
					});
			}
		}
		});

	_listViewCollections->OnEntryWidgetReleased().AddLambda([this](UUserWidget& entry) {

		if (UGsUICostumeCollectionEntry* collectionEntry = Cast<UGsUICostumeCollectionEntry>(&entry))
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

void UGsUIPageCostumeCollection::NativeConstruct()
{
	_selectedStatSet.Empty();

	Super::NativeConstruct();
}

void UGsUIPageCostumeCollection::NativeDestruct()
{
	_selectedStatSet.Empty();

	Super::NativeDestruct();
}

void UGsUIPageCostumeCollection::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (_selectedType == -1)
		_toggleGroupType.SetSelectedIndex(0, true);

	/*for (auto widget : _listViewCollections->GetDisplayedEntryWidgets())
	{
		OnListItemScrolledView(widget);
	}*/
}

#pragma region IGsSelectedPanelInterface
void UGsUIPageCostumeCollection::InitializeTab(int32 tabIndex)
{
	IGsSelectedPanelInterface::InitializeTab(tabIndex);

	_selectedCollection.Reset();
	//_selectedType = toUnderSearchType(CostumeSearchType::NONE);
	_toggleGroupType.SetSelectedIndex(0);

	if (auto userData = GGameData()->GetUserData())
		_genderType = userData->mGender;

	SetRefreshCostumeItems();
}

void UGsUIPageCostumeCollection::ReleasedTab()
{
	CloseChildWidget();

	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupFairyCollectionProgress")).Get())
		popup->Close();
	if (auto popup = GUI()->GetWidgetByKey(TEXT("PopupCostumeCollection")).Get())
		popup->Close();
}

void UGsUIPageCostumeCollection::OnSelected()
{
	SetRefreshCostumeItems();
	UpdateRedDot();
}

void UGsUIPageCostumeCollection::OnUnselected()
{
	const auto& collection = GSCostume()->GetCostumeCollection();
	for (auto e : collection)
	{
		e->RemoveNewMark();
	}

	GSCostume()->DisableCostumeCollectionRedDot();
}

void UGsUIPageCostumeCollection::OnUpdateItemList(TArray<CostumeId>& arr)
{
	if (!_isSelectedTab) return;

	SetRefreshCostumeItems();
}

bool UGsUIPageCostumeCollection::CloseChildWidget()
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
void UGsUIPageCostumeCollection::OnSelectedTypeChanged(int32 index)
{
	_selectedType = index;
	SetRefreshCostumeItems();
}

void UGsUIPageCostumeCollection::SetIntoView(int32 InIndex)
{
	int32 maxCount = _listViewCollections->GetNumItems();
	
	InIndex = FMath::Clamp(InIndex, 0, maxCount);
	_listViewCollections->SetScrollOffset(StaticCast<float>(InIndex));

	SetListCountDisplay(InIndex);
}

void UGsUIPageCostumeCollection::SetListCountDisplay(int32 InIndex)
{
	int32 maxCount = _listViewCollections->GetNumItems();
	_currentCount = FMath::Min(InIndex, maxCount);

	const static int32 MIN_COUNT = 1;
	_btnPrev->SetIsEnabled(MIN_COUNT <= _currentCount);
	_btnNext->SetIsEnabled( (PAGE_OVER_COUNT + _currentCount) < maxCount );
	
	_textPage->SetText(FText::FromString(FString::Format(TEXT("{0} / {1}"), { maxCount > 0 ? _currentCount + 1 : 0, maxCount })));
}

void UGsUIPageCostumeCollection::OnCallbackStatFilterApply(const TArray<StatType>& InSelectedStatSet)
{
	_selectedStatSet = InSelectedStatSet;

	SetRefreshCostumeItems();
}

void UGsUIPageCostumeCollection::OnClickStatFilter()
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
		statfilterHandler->OpenWidget(GSCostume()->GetContentsStatAll().Array(), _selectedStatSet, [this](const TArray<StatType>& InData)
			{
				this->OnCallbackStatFilterApply(InData);
			}
		);
	}
}

void UGsUIPageCostumeCollection::OnPrev()
{
	SetIntoView(_currentCount - PAGE_OVER_COUNT);
}

void UGsUIPageCostumeCollection::OnNext()
{
	SetIntoView(_currentCount + PAGE_OVER_COUNT);
}

void UGsUIPageCostumeCollection::OnClickProgressDetail()
{
	if (UGsUIManager* uiManager = GUI())
	{
		TWeakObjectPtr<UGsUIPopupFairyCollectionImpact> weakPopup =
			Cast<UGsUIPopupFairyCollectionImpact>(uiManager->OpenAndGetWidget("PopupFairyCollectionProgress"));
		if (auto popup = weakPopup.Get())
		{
			TArray<TWeakPtr<FGsCostumeCollection>> completecCollections;
			GSCostume()->GetCollectionSearchItems(CostumeSearchType::Complete, FText::GetEmpty(), completecCollections);
			auto costumeCollections = GSCostume()->GetCostumeCollection();

			UGsUIPopupFairyCollectionImpact::PopupInitData initParam(
				PassivityOwnerType::COSTUME_COLLECTION, 
				completecCollections.Num(), 
				costumeCollections.Num());

			popup->InitializeData(&initParam);
		}
	}
}

void UGsUIPageCostumeCollection::SetRefreshCostumeItems()
{
	TArray<TWeakPtr<FGsCostumeCollection>> collections;
	GSCostume()->GetCollectionSearchItems(static_cast<CostumeSearchType>(_selectedType), _selectedStatSet, collections);

	TArray<UGsUIListCostumeCollectionData*> listItems;
	for (auto data : collections)
	{
		if (UGsUIListCostumeCollectionData* listItem = NewObject<UGsUIListCostumeCollectionData>())
		{
			listItem->SetItemData(data, true, _genderType);
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

void UGsUIPageCostumeCollection::UpdateRedDot()
{

}

void UGsUIPageCostumeCollection::OnPopupCostumeEachCollection(CostumeId id)
{
	if (auto PopupCollection = Cast<UGsUIPopupCostumeCollection>(GUI()->OpenAndGetWidget(TEXT("PopupCostumeCollection"))))
	{
		CreatureGenderType genderType{ CreatureGenderType::ALL };
		if (auto userData = GGameData()->GetUserData())
			genderType = userData->mGender;

		UGsUIPopupCostumeCollection::PopupInitData param = { id, genderType };
		PopupCollection->InitializeData(&param);
	}
}