#include "GsUIContinentalDungeonMenu.h"

#include "Management/GsMessageHolder.h"

#include "Management/ScopeGlobal/GsUIManager.h"

#include "Management/ScopeGame/GsWorldMapManager.h"
#include "Management/ScopeGame/GsAIManager.h"

#include "Message/GsMessageContents.h"

#include "Currency/GsCostPackage.h"

#include "GameObject/Movement/LocalPlayer/GsMovementAutoDefine.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Map/GsMapFunc.h"
#include "Map/WorldMap/GsWorldMapFunc.h"
#include "Map/WorldMap/GsRegionInfo.h"

#include "UTIL/GsTableUtil.h"

#include "DataSchema/Map/GsSchemaMapData.h"
#include "DataSchema/Map/GsSchemaMapBalance.h"

#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedCommonEnum.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UMG/Public/Animation/WidgetAnimation.h"

void UGsUIContinentalDungeonMenu::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_btnAutoMove->OnClicked.AddDynamic(this, &UGsUIContinentalDungeonMenu::OnClickAutoMove);
	_btnTeleport->OnClicked.AddDynamic(this, &UGsUIContinentalDungeonMenu::OnClickTeleport);
}

void UGsUIContinentalDungeonMenu::NativeConstruct()
{
	Super::NativeConstruct();


}

void UGsUIContinentalDungeonMenu::NativeDestruct()
{

	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	Super::NativeDestruct();
}


void UGsUIContinentalDungeonMenu::OnClickAutoMove()
{
	int mapId = GSWorldMap()->GetDungeonEntranceMapId();
	FVector pos = GSWorldMap()->GetDungeonEntrancePos();

	GSAI()->MoveToPosAuto(EGsMovementAutoContentsType::RegionMapPortalMove, mapId,
		pos);

	// main hud로 이동
	GMessage()->GetContents().SendMessage(MessageContents::ACTIVE_HUD);
	// continental map window 닫기
	// 특정 ui 만 닫으면 그전에 있던 ui가 뜨므로 다닫기로 한다
	// ex)지역->대륙에서 이동시
	GUI()->CloseAllStack();
}

void UGsUIContinentalDungeonMenu::OnClickTeleport()
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
		_spotName,
		FGsCostPackage::CreateSimple(currencyType, useCost),
		mapId, spotId);
}

void UGsUIContinentalDungeonMenu::PlayAniMenu()
{
	// clear now
	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
	PlayAnimation(_aniMenu);
}