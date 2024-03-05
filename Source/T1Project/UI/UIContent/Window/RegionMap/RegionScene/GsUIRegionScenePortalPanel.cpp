#include "GsUIRegionScenePortalPanel.h"
#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Management/GsMessageHolder.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsBookmarkManager.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionWarpListItem.h"
#include "Map/Bookmark/GsBookmarkData.h"
#include "Map/WorldMap/GsRegionInfo.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionScene.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Map/Bookmark/GsBookmarkFunc.h"
#include "Map/WorldMap/GsWorldMapFunc.h"
#include "Components/WidgetSwitcher.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "DataSchema/InterServer/GsSchemaInvadeMapData.h"
#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Bookmark/GsBookmarkHandler.h"
#include "Message/MessageParam/GsUIMessageParam.h"
#include "UI/UILib/Define/GsUIDefine.h"

#ifdef USE_REGION_SCENE_PORTAL_PANNEL

void UGsUIRegionScenePortalPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_scrollBoxHelper = NewObject<UGsDynamicPanelSlotHelper>(this);
	_scrollBoxHelper->Initialize(_entryWidgetClass, _verticalBox);
	_scrollBoxHelper->OnRefreshEntry.AddDynamic(this, &UGsUIRegionScenePortalPanel::OnRefreshEntry);
	//_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIRegionScenePortalPanel::OnClickLeftListItem);
}

void UGsUIRegionScenePortalPanel::NativeConstruct()
{
	InitializeMessage();	
	
	Super::NativeConstruct();
}

void UGsUIRegionScenePortalPanel::NativeDestruct()
{	
	FinalizeMessage();

	Super::NativeDestruct();
}

void UGsUIRegionScenePortalPanel::BeginDestroy()
{
	if (_scrollBoxHelper)
	{
		_scrollBoxHelper->OnRefreshEntry.RemoveDynamic(this, &UGsUIRegionScenePortalPanel::OnRefreshEntry);
	}
	_scrollBoxHelper = nullptr;
	_toggleGroup.Clear();

	Super::BeginDestroy();
}

void UGsUIRegionScenePortalPanel::InitializeMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (msg == nullptr)
	{
		return;
	}

	MGameObject& msgObject = msg->GetGameObject();
}

void UGsUIRegionScenePortalPanel::FinalizeMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (msg == nullptr)
	{
		return;
	}

	//if (_arrGameObjDelegate.Num() != 0)
	//{
	//	for (auto iter : _arrGameObjDelegate)
	//	{
	//		msg->GetGameObject().Remove(iter);
	//	}

	//	_arrGameObjDelegate.Empty();
	//}
}

void UGsUIRegionScenePortalPanel::InitializeWidget()
{
	//포탈 씬에서는 전리품 정보가 필요 없기 때문에 비활성화 	
	GMessage()->GetUI().SendMessage(MessageUI::HIDE_WORLD_MAP_ITEM_DROP, nullptr);

	FGsUIMsgToggleParam toggleParam = FGsUIMsgToggleParam(EGsToggleType::Deactive);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_MONSTER_DROP_ICON_TOGGLE, &toggleParam);	

	FGsUIMsgActiveParam activeParam = FGsUIMsgActiveParam(false);
	GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_MAP_MONSTER_DROP_ACTIVE, &activeParam);

	InitRegionScenePortalPanelData();
}

void UGsUIRegionScenePortalPanel::OnRefreshEntry(int32 InIndex, UWidget* InEntry)
{	
	// 0: 포털 설명
	// 1~포털 사이즈: 포털 아이템
	// 포털 사이즈+ 1: 즐겨찾기 설명
	// 포털 사이즈+2 ~: 즐겨찾기 아이템

	// 아이템이 없으면 중간에 목록이 비어있다는 아이템 추가

	EGsRegionWarpListItemType itemType = EGsRegionWarpListItemType::EmptyDesc;

	// 원래 데이터 인덱스
	int originalDataIndex = 0;
	FindInfoByTotalIndex(InIndex, itemType, originalDataIndex);

	FText textData;
	bool isInTown = false;
	switch (itemType)
	{
	case EGsRegionWarpListItemType::PortalTypeDesc:
	{
		FText::FindText(TEXT("WorldMapText"), TEXT("SmallGroupPortal"), textData);
	}
		break;
	case EGsRegionWarpListItemType::BookmarkTypeDesc:
	{
		FText::FindText(TEXT("WorldMapText"), TEXT("SmallGroupUserPortal"), textData);
	}
		break;
	case EGsRegionWarpListItemType::Portal:
	{
		FGsPortalInfo* portalInfo = GSWorldMap()->GetCurrentRegionPortalInfoByIndex(originalDataIndex);
		if (portalInfo == nullptr)
		{
			return;
		}

		textData = portalInfo->_portalName;
		isInTown = portalInfo->_isInTown;
	}
		break;
	case EGsRegionWarpListItemType::Bookmark:
	{	
		int currentMapId = GSWorldMap()->GetShowRegionMapId();
		TArray<FGsBookmarkData*>* listBookmarkData = GSBookmark()->GetBookmarkDataByMapId(currentMapId);
		if (listBookmarkData == nullptr ||
			listBookmarkData->Num() <= originalDataIndex)
		{
			return;
		}

		FGsBookmarkData* info = (*listBookmarkData)[originalDataIndex];
		if (info == nullptr)
		{
			return;
		}

		textData = FText::FromString(info->GetName());
	}
		break;	
	case EGsRegionWarpListItemType::EmptyDesc:
	{
		FText::FindText(TEXT("WorldMapText"), TEXT("MenuEmptyNotice"), textData);
	}
		break;	
	}
	
	UGsUIRegionWarpListItem* item = Cast<UGsUIRegionWarpListItem>(InEntry);
	if (item == nullptr)
	{
		return;
	}

	item->SetData(itemType, textData, originalDataIndex, isInTown);
}

// 데이터 초기화
void UGsUIRegionScenePortalPanel::InitRegionScenePortalPanelData()
{		
	// 일반적인 경우에는 몬스터 드랍 아이콘 보이지 않도록 설정

	int switchListIndex = 0;
	// 리스트를 만든다(리스트 선택 후 icon 선택 표시 할수 있으므로)
	//침공전인가?
	// 침공전에서도 포털은 표시해달라고 요청(위치 즐겨찾기는 빼고)
	// https://jira.com2us.com/jira/browse/CHR-23653
	/*if (GGameData()->IsInvadeWorld())
	{
		switchListIndex = (int32)EGsRegionPortalType::Invasion;
	}
	else */if (false == FGsWorldMapFunc::IsRegionDungeon())
	{
		InitList();

		int totalCount = FGsBookmarkFunc::GetBookmarkScrollItemCount();

		// 아이템 갯수 새팅
		FString itemCount = FString::FromInt(totalCount);
		_textItemCount = FText::FromString(itemCount);

		switchListIndex = (int32)EGsRegionPortalType::Portal;
	}
	else
	{
		switchListIndex = (int32)EGsRegionPortalType::Dungeon;
	}

	_leftPanelWidgetSwitcher->SetActiveWidgetIndex(switchListIndex);
	// RegionScene 열었을 때나 탭 전환 시 아이콘 두개 딤드 해제 처리
}
// 리스트 초기화
void UGsUIRegionScenePortalPanel::InitList()
{
	// 포털 정보
	FGsRegionInfo* currentRegionInfo = GSWorldMap()->GetCurrentRegionInfo();
	if (currentRegionInfo == nullptr)
	{
		return;
	}


	FGsGameDataManager* dataManager = GGameData();
	if (dataManager == nullptr)
	{
		return;
	}
	bool isInterServer = dataManager->IsInvadeWorld();



	// 현재 포털 사이즈 크기 저장
	_currentPortalListSize = (isInterServer == false)?
		currentRegionInfo->_arrayPortalInfo.Num() :
		currentRegionInfo->_arrayInvasionEnterPortalInfo.Num();


	// 침공전에서도 포털은 표시해달라고 요청(위치 즐겨찾기는 빼고)
	// https://jira.com2us.com/jira/browse/CHR-23653

	// 침공전이 아니면 기존대로
	if(false == isInterServer)
	{ 
		// 즐겨찾기 정보
		int currentMapId = GSWorldMap()->GetShowRegionMapId();
		TArray<FGsBookmarkData*>* listBookmarkData = GSBookmark()->GetBookmarkDataByMapId(currentMapId);
		int bookMarkListSize = 0;
		if (nullptr != listBookmarkData)
		{
			bookMarkListSize = listBookmarkData->Num();
		}

		_currentBookmarkListSize = bookMarkListSize;
		// 1. 타입 설명 2개
		// 2. 리스트 사이즈 or 없으면 없다 설명 1개씩
		int totalListSize = 2 + ((_currentPortalListSize == 0) ? 1 : _currentPortalListSize)
			+ ((_currentBookmarkListSize == 0) ? 1 : _currentBookmarkListSize);	

		_scrollBoxHelper->RefreshAll(totalListSize);

		// 그룹초기화
		_toggleGroup.Clear();
		_toggleGroup.AddToggleWidgetByParentPanel(_verticalBox);


		uint64 bookMarkSelectId = GSWorldMap()->GetBookmarkSelectId();

		int selectIndex = -1;
		if (bookMarkSelectId != 0 &&
			_currentBookmarkListSize != 0 &&
			listBookmarkData != nullptr)
		{		
			for (int i=0; i< _currentBookmarkListSize; ++i)
			{
				FGsBookmarkData* data = (*listBookmarkData)[i];
				if (data == nullptr)
				{
					continue;
				}
				if (data->GetId() == bookMarkSelectId)
				{
					selectIndex = i;
					break;
				}
			}
		}

		int totalIndex = FindTotalIndexByInfo(EGsRegionWarpListItemType::Bookmark , selectIndex);
		// 선택 안됨처리
		_toggleGroup.SetSelectedIndex(totalIndex);
	}
	// 침공전이면 즐겨찾기쪽은 빼고 표시
	else
	{
		// 즐겨찾기 정보
		int bookMarkListSize = 0;	
		_currentBookmarkListSize = bookMarkListSize;
		// 1. 타입 설명 1개
		// 2. 리스트 사이즈 or 없으면 없다 설명 1개씩
		int totalListSize = 1 + ((_currentPortalListSize == 0) ? 1 : _currentPortalListSize);
		_scrollBoxHelper->RefreshAll(totalListSize);

		// 그룹초기화
		_toggleGroup.Clear();
		_toggleGroup.AddToggleWidgetByParentPanel(_verticalBox);


		// 선택 안됨처리
		_toggleGroup.SetSelectedIndex(-1);
	}
}

// click left list item
void UGsUIRegionScenePortalPanel::OnClickLeftListItem(int32 In_index)
{
	EGsRegionWarpListItemType itemType = EGsRegionWarpListItemType::EmptyDesc;
	int dataIndex = 0;
	// -1은 해제
	
	if (In_index == -1)
	{
		dataIndex = In_index;
	}
	else
	{
		FindInfoByTotalIndex(In_index, itemType, dataIndex);
	}
	

	// 설명 타입은 처리 안함
	// -1 은 해제라서 처리
	if ((itemType == EGsRegionWarpListItemType::EmptyDesc ||
		itemType == EGsRegionWarpListItemType::BookmarkTypeDesc ||
		itemType == EGsRegionWarpListItemType::PortalTypeDesc) &&
		dataIndex != -1)
	{
		return;
	}


	uint64 findId = 0;
	bool isFind = false;

	EGsRegionIconSelectType selectType = EGsRegionIconSelectType::AutoMoveTargetPosIcon;
	// 해제는 다해제
	if (dataIndex == -1)
	{
		isFind = false;
		selectType = EGsRegionIconSelectType::AutoMoveTargetPosIcon;
	}
	else if (itemType == EGsRegionWarpListItemType::Bookmark)
	{
		FGsGameFlowGame* gameFlow = GMode()->GetGameFlow();
		if (nullptr == gameFlow)
		{
			return;
		}

		FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame();
		if (nullptr == contents)
		{
			return;
		}

		FGsGameStateHud* hud = contents->GetContentsStateObject<FGsGameStateHud>(FGsContentsMode::InGame::ContentsHud);
		if (nullptr == hud)
		{
			return;
		}

		FGsBookmarkHandler* bookmarkHandler = hud->GetBookmarkHandler();
		if (nullptr == bookmarkHandler)
		{
			return;
		}

		isFind = bookmarkHandler->GetBookmarkIdByRegionMapItem(dataIndex, findId);
		selectType = EGsRegionIconSelectType::BookmarkIcon;

		FGsUISelectPortalParam param = FGsUISelectPortalParam(findId);
		GMessage()->GetUI().SendMessage(MessageUI::SELECT_REGION_SCENE_BOOKMARK, &param);

	}
	else
	{
		FGsPortalInfo* portalInfo = GSWorldMap()->GetCurrentRegionPortalInfoByIndex(dataIndex);
		int64 spotId = 0;
		if (portalInfo == nullptr)
		{
			isFind = false;
		}
		else
		{
			isFind = true;
			spotId = portalInfo->_portalSpotId;
		}
		findId = static_cast<uint64>(portalInfo->_portalSpotId);
		selectType = EGsRegionIconSelectType::PortalIcon;

		FGsUISelectPortalParam param = FGsUISelectPortalParam(dataIndex);
		GMessage()->GetUI().SendMessage(MessageUI::SELECT_REGION_SCENE_PORTAL, &param);
	}		
}

// 통합 인덱스로 type과 data index 찾기
void UGsUIRegionScenePortalPanel::FindInfoByTotalIndex(int In_totalIndex, EGsRegionWarpListItemType& Out_itemType,
	int& Out_dataIndex)
{
	// 0: 포털 설명
	// 1~포털 사이즈: 포털 아이템
	// 포털 사이즈+ 1: 즐겨찾기 설명
	// 포털 사이즈+2 ~: 즐겨찾기 아이템

	// 아이템이 없으면 중간에 목록이 비어있다는 아이템 추가

	EGsRegionWarpListItemType itemType = EGsRegionWarpListItemType::EmptyDesc;

	// 원래 데이터 인덱스
	int originalDataIndex = 0;
	// 포털 설명
	if (In_totalIndex == 0)
	{
		itemType = EGsRegionWarpListItemType::PortalTypeDesc;
	}
	// 포털이 없다면
	else if (_currentPortalListSize == 0)
	{
		// 비어있다는 표시
		if (In_totalIndex == 1)
		{
			itemType = EGsRegionWarpListItemType::EmptyDesc;
		}
		// 즐겨찾기 아이템 설명
		else if (In_totalIndex == 2)
		{
			itemType = EGsRegionWarpListItemType::BookmarkTypeDesc;
		}
		// 즐겨찾기 갯수 0이면
		else if (_currentBookmarkListSize == 0 &&
			In_totalIndex == 3)
		{
			itemType = EGsRegionWarpListItemType::EmptyDesc;
		}
		else
		{
			itemType = EGsRegionWarpListItemType::Bookmark;
			// 설명 2(포털, 즐겨찾기) + 비어있음 1
			originalDataIndex = In_totalIndex - 3;
		}
	}
	else
	{
		// 포털 아이템: 1부터 사이즈 까지
		if (In_totalIndex >= 1 &&
			In_totalIndex <= (_currentPortalListSize))
		{
			itemType = EGsRegionWarpListItemType::Portal;
			// 설명 1(포털)
			originalDataIndex = In_totalIndex - 1;
		}
		// 즐겨 찾기 설명
		else if (In_totalIndex == (_currentPortalListSize + 1))
		{
			itemType = EGsRegionWarpListItemType::BookmarkTypeDesc;
		}
		// 즐겨 찾기 갯수 0이면
		else if (_currentBookmarkListSize == 0 &&
			In_totalIndex == _currentPortalListSize + 2)
		{
			itemType = EGsRegionWarpListItemType::EmptyDesc;
		}
		else
		{
			itemType = EGsRegionWarpListItemType::Bookmark;
			// 설명 2(포털, 즐겨찾기) + 포털 사이즈
			originalDataIndex = In_totalIndex - (2 + _currentPortalListSize);
		}
	}

	Out_dataIndex = originalDataIndex;
	Out_itemType = itemType;
}

// 인포로 통합 인덱스 구하기
int UGsUIRegionScenePortalPanel::FindTotalIndexByInfo(EGsRegionWarpListItemType In_itemType,
	int In_dataIndex)
{
	// -1 은 선택 안한다는 뜻
	if (In_dataIndex == -1)
	{
		return -1;
	}

	// 정보가 없음
	if (In_itemType == EGsRegionWarpListItemType::Portal &&
		_currentPortalListSize == 0)
	{
		return -1;
	}

	if (In_itemType == EGsRegionWarpListItemType::Bookmark &&
		_currentBookmarkListSize == 0)
	{
		return -1;
	}

	int totalIndex = 0;
	// 포탈 인덱스 구하기
	if (In_itemType == EGsRegionWarpListItemType::Portal)
	{
		// 아이템 인덱스 + 설명 item 인덱스 추가
		totalIndex += In_dataIndex + 1;
		return totalIndex;
	}

	// 나머진 즐겨찾기
	if (_currentPortalListSize != 0)
	{
		// 리스트 사이즈 + 설명 item 인덱스 추가
		totalIndex += _currentPortalListSize + 1;
	}
	else
	{
		// 설명 + 빈칸
		totalIndex += 2;
	}

	if (_currentBookmarkListSize != 0)
	{
		// 리스트 사이즈 + 설명 item 인덱스 추가
		totalIndex += In_dataIndex + 1;
	}
	
	return totalIndex;
}


#endif