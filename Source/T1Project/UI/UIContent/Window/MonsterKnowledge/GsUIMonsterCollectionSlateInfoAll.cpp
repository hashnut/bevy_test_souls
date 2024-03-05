#include "GsUIMonsterCollectionSlateInfoAll.h"
#include "GsUIMonsterCollectionSlateAllStat.h"
#include "GsUIMonsterCollectionSlateProgressEntry.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlateNode.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlate.h"
#include "../MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "../Item/GsItemManager.h"
#include "../Management/GsMessageHolder.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "../Message/GsMessageContents.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIContent/Popup/GsUIPopupCommonItemConsume.h"
#include "../UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "../UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "../GameObject/Stat/GsStatHelper.h"
#include "UMG/Public/Components/ProgressBar.h"



void UGsUIMonsterCollectionSlateInfoAll::BeginDestroy()
{
	if (_btnTooltip)
	{
		_btnTooltip->OnClicked.RemoveDynamic(this, &UGsUIMonsterCollectionSlateInfoAll::OnTooltip);
	}
	_btnTooltip = nullptr;
	
	if (_slotHelperStatSet)
	{
		_slotHelperStatSet->OnRefreshEntry.RemoveDynamic(this, &UGsUIMonsterCollectionSlateInfoAll::OnRefreshStatSet);
		_slotHelperStatSet = nullptr;
	}

	if (_btnSlateResetAll)
	{
		_btnSlateResetAll->OnClicked.RemoveDynamic(this, &UGsUIMonsterCollectionSlateInfoAll::OnClickedResetAll);
	}
	//_btnSlateResetAll;

	Super::BeginDestroy();
}

void UGsUIMonsterCollectionSlateInfoAll::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnTooltip->OnClicked.AddDynamic(this, &UGsUIMonsterCollectionSlateInfoAll::OnTooltip);
	
	_slotHelperStatSet = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperStatSet->Initialize(_subClassOfStatEntry, _scrollBoxStatSet);
	_slotHelperStatSet->OnRefreshEntry.AddDynamic(this, &UGsUIMonsterCollectionSlateInfoAll::OnRefreshStatSet);

	_btnSlateResetAll->OnClicked.AddDynamic(this, &UGsUIMonsterCollectionSlateInfoAll::OnClickedResetAll);
}

void UGsUIMonsterCollectionSlateInfoAll::NativeConstruct()
{
	Super::NativeConstruct();

	_statInfoSet.Empty();
	_statValueSet.Empty();

	FText textToolTip;
	FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("UI_Slate_All_InfoButton_Text"), textToolTip);
	_tooltipSlateAll->SetDescText(textToolTip);
	_tooltipSlateAll->Close();
		
	const int32 maxCount = GsMonsterCollection()->SlateMaxCount();
	for (int32 i = 0; i < maxCount; ++i)
	{
		const FGsMonsterKnowledgeSlate* slateData = GsMonsterCollection()->SlateDataAt(i);
		if (nullptr == slateData)
		{
			continue;
		}

		for (int32 j = 0; j < slateData->NodeMaxCount(); ++j)
		{
			const FGsMonsterKnowledgeSlateNode* nodeData = slateData->NodeAt(j);
			if (nullptr == nodeData)
			{
				continue;
			}

			for (const auto& item : nodeData->StatSet())
			{	// 2023/2/8 PKT - Stat 중복 검사
				if (false == _statInfoSet.ContainsByPredicate([item](const TPair<StatType, FText>& InData)
					{
						return InData.Key == item.Key;
					}
				))
				{
					FText statName;
					FGsStatHelper::GetDisPlayStatName(item.Key, statName);
					// 2023/2/8 PKT - Add..
					_statInfoSet.Emplace(TPair<StatType, FText>(item.Key, statName));
				}				
			}
		}
	}

	_statInfoSet.Sort([](const TPair<StatType, FText>& lhr, const TPair<StatType, FText>& rhr)
		{
			return lhr.Key < rhr.Key;
		}
	);
}

void UGsUIMonsterCollectionSlateInfoAll::NativeDestruct()
{
	Super::NativeDestruct();

	_tooltipSlateAll->Close();
}

void UGsUIMonsterCollectionSlateInfoAll::OnClickedResetAll()
{
	if (false == _isResetEnable)
	{
		// 2021/12/28 PKT - 지식의 석판에 활성화 된 슬롯이 없습니다.
		FText message;
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("ticker_NoSlotToReset"), message);
		FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
		return;
	}

	ItemId resetCostId = GsMonsterCollection()->ResetCostId();
	ItemDBId resetCostDBId = INVALID_ITEM_DB_ID;

	const TArray<TWeakPtr<FGsItem>> Items = GItem()->FindByTID(resetCostId);
	if (Items.Num() && Items[0].IsValid())
	{
		resetCostDBId = Items[0].Pin().Get()->GetDBID();
	}

	TWeakObjectPtr<UGsUIPopupCommonItemConsume> popup = Cast<UGsUIPopupCommonItemConsume>(GUI()->OpenAndGetWidget(TEXT("PopupCommonItemConsume")).Get());
	if (popup.IsValid())
	{
		const static int32 USE_ITEM_AMOUNT = 1;

		FText textContent;
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slate_ResetPopupTitle"), textContent);

		FText textDesc;
		FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slate_ResetPopupDesc"), textDesc);
		FText textMsg = FText::Format(textDesc, USE_ITEM_AMOUNT);

		popup->SetDataNotCheckItem(textContent, textMsg, resetCostId, USE_ITEM_AMOUNT
			, [resetCostDBId](bool InIsOk)
			{
				if (InIsOk && INVALID_ITEM_DB_ID != resetCostDBId)
				{
					const FGsPrimitiveUInt64 message(resetCostDBId);
					GMessage()->GetMonsterKnowledgeCollection().SendMessage(MessageContentMonsterKnowledge::USE_ITEM_SLATE_RESET, &message);
				}
			}
		);
	}
}

void UGsUIMonsterCollectionSlateInfoAll::OnRefreshStatSet(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIMonsterCollectionSlateAllStat> item = Cast<UGsUIMonsterCollectionSlateAllStat>(InEntry);
	if (false == item.IsValid())
	{
		return;
	}

	if (false == _statInfoSet.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == _statInfoSet.IsValidIndex(InIndex) : [%d]"), InIndex);
		return;
	}

	if (false == _statValueSet.Contains(_statInfoSet[InIndex].Key))
	{
		GSLOG(Error, TEXT("false == _statValueSet.Contains(_statInfoSet[InIndex].Value : [%d]"), _statInfoSet[InIndex].Key);
		return;
	}

	TPair<StatValue, StatValue> data = _statValueSet[_statInfoSet[InIndex].Key];

	bool bUseSign = (0 < data.Key);

	FText textExpect;
	FGsStatHelper::GetDisPlayStatValueWithMarkFormat(_statInfoSet[InIndex].Key, StaticCast<int32>(data.Key), textExpect, bUseSign);
	
	FText textTotal;
	FGsStatHelper::GetDisPlayStatValueWithMarkFormat(_statInfoSet[InIndex].Key, StaticCast<int32>(data.Value), textTotal, false);

	item->SetData(_statInfoSet[InIndex].Value, textExpect, textTotal);
}

void UGsUIMonsterCollectionSlateInfoAll::OnTooltip()
{
	_tooltipSlateAll->ToggleOpenClose();
}

void UGsUIMonsterCollectionSlateInfoAll::BuildView()
{
	Invalidate();
}

void UGsUIMonsterCollectionSlateInfoAll::Invalidate()
{
	_isResetEnable = false;

	_statValueSet.Empty();

	const int32 slateMaxCount = GsMonsterCollection()->SlateMaxCount();
	int32 slateAcitveCount = 0;

	int32 nodeMaxCount = 0;
	int32 nodeCompletedCount = 0;

	TMap<EMonsterKnowledgeSlateCategory, TPair<int32, int32>> progressActiveNodeByCategory;

	for (int32 slateIndex = 0; slateIndex < slateMaxCount; ++slateIndex)
	{
		const FGsMonsterKnowledgeSlate* slateData = GsMonsterCollection()->SlateDataAt(slateIndex);
		if (nullptr == slateData)
		{
			continue;
		}

		if (EMonsterKnowledgeSlateState::UnLock == slateData->State())
		{
			++slateAcitveCount;
		}

		nodeMaxCount += slateData->NodeMaxCount();
		progressActiveNodeByCategory.FindOrAdd(slateData->CategoryType()).Value += slateData->NodeMaxCount();

		for (int32 nodeIndex = 0; nodeIndex < slateData->NodeMaxCount(); ++nodeIndex)
		{
			const FGsMonsterKnowledgeSlateNode* nodeData = slateData->NodeAt(nodeIndex);
			if (nullptr == nodeData)
			{
				continue;
			}

			for (const auto& item : nodeData->StatSet())
			{
				_statValueSet.FindOrAdd(item.Key).Value += item.Value;
			}

			if (EMonsterStoneSlateNodeState::Complete == nodeData->State())
			{
				progressActiveNodeByCategory.FindOrAdd(slateData->CategoryType()).Key += 1;

				nodeCompletedCount++;

				_isResetEnable = true;

				for (const auto& item : nodeData->StatSet())
				{
					_statValueSet.FindOrAdd(item.Key).Key += item.Value;
				}
			}
		}
	}

	_textSlateUnLockCount->SetText(FText::AsNumber(slateAcitveCount));
	_textSlateAllCount->SetText(FText::AsNumber(slateMaxCount));

	_textNodeAllCount->SetText(FText::AsNumber(nodeMaxCount));
	_textNodeCompletedCount->SetText(FText::AsNumber(nodeCompletedCount));

	// 2023/2/7 PKT - Stat Set
	_slotHelperStatSet->RefreshAll(_statInfoSet.Num());	

	auto DrawProgressNode = [progressActiveNodeByCategory](EMonsterKnowledgeSlateCategory InCategory, UGsUIMonsterCollectionSlateProgressEntry* OutEntry)
	{
		if (nullptr == OutEntry)
		{
			return;
		}

		float percent = 0.f;
			
		if (true == progressActiveNodeByCategory.Contains(InCategory))
		{
			float value = StaticCast<float>(progressActiveNodeByCategory[InCategory].Key);
			float totalValue = StaticCast<float>(progressActiveNodeByCategory[InCategory].Value);

			if (0.f >= totalValue)
			{
				percent = 0.f;
			}
			else
			{
				percent = FMath::Clamp(value / totalValue, 0.f, 1.0f);
			}			
		}

		OutEntry->SetData(InCategory, percent);
	};
	
	DrawProgressNode(EMonsterKnowledgeSlateCategory::ATTACK, _slateProgressAttack);		// 공격
	DrawProgressNode(EMonsterKnowledgeSlateCategory::DEFENCE, _slateProgressDefence);		// 방어
	DrawProgressNode(EMonsterKnowledgeSlateCategory::HIT, _slateProgressHit);			// 적중
	DrawProgressNode(EMonsterKnowledgeSlateCategory::RESISTANCE, _slateProgressResitance);	// 저항
	DrawProgressNode(EMonsterKnowledgeSlateCategory::SURVIVAL, _slateProgressSurvival);	// 생존
	DrawProgressNode(EMonsterKnowledgeSlateCategory::AMPLIFICATION, _slateProgressAmplification);	// 증폭
}