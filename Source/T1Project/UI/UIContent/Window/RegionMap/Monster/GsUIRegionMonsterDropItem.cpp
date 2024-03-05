#include "GsUIRegionMonsterDropItem.h"

#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"

#include "Management/GsMessageHolder.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Core/Public/Delegates/Delegate.h"

#include "UMG/Public/Components/TextBlock.h"


#define WOLRD_MAP_COLLECTION

void UGsUIRegionMonsterDropItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_targetIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_targetIconSlotHelper->Initialize(_targetItemIconSelector);
	_targetIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIRegionMonsterDropItem::OnRefreshTargetIcon);
	_targetIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIRegionMonsterDropItem::OnCreateTargetIcon);
}

void UGsUIRegionMonsterDropItem::BeginDestroy()
{
	if (nullptr != _targetIconSlotHelper)
	{
		_targetIconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIRegionMonsterDropItem::OnRefreshTargetIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}


	if (false == itemIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIRegionMonsterDropItem::OnLongPressIconSlot);
	}


	InitItemIcon(itemIcon);
}

// 아이콘 초기화
void UGsUIRegionMonsterDropItem::InitItemIcon(UGsUIIconItemInventory* In_icon)
{
	if (In_icon == nullptr)
	{
		return;
	}

	In_icon->SetNotOwnItem(_itemTblId, 0);
	In_icon->SetItemNameVisibility(false);
	In_icon->SetItemEnchantLevel(0);
	In_icon->SetActiveIconBg(true);

	// 컬랙션 표시 처리 추가
	// 지역맵 전리품, 몬스터 팝업 아이템리스트
	// https://jira.com2us.com/jira/browse/CHR-25993
#ifdef WOLRD_MAP_COLLECTION
	In_icon->SetItemCollectionMarking(In_icon->GetItemIconData(), true);
#endif
}

void UGsUIRegionMonsterDropItem::OnCreateTargetIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* itemIcon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == itemIcon)
	{
		return;
	}

	itemIcon->SetItemNameVisibility(false);
	itemIcon->SetActiveIconBg(true);
}


void UGsUIRegionMonsterDropItem::SetText(const FText& In_name)
{
	_textName = In_name;
}

// 아이템 id 세팅
void UGsUIRegionMonsterDropItem::SetItemTblId(int In_tblId,const FText& In_name, ItemGrade In_itemGrade)
{
	_itemTblId = In_tblId;

	SetText(In_name);

	// 색상설정
	_textBlockName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(In_itemGrade));

	_targetIconSlotHelper->RefreshAll(1);
}

void UGsUIRegionMonsterDropItem::OnLongPressIconSlot(UGsUIIconItem& InIcon)
{
#ifdef WOLRD_MAP_COLLECTION
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true, true);
#else
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
#endif
}