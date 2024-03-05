#include "GsUIPopupFairyCollection.h"

#include "Shared/Client/SharedEnums/SharedFairyEnum.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGame/GsFairyManager.h"
#include "Fairy/GsFairyData.h"

#include "Engine/AssetManager.h"

#include "Message/MessageParam/GsFairyMessageParam.h"
#include "Management/GsMessageHolder.h"

#include "UI/UIContent/Window/Fairy/GsUIFairyCollectionEntry.h"
#include "UI/UIContent/Window/Fairy/GsUIListItemFairyCollectionData.h"
#include "UI/UIContent/Window/Fairy/GsUISummonPortraitBase.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "ListView.h"
#include "TextBlock.h"
#include "PanelWidget.h"
#include "WidgetSwitcher.h"

void UGsUIPopupFairyCollection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnFindFairy->OnClicked.AddDynamic(this, &UGsUIPopupFairyCollection::OnClickFindFairy);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupFairyCollection::OnClickClose);
	_listSwitcher->SetActiveWidgetIndex(1);

	_levelSelector->OnCreateEntry.BindUObject(this, &UGsUIPopupFairyCollection::OnCreateMenuEntry);
	_levelSelector->OnSelectedEntry.BindUObject(this, &UGsUIPopupFairyCollection::OnInputSelected);
}

void UGsUIPopupFairyCollection::InitializeData(PopupInitData* param)
{
	_levelSelector->CloseMenuPopup();
	if (param && param->_id != INVALID_FAIRY_ID)
	{
		_data = GSFairy()->GetFairyData(param->_id);
		if (auto fairyData = _data.Pin())
		{
			FLinearColor color = FGsUIColorHelper::GetColorFairyGrade(fairyData->GetFairyGrade());

			_fairyName->SetText(fairyData->GetFairyName());
			_fairyName->SetColorAndOpacity(color);

			_portrait->SetData(_data);
			_portrait->RefreshUI();

			auto levelMax = fairyData->GetLimitLevelUp(); 
			_levelSelector->InitializeMenu(levelMax + 2); /*0~max + all*/
			_levelSelector->SetSelectMenuEntry(param->_level == INVALID_FAIRY_LEVEL ?  0: param->_level + 1);
		}

		_panelFindFairy->SetVisibility(param->_useFindItem ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UGsUIPopupFairyCollection::OnClickClose()
{
	_levelSelector->CloseMenuPopup();
	Close();
}

void UGsUIPopupFairyCollection::OnClickFindFairy()
{
	if (auto fairyData = _data.Pin())
	{
		_levelSelector->CloseMenuPopup();

		Close();
		TArray<FairyId> elem = { fairyData->_id };
		FGsFairyMsgParamUpdateList param(elem);
		GMessage()->GetFairy().SendMessage(MessageFairy::FIND_FAIRY, &param);
	}

}

bool UGsUIPopupFairyCollection::OnInputSelected(int32 index, bool forced)
{
	if (_data.IsValid())
	{
		auto level = index == 0 ? INVALID_FAIRY_LEVEL : index - 1;

		TArray<TWeakPtr<FGsFairyCollection>> collections;
		GSFairy()->GetOwnItemCollectionList(collections, _data.Pin()->_id, level);

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
		}

		_listSwitcher->SetActiveWidgetIndex(listItems.IsValidIndex(0) ? 0 : 1);
	}

	return true;
}

void UGsUIPopupFairyCollection::OnCreateMenuEntry(int32 index, struct FMenuData& menuData)
{
	FText levelFormat;
	FText::FindText(TEXT("FairyText"), TEXT("FairyUI_LevelFormat"), levelFormat);

	if (index == 0)
		 menuData._menuName = FText::FromString(TEXT("All"));
	else menuData._menuName = FText::Format(levelFormat, index - 1);
}

void UGsUIPopupFairyCollection::OnInputCancel()
{
	OnClickClose();
}