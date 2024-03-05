#include "GsUIMonsterCollectionSlateInfo.h"
#include "GsUIMonsterCollectionSlateNode.h"
#include "GsUIMonsterCollectionSlateNodeSet.h"
#include "GsUIMonsterCollectionSlateStatEntry.h"
#include "GsUIMonsterCollectionSpecialNode.h"
#include "GsUIMonsterCollectionSlateNodeDetailPanelBase.h"
#include "../Item/GsItem.h"
#include "../Item/GsItemManager.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGlobal/GsPathManager.h"
#include "../GameObject/Stat/GsStatHelper.h"
#include "../Currency/GsCurrencyHelper.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlate.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlateNode.h"
#include "../MonsterKnowledgeCollection/GsSlateSpecialNode.h"
#include "../UI/UILib/Base/GsUIVFX.h"
#include "../UI/UIContent/Helper/GsUIHelper.h"
#include "../UI/UIContent/Common/GsUICurrencyButton.h"
#include "../UI/UIContent/Tray/NotiMessage/GsUISectionMessageDefine.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "../UI/UIContent/Popup/GsUIPopupCommonItemConsume.h"
#include "../UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"
#include "../Net/GsNetSendServiceItem.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"



void UGsUIMonsterCollectionSlateInfo::BeginDestroy()
{
	if (nullptr != _specialNodeBtnPanelSlotHelper)
	{
		_specialNodeBtnPanelSlotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIMonsterCollectionSlateInfo::OnRefreshSpecialNodeBtnEntry);
		_specialNodeBtnPanelSlotHelper = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIMonsterCollectionSlateInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_currentNodeSet.Reset();

	_panelNodeSetList->ClearChildren();

	if (nullptr == _specialNodeBtnPanelSlotHelper)
	{
		_specialNodeBtnPanelSlotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
		_specialNodeBtnPanelSlotHelper->Initialize(_specialNodeBtnEntryWidgetClass, _specialNodeBtnRootPanel);
		_specialNodeBtnPanelSlotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIMonsterCollectionSlateInfo::OnRefreshSpecialNodeBtnEntry);
	}
}

void UGsUIMonsterCollectionSlateInfo::NativeConstruct()
{
	Super::NativeConstruct();

	_currentNodeSet.Reset();
	_vfxOpenEffect->HideVFX();

	const static int32 NORMAL_DETAIL_INDEX = 0;
	const static int32 SPECIAL_DETAIL_INDEX = 1;

	_normalDetailPanel = Cast<UGsUIMonsterCollectionSlateNormalNodeDetailPanel>(_switcherDetailPanel->GetWidgetAtIndex(NORMAL_DETAIL_INDEX));
	_specialDetailPanel = Cast<UGsUIMonsterCollectionSlateSpecialNodeDetailPanel>(_switcherDetailPanel->GetWidgetAtIndex(SPECIAL_DETAIL_INDEX));
}

UWidget* UGsUIMonsterCollectionSlateInfo::FindOrAddNodeWidget(const FSoftObjectPath& InPath)
{
	auto GetBPFileName = [](const FString& InName) -> FString
	{
		FString newName = InName;

		int32 index = 0;
		if (newName.FindLastChar(TEXT('.'), index))
		{
			return newName.RightChop(index + 1);
		}

		return newName;
	};

	const FString nodeSetPath = InPath.ToString() + TEXT("_C");

	FString nodeSetFileName = GetBPFileName(nodeSetPath);

	UWidget* outWidget = nullptr;
	const int32 maxCount = _panelNodeSetList->GetChildrenCount();
	for (int32 idx = 0; idx < maxCount; ++idx)
	{
		UWidget* childWidget = _panelNodeSetList->GetChildAt(idx);
		if (childWidget)
		{
			childWidget->SetVisibility(ESlateVisibility::Collapsed);

			if (nullptr == outWidget)
			{
				const FString childFileName = GetBPFileName(childWidget->GetPathName());

				if (0 == childFileName.Compare(nodeSetFileName, ESearchCase::IgnoreCase))
				{
					outWidget = childWidget;
				}
			}
		}
	}

	if (nullptr == outWidget)
	{
		UClass* newNodeSetCDO = GPath()->GetClassByObjectPath(InPath);
		outWidget = CreateWidget(GetWorld(), TSubclassOf<UUserWidget>(newNodeSetCDO), *nodeSetPath);

		if (outWidget)
		{
			Cast<UUserWidget>(outWidget)->AddToViewport();

			_panelNodeSetList->AddChild(Cast<UWidget>(outWidget));
		}
		else
		{
			GSLOG(Error, TEXT("nullptr == outWidget"));
			return outWidget;
		}
	}

	return outWidget;
}

void UGsUIMonsterCollectionSlateInfo::OnVFXPlayOpenSlate()
{
	if (_currentNodeSet.IsValid())
	{
		const FGsMonsterKnowledgeSlate* slateData = _currentNodeSet.Get()->GetData();
		if (slateData && EMonsterKnowledgeSlateState::UnLock == slateData->State())
		{
			_vfxOpenEffect->ShowVFX();
		}
	}
}

void UGsUIMonsterCollectionSlateInfo::OnSelectedNode()
{
	if (false == _currentNodeSet.IsValid())
	{
		return;
	}

	const static int32 SWITCHER_NORMAL_INDEX = 0;
	const static int32 SWITCHER_SPECIAL_INDEX = 1;

	if (SWITCHER_NORMAL_INDEX == _switcherDetailPanel->GetActiveWidgetIndex())
	{
		if (_normalDetailPanel.IsValid())
		{
			if (_normalDetailPanel.Get()->GetDetailDataId() == _currentNodeSet.Get()->GetSelectedNodeData()->Id())
			{	// 2023/11/9 PKT - 같은 상태에서 같은 노드를 클릭했다면 무시..
				return;
			}
		}
	}
	else
	{
		if (_specialDetailPanel.IsValid())
		{
			_specialDetailPanel.Get()->SetDetailData(nullptr);
		}

		for (int32 idx = 0; idx < _specialNodeBtnPanelSlotHelper->GetItemCount(); ++idx)
		{
			TWeakObjectPtr<UGsUIMonsterCollectionSpecialNode> item = Cast<UGsUIMonsterCollectionSpecialNode>(_specialNodeBtnPanelSlotHelper->GetEntry(idx));
			if (item.IsValid() && item.Get()->GetData())
			{		
				item.Get()->SetSelectedEffect(false);
			}
		}

		_switcherDetailPanel->SetActiveWidgetIndex(SWITCHER_NORMAL_INDEX);
	}
	
	if (_normalDetailPanel.IsValid())
	{
		_normalDetailPanel.Get()->SetDetailData(_currentNodeSet.Get()->GetSelectedNodeData());
	}

	Invalidate();
}

void UGsUIMonsterCollectionSlateInfo::OnRefreshSpecialNodeBtnEntry(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUIMonsterCollectionSpecialNode> item = Cast<UGsUIMonsterCollectionSpecialNode>(InEntry);
	if (false == item.IsValid())
	{
		return;
	}

	if (false == _currentNodeSet.IsValid())
	{
		return;
	}

	if (const FGsMonsterKnowledgeSlate* slateData = _currentNodeSet.Get()->GetData())
	{
		if (const FGsSlateSpecialNode* findData = slateData->FindSpecialNodeByIndex(InIndex))
		{
			item->SetData(findData, [this](EMonsterCollectionSlateCallbackReason InReason, int32 InParam) {this->OnEventStateChange(InReason, InParam); });
		}
	}
}

void UGsUIMonsterCollectionSlateInfo::OnEventStateChange(EMonsterCollectionSlateCallbackReason InReason, uint64 InParam)
{
	switch (InReason)
	{
	case EMonsterCollectionSlateCallbackReason::ChangedSelectedAll:
	{
		// 2023/11/8 PKT - Invalidate에서 다 함.여기서 할건 없음.
		Invalidate();
	}	break;
	case EMonsterCollectionSlateCallbackReason::SelectedSpecialNode:
	{
		for (int32 idx = 0; idx < _specialNodeBtnPanelSlotHelper->GetItemCount(); ++idx)
		{
			TWeakObjectPtr<UGsUIMonsterCollectionSpecialNode> item = Cast<UGsUIMonsterCollectionSpecialNode>(_specialNodeBtnPanelSlotHelper->GetEntry(idx));
			if (item.IsValid() && item.Get()->GetData())
			{
				bool isSelected = StaticCast<SlateSpecialNodeId>(InParam) == item.Get()->GetData()->GetId();
				item.Get()->SetSelectedEffect(isSelected);

				const static int32 SWITCHER_SPEICAL_NODE_INDEX = 1;
				_switcherDetailPanel->SetActiveWidgetIndex(SWITCHER_SPEICAL_NODE_INDEX);

				if (_specialDetailPanel.IsValid() && isSelected)
				{
					_specialDetailPanel.Get()->SetDetailData(item.Get()->GetData());
				}				
			}
		}
		Invalidate();
	}	break;
	case EMonsterCollectionSlateCallbackReason::ReqOpenSlate:
	{
		if (false == _currentNodeSet.IsValid())
		{
			return;
		}

		ItemId findItemId = INVALID_ITEM_ID;
		ItemDBId findItemDBId = INVALID_ITEM_DB_ID;
		const FGsMonsterKnowledgeSlate* slateData = _currentNodeSet.Get()->GetData();
		TArray<TWeakPtr<FGsItem>> haveCandinateCostItems = GItem()->GetAllOwnedItemsTheSameGroup(slateData->DisplayCostItemId());
		for (auto item : haveCandinateCostItems)
		{
			if (false == item.IsValid() || nullptr == item.Pin().Get()->GetTableData())
			{
				continue;
			}

			if (false == item.Pin().Get()->GetTableData()->tradable)
			{	// 2023/12/11 PKT - 최 우선적으로 귀속 아이템을 넣어 주고
				findItemId = item.Pin().Get()->GetTID();
				findItemDBId = item.Pin().Get()->GetDBID();
			}
			else if (findItemId == INVALID_ITEM_ID)
			{	// 2023/12/11 PKT - 귀속 아이템이 없을때에는 일반(비귀속) 아이템을 넣어 준다.
				findItemId = item.Pin().Get()->GetTID();
				findItemDBId = item.Pin().Get()->GetDBID();
			}
		}

		if (INVALID_ITEM_ID == findItemId)
		{
			return;
		}

		TWeakObjectPtr<UGsUIPopupCommonItemConsume> widget = Cast<UGsUIPopupCommonItemConsume>(GUI()->OpenAndGetWidget(TEXT("PopupCommonItemConsume")).Get());
		if (widget.IsValid())
		{
			const static int32 USE_ITEM_AMOUNT = 1;			
			ItemCategorySub subType = haveCandinateCostItems[0].Pin()->GetSubCategory();

			FText textDesc;
			FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("slate_ActivatePopupDesc"), textDesc);
			FText textMsg = FText::Format(textDesc, slateData->Title(), USE_ITEM_AMOUNT);

			widget->SetDataByGroupId(slateData->Title(), textMsg, findItemId, USE_ITEM_AMOUNT, [findItemDBId, subType](bool InIsOk)
				{
					if (InIsOk && findItemDBId != INVALID_ITEM_DB_ID)
					{
						FGsNetSendServiceItem::SendUseItem(findItemDBId, subType);
					}
				}
			);
		}
	}	break;
	case EMonsterCollectionSlateCallbackReason::ReqActiveNode:
	{
		if (false == _currentNodeSet.IsValid())
		{
			return;
		}

		if (_normalDetailPanel.IsValid())
		{
			const Currency amount = InParam;
			if (amount > FGsCurrencyHelper::GetCurrencyAmount(CurrencyType::MONSTER_BOOK_POINT))
			{	// 2023/4/27 PKT - 재화가 부족하면 부족한 티커 알리고 나감.
				FText message;
				FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("UI_Ticker_NotEnoughPoint"), message);
				FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
				return;
			}

			const FGsMonsterKnowledgeSlate* slateData = _currentNodeSet.Get()->GetData();
			const FGsMonsterKnowledgeSlateNode* nodeData = _currentNodeSet.Get()->GetSelectedNodeData();

			if (_normalDetailPanel.Get()->IsCheckedAllSelected())
			{
				FGsNetSendServiceWorld::Send_ActivateMonsterKnowledgeSlateAll(slateData->Id());
			}
			else
			{
				if (EMonsterStoneSlateNodeState::Complete == nodeData->State())
				{
					FText message;
					FText::FindText(TEXT("MonsterKnowledgeText"), TEXT("UI_Text_Slate_Finished"), message);
					FGsUIHelper::TraySectionMessageTicker(EGsNotiSection::EXTSYSTEM, message);
				}
				else
				{
					FGsNetSendServiceWorld::Send_ActivateMonsterKnowledgeSlate(slateData->Id(), nodeData->Id());
				}
			}
		}
	}	break;
	case EMonsterCollectionSlateCallbackReason::ReqOpenSpecialNode:
	{
		if (false == _currentNodeSet.IsValid() || nullptr == _currentNodeSet.Get()->GetData())
		{
			return;
		}
		
		// 2023/11/9 PKT - 여러 검증 해야 하는 코드가 있을 수 있겠으나 일단 다이렉트로 보내자. 시간 없음.
		FGsNetSendServiceWorld::Send_ActivateSlateSpecialNode(_currentNodeSet.Get()->GetData()->Id(), StaticCast<SlateSpecialNodeId>(InParam));

	}	break;
	}
}

void UGsUIMonsterCollectionSlateInfo::SetData(const FGsMonsterKnowledgeSlate* InData, const UGsUIMonsterCollectionSlateInfo::FGsCaptureSmallOption& InOption)
{
	// 2023/11/8 PKT - VFX Off
	_vfxOpenEffect->HideVFX();
	
	// 2023/11/8 PKT - Set Node
	_currentNodeSet.Reset();
	_currentNodeSet = Cast<UGsUIMonsterCollectionSlateNodeSet>(FindOrAddNodeWidget(InData->NodeSetPath()));

	if (_currentNodeSet.IsValid())
	{
		_currentNodeSet.Get()->SetData(InData, InOption._isSelectedNodeId);
		_currentNodeSet.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (false == _currentNodeSet.Get()->OnChangedState.IsBoundToObject(this))
		{
			_currentNodeSet.Get()->OnChangedState.BindUObject(this, &UGsUIMonsterCollectionSlateInfo::OnVFXPlayOpenSlate);
		}

		if (false == _currentNodeSet.Get()->OnChangedSelected.IsBoundToObject(this))
		{
			_currentNodeSet.Get()->OnChangedSelected.BindUObject(this, &UGsUIMonsterCollectionSlateInfo::OnSelectedNode);
		}
	}

	int32 maxCount = InData->GetSlateSpecialNodeCount();
	_specialNodeBtnPanelSlotHelper->RefreshAll(maxCount);

	// 2023/11/8 PKT - 특수 노드를 선택한 상태가 아니다.
	const static int32 NORMAL_DETAIL_INDEX = 0;
	const static int32 SPECIAL_DETAIL_INDEX = 1;
	
	const int32 index = (INVALID_SLATE_SPECIAL_NODE_ID >= InOption._isSelectedSpecialNodeId) ? NORMAL_DETAIL_INDEX : SPECIAL_DETAIL_INDEX;
	_switcherDetailPanel->SetActiveWidgetIndex(index);

	Invalidate();
}

void UGsUIMonsterCollectionSlateInfo::Invalidate()
{
	if (false == _currentNodeSet.IsValid())
	{
		return;
	}

	const static int32 NORMAL_DETAIL_INDEX = 0;
	const static int32 SPECIAL_DETAIL_INDEX = 1;

	/**
	 * Invalidate Node Tree
	 */
	if (_normalDetailPanel.IsValid())
	{
		_currentNodeSet.Get()->Invalidate(_normalDetailPanel.Get()->IsCheckedAllSelected());
	}

	/**
	 * Invalidate Special Node Btn
	 */
	for (int32 idx = 0; idx < _specialNodeBtnPanelSlotHelper->GetItemCount(); ++idx)
	{	// 2023/11/8 PKT - 특수 노드 Btn을 갱신
		TWeakObjectPtr<UGsUIMonsterCollectionSpecialNode> item = Cast<UGsUIMonsterCollectionSpecialNode>(_specialNodeBtnPanelSlotHelper->GetEntry(idx));
		if (item.IsValid())
		{
			item.Get()->Invalidate();
		}
	}


	/**
	 * Invalidate Detail Panel
	 */
	TWeakObjectPtr<UGsUIMonsterCollectionSlateNodeDetailPanelBase> activeBaseWidget
		= Cast<UGsUIMonsterCollectionSlateNodeDetailPanelBase>(_switcherDetailPanel->GetActiveWidget());
	if (activeBaseWidget.IsValid())
	{
		if (activeBaseWidget.Get()->GetId() != _currentNodeSet.Get()->GetData()->Id())
		{	// 2023/11/8 PKT - Set New Data
			activeBaseWidget->SetData(_currentNodeSet.Get()->GetData(), [this](EMonsterCollectionSlateCallbackReason InReason, int32 InParam) {this->OnEventStateChange(InReason, InParam); });
		}

		if (NORMAL_DETAIL_INDEX == _switcherDetailPanel->GetActiveWidgetIndex())
		{
			if (_normalDetailPanel.IsValid())
			{
				if (_normalDetailPanel.Get()->GetDetailDataId() != _currentNodeSet.Get()->GetSelectedNodeData()->Id())
				{
					_normalDetailPanel.Get()->SetDetailData(_currentNodeSet.Get()->GetSelectedNodeData());
				}
			}
		}
		else
		{
			if (_specialDetailPanel.IsValid())
			{
				// 2023/11/8 PKT - 선택되어진 Special Node Data를 세팅함.
			}
		}

		activeBaseWidget.Get()->Invalidate();
	}
}

void UGsUIMonsterCollectionSlateInfo::InvalidateCurrency()
{
	TWeakObjectPtr<UGsUIMonsterCollectionSlateNodeDetailPanelBase> activeBaseWidget	= Cast<UGsUIMonsterCollectionSlateNodeDetailPanelBase>(_switcherDetailPanel->GetActiveWidget());
	if (activeBaseWidget.IsValid())
	{
		activeBaseWidget.Get()->InvalidateCurrency();
	}
}

const FGsMonsterKnowledgeSlate* UGsUIMonsterCollectionSlateInfo::GetData() const
{
	return _currentNodeSet.IsValid() ? _currentNodeSet.Get()->GetData() : nullptr;
}

bool UGsUIMonsterCollectionSlateInfo::IsCheckedSelectedNodeAll() const
{
	if (_normalDetailPanel.IsValid())
	{
		return _normalDetailPanel.Get()->IsCheckedAllSelected();
	}
	return false;
}

MonsterKnowledgeSlateSlotId UGsUIMonsterCollectionSlateInfo::SelectedNodeId() const
{
	return _currentNodeSet.IsValid() ? _currentNodeSet.Get()->GetSelectedNodeData()->Id() : INVALID_MONSTER_KNOWLEDGE_SLATE_SLOT_ID;
}

SlateSpecialNodeId UGsUIMonsterCollectionSlateInfo::SelectedSpecialNodeId() const
{
	if (_specialDetailPanel.IsValid() && _specialDetailPanel.Get()->GetDetailData())
	{
		_specialDetailPanel.Get()->GetDetailData()->GetId();
	}

	return INVALID_SLATE_SPECIAL_NODE_ID;
}