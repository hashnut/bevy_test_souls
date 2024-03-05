#include "GsUISealPack.h"

#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/WidgetSwitcher.h"

#include "DataSchema/Seal/GsSchemaSealSlot.h"
#include "GsUISealNodeLine.h"
#include "GsUISealSlot.h"
#include "Management/GsScopeGame.h"
#include "Management/ScopeGame/GsSealManager.h"
#include "Seal/GsSealNode.h"
#include "Seal/GsSealPack.h"
#include "Shared/Client/SharedEnums/SharedItemEnum.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "UI/UIContent/Window/Seal/GsUISealNode.h"
#include "UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "UI/UIControlLib/Control/GsToggleGroup.h"
#include "UI/UIControlLib/Control/GsToggleGroupEntry.h"
#include "../../Helper/GsUIHelper.h"
#include "../UI/UILib/Base/GsUIVFX.h"

void UGsUISealPack::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 봉인라인
	_uiSealNodeLines.Empty();
	_uiSealNodeLines.Append(TArray<TWeakObjectPtr<UGsUISealNodeLine>>({
		MakeWeakObjectPtr(_sealNodeLineA),
		MakeWeakObjectPtr(_sealNodeLineB),
		MakeWeakObjectPtr(_sealNodeLineC),
		MakeWeakObjectPtr(_sealNodeLineD),
		MakeWeakObjectPtr(_sealNodeLineE)
	}));

	for (TWeakObjectPtr<UGsUISealNodeLine>& UiSealNodeLine : _uiSealNodeLines)
	{
		if (!UiSealNodeLine.IsValid())
		{
			continue;
		}

		_onUiEvent.AddUObject(UiSealNodeLine.Get(), &UGsUISealNodeLine::OnUIEvent);
	}

	// 봉인노드
	_sealNodeA->AddWidgetsOnSelected(_sealNodeSelectionA);
	_sealNodeB->AddWidgetsOnSelected(_sealNodeSelectionB);
	_sealNodeC->AddWidgetsOnSelected(_sealNodeSelectionC);
	_sealNodeD->AddWidgetsOnSelected(_sealNodeSelectionD);
	_sealNodeE->AddWidgetsOnSelected(_sealNodeSelectionE);

	_uiNodes.Empty();
	_uiNodes.Append({
		MakeWeakObjectPtr(_sealNodeA),
		MakeWeakObjectPtr(_sealNodeB),
		MakeWeakObjectPtr(_sealNodeC),
		MakeWeakObjectPtr(_sealNodeD),
		MakeWeakObjectPtr(_sealNodeE)
	});

	_uiNodeColorSwitchers.Empty();
	_uiNodeColorSwitchers.Append({
		MakeWeakObjectPtr(_sealNodeColorSwitcherA),
		MakeWeakObjectPtr(_sealNodeColorSwitcherB),
		MakeWeakObjectPtr(_sealNodeColorSwitcherC),
		MakeWeakObjectPtr(_sealNodeColorSwitcherD),
		MakeWeakObjectPtr(_sealNodeColorSwitcherE)
	});

	_uiNodeIcons.Empty();
	_uiNodeIcons.Append({
		MakeWeakObjectPtr(_sealNodeIconA),
		MakeWeakObjectPtr(_sealNodeIconB),
		MakeWeakObjectPtr(_sealNodeIconC),
		MakeWeakObjectPtr(_sealNodeIconD),
		MakeWeakObjectPtr(_sealNodeIconE)
	});

	// 모든 봉인 노드 추가
	for (const TWeakObjectPtr<UGsUISealNode>& UiNode : _uiNodes)
	{
		if (!UiNode.IsValid())
		{
			GSLOG(Error, TEXT("!UiNode.IsValid()"));
			continue;
		}
		_sealNodeToggleGroup.AddToggleWidget(UiNode->GetToggleButton());

		_onUiEvent.AddUObject(UiNode.Get(), &UGsUISealNode::OnUIEvent);
	}
	_sealNodeToggleGroup.OnSelectChanged.BindUObject(this, &UGsUISealPack::OnSelectChangedNode);

	// 봉인슬롯
	_uiNodeSlots.Empty();
	_uiNodeSlots.Append({
		{ MakeWeakObjectPtr(_sealNodeASlot0), MakeWeakObjectPtr(_sealNodeASlot1), MakeWeakObjectPtr(_sealNodeASlot2) },
		{ MakeWeakObjectPtr(_sealNodeBSlot0), MakeWeakObjectPtr(_sealNodeBSlot1), MakeWeakObjectPtr(_sealNodeBSlot2) },
		{ MakeWeakObjectPtr(_sealNodeCSlot0), MakeWeakObjectPtr(_sealNodeCSlot1), MakeWeakObjectPtr(_sealNodeCSlot2) },
		{ MakeWeakObjectPtr(_sealNodeDSlot0), MakeWeakObjectPtr(_sealNodeDSlot1), MakeWeakObjectPtr(_sealNodeDSlot2) },
		{ MakeWeakObjectPtr(_sealNodeESlot0), MakeWeakObjectPtr(_sealNodeESlot1), MakeWeakObjectPtr(_sealNodeESlot2) }
	});

	for (const TArray<TWeakObjectPtr<UGsUISealSlot>>& UiSlots : _uiNodeSlots)
	{
		for (const TWeakObjectPtr<UGsUISealSlot>& UiSlot : UiSlots)
		{
			if (!UiSlot.IsValid())
			{
				GSLOG(Error, TEXT("!UiSlot.IsValid()"));
				continue;
			}

			_onUiEvent.AddUObject(UiSlot.Get(), &UGsUISealSlot::OnUIEvent);
		}
	}
}

void UGsUISealPack::ResetSelections()
{
	// 강제로 초기화 이벤트 발생을 위해 인덱스를 2번씩 변경함.
	_sealNodeToggleGroup.SetSelectedIndexNoEvent(-1);
	_sealNodeToggleGroup.SetSelectedIndex(0);
}

void UGsUISealPack::SetParameters(Parameters InParamerters)
{
	_parameters = MoveTemp(InParamerters);

	const FGsSealPack* SealPack = GetCurrentPack();
	if (nullptr == SealPack)
	{
		GSLOG(Error, TEXT("nullptr == SealPack"));
		return;
	}

	for (int32 i = 0; i < _uiNodes.Num(); i++)
	{
		InvalidateSealNodeByIndex(i);
	}

	// 봉인노드라인
	for (int32 i = 0; i < _uiSealNodeLines.Num(); ++i)
	{
		InvalidateSealNodeLine(i);
	}

	SetSealPackVfxVisible(SealSlotColor::NONE != SealPack->GetSealPackColor());
}

void UGsUISealPack::SetSealPackVfxVisible(const bool InSealPackVfxVisible)
{
	if (nullptr != _sealNodeAllTheSameColorVfx0)
	{
		_sealNodeAllTheSameColorVfx0->SetVisibility(InSealPackVfxVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}

	if (nullptr != _sealNodeAllTheSameColorVfx1)
	{
		_sealNodeAllTheSameColorVfx1->SetVisibility(InSealPackVfxVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
	}
}

int32 UGsUISealPack::GetSelectedNodeIndex() const
{
	return _sealNodeToggleGroup.GetSelectedIndex();
}

int32 SealSlotColor2SealNodeColorSwitcherIndex(const SealSlotColor InSealSlotColor)
{
	switch (InSealSlotColor)
	{
	case SealSlotColor::RARE:
		return 1;
	case SealSlotColor::EPIC:
		return 2;
	case SealSlotColor::UNIQUE:
		return 3;
	case SealSlotColor::NONE:
	default:
		return 0;
	}
}

const FGsSealPack* UGsUISealPack::GetCurrentPack() const
{
	const FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		GSLOG(Error, TEXT("nullptr == SealManager"));
		return nullptr;
	}
	else
	{
		TWeakPtr<const FGsSealPack> SealPack = SealManager->GetSealPack(_parameters._packId);
		return SealPack.IsValid() ? SealPack.Pin().Get() : nullptr;
	}
}

const int32 UGsUISealPack::FindNodeIndexByNodeId(const SealId InNodeId) const
{
	const FGsSealPack* SealPack = GetCurrentPack();
	if (nullptr == SealPack)
	{
		GSLOG(Error, TEXT("nullptr == SealPack"));
		return -1;
	}

	return SealPack->FindNodeIndexByNodeId(InNodeId);
}

const FGsSealNode* UGsUISealPack::GetNodeAt(const int32 InNodeIndex) const
{
	const FGsSealPack* SealPack = GetCurrentPack();
	if (nullptr == SealPack)
	{
		GSLOG(Error, TEXT("nullptr == SealPack"));
		return nullptr;
	}

	return SealPack->GetSealNodeByIndex(InNodeIndex);
}

TWeakPtr<const FGsSealSlot> UGsUISealPack::GetSlotAt(const int32 InNodeIndex, const int32 InSlotIndex) const
{
	TWeakPtr<const FGsSealSlot> SlotWeak;

	const FGsSealNode* SealNode = GetNodeAt(InNodeIndex);
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode"));
	}
	else
	{
		SlotWeak = SealNode->GetSealSlotByIndex(InSlotIndex);
	}

	return SlotWeak;
}

int32 UGsUISealPack::GetSlotCountInNodeAt(const int32 InNodeIndex) const
{
	if (!_uiNodeSlots.IsValidIndex(InNodeIndex))
	{
		GSLOG(Error, TEXT("!_uiNodeSlots.IsValidIndex(%d)"), InNodeIndex);
		return 0;
	}
	else
	{
		return _uiNodeSlots[InNodeIndex].Num();
	}
}

void UGsUISealPack::InvalidateSealNode(const SealId InNodeId)
{
	InvalidateSealNodeByIndex(FindNodeIndexByNodeId(InNodeId));
}

void UGsUISealPack::InvalidateSealNodeByIndex(const int32 InNodeIndex)
{
	const FGsSealManager* SealManager = GSSeal();
	if (nullptr == SealManager)
	{
		GSLOG(Error, TEXT("nullptr == SealManager"));
		return;
	}

	const FGsSealNode* SealNode = GetNodeAt(InNodeIndex);
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode"));
		return;
	}

	const SealSlotColor CurrentNodeSealSlotColor = SealNode->GetSealColor();

	if (!_uiNodes.IsValidIndex(InNodeIndex) || !_uiNodes[InNodeIndex].IsValid())
	{
		GSLOG(Error, TEXT("!_uiNodes.IsValidIndex(%d) || !_uiNodes[%d].IsValid()"), InNodeIndex, InNodeIndex);
	}
	else
	{
		TWeakObjectPtr<UGsUISealNode> SealNodeUI = _uiNodes[InNodeIndex];
		SealNodeUI->SetParameters({
			SealNode->GetSealNodeId(),
			CurrentNodeSealSlotColor,
			SealNode->CanReroll()
		});
	}

	if (!_uiNodeColorSwitchers.IsValidIndex(InNodeIndex) || !_uiNodeColorSwitchers[InNodeIndex].IsValid())
	{
		GSLOG(Error, TEXT("!_uiNodeColorSwitchers.IsValidIndex(%d) || !_uiNodeColorSwitchers[%d].IsValid()"), InNodeIndex, InNodeIndex);
	}
	else
	{
		TWeakObjectPtr<UWidgetSwitcher> WidgetSwitcher = _uiNodeColorSwitchers[InNodeIndex];
		if (SealNode->CanReroll())
		{
			WidgetSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			const int32 SealNodeColorSwitcherIndex = SealSlotColor2SealNodeColorSwitcherIndex(CurrentNodeSealSlotColor);
			WidgetSwitcher->SetActiveWidgetIndex(SealNodeColorSwitcherIndex);
		}
		else
		{
			WidgetSwitcher->SetVisibility(ESlateVisibility::Collapsed);
		}
	}

	//TODO: _uiNodeColorSwitchers 위젯 내부로 합치기
	if (!_uiNodeIcons.IsValidIndex(InNodeIndex) || !_uiNodeIcons[InNodeIndex].IsValid())
	{
		GSLOG(Error, TEXT("!_uiNodeIcons.IsValidIndex(%d) || !_uiNodeIcons[%d].IsValid()"), InNodeIndex, InNodeIndex);
	}
	else
	{
		TWeakObjectPtr<UWidgetSwitcher> WidgetSwitcher = _uiNodeIcons[InNodeIndex];
		// TODO: 급하게 들어간 코드라 하드코딩으로 인덱스를 +1 및 0 설정함.(위 반복문 참조)
		if (SealNode->CanReroll())
		{
			const int32 SealNodeColorSwitcherIndex = SealSlotColor2SealNodeColorSwitcherIndex(CurrentNodeSealSlotColor) + 1;
			WidgetSwitcher->SetActiveWidgetIndex(SealNodeColorSwitcherIndex);
		}
		else
		{
			WidgetSwitcher->SetActiveWidgetIndex(0);
		}
	}

	// 봉인 슬롯 새로고침
	if (!_uiNodeSlots.IsValidIndex(InNodeIndex))
	{
		GSLOG(Error, TEXT("!_uiNodeSlots.IsValidIndex(%d)"), InNodeIndex);
	}
	else
	{
		const int32 SealSlotCount = GetSlotCountInNodeAt(InNodeIndex);
		for (int32 i = 0; i < SealSlotCount; i++)
		{
			InvalidateSealSlot(SealNode->GetSealNodeId(), i);
		}
	}
}

void UGsUISealPack::InvalidateSealNodeLine(const int32 InNodeIndex)
{
	if (!_uiSealNodeLines.IsValidIndex(InNodeIndex))
	{
		GSLOG(Error, TEXT("!SealNodeLineUI.IsValid(), InNodeIndex:%d"), InNodeIndex);
		return;
	}

	const TWeakObjectPtr<UGsUISealNodeLine>& SealNodeLineUI = _uiSealNodeLines[InNodeIndex];
	if (!SealNodeLineUI.IsValid())
	{
		GSLOG(Error, TEXT("!SealNodeLineUI.IsValid(), InNodeIndex:%d"), InNodeIndex);
		return;
	}

	const FGsSealPack* SealPack = GetCurrentPack();
	if (nullptr == SealPack)
	{
		GSLOG(Error, TEXT("nullptr == SealPack, InNodeIndex:%d"), InNodeIndex);
		return;
	}

	const FGsSealNode* SealNode = SealPack->GetSealNodeByIndex(InNodeIndex);
	if (nullptr == SealNode)
	{
		GSLOG(Error, TEXT("nullptr == SealNode, InNodeIndex:%d"), InNodeIndex);
		return;
	}

	SealNodeLineUI->SetParameters({
		SealNode->GetSealNodeId(),
		SealNode->CanLinkLine(),
		SealNode->GetSealColor()
	});
}

void UGsUISealPack::InvalidateSealSlot(const SealId InNodeId, const int32 InSlotIndex)
{
	const int32 NodeIndex = FindNodeIndexByNodeId(InNodeId);

	TWeakPtr<const FGsSealSlot> SealSlotPtr = GetSlotAt(NodeIndex, InSlotIndex);
	if (!SealSlotPtr.IsValid())
	{
		GSLOG(Error, TEXT("SealSlotPtr.IsValid(), NodeIndex(%d), InSlotIndex(%d)"), NodeIndex, InSlotIndex);
		return;
	}
	TSharedPtr<const FGsSealSlot> SealSlot = SealSlotPtr.Pin();

	if (!_uiNodeSlots.IsValidIndex(NodeIndex))
	{
		GSLOG(Error, TEXT("!_uiNodeSlots.IsValidIndex(%d)"), NodeIndex);
	}
	else
	{
		const TArray<TWeakObjectPtr<UGsUISealSlot>>& SlotUIs = _uiNodeSlots[NodeIndex];
		if (!SlotUIs.IsValidIndex(InSlotIndex) || !SlotUIs[InSlotIndex].IsValid())
		{
			GSLOG(Error, TEXT("!SlotUIs.IsValidIndex(%d) || !SlotUIs[%d].IsValid()"), InSlotIndex, InSlotIndex);
		}
		else
		{
			SlotUIs[InSlotIndex]->SetParameters({
				InNodeId,
				InSlotIndex,
				SealSlot->IsOpened(),
				SealSlot->IsRerollUnlock(EGsSealRerollUnlockTarget::Stat),
				SealSlot->IsRerollUnlock(EGsSealRerollUnlockTarget::Color),
				SealSlot->GetSealColor(),
			});
		}
	}
}

void UGsUISealPack::OnUIEvent(const FGsUIWindowSealEventParameters& InEventParameters)
{
	// 상황에 따라, 특정 사운드를 출력할 수도 있음.
	FString SoundName;

	switch (InEventParameters._event)
	{
	case EGsUISealWindowEvent::Reset:
		break;

	case EGsUISealWindowEvent::SealNode:
		break;

	case EGsUISealWindowEvent::SealNodeValue:
		InvalidateSealNode(InEventParameters._nodeId);
		break;

	case EGsUISealWindowEvent::SealSlot0:
		SoundName = TEXT("UI_Seal_ReRoll");
		break;

	case EGsUISealWindowEvent::SealSlot1:
	{
		// 스킵인 경우, 위에서 이미 하나 플레이하므로 생략함.
		if (!InEventParameters._isSkip)
		{
			SoundName = TEXT("UI_Seal_ReRoll");
		}
	}
	break;

	case EGsUISealWindowEvent::SealSlot2:
	{
		if (!InEventParameters._isSkip)
		{
			SoundName = TEXT("UI_Seal_ReRoll");
		}

		// 3색상이 모두 Lock 인 상태인 경우에는 반복적인 이펙트 출력이 무의미하므로, 단 하나라도 언락인지 여부를 계산함.
		const FGsSealNode* SealNode = GetNodeAt(FindNodeIndexByNodeId(InEventParameters._nodeId));
		if (nullptr != SealNode && SealNode->CanLinkLine() && SealNode->IsAnyRerollUnlocked(EGsSealRerollUnlockTarget::Color))
		{
			// 3개 모두 동일 색상일 경우, 효과 출력
			{
				FString SoundName2;
				const SealSlotColor CurrentSealSlotColor = SealNode->GetSealColor();
				switch (CurrentSealSlotColor)
				{
				case SealSlotColor::RARE: SoundName2 = TEXT("UI_Seal_Complete_Rare"); break;
				case SealSlotColor::EPIC: SoundName2 = TEXT("UI_Seal_Complete_Epic"); break;
				case SealSlotColor::UNIQUE: SoundName2 = TEXT("UI_Seal_Complete_Unique"); break;
				}
				if (!SoundName2.IsEmpty())
				{
					FGsUIHelper::Play2DSound(SoundName2);
				}
			}
		}
	}
	break;

	case EGsUISealWindowEvent::SealSlotValue0:
		InvalidateSealSlot(InEventParameters._nodeId, 0);
		break;

	case EGsUISealWindowEvent::SealSlotValue1:
		InvalidateSealSlot(InEventParameters._nodeId, 1);
		break;

	case EGsUISealWindowEvent::SealSlotValue2:
		InvalidateSealSlot(InEventParameters._nodeId, 2);
		break;

	case EGsUISealWindowEvent::SealLines:
		break;

	case EGsUISealWindowEvent::SealLinesValue:
	{
		// 모든 노드의 색상이 일치할 때 발생
		if (nullptr != _sealAllNodeSameColorOnceVfx)
		{
			// 3색상이 모두 Lock 인 상태인 경우에는 반복적인 이펙트 출력이 무의미하므로, 단 하나라도 언락인지 여부를 계산함.
			bool IsAnyRerollColorUnlocked = false;

			const int32 SealNodeIndex = FindNodeIndexByNodeId(InEventParameters._nodeId);
			const FGsSealNode* SealNode = GetNodeAt(SealNodeIndex);
			const int32 SealSlotCount = GetSlotCountInNodeAt(SealNodeIndex);
			for (int32 i = SealSlotCount - 1; 0 <= i; --i)
			{
				TWeakPtr<const FGsSealSlot> SealSlotPtr = GetSlotAt(SealNodeIndex, i);
				if (!SealSlotPtr.IsValid())
				{
					GSLOG(Error, TEXT("!SealSlotPtr.IsValid(), SealNodeIndex: %d, i: %d"), SealNodeIndex, i);
					continue;
				}
				
				if (SealSlotPtr.Pin()->IsRerollUnlock(EGsSealRerollUnlockTarget::Color))
				{
					IsAnyRerollColorUnlocked = true;
					break;
				}
			}

			const FGsSealPack* SealPack = GetCurrentPack();
			if ((nullptr != SealPack) && (SealSlotColor::NONE != SealPack->GetSealPackColor()) && IsAnyRerollColorUnlocked)
			{
				_sealAllNodeSameColorOnceVfx->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				_sealAllNodeSameColorOnceVfx->HideVFX();
				_sealAllNodeSameColorOnceVfx->ShowVFX();
			}
		}
	}
	break;

	}

	_onUiEvent.Broadcast(InEventParameters);

	if (!SoundName.IsEmpty())
	{
		FGsUIHelper::Play2DSound(SoundName);
	}
}

void UGsUISealPack::OnSelectChangedNode(int32 InIndex)
{
	//if (0 <= InIndex)
	//{
	//	_sealNodeLastSelectionIndex = InIndex;
	//}

	//if (SEAL_SUB_TAB_SEALPACK == _subTabToggleGroup.GetSelectedIndex())
	//{
	//	_subTabToggleGroup.SetSelectedIndex(SEAL_SUB_TAB_SEALNODE);
	//}

	_onSelectChangedNode.ExecuteIfBound();
}
