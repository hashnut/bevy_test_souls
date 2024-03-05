// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/Seal/GsUISealMaterialListitem.h"

#include "Runtime/Core/Public/Math/Color.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"

#include "Classes/GsMessageHandlerOneParam.h"
#include "Currency/GsCostPackage.h"
#include "Currency/GsCurrencyHelper.h"
#include "DataSchema/Item/GsSchemaItemCommon.h"
#include "Item/GsItem.h"
#include "Item/GsItemManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "T1Project.h"
#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemInventory.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIControlLib/Interface/GsIconSelectorInterface.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"

void UGsUISealMaterialListitem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _slotHelperIcon)
	{
		_slotHelperIcon = NewObject<UGsDynamicIconSlotHelper>(this);
		_slotHelperIcon->Initialize(_iconSelector);
		_slotHelperIcon->OnRefreshIcon.AddUniqueDynamic(this, &UGsUISealMaterialListitem::OnRefreshIconItem);
	}

	_materialItemInfoBtn->OnPressed.AddUniqueDynamic(this, &UGsUISealMaterialListitem::OnPressedMaterialTooltipButton);
}

void UGsUISealMaterialListitem::NativeDestruct()
{
	OnHide();

	_iconSelector->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsUISealMaterialListitem::SetParameters(Parameters InParameters)
{
	_parameters = MoveTemp(InParameters);

	// 아이콘 업데이트
	_slotHelperIcon->RefreshAll(1);

	// 텍스트 갱신
	FText ItemAmount = NSLOCTEXT("SealText", "dash", "-");
	FText ItemName = NSLOCTEXT("SealText", "dash", "-");

	if (INVALID_ITEM_ID == _parameters._itemId)
	{
		_itemName->SetColorAndOpacity(FLinearColor::White);
	}
	else
	{
		const UGsTableItemCommon* tableItemCommon = Cast<UGsTableItemCommon>(FGsSchemaItemCommon::GetStaticTable());
		if (nullptr == tableItemCommon)
		{
			GSLOG(Error, TEXT("nullptr == tableItemCommon"));
			return;
		}

		const FGsSchemaItemCommon* itemCommon = nullptr;
		if (!tableItemCommon->FindRowById(_parameters._itemId, itemCommon))
		{
			GSLOG(Error, TEXT("tableItemCommon->FindRowById(InItemId, Row), _parameters._itemId:%d"), _parameters._itemId);
			return;
		}

		if (nullptr == itemCommon)
		{
			GSLOG(Error, TEXT("nullptr == ItemCommon, _parameters._itemId:%d"), _parameters._itemId);
		}
		else
		{
			ItemName = itemCommon->name;
			_itemName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGradeByItemId(_parameters._itemId));
		}

		FGsUIStringHelper::GetTwoValueSlashedText(_parameters._ownedAmount, _parameters._requireAmount, ItemAmount);
	}

	_itemName->SetText(ItemName);
	_itemAmount->SetText(ItemAmount);

	// 부족 여부 컬러링
	const EGsUIColorType amountColorType = _parameters._isEnoughCostElement ? EGsUIColorType::ENCHANT_ENOUGH_COST : EGsUIColorType::ENCHANT_NEED_COST;
	_itemAmount->SetColorAndOpacity(FGsUIColorHelper::GetColor(amountColorType));
}

void UGsUISealMaterialListitem::OnRefreshIconItem(int32 InIndex, UGsUIIconBase* InIcon)
{
	UGsUIIconItemInventory* icon = Cast<UGsUIIconItemInventory>(InIcon);
	if (nullptr == icon)
	{
		GSLOG(Error, TEXT("nullptr == icon, InIndex:%d, GetName():%s"), InIndex, *GetName());
		return;
	}

	if (INVALID_ITEM_ID == _parameters._itemId)
	{
		icon->SetEmptyIcon();

		if (icon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			icon->OnFinishedLongPressEvent.RemoveAll(this);
		}
	}
	else
	{
		// 아이콘 롱프레스 시, 필요 갯수가 출력되도록 설정할 것이다 (제작 시스템과 통일성을 맞추기 위함)
		icon->SetNotOwnItem(_parameters._itemId, _parameters._requireAmount);

		if (!icon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUISealMaterialListitem::OnFinishedLongPressEventItemIcon);
		}
	}

	icon->SetItemNameVisibility(false);
	icon->SetRedDot(false);
}

void UGsUISealMaterialListitem::OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUISealMaterialListitem::OnPressedMaterialTooltipButton()
{
	if (nullptr == _tooltip)
	{
		GSLOG(Error, TEXT("nullptr == _tooltip"));
		return;
	}

	if (_parameters._itemId == INVALID_ITEM_ID)
	{
		return;
	}

	if (_tooltip->IsOpen())
	{
		_tooltip->Close();
		return;
	}

	// 여기서부터는 툴팁 오픈할 때만 실행됨.
	UGsItemManager* ItemManager = GItem();
	if (nullptr == ItemManager)
		return;

	TSet<ItemId> ItemIdList = UGsItemManager::GetAllItemIdsTheSameGroup(_parameters._itemId);

	TArray<TPair<ItemId, Currency>> CandidateItems;
	for (ItemId Id : ItemIdList)
	{
		ItemAmount Amount = ItemManager->GetAllItemCountByTID(Id);
		CandidateItems.Emplace(Id, Amount);
	}

	if (0 == CandidateItems.Num())
		return;

	// 툴팁 텍스트 생성
	const FText Delimiter = FText::FromString(TEXT("\n"));
	FText ItemCountFormat;
	FText::FindText(TEXT("SealText"), TEXT("UI_Common_Owned"), ItemCountFormat);

	FText TooltipTitleText;
	FText::FindText(TEXT("SealText"), TEXT("UI_Common_ItemList"), TooltipTitleText);

	TArray<FText> TooltipTexts;
	TooltipTexts.Emplace(TooltipTitleText);
	for (const TPair<ItemId, Currency>& CandidateItem : CandidateItems)
	{
		const FGsSchemaItemCommon* ItemCommon = FGsItem::GetTableDataByID(CandidateItem.Key);
		if (nullptr == ItemCommon)
		{
			GSLOG(Error, TEXT("nullptr == ItemCommon"));
			continue;
		}

		const FText ItemCountText = FText::Format(ItemCountFormat, CandidateItem.Value);
		TooltipTexts.Emplace(FText::Format(NSLOCTEXT("SealText", "TooltipItemAmount", "{0}: {1}"), ItemCommon->name, ItemCountText));
	}
	_tooltip->SetDescText(FText::Join(Delimiter, TooltipTexts));

	// 툴팁 출력
	_tooltip->Open();
}