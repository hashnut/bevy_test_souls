// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIIconItemInventory.h"

#include "Components/WidgetComponent.h"
#include "Components/WidgetSwitcher.h"
#include "Components/Overlay.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"

#include "Data/GsDataContainManager.h"

#include "Item/GsItemManager.h"
#include "Item/GsItemConsumable.h"
#include "Item/GsItemEquipment.h"
#include "Item/Collection/GsItemCollectionCondition.h"
#include "Item/Collection/EItemCollectionEnumData.h"
#include "Item/GsItemHelper.h"

#include "Management/ScopeGame/GsItemCollectionManager.h"
#include "Management/ScopeGame/GsCoolTimeManager.h"

#include "Misc/DateTime.h"

#include "UTIL/GsTimeSyncUtil.h"

#include "UI/UILib/Base/GsUIVFX.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Common/GsUICoolTimeEffect.h"
#include "UI/UIContent/Tray/GsUITrayItemNameToolTip.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"



void UGsUIIconItemInventory::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	check(_itemNameText);
	check(_selectImage);
	check(_itemBG);
	check(_equipRootPanel);
	check(_unequipRootPanel);
	check(_useRootPanel);
	check(_equipedImage);
	check(_redDot);
	check(_disableUnEquip);
	check(_rankImgWidgetSwicher);
	check(_enchantNum);
	check(_lockImage);
	check(_dimmedImg);
	check(_vcItemPanel);

	_itemSlotButton->OnReleased.AddDynamic(this, &UGsUIIconItemInventory::OnButtonRelease);
	_itemSlotButton->OnMouseButtonEventDownEx.BindUObject(this, &UGsUIIconItemInventory::OnButtonDown);
}

void UGsUIIconItemInventory::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIIconItemInventory::NativeConstruct()
{
	Super::NativeConstruct();

	SetActiveIconBg(false);
	SetSlotUseImage(false);
	SetSlotUnEquipImage(false);
	SetSlotEquipImage(false);
	SetSlotSelectionImage(false);
	SetSlotEquipedImage(false);
	SetPresetImage(false);
	SetSlotDisableUnEquiped(false);
	SetLockImage(false);
	SetDimmedImage(false);
	SetAutoImage(false);
	SetVCImage(false);
	SetLearnedImage(nullptr);
	SetSkillOptionImage(nullptr);
	SetItemCollectionMarking(nullptr);
	SetSlotEffectMagicalForge(nullptr);
	SetCoolTimeVisibility(false);

	_isExpiredTime = false;

	_itemExpireTimeText->SetVisibility(ESlateVisibility::Collapsed);
	_rankImgWidgetSwicher->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::NativeDestruct()
{
	_isExpiredTime = false;
	_paramValue = 0;
	_slotActionType = SlotActionType::None;

	Super::NativeDestruct();
}

void UGsUIIconItemInventory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (_isExpiredTime)
	{
		if (FGsItem* itemData = GetItemIconData())
		{
			SetPeriodTime(itemData);
		}
	}

	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIIconItemInventory::SetEmptyIcon()
{
	Super::SetEmptyIcon();

	SetVisibility(ESlateVisibility::Visible);
	SetActiveIconBg(true);
	SetRedDot(false);
	SetItemNameVisibility(false);
	SetSlotUseImage(false);
	SetSlotUnEquipImage(false);
	SetSlotEquipImage(false);
	SetSlotSelectionImage(false);
	SetSlotEquipedImage(false);
	SetSlotDisableUnEquiped(false);
	SetLockImage(false);
	SetDimmedImage(false);
	SetAutoImage(false);
	SetVCImage(false);
	SetLearnedImage(nullptr);
	SetSkillOptionImage(nullptr);
	SetPresetImage(false);
	SetItemCollectionMarking(nullptr);
	SetCoolTimeVisibility(false);

	_isExpiredTime = false;
	_slotActionType = SlotActionType::None;
	_itemExpireTimeText->SetVisibility(ESlateVisibility::Collapsed);
	_rankImgWidgetSwicher->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetGrade(ItemGrade InGrade)
{
	SetItemGrade(InGrade);
}

void UGsUIIconItemInventory::SetItemGrade(ItemGrade InGrade)
{
	if (ItemGrade::NONE == InGrade)
	{
		_rankImgWidgetSwicher->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	uint8 slotIndex = UGsUIIconItem::GetGradeNumByGradeType(InGrade);
	_rankImgWidgetSwicher->SetActiveWidgetIndex(slotIndex);
	_rankImgWidgetSwicher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UGsUIIconItemInventory::SetSlotEffect(const EGsItemIconSlotEffectType InType)
{
	SetItemSlotEffect(InType);
}

void UGsUIIconItemInventory::SetItemSlotEffect(const EGsItemIconSlotEffectType InType)
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

void UGsUIIconItemInventory::SetCooltimeUI(float InRemainSec, float InRate)
{
	Super::SetCooltimeUI(InRemainSec, InRate);

	if (0 < InRemainSec)
	{
		FText coolTimeText;
		int32 ceilCoolTime = FMath::CeilToInt(InRemainSec);
		if (60 <= ceilCoolTime)
		{
			coolTimeText = FText::FromString(FGsTimeStringHelper::GetMinuteString(ceilCoolTime / 60));
		}
		else
		{
			coolTimeText = FText::AsNumber(ceilCoolTime);
		}
		_textBlockRemainTime->SetText(coolTimeText);
	}
	else
	{
		_textBlockRemainTime->SetText(FText::GetEmpty());
	}

	_uiCoolTime->SetProgress(InRate);
}

void UGsUIIconItemInventory::ResetCoolTime()
{
	SetCooltimeUI(0.f, 0.f);

	_isCoolTime = false;
	_coolTime = 0.f;
	_remainCoolTime = 0.f;
}

void UGsUIIconItemInventory::SetAutoImage(bool InActive)
{
	_autoImage->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetActiveIconBg(bool InActive)
{
	_itemBG->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetSlotSelectionImage(bool InActive)
{
	_selectImage->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetSlotEquipImage(bool InActive)
{
	_equipRootPanel->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetSlotUnEquipImage(bool InActive)
{
	_unequipRootPanel->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetSlotUseImage(bool InActive)
{
	_useRootPanel->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetSlotEquipedImage(bool InActive)
{
	_equipedImage->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetSlotDisableUnEquiped(bool InActive)
{
	_disableUnEquip->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetRedDot(bool InActive)
{
	_redDot->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetCoolTimeBg(bool InIsCircularBg)
{
	_isCircularBg = InIsCircularBg;
}

void UGsUIIconItemInventory::SetLockImage(bool InActive)
{
	_lockImage->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetDimmedImage(bool InActive)
{
	_dimmedImg->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIIconItemInventory::SetVCImage(bool InActive)
{
	_vcItemPanel->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetPresetImage(bool InActive)
{
	_presetImage->SetVisibility(InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIIconItemInventory::SetSlotActionType(UGsUIIconItemInventory::SlotActionType InType)
{
	_slotActionType = InType;
}

void UGsUIIconItemInventory::ResetSlotAction()
{
	SetSlotUseImage(false);
	SetSlotEquipImage(false);
	SetSlotUnEquipImage(false);
	_slotActionType = SlotActionType::None;
}

void UGsUIIconItemInventory::OnClickItemIcon()
{
	FGsItem* itemData = GetItemIconData();
	if (nullptr != itemData)
	{
		itemData->SetIsNew(false);
	}

	// RedDot Off 처리
	SetRedDot(false);
	// OnClick Event Send

	Super::OnClickItemIcon();
}

void UGsUIIconItemInventory::SetNotOwnItem(const uint32 InTID, const ItemAmount InAmount, const int32 InEnchantLevel, const uint8 InEnchantShieldCount, const uint8 InEnchantBonusIndex)
{
	Super::SetNotOwnItem(InTID, InAmount, InEnchantLevel,InEnchantShieldCount,InEnchantBonusIndex);

	SetItemIcon();
}

void UGsUIIconItemInventory::SetOwnedItem(const uint64 InItemDBID, const FindItemStorageType InStorageType)
{
	Super::SetOwnedItem(InItemDBID, InStorageType);

	SetItemIcon();
}

void UGsUIIconItemInventory::SetItemIcon()
{
	FGsItem* itemData = GetItemIconData();
	if (nullptr == itemData)
		return;

	SetLockImage(itemData->GetLock());
	SetRedDot(itemData->IsNew());
	SetItemNameVisibility(true);
	SetItemName(itemData->GetName());
	SetSlotEquipedImage(itemData->IsEquipped());
	SetPresetImage(false == itemData->IsEquipped() && itemData->IsPresetEquipped());
	SetVCImage(itemData->IsCashItem());
	SetPeriodTime(itemData);
	SetLearnedImage(itemData);
	SetSkillOptionImage(itemData);
	SetItemCollectionMarking(itemData);
	SetCoolTimeVisibility(true);
}

void UGsUIIconItemInventory::SetItemName(FText InName) const
{
	if (nullptr != _itemNameText)
	{
		_itemNameText->SetText(InName);
	}
}

void UGsUIIconItemInventory::SetItemNameVisibility(bool InActive)
{
	if (nullptr != _itemNameText)
	{
		_itemNameText->SetVisibility(true == InActive ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
	}
}

// 만료시간이 24시간 이내인 경우 남은 시간 표기
void UGsUIIconItemInventory::SetPeriodTime(FGsItem* InItem)
{
	if (nullptr == InItem)
	{
		_itemExpireTimeText->SetVisibility(ESlateVisibility::Collapsed);
		_isExpiredTime = false;
		return;
	}

	FTimespan remainTime;
	if (InItem->GetExpiredRemainTime(remainTime))
	{
		// 1일 이상
		if (ETimespan::TicksPerDay <= remainTime.GetTicks())
		{
			// 보여지지 않는다.
			_itemExpireTimeText->SetVisibility(ESlateVisibility::Collapsed);
			_isExpiredTime = false;
			return;
		}
		// 1일 미만
		else
		{
			FText findText;
			_isExpiredTime = true;
			_itemExpireTimeText->SetVisibility(ESlateVisibility::Visible);

			// 시 표기
			int32 remainHours = remainTime.GetHours();
			if (0 < remainHours)
			{
				FText::FindText(TEXT("UICommonText"), TEXT("UI_Time_Hour"), findText);
				_itemExpireTimeText->SetText(FText::Format(findText, FText::AsNumber(remainHours)));
				return;
			}

			// 분 표기
			int32 remainMinutes = remainTime.GetMinutes();
			if (0 < remainMinutes)
			{
				FText::FindText(TEXT("UICommonText"), TEXT("UI_Time_Minute"), findText);
				_itemExpireTimeText->SetText(FText::Format(findText, FText::AsNumber(remainMinutes)));
				return;
			}

			// 초 표기
			int32 remainSeconds = remainTime.GetSeconds();
			FText::FindText(TEXT("UICommonText"), TEXT("UI_Time_Second"), findText);
			_itemExpireTimeText->SetText(FText::Format(findText, FText::AsNumber(remainSeconds)));
			return;
		}
	}
	else
	{
		// 기간 만료상태
		_itemExpireTimeText->SetVisibility(ESlateVisibility::Collapsed);
		_isExpiredTime = false;
	}
}

void UGsUIIconItemInventory::SetLearnedImage(FGsItem* InItem)
{
	bool bIsLearned = false;
	if (InItem)
	{
		if (ItemType::CONSUME == InItem->GetType() &&
			ItemCategoryMain::SKILLBOOK == InItem->GetMainCategory())
		{
			if (FGsItemConsumable* consumeItem = InItem->GetCastItem<FGsItemConsumable>())
			{
				bIsLearned = consumeItem->IsLearnedSkillBookItem();
			}
		}
	}

	_learnedImage->SetVisibility(bIsLearned ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Hidden);
}

void UGsUIIconItemInventory::SetItemCollectionMarking(const FGsItem* InItem, bool InActive /* = false */)
{
	/**
	 * https://jira.com2us.com/jira/browse/C2URWQ-4036 
	 * Item : 장착 표시 > 프리셋 표시 > 컬렉션 가능 표시 
	 * SkillBook : 컬렉션 > 배움 표시
	 */
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

	if (ESlateVisibility::SelfHitTestInvisible == _collectionImage->GetVisibility())
	{
		/**
		 * SkillBook : 컬렉션 > 배움 표시
		 */
		_learnedImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIIconItemInventory::SetSlotEffectMagicalForge(FGsItem* InItem)
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

void UGsUIIconItemInventory::SetCoolTimeVisibility(bool InActive)
{
	_uiCoolTime->SetVisibility(InActive ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}

void UGsUIIconItemInventory::SetSkillOptionImage(FGsItem* InItemData)
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

void UGsUIIconItemInventory::OnButtonDown(const FGeometry& Geometry, const FPointerEvent& MouseEvent)
{
	OnButtonDownSlot.ExecuteIfBound(Geometry, MouseEvent);
}

void UGsUIIconItemInventory::OnButtonRelease()
{
	OnButtonReleaseSlot.ExecuteIfBound();
}

bool UGsUIIconItemInventory::IsDimmed()
{
	if (_dimmedImg)
	{
		return _dimmedImg->GetVisibility() != ESlateVisibility::Collapsed;
	}
	return false;
}