#include "GsUIPopupCostumeCollection.h"

#include "Shared/Client/SharedEnums/SharedCostumeEnum.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsCostumeManager.h"
#include "Costume/GsCostumeData.h"

#include "Message/MessageParam/GsCostumeMessageParam.h"
#include "Management/GsMessageHolder.h"

#include "UI/UIContent/Window/Costume/GsUICostumeCollectionEntry.h"
#include "UI/UIContent/Window/Costume/GsUIListCostumeCollectionData.h"
#include "UI/UIContent/Window/Fairy/GsUISummonPortraitBase.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "Engine/AssetManager.h"

#include "ListView.h"
#include "TextBlock.h"
#include "WidgetSwitcher.h"

void UGsUIPopupCostumeCollection::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnFindCostume->OnClicked.AddDynamic(this, &UGsUIPopupCostumeCollection::OnClickFindCostume);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupCostumeCollection::OnClickClose);
	_listSwitcher->SetActiveWidgetIndex(1);
}

void UGsUIPopupCostumeCollection::InitializeData(PopupInitData* param)
{
	if (param && param->_id != INVALID_COSTUME_ID)
	{
		_btnFindCostume->SetVisibility(param->_type == PopupInitData::InCostume ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		_textTitle->SetVisibility(param->_type == PopupInitData::InCostume ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

		_data = GSCostume()->GetCostumeData(param->_id);
		if(auto costumeData = _data.Pin())
		{
			FLinearColor color = FGsUIColorHelper::GetColorGradeWidthItemGrade(costumeData->GetCostumeGrade());
			_costumeName->SetText(costumeData->GetCostumeName());
			_costumeName->SetColorAndOpacity(color);

			_portrait->SetData(_data, param->_genderType);
			_portrait->RefreshUI();
		}

		const TArray<TWeakPtr<FGsCostumeCollection>>& collections = GSCostume()->GetOwnItemCollectionList(param->_id);

		TArray<UGsUIListCostumeCollectionData*> listItems;
		for (auto data : collections)
		{
			if (UGsUIListCostumeCollectionData* listItem = NewObject<UGsUIListCostumeCollectionData>())
			{
				listItem->SetItemData(data, true, param->_genderType);
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

		_panelFindCostume->SetVisibility(param->_useFindItem ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	}
}

void UGsUIPopupCostumeCollection::OnClickClose()
{
	Close();
}

void UGsUIPopupCostumeCollection::OnClickFindCostume()
{
	if (auto costumeData = _data.Pin())
	{
		Close();
		TArray<CostumeId> elem = { costumeData->_id };
		FGsCostumeMsgParamUpdateList param(elem);
		GMessage()->GetCostume().SendMessage(MessageCostume::FIND_COSTUME, &param);
	}
}

// 2022/10/20 PKT - KeyBoard Mapping
void UGsUIPopupCostumeCollection::OnInputCancel()
{
	OnClickClose();
}