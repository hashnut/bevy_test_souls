// Fill out your copyright notice in the Description page of Project Settings.


#include "GsTerritoryWarpSpotActionUtility.h"
#include "Util/GsTerritoryEditorUtil.h"

//ue4
#include "Editor/Blutility/Public/EditorUtilityLibrary.h"

//territory editor
#include "Actor/Warp/GsTerritoryWarp.h"
#include "TerritoryEditor/Actor/Spot/GsTerritoryWarpSpot.h"
#include "TerritoryEditor/Actor/Warp/GsTerritoryWarp.h"


void UGsTerritoryWarpSpotActionUtility::ModifyDestSpot(FGsTerritoryMapWarp inInput)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();

	if (selectArray.Num() != 1)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, ONLY_ONE_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	AActor* select = selectArray[0];
	if (nullptr == select)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NON_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	AGsTerritoryWarpSpot* warp = Cast<AGsTerritoryWarpSpot>(select);
	if (nullptr == warp)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NON_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	if (warp->_warp)
	{
		warp->_warp->_destMapID = inInput.DestMapID;
		warp->_warp->_destSpotIndex = inInput.SpotIndex;

		warp->_warp->InvalidWidgetText();

		warp->Modify();
	}	
}

void UGsTerritoryWarpSpotActionUtility::ChangeShapeId(FGsTerritoryShapeId inShapeId)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();

	if (selectArray.Num() != 1)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, ONLY_ONE_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	AActor* select = selectArray[0];
	if (nullptr == select)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NON_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	AGsTerritoryWarpSpot* warp = Cast<AGsTerritoryWarpSpot>(select);
	if (nullptr == warp)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NON_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	if (warp->_warp)
	{
		warp->_warp->_shapeID = inShapeId.TargetID;
		warp->InvalidShape();
		warp->Modify();
	}
}

void UGsTerritoryWarpSpotActionUtility::ChangeWarpType(WarpType inWarpType)
{
	const TArray<AActor*>& selectArray = UEditorUtilityLibrary::GetSelectionSet();
	if (selectArray.Num() != 1)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, ONLY_ONE_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	AActor* select = selectArray[0];
	if (nullptr == select)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NON_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	AGsTerritoryWarpSpot* warp = Cast<AGsTerritoryWarpSpot>(select);
	if (nullptr == warp)
	{
#if SPOT_ACTION_UTILITY
		FGsTerritoryEditorUtil::OpenWarningDialog(TERRITORY_EDIOTR_TITLE, NON_TERRITORY_SPOT_SELECT);
#endif

		return;
	}

	if (warp->_warp)
	{
		warp->_warp->_warpType = inWarpType;
		warp->_warp->InvalidWidgetText();
	}
}
