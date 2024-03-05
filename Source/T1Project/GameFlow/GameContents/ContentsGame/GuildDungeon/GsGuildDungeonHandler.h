#pragma once
#include "CoreMinimal.h"
#include "GameFlow/Stage/StageGame/GsStageGameGuildDungeon.h"
#include "Message/GsMessageSystem.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"


/*
	Áö¿ª¸Ê ÄÁÅÙÃ÷ ÄÚµå
*/

struct FGsContributionUserData
{
public:
	int32 _rank;
	FString _userName;
	ContributePoint _contribution;
};

class FGsGuildDungeonHandler
{	
public:
	TArray<TSharedPtr<FGsUserData>> _currentEnterUserList;
	TArray<TSharedPtr<FGsContributionUserData>> _contributionUserList;	
	TSharedPtr<FGsContributionUserData> _localPlayerContribution;
	float _bossHpPercent;
	int32 _ticketCount;		

private:
	MsgSystemHandleArray _systemMsgHandleArray;

public:
	FGsGuildDungeonHandler();
	virtual ~FGsGuildDungeonHandler();

public:
	void Initialize();
	void Clear();

public:
	void SetLocalPlayerContribution(TSharedPtr<FGsContributionUserData> inContribution);
	void ClearLocalPlayerContribution();
	void SetCurrentEnterUserList(TArray<TSharedPtr<FGsUserData>>& inCurrentEnterUserList);
	void ClearEnterUserList();
	void SetContributionUserList(TArray<TSharedPtr<FGsContributionUserData>>& inContributionUserList);	
	void ClearContributionUserList();
	TArray<TSharedPtr<FGsUserData>> GetCurrentEnterUserList(){return _currentEnterUserList;	}
	TArray<TSharedPtr<FGsContributionUserData>> GetContributionUserList() { return _contributionUserList; }
	TSharedPtr<FGsContributionUserData> GetPlayerContribution(){return _localPlayerContribution;}
	void SetBossHpPercent(float inHpPercent){ _bossHpPercent = inHpPercent;}
	float GetBossHpPercent(){return _bossHpPercent;}
	void SetTicketCount(int32 inCount){_ticketCount = inCount;}
	int32 GetTicketCount(){return _ticketCount;}	
	bool GetClearAllDungeon();

	void OnResetDungeonTicket(const struct IGsMessageParam* inParam);
};