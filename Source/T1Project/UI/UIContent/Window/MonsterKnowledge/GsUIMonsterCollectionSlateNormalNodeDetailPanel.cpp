#include "GsUIMonsterCollectionSlateNormalNodeDetailPanel.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlate.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlateNode.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Item/GsItem.h"
#include "../GameObject/Stat/GsStatHelper.h"
#include "GsUIMonsterCollectionSlateStatEntry.h"
#include "../../Common/GsUICurrencyButton.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "../UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "../MonsterKnowledgeCollection/EMonsterKnowledgeEnumData.h"
#include "../Currency/GsCurrencyHelper.h"
#include "../../Popup/GsUIPopupCommonItemConsume.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Net/GsNetSendServiceItem.h"




void UGsUIMonsterCollectionSlateNormalNodeDetailPanel::BeginDestroy()
{
	if (_statSetSlotHelper)
	{
		_statSetSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIMonsterCollectionSlateNormalNodeDetailPanel::OnRefreshStatSet);
		_statSetSlotHelper = nullptr;
	}	

	if (_btnCheckNodeAllSelected)
	{
		_btnCheckNodeAllSelected->OnClicked.RemoveDynamic(this, &UGsUIMonsterCollectionSlateNormalNodeDetailPanel::OnChangedCheckBox);
		_btnCheckNodeAllSelected = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIMonsterCollectionSlateNormalNodeDetailPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnOpenSlate->OnClicked.AddDynamic(this, &UGsUIMonsterCollectionSlateNormalNodeDetailPanel::OnClickedOpenSlate);
	_btnCurrencyActiveNode->OnClickCurrencyButton.BindUObject(this, &UGsUIMonsterCollectionSlateNormalNodeDetailPanel::OnClickedActiveNode);
	_btnCheckNodeAllSelected->OnClicked.AddDynamic(this, &UGsUIMonsterCollectionSlateNormalNodeDetailPanel::OnChangedCheckBox);

	_statSetSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_statSetSlotHelper->Initialize(_statWidgetClass, _statSetRootPanel);
	_statSetSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIMonsterCollectionSlateNormalNodeDetailPanel::OnRefreshStatSet);
}

void UGsUIMonsterCollectionSlateNormalNodeDetailPanel::NativeConstruct()
{
	Super::NativeConstruct();

	_focusNodeId = INVALID_MONSTER_KNOWLEDGE_SLATE_SLOT_ID;

	_statRateDataMap.Empty(0);
	_statSortOrderData.Empty(0);
}

void UGsUIMonsterCollectionSlateNormalNodeDetailPanel::OnRefreshStatSet(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIMonsterCollectionSlateStatEntry> item = Cast<UGsUIMonsterCollectionSlateStatEntry>(InEntry);
	if (false == item.IsValid() || false == _statSortOrderData.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == item.IsValid() || false == _statSortOrderData.IsValidIndex(InIndex)"));
		return;
	}

	StatType statType = _statSortOrderData[InIndex];

	int32 CompletedValue = _statRateDataMap.Contains(statType) ? _statRateDataMap[statType].Key : 0;
	int32 IncreaseValue = _statRateDataMap.Contains(statType) ? _statRateDataMap[statType].Value : 0;

	bool bUseSign = (0 < IncreaseValue);

	FText textCompletedValue;
	FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statType, CompletedValue, textCompletedValue, 0 >= CompletedValue ? false : true);

	FText textIncreaseVale;
	if (0 < IncreaseValue)
	{
		IncreaseValue += CompletedValue;
		FGsStatHelper::GetDisPlayStatValueWithMarkFormat(statType, IncreaseValue, textIncreaseVale, bUseSign);
	}
	else
	{
		textIncreaseVale = FText::GetEmpty();
	}

	FText statName;
	FGsStatHelper::GetDisPlayStatName(statType, statName);

	item->SetData(statName, textCompletedValue, textIncreaseVale);
}

void UGsUIMonsterCollectionSlateNormalNodeDetailPanel::OnClickedOpenSlate()
{
	if (_callbackFunc)
	{
		_callbackFunc(EMonsterCollectionSlateCallbackReason::ReqOpenSlate, 0);
	}
}

void UGsUIMonsterCollectionSlateNormalNodeDetailPanel::OnClickedActiveNode()
{
	if (_callbackFunc)
	{
		_callbackFunc(EMonsterCollectionSlateCallbackReason::ReqActiveNode, _btnCurrencyActiveNode->GetCurrencyAmount());
	}
}

void UGsUIMonsterCollectionSlateNormalNodeDetailPanel::OnChangedCheckBox()
{
	int32 value = (0 == _btnCheckNodeAllSelected->GetSwitcherIndex()) ? 1 : 0;
	_btnCheckNodeAllSelected->SetSwitcherIndex(value);

	if (_callbackFunc)
	{
		_callbackFunc(EMonsterCollectionSlateCallbackReason::ChangedSelectedAll, value);
	}
}

void UGsUIMonsterCollectionSlateNormalNodeDetailPanel::SetData(const class FGsMonsterKnowledgeSlate* InData, TFunction<void(EMonsterCollectionSlateCallbackReason, uint64)> InCallback)
{
	Super::SetData(InData, InCallback);

	_detailType = ENodeDetailType::Normal;

	_focusNodeId = INVALID_MONSTER_KNOWLEDGE_SLATE_SLOT_ID;

	_textSlateTitle->SetText(InData->Title());	
}

void UGsUIMonsterCollectionSlateNormalNodeDetailPanel::Invalidate()
{
	Super::Invalidate();

	const FGsMonsterKnowledgeSlate* slateData = FindData();
	if (nullptr == slateData)
	{
		GSLOG(Error, TEXT("nullptr == slateData"));
		return;
	}

	const FGsMonsterKnowledgeSlateNode* focusNodeData = (INVALID_MONSTER_KNOWLEDGE_SLATE_SLOT_ID != _focusNodeId) ? slateData->FindNodeById(_focusNodeId) : nullptr;

	_statRateDataMap.Empty(0);

	// 2023/11/8 PKT - 완료한 Node 갯수
	int32 completedCount = 0;
	// 2023/11/8 PKT - 현재 선택되어진 Node까지 모든 연결되어 있는 Node를 얻어온다.
	TArray<const FGsMonsterKnowledgeSlateNode*> chainNodeSet = slateData->ChainNodeSet(focusNodeData).Array();

	const static int32 SWITCHER_SELECTED_INDEX = 1;
	const bool isCheckedNodeAll = (SWITCHER_SELECTED_INDEX == _btnCheckNodeAllSelected->GetSwitcherIndex());

	for (int32 idx = 0; idx < slateData->NodeMaxCount(); ++idx)
	{
		const FGsMonsterKnowledgeSlateNode* nodeData = slateData->NodeAt(idx);
		if (nullptr == nodeData)
		{
			continue;
		}

		if (nodeData && EMonsterStoneSlateNodeState::Complete == nodeData->State())
		{
			++completedCount;

			for (const auto& item : nodeData->StatSet())
			{	// 2023/4/24 PKT - 완성한 것은 모두 기록				
				_statRateDataMap.FindOrAdd(item.Key).Key += item.Value;
			}
		}

		if (false == isCheckedNodeAll && nullptr == chainNodeSet.FindByPredicate(
			[nodeData](const FGsMonsterKnowledgeSlateNode* InData) {
				return nodeData->Id() == InData->Id();
			}
		))
		{
			continue;
		}

			if (EMonsterStoneSlateNodeState::Complete != nodeData->State())
			{
				for (const auto& item : nodeData->StatSet())
				{
					_statRateDataMap.FindOrAdd(item.Key).Value += item.Value;
				}
			}
	}

	_statSortOrderData.Empty();
	for (const auto& item : _statRateDataMap)
	{
		_statSortOrderData.Emplace(item.Key);
	}

	_statSortOrderData.Sort([](const StatType lhr, const StatType rhr)
		{
			return lhr < rhr;
		}
	);

	// 2023/2/9 PKT - 달성 현황
	_textCompletedCount->SetText(FText::AsNumber(completedCount));
	_textAllCount->SetText(FText::AsNumber(slateData->NodeMaxCount()));

	_statSetSlotHelper->RefreshAll(_statSortOrderData.Num());


	InvalidateCurrency();
}

void UGsUIMonsterCollectionSlateNormalNodeDetailPanel::SetDetailData(const class FGsMonsterKnowledgeSlateNode* InFocusNode)
{
	_focusNodeId = (nullptr != InFocusNode) ? InFocusNode->Id() : INVALID_MONSTER_KNOWLEDGE_SLATE_SLOT_ID;
	
	Invalidate();
}

void UGsUIMonsterCollectionSlateNormalNodeDetailPanel::InvalidateCurrency()
{
	const FGsMonsterKnowledgeSlate* slateData = FindData();
	if (nullptr == slateData)
	{
		GSLOG(Error, TEXT("nullptr == slateData"));
		return;
	}

	const FGsMonsterKnowledgeSlateNode* focusNodeData = (INVALID_MONSTER_KNOWLEDGE_SLATE_SLOT_ID != _focusNodeId) ? slateData->FindNodeById(_focusNodeId) : nullptr;
	if (nullptr == focusNodeData)
	{
		return;
	}

	if (EMonsterKnowledgeSlateState::Lock == slateData->State())
	{
		/**
		 * 지식의 석판 열기 버튼 활성화
		 */
		const static int32 SWITCHER_CLOSE_SLATE = 1;
		_switchApplyButton->SetActiveWidgetIndex(SWITCHER_CLOSE_SLATE);

		// 2023/2/9 PKT - 지식의 석판 열기 버튼
		_btnOpenSlate->SetIsEnabled(0 < slateData->HaveCandinateCostAmount());

		return;
	}
	else
	{
		// 2023/11/8 PKT - 완료한 Node 갯수
		int32 completedCount = 0;
		Currency costAmount = 0;

		TSet<const FGsMonsterKnowledgeSlateNode*> chainNodeSet = slateData->ChainNodeSet(focusNodeData);

		const static int32 SWITCHER_SELECTED_INDEX = 1;
		const bool isCheckedNodeAll = (SWITCHER_SELECTED_INDEX == _btnCheckNodeAllSelected->GetSwitcherIndex());

		for (int32 idx = 0; idx < slateData->NodeMaxCount(); ++idx)
		{
			const FGsMonsterKnowledgeSlateNode* nodeData= slateData->NodeAt(idx);
			if (nullptr == nodeData)
			{
				continue;
			}

			if (EMonsterStoneSlateNodeState::Complete != nodeData->State())
			{
				if (isCheckedNodeAll || chainNodeSet.Contains(nodeData) || nodeData->Id() == focusNodeData->Id())
				{
					costAmount += nodeData->AmountOfCurrency();
				}
			}
			else
			{
				completedCount++;
			}
		}

		bool isNodeCompletedAll = (completedCount == slateData->NodeMaxCount());

		if (isNodeCompletedAll || (false == isCheckedNodeAll && EMonsterStoneSlateNodeState::Complete == focusNodeData->State()))
		{
			const static int32 SWITCHER_ALREADY_LEARNED = 2;
			// 2023/5/3 PKT - 이미 모든 슬롯을 전부 배웠거나 || 전체보기가 아닐때 현재 선택되어진 노드가 배우 노드라면
			_switchApplyButton->SetActiveWidgetIndex(SWITCHER_ALREADY_LEARNED);
		}
		else
		{
			const static int32 SWITCHER_LEARNABLE = 0;
			_switchApplyButton->SetActiveWidgetIndex(SWITCHER_LEARNABLE);

			const Currency pointAmount = FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::MONSTER_BOOK_POINT);
			// 2023/2/9 PKT - 지식의 석판 내 노드 배우기 버튼

			FText buttonName;
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("CurrencyButton"), buttonName);
			_btnCurrencyActiveNode->SetButtonName(buttonName);

			_btnCurrencyActiveNode->SetData(CurrencyType::MONSTER_BOOK_POINT, costAmount);
			_btnCurrencyActiveNode->UpdateAmountTextColorLocalPlayer();
		}
	}	
}

bool UGsUIMonsterCollectionSlateNormalNodeDetailPanel::IsCheckedAllSelected() const
{
	const static int32 SWITCHER_SELECTED_INDEX = 1;
	return (SWITCHER_SELECTED_INDEX == _btnCheckNodeAllSelected->GetSwitcherIndex());
}