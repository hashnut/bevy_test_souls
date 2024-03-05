#include "GsUISubNPCList.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/HUD/Minimap/GsUISubNPCListItem.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "../Quest/GsQuestData.h"
#include "UMG/Public/Blueprint/UserWidget.h"
#include "../Management/ScopeGlobal/GsQuestManager.h"

void UGsUISubNPCList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUISubNPCList::OnRefreshEntry);
}

void UGsUISubNPCList::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUISubNPCList::NativeDestruct()
{
	_subQuestNpcMoveDataList.Empty();

	Super::NativeDestruct();
}

void UGsUISubNPCList::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (_subQuestNpcMoveDataList.Num() <= InIndex)
	{
		return;
	}

	TSharedPtr<FGsSubQuestNpcMoveData> npcMoveData = _subQuestNpcMoveDataList[InIndex];
	if (npcMoveData.IsValid())
	{
		UGsUISubNPCListItem* item = Cast<UGsUISubNPCListItem>(InEntry);
		if (item)
		{
			item->SetData(npcMoveData);
		}
	}
}

void UGsUISubNPCList::BeginDestroy()
{
	if (nullptr != _scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUISubNPCList::OnRefreshEntry);
		_scrollBoxHelper = nullptr;
	}

	Super::BeginDestroy();
}

// 데이터 세팅
void UGsUISubNPCList::SetNpcListData()
{
	_subQuestNpcMoveDataList.Empty();
	GSQuest()->GetSubNpcList(_subQuestNpcMoveDataList);
	
	if (0 < _subQuestNpcMoveDataList.Num())
	{
		_switcherListType->SetActiveWidgetIndex(0);
		_scrollBoxHelper->RefreshAll(_subQuestNpcMoveDataList.Num());	
	}
	else
	{
		_switcherListType->SetActiveWidgetIndex(1);
	}
}
	
