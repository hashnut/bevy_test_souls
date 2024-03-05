
#include "GsUIWindowInvade.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

#include "Title/GsUITitleBattleLevelEntry.h"
#include "Invade/GsUIInvadeMapEntry.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"

#include "Management/ScopeGame/GsInvadeBattleLevelManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/GsMessageHolder.h"

#include "Map/WorldMap/GsWorldMapFunc.h"

#include "SScrollBox.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"

#include "DataSchema/InterServer/GsSchemaInvadeBattleLevel.h"
#include "DataSchema/InterServer/GsSchemaInvadeWorldConfigDataEx.h"

#include "UTIL/GsCronUtil.h"

void UGsUIWindowInvade::BeginDestroy()
{
	Super::BeginDestroy();
}

void UGsUIWindowInvade::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 상단 버튼
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowInvade::OnClickBack);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowInvade::OnClickCloseAllStack);

	// 2022/01/19 PKT - Main Category
	_toggleGroupMainTabs.AddToggleWidget(_switcherBtnInvadeDesc);
	_toggleGroupMainTabs.AddToggleWidget(_switcherBtnInvadeTitle);
	_toggleGroupMainTabs.OnSelectChanged.BindUObject(this, &UGsUIWindowInvade::OnSelectedBtnMainCategory);
	
	_slotHelperBattleTitle = NewObject<UGsDynamicPanelSlotHelper>(this);
	_slotHelperBattleTitle->Initialize(_subClassOfBattleTitleEntry, _scrollBoxBattleTitle);
	_slotHelperBattleTitle->OnRefreshEntry.AddDynamic(this, &UGsUIWindowInvade::OnRefreshEntryBattleTitle);

	_invadeMapListHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_invadeMapListHelper->Initialize(_entryInvadeMapWidget, _invadeMapPanel);
	_invadeMapListHelper->OnRefreshEntry.AddDynamic(this, &UGsUIWindowInvade::OnRefreshEntryInvasionMap);
	_invadeMapListHelper->OnCreateEntry.AddDynamic(this, &UGsUIWindowInvade::OnCreateEntryInvasionMap);
}

void UGsUIWindowInvade::NativeConstruct()
{
	Super::NativeConstruct();

	TArray<UWidget*> children = _invadeMapPanel->GetAllChildren();
	for (auto child : children)
	{
		if (child->IsA<UGsUIInvadeMapEntry>())
		{
			child->RemoveFromParent();
		}
	};

	static auto GetPlayTime = []()->FString {

		FString resultTime;
		if (const auto table = Cast<UGsTableInvadeWorldConfigDataEx>(FGsSchemaInvadeWorldConfigDataEx::GetStaticTable()))
		{
			TArray<const FGsSchemaInvadeWorldConfigDataEx*> invasionWorldConfigs;
			table->GetAllRows(invasionWorldConfigs);

			if (invasionWorldConfigs.IsValidIndex(0))
			{
				TArray<FString> invasionTimes;
				FGsCronUtil::GetCronStrings(invasionWorldConfigs[0]->invadeStartCron, GSInterServer()->GetInvasionDurationMin(), invasionTimes);
				for (FString str : invasionTimes)
				{
					if (str.IsEmpty()) continue;
					if (resultTime.IsEmpty() == false) resultTime += TEXT("\n");
					resultTime += str;
				}
			}
		}
		return resultTime; 
	};

	_playTime = FText::FromString(GetPlayTime());

	const static int32 DEFAULT_SWITCHER_INDEX = StaticCast<int32>(ContetnsViewType::InvadeDesc);
	_toggleGroupMainTabs.SetSelectedIndex(DEFAULT_SWITCHER_INDEX, true);
}

void UGsUIWindowInvade::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGsUIWindowInvade::OnSelectedBtnMainCategory(int32 InIndex)
{
	_switcherContentsView->SetActiveWidgetIndex(InIndex);

	InvalidateView();
}

void UGsUIWindowInvade::OnRefreshEntryBattleTitle(int32 InIndex, UWidget* InEntry)
{
	TWeakObjectPtr<UGsUITitleBattleLevelEntry> entry = Cast<UGsUITitleBattleLevelEntry>(InEntry);
	if (false == entry.IsValid())
	{
		GSLOG(Warning, TEXT("false == entry.IsValid()"));
		return;
	}

	FGsInvadeBattleLevelManager* battleLevelManager = GsInvadeBattleLevelManager();
	if (nullptr == battleLevelManager)
	{
		GSLOG(Error, TEXT("nullptr == battleLevelManager"));
		return;
	}

	const TArray<const FGsSchemaInvadeBattleLevel*>& viewList = battleLevelManager->ViewList();
	if (false == viewList.IsValidIndex(InIndex))
	{
		GSLOG(Error, TEXT("false == viewList.IsValidIndex(InIndex)"));
		return;
	}

	const FGsNetUserData* userData = GGameData()->GetUserData();
	int32 currentLevel = -1;
	if (userData)
	{
		currentLevel = userData->_InvadeBattleLevel;
	}

	const FGsSchemaInvadeBattleLevel* battleLevel = viewList[InIndex];
	entry->SetData(battleLevel, currentLevel);

	/**
	 * 2022/10/13 PKT - 0레벨의 칭호는 기본 등급의 칭호 로써 리스트에 보여 주지 않는다.(feat. 기획 : 김동규님 /서버 : 이길상님)
	 *					이에 따라 0레벨의 칭호는 필요 갯수를 언제나 0으로 해야 한다.
	 */
	const static int32 DEFAULT_TITLE_LEVEL = 0;
	entry->SetVisibility((DEFAULT_TITLE_LEVEL == battleLevel->battleLevel) ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
}

void UGsUIWindowInvade::OnRefreshEntryInvasionMap(int32 InIndex, UWidget* InEntry)
{
	if (_activePortalInMaps.IsValidIndex(InIndex) && InEntry->IsA<UGsUIInvadeMapEntry>())
	{
		if (auto entry = Cast<UGsUIInvadeMapEntry>(InEntry))
		{
			entry->SetMapInfo(_activePortalInMaps[InIndex]);
		}
	}
}

void UGsUIWindowInvade::OnCreateEntryInvasionMap(UWidget* InEntry)
{
	if (auto entry = Cast<UGsUIInvadeMapEntry>(InEntry))
	{
		entry->OnGoWorldMap.BindUObject(this, &UGsUIWindowInvade::OpenInvasionWorldMap);
	}
}

void UGsUIWindowInvade::InvalidateView()
{
	ContetnsViewType viewType = StaticCast<ContetnsViewType>(_switcherContentsView->GetActiveWidgetIndex());

	switch (viewType)
	{
	case ContetnsViewType::InvadeDesc:
		InvalidateDesc();
		break;
	case ContetnsViewType::InvadeTitle:
		InvalidateTitle();
		break;
	}
}

void UGsUIWindowInvade::InvalidateDesc()
{
	_reqPortalInfo = true;
	_invadeMapInfoSwitcher->SetActiveWidget(_emptyMapPanel);

	GSInterServer()->ReqInvasionPortalInfo();

	if (_reqPortalInfo)
	{
		_activePortalInMaps.Reset();

		_reqPortalInfo = false;

		auto count = GSInterServer()->GetAirRaidMapList(_activePortalInMaps);
		_invadeMapListHelper->RefreshAll(count);
		_invadeMapInfoSwitcher->SetActiveWidget(count <= 0 ? _emptyMapPanel : _activeMapPanel);
	}
}

void UGsUIWindowInvade::InvalidateTitle()
{
	FGsInvadeBattleLevelManager* battleLevelManager = GsInvadeBattleLevelManager();
	if (nullptr == battleLevelManager)
	{
		GSLOG(Error, TEXT("nullptr == battleLevelManager"));
		return;
	}

	const auto& viewList = battleLevelManager->ViewList();
	_slotHelperBattleTitle->RefreshAll(viewList.Num());

	InvalidateBattleBadge();
}

void UGsUIWindowInvade::InvalidateBattleBadge()
{
	FGsInvadeBattleLevelManager* battleLevelManager = GsInvadeBattleLevelManager();
	if (nullptr == battleLevelManager)
	{
		GSLOG(Error, TEXT("nullptr == battleLevelManager"));
		return;
	}

	if (const FGsNetUserData* userData = GGameData()->GetUserData())
	{
		InvadeBattleLevel level = userData->_InvadeBattleLevel;
		const FGsSchemaInvadeBattleLevel* schemaBattleLevel = battleLevelManager->FindBattleLevelInfo(level);

		FText message = (nullptr != schemaBattleLevel) ? schemaBattleLevel->LevelName : FText::FromString(FString::Printf(TEXT("-")));
		_textBattleTitleName->SetText(message);

		// 2023/10/11 PKT - 공격형 정수
		CurrencyDelta attackCurrencyAmount = battleLevelManager->GetAttackCurrencyItemCount();
		// 2023/10/11 PKT - 방어형 정수
		CurrencyDelta defenceCurrencyAmount = battleLevelManager->GetDefenceCurrencyItemCount();
		_textAttackCurrencyItemCount->SetText(FText::AsNumber(attackCurrencyAmount));
		_texDefenceCurrencyItemCount->SetText(FText::AsNumber(defenceCurrencyAmount));
		_textCurrencyItemCount->SetText(FText::AsNumber(attackCurrencyAmount + defenceCurrencyAmount));

		UWidget* focusEntry = _slotHelperBattleTitle->GetEntry(level - 1);
		if (focusEntry)
		{
			_scrollBoxBattleTitle->ScrollWidgetIntoView(focusEntry, false, EDescendantScrollDestination::Center);
		}
	}
}

void UGsUIWindowInvade::InvasionPortalSpawnInfo()
{
	_reqPortalInfo = false;
	_activePortalInMaps.Reset();

	auto count = GSInterServer()->GetAirRaidMapList(_activePortalInMaps);
	_invadeMapListHelper->RefreshAll(0);
	_invadeMapListHelper->RefreshAll(count);

	_invadeMapInfoSwitcher->SetActiveWidget(count <= 0 ? _emptyMapPanel : _activeMapPanel);
}

void UGsUIWindowInvade::OpenInvasionWorldMap(MapId mapId)
{
	//if (false == GSUnlock()->IsContentsUnlock(EGsUnlockContentsType::WORLDMAP))
	//{
	//	GSUnlock()->ShowLockTicker(EGsUnlockContentsType::WORLDMAP);
	//	return;
	//}

	// 언락 컨텐츠와 가능 타입 맵, 관전 모드 체크 한번에 하자
	if (FGsWorldMapFunc::IsRegionMapOpenPossible() == false)
	{
		return;
	}

	if (FGsGameFlowGame* gameFlow = GMode() ? GMode()->GetGameFlow() : nullptr)
	{
		if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
		{
			if (FGsGameStateRegionMap* regionMap =
				contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
			{
				regionMap->SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_INVASION, mapId);
			}
		}
	}

	// 지역맵으로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);
}

bool UGsUIWindowInvade::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsInvade))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_INVADE);
		return true;
	}

	return false;
}