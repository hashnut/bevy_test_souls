// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/MultiLevelRank/GsUIMultiRankRewardData.h"
#include "UI/UIContent/Common/Icon/GsUIIconItem.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"
#include "UI/UIContent/Define/GsUIContentDefine.h"
#include "UI/UIContent/Define/EGsUIColorType.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"

#include "Item/GsItem.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/TextBlock.h"
#include "T1Project.h"

void UGsUIMultiRankRewardData::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshIcon.RemoveDynamic(this, &UGsUIMultiRankRewardData::OnRefreshIcon);
		_slotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIMultiRankRewardData::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_slotHelper = NewObject<UGsDynamicIconSlotHelper>(this);
	_slotHelper->Initialize(_iconSelector);
	_slotHelper->OnRefreshIcon.AddDynamic(this, &UGsUIMultiRankRewardData::OnRefreshIcon);

	_rewardData._type = FGsRefreshRewardBuffer::Type::None;
	_rewardData._id = FGsRefreshRewardBuffer::Invaild_Value;
	_rewardData._minCount = FGsRefreshRewardBuffer::Invaild_Value;
	_rewardData._maxCount = FGsRefreshRewardBuffer::Invaild_Value;

	_toggleOnEffect->SetVisibility(ESlateVisibility::Collapsed);
}

void UGsUIMultiRankRewardData::OnRefreshIcon(int32 InIndex, UGsUIIconBase* InIcon)
{
	/**
	 * 2021/06/30 PKT
	 * 현재 보상 정책 변경으로 재화/아이템 Amount에 Min Max가 생겼으나 아직 정상적으로 적용되지 않아, min 값을 기준으로 출력
	 * 추후 이 부분은 수정 되어야 한다.,
	 */
	TWeakObjectPtr<UGsUIIconItemReward> icon = Cast<UGsUIIconItemReward>(InIcon);
	if (true == icon.IsValid())
	{
		switch (_rewardData._type)
		{
			case FGsRefreshRewardBuffer::Type::None:
			{
#if WITH_EDITOR
				GSLOG(Warning, TEXT("UGsUIMultiRankRewardData::OnRefreshIcon : invaild data"));
#endif
			}	break;
			case FGsRefreshRewardBuffer::Type::Item:
			{
				icon->SetNotOwnItem(_rewardData._id, _rewardData._minCount);
				icon->SetEnableDetailPopup(true);
				icon->SetCenterPopup(true);
				icon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
				icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIMultiRankRewardData::OnFinishedLongPressEventItemIcon);

				// 2021/06/22 PKT - Set Text Name
				FGsItem* itemData = icon->GetItemIconData();
				_textName->SetText(itemData->GetName());
				_textName->SetColorAndOpacity(FGsUIColorHelper::GetColorItemGrade(itemData->GetGradeType()));
			} break;
			case FGsRefreshRewardBuffer::Type::Currency:
			{
				icon->SetCurrency(static_cast<CurrencyType>(_rewardData._id), static_cast<Currency>(_rewardData._minCount));
				icon->SetEnableDetailPopup(false);
				icon->SetOnChangeDisplayDetailWndType(UGsUIIconItem::UseDisplayDetailWndType::LongPressType);
				icon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIMultiRankRewardData::OnFinishedLongPressEventItemIcon);

				// 2021/06/22 PKT - Set Text Name
				FText outText;
				FGsUIStringHelper::GetAmountText(_rewardData._minCount, MAX_uint64, outText);
				_textName->SetText(outText);
				_textName->SetColorAndOpacity(FGsUIColorHelper::GetColor(EGsUIColorType::NORMAL_TEXT_COLOR));
			} break;
		}
	}
}

void UGsUIMultiRankRewardData::InvalidateData(const FGsRefreshRewardBuffer& InRewardData)
{
	_rewardData = InRewardData;

	// 2021/06/22 PKT - 한개의 보상 아이콘이 존재한다.
	const static int32 MaxIconCount = 1;
	_slotHelper->RefreshAll(MaxIconCount);
}

void UGsUIMultiRankRewardData::OnFinishedLongPressEventItemIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}
