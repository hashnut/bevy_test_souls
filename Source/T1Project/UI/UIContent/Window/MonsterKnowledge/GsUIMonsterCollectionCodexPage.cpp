// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UIContent/Window/MonsterKnowledge/GsUIMonsterCollectionCodexPage.h"

#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeData.h"
#include "../MonsterKnowledgeCollection/GsMonsterKnowledgeUICaptureData.h"
#include "../Management/ScopeGlobal/GsUIManager.h"
#include "../Management/ScopeGame/GsMonsterCollectionManager.h"
#include "../Management/ScopeGame/GsCodexManager.h"
#include "../Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGame/GsEffectTextManager.h"
#include "../UI/UIControlLib/ContentWidget/GsButton.h"
#include "../UI/UIControlLib/ContentWidget/GsToggleButton.h"
#include "../UI/UIContent/Common/GsUITabButtonCommon.h"
#include "../UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "../UI/UIContent/Popup/MonsterKnowledgeCollection/GsUIPopupMonsterBookDetail.h"
#include "../UI/UIContent/Popup/MonsterKnowledgeCollection/GsUIPopupMonsterBookUsedItem.h"
#include "../UI/UIContent/Popup/MonsterKnowledgeCollection/GsUIPopupMonsterCollectionBookResult.h"
#include "../UI/UIContent/Popup/MonsterKnowledgeCollection/GsUIPopupMonsterCollectionBookGradeRewardInfo.h"
#include "../UI/UIContent/Popup/MonsterKnowledgeCollection/GsUIPopupCodexGemUse.h"
#include "UI/UIContent/Window/MonsterKnowledge/GsUICodexMapGroupListEntry.h"
#include "UI/UIContent/Window/MonsterKnowledge/GsUICodexExplorationLevelEntry.h"
#include "UI/UIContent/Window/MonsterKnowledge/GsUICodexPortrait.h"
#include "Object.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/TileView.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "../Net/GsNetSendServiceWorld.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "T1Project.h"
#include "Codex/GsCodexTypes.h"
#include "EffectText/GsEffectTextDefine.h"
#include "GameObject/Stat/GsStatHelper.h"

#include "Classes/GsPoolUObject.h"

#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "UI/UIContent/Common/RedDot/GsUIRedDotDefault.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipDesc.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "Image.h"
#include "UI/UIContent/Helper/GsUIStringHelper.h"



void UGsUIMonsterCollectionCodexPage::BeginDestroy()
{
	if (_poolPortraitData)
	{
		_poolPortraitData->RemovePool();
		_poolPortraitData = nullptr;
	}

	if (_poolMapGroupItem)
	{
		_poolMapGroupItem->RemovePool();
		_poolMapGroupItem = nullptr;
	}

	Super::BeginDestroy();
}

void UGsUIMonsterCollectionCodexPage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 대륙 카테고리
	_slotHelperCategoryView = NewObject<UGsDynamicPanelSlotHelper>();
	_slotHelperCategoryView->Initialize(_subClassOfCategoryView, _scrollCategoryView);
	_slotHelperCategoryView->OnRefreshEntry.AddUniqueDynamic(this, &UGsUIMonsterCollectionCodexPage::OnRefreshEntryCategoryView);
	_toggleGroupCategoryView.OnSelectChanged.BindUObject(this, &UGsUIMonsterCollectionCodexPage::OnSelectChangedCategoryView);

	_listViewMapGroup->OnItemClicked().AddUObject(this, &UGsUIMonsterCollectionCodexPage::OnClickedMapGroupListItem);
	_listViewMapGroup->OnListViewScrolled().AddUObject(this, &UGsUIMonsterCollectionCodexPage::OnScrolledMapGroupList);

	const static int32 PORTRAIT_POOL_WARM_UP_SIZE = 50;
	_poolPortraitData = NewObject<UGsPoolUObject>(this);
	_poolPortraitData->WarmUp<UGsUICodexPortraitEntryData>(PORTRAIT_POOL_WARM_UP_SIZE);

	const static int32 MAP_GROUP_POOL_WARM_UP_SIZE = 15;
	_poolMapGroupItem = NewObject<UGsPoolUObject>(this);
	_poolMapGroupItem->WarmUp<UGsUICodexMapGroupListItem>(MAP_GROUP_POOL_WARM_UP_SIZE);

	_btnConvertPoint->OnClicked.AddUniqueDynamic(this, &UGsUIMonsterCollectionCodexPage::OnClickedConvertCodexPoint);
	_btnExploreLevelUp->OnClicked.AddUniqueDynamic(this, &UGsUIMonsterCollectionCodexPage::OnClickedCategoryLevelUp);
	 
	// 대륙 레벨업 버튼의 경우, 광클방지 시간을 설정
	_btnExploreLevelUp->SetOnClickLockSec(1.0f);

	_btnUseKnowledgeGem->OnClicked.AddUniqueDynamic(this, &UGsUIMonsterCollectionCodexPage::OnClickedUseKnowledgeGem);

	_toggleBtnKnowledgeGain->OnClicked.AddUniqueDynamic(this, &UGsUIMonsterCollectionCodexPage::OnClickedKnowledgeGainTab);
	_toggleBtnExploreCategory->OnClicked.AddUniqueDynamic(this, &UGsUIMonsterCollectionCodexPage::OnClickedCategoryExploreTab);
	
	_btnCloseMapGroupMonster->OnClicked.AddUniqueDynamic(this, &UGsUIMonsterCollectionCodexPage::OnClickedCloseMapGroupMonster);

	// 텍스트 캐싱
	FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_Text_MapGroupLevel"), _levelText);

	FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_InfoIcon_TotalKnowledge"), _totalKnowledgeTooltipText);
	FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_InfoIcon_MonsterBookPoint"), _codexPointTooltipText);
	FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_InfoIcon_CategoryLevelInfo"), _exploreLevelTooltipText);
	FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_Popup_SelectedMapGroup_InfoIcon_MapGroup"), _mapNameTooltipText);
	FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_SelectedMapGroup_InfoIcon_MonsterList"), _monsterListTooltipText);

	_tileViewMapGroupMonster->OnItemClicked().AddUObject(this, &UGsUIMonsterCollectionCodexPage::OnClickedMonsterTileViewItem);

	_btnTooltipTotalKnowledge->OnPressed.AddUniqueDynamic(this, &UGsUIMonsterCollectionCodexPage::OnPressedBtnTooltipTotalKnowledge);
	_btnTooltipCodexPoint->OnPressed.AddUniqueDynamic(this, &UGsUIMonsterCollectionCodexPage::OnPressedBtnTooltipCodexPoint);
	_btnTooltipExploreLevel->OnPressed.AddUniqueDynamic(this, &UGsUIMonsterCollectionCodexPage::OnPressedBtnTooltipExploreLevel);
	_btnTooltipMapName->OnPressed.AddUniqueDynamic(this, &UGsUIMonsterCollectionCodexPage::OnPressedBtnTooltipMapName);
	_btnTooltipMonsterList->OnPressed.AddUniqueDynamic(this, &UGsUIMonsterCollectionCodexPage::OnPressedBtnTooltipMonsterList);
}
void UGsUIMonsterCollectionCodexPage::NativeConstruct()
{
	Super::NativeConstruct();
}

void UGsUIMonsterCollectionCodexPage::NativeDestruct()
{
	SetTutorial(false);
	SetIsTutorialInteraction(false);

	Super::NativeDestruct();
}

void UGsUIMonsterCollectionCodexPage::Enter(FGsMonsterKnowledgeUICaptureData* InData)
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	if (nullptr == InData)
		return;

	// 예약된 것이 있으면 해당 지점으로 돌아가고, 아니면 현재 속한 Map 에 맞는 카테고리가 선택되도록 한다
	if (InData->_currentCategoryId == INVALID_CODEX_CATEGORY_ID)
	{
		int32 mapId = GLevel()->GetCurrentLevelId();
		_currentCategoryId = GsCodex()->GetCategoryIdWithMapId(mapId);

		// 캡쳐된 녀석이 없으면 다시 초기화
		_currentMapGroupId = INVALID_CODEX_MAP_GROUP_ID;
		_mapGroupIndex = -1;
		_isUseKnowledgeGemPopup = false;
		_monsterTileIndex = -1;
	}
	else
	{
		_currentCategoryId = InData->_currentCategoryId;
		_currentMapGroupId = InData->_currentMapGroupId;
	}

	// 만약 유효한 카테고리를 찾지 못했으면, 첫 녀석을 선택한다
	if (_currentCategoryId == INVALID_CODEX_CATEGORY_ID)
		_currentCategoryId = codexManager->GetCategoryIdWithIndex(0);

	_slotHelperCategoryView->RefreshAll(codexManager->GetTotalCategoryCount());

	_currentTabIndex = codexManager->GetIndexWithCategoryId(_currentCategoryId);
	_toggleGroupCategoryView.SetSelectedIndex(_currentTabIndex);

	_mapGroupIndex = InData->_mapGroupIndex;

	if (_mapGroupIndex == -1)
		InvalidateMapGroupList(EGsMapGroupRefreshType::SCROLL_TO_TOP);
	else
		InvalidateMapGroupList(EGsMapGroupRefreshType::SCROLL_TO_ITEM);

	if (_currentMapGroupId != INVALID_CODEX_MAP_GROUP_ID)
		InvalidateRightArea(EGsRightArea::MONSTER_MAPGROUP);
	else
		InvalidateRightArea(EGsRightArea::KNOWLEDGE_GAIN);

	if (InData->_isUseKnowledgeGemPopup)
	{
		OnClickedUseKnowledgeGem();
	}

	InvalidateRedDot();

	// 첫 진입 시, 배경 이미지도 바꿔주어야 함
	InvalidateCategoryBG();
}

void UGsUIMonsterCollectionCodexPage::Leave()
{
}

void UGsUIMonsterCollectionCodexPage::Capture(FGsMonsterKnowledgeUICaptureData* OutData)
{
	OutData->_currentCategoryId = _currentCategoryId;
	OutData->_currentMapGroupId = _currentMapGroupId;
	OutData->_mapGroupIndex = _mapGroupIndex;
	
	OutData->_isUseKnowledgeGemPopup = _isUseKnowledgeGemPopup;
	OutData->_monsterTileIndex = _monsterTileIndex;
}

void UGsUIMonsterCollectionCodexPage::InvalidateMapGroupList(EGsMapGroupRefreshType InScrollType /*= EGsMapGroupRefreshType::NO_SCROLL*/)
{
	if (INVALID_CODEX_CATEGORY_ID == _currentCategoryId)
		return;

	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	_poolMapGroupItem->ReleaseAll();

	TArray<UGsUICodexMapGroupListItem*> mapGroupItemList;
	for (CodexMapGroupId mapGroupId : codexManager->GetMapGroupListWithId(_currentCategoryId))
	{
		UGsUICodexMapGroupListItem* item = _poolMapGroupItem->Claim<UGsUICodexMapGroupListItem>();

		item->SetData(
			mapGroupId,
			codexManager->GetMapGroupIsMaxLevelWithId(mapGroupId),
			FText::FromString(FString::FromInt(codexManager->GetMapGroupCurrentLevelWithId(mapGroupId))),
			codexManager->GetMapGroupNameWithId(mapGroupId),
			codexManager->GetMapGroupImageWithId(mapGroupId),
			codexManager->GetMapGroupCurrentExpPercentWithId(mapGroupId)
		);

		// 혹시 선택된 녀석이 있다면 하이라이트 처리
		if (_currentMapGroupId == mapGroupId)
		{
			item->SetIsSelected(true);
		}

		mapGroupItemList.Emplace(item);
	}

	_listViewMapGroup->SetListItems(mapGroupItemList);
	_listViewMapGroup->RegenerateAllEntries();
	_listViewMapGroup->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	// 이전에 선택되어 돌아와야 하는 경우
	switch (InScrollType)
	{
	case EGsMapGroupRefreshType::NO_SCROLL:
		break;
	case EGsMapGroupRefreshType::SCROLL_TO_ITEM:
		_listViewMapGroup->ScrollIndexIntoView(_mapGroupIndex);
		break;
	case EGsMapGroupRefreshType::SCROLL_TO_TOP:
		_listViewMapGroup->ScrollToTop();
		break;
	default:
		break;
	}

	// 지식의 결정 사용 시, 맵 그룹 레벨이 증가할 수도 있어서 추가
	InvalidateMapGroupText();
}

void UGsUIMonsterCollectionCodexPage::InvalidateRightArea(EGsRightArea InRightArea /*= EGsRightArea::KNOWLEDGE_GAIN*/)
{
	_widgetSwitcherRightArea->SetActiveWidgetIndex(static_cast<int32>(InRightArea));

	switch (InRightArea)
	{
	case EGsRightArea::KNOWLEDGE_GAIN:
	{
		_knowledgeExploreTabArea->SetVisibility(ESlateVisibility::Visible);
		_toggleBtnKnowledgeGain->SetIsSelected(true);
		_toggleBtnExploreCategory->SetIsSelected(false);
		InvalidateKnowledgeGain();
		break;
	}
	case EGsRightArea::CATEGORY_EXPLORE:
	{
		_knowledgeExploreTabArea->SetVisibility(ESlateVisibility::Visible);
		_toggleBtnKnowledgeGain->SetIsSelected(false);
		_toggleBtnExploreCategory->SetIsSelected(true);
		InvalidateCategoryExplore();
		break;
	}
	case EGsRightArea::MONSTER_MAPGROUP:
	{
		_knowledgeExploreTabArea->SetVisibility(ESlateVisibility::Collapsed);
		InvalidateMonsterMapGroup();
		break;
	}
	default:
		break;
	}
}

void UGsUIMonsterCollectionCodexPage::InvalidateKnowledgeGain()
{
	FGsCodexManager* codexMgr = GsCodex();
	if (nullptr == codexMgr)
		return;

	MonsterKnowledgeExp accumlateKnowledgeExp = codexMgr->GetAccumulateMonsterKnowledgeExp();
	MonsterKnowledgeExp finalMonsterKnowledgeExp = codexMgr->GetFinalMonsterKnowledgeExp();
	Currency finalMonsterBookPoint = finalMonsterKnowledgeExp * 10;

	FText totalKnowledgeText;
	FGsUIStringHelper::GetAmountText(accumlateKnowledgeExp, MAX_uint64, totalKnowledgeText);
	_textTotalKnowledge->SetText(totalKnowledgeText);

	FText finalMonsterKnowledgeExpText;
	FGsUIStringHelper::GetAmountText(finalMonsterKnowledgeExp, MAX_uint64, finalMonsterKnowledgeExpText);
	_textCurrentTotalKnowledge->SetText(finalMonsterKnowledgeExpText);

	FText finalMonsterBookPointText;
	FGsUIStringHelper::GetAmountText(finalMonsterBookPoint, MAX_uint64, finalMonsterBookPointText);
	_textConvertibleTotalBookPoint->SetText(finalMonsterBookPointText);

	FText convertText;
	if (finalMonsterKnowledgeExp != 0)
		FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_Text_CanGetMonsterBookPoint"), convertText);
	else
		FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_Text_CanNotGetMonsterBookPoint"), convertText);

	_textConvertKnowledge->SetText(convertText);

	_btnConvertPoint->SetIsEnabled(finalMonsterKnowledgeExp != 0);

	InvalidateRedDotKnowledgeGain();
	InvalidateRedDotCategory();
}

void UGsUIMonsterCollectionCodexPage::InvalidateCategoryExplore()
{
	// '탐사 레벨업' 은 기본적으로 딤드 처리 (아래에서 레벨업 가능할 때만 활성화)
	_btnExploreLevelUp->SetIsEnabled(false);

	if (INVALID_CODEX_CATEGORY_ID == _currentCategoryId)
		return;

	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	FGsEffectTextManager* effectTextMgr = GSEffectText();
	if (nullptr == effectTextMgr)
	{
		GSLOG(Error, TEXT("nullptr == effectTextMgr"));
		return;
	}

	// {0} 탐사 {1}레벨 채우기
	FText nameText;
	FText::FindText(TEXT("CodexUIText"), TEXT("UI_MapList_Ticker_CategoryLevelUp_CategoryNameAndLevel"), nameText);
	nameText = FText::Format(nameText, GsCodex()->GetCategoryNameWithId(_currentCategoryId), FText::FromString(FString::FromInt(codexManager->GetCurrentCategoryMapGroupLevelSumWithId(_currentCategoryId))));

	_textSlateTitle->SetText(nameText);

	int32 currentCategoryMapGroupLevelSum = 0;
	TArray<CodexMapGroupId> mapGroupIdList = codexManager->GetMapGroupListWithId(_currentCategoryId);
	for (CodexMapGroupId mapGroupId : codexManager->GetMapGroupListWithId(_currentCategoryId))
		currentCategoryMapGroupLevelSum +=  codexManager->GetMapGroupCurrentLevelWithId(mapGroupId);

	// 미달성된 항목 중 제일 작은 Index. 이 항목으로 Scroll 해 줄 것임
	TOptional<int32> _minUnachievedIndex;

	TArray<UGsUICodexExplorationLevelItem*> mapGroupItemList;
	TArray<CategoryBalanceData> categoryBalanceDataList = codexManager->GetCategoryBalanceDataListWithId(_currentCategoryId);
	for (int32 i = 0; i < categoryBalanceDataList.Num(); ++i)
	{
		UGsUICodexExplorationLevelItem* item = NewObject<UGsUICodexExplorationLevelItem>(this);

		int32 mapGroupLevelUpCount = categoryBalanceDataList[i].mapGroupLevelUpCount;

		int32 currentCategoryLevel = codexManager->GetCurrentCategoryLevelWithId(_currentCategoryId);

		// 현재 레벨이 도달 가능한 레벨인지를 체크
		bool isCanAchievable = (currentCategoryMapGroupLevelSum >= mapGroupLevelUpCount);

		// 지역 스탯인지 글로벌한 스탯인지 체크
		bool isGlobal = categoryBalanceDataList[i].isGlobal;

		EGsExplorationLevelState levelState = (isGlobal ? EGsExplorationLevelState::LEVEL_INACTIVE_GLOBAL : EGsExplorationLevelState::LEVEL_INACTIVE_LOCAL);

		if (isCanAchievable)
		{
			// 이미 달성했다면
			if (currentCategoryLevel >= i + 1)
			{
				levelState = (isGlobal) ? EGsExplorationLevelState::LEVEL_ACHIEVED_GLOBAL : EGsExplorationLevelState::LEVEL_ACHIEVED_LOCAL;
			}
			// 레벨 업 가능한 상태
			else
			{
				levelState = (isGlobal) ? EGsExplorationLevelState::LEVEL_UP_GLOBAL : EGsExplorationLevelState::LEVEL_UP_LOCAL;
			}
		}

		// 레벨 업 가능 시, '탐사 레벨업' 활성화
		if (levelState == EGsExplorationLevelState::LEVEL_UP_LOCAL || levelState == EGsExplorationLevelState::LEVEL_UP_GLOBAL)
			_btnExploreLevelUp->SetIsEnabled(true);

		// 달성 불가능하며, 아직 제일 낮은 항목을 찾지 않았을 때
		if (!isCanAchievable && !_minUnachievedIndex.IsSet())
			_minUnachievedIndex = i;

		// 스탯 텍스트 파싱
		FGsEffectTextCollection effectCollection;
		effectTextMgr->GetEffectTextColectionPassivity(categoryBalanceDataList[i].passivityId, effectCollection);

		FText statText;
		FString statDescString;
		for (TPair<StatType, int32> iter : effectCollection._statMap)
		{
			// 개행처리
			if (false == statDescString.IsEmpty())
				statDescString.Append(TEXT("\n"));
			FGsStatHelper::GetDesplayStatNameAndValue(iter.Key, iter.Value, statDescString);
		}
		statText = FText::FromString(statDescString);

		item->SetData(
			(i == 0),
			(levelState == EGsExplorationLevelState::LEVEL_INACTIVE_LOCAL || levelState == EGsExplorationLevelState::LEVEL_INACTIVE_GLOBAL ? false : true),
			levelState,
			FText::FromString(FString::FromInt(categoryBalanceDataList[i].mapGroupLevelUpCount)),
			MoveTemp(statText));

		mapGroupItemList.Emplace(item);
	}
	_listViewExploreLevel->SetListItems(mapGroupItemList);
	_listViewExploreLevel->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	if (_minUnachievedIndex.IsSet())
		_listViewExploreLevel->NavigateToIndex(_minUnachievedIndex.GetValue());

	InvalidateRedDotCategory();
}

void UGsUIMonsterCollectionCodexPage::InvalidateMonsterMapGroup()
{
	InvalidateMapGroupText();
	InvalidateMonsterTileView();
}

void UGsUIMonsterCollectionCodexPage::InvalidateMapGroupText()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	// 아직 _currentMapGroupId 가 세팅되지 않았다면 Pass
	if (_currentMapGroupId == INVALID_CODEX_MAP_GROUP_ID)
		return;

	// 맵 그룹 영역 텍스트 관련 세팅
	_textMapGroupName->SetText(codexManager->GetMapGroupNameWithId(_currentMapGroupId));

	int32 currentLevel = codexManager->GetMapGroupCurrentLevelWithId(_currentMapGroupId);
	_textMapGroupLevel->SetText(FText::Format(_levelText, currentLevel));

	// 만약 최대 레벨이면, 목표 누적 경험치는 최댓값을 넣어줄 것이다.
	int32 targetExp = 0;
	TArray<int32> accumulatedExpList = codexManager->GetAccumulateKnowledgeExpListWithMapGroupId(_currentMapGroupId);
	if (accumulatedExpList.Num() == 0)
		return;

	if (currentLevel == accumulatedExpList.Num())
		targetExp = accumulatedExpList.Last();
	else if (accumulatedExpList.IsValidIndex(currentLevel))
		targetExp = accumulatedExpList[currentLevel];
	else
		GSLOG(Error, TEXT("Something went WRONG!!! _currentMapGroupId : %d, currentLevel : %d"), _currentMapGroupId, currentLevel);

	CodexMapGroupExp currentExp = codexManager->GetMapGroupCurrentExpWithId(_currentMapGroupId);
	CodexMapGroupExp prevLevelExp = 0;
	if (accumulatedExpList.IsValidIndex(currentLevel - 1))
		prevLevelExp = accumulatedExpList[currentLevel - 1];

	_textCurrenMapGroupKnowledge->SetText(FText::FromString(FString::FromInt(currentExp - prevLevelExp)));
	_textRequireKnowledgeForNextLevel->SetText(FText::FromString(FString::FromInt(targetExp - prevLevelExp)));

}

void UGsUIMonsterCollectionCodexPage::InvalidateMonsterTileView()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	// 맵 그룹 영역 몬스터 목록 갱신
	_poolPortraitData->ReleaseAll();
	
	TArray<UGsUICodexPortraitEntryData*> monsterList;
	for (const TPair<MapId, const FGsSchemaNpcData*>& mapNpcPair : GsCodex()->GetNpcDataListWithId(_currentMapGroupId))
	{
		UGsUICodexPortraitEntryData* data = _poolPortraitData->Claim<UGsUICodexPortraitEntryData>();
		data->SetData(mapNpcPair.Key, mapNpcPair.Value);
		monsterList.Emplace(data);
	}
	_tileViewMapGroupMonster->SetListItems(monsterList);
	_tileViewMapGroupMonster->RegenerateAllEntries();
	_tileViewMapGroupMonster->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	_tileViewMapGroupMonster->ScrollToTop();

	// 월드맵에서 돌아오는 경우
	if (monsterList.IsValidIndex(_monsterTileIndex))
		_tileViewMapGroupMonster->NavigateToIndex(_monsterTileIndex);
}

void UGsUIMonsterCollectionCodexPage::InvalidateRedDot()
{
	InvalidateRedDotKnowledgeGain();
	InvalidateRedDotCategory();
}

void UGsUIMonsterCollectionCodexPage::InvalidateRedDotKnowledgeGain()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	bool knowledgeRedDot = codexManager->CheckRedDotKnowledge();

	_redDotKnowledgeGainTab->SetRedDot(knowledgeRedDot);
	_redDotKnowledgeConvert->SetRedDot(knowledgeRedDot);
}

void UGsUIMonsterCollectionCodexPage::InvalidateRedDotCategory()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	// 대륙탐사 쪽 레드닷 설정
	TArray<CodexCategoryId> categoryRedDotList = codexManager->GetRedDotCategoryList();
	_redDotCategoryExploreTab->SetRedDot(categoryRedDotList.Contains(_currentCategoryId));
	_redDotCategoryLevelUp->SetRedDot(categoryRedDotList.Contains(_currentCategoryId));

	// 왼쪽 카테고리 레드닷 갱신 (모든 카테고리)
	TArray<UWidget*> categoryWidgetList = _scrollCategoryView->GetAllChildren();
	for (int32 i = 0; i < categoryWidgetList.Num(); ++i)
	{
		UWidget* tabWidget = categoryWidgetList[i];

		if (nullptr == tabWidget)
			return;

		UGsUITabButtonCommon* tabButtonCommon = Cast<UGsUITabButtonCommon>(tabWidget);
		if (nullptr == tabButtonCommon)
			return;

		CodexCategoryId tabCategoryId = codexManager->GetCategoryIdWithIndex(i);

		tabButtonCommon->SetIsRedDot(EGsIconRedDotType::NORMAL, categoryRedDotList.Contains(tabCategoryId));
	}
	//_slotHelperCategoryView->RefreshAll(codexManager->GetTotalCategoryCount());
	//_toggleGroupCategoryView.SetSelectedIndex(_currentTabIndex);
}

void UGsUIMonsterCollectionCodexPage::ClearMapGroupListSelection()
{
	// 기존 모든 녀석들을 선택 해제 및 UI Off 처리한다
	for (UObject* itemObject : _listViewMapGroup->GetListItems())
		ToggleMapGroupClick(itemObject, false);

	_currentMapGroupId = INVALID_CODEX_MAP_GROUP_ID;
	_mapGroupIndex = -1;
	_monsterTileIndex = -1;
}

void UGsUIMonsterCollectionCodexPage::ToggleMapGroupClick(UObject* InObj, bool InSelected)
{
	UGsUICodexMapGroupListItem* item = Cast<UGsUICodexMapGroupListItem>(InObj);
	if (nullptr == item)
		return;

	_currentMapGroupId = item->GetCodexMapGroupId();
	_mapGroupIndex = _listViewMapGroup->GetIndexForItem(InObj);

	item->SetIsSelected(InSelected);

	auto entry = Cast<UGsUICodexMapGroupListEntry>(_listViewMapGroup->GetEntryWidgetFromItem(item));
	if (nullptr == entry)
		return;
	
	entry->SetSelectImage(InSelected);
}

void UGsUIMonsterCollectionCodexPage::InvalidateCategoryBG()
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	FSoftObjectPath pathBG = codexManager->GetCategoryBGWithCategoryId(_currentCategoryId);
	if (!pathBG.IsValid())
		return;

	UTexture2D* textureBG = FGsUIHelper::LoadTextureFromPath(pathBG);
	_imageCategoryBG->SetBrushFromTexture(textureBG);
}

void UGsUIMonsterCollectionCodexPage::OnRefreshEntryCategoryView(int32 InIndex, UWidget* InEntry)
{
	UGsUITabButtonCommon* tabButtonCommon = Cast<UGsUITabButtonCommon>(InEntry);
	if (nullptr == tabButtonCommon)
	{
		GSLOG(Error, TEXT("nullptr == tabButtonCommon, *InEntry->GetName():%s"), *InEntry->GetName());
		return;
	}

	IGsToggleGroupEntry* toggleGroupEntry = tabButtonCommon->GetToggleGroupEntry();
	if (nullptr == toggleGroupEntry)
	{
		GSLOG(Error, TEXT("nullptr == toggleGroupEntry"));
		return;
	}
	else
	{
		toggleGroupEntry->SetIsSelected(false);
		_toggleGroupCategoryView.AddToggleWidget(tabButtonCommon->GetToggleGroupEntry());
	}

	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	CodexCategoryId categoryId = codexManager->GetCategoryIdWithIndex(InIndex);
	tabButtonCommon->SetTitleText(codexManager->GetCategoryNameWithId(categoryId));
	tabButtonCommon->SetIsEnabled(true);

	TArray<CodexCategoryId> categoryRedDotList = codexManager->GetRedDotCategoryList();
	tabButtonCommon->SetIsRedDot(EGsIconRedDotType::NORMAL, categoryRedDotList.Contains(categoryId));
}

void UGsUIMonsterCollectionCodexPage::OnSelectChangedCategoryView(int32 InIndex)
{
	FGsCodexManager* codexManager = GsCodex();
	if (nullptr == codexManager)
		return;

	_currentTabIndex = _toggleGroupCategoryView.GetSelectedIndex();
	_currentCategoryId = codexManager->GetCategoryIdWithIndex(_currentTabIndex);
	_mapGroupIndex = -1;

	InvalidateMapGroupList(EGsMapGroupRefreshType::SCROLL_TO_TOP);
	ClearMapGroupListSelection();

	// 카테고리가 선택되면 오른쪽 UI 영역도 갱신
	InvalidateRightArea(EGsRightArea::KNOWLEDGE_GAIN);

	// 카테고리 변경 시, 대륙탐사 레드닷도 갱신해 줘야 함
	InvalidateRedDotCategory();

	// 카테고리 변경 시, 배경 이미지도 바꿔주어야 함
	InvalidateCategoryBG();
}

void UGsUIMonsterCollectionCodexPage::OnClickedMapGroupListItem(UObject* InListItem)
{
	// 기존 모든 녀석들을 선택 해제 및 UI Off 처리한다
	ClearMapGroupListSelection();

	ToggleMapGroupClick(InListItem, true);

	// 맵 그룹이 선택되면 오른쪽 UI 영역도 갱신
	InvalidateRightArea(EGsRightArea::MONSTER_MAPGROUP);

	FGsUIHelper::Play2DSound(PAGE_SOUND_CLICK);

	if (IsTutorialInteraction())
	{
		// 리스트의 첫 아이템을 클릭한 경우
		if (UObject* targetItem = _listViewMapGroup->GetItemAt(0))
		{
			// 같은 아이템이 클릭되었으면 튜토리얼 통과ㄴ 
			if (InListItem == targetItem)
			{
				OnTutorialInteraction.ExecuteIfBound();
			}
		}
	}
}

void UGsUIMonsterCollectionCodexPage::OnScrolledMapGroupList(float InOffsetInItems, float InDistanceRemaining)
{
	// 튜토리얼 중 스크롤 막음
	if (IsTutorialInteraction())
	{
		_listViewMapGroup->SetScrollOffset(0);
	}
}

void UGsUIMonsterCollectionCodexPage::OnClickedConvertCodexPoint()
{
	FGsNetSendServiceWorld::Send_MonsterBookPointExchange();

	FGsUIHelper::Play2DSound(PAGE_SOUND_CONVERT);
}

void UGsUIMonsterCollectionCodexPage::OnClickedCategoryLevelUp()
{
	FGsNetSendServiceWorld::Send_CodexCategoryLevelUp(_currentCategoryId);

	// 티커 자체에 사운드가 삽입되어 있어, 어색하므로 제거 요청 받음 (by. 김덕규)
	//FGsUIHelper::Play2DSound(PAGE_SOUND_LEVEL_UP);
}

void UGsUIMonsterCollectionCodexPage::OnClickedUseKnowledgeGem()
{
	// 크래시 이슈가 있어, 널체크 및 유효성을 조금 더 엄밀하게 체크
	if (_currentMapGroupId == INVALID_CODEX_MAP_GROUP_ID)
		return;

	if (_mapGroupIndex == -1)
		return;

	UGsUIManager* uiManager = GUI();
	if (nullptr == uiManager)
		return;

	// 지식의 결정 사용 팝업 띄우기
	UGsUIPopupCodexGemUse* popupCodexGemUse = Cast<UGsUIPopupCodexGemUse>(uiManager->OpenAndGetWidget(TEXT("PopupCodexGemUse")));
	if (nullptr == popupCodexGemUse)
		return;

	popupCodexGemUse->SetParameters(
		{
			_currentCategoryId,
			_currentMapGroupId
		}
	);
	
	_isUseKnowledgeGemPopup = true;
	
	popupCodexGemUse->OnClickedCancelDelegate.BindUObject(this, &UGsUIMonsterCollectionCodexPage::OnCloseGemUsePopup);
}

void UGsUIMonsterCollectionCodexPage::OnClickedKnowledgeGainTab()
{
	InvalidateRightArea(EGsRightArea::KNOWLEDGE_GAIN);
}

void UGsUIMonsterCollectionCodexPage::OnClickedCategoryExploreTab()
{
	InvalidateRightArea(EGsRightArea::CATEGORY_EXPLORE);
}

void UGsUIMonsterCollectionCodexPage::OnClickedMonsterTileViewItem(UObject* InItem)
{
	UGsUICodexPortraitEntryData* data = Cast<UGsUICodexPortraitEntryData>(InItem);
	if (data == nullptr)
		return;

	MapId mapId = data->GetMapId();

	const FGsSchemaNpcData* npcData = data->GetData();
	if (npcData == nullptr)
		return;

	_monsterTileIndex = _tileViewMapGroupMonster->GetIndexForItem(InItem);

	FGsUIHelper::Play2DSound(PAGE_SOUND_CLICK);

	// 월드맵 이동함수 (himichani)
	if (GMode())
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (FGsGameStateRegionMap* regionMap =
					contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
				{
					regionMap->ShowMonsterInfoPopupAndMap(mapId, npcData->id);
				}
			}
		}
	}
}

void UGsUIMonsterCollectionCodexPage::OnCloseGemUsePopup()
{
	_isUseKnowledgeGemPopup = false;
}

void UGsUIMonsterCollectionCodexPage::OnClickedCloseMapGroupMonster()
{
	InvalidateRightArea(EGsRightArea::KNOWLEDGE_GAIN);
	ClearMapGroupListSelection();
}

void UGsUIMonsterCollectionCodexPage::OnPressedBtnTooltipTotalKnowledge()
{
	if (nullptr == _tooltipTotalKnowledge)
		return;

	if (_tooltipTotalKnowledge->IsOpen())
		_tooltipTotalKnowledge->Close();

	_tooltipTotalKnowledge->SetDescText(_totalKnowledgeTooltipText);
	_tooltipTotalKnowledge->Open();
}

void UGsUIMonsterCollectionCodexPage::OnPressedBtnTooltipCodexPoint()
{
	if (nullptr == _tooltipCodexPoint)
		return;

	if (_tooltipCodexPoint->IsOpen())
		_tooltipCodexPoint->Close();

	_tooltipCodexPoint->SetDescText(_codexPointTooltipText);
	_tooltipCodexPoint->Open();
}

void UGsUIMonsterCollectionCodexPage::OnPressedBtnTooltipExploreLevel()
{
	if (nullptr == _tooltipExploreLevel)
		return;

	if (_tooltipExploreLevel->IsOpen())
		_tooltipExploreLevel->Close();

	_tooltipExploreLevel->SetDescText(_exploreLevelTooltipText);
	_tooltipExploreLevel->Open();
}

void UGsUIMonsterCollectionCodexPage::OnPressedBtnTooltipMapName()
{
	if (nullptr == _tooltipMapName)
		return;

	if (_tooltipMapName->IsOpen())
		_tooltipMapName->Close();

	_tooltipMapName->SetDescText(_mapNameTooltipText);
	_tooltipMapName->Open();
}

void UGsUIMonsterCollectionCodexPage::OnPressedBtnTooltipMonsterList()
{
	if (nullptr == _tooltipMonsterList)
		return;

	if (_tooltipMonsterList->IsOpen())
		_tooltipMonsterList->Close();

	_tooltipMonsterList->SetDescText(_monsterListTooltipText);
	_tooltipMonsterList->Open();
}

void UGsUIMonsterCollectionCodexPage::SetIsTutorialInteraction(bool bIsTutorial)
{
	_bIsTutorialInteraction = bIsTutorial;

	if (false == _bIsTutorialInteraction)
	{
		OnTutorialInteraction.Unbind();
	}
}

UWidget* UGsUIMonsterCollectionCodexPage::SetTutorial(bool bIsTutorial)
{
	if (bIsTutorial)
	{
		_listViewMapGroup->ScrollToTop();

		// 리스트의 첫 아이템을 리턴
		if (UObject* targetItem = _listViewMapGroup->GetItemAt(0))
		{
			if (UWidget* firstEntry = _listViewMapGroup->GetEntryWidgetFromItem(targetItem))
			{
				return firstEntry;
			}
		}
	}

	return nullptr;
}
