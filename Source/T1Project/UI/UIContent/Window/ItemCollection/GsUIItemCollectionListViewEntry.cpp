
#include "GsUIItemCollectionListViewEntry.h"

#include "../Item/Collection/GsItemCollectionField.h"
#include "../Item/Collection/GsItemCollectionCondition.h"
#include "../Item/Collection/EItemCollectionEnumData.h"

#include "Item/Collection/EItemCollectionState.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemCollectionSlot.h"
#include "UI/UIContent/Define/EGsUIColorType.h"

#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"

#include "../Runtime/UMG/Public/Components/Image.h"
#include "../Runtime/UMG/Public/Components/WidgetSwitcher.h"
#include "../Runtime/UMG/Public/Components/TextBlock.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "T1Project/T1Project.h"


void UGsItemCollectionViewEntryData::Initialze(const FGsItemCollectionField* InData)
{
	_fieldData = InData;	
	_isSelected = false;
	_seletecdConditionId = INVALID_COLLECTION_CONDITION_ID;
}

void UGsItemCollectionViewEntryData::SetSelectedCondition(CollectionConditionId InCollectionConditionId)
{
	_seletecdConditionId = INVALID_COLLECTION_CONDITION_ID;
	auto condition = _fieldData->Conditions().FindByPredicate([InCollectionConditionId](const FGsItemCollectionCondition& InData)
		{
			return InCollectionConditionId == InData.Id();
		}
	);
	
	if (nullptr != condition)
	{
		_seletecdConditionId = InCollectionConditionId;
	}
}

void UGsItemCollectionViewEntryData::SetSelectedField(bool InSelected)
{
	_isSelected = InSelected;
}



void UGsUIItemCollectionListViewEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_slotHelper->Initialize(_iconSelector);
	_slotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIItemCollectionListViewEntry::OnRefreshItemConditionIcon);

	if (_btnInActive && false == _btnInActive->OnClicked.IsBound())
	{
		_btnInActive->OnClicked.AddDynamic(this, &UGsUIItemCollectionListViewEntry::OnClickBtnLock);
	}

	if (_BtnBookmark && false == _BtnBookmark->OnClicked.IsBound())
	{
		_BtnBookmark->OnClicked.AddDynamic(this, &UGsUIItemCollectionListViewEntry::OnClickBtnBookMark);
	}
}


void UGsUIItemCollectionListViewEntry::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIItemCollectionListViewEntry::OnRefreshItemConditionIcon);
		_slotHelper = nullptr;
	}

	if (_btnInActive && _btnInActive->OnClicked.IsBound())
	{
		_btnInActive->OnClicked.RemoveDynamic(this, &UGsUIItemCollectionListViewEntry::OnClickBtnLock);
	}

	if (_BtnBookmark && _BtnBookmark->OnClicked.IsBound())
	{
		_BtnBookmark->OnClicked.RemoveDynamic(this, &UGsUIItemCollectionListViewEntry::OnClickBtnBookMark);
	}

	if (_iconSelector)
	{
		_iconSelector->RemoveAllChildren();
	}

	Super::BeginDestroy();
}

void UGsUIItemCollectionListViewEntry::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIItemCollectionListViewEntry::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	UGsItemCollectionViewEntryData* entryData = Cast<UGsItemCollectionViewEntryData>(inListItemObject);
	if (nullptr == entryData || nullptr == entryData->GetData())
	{
		GSLOG(Error, TEXT("nullptr == entryData || nullptr == entryData->GetData()"));
		return;
	}

	/**
	 * Set Data
	*/
	const FGsItemCollectionField* fieldData = entryData->GetData();
	_textTitle->SetText(fieldData->Title());
	
	const static int32 SWITHCER_NORMAL_INDEX = 0;
	const static int32 SWITCHER_EVENT_INDEX = 1;
	_switcherBackGround->SetActiveWidgetIndex(fieldData->IsEventField() ? SWITCHER_EVENT_INDEX : SWITHCER_NORMAL_INDEX);

	_registeredEffectFlags.Empty();
	for (const auto item : fieldData->Conditions())
	{
		_registeredEffectFlags.FindOrAdd(item.Id()) = (ECollectionMaterialActionType::Completed == item.ActionType());
	}	

	Refresh();
}

void UGsUIItemCollectionListViewEntry::Refresh()
{
	UGsItemCollectionViewEntryData* entryData = Cast<UGsItemCollectionViewEntryData>(this->GetListItem());
	if (nullptr == entryData || nullptr == entryData->GetData())
	{
		GSLOG(Error, TEXT("nullptr == entryData || nullptr == entryData->GetData()"));
		return;
	}

	_selectedFieldImage->SetVisibility(entryData->IsSelected() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	const FGsItemCollectionField* fieldData = entryData->GetData();
	
	const int32 conditionCount = fieldData->Conditions().Num();
	_slotHelper->RefreshAll(conditionCount);

	// 2023/7/18 PKT - 등록된 것들 Effect 연출
	for (int32 idx = 0; idx < conditionCount; ++idx)
	{
		auto item = fieldData->Conditions()[idx];

		if (ECollectionMaterialActionType::Completed == item.ActionType() && _registeredEffectFlags.Contains(item.Id()))
		{
			if (false == _registeredEffectFlags[item.Id()])
			{
				UGsUIIconItemCollectionSlot* itemIcon = Cast<UGsUIIconItemCollectionSlot>(_slotHelper->GetIcon(idx));
				itemIcon->PlayRegisteredEffect();

				_registeredEffectFlags[item.Id()] = true;
			}
		}
	}

	// 2021/08/03 PKT - 즐겨 찾기 상태에 따른 변경
	_BtnBookmark->SetIsSelected(fieldData->IsBookMarkEnable());

	_textStat->SetText(fieldData->StatSetText());
	_textDate->SetText(fieldData->TextDateTime());
	
	ECollectionSortOrder progressState = fieldData->SortOrder();
	if (ECollectionSortOrder::Completed == progressState)
	{	// 2021/07/26 PKT - 완료 상태일때는 텍스트 색상 변경
		_textStat->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::ITEM_COLLECTION_REGISTER));
		_textDate->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::ITEM_COLLECTION_REGISTER));
	}
	else
	{	// 2021/07/26 PKT - 미완료 색상
		_textStat->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::ITEM_COLLECTION_UNREGISTER));
		_textDate->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::ITEM_COLLECTION_UNREGISTER));
	}

	/**
	 * Set Field State
	 */
	const static int32 NORMAL_SWITCHER_INDEX = 0;
	const static int32 COMPLETED_SWITCHER_INDEX = 1;
	const static int32 INACTIVE_SWITCHER_INDEX = 2;

	if (ECollectionSortOrder::Completed == progressState)
	{	// 2021/08/06 PKT - 체크(완료)
		_switcherActive->SetActiveWidgetIndex(COMPLETED_SWITCHER_INDEX);
		// 2021/08/03 PKT - Lock Button 비활성화
		_btnInActive->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if (ECollectionSortOrder::InActive == progressState)
	{
		// 2021/08/06 PKT - 비활성 상태라면 자물쇠 모양
		_switcherActive->SetActiveWidgetIndex(INACTIVE_SWITCHER_INDEX);
		// 2021/08/03 PKT - Lock Button 활성화
		_btnInActive->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{	// 2021/08/06 PKT - 일반
		_switcherActive->SetActiveWidgetIndex(NORMAL_SWITCHER_INDEX);
		// 2021/08/03 PKT - Lock Button 비활성화
		_btnInActive->SetVisibility(ESlateVisibility::Collapsed);
	}

	// 2023/1/3 PKT - 진행도 표기 한다.
	int32 completedCount = 0;
	for (auto& item : fieldData->Conditions())
	{
		if (ECollectionMaterialActionType::Completed == item.ActionType())
		{
			completedCount++;
		}
	}

	// 2021/10/12 PKT - 진행도 표시
	_textCompleteProgressCount->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), completedCount, conditionCount)));
}

void UGsUIItemCollectionListViewEntry::OnRefreshItemConditionIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsItemCollectionViewEntryData* entryData = Cast<UGsItemCollectionViewEntryData>(this->GetListItem());
	if (nullptr == entryData || nullptr == entryData->GetData())
	{
		GSLOG(Error, TEXT("nullptr == entryData || nullptr == entryData->GetData()"));
		return;
	}

	if (false == entryData->GetData()->Conditions().IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == entryData->GetData()->Conditions().IsValidIndex(InIndex) : [%d]"), InIndex);
		return;
	}

	const FGsItemCollectionCondition& condition = entryData->GetData()->Conditions()[InIndex];

	UGsUIIconItemCollectionSlot* itemIcon = Cast<UGsUIIconItemCollectionSlot>(InIcon);
	if (nullptr == itemIcon)
	{
		GSLOG(Error, TEXT("nullptr == itemIcon"));
		return;
	}

	itemIcon->SetData(condition);
	itemIcon->SetSlotSelectionImage(entryData->IsSelectedCondition(condition.Id()));

	if (false == itemIcon->OnClickSlot.IsBoundToObject(this))
	{
		itemIcon->OnClickSlot.AddUObject(this, &UGsUIItemCollectionListViewEntry::OnClickSlotItemIcon);
	}

	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIItemCollectionListViewEntry::OnLongPressedItemIcon);
	}
}


void UGsUIItemCollectionListViewEntry::OnClickBtnLock()
{
	/**
	 * 
	 */
}


void UGsUIItemCollectionListViewEntry::OnClickBtnBookMark()
{
	UGsItemCollectionViewEntryData* entryData = Cast<UGsItemCollectionViewEntryData>(this->GetListItem());
	if (nullptr == entryData || nullptr == entryData->GetData())
	{
		GSLOG(Error, TEXT("nullptr == entryData || nullptr == entryData->GetData()"));
		return;
	}
	// 2023/1/4 PKT - 즐겨 찾기 상태 갱신 요청
	bool newToggle = (false == entryData->GetData()->IsBookMarkEnable());
	FGsNetSendServiceWorld::SendItemCollectionBookmark(entryData->GetData()->Id(), newToggle);
}

void UGsUIItemCollectionListViewEntry::OnClickSlotItemIcon(UGsUIIconItem& InIcon)
{
	UGsItemCollectionViewEntryData* entryData = Cast<UGsItemCollectionViewEntryData>(this->GetListItem());
	if (nullptr == entryData || nullptr == entryData->GetData())
	{
		GSLOG(Error, TEXT("nullptr == entryData || nullptr == entryData->GetData()"));
		return;
	}

	UGsUIIconItemCollectionSlot* Icon = Cast<UGsUIIconItemCollectionSlot>(&InIcon);
	if (Icon)
	{
		entryData->OnClickedCondition.ExecuteIfBound(this, Icon->GetCollectionConditionId());
	}
}

void UGsUIItemCollectionListViewEntry::OnLongPressedItemIcon(UGsUIIconItem& InIcon)
{
	UGsItemCollectionViewEntryData* entryData = Cast<UGsItemCollectionViewEntryData>(this->GetListItem());
	if (nullptr == entryData || nullptr == entryData->GetData())
	{
		GSLOG(Error, TEXT("nullptr == entryData || nullptr == entryData->GetData()"));
		return;
	}

	UGsUIIconItemCollectionSlot* Icon = Cast<UGsUIIconItemCollectionSlot>(&InIcon);
	if (Icon)
	{
		entryData->OnLongPressedIcon.ExecuteIfBound(Icon, this, Icon->GetCollectionConditionId());
	}
}