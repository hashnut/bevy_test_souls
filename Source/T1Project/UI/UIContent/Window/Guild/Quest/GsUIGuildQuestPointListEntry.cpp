// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIGuildQuestPointListEntry.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"
#include "UI/UIContent/Common/Icon/GsUIIconItemReward.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "DataSchema/Guild/GsSchemaGuildZealPointReward.h"
#include "DataSchema/Guild/GsSchemaGuildLevelDetailData.h"
#include "DataSchema/Reward/GsSchemaRewardData.h"
#include "Reward/GsRewardHelper.h"
#include "Reward/GsRewardIconDataWrapper.h"


void UGsUIGuildQuestPointListEntry::SetData(const FGsSchemaGuildZealPointReward* InData)
{
	_zealPointLevel = InData->level;

	if (const FGsSchemaRewardData* table = InData->guildRewardId.GetRow())
	{
		// 아이콘 세팅
		if (UGsUIIconItemReward* icon = GetRewardIcon())
		{
			FGsRewardUnboxingData unboxData;
			FGsRewardHelper::UnboxingRewardData(table, unboxData);
			unboxData._bUseCurrencyIcon = true;
			TArray<FGsRewardIconDataWrapper> iconDataList;
			FGsRewardHelper::GetIconDataList(&unboxData, iconDataList);

			if (0 < iconDataList.Num())
			{
				FGsRewardIconDataWrapper::SetUIIconItem(&iconDataList[0], icon);
			}
		}
	}

	_textBlockAmount->SetText(FText::AsNumber(InData->point));
}

void UGsUIGuildQuestPointListEntry::SetState(EGsGuildQuestPointState InState)
{
	_state = InState;

	switch (_state)
	{
	case EGsGuildQuestPointState::REWARDED:
	{
		_imgLock->SetVisibility(ESlateVisibility::Collapsed);

		if (UGsUIIconItemReward* icon = GetRewardIcon())
		{
			icon->SetIsRewarded(true);
			icon->SetIsOpenReward(true);
		}
	}
	break;
	case EGsGuildQuestPointState::LOCKED:
	{
		_imgLock->SetVisibility(ESlateVisibility::HitTestInvisible);

		if (UGsUIIconItemReward* icon = GetRewardIcon())
		{
			icon->SetIsRewarded(false);
			icon->SetIsOpenReward(false);
		}
	}
	break;
	case EGsGuildQuestPointState::ACTIVE:
	{
		_imgLock->SetVisibility(ESlateVisibility::Collapsed);

		if (UGsUIIconItemReward* icon = GetRewardIcon())
		{
			icon->SetIsRewarded(false);
			icon->SetIsOpenReward(true);
		}
	}
	break;
	}
}

void UGsUIGuildQuestPointListEntry::SetProgressBar(float InRate)
{
	_progressBar->SetPercent(InRate);
}

void UGsUIGuildQuestPointListEntry::OnClickSlot(UGsUIIconItem& InIcon)
{
	if (EGsGuildQuestPointState::LOCKED != _state)
	{
		return;
	}	

	// 기사단 레벨 {0} 달성 시, 획득할 수 있는 보상입니다.
	int32 guildLevel = FindRewardOpenGuildLevel(_zealPointLevel);

	// TEXT: 기사단 레벨 {0} 달성 시, 획득하실 수 있는 보상입니다.
	FText textFormat;
	if (FText::FindText(TEXT("GuildText"), TEXT("GuildQuest_UI_Ticker_NotEnoughGuildLevel"), textFormat))
	{
		FText textMsg = FText::Format(textFormat, guildLevel);
		FGsUIHelper::TrayMessageTicker(textMsg);
	}
}

void UGsUIGuildQuestPointListEntry::OnLongPressIcon(class UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

UGsUIIconItemReward* UGsUIGuildQuestPointListEntry::GetRewardIcon()
{
	if (0 == _iconSelectorReward->GetChildrenCount())
	{
		UGsUIIconBase* newIcon = _iconSelectorReward->CreateIcon();
		_iconSelectorReward->AddChild(newIcon);
	}

	UGsUIIconItemReward* outIcon = Cast<UGsUIIconItemReward>(_iconSelectorReward->GetChildAt(0));
	if (outIcon)
	{
		outIcon->SetEnableDetailPopup(true);
		outIcon->SetCenterPopup(true);

		if (false == outIcon->OnClickSlot.IsBoundToObject(this))
		{
			outIcon->OnClickSlot.AddUObject(this, &UGsUIGuildQuestPointListEntry::OnClickSlot);
		}

		if (false == outIcon->OnFinishedLongPressEvent.IsBoundToObject(this))
		{
			outIcon->OnFinishedLongPressEvent.AddUObject(this, &UGsUIGuildQuestPointListEntry::OnLongPressIcon);
		}
	}

	return outIcon;
}

int32 UGsUIGuildQuestPointListEntry::FindRewardOpenGuildLevel(int32 InZealPointLevel)
{
	TArray<const FGsSchemaGuildLevelDetailData*> rowList;
	if (const UGsTable* table = FGsSchemaGuildLevelDetailData::GetStaticTable())
	{
		table->GetAllRows(rowList);

		for (const FGsSchemaGuildLevelDetailData* iter : rowList)
		{
			if (iter->maxZealPointLevel >= InZealPointLevel)
			{
				return iter->level;
			}
		}
	}

	return 0;
}
