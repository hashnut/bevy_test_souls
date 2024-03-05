#include "GsUIItemCollectionSlot.h"

#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "Item/Collection/EItemCollectionSlotState.h"
#include "Item/Collection/GsItemCollectionSlotState.h"
#include "Item/Collection/GsItemCollectionState.h"
#include "Item/GsItem.h"
#include "Management/GsScopeGame.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "T1Project.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemCollectionSlot.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Window/GsUIWindowItemCollection.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"

#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "../Net/GsNetSendServiceWorld.h"


void UGsUIItemCollectionSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//
}

void UGsUIItemCollectionSlot::NativeDestruct()
{
	OnHide();

	Super::NativeDestruct();
}

void UGsUIItemCollectionSlot::OnShow()
{
	_slotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_slotHelper->Initialize(_iconSelector);
	_slotHelper->OnCreateIcon.AddDynamic(this, &UGsUIItemCollectionSlot::OnCreateIconOwnedItem);
	_slotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIItemCollectionSlot::OnRefreshIconOwnedItem);

	if (false == _btnLock->OnClicked.IsBound())
	{
		_btnLock->OnClicked.AddDynamic(this, &UGsUIItemCollectionSlot::OnClickBtnLock);
	}

	if (false == _BtnBookmark->OnClicked.IsBound())
	{
		_BtnBookmark->OnClicked.AddDynamic(this, &UGsUIItemCollectionSlot::OnClickBtnBookMark);
	}

	OnClickItemCollectionSlotIcon.Unbind();
	
	_itemCollection = nullptr;
	
	_window = nullptr;
}

void UGsUIItemCollectionSlot::OnHide()
{
	OnClickItemCollectionSlotIcon.Unbind();
	
	_itemCollection = nullptr;
	
	_window = nullptr;

	if (_slotHelper)
	{
		_slotHelper->OnCreateIcon.RemoveDynamic(this, &UGsUIItemCollectionSlot::OnCreateIconOwnedItem);
		_slotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIItemCollectionSlot::OnRefreshIconOwnedItem);
		_slotHelper = nullptr;
	}

	if (_btnLock && _btnLock->OnClicked.IsBound())
	{
		_btnLock->OnClicked.RemoveDynamic(this, &UGsUIItemCollectionSlot::OnClickBtnLock);
	}

	if (_BtnBookmark && _BtnBookmark->OnClicked.IsBound())
	{
		_BtnBookmark->OnClicked.RemoveDynamic(this, &UGsUIItemCollectionSlot::OnClickBtnBookMark);
	}
}

void UGsUIItemCollectionSlot::SetCollectionState(const FGsItemCollectionState* InData)
{
	const static int32 NORMAL_INDEX = 0;
	const static int32 COMPLETED_INDEX = 1;
	const static int32 LOCK_INDEX = 2;

	bool isCompleted = (EItemCollectionState::Completed == InData->GetType());
	bool isUnLock = InData->IsUnLock();

	_btnLock->SetVisibility(ESlateVisibility::Collapsed);

	if (isCompleted)
	{	// 2021/08/06 PKT - 체크(완료)
		_switcherState->SetActiveWidgetIndex(COMPLETED_INDEX);
		// 2021/08/03 PKT - Lock Button 비활성화
		_btnLock->SetVisibility(ESlateVisibility::Collapsed);
	}
	else if ( false == isUnLock)
	{	// 2021/08/06 PKT - 자물쇠
		_switcherState->SetActiveWidgetIndex(LOCK_INDEX);
		// 2021/08/03 PKT - Lock Button 활성화
		_btnLock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{	// 2021/08/06 PKT - 일반
		_switcherState->SetActiveWidgetIndex(NORMAL_INDEX);
		// 2021/08/03 PKT - Lock Button 비활성화
		_btnLock->SetVisibility(ESlateVisibility::Collapsed);

		// 2021/08/03 PKT - 진행도 표기
		const int32 maxCount = _itemCollection->GetInnerSlotCount();
		int32 InnerSlotCompletedCount = 0;
		for (int32 idx = 0; idx < _itemCollection->GetInnerSlotCount(); ++idx)
		{
			const FGsItemCollectionSlotState* innerSlot = _itemCollection->GetInnerSlotByIndex(idx);
			if (innerSlot && EItemCollectionSlotState::Completed == innerSlot->GetType())
			{
				InnerSlotCompletedCount++;
			}
		}
		_textStateCount->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), InnerSlotCompletedCount, maxCount)));
	}
}


void UGsUIItemCollectionSlot::SetData(const UGsUIWindowItemCollection* InWindow, const FGsItemCollectionState* InData)
{
	check(!!InWindow);
	check(!!InData);

	_window = InWindow;
	_itemCollection = InData;

	// 2021/08/03 PKT - title
	_textTitle->SetText(_itemCollection->GetNameText());
	// 2021/08/03 PKT - stat
	_textDesc->SetText(_itemCollection->GetStatEffectText());
	// 2021/08/03 PKT - date
	_textSubDesc->SetText(_itemCollection->GetDateTimeText());

	if (EItemCollectionState::Completed == _itemCollection->GetType())
	{	// 2021/07/26 PKT - 완료 상태일때는 텍스트 색상 변경
		_textDesc->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_DISABLE));
	}
	else
	{	// 2021/07/26 PKT - 미완료 색상
		_textDesc->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::DEFAULT_VALID));
	}

	int32 maxCount = _itemCollection->GetInnerSlotCount();
	_slotHelper->RefreshAll(maxCount);

	// 2021/08/03 PKT - 즐겨 찾기 상태에 따른 변경
	_BtnBookmark->SetIsSelected(_itemCollection->IsActivatedBookMark());

	// 2021/08/03 PKT - 진행 상황에 따른 이미지 및 버튼 변경
	SetCollectionState(_itemCollection);
}


const FGsItemCollectionState* UGsUIItemCollectionSlot::GetData() const
{
	return _itemCollection;
}


int32 UGsUIItemCollectionSlot::GetItemIconCount() const
{
	return _itemCollection->GetInnerSlotCount();
}

//const UGsUIIconItem* UGsUIItemCollectionSlot::GetItemIconByIndex(int32 InIndex) const
//{
//	return _slotHelper->GetIcon(InIndex);
//}


void UGsUIItemCollectionSlot::OnCreateIconOwnedItem(UGsUIIconBase* InIcon)
{
	UGsUIIconItemCollectionSlot* ItemIcon = Cast<UGsUIIconItemCollectionSlot>(InIcon);
	if (nullptr == ItemIcon)
	{
		GSLOG(Error, TEXT("nullptr == ItemIcon"));
		return;
	}
}


void UGsUIItemCollectionSlot::OnRefreshIconOwnedItem(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemCollectionSlot* ItemIcon = Cast<UGsUIIconItemCollectionSlot>(InIcon);
	if (nullptr == ItemIcon)
	{
		GSLOG(Error, TEXT("nullptr == ItemIcon"));
		return;
	}

	if (nullptr == _itemCollection)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("nullptr == _itemCollection"));
#endif
		return;
	}

	const FGsItemCollectionSlotState* innerSlot = _itemCollection->GetInnerSlotByIndex(InIndex);
	const FGsItemCollectionSlotState::MaterialData mainMaterialData = innerSlot->GetMainMaterialData();

	const static int32 DEFAULT_ITEM_AMOUNT = 1;
	ItemIcon->SetNotOwnItem(mainMaterialData.Key, DEFAULT_ITEM_AMOUNT);
	ItemIcon->SetItemEnchantLevel(mainMaterialData.Value);
	ItemIcon->SetSlotSelectionImage(false);

	// 2021/08/06 PKT - 슬롯 상태는 락이 잠겨져 있다면 무조건 불가능 한것으로 처리. 레드닷도 막는다.
	/*EItemCollectionSlotState slotState = (_itemCollection->IsUnLock()) ? innerSlot->GetType() : EItemCollectionSlotState::Impossible;
	ItemIcon->SetItemCollectionSlotState(slotState);*/
	// 2021/08/19 PKT - long press off
	ItemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::OnClickType);

	if (!ItemIcon->OnClickSlot.IsBoundToObject(this))
	{
		ItemIcon->OnClickSlot.AddUObject(this, &UGsUIItemCollectionSlot::OnClickSlotItemIcon);
	}
}

void UGsUIItemCollectionSlot::OnClickSlotItemIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem* Icon = Cast<UGsUIIconItemCollectionSlot>(&InIcon);
	if (Icon)
	{
		CollectionConditionId collectionConditionId = INVALID_COLLECTION_CONDITION_ID;

		int32 maxCount = _itemCollection->GetInnerSlotCount();
		for (int32 idx = 0; idx < maxCount; ++idx)
		{
			UGsUIIconBase* innerSlotIcon = _slotHelper->GetIcon(idx);
			if (innerSlotIcon->GetUniqueID() == Icon->GetUniqueID())
			{
				collectionConditionId = _itemCollection->GetInnerSlotByIndex(idx)->GetItemCollectionConditionId();
			}
		}
		
		OnClickItemCollectionSlotIcon.ExecuteIfBound(Icon, _itemCollection->GetCollectionId(), collectionConditionId);
	}
}

void UGsUIItemCollectionSlot::OnClickBtnLock()
{
	if (nullptr == _itemCollection)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("nullptr == _itemCollection"));
#endif
		return;
	}

	if (true == _itemCollection->IsUnLock())
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("true == _itemCollection->IsUnLock()"));
#endif
		return;
	}

	FGsUIHelper::TrayMessageTicker(_itemCollection->ReasonLockMessage());
}

void UGsUIItemCollectionSlot::OnClickBtnBookMark()
{
	FGsItemCollectionManager* itemCollectionManager = GSItemCollection();
	if (nullptr == itemCollectionManager)
	{
#if WITH_EDITOR
		GSLOG(Warning, TEXT("#ssdef_return"));
#endif
		return;
	}

	bool bNextFlag = (false == _BtnBookmark->GetIsSelected());
	FGsNetSendServiceWorld::SendItemCollectionBookmark(_itemCollection->GetCollectionId(), bNextFlag);
}
