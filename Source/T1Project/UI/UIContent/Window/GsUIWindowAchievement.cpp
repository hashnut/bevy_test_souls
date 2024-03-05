#include "GsUIWindowAchievement.h"

#include "Engine/AssetManager.h"

#include "Management/ScopeGame/GsAchievementManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Achievement/GsAchievementData.h"
#include "Achievement/GsUIAchievementItemMain.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsWrapBoxIconSelector.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/Control/GsDynamicIconSlotHelper.h"
#include "UI/UIContent/Common/GsUIProgressBar.h"
#include "UI/UIContent/Popup/GsUIPopupAchievementGroupList.h"

#include "ScrollBox.h"
#include "Image.h"
#include "TextBlock.h"
#include "WidgetSwitcher.h"

#include "Net/GsNetSendServiceWorld.h"

using AchievementList = UGsAchievementManager::AchievementList;
using MainAchievementPair = UGsAchievementManager::MainAchievementPair;

void UGsUIWindowAchievement::BeginDestroy()
{
	if (nullptr != _tabScrollBoxHelper)
	{
		_tabScrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIWindowAchievement::OnCreateTabButtonEntry);
		_tabScrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowAchievement::OnRefreshTabButtonEntry);
	}

	if (nullptr != _itemScrollBoxHelper)
	{
		_itemScrollBoxHelper->OnCreateEntry.RemoveDynamic(this, &UGsUIWindowAchievement::OnCreateListEntry);
		_itemScrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIWindowAchievement::OnRefreshListEntry);
	}

//	_tabIndexCategory.Reset();

	Super::BeginDestroy();
}

void UGsUIWindowAchievement::NativeOnInitialized()
{
	_helpContentsKey = EGsHelpContents::Achievement;

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowAchievement::OnClickCloseAllStack);
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowAchievement::OnClickBack);
	_btnRecvRewardAll->OnClicked.AddDynamic(this, &UGsUIWindowAchievement::OnReceiveAll);

	TArray<UWidget*> childList = _tabScrollPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUITabButtonCommon>())
		{
			child->RemoveFromParent();
		}
	}
	childList.Empty();


	_tabScrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_tabScrollBoxHelper->Initialize(_tabWidgetClass, _tabScrollPanel);
	_tabScrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIWindowAchievement::OnCreateTabButtonEntry);
	_tabScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowAchievement::OnRefreshTabButtonEntry);

	MaxTab = GSAchivement()->GetMaxTabIndex() + 1;

	_tabButtons.AddZeroed(MaxTab);
	_tabScrollBoxHelper->RefreshAll(MaxTab); //  static_cast<int>(EGsAchievementTabCategory::Max));

	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIWindowAchievement::OnSelectTab);

	childList = _listScrollPanel->GetAllChildren();
	for (auto child : childList)
	{
		if (child->IsA<UGsUIAchievementItem>())
		{
			child->RemoveFromParent();
		}
	}
	childList.Empty();

	_itemScrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_itemScrollBoxHelper->Initialize(_listEntryWidgetClass, _listScrollPanel);
	_itemScrollBoxHelper->OnCreateEntry.AddDynamic(this, &UGsUIWindowAchievement::OnCreateListEntry);
	_itemScrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowAchievement::OnRefreshListEntry);

	Super::NativeOnInitialized();
}

void UGsUIWindowAchievement::NativeConstruct()
{
	Super::NativeConstruct();

	_curTabIndex = MaxTab;
	_toggleGroup.SetSelectedIndex(0, true);

	//if (_curTabIndex >= GSAchivement()->GetMaxTabIndex())
	//{
	//	_toggleGroup.SetSelectedIndex(0, true);
	//}
	//else
	//{
	//	auto select = _curTabIndex;
	//	_curTabIndex = MaxTab;
	//	_toggleGroup.SetSelectedIndex(select, true);
	//}

	OnUpdateRedDot();
}

void UGsUIWindowAchievement::NativeDestruct()
{

	Super::NativeDestruct();
}

void UGsUIWindowAchievement::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UGsUIWindowAchievement::InitializeData(UIInitData* initParam)
{

}

void UGsUIWindowAchievement::OnClickBack()
{
	Super::OnClickBack();
}

void UGsUIWindowAchievement::OnClickCloseAllStack()
{
	Super::OnClickCloseAllStack();
}

bool UGsUIWindowAchievement::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsAchievement))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_ACHIEVEMENT);
		return true;
	}

	return false;
}

void UGsUIWindowAchievement::OnSelectTab(int32 index)
{
	if(_curTabIndex != index)
	{
		_curTabIndex = index;

		_listView->ScrollToStart();
		RefreshAchievementItem(_curTabIndex);
	}
}

void UGsUIWindowAchievement::OnCreateTabButtonEntry(UWidget* InEntry)
{
	if (auto tab = Cast<UGsUITabButtonCommon>(InEntry))
	{	
		_toggleGroup.AddToggleWidget(tab->GetToggleGroupEntry());
	}
}

void UGsUIWindowAchievement::OnRefreshTabButtonEntry(int32 index, UWidget* InEntry)
{
	if (auto tab = Cast<UGsUITabButtonCommon>(InEntry))
	{
		if (index >= MaxTab) return;

		_tabButtons[index] = tab;

		FText titleText;
		FText::FindText(TEXT("AchievementText"), FString::Printf(TEXT("UI_Text_Theme_%02d"), index + 1), titleText);
	
		tab->SetTitleText(titleText);
		tab->SetIsRedDot(EGsIconRedDotType::NORMAL, GSAchivement()->IsTabRedDot(index));
	}
}

void UGsUIWindowAchievement::OnCreateListEntry(UWidget* InEntry)
{

}

void UGsUIWindowAchievement::OnRefreshListEntry(int32 index, UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIAchievementItem>(InEntry))
	{
		if (_selectAchievements.IsValidIndex(index) && _selectAchievements[index].IsValid())
		{
			entry->SetData(_selectAchievements[index]);
			entry->RefreshUI();

			_selectAchievements[index].Pin()->_IsProgressUp = false;
		}
	}
}

void UGsUIWindowAchievement::RefreshAchievementItem(int32 tabIndex)
{
	MainAchievementPair dataSet = MainAchievementPair{nullptr, AchievementList()};
	GSAchivement()->GetGoOnAchievementsList(tabIndex, dataSet);

	RefreshMainAchievement(dataSet.Key);
	RefreshAchievements(dataSet.Value);
}

void UGsUIWindowAchievement::RefreshMainAchievement(TWeakPtr<FGsAchievementData> mainAchievement)
{
	_selectMainAchievement = mainAchievement;
	_mainAchievementItem->SetVisibility(mainAchievement.IsValid() ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

	_mainAchievementItem->SetData(mainAchievement);
	_mainAchievementItem->RefreshUI();

	mainAchievement.Pin()->_IsProgressUp = false;
}

void UGsUIWindowAchievement::RefreshAchievements(const TArray<TWeakPtr<FGsAchievementData>>& list)
{
	_selectAchievements.Empty();

	Move(_selectAchievements, list);

	_itemScrollBoxHelper->RefreshAll(_selectAchievements.Num());
}

void UGsUIWindowAchievement::OnReceive()
{
	if (auto mainAchievement = _selectMainAchievement.Pin())
	{
		FGsNetSendServiceWorld::SendReqAchievementRewards(mainAchievement->_achievementId);
	}
}

void UGsUIWindowAchievement::OnReceiveAll()
{
	FGsNetSendServiceWorld::SendReqAchievementEnableRewardList();
}

void UGsUIWindowAchievement::OnAchievementGroupList()
{
	if (auto mainAchievement = _selectMainAchievement.Pin())
	{
		if (auto PopupDetail = Cast<UGsUIPopupAchievementGroupList>(GUI()->OpenAndGetWidget(TEXT("PopupAchievementDetail"))))
		{
			UGsUIPopupAchievementGroupList::PopupInitData param = { mainAchievement->_groupId };
			PopupDetail->InitializeData(&param);
			PopupDetail->RefreshUI();
		}
	}
}

void UGsUIWindowAchievement::OnLongPressIcon(UGsUIIconItem& InIcon)
{
	UGsUIIconItem::DefaultItemDetailOpener(InIcon, true);
}

void UGsUIWindowAchievement::OnUpdateAchievementItem(TWeakPtr<FGsAchievementData> achievementData)
{
	auto IsCurTabAchievement = [this](TWeakPtr<FGsAchievementData> achievementData)->bool {

		if (achievementData.IsValid() && _selectMainAchievement.IsValid())
			return achievementData.Pin()->TabIndex() == _selectMainAchievement.Pin()->TabIndex();
		return false;
	};
	

	if (achievementData.IsValid() && IsCurTabAchievement(achievementData))
	{	
		if (achievementData.Pin()->IsMainAchievement())
		{
			RefreshMainAchievement(achievementData);
		}
		else
		{
			auto groupId = achievementData.Pin()->_groupId;
			auto index = _selectAchievements.IndexOfByPredicate([groupId](auto e) { return e.Pin()->_groupId == groupId; });
			if (index != INDEX_NONE)
			{
				_selectAchievements[index] = achievementData;
				_itemScrollBoxHelper->RefreshByIndex(index);
			}
			else //새로운 것이 추가(groupId가 추가되는 경우는 없을 것 같지만 만약)되면 전체 갱신
			{
				MainAchievementPair dataSet = MainAchievementPair{ nullptr, AchievementList() };
				GSAchivement()->GetGoOnAchievementsList(_curTabIndex, dataSet);
				RefreshAchievements(dataSet.Value);
			}
		}
	}
}

void UGsUIWindowAchievement::OnAckReceiveAchievementReward(AchievementId id)
{
	if (_selectMainAchievement.IsValid() && _selectMainAchievement.Pin()->_achievementId == id)
	{	
		RefreshMainAchievement(_selectMainAchievement);
	}
	else
	{
		auto index = _selectAchievements.IndexOfByPredicate([id](auto e) { return e.Pin()->_achievementId == id; });
		if (index != INDEX_NONE)
		{
			_itemScrollBoxHelper->RefreshByIndex(index);
		}
	}
}

void UGsUIWindowAchievement::OnAckReceiveAchievementRewardAll(const TArray<AchievementId>& achievementIds) //const TArray<AchievementThemeId>& themeIds, 
{
	if (_selectMainAchievement.IsValid() && achievementIds.Find(_selectMainAchievement.Pin()->_achievementId))
	{
		RefreshMainAchievement(_selectMainAchievement);
	}

	for (auto id : achievementIds)
	{
		auto index = _selectAchievements.IndexOfByPredicate([id](auto e) { return e.Pin()->_achievementId == id;});
		if (index != INDEX_NONE)
		{
			_itemScrollBoxHelper->RefreshByIndex(index);
		}
	}

	_btnRecvRewardAll->SetIsEnabled(GSAchivement()->IsRedDot());
}

void UGsUIWindowAchievement::OnUpdateRedDot()
{
	_tabScrollBoxHelper->RefreshAll(MaxTab);
	_btnRecvRewardAll->SetIsEnabled(GSAchivement()->IsRedDot());
}
