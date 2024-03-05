#include "GsGuildDungeonHandler.h"
#include "Management/ScopeGlobal/GsGameDataManager.h"
#include "Management/GsMessageHolder.h"
#include "Message/GsMessageSystem.h"
#include "Guild/GsGuildHelper.h"
#include "DataSchema/Guild/GsSchemaGuildConfig.h"


FGsGuildDungeonHandler::FGsGuildDungeonHandler()
{
	
}

FGsGuildDungeonHandler::~FGsGuildDungeonHandler()
{

}

void FGsGuildDungeonHandler::Initialize()
{
	_currentEnterUserList.Empty();
	_contributionUserList.Empty();
	_localPlayerContribution.Reset();
	_bossHpPercent  = 0;

	if (FGsMessageHolder* msg = GMessage())
	{
		_systemMsgHandleArray.Emplace(
			msg->GetSystemParam().AddRaw(MessageSystem::RESET_TIME_CONTENTS, this,
				&FGsGuildDungeonHandler::OnResetDungeonTicket));
	}	
}

void FGsGuildDungeonHandler::Clear()
{
	_currentEnterUserList.Empty();
	_contributionUserList.Empty();
	_localPlayerContribution.Reset();

	if (FGsMessageHolder* msg = GMessage())
	{
		for (MsgSystemHandle& messagePair : _systemMsgHandleArray)
		{
			msg->GetSystemParam().Remove(messagePair);
		}
	}	
}

void FGsGuildDungeonHandler::SetLocalPlayerContribution(TSharedPtr<FGsContributionUserData> inContribution)
{
	_localPlayerContribution = inContribution;
}

void FGsGuildDungeonHandler::ClearLocalPlayerContribution()
{
	_localPlayerContribution.Reset();
}

void FGsGuildDungeonHandler::SetCurrentEnterUserList(TArray<TSharedPtr<FGsUserData>>& inCurrentEnterUserList)
{
	_currentEnterUserList = inCurrentEnterUserList;
}
	
void FGsGuildDungeonHandler::ClearEnterUserList()
{
	_currentEnterUserList.Empty();
}

void FGsGuildDungeonHandler::SetContributionUserList(TArray<TSharedPtr<FGsContributionUserData>>& inContributionUserList)
{
	_contributionUserList = inContributionUserList;
	_contributionUserList.Sort([](TSharedPtr<FGsContributionUserData> inSource, TSharedPtr<FGsContributionUserData> inDest)->bool {
			return inSource->_contribution > inDest->_contribution;
		});

	for (int32 i = 0; i < _contributionUserList.Num(); ++i)
	{
		_contributionUserList[i].Get()->_rank = i + 1;
	}

	const FString& userName = GGameData()->GetUserData()->mUserName;
	TSharedPtr<FGsContributionUserData>* userData = _contributionUserList.FindByPredicate([&](TSharedPtr<FGsContributionUserData> inSource)->bool {
			return inSource->_userName.Equals(userName);
		});

	if (userData)
	{
		SetLocalPlayerContribution((*userData));
	}	
}


void FGsGuildDungeonHandler::ClearContributionUserList()
{
	_contributionUserList.Empty();
}

bool FGsGuildDungeonHandler::GetClearAllDungeon()
{
	return GGameData()->GetUserData()->IsClearGuildDungeon();
}

void FGsGuildDungeonHandler::OnResetDungeonTicket(const IGsMessageParam* inParam)
{
	const FGsPrimitiveInt32* param = inParam->Cast<const FGsPrimitiveInt32>();
	if (nullptr == param)
	{
		return;
	}
	
	if (ResetTimeTicket::RTT_GUILD_DUNGEON == (ResetTimeTicket)param->_data)
	{
		_currentEnterUserList.Empty();
		_contributionUserList.Empty();
		_localPlayerContribution.Reset();

		_bossHpPercent = 100.0f;
		_ticketCount = FGsGuildHelper::GetGuildConfig()->dungeonTicketLimitCount;
		GGameData()->ResetGuildDungoen();

		GMessage()->GetGuild().SendMessage(MessageContentGuild::INVALID_DUNGOEN_TAB, nullptr);
	}
}
