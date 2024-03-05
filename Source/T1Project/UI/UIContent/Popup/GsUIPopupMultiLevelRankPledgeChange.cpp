// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIPopupMultiLevelRankPledgeChange.h"

#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"

#include "Classes/PaperSprite.h"

#include "Currency/GsCurrencyHelper.h"

#include "GameObject/Stat/GsStatHelper.h"

#include "Management/ScopeGame/GsMultiLevelRankManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "UI/UIContent/Window/MultiLevelRank/GsUIMultiLevelRankStatEntry.h"
#include "UI/UIContent/Window/MultiLevelRank/GsUIMultiLevelRankIcon.h"
#include "UI/UIContent/Common/GsUICurrencyButton.h"
#include "UI/UIContent/Helper/GsEffectStringHelper.h"
#include "UI/UIContent/Helper/GsUIHelper.h"

#include "Item/GsItemManager.h"


void UGsUIPopupMultiLevelRankPledgeChange::BeginDestroy()
{
	if (nullptr != _btnClose)
	{
		_btnClose->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeChange::OnClickCancel);
	}
	_btnClose = nullptr;

	if (nullptr != _slotHelperLostStat)
	{
		_slotHelperLostStat->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeChange::OnRefreshLostStat);
	}
	_slotHelperLostStat = nullptr;

	if (nullptr != _slotHelperAcquiredStat)
	{
		_slotHelperAcquiredStat->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeChange::OnRefreshAcquiredStat);
	}
	_slotHelperAcquiredStat = nullptr;

	if (nullptr != _btnCancelChange)
	{
		_btnCancelChange->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeChange::OnClickCancel);
	}
	_btnCancelChange = nullptr;

	if (nullptr != _btnPledge)
	{
		_btnPledge->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeChange::OnClickPledge);
	}
	_btnPledge = nullptr;

	if (nullptr != _btnCancelPledge)
	{
		_btnCancelPledge->OnClicked.RemoveDynamic(this, &UGsUIPopupMultiLevelRankPledgeChange::OnClickCancel);
	}
	_btnCancelPledge = nullptr;

	Super::BeginDestroy();
}

void UGsUIPopupMultiLevelRankPledgeChange::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (nullptr == _slotHelperLostStat)
	{
		_slotHelperLostStat = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperLostStat->Initialize(_subClassOfLostStat, _scrollLostStat);
		_slotHelperLostStat->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeChange::OnRefreshLostStat);
	}

	if (nullptr == _slotHelperAcquiredStat)
	{
		_slotHelperAcquiredStat = NewObject<UGsDynamicPanelSlotHelper>(this);
		_slotHelperAcquiredStat->Initialize(_subClassOfAcquiredStat, _scrollAcquiredStat);
		_slotHelperAcquiredStat->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeChange::OnRefreshAcquiredStat);
	}

	_btnChange->OnClickCurrencyButton.BindUObject(this, &UGsUIPopupMultiLevelRankPledgeChange::OnClickChange);
	_btnPledge->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeChange::OnClickPledge);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeChange::OnClickCancel);
	_btnCancelChange->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeChange::OnClickCancel);
	_btnCancelPledge->OnClicked.AddDynamic(this, &UGsUIPopupMultiLevelRankPledgeChange::OnClickCancel);
}

void UGsUIPopupMultiLevelRankPledgeChange::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIPopupMultiLevelRankPledgeChange::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIPopupMultiLevelRankPledgeChange::OnClickChange()
{
	const FGsMultiLevelRankSlotData* targetRankSlot = GetSelectedPledgeTopLevelRankSlot();
	if (nullptr == targetRankSlot)
		return;

	const FGsMultiLevelRankData* targetRankData = targetRankSlot->_ownerRank;
	if (nullptr == targetRankData)
		return;

	if (targetRankData->_rankSchema->costType == CostType::CURRENCY)
	{
		if (false == FGsCurrencyHelper::CheckCurrency(targetRankData->_rankSchema->currencyType, targetRankData->_rankSchema->currencyCost, false))
		{
			// 요구 재화가 부족해, 서약을 변경할 수 없습니다.
			FText textFormat;
			if (FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Ticker_NoMoney"), textFormat))
			{
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textFormat);
			}
			return;
		}
	}
	else if (targetRankData->_rankSchema->costType == CostType::ITEM)
	{
		ItemAmount costItemAmount = targetRankData->_rankSchema->costItemAmount;
		if (GItem()->GetAllItemCountByTID(targetRankData->_rankSchema->costItemId) < costItemAmount)
		{
			// 요구 아이템이 부족해, 서약을 변경할 수 없습니다.
			FText textFormat;
			if (FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Ticker_NoItem"), textFormat))
			{
				/*const FGsSchemaItemCommon* tableData = FGsItem::GetTableDataByID(targetRankData->_rankSchema->costItemId);
				if (nullptr == tableData)
					return;

				FText textMsg = FText::Format(textFormat, tableData->name);*/
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, textFormat);
			}
			return;
		}
	}
	else
	{
		return;
	}

	MultiLevelRankId rankId = targetRankSlot->_slotSchema->rankId;
	GSMultiLevelRank()->SendRankChange(rankId);
	Close();
}

void UGsUIPopupMultiLevelRankPledgeChange::OnClickPledge()
{
	if (FGsMultiLevelRankManager* rankManager = GSMultiLevelRank())
	{
		const FGsMultiLevelRankSlotData* targetRankSlot = GetSelectedPledgeTopLevelRankSlot();
		if (nullptr == targetRankSlot)
			return;

		do
		{
			// send...
			const FGsSchemaQuestMultiLevelRankStory* story = targetRankSlot->_slotSchema->storyId.GetRow();
			if (nullptr == story)
			{
				return;
			}

			GSMultiLevelRank()->SendQuestAccept(story->id);

		} while (0);
	}

	GUI()->CloseByKeyName(TEXT("PopupMultiLevelRankPledgeSelect"));
	Close();
}

void UGsUIPopupMultiLevelRankPledgeChange::OnClickCancel()
{
	GUI()->Open(TEXT("PopupMultiLevelRankPledgeSelect"));
	Close();
}

void UGsUIPopupMultiLevelRankPledgeChange::OnRefreshLostStat(int32 InIndex, UWidget* InEntry)
{
	if (false == _refreshLostStatBuffer.IsValidIndex(InIndex))
		return;

	const TPair<StatType, int32>& statValue = _refreshLostStatBuffer[InIndex];

	FText statName;
	if (false == FGsStatHelper::GetDisPlayStatName(statValue.Key, statName))
		return;

	TWeakObjectPtr<UGsUIMultiLevelRankStatEntry> item = Cast<UGsUIMultiLevelRankStatEntry>(InEntry);
	if (item.IsValid())
	{
		FText textStat;
		int32 statPlusValue = (statValue.Value > 0) ? statValue.Value : statValue.Value * -1;
		FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statValue.Key, statPlusValue, textStat);

		item->SetData(statName, textStat);
	}
}

void UGsUIPopupMultiLevelRankPledgeChange::OnRefreshAcquiredStat(int32 InIndex, UWidget* InEntry)
{
	if (false == _refreshAcquiredStatBuffer.IsValidIndex(InIndex))
		return;

	const TPair<StatType, int32>& statValue = _refreshAcquiredStatBuffer[InIndex];

	FText statName;
	if (false == FGsStatHelper::GetDisPlayStatName(statValue.Key, statName))
		return;

	TWeakObjectPtr<UGsUIMultiLevelRankStatEntry> item = Cast<UGsUIMultiLevelRankStatEntry>(InEntry);
	if (item.IsValid())
	{
		FText textStat;
		FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statValue.Key, statValue.Value, textStat);

		item->SetData(statName, textStat);
	}
}

void UGsUIPopupMultiLevelRankPledgeChange::ComputeChangedStat(const FGsMultiLevelRankSlotData* InTargetSlot)
{
	const FGsMultiLevelRankSlotData* currentSlot = GSMultiLevelRank()->GetActiveLastSlot();
	if (nullptr == currentSlot)
		return;

	FGsEffectStringHelper::FGsEffectAddData outCurrentStatList;
	FGsEffectStringHelper::FGsEffectAddData outTargetStatList;

	// 해당 슬롯까지 획득 가능한 스탯 목록 얻기
	GetAddedStat(currentSlot, outCurrentStatList);
	GetAddedStat(InTargetSlot, outTargetStatList);

	// 변경 예정 - 현재 슬롯의 차이 계산
	for (const auto& data : outCurrentStatList._statMap)
	{
		if (outTargetStatList._statMap.Contains(data.Key))
		{
			outTargetStatList._statMap[data.Key] -= data.Value;
		}
		else
		{
			outTargetStatList._statMap.Emplace(data.Key, -data.Value);
		}
	}

	_refreshAcquiredStatBuffer.Empty();
	_refreshLostStatBuffer.Empty();

	// 획득, 상실 스탯 분류
	for (const auto& data : outTargetStatList._statMap)
	{
		if (data.Key != StatType::NONE)
		{
			if (data.Value > 0)
			{
				_refreshAcquiredStatBuffer.Emplace(data.Key, data.Value);
			}
			else if (data.Value < 0)
			{
				_refreshLostStatBuffer.Emplace(data.Key, data.Value);
			}
		}
	}

	// 정렬
	_refreshAcquiredStatBuffer.Sort([=](const TPair<StatType, int32>& source, const TPair<StatType, int32>& dest)->bool
		{
			return source.Key < dest.Key;
		});
	_refreshLostStatBuffer.Sort([=](const TPair<StatType, int32>& source, const TPair<StatType, int32>& dest)->bool
		{
			return source.Key < dest.Key;
		});
}

void UGsUIPopupMultiLevelRankPledgeChange::GetAddedStat(const FGsMultiLevelRankSlotData* InTargetSlot, FGsEffectStringHelper::FGsEffectAddData& OutStatList)
{
	const FGsMultiLevelRankSlotData* findSlot = InTargetSlot;
	while (true)
	{
		if (nullptr == findSlot)
			break;

		for (const FGsSchemaPassivitySetRow& SetRow : findSlot->_slotSchema->passivityIdList)
		{
			FGsEffectStringHelper::AddPassivityEffect(SetRow.GetRow(), OutStatList);
		}

		// prev...
		findSlot = findSlot->_linkedPrevSlot;
	}
}

const FGsMultiLevelRankSlotData* UGsUIPopupMultiLevelRankPledgeChange::GetSelectedPledgeTopLevelRankSlot()
{
	if (FGsMultiLevelRankManager* rankManager = GSMultiLevelRank())
	{
		const FGsMultiLevelRankSlotData* currentSlot = rankManager->GetActiveLastSlot();
		if (nullptr == currentSlot)
			return nullptr;

		// 서약 후 달성한 랭크가 있는 상태
		MultiLevelRank rank = currentSlot->_ownerRank->_rankSchema->rank;

		// 서약 선택만 된 상태 : 서약 후 첫 랭크 퀘스트 진행 중
		if (rankManager->GetPledgePrevSlot()->_ownerRank->_rankSchema->rank == currentSlot->_ownerRank->_rankSchema->rank)
		{
			rank += 1;
		}
		
		return rankManager->GetRankSlot(rank, _selectedGroupId);
	}

	return nullptr;
}

void UGsUIPopupMultiLevelRankPledgeChange::SetNextPledge(int32 InGroupId)
{
	_selectedGroupId = InGroupId;

	InvalidateInfo();
	InvalidateStat();
}

void UGsUIPopupMultiLevelRankPledgeChange::InvalidateInfo()
{
	const FGsMultiLevelRankSlotData* targetRankSlot = GetSelectedPledgeTopLevelRankSlot();
	if (nullptr == targetRankSlot)
		return;

	const FGsMultiLevelRankData* targetRankData = targetRankSlot->_ownerRank;
	if (nullptr == targetRankData)
		return;

	FGsMultiLevelRankManager* rankManager = GSMultiLevelRank();
	if (nullptr == rankManager)
		return;

	if (rankManager->IsPledgeSelected())
	{
		// 서약 후 달성한 슬롯 정보 사용
		const FGsMultiLevelRankSlotData* prevSlot = rankManager->GetActiveLastSlot();
		if (nullptr == prevSlot)
			return;

		// 서약 후 달성한 슬롯이 없을 경우 현재 진행중인 슬롯 정보 사용
		if (rankManager->GetPledgePrevSlot()->_ownerRank->_rankSchema->rank == prevSlot->_ownerRank->_rankSchema->rank)
		{
			prevSlot = rankManager->GetQuestInProgressSlot();
			if (nullptr == prevSlot)
				return;
		}

		FText titleText;
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Popup_ChangePledge_Title"), titleText);
		_textBlockName->SetText(titleText);

		_switcherSelectOrChange->SetActiveWidgetIndex(0);

		_textBlockNamePrev->SetText(prevSlot->_slotSchema->NameText);
		_textBlockNameNext->SetText(targetRankSlot->_slotSchema->NameText);

		_imageMainIconPrev->SetMainImage(prevSlot->_slotSchema->iconPath);
		_imageMainIconPrev->SetSubImage(prevSlot->_slotSchema->subIconPath);
		_imageMainIconPrev->SetSubImageCount(prevSlot->_slotSchema->subIconCount);

		_imageMainIconNext->SetMainImage(targetRankSlot->_slotSchema->iconPath);
		_imageMainIconNext->SetSubImage(targetRankSlot->_slotSchema->subIconPath);
		_imageMainIconNext->SetSubImageCount(targetRankSlot->_slotSchema->subIconCount);

		if (targetRankData->_rankSchema->costType == CostType::CURRENCY)
		{
			_btnChange->SetData(targetRankData->_rankSchema->currencyType, targetRankData->_rankSchema->currencyCost);
			_btnChange->UpdateAmountTextColorLocalPlayer();
		}
		else if (targetRankData->_rankSchema->costType == CostType::ITEM)
		{
			_btnChange->SetData(targetRankData->_rankSchema->costItemId, targetRankData->_rankSchema->costItemAmount);

			ItemAmount costItemAmount = targetRankData->_rankSchema->costItemAmount;
			bool bIsEnough = (GItem()->GetAllItemCountByTID(targetRankData->_rankSchema->costItemId) >= costItemAmount);
			_btnChange->SetAmountTextColor(bIsEnough ? EGsUIColorType::DEFAULT_ENOUGH : EGsUIColorType::DEFAULT_LACK);

		}
		FText buttonNameText;
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Popup_ChangePledge_Confirm"), buttonNameText);
		_btnChange->SetButtonName(buttonNameText);
	}
	else
	{
		FText titleText;
		FText::FindText(TEXT("MultiLevelRank"), TEXT("UI_Popup_SelectPledge_Title"), titleText);
		_textBlockName->SetText(titleText);

		_switcherSelectOrChange->SetActiveWidgetIndex(1);

		_textBlockNameSelect->SetText(targetRankSlot->_slotSchema->NameText);

		_imageMainIconSelect->SetMainImage(targetRankSlot->_slotSchema->iconPath);
		_imageMainIconSelect->SetSubImage(targetRankSlot->_slotSchema->subIconPath);
		_imageMainIconSelect->SetSubImageCount(targetRankSlot->_slotSchema->subIconCount);
	}
}

void UGsUIPopupMultiLevelRankPledgeChange::InvalidateStat()
{
	const FGsMultiLevelRankSlotData* targetRankSlot = GetSelectedPledgeTopLevelRankSlot();
	if (nullptr == targetRankSlot)
		return;

	ComputeChangedStat(targetRankSlot);

	_panelLostEmpty->SetVisibility((_refreshLostStatBuffer.Num() == 0) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	_scrollLostStat->SetVisibility((_refreshLostStatBuffer.Num() == 0) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	_panelAcquiredEmpty->SetVisibility((_refreshAcquiredStatBuffer.Num() == 0) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	_scrollAcquiredStat->SetVisibility((_refreshAcquiredStatBuffer.Num() == 0) ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	
	_slotHelperAcquiredStat->RefreshAll(_refreshAcquiredStatBuffer.Num());
	_slotHelperLostStat->RefreshAll(_refreshLostStatBuffer.Num());
}

void UGsUIPopupMultiLevelRankPledgeChange::OnInputCancel()
{
	OnClickCancel();
}