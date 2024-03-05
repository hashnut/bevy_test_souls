// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIIconItemReward.h"
#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Engine/AssetManager.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Management/GsMessageHolder.h"
#include "Message/MessageParam/GsItemContentsMessageParam.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "Components/Widget.h"
#include "Item/GsItem.h"
#include "Item/GsItemEquipment.h"
#include "Item/GsItemHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Data/GsDataContainManager.h"


void UGsUIIconItemReward::NativeConstruct()
{
	Super::NativeConstruct();

	// 선택은 기본적으로 끈다
	SetIsSelected(false);
	// 보상 완료는 기본적으로 끈다.
	SetIsRewarded(false);
	// 보상 수령 가능 상태는 기본적으로 끝다.
	SetIsAcceptable(false);

	//추가 보상 열림(배틀패스 추가리워드 오픈) 상태는 기본적으로 끈다
	SetIsRewardLock(false);
	//보상 열리고 받지 않은 보상(배틀패스)에 대한 알림 상태는 기본적으로 끈다
	SetIsRewardNoti(false);
	//보상이 잠기지 않았으나 오픈되지 않음
	SetIsOpenReward(true);

	SetIsEnabled(true);

	SetSkillOptionImage(nullptr);

	SetSlotEffectMagicalForge(nullptr);
}

void UGsUIIconItemReward::NativeDestruct()
{
	_toggleCallback = nullptr;

	Super::NativeDestruct();
}

void UGsUIIconItemReward::SetIsSelected(bool bInIsSelected)
{
	_bIsSelected = bInIsSelected;

	if (_imgSelect)
	{
		_imgSelect->SetVisibility((bInIsSelected) ? 
			ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

bool UGsUIIconItemReward::GetIsSelected() const
{
	return _bIsSelected;
}

void UGsUIIconItemReward::SetIconImageSprite(UPaperSprite* InSprite)
{
	if (InSprite)
	{
		_iconImage->SetBrushFromAtlasInterface(InSprite);
		_iconImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_iconImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIIconItemReward::SetIsRewardLock(bool isRewardLock)
{
	_bIsRewardLock = isRewardLock;

	_lockImage->SetVisibility(_bIsRewardLock ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIIconItemReward::SetIsRewardNoti(bool isRewardNoti)
{
	_bIsRewardNoti = isRewardNoti;

	_redDot->SetVisibility(_bIsRewardNoti ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIIconItemReward::SetItemGrade(ItemGrade InGrade)
{
	// BG컬러 세팅
	//SetBGColorAndOpacity(FGsUIColorHelper::GetColorItemGradeBg(InGrade));
	if (ItemGrade::NONE == InGrade)
	{
		_rankImgWidgetSwicher->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	uint8 slotIndex = UGsUIIconItem::GetGradeNumByGradeType(InGrade);
	_rankImgWidgetSwicher->SetActiveWidgetIndex(slotIndex);
	_rankImgWidgetSwicher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIIconItemReward::SetItemSlotEffect(const EGsItemIconSlotEffectType InType)
{
	if (nullptr == _slotEffectWidget)
		return;

	if (EGsItemIconSlotEffectType::None == InType)
	{
		_slotEffectWidget->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	_slotEffectWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	_slotEffectWidget->SetSlotEffect(InType);
}

void UGsUIIconItemReward::OnClickItemIcon()
{
	Super::OnClickItemIcon();

	if (_toggleCallback)
	{
		// 토글을 위해 반대로 보냄
		_toggleCallback(_indexInGroup, !_bIsSelected);
	}

	// 롱프레스가 생겼으니 필요없음
	/*
	if (_bEnableDetailPopup)
	{
		FGsItem* itemData = GetItemIconData();
		if (nullptr == itemData)
			return;

		FGsItemDetailOpenMessageParam param(itemData, true, _bCenterPopup, _bCenterPopup);
		GMessage()->GetItemContents().SendMessage(MessageContentItem::DETAIL_DISPLAY_ITEM, &param);
	}
	*/
}

void UGsUIIconItemReward::SetBGImage(UPaperSprite* InImg)
{
	_imgBG->SetBrushFromAtlasInterface(InImg);
}

void UGsUIIconItemReward::SetBGVisibility(bool bVisible)
{	
	_imgBG->SetVisibility((bVisible) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemReward::SetBGColorAndOpacity(const FLinearColor& InColor)
{
	_imgBG->SetColorAndOpacity(InColor);
}

void UGsUIIconItemReward::SetIsRewarded(bool bInRewarded)
{
	_widgetRewarded->SetVisibility(bInRewarded ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemReward::SetIsAcceptable(bool bInAcceptable)
{
	_widgetAcceptable->SetVisibility((bInAcceptable) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UGsUIIconItemReward::SetIsEnabled(bool bInEnableIcon)
{	
	_itemSlotButton->SetIsEnabled(bInEnableIcon);
}

void UGsUIIconItemReward::SetIsOpenReward(bool bIsOpenReward)
{
	_widgeIneligible->SetVisibility(bIsOpenReward ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIIconItemReward::SetSkillOptionImage(FGsItem* InItem)
{
	if (nullptr == InItem || false == InItem->IsEquipment())
	{
		_skillOptionImage->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FGsItemEquipment* equipData = InItem->GetCastItem<FGsItemEquipment>();
	if (nullptr != equipData)
	{
		_skillOptionImage->SetVisibility(equipData->IsExistSkillOption() ?
			ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void UGsUIIconItemReward::SetSlotEffectMagicalForge(FGsItem* InItem)
{
	int32 slotIndex = INDEX_NONE;
	do
	{
		if (false == IsBranch(EGsGameClientBranchType::MAGICAL_FORGE))
			break;

		if (nullptr == InItem)
			break;

		if (false == InItem->IsEquipment())
			break;

		FGsItemEquipment* equipData = InItem->GetCastItem<FGsItemEquipment>();
		if (nullptr == equipData)
			break;

		if (false == equipData->GetMagicalForgeData().IsMagicalForge())
			break;

		ItemGrade outGrade = ItemGrade::NONE;
		if (nullptr == FGsItemHelper::GetMagicalForgePassivitySetAndGrade(equipData->GetMagicalForgeData(), outGrade))
			break;

		slotIndex = static_cast<uint8>(outGrade) - static_cast<uint8>(ItemGrade::RARE);

	} while (0);

	_slotEffectMagicalForge->SetSlotEffectByIndex(slotIndex);
}
