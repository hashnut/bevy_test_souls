#include "GsUIRegionScene.h"

#include "Map/Bookmark/GsBookmarkData.h"
#include "Map/Bookmark/GsBookmarkFunc.h"
#include "Map/WorldMap/GsRegionInfo.h"
#include "Map/WorldMap/GsRegionMonsterInfo.h"
#include "Map/WorldMap/GsWorldMapFunc.h"
#include "Map/WorldMap/GsWorldMapDefine.h"

#include "UI/UIContent/Window/RegionMap/GsUIRegionSceneImage.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionIconRoot.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionWarpListItem.h"
#include "UI/UIContent/Window/RegionMap/GsUIRegionIconFiltering.h"

#include "RegionScene/GsUIRegionScenePortalPanel.h"
#include "RegionScene/GsUIRegionSceneSpoilsPanel.h"
#include "RegionScene/GsUIRegionSceneItemDropPanel.h"

#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconSelect.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconSelectMonster.h"
#include "UI/UIContent/Window/RegionMap/MapIcon/GsRegionMapIconSelectSanctum.h"
#include "UI/UIContent/Window/RegionMap/Monster/GsUIRegionMonsterList.h"

#include "UI/UIContent/Popup/RegionScene/GsUIPopupRegionSceneSpoils.h"
#include "UI/UIContent/Popup/ItemDropNavigator/GsUIItemDropListContainer.h"

#include "UI/UIContent/Common/Icon/GsUIIconAbnormality.h"
#include "UI/UIContent/Common/Icon/GsUIIconRegionMapInvasion.h"
#include "UI/UIContent/Common/Tooltip/GsUITooltipTitleDesc.h"
#include "UI/UIContent/Common/Sanctum/GsUISanctumBuffInfoRegionMap.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Helper/GsUIColorHelper.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"
#include "UI/UIControlLib/ContentWidget/GsSwitcherButton.h"
#include "UI/UIControlLib/ContentWidget/GsHorizontalBoxIconSelector.h"

#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "UI/UILib/Container/GsWidgetPool.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGameClientVersion.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapTerritoryData.h"
#include "DataSchema/Map/Spot/GsSchemaSpotInfo.h"
#include "DataSchema/Map/WorldMap/GsSchemaRegionMapData.h"
#include "DataSchema/Skill/Passivity/GsSchemaPassivitySet.h"
#include "DataSchema/InterServer/GsSchemaInvadeMapData.h"

#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "Message/GsMessageContents.h"
#include "Message/GsMessageGameObject.h"

#include "Message/MessageParam/GsUIMessageParam.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsClientSpawnManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsBookmarkManager.h"
#include "Management/ScopeGame/GsEventProgressManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"

#include "GameObject/Movement/GsMovementHandlerLocalPlayer.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/ContentsGame/Hud/GsGameStateHud.h"
#include "GameFlow/GameContents/ContentsGame/Hud/Bookmark/GsBookmarkHandler.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "UTIL/GsTableUtil.h"
#include "UTIL/GsUIUtil.h"

#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/Widget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "UMG/Public/Components/ScrollBox.h"
#include "UMG/Public/Blueprint/UserWidget.h"

#include "Core/Public/Delegates/Delegate.h"

#include "T1Project.h"




void UGsUIRegionScene::NativeOnInitialized()
{
	Super::NativeOnInitialized();	

	//우측 지도 표시 기능 
	_switcherBtnAutoMovePos->OnClicked.AddDynamic(this, &UGsUIRegionScene::OnClickAutoMovePos);
	_switcherBtnMyPos->OnClicked.AddDynamic(this, &UGsUIRegionScene::OnClickMyPos);
	_btnFiltering->OnClicked.AddDynamic(this, &UGsUIRegionScene::OnClickFiltering);
	_switcherBtnMonsterDropFilter->OnClicked.AddDynamic(this, &UGsUIRegionScene::OnClickMonsterDropFiltering);	

	_iconRoot->SetCallbackClickPortalIcon([this](UGsRegionMapIconSelect* In_selIcon)
		{
			OnClickPortalIcon(In_selIcon);
		});

	_iconRoot->SetCallbackClickInvasionPortalIcon([this](UGsUIIconRegionMapInvasion* In_icon,
		int64 In_spotId, FVector In_pos, FString In_text)
		{
			OnClickInvasionPortalIcon(In_icon, In_spotId, In_pos, In_text);
		});

	_iconRoot->SetCallbackClickMonsterDropIcon([this](UGsRegionMapIconSelectMonster* In_selIcon, FVector In_pos)
		{
			OnClickMonsterDropIcon(In_selIcon, In_pos);
		});

	_iconRoot->SetCallbackClickSanctum([this](UGsRegionMapIconSelectSanctum* In_selIcon)
		{
			OnClickSanctum(In_selIcon);
		});

	_sceneImage->SetCallbackClickImage([this](FVector In_pos)
		{
			_iconRoot->OnClickMoveableTerrain(In_pos);
			// release icon menu & select list
			ReleaseSelectList();
		});
	_sceneImage->SetCallbackClickCantMoveZone([this]
		{
			// click can't move zone -> hide touch menu
			_iconRoot->HideTouchMenu();
		});

	_monsterList->OnSelectMonsterList.BindUObject(this, &UGsUIRegionScene::OnSelectMonsterList);
}

void UGsUIRegionScene::NativeConstruct()
{
	for (UGsUITooltipTitleDesc* tooltip : _arrayTooltipEnv)
	{
		if (nullptr == tooltip)
		{
			continue;
		}

		tooltip->Close();
	}

	//Hide item data	
	HideIconFilterring();
	HideItemDrop();

	InitializeMessage();
	Super::NativeConstruct();
}

void UGsUIRegionScene::NativeDestruct()
{
	for (UGsHorizontalBoxIconSelector* selector : _arrayIconSelectorEnv)
	{
		if (selector == nullptr)
		{
			continue;
		}

		selector->RemoveAllChildren();
	}

	FinalizeMessage();
	Super::NativeDestruct();
}

void UGsUIRegionScene::InitializeMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
	{
		return;
	}

	MGameObject& msgObject = msg->GetGameObject();

	_arrGameObjDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::UI_MAP_MONSTER_DROP_ICON_TOGGLE, this, &UGsUIRegionScene::OnInitMonsterDropFiltering
	));
	_arrGameObjDelegate.Emplace(msgObject.AddUObject(
		MessageGameObject::UI_MAP_MONSTER_DROP_ACTIVE, this, &UGsUIRegionScene::OnActiveMonsterDropFiltering
	));

	MUI& msgUI = msg->GetUI();
	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::SHOW_WORLD_MAP_ITEM_DROP, this,  &UGsUIRegionScene::OnShowItemDrop));
	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::HIDE_WORLD_MAP_ITEM_DROP, this, &UGsUIRegionScene::OnHideItemDrop));
	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::SELECT_REGION_SCENE_PORTAL, this, &UGsUIRegionScene::OnSelectPortal));

	_uiMsgArray.Emplace(msgUI.AddUObject(MessageUI::SELECT_REGION_SCENE_BOOKMARK, this, &UGsUIRegionScene::OnSelectBookmark));

}

void UGsUIRegionScene::FinalizeMessage()
{
	FGsMessageHolder* msg = GMessage();
	if (nullptr == msg)
	{
		return;
	}

	if (0 != _arrGameObjDelegate.Num())
	{
		for (auto iter : _arrGameObjDelegate)
		{
			msg->GetGameObject().Remove(iter);
		}
		_arrGameObjDelegate.Empty();
	}

	if (0 != _uiMsgArray.Num())
	{
		for (auto iter : _uiMsgArray)
		{
			msg->GetUI().Remove(iter);
		}
		_uiMsgArray.Empty();
	}
}

// 해당 씬 데이터로 세팅
void UGsUIRegionScene::SetSceneData(EGsRegionMapType inMapType, bool In_byDropItem)
{
	InitRegionSceneData();

	ResetSelectPortalIndex();
	ResetSelectBookmarkIndex();
	ResetSelectInvasionPortalSpotId();
	ResetSelectMonsterDropId();
	ResetSelectSanctumId();

	HideIconFilterring();
	HideItemDrop();

	SetMapTypeTabChanged(inMapType, In_byDropItem);

	// 몬스터 리스트 선택 dirty 값 여기서 클리어하자
	// 토클 construct 시점에 디폴트 값(0)과 다른값 세팅시 
	// 이벤트가 일어나서 몬스터 리스트 세팅 2번 호출되어
	// 1번째꺼에서 dirty clear시켜버려서
	// 2번째에서 몬스터 리스트 선택이 안됨(보스 -> 지역맵)
	bool isUseMonsterSelectIndex = GSWorldMap()->GetIsUseMonsterSelectIndex();
	if (isUseMonsterSelectIndex == true)
	{
		// clear bool flag
		GSWorldMap()->SetIsUseMonsterSelectIndex(false);
	}

}
// 탭변경에 의한 데이터 세팅
// 기존에는 다 세팅하던거 클릭만 일어났을때 분리해서 처리
void UGsUIRegionScene::SetMapTypeTabChanged(EGsRegionMapType inMapType, bool In_byDropItem)
{
	FGsGameStateRegionMap* gameStateRegionMap = UGsUIItemDropListContainer::GetGameStateRegionMap();
	if (nullptr == gameStateRegionMap)
	{
		return;
	}

	_currentTabMapType = inMapType;

	// 그림자 전장에서는 왼쪽 리스트 안보여줌
	// https://jira.com2us.com/jira/browse/C2URWQ-7853
	MapType mapType = GSWorldMap()->GetShowRegionMapTableType();

	
	if (mapType == MapType::GUILD_WAR)
	{	
		_switcherLeftList->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	// 그외에는 킴
	_switcherLeftList->SetVisibility(ESlateVisibility::SelfHitTestInvisible);


	switch (inMapType)
	{
	case EGsRegionMapType::Spoils:
	{
		_switcherLeftList->SetActiveWidgetIndex(LEFT_LIST_INDEX_SPOIL);
		_spoilsPanel->InitializeRegionSceneSpoilsPanelWidget();

		// 몬스터 드랍 필터 버튼 갱신
		// 전에는 획득처에서만 처리했는데
		// 몬스터 탭에서도 처리하게 되어서 일괄 처리
		gameStateRegionMap->InvalidateMonsterDropFilter();

		//전리품 탭에서 이전에 선택한 전리품과 전리품 드랍 몬스터 정보가 있다면 전리품 몬스터 정보를 보여줘야한다.
		_switcherBtnMonsterDropFilter->SetVisibility(ESlateVisibility::Visible);

		// 프리징 이슈 대응
		// https://jira.com2us.com/jira/browse/C2URWQ-6455
		bool isShowDropItemMap = true;
		// 신규 버전일때만 인자로 체크
		// drop item에 의한 호출이면 안함(처리시 재귀호출)		
		if (In_byDropItem == true)
		{
			isShowDropItemMap = false;
		}

		if (isShowDropItemMap == true)
		{
			FGsGameStateRegionMap::FGsSpoilsMonster selectMonster;
			if (gameStateRegionMap->TryGetSpoilsMonster(selectMonster)
				&& GSWorldMap()->GetShowRegionMapId() == selectMonster.mapId)
			{
				gameStateRegionMap->ShowDropItemMap(selectMonster.mapId, selectMonster.level, selectMonster.npcId, EGsToggleType::Active);
			}

		}
	}
	break;

	// 몬스터도 지도를 표현해야하므로 몬스터 리스트도 여기로 와야함
	// https://jira.com2us.com/jira/browse/CHR-23756
	case EGsRegionMapType::Monster:
	{
		_switcherBtnMonsterDropFilter->SetVisibility(ESlateVisibility::Visible);
		// 몬스터 리스트 정보(좌측) switch
		_switcherLeftList->SetActiveWidgetIndex(LEFT_LIST_INDEX_MONSTER);


		// 몬스터 드랍 필터 버튼 갱신
		// 전에는 획득처에서만 처리했는데
		// 몬스터 탭에서도 처리하게 되어서 일괄 처리
		// 이게 먼저 되고
		// 아래서 몬스터 선택해야함
		// void FGsGameStateRegionMap::InvalidateMonsterDropFilter() 1304 line
		// if ((false == IsEmptySpoilsMonster() && false == IsEmptySpoils())
		// 여기 IsEmptySpoils 에서 걸려서 보여준 처리가 안됨...
		gameStateRegionMap->InvalidateMonsterDropFilter();


		// 몬스터 리스트 갱신
		_monsterList->InitRegionMonsterListData();
	}
	break;
	case EGsRegionMapType::Sanctum:
	{
		_switcherBtnMonsterDropFilter->SetVisibility(ESlateVisibility::Visible);
		// 성소 정보(좌측) switch
		_switcherLeftList->SetActiveWidgetIndex(LEFT_LIST_INDEX_SANCTUM);

		// 몬스터 드랍 필터 버튼 갱신
		// 전에는 획득처에서만 처리했는데
		// 몬스터 탭에서도 처리하게 되어서 일괄 처리
		gameStateRegionMap->InvalidateMonsterDropFilter();

		// 성소 정보 갱신
		// 0이면 선택된게 없음
		if (_selectSanctumId == 0)
		{
			_sanctumInfo->SetSelectInfo(false);
		}
		else
		{
			_sanctumInfo->SetData(_selectSanctumId);
			_sanctumInfo->SetSelectInfo(true);
		}

	}
	break;
	}
}
void UGsUIRegionScene::BeginDestroy()
{		
	Super::BeginDestroy();
}

// 데이터 초기화
void UGsUIRegionScene::InitRegionSceneData()
{	
	_sceneImage->LoadSceneImage();
	_iconRoot->ClearAllIcon();
	// 아이콘 부터 만들고
	_iconRoot->MakeAllIcon();

	// 일반적인 경우에는 몬스터 드랍 아이콘 보이지 않도록 설정
	GSWorldMap()->SetIsMonsterDropIconFiltered(false);

	if (GSInterServer() && GSInterServer()->IsValidInvasionTime())
		GSInterServer()->ReqInvasionPortalInfo();

	_switcherBtnMonsterDropFilter->SetVisibility(ESlateVisibility::Collapsed);

	// 리스트를 만든다(리스트 선택 후 icon 선택 표시 할수 있으므로)
		//침공전인가?
	if (GGameData()->IsInvadeWorld())
	{
		_btnSection->SetVisibility(ESlateVisibility::Hidden);
	}
	// 월드맵 개선
	// 던전에서도 우측 버튼들 사용 가능하게 변경
	// https://jira.com2us.com/jira/browse/CHR-23756
	else 
	{
		_btnSection->SetVisibility(ESlateVisibility::SelfHitTestInvisible);		
	}
	

	SetEnvIcon(GSWorldMap()->GetShowRegionMapId());

	// RegionScene 열었을 때나 탭 전환 시 아이콘 두개 딤드 해제 처리
	_switcherBtnAutoMovePos->SetIsSelected(true);
	_switcherBtnAutoMovePos->SetVisibility(ESlateVisibility::Visible);
	_switcherBtnMyPos->SetIsSelected(true);
	_switcherBtnMyPos->SetVisibility(ESlateVisibility::Visible);
}

// 목적지 클릭
void UGsUIRegionScene::OnClickAutoMovePos()
{
	// 목적지 표시 가능하지 않으면 안함	
	if (false == GSWorldMap()->IsUseableShowAutoMovePos())
	{
		FText findText;
		FText::FindText(TEXT("WorldMapText"), TEXT("ErrorNotMoving"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	// 현재랑 같아도 안함
	if (true == GSWorldMap()->IsTargetMapSame(EGsWorldMapChangeType::TO_REGIONMAP_AUTOMOVEPOS))
	{
		// 바로 선택 표시
		_iconRoot->SelectIcon(EGsRegionIconSelectType::AutoMoveTargetPosIcon, true);
		return;
	}

	// 현재 목적지의 지역맵으로 이동(automove에서 데이터 접근)
	if (GMode())
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (FGsGameStateRegionMap* regionMap =
					contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
				{
					regionMap->SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_AUTOMOVEPOS);
				}
			}
		}
	}
	GMessage()->GetUI().SendMessage(MessageUI::CHANGE_REGION_MAP, nullptr);

	// old cod: 341 line backup -> select icon -> init scene -> clear select
	// select call after make icon 
	_iconRoot->SelectIcon(EGsRegionIconSelectType::AutoMoveTargetPosIcon, true);
}
// 내위치 클릭
void UGsUIRegionScene::OnClickMyPos()
{
	// 현재랑 같으면 안함
	if (true == GSWorldMap()->IsTargetMapSame(EGsWorldMapChangeType::TO_REGIONMAP_MYPOS))
	{
		return;
	}

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
					regionMap->SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_MYPOS);
				}
			}
		}
	}

	GMessage()->GetUI().SendMessage(MessageUI::CHANGE_REGION_MAP, nullptr);
}

void UGsUIRegionScene::OnPressIconEnv(class UGsUIIconAbnormality* InIcon)
{
	if (InIcon == nullptr ||
		InIcon->_targetTooltip == nullptr)
	{
		return;
	}

	UGsUITooltipTitleDesc* tooltip = Cast<UGsUITooltipTitleDesc>(InIcon->_targetTooltip);
	if (tooltip == nullptr)
	{
		return;
	}

	if (tooltip->IsOpen())
	{
		tooltip->Close();
		return;
	}

	const FGsSchemaPassivitySet* passivitySet = GetEnvPassivitySetByMapId(GSWorldMap()->GetShowRegionMapId());
	if (passivitySet)
	{
		tooltip->SetTitleColor(FGsUIColorHelper::GetColorNameTextPassivity(
			passivitySet->isBuff, PassivityOwnerType::MAP));
		tooltip->SetTitleText(passivitySet->nameText);
		tooltip->SetDescText(passivitySet->descText);
	}
	else
	{
		tooltip->SetDefaultColor();

		// TEXT: 없음
		FText textTitle;
		FText::FindText(TEXT("EnvironmentText"), TEXT("EnvNone"), textTitle);
		tooltip->SetTitleText(textTitle);

		// TEXT: 효과 없음
		FText textDesc;
		FText::FindText(TEXT("EnvironmentText"), TEXT("DebuffNone"), textDesc);
		tooltip->SetDescText(textDesc);
	}

	tooltip->Open();
}
void UGsUIRegionScene::OnPressIconInvasionInvadeEnv(class UGsUIIconAbnormality* InIcon)
{
	if (InIcon == nullptr ||
		InIcon->_targetTooltip == nullptr)
	{
		return;
	}

	UGsUITooltipTitleDesc* tooltip = Cast<UGsUITooltipTitleDesc>(InIcon->_targetTooltip);
	if (nullptr == tooltip)
	{
		return;
	}

	if (tooltip->IsOpen())
	{
		tooltip->Close();
		return;
	}

	const FGsSchemaPassivitySet* passivitySet = GetInvadeEnvPassivitySetByMapId(GSWorldMap()->GetShowRegionMapId(), true);
	if (passivitySet)
	{
		tooltip->SetTitleColor(FGsUIColorHelper::GetColorNameTextPassivity(
			passivitySet->isBuff, PassivityOwnerType::MAP));
		tooltip->SetTitleText(passivitySet->nameText);
		tooltip->SetDescText(passivitySet->descText);
	}
	else
	{
		tooltip->SetDefaultColor();

		// TEXT: 없음
		FText textTitle;
		FText::FindText(TEXT("EnvironmentText"), TEXT("EnvNone"), textTitle);
		tooltip->SetTitleText(textTitle);

		// TEXT: 효과 없음
		FText textDesc;
		FText::FindText(TEXT("EnvironmentText"), TEXT("DebuffNone"), textDesc);
		tooltip->SetDescText(textDesc);
	}

	tooltip->Open();
}
void UGsUIRegionScene::OnPressIconInvasionDefenseEnv(class UGsUIIconAbnormality* InIcon)
{
	if (InIcon == nullptr ||
		InIcon->_targetTooltip == nullptr)
	{
		return;
	}

	UGsUITooltipTitleDesc* tooltip = Cast<UGsUITooltipTitleDesc>(InIcon->_targetTooltip);
	if (nullptr == tooltip)
	{
		return;
	}

	if (tooltip->IsOpen())
	{
		tooltip->Close();
		return;
	}

	const FGsSchemaPassivitySet* passivitySet = GetInvadeEnvPassivitySetByMapId(GSWorldMap()->GetShowRegionMapId(), false);
	if (passivitySet)
	{
		tooltip->SetTitleColor(FGsUIColorHelper::GetColorNameTextPassivity(
			passivitySet->isBuff, PassivityOwnerType::MAP));
		tooltip->SetTitleText(passivitySet->nameText);
		tooltip->SetDescText(passivitySet->descText);
	}
	else
	{
		tooltip->SetDefaultColor();

		// TEXT: 없음
		FText textTitle;
		FText::FindText(TEXT("EnvironmentText"), TEXT("EnvNone"), textTitle);
		tooltip->SetTitleText(textTitle);

		// TEXT: 효과 없음
		FText textDesc;
		FText::FindText(TEXT("EnvironmentText"), TEXT("DebuffNone"), textDesc);
		tooltip->SetDescText(textDesc);
	}

	tooltip->Open();
}
void UGsUIRegionScene::SetEnvIcon(int32 InMapId)
{
	int currentIconIndex = 0;
	// 1. check map env
	const FGsSchemaPassivitySet* mapPassivitySet = GetEnvPassivitySetByMapId(InMapId);
	if (mapPassivitySet &&
		false == mapPassivitySet->hideUI)
	{
		UGsUIIconAbnormality* icon = GetEnvIconByIndex(currentIconIndex);
		if (icon != nullptr)
		{
			icon->SetData(mapPassivitySet);
			icon->OnPressIcon.Clear();
			icon->OnPressIcon.AddUObject(this, &UGsUIRegionScene::OnPressIconEnv);

			icon->_targetTooltip = GetEnvTooltipByIndex(currentIconIndex++);
		}
	}	
	// 2. check invasion
	// 2.1 invade state(in other server)
	if (GGameData()->IsInvadeWorld() == true)
	{
		const FGsSchemaPassivitySet* invadeMapPassivitySet = GetInvadeEnvPassivitySetByMapId(InMapId, true);
		if (invadeMapPassivitySet &&
			false == invadeMapPassivitySet->hideUI)
		{
			UGsUIIconAbnormality* icon = GetEnvIconByIndex(currentIconIndex);
			if (icon != nullptr)
			{
				icon->SetData(invadeMapPassivitySet);
				icon->OnPressIcon.Clear();
				icon->OnPressIcon.AddUObject(this, &UGsUIRegionScene::OnPressIconInvasionInvadeEnv);

				icon->_targetTooltip = GetEnvTooltipByIndex(currentIconIndex++);
			}
		}
	}
	// 2.2 defense state(invade possible time)
	else if (GSInterServer()->IsValidInvasionMap(InMapId) == true)
	{
		const FGsSchemaPassivitySet* invadeMapPassivitySet = GetInvadeEnvPassivitySetByMapId(InMapId, false);
		if (invadeMapPassivitySet &&
			false == invadeMapPassivitySet->hideUI)
		{
			UGsUIIconAbnormality* icon = GetEnvIconByIndex(currentIconIndex);
			if (icon != nullptr)
			{
				icon->SetData(invadeMapPassivitySet);
				icon->OnPressIcon.Clear();
				icon->OnPressIcon.AddUObject(this, &UGsUIRegionScene::OnPressIconInvasionDefenseEnv);

				icon->_targetTooltip = GetEnvTooltipByIndex(currentIconIndex++);
			}
		}
	}
	// 2.3 defense state(invade impossible time)-> no invasion icon

	// hide remain icon
	for (int i = currentIconIndex; _arrayIconSelectorEnv.Num() > i; ++i)
	{
		UGsUIIconAbnormality* icon = GetEnvIconByIndex(i);
		if (nullptr == icon)
		{
			continue;
		}
		icon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

UGsUIIconAbnormality* UGsUIRegionScene::GetEnvIconByIndex(int In_index)
{
	if (_arrayIconSelectorEnv.Num() <= In_index)
	{
		return nullptr;
	}
	UGsHorizontalBoxIconSelector* selector = _arrayIconSelectorEnv[In_index];

	if (nullptr == selector)
	{
		return nullptr;
	}

	UGsUIIconAbnormality* icon = Cast<UGsUIIconAbnormality>(selector->GetChildAt(0));
	if (nullptr == icon)
	{
		icon = Cast<UGsUIIconAbnormality>(selector->CreateIcon());
		selector->AddChild(icon);
	}

	return icon;
}

UGsUITooltipTitleDesc* UGsUIRegionScene::GetEnvTooltipByIndex(int In_index)
{
	if (_arrayTooltipEnv.Num() <= In_index)
	{
		return nullptr;
	}

	return _arrayTooltipEnv[In_index];
}

const FGsSchemaPassivitySet* UGsUIRegionScene::GetEnvPassivitySetByMapId(int32 InMapId) const
{
	const UGsTableMapData* mapTable = Cast<UGsTableMapData>(FGsSchemaMapData::GetStaticTable());
	if (nullptr == mapTable)
	{
		return nullptr;
	}

	const FGsSchemaMapData* mapData = nullptr;
	if (false == mapTable->FindRowById(InMapId, mapData))
	{
		return nullptr;
	}

	const FGsSchemaMapBalance* mapBalanceData = mapData->balanceId.GetRow();
	if (nullptr == mapBalanceData)
	{
		return nullptr;
	}

	if (false == mapBalanceData->passivityIdList.IsValidIndex(mapBalanceData->passivityIdListIndexEnv))
	{
		return nullptr;
	}

	// 지정된 인덱스의 아이콘만 표시한다
	return mapBalanceData->passivityIdList[mapBalanceData->passivityIdListIndexEnv].GetRow();
}

const FGsSchemaPassivitySet* UGsUIRegionScene::GetInvadeEnvPassivitySetByMapId(int32 InMapId, bool In_isInvade) const
{
	const UGsTableInvadeMapData* table = Cast<UGsTableInvadeMapData>(FGsSchemaInvadeMapData::GetStaticTable());
	if (nullptr == table)
	{
		return nullptr;
	}
	TArray<const FGsSchemaInvadeMapData*> arrayData;
	table->GetAllRows<FGsSchemaInvadeMapData>(arrayData);

	const FGsSchemaInvadeMapData* invadeMapTbl = nullptr;
	for (const FGsSchemaInvadeMapData* iter : arrayData)
	{
		if (nullptr == iter)
		{
			continue;
		}
		if (nullptr == iter->mapId.GetRow())
		{
			continue;
		}
		const FGsSchemaMapData* mapData = iter->mapId.GetRow();
		if (mapData == nullptr)
		{
			continue;
		}
		if (mapData->id == InMapId)
		{
			invadeMapTbl = iter;
			break;
		}
	}
	
	if (invadeMapTbl == nullptr)
	{
		return nullptr;
	}

	int findIndex = (In_isInvade == true) ? invadeMapTbl->invadePassivityIdListIndexEnv : invadeMapTbl->defensePassivityIdListIndexEnv;
	TArray<FGsSchemaPassivitySetRow> passivityList = (In_isInvade == true) ? invadeMapTbl->invadePassivityIdList : invadeMapTbl->defensePassivityIdList;

	if (passivityList.Num() == 0)
	{
		return nullptr;
	}

	if (false == passivityList.IsValidIndex(findIndex))
	{
		return nullptr;
	}

	// 지정된 인덱스의 아이콘만 표시한다
	return passivityList[findIndex].GetRow();
}

// 통합 인덱스로 type과 data index 찾기
void UGsUIRegionScene::FindInfoByTotalIndex(int In_totalIndex, EGsRegionWarpListItemType& Out_itemType,
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
int UGsUIRegionScene::FindTotalIndexByInfo(EGsRegionWarpListItemType In_itemType,
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


// click filtering btn
void UGsUIRegionScene::OnClickFiltering()
{
	ESlateVisibility newVisibility =
		(_iconFiltering->GetVisibility() == ESlateVisibility::Hidden) ?
		ESlateVisibility::SelfHitTestInvisible :
		ESlateVisibility::Hidden;
	_iconFiltering->SetVisibility(newVisibility);

	if (newVisibility == ESlateVisibility::SelfHitTestInvisible)
	{
		_iconFiltering->InitList();
	}
}

void UGsUIRegionScene::OnClickMonsterDropFiltering()
{
	SetMonsterDropFiltering();
}

void UGsUIRegionScene::OnInitMonsterDropFiltering(const IGsMessageParam* In_param)
{
	EGsToggleType toggleType = EGsToggleType::Auto;
	if (const FGsUIMsgToggleParam* param = In_param->Cast<const FGsUIMsgToggleParam>())
	{
		toggleType = param->_toggle;
	}

	_switcherBtnMonsterDropFilter->SetVisibility(ESlateVisibility::Visible);

	SetMonsterDropFiltering(toggleType);
}

void UGsUIRegionScene::OnActiveMonsterDropFiltering(const struct IGsMessageParam* In_param /*= nullptr*/)
{	
	const FGsUIMsgActiveParam* param = In_param->Cast<const FGsUIMsgActiveParam>();
	if (nullptr == param)
	{
		return;
	}
		
	if (param->_isActive)
	{					
		bool isMonsterDropIconFiltered = GSWorldMap()->GetIsMonsterDropIconFiltered();
		_switcherBtnMonsterDropFilter->SetIsSelected(isMonsterDropIconFiltered);
	}
	else
	{
		_switcherBtnMonsterDropFilter->SetSwitcherIndex((int32)EGsButtonType::Disable);
	}

	_switcherBtnMonsterDropFilter->SetIsEnabled(param->_isActive);	
}

void UGsUIRegionScene::HideIconFilterring()
{
	_iconFiltering->SetVisibility(ESlateVisibility::Hidden);
}

void UGsUIRegionScene::SetMonsterDropFiltering(EGsToggleType inToggleType /*= EGsToggleType::Auto*/)
{
	bool isMonsterDropIconFiltered  = GSWorldMap()->GetIsMonsterDropIconFiltered();
	// 이전 값
	bool oldValue = isMonsterDropIconFiltered;
	switch (inToggleType)
	{
	case EGsToggleType::Active:
	{
		isMonsterDropIconFiltered = true;		
	}
	break;
	case EGsToggleType::Deactive:
	{
		isMonsterDropIconFiltered = false;
	}
	break;
	case EGsToggleType::Auto:
	{
		isMonsterDropIconFiltered = !isMonsterDropIconFiltered;
	}
	break;
	}

	GSWorldMap()->SetIsMonsterDropIconFiltered(isMonsterDropIconFiltered);

	// 기존 값이랑 바뀌었으면 아이콘 메뉴 클리어 하기
	if (isMonsterDropIconFiltered != oldValue)
	{
		_iconRoot->HideIconMenu();

		ResetSelectPortalIndex();
		ResetSelectBookmarkIndex();
		ResetSelectInvasionPortalSpotId();
		ResetSelectMonsterDropId();
		ResetSelectSanctumId();
	}

	// 몬스터 필터링 아이콘 발광 효과
	_switcherBtnMonsterDropFilter->SetIsSelected(isMonsterDropIconFiltered);

	_iconRoot->MonsterIconVisibleToggle(isMonsterDropIconFiltered);
	//ReleaseSelectList();
	// 성소 클릭시 위에 이동 메뉴가 꺼져서 여기서 일단 뺀다
	// Deselect Monster Drop Icon
	_iconRoot->SelectMonsterDropIcon(false);
	_iconRoot->HideTouchMenu();
}

void UGsUIRegionScene::OnClickPortalIcon(UGsRegionMapIconSelect* In_clickedIconObj)
{
	int selectIndex = In_clickedIconObj->GetListSelectIndex();

	ResetSelectInvasionPortalSpotId();
	ResetSelectMonsterDropId();
	ResetSelectSanctumId();

	// 기존꺼랑 다른걸 선택한거면
	if (_selectPortalIndex != selectIndex)
	{
		// 인덱스 저장
		SelectPortalIndex(selectIndex);
		// 아이콘 메뉴 보여주기(워프, 자동 이동)
		_iconRoot->ShowIconMenu(In_clickedIconObj);
		// 아이콘 선택 표시 처리
		In_clickedIconObj->SetSelect(true);
	}
	else
	{
		// 기존과 같은거면 기존 메뉴 끔
		ResetSelectPortalIndex();		
		ResetSelectBookmarkIndex();
		_iconRoot->HideIconMenu();
	}
}
void UGsUIRegionScene::OnClickInvasionPortalIcon(
	UGsUIIconRegionMapInvasion* In_icon, int64 In_spotId, FVector In_pos, FString In_text)
{
	// 켜져 리스트 기반 포털 관련 정보 있는 것들 clear
	ResetSelectPortalIndex();
	ResetSelectBookmarkIndex();
	ResetSelectMonsterDropId();
	ResetSelectSanctumId();

	// 기존꺼랑 다른거라면
	if (_selectInvasionPortalSpotId != In_spotId)
	{
		SelectInvasionPortalSpotId(In_spotId);
		_iconRoot->ShowInvasionIconMenu(In_icon, In_spotId, In_pos, In_text);
	}
	// 같은거면 끄기
	else
	{
		ResetSelectInvasionPortalSpotId();
		_iconRoot->HideIconMenu();
	}	
}

void UGsUIRegionScene::OnSelectPortal(const struct IGsMessageParam* inParam)
{
	const FGsUISelectPortalParam* param = inParam->Cast<const FGsUISelectPortalParam>();
	if (nullptr == param)
	{
		return;
	}

	ResetSelectBookmarkIndex();
	ResetSelectInvasionPortalSpotId();

	if (_selectPortalIndex != param->_spotId)
	{
		SelectPortalIndex(param->_spotId);
		_iconRoot->ShowIconMenuByPortalIndex(param->_spotId);
	}
}

void UGsUIRegionScene::OnSelectBookmark(const struct IGsMessageParam* inParam)
{
	const FGsUISelectPortalParam* param = inParam->Cast<const FGsUISelectPortalParam>();
	if (nullptr == param)
	{
		return;
	}

	ResetSelectPortalIndex();
	ResetSelectInvasionPortalSpotId();
	ResetSelectMonsterDropId();
	ResetSelectSanctumId();

	if (_selectBookmarkIndex != param->_spotId)
	{
		SelectBookmarkIndex(param->_spotId);			
		_iconRoot->ShowIconMenuByBookmarkIndex(param->_spotId);
	}
}

void UGsUIRegionScene::OnClickMonsterDropIcon(UGsRegionMapIconSelectMonster* In_clickedIconObj, FVector In_pos)
{
	if (nullptr == In_clickedIconObj)
	{
		return;
	}

	ReleaseSelectList();

	ResetSelectPortalIndex();
	ResetSelectInvasionPortalSpotId();
	ResetSelectBookmarkIndex();
	ResetSelectSanctumId();

	int selectIndex = In_clickedIconObj->GetListSelectIndex();

	// 기존과 같은 인덱스면
	if (_selectMonsterDropIndex == selectIndex)
	{
		// id 클리어
		ResetSelectMonsterDropId();
		_iconRoot->HideIconMenu();
	}
	// 다른거면
	else
	{
		SelectMonsterDropId(selectIndex);
		// 선택 표시
		_iconRoot->SelectIcon(EGsRegionIconSelectType::MonsterDropIcon, true, selectIndex);
		// 아이콘 메뉴 띄우기
		_iconRoot->ShowMonsterDropIconMenu(In_clickedIconObj, In_pos);
	}
	
	_iconRoot->HideTouchMenu();
	//FGsWorldMapFunc::ClickTeleportRegionMapMonster(In_clickedIconObj->GetMapId(), In_clickedIconObj->GetNpcId(),  In_clickedIconObj->GetSpawnGroupId());
}

// release select list and hide icon menu
void UGsUIRegionScene::ReleaseSelectList()
{

	// hide menu
	_iconRoot->HideIconMenu();

	// Deselect Monster Drop Icon
	_iconRoot->SelectMonsterDropIcon(false);
}

void UGsUIRegionScene::OnShowItemDrop(const struct IGsMessageParam* inParam)
{
	//show item
	const FGsUIMsgWorldMapItemDrop* param = inParam->Cast<const FGsUIMsgWorldMapItemDrop>();
	if (nullptr == param)
	{
		return;
	}

	ShowItemDrop(param->_monsterArray);
}

void UGsUIRegionScene::OnHideItemDrop(const struct IGsMessageParam* inParam)
{
	HideItemDrop();
}

void UGsUIRegionScene::ShowItemDrop(const TArray<FGsSchemaWorldMapSpoilsMonsterLevel>& inData)
{
	TWeakObjectPtr<UGsUIWidgetBase> widget = GUI()->OpenAndGetWidget(TEXT("PopupItemDrop"));
	if (false == widget.IsValid())
	{
		return;
	}

	UGsUIPopupRegionSceneSpoils* regionScene = Cast<UGsUIPopupRegionSceneSpoils>(widget.Get());

	regionScene->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	regionScene->SetMonsterData(inData);
}

void UGsUIRegionScene::HideItemDrop()
{
	GUI()->CloseByKeyName(TEXT("PopupItemDrop"));
}
// 몬스터 리스트 선택
// https://jira.com2us.com/jira/browse/CHR-23756
void UGsUIRegionScene::OnSelectMonsterList(int32 In_index)
{

	FGsGameStateRegionMap* gameStateRegionMap = UGsUIItemDropListContainer::GetGameStateRegionMap();
	if ( nullptr== gameStateRegionMap)
	{
		return;
	}

	FGsRegionMonsterInfo* monsterInfo =
		GSWorldMap()->GetCurrentRegionMonsterInfoByIndex(In_index);

	if (monsterInfo == nullptr)
	{
		return;
	}

	int currentMapId = GSWorldMap()->GetShowRegionMapId();
	if (monsterInfo->_npcData == nullptr)
	{
		return;
	}
	int npcId = monsterInfo->_npcData->id;

	// 아이콘 출력& 필터 on
	gameStateRegionMap->ShowSpoilIconAndFilterButtonOn(currentMapId, npcId, monsterInfo->level);

	// 선택 되어있는 드랍 아이텝 정보가있으면 클리어
	if(_selectMonsterDropIndex != -1)
	{ 
		// id 클리어
		ResetSelectMonsterDropId();
		_iconRoot->HideIconMenu();
	}

}

// 성소 클릭
void UGsUIRegionScene::OnClickSanctum(UGsRegionMapIconSelectSanctum* In_clickSanctumObj)
{
	if (In_clickSanctumObj == nullptr)
	{
		return;
	}

	ResetSelectInvasionPortalSpotId();
	ResetSelectMonsterDropId();
	ResetSelectPortalIndex();
	ResetSelectBookmarkIndex();


	SanctumAreaId selectId = In_clickSanctumObj->GetSanctumId();

	// 기존 꺼랑 다른거 선택
	if (_selectSanctumId != selectId)
	{
		// 선택 id 저장
		SelectSanctumId(selectId);
		// 아이콘 메뉴 보여주기(자동이동)
		_iconRoot->ShowSanctumIconMenu(In_clickSanctumObj);

		In_clickSanctumObj->SetSelect(true);

		// 탭변경
		FGsUIMsgRegionMapTypeParam sendParam(EGsRegionMapType::Sanctum);
		GMessage()->GetGameObject().SendMessage(MessageGameObject::UI_CLICK_REGION_MAP_TYPE_TAB, &sendParam);
	}
	else
	{
		// 같은거 선택이면 clear 와 메뉴 hide
		ResetSelectSanctumId();
		_iconRoot->HideIconMenu();
	}
}
// id 초기화
void UGsUIRegionScene::ResetSelectSanctumId()
{ 
	// 기존에 선택되어있고
	// 토글 페이지가 선택되어 있다면
	// 선택안됨으로 바꿈
	if (_selectSanctumId != 0)
	{
		if (_currentTabMapType == EGsRegionMapType::Sanctum)
		{
			_sanctumInfo->SetSelectInfo(false);
		}
	}
	_selectSanctumId = 0; 
}
// 아무 성소아이콘 선택되게 처리(탭변경시 처리)
void UGsUIRegionScene::OnForcedSelectSanctum()
{
	// 성소 탭 변경 클릭시
	// 1. 현재 성소가 선택되어있지 않고
	// 2. 현재맵에 성소가 하나라도 있으면
	// 3. 강제로 하나 선택하게 처리
	if (_selectSanctumId != 0)
	{
		return;
	}

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

	FGsGameDataManager* dataManager = GGameData();
	if (dataManager == nullptr)
	{
		return;
	}
	bool isInvadeWorld = dataManager->IsInvadeWorld();
	// 침공전이거나 성소 정보 없으면 return
	if (regionInfo->_arrayRegionMapSanctumInfo.Num() == 0 || isInvadeWorld == true)
	{
		return;
	}

	FGsRegionMapSanctumInfo* info = regionInfo->_arrayRegionMapSanctumInfo[0];
	if (info == nullptr)
	{
		return;
	}


	// 아이콘 클릭과 같은 처리
	_iconRoot->UnselectAllPortal();
	_iconRoot->UnselectAllBookmark();
	_iconRoot->UnselectAllSanctum();

	
	ResetSelectInvasionPortalSpotId();
	ResetSelectMonsterDropId();
	ResetSelectPortalIndex();
	ResetSelectBookmarkIndex();


	SanctumAreaId selectId = info->_sanctumAreaId;


	// 선택 id 저장
	SelectSanctumId(selectId);

	// 해당 아이콘 찾기

	// 아이콘 메뉴 보여주기(자동이동)
	_iconRoot->ShowSanctumIconMenuBySanctumId(selectId);

	// 현재 탭 정보 갱신
	_sanctumInfo->SetData(selectId);
	_sanctumInfo->SetSelectInfo(true);
}