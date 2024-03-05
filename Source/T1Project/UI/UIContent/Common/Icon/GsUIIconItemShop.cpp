// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIIconItemShop.h"

#include "Management/GsMessageHolder.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"
#include "Item/GsItemConsumable.h"
#include "Item/GsItemEquipment.h"

#include "Components/Image.h"
#include "Components/WidgetSwitcher.h"
#include "Components/CanvasPanel.h"

#include "Blueprint/UserWidget.h"

#include "Management/ScopeGame/GsCoolTimeManager.h"
#include "Management/ScopeGame/GsNpcShopManager.h"
#include "Management/ScopeGame/GsItemCollectionManager.h"

#include "Message/MessageParam/GsCoolTimeMessageParam.h"

#include "UI/UIControlLib/ContentWidget/GsGrayscaleImage.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Skill/GsSkillHelper.h"

#include "NpcShop/GsNpcShopHelper.h"

void UGsUIIconItemShop::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(_itemBG);
	check(_dimmedImg);
	check(_limitLock);
	check(_limitText);
	check(_rankImgWidgetSwicher);
	check(_vcItemPanel);
	check(_iconImage);
	check(_ownedTextPanel);
}

void UGsUIIconItemShop::NativeConstruct()
{
	Super::NativeConstruct();

	SetActiveIconBg(false);
	SetDimmedImage(false);
	SetLimitLock(false);
	SetLimitText(false);
	SetGradeImage(false);
	SetVCImage(false);
	SetImageGrayscale(false);
	SetOwnedText(false);
	SetSkillOptionImage(nullptr);
}

void UGsUIIconItemShop::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIIconItemShop::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIIconItemShop::SetEmptyIcon()
{
	Super::SetEmptyIcon();

	SetVisibility(ESlateVisibility::Visible);
	SetActiveIconBg(true);
	SetDimmedImage(false);
	SetLimitLock(false);
	SetLimitText(false);
	SetGradeImage(false);
	SetVCImage(false);
	SetIconImageSprite(nullptr);
	SetOwnedText(false);
	SetSkillOptionImage(nullptr);
}

void UGsUIIconItemShop::SetNotOwnItem(const uint32 InTID, const ItemAmount InAmount, const int32 InEnchantLevel, const uint8 InEnchantShieldCount, const uint8 InEnchantBonusIndex)
{
	Super::SetNotOwnItem(InTID, InAmount, InEnchantLevel,InEnchantShieldCount,InEnchantBonusIndex);

	SetItemIcon();
}

void UGsUIIconItemShop::SetOwnedItem(const uint64 InItemDBID, const FindItemStorageType InStorageType)
{
	Super::SetOwnedItem(InItemDBID, InStorageType);

	SetItemIcon();
}

void UGsUIIconItemShop::SetItemIcon()
{
	FGsItem* itemData = GetItemIconData();
	if (nullptr == itemData)
		return;

	SetVCImage(itemData->IsCashItem());
	SetSkillOptionImage(itemData);
}

void UGsUIIconItemShop::SetItemShopInfo(FGsNpcShopItemInfo* InNpcShopItemInfo)
{
	if (nullptr == InNpcShopItemInfo)
		return;

	_npcShopItemInfo = InNpcShopItemInfo;
	
	InvalidateItem();
}

void UGsUIIconItemShop::InvalidateItem()
{
	SetDimmedImage(false);
	SetLimitLock(false);
	SetLimitText(false);
	SetOwnedText(false);

	UGsNpcShopManager* shopManager = GSNpcShop();
	if (nullptr == shopManager)
		return;

	if (nullptr == _npcShopItemInfo)
		return;

	ItemAmount remainAmount = 1; 
	if (0 != _npcShopItemInfo->_pcLimitBuyerAmount)
		remainAmount = _npcShopItemInfo->_pcLimitBuyerAmount - _npcShopItemInfo->_purchasedAmount;
	SetItemCount(remainAmount);

	EGsNotAllowedBuyReason type = EGsNotAllowedBuyReason::NONE;
	if (false == FGsNpcShopHelper::IsAllowedBuyItem(_npcShopItemInfo, type))
	{
		// 구매가 불가능할 경우 딤드 처리 후 필요한 이미지/문자 설정
		SetDimmedImage(true);
		switch (type)
		{
		case EGsNotAllowedBuyReason::UNDER_LIMIT_LEVEL:
		{
			SetLimitLock(true);
		}
		break;
		case EGsNotAllowedBuyReason::UNDER_LIMIT_GUILD_LEVEL:
		{
			SetLimitLock(true);
		}
		break;
		case EGsNotAllowedBuyReason::UNDER_LIMIT_GUILD_MEMBER_GRADE:
		{
			SetLimitLock(true);
		}
		break;
		case EGsNotAllowedBuyReason::SOLD_OUT:
		{
			SetLimitText(true);
		}
		break;
		}
	}

	// 판매되는 아이템이 스킬북일 때 보유중인 스킬인지 확인한다.
	FGsItem* itemData = GetItemIconData();
	if (ItemCategoryMain::SKILLBOOK == itemData->GetMainCategory())
	{
		if (FGsItemConsumable* consumItem = static_cast<FGsItemConsumable*>(itemData))
		{
			TArray<int32> userSkillSetIdList;
			if (consumItem->GetUserSkillSetIdList(userSkillSetIdList))
			{
				for (int i = 0; i < userSkillSetIdList.Num(); ++i)
				{
					// 하나라도 보유중이지 않으면 보유중이 아니라고 판단
					if (false == FGsSkillHelper::IsLearnedSkill(userSkillSetIdList[i]))
					{
						return;
					}
				}

				SetOwnedText(true);
				return;
			}
		}
	}
}

void UGsUIIconItemShop::SetActiveIconBg(bool InActive)
{
	_itemBG->SetVisibility((true == InActive) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemShop::SetDimmedImage(bool InActive)
{
	_dimmedImg->SetVisibility((true == InActive) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemShop::SetLimitLock(bool InActive)
{
	_limitLock->SetVisibility((true == InActive) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemShop::SetLimitText(bool InActive)
{
	_limitText->SetVisibility((true == InActive) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemShop::SetItemGrade(ItemGrade InGrade)
{
	SetGradeImage(true);
	uint8 slotIndex = UGsUIIconItem::GetGradeNumByGradeType(InGrade);
	_rankImgWidgetSwicher->SetActiveWidgetIndex(slotIndex);
}

void UGsUIIconItemShop::SetItemSlotEffect(const EGsItemIconSlotEffectType InType)
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

void UGsUIIconItemShop::SetGradeImage(bool InActive)
{
	_rankImgWidgetSwicher->SetVisibility((true == InActive) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemShop::SetVCImage(bool InActive)
{
	_vcItemPanel->SetVisibility((true == InActive) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemShop::SetOwnedText(bool InActive)
{
	_ownedTextPanel->SetVisibility((true == InActive) ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemShop::SetItemCollectionMarking(const FGsItem* InItem, bool InActive /*= false*/)
{
	bool isActive = (nullptr == InItem) ? false : InActive;
	bool isEnable = false;
	if (isActive)
	{
		isActive = false;
		// 2023/4/18 PKT - 장착중이 아니고 프리셋도 아니고 컬렉션 조건 리스트에 들어 있다면 컬렉션 아이콘 활성화
		if (false == InItem->IsEquipped() && false == InItem->IsPresetEquipped())
		{
			TArray<const FGsItemCollectionCondition*> materialList = GSItemCollection()->FindConditionsByItemId(InItem->GetTID());
			for (const auto& material : materialList)
			{
				if (ECollectionMaterialActionType::Completed == material->ActionType())
				{
					continue;
				}

				// 2023/8/24 PKT - [9/26][컬렉션] 등급 높은 아이템 보유 시 컬렉션 활성화 표기 제거
				//                  https://jira.com2us.com/jira/browse/CHR-21782
				//isActive = true;

				for (auto m : material->MaterialAll())
				{
					// 2023/8/24 PKT - [9/26][컬렉션] 등급 높은 아이템 보유 시 컬렉션 활성화 표기 제거
					//                  https://jira.com2us.com/jira/browse/CHR-21782
					if (false == isActive && m.Key == InItem->GetTID() && m.Value >= InItem->GetLevel())
					{
						isActive = true;
					}

					if (true == isActive && m.Key == InItem->GetTID() && m.Value == InItem->GetLevel())
					{
						// 2023/4/18 PKT - 즉시 등록 가능한 것이 존재 한다면 활성화
						isEnable = true;
						break;
					}
				}
			}
		}
	}

	_collectionImage->SetVisibility(isActive ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	_collectionImage->SetIsEnabled(isEnable);
}

void UGsUIIconItemShop::SetIconImageSprite(UPaperSprite* InSprite)
{
	if (InSprite)
	{
		_iconImage->SetPaperSprite(InSprite);
		_iconImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	else
	{
		_iconImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UGsUIIconItemShop::SetImageGrayscale(const bool InIsGray)
{
	_iconImage->SetGrayscale(InIsGray);
}

void UGsUIIconItemShop::SetSkillOptionImage(FGsItem* InItemData)
{
	if (nullptr == InItemData || false == InItemData->IsEquipment())
	{
		_skillOptionImage->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	FGsItemEquipment* equipData = InItemData->GetCastItem<FGsItemEquipment>();
	if (nullptr != equipData)
	{
		_skillOptionImage->SetVisibility(equipData->IsExistSkillOption() ?
			ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}