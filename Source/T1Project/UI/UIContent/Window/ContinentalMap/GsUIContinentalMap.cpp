#include "GsUIContinentalMap.h"

#include "UI/UIControlLib/Control/GsDynamicPanelSlotHelper.h"
#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "UI/UIContent/Common/Icon/GsUIIconBase.h"
#include "UI/UIContent/Factory/GsIconFactory.h"
#include "UI/UIContent/Window/ContinentalMap/GsUIContinentalListItem.h"
#include "UI/UIContent/Window/ContinentalMap/GsUIContinentalBP.h"
#include "UI/UIContent/Window/ContinentalMap/GsUIRegionItem.h"
#include "UI/UIContent/Window/ContinentalMap/GsUIContinentalBPDragZone.h"
#include "UI/UIContent/Window/ContinentalMap/GsUIContinentalName.h"
#include "UI/UIContent/Window/GsUIWindowContinentalMap.h"

#include "UI/UILib/Manager/GsWidgetPoolManager.h"
#include "UI/UILib/Container/GsWidgetPool.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "Management/ScopeGlobal/GsPathManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/ScopeGlobal/GsLevelManager.h"
#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsGameObjectManager.h"
#include "Management/ScopeGame/GsPartyManager.h"

#include "Party/GsPartyMemberInfo.h"

#include "Data/GsDataContainManager.h"
#include "Data/GsGlobalConstant.h"

#include "DataSchema/Map/WorldMap/GsSchemaContinentalMapData.h"
#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/Area/GsSchemaAreaData.h"
#include "DataSchema/Icon/GsSchemaContinentalMapIconData.h"

#include "DataCenter/Public/Shared/Shared/SharedInclude/SharedTypes.h"

#include "UTIL/GsTableUtil.h"

#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/CanvasPanel.h"
#include "UMG/Public/Components/CanvasPanelSlot.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/HorizontalBoxSlot.h"
#include "UMG/Public/Components/PanelSlot.h"

#include "UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "UMG/Public/Blueprint/SlateBlueprintLibrary.h"

#include "UMG/Public/Animation/WidgetAnimation.h"

#include "Core/Public/Delegates/Delegate.h"

#include "Engine/Classes/Engine/GameViewportClient.h"
#include "Engine/Classes/Curves/CurveFloat.h"

#include "T1Project.h"

#include "SlateCore/Public/Layout/Geometry.h"


void UGsUIContinentalMap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

}

void UGsUIContinentalMap::NativeConstruct()
{
	Super::NativeConstruct();

	// 최근에 본 지역맵의 상위 대륙맵 인덱스 구하기
	int index = GSWorldMap()->FindContinentalIndexByCurrentRegionId();

	SelectContinentalMap(index);

	_isMapMoveFinish = true;

	TryMakeCenterRegionItem();

	_currentSelDungeonItem = nullptr;
}

void UGsUIContinentalMap::MakeCenterCurrentRegionItem()
{
	// move to current show region map item
	int currentShowRegionMapId = GSWorldMap()->GetShowRegionMapId();

	// check dungeon 
	int dungeonGroupId = GSWorldMap()->FindDungeonGroupTblIdByMapId(currentShowRegionMapId);

	int showItemId = currentShowRegionMapId;
	// -1: not dungeon
	// else: dungeon
	if (dungeonGroupId != -1)
	{
		showItemId = dungeonGroupId;
	}
	MakeCenterRegionMapItem(showItemId, false);
}

void UGsUIContinentalMap::TryMakeCenterRegionItem()
{
	if (_loadedContinentalBP == nullptr ||
		_isInitSize == false ||
		_isBPInitSize == false)
	{
		_isDirtyFirstShowRegionItemCenter = true;
		return;
	}

	MakeCenterCurrentRegionItem();
}

void UGsUIContinentalMap::MakeCenterRegionMapItem(int In_regionMapId, bool In_isTargetShowed)
{
	if (_loadedContinentalBP == nullptr)
	{
		return;
	}

	UGsUIRegionItem* findItem = _loadedContinentalBP->FindRegionItem(In_regionMapId);

	if (findItem == nullptr)
	{
		return;
	}

	MoveCenterRegionItem(findItem, In_isTargetShowed);
	// https://jira.com2us.com/jira/browse/C2URWQ-4984
	// 던전 메뉴 보여줄지
	if (GSWorldMap()->GetIsShowDungeonMenu() == true)
	{
		SetSelectDungeonItem(findItem);
	}
}
void UGsUIContinentalMap::NativeDestruct()
{
	

	// 드래그 이벤트 해제
	RemoveDragEvent();

	if (_loadedContinentalBP != nullptr)
	{
		ReleaseContinentalBP(_loadedContinentalBP);
		_loadedContinentalBP = nullptr;
	}
	if (_beforeContinentalBP != nullptr)
	{
		ReleaseContinentalBP(_beforeContinentalBP);
		_beforeContinentalBP = nullptr;
	}

	if (_loadedContinentalLoopBP != nullptr)
	{
		ReleaseContinentalBP(_loadedContinentalLoopBP);
		_loadedContinentalLoopBP = nullptr;
	}

	ReleaseAllIcon();

	Super::NativeDestruct();
}

void UGsUIContinentalMap::BeginDestroy()
{

	Super::BeginDestroy();
}


// select map	
void UGsUIContinentalMap::SelectContinentalMap(int32 In_index)
{
	_currentContinentalId =
		GSWorldMap()->GetContinentalMapId(In_index);

	_currentMapSelectIndex = In_index;

	// 대륙 테이블 접근
	const FGsSchemaContinentalMapData* findData =
		GSWorldMap()->FindContinentalMapDataByIndex(In_index);

	if (findData == nullptr)
	{
		return;
	}

	// 이전아이콘  삭제
	ReleaseAllIcon();

	// 드래그 이벤트 해제
	RemoveDragEvent();

	if (_loadedContinentalBP != nullptr)
	{
		StartDisappearBP(_loadedContinentalBP);
	}
	UUserWidget* makeWidget = MakeContinentalBP(findData->continentalBP);
	_loadedContinentalBP = Cast<UGsUIContinentalBP>(makeWidget);

	// 드래그 이벤트 연결
	AddDragEvent();

	if (_continentalBPRoot != nullptr &&
		_loadedContinentalBP != nullptr)
	{
		UCanvasPanelSlot* canvasPanelSlot = _continentalBPRoot->AddChildToCanvas(_loadedContinentalBP);
		if (nullptr == canvasPanelSlot)
		{
			return;
		}

		canvasPanelSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		canvasPanelSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));

		// renter trnaslation은 0값으로 초기화
		_loadedContinentalBP->SetRenderTranslation(FVector2D::ZeroVector);

		StartAppearBP();
	}

	// 루프 로드
	// 기존꺼 삭제
	if (_loadedContinentalLoopBP != nullptr)
	{
		ReleaseContinentalBP(_loadedContinentalLoopBP);
		_loadedContinentalLoopBP = nullptr;
	}

	// 테이블에 값이 있다면
	if (findData->continentalLoopBP.IsValid() == true)
	{
		// 로드
		_loadedContinentalLoopBP = MakeContinentalBP(findData->continentalLoopBP);
	}

	if (_continentalLoopBPRoot != nullptr &&
		_loadedContinentalLoopBP != nullptr)
	{
		UCanvasPanelSlot* canvasPanelSlot = _continentalLoopBPRoot->AddChildToCanvas(_loadedContinentalLoopBP);
		if (nullptr == canvasPanelSlot)
		{
			return;
		}

		canvasPanelSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
		canvasPanelSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));
	}



	_isBPInitSize = false;

	// 현재 맵 아이콘 로드
	LoadAllIcon();

	if (_callbackSelectList != nullptr)
	{
		_callbackSelectList(_currentContinentalId);
	}

	// 이동 초기화
	_isMapMoveFinish = true;
	// side 끄기

	TWeakObjectPtr<UGsUIWidgetBase> wigdet = GUI()->GetWidgetByKey(TEXT("WindowContinentalMap"));

	if (wigdet.IsValid() == false)
	{
		return;
	}
	TWeakObjectPtr<UGsUIWindowContinentalMap> continentalWindow = Cast<UGsUIWindowContinentalMap>(wigdet);

	continentalWindow->ShowDungeonSide(false);

	if (_currentSelDungeonItem != nullptr)
	{
		_currentSelDungeonItem->ShowDungeonMenu(false);
		_currentSelDungeonItem = nullptr;
	}
}

// 대륙 BP 만들기
UUserWidget* UGsUIContinentalMap::MakeContinentalBP(const FSoftObjectPath& In_path)
{
	UClass* widgetClass = nullptr;
	if (UGsPathManager* pathMgr = GPath())
	{
		widgetClass = pathMgr->GetClassByObjectPath(In_path);
	}

	if (nullptr == widgetClass)
	{
		return nullptr;
	}

	UUserWidget* widget = Claim(widgetClass);
	if (widget)
	{
		widget->AddToViewport();
	}

	return widget;
}
UUserWidget* UGsUIContinentalMap::Claim(UClass* In_class)
{
	if (false == _continentalBPPoolManager.IsValid())
	{
		_continentalBPPoolManager = UGsBlueprintFunctionLibraryUI::GetWidgetPoolManager();
		if (false == _continentalBPPoolManager.IsValid())
		{
			return nullptr;
		}
	}

	UGsWidgetPool* pool = _continentalBPPoolManager->FindPool(In_class);
	if (nullptr == pool)
	{
		pool = _continentalBPPoolManager->CreatePool(In_class);
		if (nullptr == pool)
		{
			return nullptr;
		}
	}

	return pool->Claim<UUserWidget>();
}

// 대륙 BP 해제
void UGsUIContinentalMap::ReleaseContinentalBP(UUserWidget* In_BP)
{
	if (In_BP == nullptr)
	{
		return;
	}

	if (_continentalBPPoolManager.IsValid())
	{
		In_BP->RemoveFromParent();
		_continentalBPPoolManager->ReleaseWidget(In_BP->GetClass(), In_BP);
	}
}

void UGsUIContinentalMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	InitRootCanvasSize();
	InitBPSize();

	UpdateMapMove(InDeltaTime);

	CheckDirtyCenterRegionItem();
}
void UGsUIContinentalMap::CheckDirtyCenterRegionItem()
{
	if (_isDirtyFirstShowRegionItemCenter == false)
	{
		return;
	}
	if (_loadedContinentalBP == nullptr ||
		_isInitSize == false ||
		_isBPInitSize == false)
	{
		return;
	}

	MakeCenterCurrentRegionItem();
	_isDirtyFirstShowRegionItemCenter = false;
}
// root 캔버스 크기 가져오기
void UGsUIContinentalMap::InitRootCanvasSize()
{
	if (_continentalBPRoot == nullptr)
	{
		return;
	}
	const FGeometry panelGeometry = _continentalBPRoot->GetCachedGeometry();
	FVector2D getSize = panelGeometry.GetLocalSize();
	// 세팅 전
	if (true == getSize.IsZero())
	{
		return;
	}
	// https://jira.com2us.com/jira/browse/C2URWQ-4716
	// 갤럭시 폴더 z 드래그 안되는 부분 의심 되는곳 수정
	// root 크기가 바뀌면 크기 재갱신하기
	if (_isInitSize == true &&
		_BPRootCanvasSizes.Equals(getSize) == true)
	{
		return;
	}

	_isInitSize = true;
	_isBPInitSize = false;

	_BPRootCanvasSizes = getSize;
}


// bp 사이즈 구하기
void UGsUIContinentalMap::InitBPSize()
{
	if (_isBPInitSize == true ||
		_loadedContinentalBP == nullptr)
	{
		return;
	}
	const FGeometry panelGeometry = _loadedContinentalBP->GetDragZone()->GetSizeCanvas()->GetCachedGeometry();
	FVector2D getSize = panelGeometry.GetLocalSize();
	// 세팅 전
	if (true == getSize.IsZero())
	{
		return;
	}

	_isBPInitSize = true;

	_BPSizes = getSize;

	_RootBPDiffSize =  _BPRootCanvasSizes - _BPSizes;

	// diff 값이 0보다 더 크면 diff가 max 그외는 diff가 min
	if (_RootBPDiffSize.X >= 0.0f)
	{
		_moveMax.X = _RootBPDiffSize.X;
		_moveMin.X = 0.0f;
	}
	else
	{
		_moveMax.X = 0.0f;
		_moveMin.X = _RootBPDiffSize.X;
	}

	if (_RootBPDiffSize.Y >= 0.0f)
	{
		_moveMax.Y = _RootBPDiffSize.Y;
		_moveMin.Y = 0.0f;
	}
	else
	{
		_moveMax.Y = 0.0f;
		_moveMin.Y = _RootBPDiffSize.Y;
	}
}


// bp 위치 괜찮은지(canvas 내인지)
bool UGsUIContinentalMap::IsBPPosOK(const FVector2D In_pos, FVector2D& Out_pos)
{

	// 신규 위치(x, y)가 0보다 크거나
	// diff size 보다 더 작거나
	// diff 사이즈가 0보다 크거나 같음(canvas 사이즈보다 bp가 작은 경우)

	// https://jira.com2us.com/jira/browse/C2URWQ-4716
	// 갤럭시 폴더 z 드래그 안되는 부분 수정
	// 지도 y가 canvas 보다 작을때 드래그 안됨
	// 캔버스가 x, y 둘다 클때 드래그 안되게 수정
	if (_RootBPDiffSize.X >= 0 &&
		_RootBPDiffSize.Y >= 0)
	{
		return false;
	}
	// diff 값이 0보다 더 크면 diff가 max 그외는 diff가 min
	

	Out_pos.X = FMath::Clamp<float>(In_pos.X, _moveMin.X, _moveMax.X);
	Out_pos.Y = FMath::Clamp<float>(In_pos.Y, _moveMin.Y, _moveMax.Y);

	return true;
}

void UGsUIContinentalMap::UpdateInvaderCount(const TArray<MapId>& updateMapList)
{
	if (_loadedContinentalBP == nullptr)
	{
		return;
	}

	for (auto id : updateMapList)
	{
		if (auto findItem = _loadedContinentalBP->FindRegionItem(id))
		{
			findItem->UpdateInvaderData();
		}
	}
}

void UGsUIContinentalMap::UpdatePortalCount(MapId id, int32 count, int32 maxCount)
{
	if (auto findItem = _loadedContinentalBP->FindRegionItem(id))
	{
		findItem->UpdateInvaderData();
	}
}

// 대륙 bp 사라지는 연출 시작
void UGsUIContinentalMap::StartDisappearBP(UGsUIContinentalBP* In_BP)
{
	if (In_BP == nullptr||
		_disappearBPRoot == nullptr ||
		_disappearAnimation == nullptr)
	{
		return;
	}

	if (_beforeContinentalBP != nullptr)
	{
		FinishDiappearBP();
	}

	_beforeContinentalBP = In_BP;

	// 부모 때고
	In_BP->RemoveFromParent();
	// 연출 캔버스에 붙인다

	UCanvasPanelSlot* canvasPanelSlot = _disappearBPRoot->AddChildToCanvas(In_BP);
	if (nullptr == canvasPanelSlot)
	{
		return;
	}

	canvasPanelSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
	canvasPanelSlot->SetOffsets(FMargin(0.f, 0.f, 0.f, 0.f));	

	// 애니 시작
	PlayAnimation(_disappearAnimation);

	FScriptDelegate finishDelegate;
	finishDelegate.BindUFunction(this, "FinishDiappearBP");

	BindToAnimationFinished(_disappearAnimation, FWidgetAnimationDynamicEvent(finishDelegate));
}

// 대륙 bp 생기는 연출 시작
void UGsUIContinentalMap::StartAppearBP()
{
	// 애니 시작
	PlayAnimation(_appearAnimation);
}

// 연출 끝
void UGsUIContinentalMap::FinishDiappearBP()
{
	FScriptDelegate finishDelegate;
	finishDelegate.BindUFunction(this, "FinishDiappearBP");

	UnbindFromAnimationFinished(_disappearAnimation, FWidgetAnimationDynamicEvent(finishDelegate));

	ReleaseContinentalBP(_beforeContinentalBP);
	_beforeContinentalBP = nullptr;
}
// 아이콘 만들기
UGsUIIconBase* UGsUIContinentalMap::CreateMapIcon(EGsContinentalMapIconType In_iconType)
{
	const FGsSchemaContinentalMapIconData* findIconTbl =
		UGsTableUtil::FindRowById<UGsTableContinentalMapIconData, FGsSchemaContinentalMapIconData>(In_iconType);

	if (findIconTbl == nullptr)
	{
		return nullptr;
	}
	UGsUIIconBase* iconWidget = FGsIconFactory::CreateIcon(findIconTbl->_iconShapeType);
	if (iconWidget)
	{
		if (findIconTbl->_iconImagePath.IsValid())
		{
			iconWidget->SetIconImage(findIconTbl->_iconImagePath);
		}

		iconWidget->SetIconImageColor(findIconTbl->_iconColor);
	}

	return iconWidget;

}
// icon 만들기
void UGsUIContinentalMap::SpawnIcon(int64 In_id, EGsContinentalMapIconType In_iconType, int In_mapId)
{
	const FGsSchemaMapData* mapTable = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(In_mapId);
	if (nullptr == mapTable 
	|| nullptr == mapTable->areaId.GetRow())
	{
		return;
	}

	const FGsSchemaAreaData* findAreaTbl = mapTable->areaId.GetRow();		
	
	// 다른 대륙임
	if (findAreaTbl->continentId != _currentContinentalId)
	{
		return;
	}

	if (_loadedContinentalBP == nullptr)
	{
		return;
	}

	UGsUIIconBase* findIcon = _mapIcons.FindRef(In_id);
	// 이미 있으면 삭제
	if (findIcon != nullptr)
	{
		DespawnIcon(In_id);
	}
	UGsUIIconBase* makeIcon = CreateMapIcon(In_iconType);
	if (nullptr == makeIcon)
	{
		return;
	}

	_arrayIcons.Add(makeIcon);
	_mapIcons.Add(In_id, makeIcon);

	// 그룹정보도 참조
	// https://jira.com2us.com/jira/browse/C2URWQ-7502
	UGsUIRegionItem* findItem = _loadedContinentalBP->FindRegionItemMapIdAndGroupIds(In_mapId);
	if (findItem == nullptr)
	{
		return;
	}

	UHorizontalBox* iconRoot = findItem->GetIconRoot();

	if (nullptr == iconRoot)
	{
		return;
	}
	UHorizontalBoxSlot* slot = iconRoot->AddChildToHorizontalBox(makeIcon);
	if (slot == nullptr)
	{
		return;
	}
	FSlateChildSize sizeType;
	sizeType.SizeRule = ESlateSizeRule::Fill;
	slot->SetSize(sizeType);
	slot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
}
// icon 삭제
void UGsUIContinentalMap::DespawnIcon(int64 In_id)
{
	UGsUIIconBase* findIcon = _mapIcons.FindRef(In_id);
	if (findIcon == nullptr)
	{
		return;
	}
	FGsIconFactory::ReleaseIcon(findIcon);
	_arrayIcons.RemoveSwap(findIcon);
	findIcon = nullptr;

	_mapIcons.Remove(In_id);
}
// 정렬용
struct FGsContinentalPartyIconInfo
{
	int64 _gameId;
	EGsContinentalMapIconType _iconType;
	int _mapId;
	int _slotId;

	FGsContinentalPartyIconInfo(int64 In_gameId, EGsContinentalMapIconType In_iconType,
		int In_mapId, int In_slotId) :
		_gameId(In_gameId), _iconType(In_iconType), _mapId(In_mapId), _slotId(In_slotId)
	{
	}
};
// 현재맵 모든 아이콘 로드
void UGsUIContinentalMap::LoadAllIcon()
{
	// 나
	UGsGameObjectBase* local = GSGameObject()->FindObject(EGsGameObjectType::LocalPlayer);
	if (local == nullptr)
	{
		return;
	}
	
	int currentLevel = GLevel()->GetCurrentLevelId();

	int64 localGameId = local->GetGameId();
	// 로컬 icon
	SpawnIcon(localGameId, EGsContinentalMapIconType::LocalPlayer, currentLevel);

	// 파티중인가
	if (GSParty()->GetIsInParty() == false)
	{
		return;
	}
	int currentChannel = GGameData()->GetCurrentChannel();
	// 파티원들
	const TArray<FGsPartyMemberInfo* > arrPartyMemberInfo = GSParty()->GetAllPartyMemberInfo();

	TArray<FGsContinentalPartyIconInfo> arraySortIconInfo;

	// check party member world id
	WorldId localNowMapWorldId = GGameData()->GetLoadWorldId();

	for (FGsPartyMemberInfo* info: arrPartyMemberInfo)
	{
		if (info == nullptr)
		{
			continue;
		}
		// 나는 제외
		if (info->GetGameId() == localGameId)
		{
			continue;
		}

		// check world id
		WorldId memberWorldId = info->GetWorldId();
		if (memberWorldId != 0 &&
			localNowMapWorldId != memberWorldId)
		{
			continue;
		}


		EGsContinentalMapIconType iconType = EGsContinentalMapIconType::None;

		bool isSameChannelWithLocal = (currentChannel == info->GetChannelId());
		switch (info->GetPartySlotId())
		{
		case 0:
			iconType =  (isSameChannelWithLocal == true)? EGsContinentalMapIconType::Party1 :
				EGsContinentalMapIconType::Party1_OtherChannel;
			break;
		case 1:
			iconType = (isSameChannelWithLocal == true) ? EGsContinentalMapIconType::Party2 :
				EGsContinentalMapIconType::Party2_OtherChannel;
			break;
		case 2:
			iconType = (isSameChannelWithLocal == true) ? EGsContinentalMapIconType::Party3 :
				EGsContinentalMapIconType::Party3_OtherChannel;
			break;
		case 3:
			iconType = (isSameChannelWithLocal == true) ? EGsContinentalMapIconType::Party4 :
				EGsContinentalMapIconType::Party4_OtherChannel;
			break;
		}
		FGsContinentalPartyIconInfo sortInfo(
			info->GetGameId(), iconType, 
			info->GetMapId(), info->GetPartySlotId());
		arraySortIconInfo.Add(sortInfo);		
	}
	// 슬롯번호순 정렬
	arraySortIconInfo.Sort([](const FGsContinentalPartyIconInfo& lhs, const FGsContinentalPartyIconInfo& rhs)
	{
		return lhs._slotId < rhs._slotId;
	});

	for (FGsContinentalPartyIconInfo& sortInfo: arraySortIconInfo)
	{
		SpawnIcon(sortInfo._gameId, sortInfo._iconType, sortInfo._mapId);
	}
}
// 현재맵 모든 아이콘 해제
void UGsUIContinentalMap::ReleaseAllIcon()
{
	for (UGsUIIconBase* icon: _arrayIcons)
	{
		if (icon == nullptr)
		{
			continue;
		}
		FGsIconFactory::ReleaseIcon(icon);
	}

	_mapIcons.Empty();
	_arrayIcons.Empty();
}

void UGsUIContinentalMap::AddDragEvent()
{
	if (_loadedContinentalBP == nullptr)
	{
		return;
	}


	_loadedContinentalBP->GetDragZone()->OnBPTouchMoved.BindUObject(this, &UGsUIContinentalMap::OnBPTouchMoved);
	_loadedContinentalBP->GetDragZone()->OnBPTouchStarted.BindUObject(this, &UGsUIContinentalMap::OnBPTouchStarted);
	_loadedContinentalBP->GetDragZone()->OnBPTouchEnd.BindUObject(this, &UGsUIContinentalMap::OnBPTouchEnded);
}

void UGsUIContinentalMap::RemoveDragEvent()
{
	if (_loadedContinentalBP == nullptr)
	{
		return;
	}

	_loadedContinentalBP->GetDragZone()->OnBPTouchMoved = nullptr;
	_loadedContinentalBP->GetDragZone()->OnBPTouchStarted = nullptr;
}


void UGsUIContinentalMap::OnBPTouchMoved(const FVector2D& In_pos)
{
	if (_isInitSize == true &&
		_isBPInitSize == true &&
		_loadedContinentalBP != nullptr &&
		_isTouchPress == true)
	{

		if (In_pos.Equals(_oldMousePos) == false)
		{
			FVector2D diffPos = In_pos - _oldMousePos;
			// 저장
			_oldMousePos = In_pos;

			FVector2D currUIPos = _loadedContinentalBP->RenderTransform.Translation;

			FVector2D newUIPos = currUIPos + diffPos;
			FVector2D calcedUIPos = FVector2D::ZeroVector;
			if (IsBPPosOK(newUIPos, calcedUIPos))
			{
				_loadedContinentalBP->SetRenderTranslation(calcedUIPos);
			}
		}

	}
}


void UGsUIContinentalMap::OnBPTouchStarted(const FVector2D& In_pos)
{
	_oldMousePos = In_pos;
	_isTouchPress = true;
}
void UGsUIContinentalMap::OnBPTouchEnded(const FVector2D& In_pos)
{	
	_isTouchPress = false;
}

// 지역 아이템 클릭
void UGsUIContinentalMap::MoveCenterRegionItem(UUserWidget* In_widget, bool In_isTargetShowed)
{

	FVector2D movePos;
	if (false == GetContinentalBPMovePos(In_widget, movePos, In_isTargetShowed))
	{
		return;
	}

	_isMapMoveFinish = false;
	_mapMoveProgressRate = 0.0f;

	_mapMovePosTo = movePos;
	_mapMovePosFrom = _loadedContinentalBP->RenderTransform.Translation;
}
bool UGsUIContinentalMap::GetContinentalBPMovePos(UUserWidget* In_widget, FVector2D& Out_movePos, bool In_isTargetShowed)
{
	// 로드된게 없거나
	// 로드된 bp 사이즈가 기본 사이즈보다 작으면 안함
	if (_loadedContinentalBP == nullptr ||
		(_RootBPDiffSize.X >= 0 &&
		_RootBPDiffSize.Y >= 0))
	{
		return false;
	}
	FVector2D position = FVector2D::ZeroVector;
	if (In_isTargetShowed == true)
	{
		FGeometry parentGeometry = _loadedContinentalBP->GetCachedGeometry();
		FGeometry targetGeometry = In_widget->GetCachedGeometry();

		FVector2D targetAbsolutePos = targetGeometry.GetAbsolutePosition();
		FVector2D targetLocalSize = targetGeometry.GetLocalSize();
		// 위젯의 위치
		position = parentGeometry.AbsoluteToLocal(
			targetAbsolutePos +
			targetLocalSize * 0.5f);
	}
	else
	{
		// get canvas offset
		UPanelSlot* panelSlot = In_widget->Slot;
		if (panelSlot == nullptr)
		{
			return false;
		}

		UPanelWidget* parentPanel = panelSlot->Parent;
		if (parentPanel == nullptr)
		{
			return false;
		}
		const TArray<UPanelSlot*> slots = parentPanel->GetSlots();

		const UCanvasPanelSlot* canvasPanelSlot = nullptr;
		for (const UPanelSlot* iter: slots)
		{
			if (iter == nullptr)
			{
				continue;
			}

			if (iter->Content == In_widget)
			{
				canvasPanelSlot = Cast<UCanvasPanelSlot>(iter);
				break;
			}
		}

		if (canvasPanelSlot == nullptr)
		{
			return false;
		}
		FVector2D canvasPos = canvasPanelSlot->LayoutData.Offsets.GetTopLeft();
		// 지역맵에서 대륙맵 올라갈때 위치 찾을때 사용(화면밖에 있는 아이템은 GetCachedGeometry 가 비어있음)
		// 기존 좌상 앵커일때는 사이즈가 FVector2D canvasSize(canvasPanelSlot->LayoutData.Offsets.Right, canvasPanelSlot->LayoutData.Offsets.Bottom);
		// 기준으로 가면 되었는데
		// 해상도에 따라 찌그러지는 이슈가 있어서(원하는 위치 안가지는)
		// 앵커를 전체로 잡았을때 위에 값이 우측에서 여기까지, 바닥에서 여기까지로 바뀌어서
		// 아이템 사이즈를 별도로 뺌(안그러면 캔버스 사이즈를 찾아와야되는데 어딘지 모름...)
		//FVector2D canvasSize(canvasPanelSlot->LayoutData.Offsets.Right, canvasPanelSlot->LayoutData.Offsets.Bottom);
		FVector2D canvasSize = GData()->GetGlobalData()->_continentalMapRegionItemSize;
		position = canvasPos + canvasSize * 0.5f;		
	}

	// 센터 구하기
	FVector2D halfSize = _BPRootCanvasSizes * 0.5f;

	FVector2D movePos = halfSize - position;

	// 갈수 있는 위치 내로 처리
	// 신규 위치(x, y)가 0보다 크거나
	// diff size 보다 더 작거나
	// diff 사이즈가 0보다 크거나 같음(canvas 사이즈보다 bp가 작은 경우)

	FVector2D checkedPos = movePos;
	// 오른쪽으로 최대치 넘게 이동
	if (checkedPos.X > 0.0f)
	{
		checkedPos.X = 0.0f;
	}
	// bp 자체가 이동 불가능한 사이즈
	else if (_RootBPDiffSize.X >= 0.0f)
	{
		checkedPos.X = 0.0f;
	}
	// 왼쪽으로 최대치 넘게 이동
	else if (checkedPos.X < _RootBPDiffSize.X)
	{
		checkedPos.X = _RootBPDiffSize.X;
	}


	// 아래쪽으로 최대치 넘게 이동
	if (checkedPos.Y > 0.0f)
	{
		checkedPos.Y = 0.0f;
	}
	// bp 자체가 이동 불가능한 사이즈
	else if (_RootBPDiffSize.Y >= 0.0f)
	{
		checkedPos.Y = 0.0f;
	}
	// 위쪽으로 최대치 넘게 이동
	else if (checkedPos.Y < _RootBPDiffSize.Y)
	{
		checkedPos.Y = _RootBPDiffSize.Y;
	}

	Out_movePos = checkedPos;

	return true;
}

bool UGsUIContinentalMap::IsCanMoveCenter(UUserWidget* In_widget)
{
	FVector2D movePos;
	if (false == GetContinentalBPMovePos(In_widget, movePos))
	{
		return false;
	}

	if (_loadedContinentalBP == nullptr)
	{
		return false;
	}

	return  (movePos == _loadedContinentalBP->RenderTransform.Translation) ? false : true;	
}

// 맵 이동 갱신
void UGsUIContinentalMap::UpdateMapMove(float In_delta)
{
	if (_isMapMoveFinish == true ||
		_loadedContinentalBP == nullptr)
	{
		return;
	}

	float moveSpeed = GData()->GetGlobalData()->_continentalMapMoveSpeed;

	float addRate = In_delta * moveSpeed;
	_mapMoveProgressRate = FMath::Clamp<float>(_mapMoveProgressRate + addRate, 0.f, 1.f);
	UCurveFloat* curve = GData()->GetCamFollowCurve();

	float curveVal = 0.0f;
	if (nullptr != curve)
	{
		curveVal = FMath::Clamp<float>(curve->GetFloatValue(_mapMoveProgressRate), 0.f, 1.f);
	}

	FVector2D newPos = FMath::Lerp(_mapMovePosFrom, _mapMovePosTo, curveVal);

	_loadedContinentalBP->SetRenderTranslation(newPos);

	if (FMath::IsNearlyEqual(1.0f, _mapMoveProgressRate, 0.001f) == false)
	{

	}
	else
	{
		_isMapMoveFinish = true;
	}
}




void UGsUIContinentalMap::TrySelectNextMap(int In_addIndex)
{
	int orderCount = GSWorldMap()->GetContinentalOrderCount();
	if (orderCount == 0)
	{
		return;
	}
	int nextIndex = _currentMapSelectIndex;
	// check if max and add 1
	if (_currentMapSelectIndex == orderCount - 1 &&
		In_addIndex == 1)
	{
		// go to first
		nextIndex = 0;
	}
	// check if min and add -1
	else if(_currentMapSelectIndex == 0 && 
		In_addIndex == -1)
	{
		// go to last
		nextIndex = orderCount - 1;
	}
	else
	{
		// add index
		nextIndex = _currentMapSelectIndex + In_addIndex;
	}
	// change map
	SelectContinentalMap(nextIndex);

}
void UGsUIContinentalMap::SetSelectDungeonItem(UGsUIRegionItem* In_item)
{
	if (In_item == nullptr)
	{
		return;
	}
	if (_currentSelDungeonItem != nullptr &&
		In_item != _currentSelDungeonItem)
	{
		_currentSelDungeonItem->ShowDungeonMenu(false);
	}

	_currentSelDungeonItem = In_item;

	_currentSelDungeonItem->ShowDungeonMenu(true);
}
bool UGsUIContinentalMap::IsShowMapChangeButton(bool In_isLeft)
{
	int orderCount = GSWorldMap()->GetContinentalOrderCount();
	if (orderCount == 0)
	{
		return false;
	}

	bool isShowButton = false;
	// check if max and right
	if (_currentMapSelectIndex == orderCount - 1 &&
		In_isLeft == false)
	{
		// not show
		isShowButton = false;
	}
	// check if min and left
	else if (_currentMapSelectIndex == 0 &&
		In_isLeft == true)
	{
		// not show
		isShowButton = false;
	}
	else
	{
		isShowButton = true;
	}
	return isShowButton;
}