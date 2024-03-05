/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Guild/GsSchemaGuildConfig.h"
#include "Runtime/DataCenter/Public/DataSchema/Guild/GsSchemaGuildAuctionMinPrice.h"
#include "Runtime/DataCenter/Public/DataSchema/Guild/GsSchemaGuildStoreExtend.h"
#include "Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Runtime/DataCenter/Public/DataSchema/Reward/GsSchemaRewardData.h"
#include "Runtime/DataCenter/Public/Shared/Client/SharedEnums/SharedGuildEnum.h"
#include "Runtime/Engine/Classes/EditorFramework/AssetImportData.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "../Plugins/RSQL/Source/Runtime/Public/DataTable/GsDataTable.h"
FGsSchemaGuildConfig::FGsSchemaGuildConfig()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaGuildConfig)
	
	requestUserLevel = 0;
	creationCost = 0;
	reJoinCooldownHour = 0;
	changeEmblemCost = 0;
	bonusCountFromDonate = 0.000000f;
	guildBuffReactivateMin = 0;
	maxJoinRequestCount = 0;
	dailyDonateLimits = 0;
	allyWaitTimeM = 0;
	prepareWarTimeM = 0;
	warTimeM = 0;
	protectTimeM = 0;
	maxAllySendCount = 0;
	maxAllyReceiveCount = 0;
	maxAllyCount = 0;
	maxEnemyCount = 0;
	surrenderCurrencyAmount = 0;
	itemCollectionRegisterUseLimitCount = 0;
	dungeonRestTime = ResetTimeTicket::RTT_NONE;
	dungeonEnterLevel = 0;
	dungeonTicketLimitCount = 0;
	dungeonDurationTime = 0;
	newMemberRestrictDungeonForTimeM = 0;
	dungeonRewardPersonMailTypeId = 0;
	dungeonRewardJoinMailTypeId = 0;
	guildQuestOpenLevel = 0;
	guildCoinId.DataTable = nullptr;
	guildDungeonDeathKickSec = 0;
	newMemberRestrictGuildArenaForTimeM = 0;
}
