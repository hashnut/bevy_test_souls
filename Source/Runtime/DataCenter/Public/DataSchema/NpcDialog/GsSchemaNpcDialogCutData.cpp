/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/NpcDialog/GsSchemaNpcDialogCutData.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaNpcDialogCutData::FGsSchemaNpcDialogCutData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaNpcDialogCutData)
	
	Id = 0;
	dialogType = EGsNpcDialogType::DIALOG_TYPE_DIALOG;
	isMy = false;
	textColorType = EGsDialogTextColorType::TEXT_COLOR_DEFAULT;
	voiceId = 0;
	playerPosSpotId = 0;
	dialogViewType.DataTable = nullptr;
	imgInfo.DataTable = nullptr;
	playEffectOffset = FVector(0.000000, 0.000000, 0.000000);
	playEffectScale = 1.000000f;
	playEffectDelayTime = 0.000000f;
	isHideAllUI = false;
	isFadeInEffect = false;
}
