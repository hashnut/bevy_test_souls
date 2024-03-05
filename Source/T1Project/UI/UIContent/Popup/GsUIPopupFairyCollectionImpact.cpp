#include "GsUIPopupFairyCollectionImpact.h"
#include "Fairy/GsFairyCollection.h"

#include "Management/ScopeGame/GsFairyManager.h"

#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "UTIL/GsTableUtil.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Window/Fairy/GsUIFairyStat.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "ScrollBox.h"
#include "TextBlock.h"
#include "PanelWidget.h"

void UGsUIPopupFairyCollectionImpact::NativeOnInitialized()
{
	_closeButton->OnClicked.AddDynamic(this, &UGsUIPopupFairyCollectionImpact::OnClickClose);

	TArray<UWidget*> childList = _listPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIFairyStat>())
		{
			child->RemoveFromParent();
		}
	}

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _listPanel);
	_scrollBoxHelper->SetEmptyListPanel(_emptyPanel);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupFairyCollectionImpact::OnRefreshEntry);

	Super::NativeOnInitialized();
}

void UGsUIPopupFairyCollectionImpact::CloseInternal()
{
	Super::CloseInternal();
}

void UGsUIPopupFairyCollectionImpact::InitializeData(PopupInitData* initParam)
{
	_collectionRewards.Empty();

	if (initParam && initParam->_collectionCount > 0)
	{
		_textProgressRate->SetText(FText::FromString(FString::Printf(TEXT("%.2f%% (%d/%d)"),
			(initParam->_completeCount * 100.f) / initParam->_collectionCount, initParam->_completeCount, initParam->_collectionCount)));

		FGsEffectStringHelper::GetCollectionEffectStringList(initParam->_collecionOwnType, _collectionRewards);
	}
	
	_scrollBoxHelper->RefreshAll(_collectionRewards.Num());
}

void UGsUIPopupFairyCollectionImpact::OnClickClose()
{
	Close();
}

void UGsUIPopupFairyCollectionImpact::OnRefreshEntry(int32 index, UWidget* InEntry)
{
	if (!_collectionRewards.IsValidIndex(index)) return;

	if (auto entry = Cast<UGsUIFairyStat>(InEntry))
	{
		entry->SetText(_collectionRewards[index].Key, _collectionRewards[index].Value);
	}
}

// 2022/10/20 PKT - KeyBoard Mapping
void UGsUIPopupFairyCollectionImpact::OnInputCancel()
{
	OnClickClose();
}