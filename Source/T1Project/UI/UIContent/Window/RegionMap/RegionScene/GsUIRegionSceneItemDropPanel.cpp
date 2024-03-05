#include "GsUIRegionSceneItemDropPanel.h"
#include "UI/UIContent/Popup/ItemDropNavigator/GsUIItemDropListContainer.h"
#include "GsUIRegionSceneItemDropEntry.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "Components/TextBlock.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "Components/ScrollBox.h"

void UGsUIRegionSceneItemDropPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelScrollRoot);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIRegionSceneItemDropPanel::OnRefreshEntry);

	_closeButton->OnClicked.AddDynamic(this, &UGsUIRegionSceneItemDropPanel::OnClickClose);
}

void UGsUIRegionSceneItemDropPanel::NativeConstruct()
{
	Super::NativeConstruct();

	_scrollBox->SetScrollBarVisibility(ESlateVisibility::SelfHitTestInvisible);
	_scrollBox->SetScrollOffset(0);
}

void UGsUIRegionSceneItemDropPanel::NativeDestruct()
{
	_toggleGroup.Clear();

	Super::NativeDestruct();
}

void UGsUIRegionSceneItemDropPanel::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIRegionSceneItemDropPanel::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	_toggleGroup.Clear();

	Super::BeginDestroy();
}

void UGsUIRegionSceneItemDropPanel::InvalidateAllInternal()
{
	_slotHelper->RefreshAll(_dataArray.Num());

	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_panelScrollRoot);
	// 선택을 해제 시켜놓기 위함
	_toggleGroup.SetSelectedIndex(-1);
}

void UGsUIRegionSceneItemDropPanel::SetMonsterData(const TArray<FGsSchemaWorldMapSpoilsMonsterLevel>& inData)
{
	_dataArray = inData;
	// 드랍 리스트 채우기
	InvalidateAllInternal();

	_scrollBox->SetScrollOffset(0);
}

void UGsUIRegionSceneItemDropPanel::OnClickClose()
{
	SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIRegionSceneItemDropPanel::OnRefreshEntry(int32 InIndex, class UWidget* InEntry)
{
	if (false == _dataArray.IsValidIndex(InIndex))
	{
		return;
	}

	UGsUIRegionSceneItemDropEntry* entry = Cast<UGsUIRegionSceneItemDropEntry>(InEntry);
	if (nullptr == entry)
	{
		return;
	}

	entry->SetData(_dataArray[InIndex]);
}
