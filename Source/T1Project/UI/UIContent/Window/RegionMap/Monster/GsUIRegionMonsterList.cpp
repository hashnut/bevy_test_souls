#include "GsUIRegionMonsterList.h"

#include "UI/UIContent/Window/RegionMap/Monster/GsUIRegionMonsterListItem.h"
#include "UI/UIContent/Popup/ItemDropNavigator/GsUIItemDropListContainer.h"
#include "UI/UIContent/Popup/GsUIPopupMonsterInfo.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Map/WorldMap/GsContinentalInfo.h"
#include "Map/WorldMap/GsRegionInfo.h"
#include "Map/WorldMap/GsRegionMonsterInfo.h"
#include "Map/WorldMap/GsWorldMapDefine.h"

#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/WorldMap/GsSchemaWorldMapMonsterData.h"
#include "DataSchema/Map/FieldBoss/GsSchemaFieldBossData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcData.h"
#include "DataSchema/GameObject/Npc/GsSchemaNpcBalance.h"

#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedBossEnum.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsUIUtil.h"

#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/ScrollBox.h"

#include "Core/Public/Delegates/Delegate.h"
#include "Components/WidgetSwitcher.h"

void UGsUIRegionMonsterList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIRegionMonsterList::OnRefreshEntry);

	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIRegionMonsterList::OnClickComboboxItem);
}

void UGsUIRegionMonsterList::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	FGsRegionMonsterInfo* monsterInfo = GSWorldMap()->GetCurrentRegionMonsterInfoByIndex(InIndex);
	if (nullptr == monsterInfo)
	{
		return;
	}

	const FGsSchemaNpcData* npcData = monsterInfo->_npcData;
	const FGsSchemaNpcBalance* balanceData = monsterInfo->_npcBalance;
	if (nullptr == npcData ||
		nullptr == balanceData)
	{
		return;
	}

	UGsUIRegionMonsterListItem* item = Cast<UGsUIRegionMonsterListItem>(InEntry);
	if (item == nullptr)
	{
		return;
	}

#if WORLD_MAP_MONSTER_DEBUG
	FString npcName = FString::Printf(TEXT("%s(npc id : %d)"), *npcData->nameText.ToString(), npcData->id);		
	item->SetRegionMonsterListItem(FText::FromString(npcName), monsterInfo->level);
#else
	item->SetRegionMonsterListItem(npcData->nameText, monsterInfo->level);
#endif

	EGsReionMapMonsterSwitchType switchType = EGsReionMapMonsterSwitchType::Normal;
	// 1. check field boss
	if (monsterInfo->_fieldBossData != nullptr)
	{
		// 2. check cooperation or competition
		switchType = (monsterInfo->_fieldBossData->bossType == BossType::COMPETITION ||
			monsterInfo->_fieldBossData->bossType == BossType::COMPETITION_MUTANT) ?
			EGsReionMapMonsterSwitchType::FieldBossCompetition :
			EGsReionMapMonsterSwitchType::Boss;
	}
	else
	{
		// 3. else check balance rank		
		switchType = (npcData->grade == CreatureRankType::BOSS) ?
			EGsReionMapMonsterSwitchType::Boss :
			EGsReionMapMonsterSwitchType::Normal;
	}
	
	item->SetBoss(switchType);

	item->SetLongPressCallback([this](int32 In_index)
		{
			ShowMonsterInfo(In_index);
		});
}

// 아이템 클릭
void UGsUIRegionMonsterList::OnClickComboboxItem(int32 In_index)
{
	if (In_index == -1)
	{
		return;
	}

	OnSelectMonsterList.ExecuteIfBound(In_index);
	
}

void UGsUIRegionMonsterList::BeginDestroy()
{
	if (_scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIRegionMonsterList::OnRefreshEntry);
	}
	_scrollBoxHelper = nullptr;

	_toggleGroup.Clear();
	Super::BeginDestroy();
}

// 리스트 초기화
void UGsUIRegionMonsterList::InitList()
{
	FGsRegionInfo* info = GSWorldMap()->GetCurrentRegionInfo();
	if (nullptr == info)
	{
		_switcher->SetActiveWidgetIndex(1);
		return;
	}

	int32 num = info->_arrayMonsterInfo.Num();
	if (0 == num)
	{
		_switcher->SetActiveWidgetIndex(1);
		return;
	}

	_switcher->SetActiveWidgetIndex(0);
	_scrollBoxHelper->RefreshAll(num);

	// 그룹초기화
	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_verticalBox);

	// backkey open check
	
	bool useListSetting = false;
	bool isBackKeyOpen = GSWorldMap()->GetRegionMapIsBackkeyOpen();
	if (isBackKeyOpen == true)
	{
		useListSetting = true;
	}
	// use first time
	bool isUseMonsterSelectIndex = GSWorldMap()->GetIsUseMonsterSelectIndex();
	if (isUseMonsterSelectIndex == true)
	{
		useListSetting = true;
	}
	int backupMonListIndex = GSWorldMap()->GetBackupSelMonsterListIndex();
	
	// 기본 선택은 -1(선택 안됨)
	// https://jira.com2us.com/jira/browse/CHR-23756
	int index = (useListSetting == true) ? backupMonListIndex : -1;

	SelectList(index);
}



// 최신 데이터로 세팅
void UGsUIRegionMonsterList::InitRegionMonsterListData()
{
	InitList();
}

void UGsUIRegionMonsterList::SelectList(int In_index)
{
	_toggleGroup.SetSelectedIndex(In_index, true);

	float totalSize = UGsUIUtil::GetCaledListSize(_verticalBox, In_index);

	_scrollBox->SetScrollOffset(totalSize);

	// not ready state
	if (In_index != 0 &&
		totalSize == 0.0f)
	{
		_isDirtyScrollOffsetMove = true;
		_moveIndex = In_index;
	}
	
}
void UGsUIRegionMonsterList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	CheckDirtyScrollBoxOffsetMove();
}
void UGsUIRegionMonsterList::CheckDirtyScrollBoxOffsetMove()
{
	if (_isDirtyScrollOffsetMove == false)
	{
		return;
	}

	bool isReady = UGsUIUtil::GetIsReadyVercitalBoxChild(_verticalBox);

	if (isReady == false)
	{
		return;
	}

	float totalSize = UGsUIUtil::GetCaledListSize(_verticalBox, _moveIndex);
	_scrollBox->SetScrollOffset(totalSize);

	_isDirtyScrollOffsetMove = false;
}

// 몬스터 정보 출력
void UGsUIRegionMonsterList::ShowMonsterInfo(int32 In_index)
{
	TWeakObjectPtr<UGsUIWidgetBase> wigdet = GUI()->OpenAndGetWidget(TEXT("PopupMonsterInfo"));

	if (wigdet.IsValid() == false)
	{
		return;
	}
	// 선택된 정보 저장해놓기
	// 선택된 정보 가지고 콤보 박스 세팅함(먼저 저장해놓아야 함)
	GSWorldMap()->SetSelectMonsterInfoByIndex(In_index);

	TWeakObjectPtr<UGsUIPopupMonsterInfo> popupBookmark = Cast<UGsUIPopupMonsterInfo>(wigdet);
	popupBookmark->InitMonsterInfoList(In_index, false);
}