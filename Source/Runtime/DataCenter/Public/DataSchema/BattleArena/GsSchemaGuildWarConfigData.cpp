/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/BattleArena/GsSchemaGuildWarConfigData.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaGuildWarConfigData::FGsSchemaGuildWarConfigData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaGuildWarConfigData)
	
	dungoneGroupId.DataTable = nullptr;
	mapDataId.DataTable = nullptr;
	seasonDataId.DataTable = nullptr;
	limitGuildLevel = 0;
	limitMinApplyGrade = GuildMemberGradeType::UNKNOWN;
	limitMinMemberCount = 0;
	limitMaxMemberCount = 0;
	limitMinRewardPoint = 0;
	userKillPoint = 0;
	userAssitPoint = 0;
	redSpotId = 0;
	blueSpotId = 0;
	afterDelayTimeApplyEnable = 0;
	afterDelayTimeReady = 0;
	afterDelayTimePlayWait = 0;
	afterDelayTimePlay = 0;
	afterDelayTimeEnd = 0;
	afterDelayTimeExit = 0;
	winMailTypeId.DataTable = nullptr;
	defeatMailTypeId.DataTable = nullptr;
	drawMailTypeId.DataTable = nullptr;
	bossNpcId.DataTable = nullptr;
	coinId.DataTable = nullptr;
	basicMatchingPoint = 0;
	matchingPointVariable1 = 0;
	matchingPointVariable2 = 0;
	matchingPointWin = 0;
	matchingPointDraw = 0;
	matchingPointNoBattleWin = 0;
	matchingPointLose = 0;
	openEventTickerId.DataTable = nullptr;
	openEventChatTickerId.DataTable = nullptr;
	mainInterWorldServerId.DataTable = nullptr;
	bonusRankingPoint = 0;
	gameOverScore = 0;
}
