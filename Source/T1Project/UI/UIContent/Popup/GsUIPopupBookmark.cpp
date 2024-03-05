#include "GsUIPopupBookmark.h"

#include "Map/Bookmark/GsBookmarkData.h"
#include "Map/Bookmark/GsBookmarkDefine.h"
#include "Map/Bookmark/GsBookmarkFunc.h"

#include "DataSchema/Map/GsSchemaMapData.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"

#include "Management/ScopeGame/GsBookmarkManager.h"
#include "Management/ScopeGame/GsUnlockManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"

#include "Map/WorldMap/GsWorldMapFunc.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "GameObject/Fence/GsFenceHandler.h"

#include "Item/GsItemManager.h"
#include "Item/GsItem.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIContent/Popup/Bookmark/GsUIBookmarkItem.h"
#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Popup/GsUIPopupBookmarkNameChange.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsUIUtil.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/EditableTextBox.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/CanvasPanel.h"

#include "Core/Public/Delegates/Delegate.h"

void UGsUIPopupBookmark::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIPopupBookmark::OnRefreshEntry);

	_btnSort->OnClicked.AddDynamic(this, &UGsUIPopupBookmark::OnClickSort);
	_btnDel->OnClicked.AddDynamic(this, &UGsUIPopupBookmark::OnClickDel);
	_btnUpdate->OnClicked.AddDynamic(this, &UGsUIPopupBookmark::OnClickUpdate);
	_btnMoveToRegionMap->OnClicked.AddDynamic(this, &UGsUIPopupBookmark::OnClickMoveToRegionMap);

	_btnClose->OnClicked.AddDynamic(this, &UGsUIPopupBookmark::OnClickClose);
	_btnCreate->OnClicked.AddDynamic(this, &UGsUIPopupBookmark::OnClickCreate);

	_btnNameSort->OnClicked.AddDynamic(this, &UGsUIPopupBookmark::OnClickNameSort);
	_btnTimeSort->OnClicked.AddDynamic(this, &UGsUIPopupBookmark::OnClickTimeSort);

	// 리스트 클릭관리
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIPopupBookmark::OnClickListItem);

	_editCreateName->OnTextChanged.AddDynamic(this, &UGsUIPopupBookmark::OnTextChanged);
}

void UGsUIPopupBookmark::NativeConstruct()
{
	Super::NativeConstruct();

	// ui 열릴때 정렬해야될께 있는지 체크후 정렬 요청
	GSBookmark()->OnPreOpenUI();
	// 리스트 초기화
	InitData();

	FGsMessageHolder* msgM = GMessage();
	
	_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
		MessageGameObject::BOOKMARK_ALL_LIST_CHANGE, this, &UGsUIPopupBookmark::OnAllListChange)
	);
	_arrDelegateGameObjectMsg.Emplace(msgM->GetGameObject().AddUObject(
		MessageGameObject::BOOKMARK_SEL_LIST_CHANGE, this, &UGsUIPopupBookmark::OnSelListChange)
	);
	
	_shareMenuItemIndex = -1;
}
void UGsUIPopupBookmark::NativeDestruct()
{
	if(FGsMessageHolder* msgM = GMessage())
	{ 
		if (_arrDelegateGameObjectMsg.Num() != 0)
		{
			for (auto iter : _arrDelegateGameObjectMsg)
			{
				msgM->GetGameObject().Remove(iter);
			}
			_arrDelegateGameObjectMsg.Empty();
		}
	}
	
	Super::NativeDestruct();
}

// 데이터 초기화
void UGsUIPopupBookmark::InitData()
{
	InitList();

	// 현재 맵이름 생성 텍스트에 밀어 넣기
	int mapId = GLevel()->GetCurrentLevelId();
	const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(mapId);
	if (mapData == nullptr)
	{
		return;
	}

	_editCreateName->SetText(mapData->MapStringId);

	SetVisibleSortPanel(false);

	int totalCount = FGsBookmarkFunc::GetBookmarkScrollItemCount();

	// 아이템 갯수 새팅
	FString itemCount = FString::FromInt(totalCount);
	_textItemCount = FText::FromString(itemCount);
}
// 리스트 초기화
void UGsUIPopupBookmark::InitList()
{
	TArray<FGsBookmarkData*> arrInfo = GSBookmark()->GetBookmarkData();
	if (nullptr == _scrollBoxHelper)
	{
		return;
	}
	_scrollBoxHelper->RefreshAll(arrInfo.Num());

	// 그룹 초기화
	_toggleGroup.Clear();
	_toggleGroup.AddToggleWidgetByParentPanel(_verticalBox);

	// 선택 안됨처리
	_toggleGroup.SetSelectedIndex(-1);

	if (_switcherBtnEnable == nullptr)
	{
		return;
	}

	int index = (arrInfo.Num() == 0)?0: 1;
	_switcherBtnEnable->SetActiveWidgetIndex(index);

	// 리스트 갯수 세팅
	int currentListCount = arrInfo.Num();
	int maxCount = GData()->GetGlobalData()->_bookmarkListItemMaxCount;

	FString listCount = FString::Printf(TEXT("%i/%i"), currentListCount, maxCount);
	_textListCount = FText::FromString(listCount);

	bool visibleNoListWidget = (currentListCount == 0) ? true : false;
	SetVisibleNoListWidget(visibleNoListWidget);
}

void UGsUIPopupBookmark::BeginDestroy()
{
	if (_scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIPopupBookmark::OnRefreshEntry);
	}
	_scrollBoxHelper = nullptr;

	_toggleGroup.Clear();
	Super::BeginDestroy();
}

void UGsUIPopupBookmark::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{
	TArray<FGsBookmarkData*> arrInfo = GSBookmark()->GetBookmarkData();

	if (arrInfo.Num() <= InIndex)
	{
		return;
	}

	FGsBookmarkData* info = arrInfo[InIndex];

	UGsUIBookmarkItem* item =  Cast<UGsUIBookmarkItem>(InEntry);

	if (item == nullptr)
	{
		return;
	}

	bool isShowShareMenu = (_shareMenuItemIndex == InIndex) ? true : false;

	// 데이터 세팅
	item->SetData(
		isShowShareMenu,
		InIndex,
		info->GetId(), info->GetName());
}

// 정렬 클릭
void UGsUIPopupBookmark::OnClickSort()
{
	// 스위치 상태 갱신

	if (_switcherNameSortState == nullptr ||
		_switcherTimeSortState == nullptr ||
		_panelSort == nullptr)
	{
		return;
	}
	// 이미 켜져있으면 끈다
	if (_panelSort->GetVisibility() != ESlateVisibility::Hidden)
	{
		SetVisibleSortPanel(false);
		return;
	}

	EGsBookmarkSortType currSortType = GSBookmark()->GetCurrSortType();

	int nameSortEnable = (currSortType == EGsBookmarkSortType::Name) ? 1 : 0;
	int timeSortEnable = (currSortType == EGsBookmarkSortType::EnrollTime) ? 1 : 0;

	_switcherNameSortState->SetActiveWidgetIndex(nameSortEnable);
	_switcherTimeSortState->SetActiveWidgetIndex(timeSortEnable);

	SetVisibleSortPanel(true);
}
// 삭제 클릭
void UGsUIPopupBookmark::OnClickDel()
{
	int selectIndex = _toggleGroup.GetSelectedIndex();
	// 선택된게 없다면 return
	if (selectIndex == -1)
	{
		// 노티 메시지 띄우기
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorNoSelect"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}
	// 삭제 클릭
	GSBookmark()->SendBookmarkDelete(selectIndex);
}
// 수정 클릭
void UGsUIPopupBookmark::OnClickUpdate()
{
	int selectIndex = _toggleGroup.GetSelectedIndex();
	// 선택된게 없다면
	if (selectIndex == -1)
	{
		// 노티 메시지 띄우기
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorNoSelect"), findText);
		FGsUIHelper::TrayMessageTicker(findText);		
		return;
	}

	TArray<FGsBookmarkData*> arrInfo = GSBookmark()->GetBookmarkData();

	if (arrInfo.Num() <= selectIndex)
	{
		return;
	}

	FGsBookmarkData* info = arrInfo[selectIndex];
	if (nullptr == info)
	{
		return;
	}
	
	// 이름변경 팝업 띄우기
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupBookmarkNameChange"));
	if (widget.IsValid() == false)
	{
		return;
	}
	UGsUIPopupBookmarkNameChange* popupNameChange = Cast<UGsUIPopupBookmarkNameChange>(widget.Get());
	if (popupNameChange == nullptr)
	{
		return;
	}
	popupNameChange->SetData(selectIndex, info->GetName());
}

// 지역맵 이동 클릭
void UGsUIPopupBookmark::OnClickMoveToRegionMap()
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

	int selectIndex = _toggleGroup.GetSelectedIndex();
	// 선택된게 없다면
	if (selectIndex == -1)
	{
		// 노티 메시지 띄우기
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorNoSelect"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}
	TArray<FGsBookmarkData*> arrInfo = GSBookmark()->GetBookmarkData();

	if (arrInfo.Num() <= selectIndex)
	{
		return;
	}

	FGsBookmarkData* info = arrInfo[selectIndex];
	if (nullptr == info)
	{
		return;
	}
	//
	// 현재 내위치 지역맵으로 이동(level 데이터 접근)
	if (GMode())
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (FGsGameStateRegionMap* regionMap =
					contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
				{
					regionMap->SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_BOOKMARK, info->GetMapId(),
						info->GetId());
				}
			}
		}
	}

	// 지역맵으로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);


	Close();
}

// 닫기 클릭
void UGsUIPopupBookmark::OnClickClose()
{
	Close();
}
// 생성 클릭
void UGsUIPopupBookmark::OnClickCreate()
{
	// 개인 이벤트 채널에서의 즐겨찾기 등록을 막는다
	// 던전이어도 등록을 막는다
	// 싱글맵이어도 막는다
	// 대결장 안에서도 막는다
	if (GLevel()->IsInstanceSingleMap()||
		GLevel()->IsDungeonMap() ||
		GLevel()->IsMapType(MapType::SINGLE_FIELD) ||
		GFence()->IsEnteredTerritoryType(TerritoryType::ARENA))
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorCreateInvalidArea"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	TArray<FGsBookmarkData*> arrInfo = GSBookmark()->GetBookmarkData();
	int maxCount =
		GData()->GetGlobalData()->_bookmarkListItemMaxCount;

	if (arrInfo.Num() >= maxCount)
	{
		FText findText;
		FText::FindText(TEXT("UserTeleportText"), TEXT("ErrorCreateMaxValue"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}


	if (_editCreateName == nullptr)
	{
		return;
	}
	FString sendText = _editCreateName->GetText().ToString();
	if (false == FGsBookmarkFunc::CheckBookmarkNameLength(sendText))
	{
		return;
	}

	GSBookmark()->SendBookmarkCreate(sendText);
}

// 인덱스 클릭
void UGsUIPopupBookmark::OnClickListItem(int32 In_index)
{
	// 기존 인덱스가 -1도 아니고
	// 기존 인덱스와 다른 인덱스가 선택된거면
	if (_shareMenuItemIndex != -1 &&
		_shareMenuItemIndex != In_index)
	{
		// 공유 메뉴 hide
		SetShareMenuItemIndex(-1);
	}
}

// 모든 리스트 변경
void UGsUIPopupBookmark::OnAllListChange(const IGsMessageParam*)
{
	InitList();
}
// 선택 리스트 변경
void UGsUIPopupBookmark::OnSelListChange(const IGsMessageParam*)
{
	int selectIndex = _toggleGroup.GetSelectedIndex();

	if (nullptr == _scrollBoxHelper ||
		selectIndex == -1)
	{
		return;
	}
	_scrollBoxHelper->RefreshByIndex(selectIndex);
}

// 이름 정렬 클릭
void UGsUIPopupBookmark::OnClickNameSort()
{
	GSBookmark()->OnSortChange(EGsBookmarkSortType::Name);
	InitList();

	// 정렬 타입 버튼 닫기
	SetVisibleSortPanel(false);
}
// 시간 정렬 클릭
void UGsUIPopupBookmark::OnClickTimeSort()
{
	GSBookmark()->OnSortChange(EGsBookmarkSortType::EnrollTime);
	InitList();
	// 정렬 타입 버튼 닫기

	SetVisibleSortPanel(false);
}

// 정렬 패널 visible 처리
void UGsUIPopupBookmark::SetVisibleSortPanel(bool In_isVisible)
{
	// 정렬 리스트 보여주기 처리
	if (_panelSort == nullptr)
	{
		return;
	}
	ESlateVisibility visible = (In_isVisible == true) ?
		ESlateVisibility::Visible : ESlateVisibility::Hidden;
	// 보여주게 처리
	_panelSort->SetVisibility(visible);
}

// 리스트 없음 표시
void UGsUIPopupBookmark::SetVisibleNoListWidget(bool In_isVisible)
{
	ESlateVisibility visible = (In_isVisible == true) ?
		ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;

	_widgetNoListItem->SetVisibility(visible);
}
// 공유 메뉴 아이템 인덱스 세팅
void UGsUIPopupBookmark::SetShareMenuItemIndex(int In_index)
{
	int oldIndex = _shareMenuItemIndex;
	_shareMenuItemIndex = In_index;

	// 이전 index 가 -1이 아니면 갱신
	if (oldIndex != -1)
	{
		_scrollBoxHelper->RefreshByIndex(oldIndex);
	}
	// 새index가 -1이 아니면 갱신
	if (_shareMenuItemIndex != -1)
	{
		_scrollBoxHelper->RefreshByIndex(_shareMenuItemIndex);
	}
	
}

void UGsUIPopupBookmark::OnTextChanged(const FText& InText)
{
	int textMaxLen = GData()->GetGlobalData()->_bookmarkNameLenMax;
	UGsUIUtil::CheckEditableTextNameLength(_editCreateName, textMaxLen, false);
}

void UGsUIPopupBookmark::OnInputCancel()
{
	OnClickClose();
}