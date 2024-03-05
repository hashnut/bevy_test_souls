#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContents.h"
#include "UI/UIContent/Window/GsUIWindowBattlePass.h"

/**
 *
 */
class T1PROJECT_API FGsGameStateBattlePass : public FGsContentsGameBase
{
private:
	TWeakObjectPtr<UGsUIWindowBattlePass>	_windowUI;

protected:
	TArray<TPair<MessageContentBattlePass, FDelegateHandle>>_battlePassDelegates;
	MsgHUDRedDotHandle _msgRedDotHandle;

public:
	FGsGameStateBattlePass();
	virtual ~FGsGameStateBattlePass();

public:
	virtual void Enter() override;
	virtual void Exit() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

	void UpdateBattlePass(const IGsMessageParam* InParam);
	void AckBattlePassLevelReward(const IGsMessageParam* InParam);
	void UpdateBattlePassLevel(const IGsMessageParam* InParam);
	void UpdateBattlePassMission(const IGsMessageParam* InParam);
	void UpdateBattlePassMissionTab(const IGsMessageParam* InParam);
	void UpdateBattlePassLevelRewardAll(const IGsMessageParam* InParam);
	void ModifyBattlePassInfo(const IGsMessageParam* InParam);

	void OnUpdateRedDot(bool bInActive);
};

