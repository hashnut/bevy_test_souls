#include "GsUIRegionMap.h"

#include "UI/UIContent/Window/RegionMap/GsUIRegionTab.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionScene.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionMonster.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionCombobox.h"

#include "Message/GSMessageContents.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"

#include "Map/WorldMap/GsWorldMapFunc.h"
#include "Map/WorldMap/GsRegionInfo.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGameClientVersion.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"

#include "Net/GsNetSendServiceWorld.h"
#include "Net/GsNetSendService.h"

#include "UI/UIContent/Popup/ItemDropNavigator/GsUIItemDropListContainer.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

void UGsUIRegionMap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_toggleGroup.AddToggleWidgetByParentPanel(_panelTab);
	_toggleGroup.OnLeavePreSelected.BindUObject(this, &UGsUIRegionMap::OnLeavePreSelected);
	_toggleGroup.OnSelectChanged.BindUObject(this, &UGsUIRegionMap::OnClickTab);

	FText findTextSpoils;
	FText::FindText(TEXT("WorldMapText"), TEXT("BigGroupItem"), findTextSpoils);
	_tabSpoils->SetText(findTextSpoils);

	FText findTextMonster;
	FText::FindText(TEXT("WorldMapText"), TEXT("BigGroupMonster"), findTextMonster);
	_tabMonster->SetText(findTextMonster);

	FText findTextSanctum;
	FText::FindText(TEXT("SanctumUIText"), TEXT("Sanctum_Title"), findTextSanctum);
	_tabSanctum->SetText(findTextSanctum);

	InitializeMessage();
}

void UGsUIRegionMap::InitializeMessage()
{
	UGsScopeHolder* scopeHolder = GScope();
	if (nullptr == scopeHolder)
	{
		return;
	}

	FGsMessageHolder* msg = scopeHolder->GetMessageHolder();
	if (nullptr == msg)
	{
		return;
	}

	MGameObject& msgObject = msg->GetGameObject();

	_msgGameObjectHandleList.Emplace(msgObject.AddUObject(
		MessageGameObject::UI_CLICK_REGION_MAP_TYPE_TAB, this, &UGsUIRegionMap::OnClickRegionMapTypeTab
	));
}

// 탭 클릭
void UGsUIRegionMap::OnClickTab(int32 In_index)
{
	EGsRegionMapType mapType = (EGsRegionMapType)In_index;
	SelectPanelType(mapType);
	// 기존에는 데이터 세팅을 전체 다 했는데
	// 월드맵개선에서 성소 아이콘 클릭시 탭변경이 일아나면서
	// 선택한 정보가 클리어 됨
	// 갱신할 정보만 갱신하자
	//SetRegionMapTypeData(mapType);

	_regionScene->SetMapTypeTabChanged(mapType);


	// other ui back key enther region map(click monster tab backup)
	GSWorldMap()->SetShowRegionMapType(mapType);

	// 성소 탭 변경 클릭시
	// 1. 현재 성소가 선택되어있지 않고
	// 2. 현재맵에 성소가 하나라도 있으면
	// 3. 강제로 하나 선택하게 처리
	if(mapType == EGsRegionMapType::Sanctum)
	{
		// 아무 성소아이콘 선택되게 처리(탭변경시 처리)
		_regionScene->OnForcedSelectSanctum();
	}
}

void UGsUIRegionMap::OnLeavePreSelected(int32 In_index)
{
	if (In_index < 0)
	{
		return;
	}


	// 기존에는 획득터 탭에서 다른탭 넘어갈때만 정보 클리어했는데
	// 몬스터 탭에서도 정보 사용해서 그냥 모든탭 넘어갈때 다 클리어하자
	// https://jira.com2us.com/jira/browse/CHR-23756
	FGsGameStateRegionMap* gameStateRegionMap = UGsUIItemDropListContainer::GetGameStateRegionMap();
	if (gameStateRegionMap)
	{
		gameStateRegionMap->ClearSpoilsMonster();
	}
}

void UGsUIRegionMap::OnClickRegionMapTypeTab(const IGsMessageParam* InParam)
{
	const FGsUIMsgRegionMapTypeParam* paramTarget = InParam->Cast<const FGsUIMsgRegionMapTypeParam>();

	int switchIndex = TAB_SPOILS;
	bool isToggleForce = false;
	switch (paramTarget->_regionMapType)
	{
	case EGsRegionMapType::Spoils:
	{
		switchIndex = TAB_SPOILS;
	}
	break;
	case EGsRegionMapType::Monster:
	{
		switchIndex = TAB_MONSTER_LIST;
	}
	break;
	case EGsRegionMapType::Sanctum:
	{
		switchIndex = TAB_SANCTUM;
		isToggleForce = true;
	}
	break;
	}

	// 성소 탭 페이지 쪽 클릭시 토글을 강제로 처리안하면
	// 갱신이 안됭서 강제로 토글 처리하게하니까
	// 기존 획득처 쪽 메시지에서 재귀호출이 타는거 같다...
	// 토글 메시지 강제로 쏘는거는 특정 상황에서만 쓰자...

	_toggleGroup.SetSelectedIndex(switchIndex, isToggleForce);

}

void UGsUIRegionMap::FinalizeMessage()
{
	UGsScopeHolder* scopeHolder = GScope();
	if (nullptr == scopeHolder)
	{
		return;
	}

	FGsMessageHolder* msg = scopeHolder->GetMessageHolder();
	if (nullptr == msg)
	{
		return;
	}

	if (_msgGameObjectHandleList.Num() != 0)
	{
		for (auto iter : _msgGameObjectHandleList)
		{
			msg->GetGameObject().Remove(iter);
		}

		_msgGameObjectHandleList.Empty();
	}
}

void UGsUIRegionMap::BeginDestroy()
{
	_toggleGroup.Clear();
	Super::BeginDestroy();

	FinalizeMessage();
}

void UGsUIRegionMap::NativeConstruct()
{
	Super::NativeConstruct();

	GSWorldMap()->SetIsUIRegionMapConstruct(true);

	// 포털탭이 우선(0번 탭)
	// 몬스터가 먼저일수 있어서
	// 월드맵에서 가져온다

	// 월드맵 개선에서
	// 성소 선택시 탭변경이 일어나면서
	// 기존에서는 탭변경시 데이터 세팅이 일어났는데
	// 기존대로 하면 선택된 정보가 날라가는 현상이 나옴
	// 이젠 실제 세팅은 construct에서 한번 일어나고(지도 세팅, )
	// 그외에는 필요에 의해서 세팅해야함

	// 탭 변경에 대한 정보만 갱신
	EGsRegionMapType showRegionMapType = GSWorldMap()->GetShowRegionMapType();
	// 강제로 갱신 안보낸다
	// toggle true로 하면 이벤트 세팅되어서
	// 보스 맵에서 지도 올때 toggle로 한번 세팅된후(OnClickTab 호출됨)
	// 밑에서 SetRegionMapTypeData로 세팅되어서
	// 몬스터 리스트 선택된처리가 지워짐
	_toggleGroup.SetSelectedIndex((int)showRegionMapType);	

	// 실제 지도에 대한 정보 갱신(이전에는 토글그룹안에서 처리하던거 직접 호출)
	SetRegionMapTypeData(showRegionMapType);


	GSWorldMap()->SetIsUIRegionMapConstruct(false);

	if (GGameData()->IsInvadeWorld())
	{
		_combobox->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_combobox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}	
}

void UGsUIRegionMap::NativeDestruct()
{
	Super::NativeDestruct();
}

// 지역맵 변경(현재 탭껄로 갱신)
void UGsUIRegionMap::ChangeRegionMap(bool In_byDropItem)
{
	int activeIndex = _toggleGroup.GetSelectedIndex();
	SetRegionMapTypeData((EGsRegionMapType)activeIndex, In_byDropItem);

	if (GGameData()->IsInvadeWorld())
	{
		_combobox->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		_combobox->SetVisibility(
			ESlateVisibility::SelfHitTestInvisible);
		_combobox->InitData();
	}
}

// 지역맵 타입으로 변경(획득처, 몬스터)
void UGsUIRegionMap::ChangeRegionMapByType(EGsRegionMapType In_type)
{
	switch (In_type)
	{
	case EGsRegionMapType::Spoils:		
	{
		_toggleGroup.SetSelectedIndex(TAB_SPOILS, true);
	}
		break;
	case EGsRegionMapType::Monster:
	{
		_toggleGroup.SetSelectedIndex(TAB_MONSTER_LIST, true);
	}
		break;	
	case EGsRegionMapType::Sanctum:
	{
		_toggleGroup.SetSelectedIndex(TAB_SANCTUM, true);
	}
	break;
	}	

	_combobox->SetVisibility(
		ESlateVisibility::SelfHitTestInvisible);
	_combobox->InitData();	
}

// 타입 데이터 세팅
void UGsUIRegionMap::SetRegionMapTypeData(EGsRegionMapType In_type, bool In_byDropItem)
{
	// 이제 monster bp는 사용하지 않고 Scene만(지도) 사용
	// https://jira.com2us.com/jira/browse/CHR-23756
	_regionScene->SetSceneData(In_type, In_byDropItem);	

	// 성소 아이콘이 있는 지역이면 정보 요청 하기
	SendSanctumInfoReq();


	// 컨텐츠 타입에따라 보여줌 처리
	// 0: 보여줌
	// 1: 해당 컨텐츠는 안보여줌
	// https://jira.com2us.com/jira/browse/C2URWQ-7853

	// 그림자 전장에서는 왼쪽 리스트 안보여줌
	MapType mapType = GSWorldMap()->GetShowRegionMapTableType();

	int switchContentsType = 0;
	if(mapType == MapType::GUILD_WAR)
	{
		switchContentsType = 1;
	}

	_switcherContentsType->SetActiveWidgetIndex(switchContentsType);
}

void UGsUIRegionMap::SelectPanelType(EGsRegionMapType inType)
{
	// 몬스터 스위치 삭제 -> 팝업으로 대체(월드맵 개선)
	// https://jira.com2us.com/jira/browse/CHR-23756

	// 0번은 씬정보(지도 정보)
	_switcherRegionType->SetActiveWidgetIndex(0);
}
// 성소 정보 요청
void UGsUIRegionMap::SendSanctumInfoReq()
{
	FGsWorldMapManager* worldmapManager = GSWorldMap();
	if (worldmapManager == nullptr)
	{
		return;
	}

	FGsRegionInfo* regionInfo =
		worldmapManager->GetCurrentRegionInfo();

	if (regionInfo == nullptr)
	{
		return;
	}

	// 침공전 넘어가서도 포털만 표시 요청
	// https://jira.com2us.com/jira/browse/CHR-23653
	FGsGameDataManager* dataManager = GGameData();
	if (dataManager == nullptr)
	{
		return;
	}
	bool isInvadeWorld = dataManager->IsInvadeWorld();

	// 성소 정보 요청하기
	if (regionInfo->_arrayRegionMapSanctumInfo.Num() != 0 && isInvadeWorld == false)
	{
		int regionMapId = GSWorldMap()->GetShowRegionMapId();
		//FGsNetSendServiceWorld::SendReqSanctumMapInfo(regionMapId);
		FGsNetSendService::SendReqMapSanctumInfo(regionMapId);
	}
}