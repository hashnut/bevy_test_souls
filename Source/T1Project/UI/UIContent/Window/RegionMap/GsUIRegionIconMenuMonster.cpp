// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/UIContent/Window/RegionMap/GsUIRegionIconMenuMonster.h"

#include "Management/ScopeGame/GsWorldMapManager.h"

#include "UI/UIControlLib/ContentWidget/GsButton.h"

#include "Map/WorldMap/GsWorldMapFunc.h"
#include "Map/WorldMap/GsRegionInfo.h"

#include "BPFunction/UI/GsBlueprintFunctionLibraryUI.h"

#include "UMG/Public/Animation/WidgetAnimation.h"

//void UGsUIRegionIconMenuMonster::NativeOnInitialized()
//{
//	Super::NativeOnInitialized();
//
//	_btnAutoMove->OnClicked.AddDynamic(this, &UGsUIRegionIconMenuMonster::OnClickAutoMove);
//	_btnTeleport->OnClicked.AddDynamic(this, &UGsUIRegionIconMenuMonster::OnClickTeleport);
//}
//
//void UGsUIRegionIconMenuMonster::NativeConstruct()
//{
//	Super::NativeConstruct();
//
//
//}
//
//void UGsUIRegionIconMenuMonster::NativeDestruct()
//{
//
//	// clear now
//	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
//	Super::NativeDestruct();
//}
//
//void UGsUIRegionIconMenuMonster::OnClickAutoMove()
//{
//	FGsWorldMapFunc::ClickAutoMoveRegionMap(true, _listSelectIndex);
//}
//
//void UGsUIRegionIconMenuMonster::OnClickTeleport()
//{
//	FGsPortalInfo* portalInfo =
//		GSWorldMap()->GetCurrentRegionPortalInfoByIndex(_listSelectIndex);
//
//	if (portalInfo == nullptr)
//	{
//		return;
//	}
//
//	FGsWorldMapFunc::ClickTeleportRegionMap(true, _listSelectIndex, portalInfo->_portalName.ToString());
//}
//
//void UGsUIRegionIconMenuMonster::SetData(int In_index)
//{
//	_listSelectIndex = In_index;
//
//	// clear now
//	UGsBlueprintFunctionLibraryUI::StopAllAndClearAnimation(this);
//	PlayAnimation(_aniMenu);
//}