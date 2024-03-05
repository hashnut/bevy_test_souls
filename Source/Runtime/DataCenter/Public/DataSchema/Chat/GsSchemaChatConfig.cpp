/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "Runtime/DataCenter/Public/DataSchema/Chat/GsSchemaChatConfig.h"
FGsSchemaChatConfig::FGsSchemaChatConfig()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaChatConfig)
	
	spamCount = 10;
	spamIntervalTimeS = 3;
	spamPenaltyTimeS = 60;
	invadeChatOpenLevel = 30;
	freeInvadeChatCount = 5;
	invadeChatCurrencyType = CurrencyType::DIA;
	invadeChatCurrencyAmount = 10;
	groupChatRoomMaxCount = 5;
	groupChaRoomMemberMaxCount = 10;
}
