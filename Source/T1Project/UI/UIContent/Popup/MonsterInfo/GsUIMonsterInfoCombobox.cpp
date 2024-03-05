#include "GsUIMonsterInfoCombobox.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGame/GsWorldMapManager.h"

#include "Message/GsMessageUI.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "Map/WorldMap/GsRegionMonsterInfo.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Popup/MonsterInfo/GsUIMonsterInfoComboboxItem.h"

#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"

void UGsUIMonsterInfoCombobox::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIMonsterInfoCombobox::OnRefreshEntry);

	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIMonsterInfoCombobox::OnClickComboboxItem);
	_toggleGroup.OnNotChangeEvent.BindUObject(this, &UGsUIMonsterInfoCombobox::OnSelectNotChange);

	_btnShowList->OnClicked.AddDynamic(this, &UGsUIMonsterInfoCombobox::OnClickShowList);

}

void UGsUIMonsterInfoCombobox::NativeConstruct()
{	
	Super::NativeConstruct();	
	// Lv. {0}
	FText::FindText(TEXT("WorldMapText"), TEXT("MonLevel"), _textLevelFormat);

	InitData();

	SetVisibleList(false);
}
void UGsUIMonsterInfoCombobox::NativeDestruct()
{
	SetVisibleList(false);
	Super::NativeDestruct();
}

void UGsUIMonsterInfoCombobox::BeginDestroy()
{
	if (_scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIMonsterInfoCombobox::OnRefreshEntry);
	}
	_scrollBoxHelper = nullptr;

	_toggleGroup.Clear();
	Super::BeginDestroy();
}

void UGsUIMonsterInfoCombobox::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	
	int level = GSWorldMap()->GetCurrentSelectMonsterLevelByLevelIndex(InIndex);
	int currentLevel = GSWorldMap()->GetSelectInfoPopupMonsterLevel();

	bool isSameLevelItem = (level == currentLevel) ? true : false;

	UGsUIMonsterInfoComboboxItem* item = Cast<UGsUIMonsterInfoComboboxItem>(InEntry);
	if (item == nullptr)
	{
		return;
	}

	FText resultText = FText::Format(_textLevelFormat, level);
	
	item->SetText(resultText);
	item->SetCurrentMonsgerLevelItem(isSameLevelItem);	
}

// 아이템 클릭
void UGsUIMonsterInfoCombobox::OnClickComboboxItem(int32 In_index)
{
	if (-1 == In_index)
	{
		return;
	}

	// 현재 선택된 레벨과 같은건가
	// 현재 보여지는 몬스터 정보 레벨
	int currentLevel = GSWorldMap()->GetSelectInfoPopupMonsterLevel();
	// 콤보 박스에서 선택한 레벨
	int selectLevel = GSWorldMap()->GetCurrentSelectMonsterLevelByLevelIndex(In_index);

	// 같은 레벨이면 그냥 return
	if (currentLevel == selectLevel)
	{
		SetVisibleList(false);
		return;
	}

	// 현재 레벨 저장
	GSWorldMap()->SetSelectInfoPopupMonsterLevel(selectLevel);

	int monsterIndex = GSWorldMap()->GetCurrentSelectMonsterIndexByLevelIndex(In_index);
	// 찾은 데이터 없으면 return
	if (monsterIndex == -1)
	{
		SetVisibleList(false);
		return;
	}



	FGsUIMsgParamInt param(monsterIndex);
	GMessage()->GetUI().SendMessage(MessageUI::SELECT_LEVEL_POPUP_MONSTER_INFO, &param);


	SetVisibleList(false);
}

void UGsUIMonsterInfoCombobox::OnSelectNotChange()
{
	SetVisibleList(false);
}

// 리스트 보여주기 클릭
void UGsUIMonsterInfoCombobox::OnClickShowList()
{
	ESlateVisibility currentVisibility = _verticalBox->GetVisibility();

	bool newIsShow = (currentVisibility == ESlateVisibility::Hidden) ?
		true : false;

	SetVisibleList(newIsShow);
}

// 최신 데이터로 세팅
void UGsUIMonsterInfoCombobox::InitData()
{
	InitList();
	
	// 현재 보여지는 몬스터 정보 레벨
	int currentLevel = GSWorldMap()->GetSelectInfoPopupMonsterLevel();

	_textCurrenLevel = FText::Format(_textLevelFormat, currentLevel);
}
// 리스트 초기화
void UGsUIMonsterInfoCombobox::InitList()
{
	FGsRegionMonsterSameIDInfo* findInfo = GSWorldMap()->GetCurrentMonsterSameIdInfo();
	if (findInfo == nullptr)
	{
		return;
	}

	_scrollBoxHelper->RefreshAll(findInfo->_arrayLevel.Num());

	// 그룹초기화
	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_verticalBox);

	// 선택 안됨처리
	_toggleGroup.SetSelectedIndex(-1);
}

// 리스트 보이게 할지
void UGsUIMonsterInfoCombobox::SetVisibleList(bool In_show)
{
	ESlateVisibility newVisibility = (In_show == true) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;

	_verticalBox->SetVisibility(newVisibility);

	// 화살표 처리
	int switchIndex = (In_show == false)? 0: 1;

	_switcherListArrow->SetActiveWidgetIndex(switchIndex);
}