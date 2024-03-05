#include "GsUIWindowRegionMap.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "DataSchema/Map/WorldMap/GsSchemaContinentalMapData.h"
#include "DataSchema/Map/GsSchemaMapData.h"

#include "Map/WorldMap/GsWorldMapFunc.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Window/RegionMap/GsUIRegionMap.h"

#include "UI/UIContent/Popup/ItemDropNavigator/GsUIItemDropListContainer.h"

#include "UTIL/GsTableUtil.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"

#include "Core/Public/Delegates/Delegate.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

void UGsUIWindowRegionMap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::RegionMap;

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowRegionMap::OnClickCloseAllStack);
	// 지역맵에서는 뒤로가기처리가 닫기임(본부장님 컨펌 사항)
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowRegionMap::OnClickBack);
	// 대륙지도 위로 클릭
	_btnGoToContinentalMap->OnClicked.AddDynamic(this, &UGsUIWindowRegionMap::OnClickContinentalMap);

	BindMessage();
}

void UGsUIWindowRegionMap::NativeConstruct()
{
	Super::NativeConstruct();

	// 상단 이름 갱신
	UpdateUpperName();

	_btnGoToContinentalMap->SetIsEnabled(!(GGameData()->IsInvadeWorld()));
}

void UGsUIWindowRegionMap::NativeDestruct()
{
	//월드맵 정료시 전리품 저장 정보 리셋
	FGsGameStateRegionMap* GameStateRegionMap = UGsUIItemDropListContainer::GetGameStateRegionMap();
	if (GameStateRegionMap)
	{
		GameStateRegionMap->ClearSpoils();
		GameStateRegionMap->ClearSpoilsMonster();
	}

	Super::NativeDestruct();
}

void UGsUIWindowRegionMap::BeginDestroy()
{
	Super::BeginDestroy();

	UnBindMessage();
}

bool UGsUIWindowRegionMap::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsRegionMap))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);
		return true;
	}

	return false;
}

// 지역맵 변경(현재 탭 갱신)
void UGsUIWindowRegionMap::ChangeRegionMap(bool In_byDropItem)
{
	_regionMap->ChangeRegionMap(In_byDropItem);

	// 상단 이름 갱신
	UpdateUpperName();
}

// 지역맵 변경(몬스터, 씬 고름)
void UGsUIWindowRegionMap::ChangeRegionMapByType(EGsRegionMapType In_type)
{
	_regionMap->ChangeRegionMapByType(In_type);

	// 상단 이름 갱신
	UpdateUpperName();
}

// 상단 이름 변경
void UGsUIWindowRegionMap::UpdateUpperName()
{
	// 던전에서 나갈수 있게 하기 위해서
	// 무조건 대륙맵 보이게 처리
	bool isDungeon = false;

	// 지역맵 id
	int regionMapId = GSWorldMap()->GetShowRegionMapId();

	const FGsSchemaMapData* mapData = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(regionMapId);
	if (mapData == nullptr)
	{
		return;
	}

	int switchIndex = 0;
	if (isDungeon == false)
	{
		switchIndex = 0;
		// 현재 지역맵 상위 대륙맵 tbl id
		int currentContinentalTblId =
			GSWorldMap()->FindContinentalTblIdByCurrentRegionId();

		const FGsSchemaContinentalMapData* continentalMapData =
			UGsTableUtil::FindRowById<UGsTableContinentalMapData, FGsSchemaContinentalMapData>(currentContinentalTblId);

		if (continentalMapData == nullptr)
		{
			return;
		}

		_textContinentalMapName = continentalMapData->mapStringId;
		_textRegionMapNameNormal = FGsWorldMapManager::GetMapName(regionMapId);
	}
	else
	{
		switchIndex = 1;
		_textRegionMapNameDungeon = mapData->MapStringId;
	}

	_switcherUpperName->SetActiveWidgetIndex(switchIndex);
}


// 대륙맵 클릭
void UGsUIWindowRegionMap::OnClickContinentalMap()
{
	// https://jira.com2us.com/jira/browse/C2URWQ-4984
	// 던전 메뉴 안보이게
	GSWorldMap()->SetIsShowDungeonMenu(false);
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_CONTINENTALMAP);
}

void UGsUIWindowRegionMap::OnMonsterDrop(const IGsMessageParam* InParam)
{
	const FGsUIMsgRegionMapTypeParam* paramTarget = InParam->Cast<const FGsUIMsgRegionMapTypeParam>();

	// 획득처에서만 처리
	if(paramTarget->_regionMapType == EGsRegionMapType::Spoils)
	{	
		ChangeRegionMap(true);
	}
}

// 백버튼 클릭
bool UGsUIWindowRegionMap::OnBack()
{
	// 대륙맵도 닫는다
	if (UGsUIManager* uiMgr = GUI())
	{
		uiMgr->CloseByKeyName(TEXT("WindowContinentalMap"));
		uiMgr->CloseByKeyName(TEXT("PopupItemDrop"));
	}

	GSLOG(Log, TEXT("WorldMap - UGsUIWindowRegionMap - OnBack"));

	return Super::OnBack();
}

void UGsUIWindowRegionMap::BindMessage()
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
		MessageGameObject::UI_CLICK_REGION_MAP_TYPE_TAB, this, &UGsUIWindowRegionMap::OnMonsterDrop
	));
}

void UGsUIWindowRegionMap::UnBindMessage()
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
