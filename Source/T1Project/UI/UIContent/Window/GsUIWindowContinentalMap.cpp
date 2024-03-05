#include "GsUIWindowContinentalMap.h"

#include "Management/GsMessageHolder.h"
#include "Management/ScopeGlobal/GsGameFlowManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsInterServerManager.h"

#include "DataSchema/Map/WorldMap/GsSchemaContinentalMapData.h"

#include "GameFlow/GsGameFlowGame.h"
#include "GameFlow/GameContents/ContentsGame/GsContentsManagerGame.h"
#include "GameFlow/GameContents/ContentsGame/GsGameStateRegionMap.h"
#include "GameFlow/GameContents/GsContentsMode.h"

#include "Map/WorldMap/GsWorldMapDefine.h"
#include "Map/WorldMap/GsRegionGroupInfoBase.h"

#include "Message/GsMessageContents.h"
#include "Message/MessageParam/GsUIMessageParam.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Helper/GsUIHelper.h"
#include "UI/UIContent/Window/ContinentalMap/GsUIContinentalMap.h"
#include "UI/UIContent/Window/ContinentalMap/GsUIContinentalDungeonSide.h"
#include "UI/UIContent/Window/ContinentalMap/GsUIContinentalName.h"
#include "UI/UIContent/Window/ContinentalMap/GsUIContinentalSpaceCrackList.h"
#include "UI/UIContent/Window/ContinentalMap/GsUIRegionItem.h"

#include "Core/Public/Delegates/Delegate.h"

#include "UTIL/GsTableUtil.h"

#include "SlateCore/Public/Layout/Geometry.h"

#include "UMG/Public/Blueprint/UserWidget.h"
#include "UMG/Public/Components/WidgetSwitcher.h"
#include "Classes/GsMessageHandlerOneParam.h"
#include "PanelWidget.h"


void UGsUIWindowContinentalMap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_helpContentsKey = EGsHelpContents::WorldMap;

	_btnClose->OnClicked.AddDynamic(this, &UGsUIWindowContinentalMap::OnClickCloseAllStack);
	_btnBack->OnClicked.AddDynamic(this, &UGsUIWindowContinentalMap::OnClickBack);
	_btnAutoMovePos->OnClicked.AddDynamic(this, &UGsUIWindowContinentalMap::OnClickAutoMovePos);
	_btnMyPos->OnClicked.AddDynamic(this, &UGsUIWindowContinentalMap::OnClickMyPos);

	_btnMapSelectLeft->OnClicked.AddDynamic(this, &UGsUIWindowContinentalMap::OnClickMapSelectLeft);
	_btnMapSelectRight->OnClicked.AddDynamic(this, &UGsUIWindowContinentalMap::OnClickMapSelectRight);

	_toggleBtnSpaceCrack->OnClicked.AddDynamic(this, &UGsUIWindowContinentalMap::OnClickSpaceCrack);
}

void UGsUIWindowContinentalMap::NativeConstruct()
{
	Super::NativeConstruct();

	RegisterMessage();
	ReqInvasionInfo(true);

	_continentalMap->_callbackSelectList = [this](int continentalId)
	{
		ChangeMapProc(continentalId);
	};

	int currentId =
		GSWorldMap()->FindContinentalTblIdByCurrentRegionId();
	

	ChangeMapProc(currentId);

	SetInvasionInfo();
	ShowDungeonSide(false);
}

void UGsUIWindowContinentalMap::NativeDestruct()
{
	_continentalMap->_callbackSelectList = nullptr;

	RemoveMessage();
	ReqInvasionInfo(false);

	Super::NativeDestruct();
}
// 백버튼 클릭
bool UGsUIWindowContinentalMap::OnBack()
{
	// 이전 지역맵으로 이동
	if (GMode())
	{
		if (FGsGameFlowGame* gameFlow = GMode()->GetGameFlow())
		{
			if (FGsContentsManagerGame* contents = gameFlow->GetContentsManagerGame())
			{
				if (FGsGameStateRegionMap* regionMap =
					contents->GetContentsStateObject<FGsGameStateRegionMap>(FGsContentsMode::InGame::ContentsRegionMap))
				{
					regionMap->SetMapChangeType(EGsWorldMapChangeType::TO_REGIONMAP_OLD_REGIONMAP);
				}
			}
		}
	}
	
	return Super::OnBack();
}

bool UGsUIWindowContinentalMap::SyncContentState()
{
	if (false == IsSameContentState(FGsContentsMode::InGame::ContentsContinentalMap))
	{
		GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_CONTINENTALMAP);
		return true;
	}

	return false;
}

void UGsUIWindowContinentalMap::ReqInvasionInfo(bool reqStart)
{
	if(UGsInterServerManager* interServerManager = GSInterServer())
	{ 
		interServerManager->ReqInvasionInfoCheck(reqStart);
	}
}

void UGsUIWindowContinentalMap::RegisterMessage()
{
	if (auto msgMgr = GMessage())
	{
		_msgHandleUIList.Emplace(msgMgr->GetUI().AddUObject(MessageUI::INVADE_INVADER_COUNT,
			this, &UGsUIWindowContinentalMap::OnUpdateInvasionInfo));
	}
}

void UGsUIWindowContinentalMap::RemoveMessage()
{
	if (auto msgMgr = GMessage())
	{
		for (MsgUIHandle& handleUI : _msgHandleUIList)
		{
			msgMgr->GetUI().Remove(handleUI);
		}
	}
	_msgHandleUIList.Empty();
}

// 목적지 클릭
void UGsUIWindowContinentalMap::OnClickAutoMovePos()
{
	// 목적지 표시 가능하지 않으면 안함	
	if (false == GSWorldMap()->IsUseableShowAutoMovePos())
	{
		FText findText;
		FText::FindText(TEXT("WorldMapText"), TEXT("ErrorNotMoving"), findText);
		FGsUIHelper::TrayMessageTicker(findText);
		return;
	}

	// 현재 대륙-> 지역맵 갈땐 기존 스택 날리고 간다
	// 지역-대륙-지역 스택이면 백버튼시 지역이 남고 미니맵 동작안하는 이슈 때문에
	GUI()->CloseAllStack(nullptr, false);

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
	// 지역맵으로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);
}
// 내위치 클릭
void UGsUIWindowContinentalMap::OnClickMyPos()
{
	// 현재 대륙-> 지역맵 갈땐 기존 스택 날리고 간다
	// 지역-대륙-지역 스택이면 백버튼시 지역이 남고 미니맵 동작안하는 이슈 때문에
	GUI()->CloseAllStack(nullptr, false);

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
	// 지역맵으로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_WINDOW_REGIONMAP);
}

// 상단 이름 변경
void UGsUIWindowContinentalMap::UpdateContinentalName(int In_continentalTblId)
{
	const FGsSchemaContinentalMapData* continentalMapData =
		UGsTableUtil::FindRowById<UGsTableContinentalMapData, FGsSchemaContinentalMapData>(In_continentalTblId);

	if (continentalMapData == nullptr)
	{
		return;
	}

	_textContinentalMapName = continentalMapData->mapStringId;

	_nameBP->SetText(continentalMapData->mapStringId, continentalMapData->timeZoneStringId);
}

// 던전 우측 켤지 말지
void UGsUIWindowContinentalMap::ShowDungeonSide(bool In_isShow)
{
	_dungeonSide->SetVisibility((In_isShow == true) ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

// 던전 입구 이름 세팅
void UGsUIWindowContinentalMap::SetDungeonEntranceName(const FText& In_name)
{
	_dungeonSide->SetData(In_name);
}

// 지역 아이템 클릭
void UGsUIWindowContinentalMap::MoveCenterRegionItem(UUserWidget* In_widget)
{
	_continentalMap->MoveCenterRegionItem(In_widget);
}
// 던전 side가 보이는가
bool UGsUIWindowContinentalMap::GetDungeonSideShow()
{
	ESlateVisibility nowVisible = _dungeonSide->GetVisibility();
	return (nowVisible == ESlateVisibility::Hidden) ? false : true;
}

void UGsUIWindowContinentalMap::OnUpdateInvasionInfo(const IGsMessageParam* InParam)
{
	if (const auto param = InParam ? InParam->Cast<const FGsUIMsgParamInvadeMapInfo>() : nullptr)
	{
		_continentalMap->UpdateInvaderCount(param->_updateMapList);
		if (param->_mapUpdateCount == 1)
		{
			SetInvasionInfo();
		}
	}
}

void UGsUIWindowContinentalMap::SetInvasionInfo()
{
	auto remainTime = GSInterServer()->GetInvasionRemainStayTime();
	_invadeInfoPanel->SetVisibility(remainTime.IsZero() ? ESlateVisibility::Collapsed : ESlateVisibility::SelfHitTestInvisible);
	if (!remainTime.IsZero())
	{
		//SetInvasionRemainTime(remainTime);

		if (auto worldInfo = GSInterServer()->GetAirRaidWorld().Pin())
		{
			FText formatText;
			FText::FindText(TEXT("InvadeText"), TEXT("InvadeTargerWorldInfo"), formatText);

			_invasionWorld = FText::Format(formatText, worldInfo->GetWorldName());
		}
	}
	else
	{
		_invasionWorld = FText::GetEmpty();
		_invasionTime = FText::GetEmpty();
	}
}

void UGsUIWindowContinentalMap::SetInvasionRemainTime(FTimespan remainTime)
{
	if (!remainTime.IsZero())
	{
		//FText InvadeAllowTimeText;
		FText InvadeAllowTime_Hour;
		FText InvadeAllowTime;
		FText InvadeAllowTime_Second;
		//FText::FindText(TEXT("InvadeText"), TEXT("InvadeAllowTimeText"), InvadeAllowTimeText);
		FText::FindText(TEXT("InvadeText"), TEXT("InvadeAllowTime_Hour"), InvadeAllowTime_Hour);
		FText::FindText(TEXT("InvadeText"), TEXT("InvadeAllowTime"), InvadeAllowTime);
		FText::FindText(TEXT("InvadeText"), TEXT("InvadeAllowTime_Second"), InvadeAllowTime_Second);

		int hour = remainTime.GetTotalHours();
		int min = remainTime.GetMinutes();
		int sec = remainTime.GetSeconds();

		if (hour > 0)
		{
			_invasionTime = FText::Format(InvadeAllowTime_Hour, {hour, min, sec});
		}
		else if (min > 0)
		{
			_invasionTime = FText::Format(InvadeAllowTime, { min, sec });
		}
		else if (sec > 0)
		{
			_invasionTime = FText::Format(InvadeAllowTime_Second, { sec });
		}
	}
	else
	{
		_invasionTime = FText::GetEmpty();
		_invasionWorld = FText::GetEmpty();
	}
}
bool UGsUIWindowContinentalMap::IsCanMoveCenter(UUserWidget* In_widget)
{
	return _continentalMap->IsCanMoveCenter(In_widget);
}
void UGsUIWindowContinentalMap::OnClickMapSelectLeft()
{
	_continentalMap->TrySelectNextMap(-1);
}

void UGsUIWindowContinentalMap::OnClickMapSelectRight()
{
	_continentalMap->TrySelectNextMap(1);
}

void UGsUIWindowContinentalMap::OnClickSpaceCrack()
{
	ESlateVisibility nowVisibility = _spaceCrackList->GetVisibility();

	// toggle space crack list visibility
	ESlateVisibility newVisibility = (nowVisibility == ESlateVisibility::Hidden) ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden;

	if (newVisibility == ESlateVisibility::SelfHitTestInvisible)
	{
		int currentContinentalMapId = _continentalMap->GetCurrentContinentalId();
		GSWorldMap()->SetCurrentSpaceCrackListMapId(currentContinentalMapId);

		_spaceCrackList->SetListData();
	}

	// set visibility
	_spaceCrackList->SetVisibility(newVisibility);

	

	// switch btn toggle image 
	int switcherIndex = (newVisibility == ESlateVisibility::Hidden) ? 0 : 1;
	_switcherSpaceCrackBtn->SetActiveWidgetIndex(switcherIndex);
}

void UGsUIWindowContinentalMap::InitializeSpaceCrackInfo()
{
	// hide 
	ESlateVisibility newVisibility = ESlateVisibility::Hidden;
	_spaceCrackList->SetVisibility(newVisibility);

	int switcherIndex = (newVisibility == ESlateVisibility::Hidden) ? 0 : 1;
	_switcherSpaceCrackBtn->SetActiveWidgetIndex(switcherIndex);
}

void UGsUIWindowContinentalMap::MakeCenterRegionMapItem(int In_regionMapId, bool In_isTargetShowed)
{
	_continentalMap->MakeCenterRegionMapItem(In_regionMapId , In_isTargetShowed);
}

void UGsUIWindowContinentalMap::SetSelectDungeonItem(UGsUIRegionItem* In_item)
{
	_continentalMap->SetSelectDungeonItem(In_item);
}

void UGsUIWindowContinentalMap::UpdateSelectButtonState()
{
	bool isLeftButtonShow = _continentalMap->IsShowMapChangeButton(true);
	bool isRightButtonShow = _continentalMap->IsShowMapChangeButton(false);

	ESlateVisibility leftButtonVisibility = (isLeftButtonShow == false)? ESlateVisibility::Hidden: ESlateVisibility::Visible;
	ESlateVisibility rightButtonVisibility = (isRightButtonShow == false) ? ESlateVisibility::Hidden : ESlateVisibility::Visible;

	_btnMapSelectLeft->SetVisibility(leftButtonVisibility);
	_btnMapSelectRight->SetVisibility(rightButtonVisibility);
}

void UGsUIWindowContinentalMap::ChangeMapProc(int In_continentalTblId)
{
	UpdateContinentalName(In_continentalTblId);
	InitializeSpaceCrackInfo();
	UpdateSelectButtonState();

	// 시공의 틈새 버튼 보일지 처리(현재 대륙에 시공의 틈새가 있으면 표시, 없으면 표시 안함)
	UpdateSpaceCrackButton(In_continentalTblId);
}
// 시공의 틈새 버튼 보일지 처리(현재 대륙에 시공의 틈새가 있으면 표시, 없으면 표시 안함)
void UGsUIWindowContinentalMap::UpdateSpaceCrackButton(int In_continentalTblId)
{
	FGsRegionGroupInfoBase* findInfo = GSWorldMap()->GetSpaceCrackGroupData(In_continentalTblId);
	// 시공의 틈새 정보가 없으면 버튼 표시 안함
	bool isExistSpaceCrackInfo = (findInfo == nullptr)? false: true;

	ESlateVisibility newVisibility = (isExistSpaceCrackInfo == true) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed;
	_toggleBtnSpaceCrack->SetVisibility(newVisibility);
}