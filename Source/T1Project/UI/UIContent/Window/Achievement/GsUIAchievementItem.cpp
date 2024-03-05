#include "GsUIAchievementItem.h"

#include "Engine/AssetManager.h"

#include "Achievement/GsAchievementData.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIContent/Common/GsUIProgressBar.h"

#include "TextBlock.h"
#include "Image.h"
#include "WidgetSwitcher.h"

void UGsUIAchievementItem::BeginDestroy()
{
	if (nullptr != _rewardIconSlotHelper)
	{
		_rewardIconSlotHelper->OnCreateIcon.RemoveDynamic(this, &UGsUIAchievementItem::OnCreateRewardIcon);
		_rewardIconSlotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIAchievementItem::OnRefreshRewardIcon);
		_rewardIconSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIAchievementItem::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_rewardIconSelector->RemoveAllChildren();

	_rewardIconSlotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_rewardIconSlotHelper->Initialize(_rewardIconSelector);
	_rewardIconSlotHelper->OnCreateIcon.AddDynamic(this, &UGsUIAchievementItem::OnCreateRewardIcon);
	_rewardIconSlotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIAchievementItem::OnRefreshRewardIcon);
}

void UGsUIAchievementItem::NativeDestruct()
{
	_rewardIconSlotHelper->Clear();
	_rewardIconSelector->RemoveAllChildren();

	Super::NativeDestruct();
}

void UGsUIAchievementItem::RefreshUI()
{
	if (auto achievement = _data.Pin())
	{
		if (UObject* loadIcon = UAssetManager::GetStreamableManager().LoadSynchronous(achievement->IconPath()))
		{
			_achievementIcon->SetBrushResourceObject(loadIcon);
		}

		auto rewardCount = 0;
		const auto rewards = achievement->GetRewards();
		if (rewards.IsValidIndex(0))
		{
			rewardCount = rewards.Num();
		}
		_rewardIconSlotHelper->RefreshAll(rewardCount);

		_textTitle->SetText(achievement->Title());
		_textDesc->SetText(achievement->Description());
		_textLevel->SetText(FText::AsNumber(achievement->GetLevel()));

		_completeSwitcher->SetActiveWidgetIndex(static_cast<int32>(achievement->AchievementProcessState()));
	}
}

void UGsUIAchievementItem::OnCreateRewardIcon(UGsUIIconBase* InIcon)
{
	UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);
	if (itemIcon)
	{
		itemIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIAchievementItem::OnLongPressIcon);
	}
}

void UGsUIAchievementItem::OnRefreshRewardIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	if (auto achievement = _data.Pin())
	{
		const auto rewards = achievement->GetRewards();
		UGsUIIconItemReward* itemIcon = Cast<UGsUIIconItemReward>(InIcon);

		InIndex = rewards.Num() - InIndex - 1;
		if (itemIcon && rewards.IsValidIndex(InIndex))
		{
			if (const FGsSchemaRewardItemBagData* itemData = rewards[InIndex])
			{
				if (itemData->itemId != INVALID_ITEM_ID)
				{
					itemIcon->SetEnableDetailPopup(true);
					itemIcon->SetItemNameToolTipFlag(true);
					itemIcon->SetNotOwnItem(itemData->itemId, itemData->itemCountMin);
				}
				else if (itemData->currencyType != CurrencyType::NONE)
				{
					itemIcon->SetEnableDetailPopup(false);
					itemIcon->SetItemNameToolTipFlag(true);
					itemIcon->SetCurrency(itemData->currencyType, itemData->currencyAmountMin);
				}
			}
		}
	}
}


void UGsUIAchievementItem::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

