#include "GsUIContinentalDungeonSide.h"

#include "Message/GsMessageContents.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"

#include "Management/ScopeGame/GsAIManager.h"
#include "Management/ScopeGame/GsWorldMapManager.h"

#include "Map/GsMapFunc.h"
#include "Map/WorldMap/GsWorldMapDefine.h"

#include "Currency/GsCurrencyHelper.h"

#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"

#include "Net/GsNetSendServiceWorld.h"

#include "UTIL/GsTableUtil.h"

#include "UI/UIContent/Popup/GsUIPopupWarp.h"
#include "UI/UIContent/Window/GsUIWindowContinentalMap.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "EventProgress/GsEventProgressDefine.h"

#include "Message/MessageParam/GsUIMessageParam.h"
#include "Message/GsMessageUI.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapBalance.h"

#include "DataSchema/Dungeon/GsSchemaDungeonGroupData.h"
#include "DataSchema/Dungeon/GsSchemaDungeonData.h"

#include "UMG/Public/Components/WidgetSwitcher.h"

#include "Core/Public/Delegates/Delegate.h"

void UGsUIContinentalDungeonSide::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnAutoMove->OnClicked.AddDynamic(this, &UGsUIContinentalDungeonSide::OnClickAutoMove);
	_btnTeleport->OnClicked.AddDynamic(this, &UGsUIContinentalDungeonSide::OnClickTeleport);
	_btnDungeonInfo->OnClicked.AddDynamic(this, &UGsUIContinentalDungeonSide::OnClickDungeonInfo);
	_btnClose->OnClicked.AddDynamic(this, &UGsUIContinentalDungeonSide::OnClickClose);
}

void UGsUIContinentalDungeonSide::OnClickAutoMove()
{
	int mapId = GSWorldMap()->GetDungeonEntranceMapId();
	FVector pos = GSWorldMap()->GetDungeonEntrancePos();

	GSAI()->MoveToPosAuto(EGsMovementAutoContentsType::RegionMapPortalMove, mapId,
		pos);

	// main hud로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	// region map window 닫기
	// 특정 ui 만 닫으면 그전에 있던 ui가 뜨므로 다닫기로 한다
	// ex)지역->대륙->지역에서 이동시
	GUI()->CloseAllStack();
}
void UGsUIContinentalDungeonSide::OnClickTeleport()
{
	// local이  상태이상이면 못보냄
	if (true == FGsMapFunc::IsLocalWarpImpossible())
	{
		return;
	}

	int useCost = 0;
	CurrencyType currencyType = CurrencyType::GOLD;
	
	int mapId = GSWorldMap()->GetDungeonEntranceMapId();
	const FGsSchemaMapData* mapTbl = UGsTableUtil::FindRowById<UGsTableMapData, FGsSchemaMapData>(mapId);
	const FGsSchemaMapBalance* mapBalanceData = mapTbl->balanceId.GetRow();

	if (mapBalanceData != nullptr)
	{
		currencyType =
			mapBalanceData->cost.costCurrencyType;
		useCost = mapBalanceData->cost.costCurrencyAmount;
	}

	int64 spotId = GSWorldMap()->GetDungeonEntranceSpotId();

	GSWorldMap()->ReserveWarpDungeonSide(
		_textName.ToString(),
		FGsCostPackage::CreateSimple(currencyType, useCost),
		mapId, spotId);
	
}

void UGsUIContinentalDungeonSide::OnClickDungeonInfo()
{
	const FGsSchemaDungeonGroupData* dungeonGroupTbl =
		UGsTableUtil::FindRowById<UGsTableDungeonGroupData, FGsSchemaDungeonGroupData>(GSWorldMap()->GetDungeonId());

	if (dungeonGroupTbl == nullptr)
	{
		return;
	}

	if (0 >= dungeonGroupTbl->dungeonList.Num())
	{
		return;
	}

	const FGsSchemaDungeonData* dungeonData = dungeonGroupTbl->dungeonList[0].GetRow();
	if (dungeonData == nullptr)
	{
		return;
	}

	const FGsSchemaMapData* mapData = dungeonData->mapId.GetRow();
	if (mapData == nullptr)
	{
		return;
	}

	FGsUIMsgShortcutMap param(mapData->id);
	GMessage()->GetUI().SendMessage(MessageUI::REQUEST_SHORTCUT_CONTENTS, &param);
}

void UGsUIContinentalDungeonSide::OnClickClose()
{
	TWeakObjectPtr<UGsUIWidgetBase> wigdet = GUI()->GetWidgetByKey(TEXT("WindowContinentalMap"));

	if (wigdet.IsValid() == false)
	{
		return;
	}
	TWeakObjectPtr<UGsUIWindowContinentalMap> continentalWindow = Cast<UGsUIWindowContinentalMap>(wigdet);

	continentalWindow->ShowDungeonSide(false);
}