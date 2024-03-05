
#include "GsUIAlternateItemEntry.h"
#include "../Item/GsItem.h"
#include "../../Common/Icon/GsUIIconBase.h"
#include "../../Common/Icon/GsUIIconItemInventory.h"
#include "../../Common/Icon/GsUIIconItem.h"
#include "../Item/Collection/GsItemCollectionCondition.h"
#include "../UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"

#include "../../Helper/GsUIColorHelper.h"

#include "DataSchema/Item/GsSchemaItemCommon.h"


void UGsUIAlternateItemEntry::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIAlternateItemEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	_btnSelect->OnClicked.AddDynamic(this, &UGsUIAlternateItemEntry::OnClickedSelected);
}

void UGsUIAlternateItemEntry::NativeConstruct()
{
	Super::NativeConstruct();

	_data = INVALID_ITEM_ID;

	_imgSelectedBoard->SetVisibility(ESlateVisibility::Collapsed);
	_imgSelectedEdge->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIAlternateItemEntry::NativeDestruct()
{
	Super::NativeDestruct();
}

UGsUIIconItemInventory* UGsUIAlternateItemEntry::GetIcon()
{
	if (0 == _iconSelector->GetChildrenCount())
	{
		UGsUIIconBase* newIcon = _iconSelector->CreateIcon();
		if (newIcon)
		{
			_iconSelector->AddChild(newIcon);

			if (UGsUIIconItemInventory* newItemIcon = Cast<UGsUIIconItemInventory>(newIcon))
			{
				newItemIcon->ResetCoolTime();
				//newItemIcon->SetItemNameVisibility(false); // 어짜피 또해줌
				newItemIcon->SetItemCount(0);

				return newItemIcon;
			}
		}
	}

	// 아이콘을 하나만 생성하므로 0번째 아이콘 리턴
	return Cast<UGsUIIconItemInventory>(_iconSelector->GetChildAt(0));
}

void UGsUIAlternateItemEntry::OnClickedSelected()
{
	// 2023/1/13 PKT - 선택이 아닐 경우에만 이벤트 발생
	if (_imgSelectedBoard->GetVisibility() == ESlateVisibility::Collapsed)
	{	
		OnClicked.ExecuteIfBound(this);
	}
}

void UGsUIAlternateItemEntry::SetData(const FGsItemCollectionCondition* InData, int32 InIndex)
{
	if (nullptr == InData || false == InData->MaterialAll().IsValidIndex(InIndex))
	{
		return;
	}

	TPair<ItemId, Level> materialInfo = InData->MaterialAll()[InIndex];
	
	if (UGsUIIconItemInventory* itemIcon = GetIcon())
	{
		const FGsSchemaItemCommon* itemData = FGsItem::GetTableDataByID(materialInfo.Key);
		if (nullptr == itemData)
		{
			return;
		}

		itemIcon->SetNotOwnItem(materialInfo.Key, 1, materialInfo.Value);
		itemIcon->SetActiveIconBg(true);
		itemIcon->SetItemNameVisibility(false);
		itemIcon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::NotUseDetailWndType);
		itemIcon->SetRedDot(false);
		itemIcon->SetDimmedImage(false);

		ECollectionMaterialActionType actionType = InData->IsPossiveAction(materialInfo.Key);
		if (ECollectionMaterialActionType::ImmediatePossible == actionType)
		{	// 2023/1/13 PKT - 즉시 등록 가능하면 레드닷
			itemIcon->SetRedDot(true);
		}
		else if (ECollectionMaterialActionType::Impossible == actionType)
		{	// 2023/1/13 PKT - 소유 하고 있지 않은 상태( 딤드 )
			itemIcon->SetDimmedImage(true);
		}
		
		_textItemName->SetText(itemData->name);
		_textItemName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(itemData->grade));

		_data = itemData->id;
	}	
}

void UGsUIAlternateItemEntry::SetIsSelected(bool InFlag)
{
	ESlateVisibility visibility = InFlag ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed;

	_imgSelectedBoard->SetVisibility(visibility);
	_imgSelectedEdge->SetVisibility(visibility);
}