// Fill out your copyright notice in the Description page of Project Settings.


#include "GsUIWindowGameStatistics.h"
#include "Components/PanelWidget.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIContent/Window/GameStatistics/GsUIGameStatisticsListEntry.h"
#include "UI/UIContent/Window/GameStatistics/GsUILineGraph.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsGameStatisticsManager.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "UI/UIContent/Helper/GsTimeStringHelper.h"


void UGsUIWindowGameStatistics::BeginDestroy()
{
	if (_slotHelper)
	{
		_slotHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowGameStatistics::OnRefreshEntry);
	}
	_slotHelper = nullptr;

	Super::BeginDestroy();
}

void UGsUIWindowGameStatistics::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::Statistics;

	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowGameStatistics::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowGameStatistics::OnClickCloseAllStack);

	_slotHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelper->Initialize(_entryWidgetClass, _panelRootList);
	_slotHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowGameStatistics::OnRefreshEntry);

	_toggleGroupTab.AddToggleWidgetByParentPanel(_panelRootTab);
	_toggleGroupTab.OnSelectChanged.BindUObject(this, &UGsUIWindowGameStatistics::OnSelectTab);
	_toggleGroupList.OnSlotClicked.BindUObject(this, &UGsUIWindowGameStatistics::OnClickList);
}

void UGsUIWindowGameStatistics::NativeConstruct()
{
	Super::NativeConstruct();

	BindMessages();
	
	_selectedIndex = -1;
	_toggleGroupTab.SetSelectedIndex(0, true);
	
	InvalidateAll();
}

void UGsUIWindowGameStatistics::NativeDestruct()
{
	UnbindMessages();

	Super::NativeDestruct();
}

bool UGsUIWindowGameStatistics::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsGameStatistics))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_GAME_STATISTICS);
		return true;
	}

	return false;
}

void UGsUIWindowGameStatistics::InvalidateAllInternal()
{
	FDateTime startDate;
	FDateTime endDate;
	if (GSGameStatistics()->GetDateTime(startDate, endDate))
	{
		int32 dateNum[3];
		int32 endDateNum[3];
		startDate.GetDate(dateNum[0], dateNum[1], dateNum[2]);
		endDate.GetDate(endDateNum[0], endDateNum[1], endDateNum[2]);

		FString strStartTime;
		FGsTimeStringHelper::GetTimeStringNYMD(startDate, strStartTime);

		FString strEndTime;
		FGsTimeStringHelper::GetTimeStringNYMD(endDate, strEndTime);

		FString strDate = FString::Format(TEXT("{0} ~ {1}"), { *strStartTime, *strEndTime });

		_textBlockDate->SetText(FText::FromString(strDate));
	}
	else
	{
		_textBlockDate->SetText(FText::GetEmpty());
	}

	InvalidateList(_selectedIndex);
	InvalidateGraph(_selectedIndex);
}

void UGsUIWindowGameStatistics::BindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	_msgHandleUIList.Emplace(msgMgr->GetUI().AddUObject(MessageUI::GAME_STATISTICS_UPDATE_LIST,
		this, &UGsUIWindowGameStatistics::OnUpdateList));
	_msgHandleUIList.Emplace(msgMgr->GetUI().AddUObject(MessageUI::GAME_STATISTICS_UPDATE_GRAPH,
		this, &UGsUIWindowGameStatistics::OnUpdateGraph));
}

void UGsUIWindowGameStatistics::UnbindMessages()
{
	FGsMessageHolder* msgMgr = GMessage();
	if (nullptr == msgMgr)
	{
		return;
	}

	MUI& msgUI = msgMgr->GetUI();
	for (MsgUIHandle& handle : _msgHandleUIList)
	{
		msgUI.Remove(handle);
	}
	_msgHandleUIList.Empty();
}

void UGsUIWindowGameStatistics::InvalidateList(int32 InSelectedIndex)
{
	StatisticsTabId tabId = GetSelectedTab();

	_dataList.Empty();
	_dataList.Append(GSGameStatistics()->GetDataList(tabId));

	// 카테고리 순으로 정렬
	_dataList.Sort([](const FGsGameStatisticsData& A, const FGsGameStatisticsData& B)
		{
			return (A._table->subCategory <= B._table->subCategory) ? true : false;
		});
	
	_slotHelper->RefreshAll(_dataList.Num());
	_toggleGroupList.Clear();
	_toggleGroupList.AddToggleWidgetByParentPanel(_panelRootList);
	_toggleGroupList.SetSelectedIndex(InSelectedIndex);
	
	_scroll->ScrollToStart();
}

void UGsUIWindowGameStatistics::InvalidateGraph(int32 InSelectedIndex)
{
	if (0 <= InSelectedIndex)
	{
		if (_dataList.IsValidIndex(InSelectedIndex))
		{
			if (const FGsGameStatisticsData* data = _dataList[InSelectedIndex])
			{
				if (data->_table &&
					data->_table->useDailyGraph)
				{
					_panelRootGraph->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					_panelRootEmptyGraph->SetVisibility(ESlateVisibility::Collapsed);

					_textBlockDataSubCategory->SetText(data->_table->subCategoryNameText);
					_textBlockDataName->SetText(data->_table->nameText);

					_uiLineGraph->SetData(data);

					return;
				}
			}
		}
	}

	_panelRootGraph->SetVisibility(ESlateVisibility::Collapsed);
	_panelRootEmptyGraph->SetVisibility(ESlateVisibility::HitTestInvisible);

	FText textDesc;
	if (0 > InSelectedIndex)
	{
		// TEXT: 목록을 선택하여 일일 변화 정보를 확인할 수 있습니다
		FText::FindText(TEXT("GameStatisticsText"), TEXT("DescSelectList"), textDesc);
	}
	else
	{
		// TEXT: 일일 변화 정보를 지원하지 않는 항목입니다
		FText::FindText(TEXT("GameStatisticsText"), TEXT("DescEmptyDailyInfo"), textDesc);
	}
	_textBlockEmptyGraph->SetText(textDesc);
}

void UGsUIWindowGameStatistics::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	if (false == _dataList.IsValidIndex(InIndex))
	{
		return;
	}

	if (UGsUIGameStatisticsListEntry* entry = Cast<UGsUIGameStatisticsListEntry>(InEntry))
	{
		entry->SetData(_dataList[InIndex]);
	}
}

void UGsUIWindowGameStatistics::OnSelectTab(int32 InIndex)
{
	_selectedIndex = -1;

	GSGameStatistics()->SendReqStatisticsInfo(GetSelectedTab());

	InvalidateAll();
}

void UGsUIWindowGameStatistics::OnClickList(int32 InIndex, bool bIsSelected)
{
	if (_selectedIndex == InIndex)
	{
		return;
	}

	_selectedIndex = InIndex;

	if (const FGsGameStatisticsData* data = GetSelectedData())
	{
		if (data->_table &&
			data->_table->useDailyGraph)
		{
			if (GSGameStatistics()->SendReqStaticsDailyInfo(data->_id))
			{
				return;
			}
		}
	}	
	
	InvalidateGraph(_selectedIndex);
}

void UGsUIWindowGameStatistics::OnUpdateList(const IGsMessageParam* InParam)
{
	InvalidateAll();
}

void UGsUIWindowGameStatistics::OnUpdateGraph(const IGsMessageParam* InParam)
{
	if (const FGsUIMsgParamInt* param = InParam->Cast<const FGsUIMsgParamInt>())
	{
		if (const FGsGameStatisticsData* data = GetSelectedData())
		{
			if (param->_data == data->_id)
			{
				InvalidateGraph(_selectedIndex);
			}
		}
	}
}

StatisticsTabId UGsUIWindowGameStatistics::GetSelectedTab() const
{
	return _toggleGroupTab.GetSelectedIndex() + 1;
}

const FGsGameStatisticsData* UGsUIWindowGameStatistics::GetSelectedData() const
{
	if (_dataList.IsValidIndex(_selectedIndex))
	{
		return _dataList[_selectedIndex];
	}

	return nullptr;
}
