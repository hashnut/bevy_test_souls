#include "GsUIPopupMonsterInfo.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsWorldMapManager.h"

#include "Map/WorldMap/GsRegionMonsterInfo.h"

#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "UTIL/GsGameObjectUtil.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Window/RegionMap/Monster/GsUIRegionMonsterDropItem.h"
#include "UI/UIContent/Window/MonsterKnowledge/GSUIMonsterCollectionPortrait.h"
#include "UI/UIContent/Window/NPCPortrait/GsUIMonsterPortrait.h"

#include "UI/UIContent/Popup/MonsterInfo/GsUIMonsterInfoCombobox.h"

#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/VerticalBox.h"

#include "Components/WidgetSwitcher.h"

void UGsUIPopupMonsterInfo::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupMonsterInfo::OnRefreshEntry);

	_btnOK->OnClicked.AddDynamic(this, &UGsUIPopupMonsterInfo::OnClickOK);
}

// 몬스터 리스트 초기화
void UGsUIPopupMonsterInfo::InitMonsterInfoList(int In_monsterIndex, bool In_isShowLevelCombobx)
{
	_monsterIndex = In_monsterIndex;


	FGsRegionMonsterInfo* monsterInfo =
		GSWorldMap()->GetCurrentRegionMonsterInfoByIndex(_monsterIndex);
	if (nullptr == monsterInfo)
	{
		return;
	}

	TArray<FGsRegionMonsterItemInfo*> arrayItems;

	FGsGameStateRegionMap* regionMap = GSWorldMap()->GetGameStateRegionMap();
	if (regionMap == nullptr)
	{
		return;
	}


	EGsRegionItemInfoSetType nowMapType = regionMap->GetNowMapItemInfoSetType(monsterInfo);
	monsterInfo->GetDropItem(nowMapType, arrayItems);
	
	// 전리품이 0개면 없음 표시를 해준다
	// https://jira.com2us.com/jira/browse/C2URWQ-7461
	if(arrayItems.Num() == 0)
	{ 
		// 1 은 없음
		_switcherListType->SetActiveWidgetIndex(1);
	}
	else
	{ 
		// 0은 있음
		_switcherListType->SetActiveWidgetIndex(0);
		_scrollBoxHelper->RefreshAll(arrayItems.Num());
	}
	_scrollBox->SetScrollOffset(0.0f);

	const FGsSchemaNpcData* npcData = monsterInfo->_npcData;
	const FGsSchemaNpcBalance* balanceData = monsterInfo->_npcBalance;

	if (npcData == nullptr ||
		balanceData == nullptr)
	{
		return;
	}

	// 정보 세팅
	_textMonsterName = npcData->nameText;
	_textMonsterGrade = UGsGameObjectUtil::GetRankText(npcData->grade);
	
	_textMonsterOffensive = UGsGameObjectUtil::GetOffensiveText(monsterInfo->_isOffensive);


	// 종족 텍스트
	_textMonsterCreatureRace = UGsGameObjectUtil::GetRaceText(npcData->race);

	// 약점 재질 텍스트
	_textMonsterWeaknessMaterial = UGsGameObjectUtil::GetRaceWeaknessItemMaterialText(npcData->race);


	// 속성 텍스트
	_textMonsterElement = UGsGameObjectUtil::GetMonsterElementalText(npcData->element);

	// 포트레이트 갱신
	_monsterPortraitBP->SetData(npcData);

	// 콤보 박스 보여줄지
	if (In_isShowLevelCombobx == true)
	{
		_comboboxMonsterInfo->SetVisibility(
			ESlateVisibility::SelfHitTestInvisible);
		_comboboxMonsterInfo->InitData();
	}
	else
	{
		_comboboxMonsterInfo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UGsUIPopupMonsterInfo::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	FGsRegionMonsterInfo* monsterInfo =
		GSWorldMap()->GetCurrentRegionMonsterInfoByIndex(_monsterIndex);
	if (nullptr == monsterInfo)
	{
		return;
	}

	FGsGameStateRegionMap* regionMap = GSWorldMap()->GetGameStateRegionMap();
	if (regionMap == nullptr)
	{
		return;
	}

	EGsRegionItemInfoSetType nowMapType = regionMap->GetNowMapItemInfoSetType(monsterInfo);
	TArray<FGsRegionMonsterItemInfo*> arrayItems;
	monsterInfo->GetDropItem(nowMapType, arrayItems);


	if (arrayItems.Num() <= InIndex)
	{
		return;
	}
	FGsRegionMonsterItemInfo* itemInfo = arrayItems[InIndex];

	if (nullptr == itemInfo)
	{
		return;
	}
	const FGsSchemaItemCommon* itemTbl = itemInfo->_itemTbl;

	if (itemTbl == nullptr)
	{
		return;
	}

	UGsUIRegionMonsterDropItem* item = Cast<UGsUIRegionMonsterDropItem>(InEntry);
	if (item == nullptr)
	{
		return;
	}
	item->SetItemTblId(itemTbl->id, itemTbl->name, itemTbl->grade);
}
void UGsUIPopupMonsterInfo::BeginDestroy()
{
	if (_scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupMonsterInfo::OnRefreshEntry);
	}
	_scrollBoxHelper = nullptr;
	Super::BeginDestroy();
}

void UGsUIPopupMonsterInfo::OnClickOK()
{
	Close();
}

// esc 단축키 닫기 안됨 처리
// https://jira.com2us.com/jira/browse/C2URWQ-7203
bool UGsUIPopupMonsterInfo::OnBack()
{
	Close();
	return true;
}

// 이건 esc 키
// https://jira.com2us.com/jira/browse/C2URWQ-7203
void UGsUIPopupMonsterInfo::OnInputCancel()
{
	Close();
}

// 레벨 변경
void UGsUIPopupMonsterInfo::OnSelectLevelPopupMonsterInfo(const IGsMessageParam* InParam)
{
	const FGsUIMsgParamInt* param = InParam->Cast<const FGsUIMsgParamInt>();
	if (param)
	{
		// 요 이벤트는 레벨 콤보박스에 의해서 온거라 true
		InitMonsterInfoList(param->_data, true);
	}
}

void UGsUIPopupMonsterInfo::NativeConstruct()
{
	Super::NativeConstruct();

	FGsMessageHolder* msg = GMessage();
	MUI& msgUI = msg->GetUI();
	_msgUIHandlerList.Emplace(msgUI.AddUObject(MessageUI::SELECT_LEVEL_POPUP_MONSTER_INFO,
		this, &UGsUIPopupMonsterInfo::OnSelectLevelPopupMonsterInfo));
}

void UGsUIPopupMonsterInfo::NativeDestruct()
{
	if (FGsMessageHolder* message = GMessage())
	{
		for (MsgUIHandle& msgHandler : _msgUIHandlerList)
		{
			message->GetUI().Remove(msgHandler);
		}
	}
	_msgUIHandlerList.Empty();


	Super::NativeDestruct();
}
