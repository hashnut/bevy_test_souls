#include "GsUIIconItemCollectionSlot.h"

#include "Runtime/UMG/Public/Components/SlateWrapperTypes.h"
#include "Runtime/UMG/Public/Components/Widget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "Components/Image.h"

#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/Collection/EItemCollectionSlotState.h"
#include "Item/Collection/GsItemCollectionCondition.h"

#include "../../Helper/GsUIColorHelper.h"
#include "UI/UILib/Base/GsUIVFX.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotBase.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Popup/GsUIPopupItemDetailMain.h"
#include "UMG/Public/Components/TextBlock.h"

#include "Message/MessageParam/GsItemContentsMessageParam.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"

#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "../../../../T1Project.h"
#include "../Item/Collection/EItemCollectionEnumData.h"


/**
 * 아이템 컬렉션 슬롯 전용
 */

void UGsUIIconItemCollectionSlot::NativeConstruct()
{
	Super::NativeConstruct();

	_isActiveShiftAbleBtn = false;
	_collectionConditionId = INVALID_COLLECTION_CONDITION_ID;

	_registeredEffect->HideVFX();

	SetEmptyIcon();
}

void UGsUIIconItemCollectionSlot::NativeDestruct()
{
	/*None*/
	
	Super::NativeDestruct();
}

void UGsUIIconItemCollectionSlot::SetItemGrade(ItemGrade InGrade)
{
	if (ItemGrade::NONE == InGrade)
	{
		_swicherGrade->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_swicherGrade->SetVisibility(ESlateVisibility::Visible);
		const uint8 WidgetIndex = UGsUIIconItem::GetGradeNumByGradeType(InGrade);
		_swicherGrade->SetActiveWidgetIndex(WidgetIndex);
	}
}

void UGsUIIconItemCollectionSlot::SetItemSlotEffect(const EGsItemIconSlotEffectType InType)
{

}

void UGsUIIconItemCollectionSlot::SetEmptyIcon()
{
	Super::SetEmptyIcon();

	SetItemGrade(ItemGrade::NONE);
	SetSlotSelectionImage(false);
	SetSkillOptionImage(nullptr);

	_redDot->SetRedDot(false, EGsIconRedDotType::NORMAL);
	_widgetImpossible->SetVisibility(ESlateVisibility::Hidden);
	_widgetCompleted->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIIconItemCollectionSlot::SetData(const FGsItemCollectionCondition& InCondition)
{
	_collectionConditionId = InCondition.Id();
	
	const static int32 DEFAULT_AMOUNT = 1;
	TPair<ItemId, Level> materialInfo = InCondition.EntryMaterialInfo();
	
	Super::SetNotOwnItem(materialInfo.Key, DEFAULT_AMOUNT, materialInfo.Value);

	SetSkillOptionImage(GetItemIconData());

	// 2023/1/13 PKT - Long Pressed 지원
	SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);

	SetSlotSelectionImage(false);

	ECollectionMaterialActionType actionType = ECollectionMaterialActionType::Impossible;

	if (ECollectionMaterialActionType::Completed == InCondition.ActionType())
	{
		actionType = ECollectionMaterialActionType::Completed;
	}
	else
	{
		actionType = InCondition.IsPossiveAction(materialInfo.Key);
	}

	_redDot->SetRedDot(actionType == ECollectionMaterialActionType::ImmediatePossible ? true : false, EGsIconRedDotType::NORMAL);

	_widgetImpossible->SetVisibility(actionType == ECollectionMaterialActionType::Impossible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	_widgetCompleted->SetVisibility(actionType == ECollectionMaterialActionType::Completed ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	// 2023/1/13 PKT - 해당 아이템이 강화도가 부족한 타입이라면 강화도 색을 바꾼다.
	if (actionType == ECollectionMaterialActionType::ConditionalPossible)
	{	
		_enchantNum->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::ENCHANT_NEED_COST));
	}

	bool isShowPiledCountText = 1 < InCondition.ConditionCount();
	if (isShowPiledCountText)
	{
		FString textCount = FString::Format(TEXT("{0}/{1}"), { InCondition.RegisterCount(), InCondition.ConditionCount() });
		_textPiledCount->SetText(FText::FromString(textCount));
	}
	else
	{
		_textPiledCount->SetText(FText::GetEmpty());
	}
	
}

void UGsUIIconItemCollectionSlot::SetSlotSelectionImage(const bool InActive)
{
	_selectImage->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemCollectionSlot::SetSkillOptionImage(const FGsItem* InItemData)
{
	if (nullptr == InItemData || false == InItemData->IsEquipment())
	{
		_skillOptionImage->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	const FGsItemEquipment* equipData = static_cast<const FGsItemEquipment*>(InItemData);
	if (nullptr != equipData)
	{
		_skillOptionImage->SetVisibility(equipData->IsExistSkillOption() ?
			ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

bool UGsUIIconItemCollectionSlot::IsSelected() const
{
	return (ESlateVisibility::HitTestInvisible == _selectImage->GetVisibility());
}

void UGsUIIconItemCollectionSlot::PlayRegisteredEffect()
{
	_registeredEffect->ShowVFX();
}