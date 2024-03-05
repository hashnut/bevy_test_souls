/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/NpcDialog/GsSchemaNpcDialogData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaNpcDialogData::FGsSchemaNpcDialogData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaNpcDialogData)
	
	Id = 0;
	playerPosSpotId = 0;
	dialogViewType.DataTable = nullptr;
	dialogViewType.RowName = FName(TEXT("1"));
	fairyId.DataTable = nullptr;
	fairyPosType = EGsNpcDialogFairyPosType::LEFT;
	IsZoomFirstCameraMove = false;
	closeFadeInTime = 0.800000f;
}
