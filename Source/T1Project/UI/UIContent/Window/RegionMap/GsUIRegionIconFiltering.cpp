#include "GsUIRegionIconFiltering.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Window/RegionMap/GsUIRegionIconFilteringItem.h"

#include "Management/ScopeGame/GsWorldMapManager.h"

#include "Map/GsMapIconType.h"

#include "Option/GsServerOption.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/Widget.h"

void UGsUIRegionIconFiltering::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIRegionIconFiltering::OnRefreshEntry);
}

void UGsUIRegionIconFiltering::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	UGsUIRegionIconFilteringItem* item = Cast<UGsUIRegionIconFilteringItem>(InEntry);

	if (item == nullptr)
	{
		return;
	}
	EGsRegionMapIconFiltering filteringType = (EGsRegionMapIconFiltering)InIndex;
	item->SetFilteringItemData(filteringType);
}

void UGsUIRegionIconFiltering::InitList()
{
	int listSize = (int)EGsRegionMapIconFiltering::MAX_COUNT;
	_scrollBoxHelper->RefreshAll(listSize);


}

void UGsUIRegionIconFiltering::NativeDestruct()
{
	// send server option
	// check: if changed value exist , server send
	FGsWorldMapManager* worldmapManager = GSWorldMap();
	FGsServerOption* serverOption = GServerOption();

	if(worldmapManager != nullptr && serverOption != nullptr)
	{ 
		if (true == worldmapManager->IsChangedServerOptionRegionMapFilterInfo())
		{
			serverOption->SendToServerGameOption();
		}
	}
	Super::NativeDestruct();
}

void UGsUIRegionIconFiltering::BeginDestroy()
{
	if (_scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIRegionIconFiltering::OnRefreshEntry);
	}
	_scrollBoxHelper = nullptr;
	
	Super::BeginDestroy();
}

void UGsUIRegionIconFiltering::NativeConstruct()
{
	Super::NativeConstruct();

	// backup server data
	GSWorldMap()->SaveServerOptionRegionMapFilterInfo();
}