#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageContents.h"

struct IGsMessageParam;
class UGsUIWindowAchievement;

/**
 * Ä¿¹Â´ÏÆ¼ ¼Ò³à
 */
class T1PROJECT_API FGsGameStateAchievement : public FGsContentsGameBase
{
public:
	using MsgAchievementHandle = TPair<MessageContentAchievement, FDelegateHandle>;

protected:
	TArray<MsgAchievementHandle> _msgAchievementHandleList;
	MsgHUDRedDotHandle _msgRedDotHandle;

private:
	TWeakObjectPtr<class UGsUIWindowAchievement>	_windowUI;

public:
	FGsGameStateAchievement();
	virtual ~FGsGameStateAchievement();

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

	virtual void Init() override;
	virtual void Close() override;

protected:
	void RegisterMessages();
	void UnregisterMessages();
	//void OnUpdateThemeItem(const IGsMessageParam* InParam);
	void OnUpdateAchievementItem(const IGsMessageParam* InParam);
	//void OnUpdateThemeProgress(const IGsMessageParam* InParam);
	void OnUpdateAchievementProgress(const IGsMessageParam* InParam);
	//void OnUpdateThemeLevelChanged(const IGsMessageParam* InParam);
	void OnUpdateAchievementLevelChanged(const IGsMessageParam* InParam);
	//void OnAckReceiveThemeReward(const IGsMessageParam* InParam);
	void OnAckReceiveAchievementReward(const IGsMessageParam* InParam);
	void OnAckReceiveAchievementRewardAll(const IGsMessageParam* InParam);
	void OnUpdateRedDot(bool bInActive);
};
