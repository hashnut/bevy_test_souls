#include "GsUIMonsterCollectionSlateNodeSet.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlate.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeSlateNode.h"
#include "UMG/Public/Components/PanelWidget.h"
#include "T1Project.h"


void UGsUIMonsterCollectionSlateNodeSet::BeginDestroy()
{
	_nodeSet.Empty();

	Super::BeginDestroy();
}

void UGsUIMonsterCollectionSlateNodeSet::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	for (auto& item : _panelNodeSet->GetSlots())
	{
		if (item && item->Content)
		{
			TWeakObjectPtr<UGsUIMonsterCollectionSlateNode> node = Cast<UGsUIMonsterCollectionSlateNode>(item->Content);
			if (node.IsValid())
			{
				node->SetIsEnabled(false);

				_nodeSet.Emplace(TWeakObjectPtr<UGsUIMonsterCollectionSlateNode>(node));
			}
		}
	}

	for (auto& item : _panelLineSet->GetSlots())
	{
		if (item && item->Content)
		{
			TWeakObjectPtr<UGsUIMonsterCollectionSlateNodeLine> line = Cast<UGsUIMonsterCollectionSlateNodeLine>(item->Content);
			if (line.IsValid())
			{
				line->SetIsEnabled(false);

				_lineSet.Emplace(TWeakObjectPtr<UGsUIMonsterCollectionSlateNodeLine>(line));
			}
		}
	}
}

void UGsUIMonsterCollectionSlateNodeSet::NativeConstruct()
{
	Super::NativeConstruct();

	for (auto& item : _nodeSet)
	{
		if (item.IsValid())
		{
			item.Get()->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	for (auto& item : _lineSet)
	{
		if (item.IsValid())
		{
			item.Get()->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UGsUIMonsterCollectionSlateNodeSet::OnClickedNodeDelegate(const FGsMonsterKnowledgeSlateNode* InData)
{
	if (nullptr == InData || nullptr == _data)
	{
		return;
	}

	_selectedNodeData = InData;

	OnChangedSelected.ExecuteIfBound();
}

void UGsUIMonsterCollectionSlateNodeSet::SetData(const class FGsMonsterKnowledgeSlate* InSlate, MonsterKnowledgeSlateSlotId InSelectedNodeId)
{
	if (nullptr == InSlate)
	{
		return;
	}

	_data = InSlate;
	_prevState = _data->State();
	_selectedNodeData = _data->FindNodeById(InSelectedNodeId);
	if (nullptr == _selectedNodeData)
	{
		_selectedNodeData = _data->BeginNode();
	}

	// 2023/3/7 PKT - Active Node
	for (int32 idx = 0; idx < _data->NodeMaxCount(); ++idx)
	{
		const FGsMonsterKnowledgeSlateNode* node = _data->NodeAt(idx);
		for (auto& item : _nodeSet)
		{
			if (item.IsValid())
			{
				if (node->ResourceId() == item.Get()->ResourceId())
				{
					item.Get()->SetData(_data);
					item.Get()->SetIsEnabled(true);
					item.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					if (false == item.Get()->OnClickedDelegate.IsBoundToObject(this))
					{
						item.Get()->OnClickedDelegate.BindUObject(this, &UGsUIMonsterCollectionSlateNodeSet::OnClickedNodeDelegate);
					}

					break;
				}
			}
		}
	}

	// 2023/3/7 PKT - Active Line
	for (int32 idx = 0; idx < _data->NodeMaxCount(); ++idx)
	{
		const FGsMonsterKnowledgeSlateNode* node = _data->NodeAt(idx);
		if (nullptr == node->PrevNodeData())
		{
			continue;
		}

		for (auto& item : _lineSet)
		{
			if (item.IsValid())
			{
				if (item.Get()->PrevBindNodeId() == node->PrevNodeId() && item.Get()->NextBindNodeId() == node->Id())
				{
					item.Get()->SetData(_data);
					item.Get()->SetIsEnabled(true);
					item.Get()->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

					break;
				}
			}
		}
	}

	Invalidate();
}

void UGsUIMonsterCollectionSlateNodeSet::Invalidate(bool InIsForceSelected /* = false */)
{
	if (_prevState != _data->State())
	{
		_prevState = _data->State();

		OnChangedState.ExecuteIfBound();
	}

	for (auto& item : _nodeSet)
	{
		if (item.IsValid() && ESlateVisibility::SelfHitTestInvisible == item.Get()->GetVisibility())
		{
			item.Get()->Invalidate(_selectedNodeData, InIsForceSelected);
		}
	}

	for (auto& item : _lineSet)
	{
		if (item.IsValid() && ESlateVisibility::SelfHitTestInvisible == item.Get()->GetVisibility())
		{
			item.Get()->Invalidate(_selectedNodeData, InIsForceSelected);
		}
	}
}

const FGsMonsterKnowledgeSlate* UGsUIMonsterCollectionSlateNodeSet::GetData() const
{
	return _data;
}

const class FGsMonsterKnowledgeSlateNode* UGsUIMonsterCollectionSlateNodeSet::GetSelectedNodeData() const
{
	return _selectedNodeData;
}