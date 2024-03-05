#pragma once
#include "CoreMinimal.h"
#include "GsContentsGameBase.h"
#include "Message/GsMessageCostume.h"
#include "Message/GsMessageContentHud.h"

#include "UI/UILib/Define/GsUIDefine.h"

class UGsUIWindowExchangeCostume;
class FGsSummonHandler;
struct IGsMessageParam;

class T1PROJECT_API FGsGameStateExchangeCostume : public FGsContentsGameBase
{
public:
	using MsgCostumeHandle = TPair<MessageCostume, FDelegateHandle>;

protected:
	TArray<MsgCostumeHandle> _msgCostumeHandleList;

private:
	TWeakObjectPtr<UGsUIWindowExchangeCostume>	_windowUI;
	FGsSummonHandler* _summonSceneHandler{ nullptr };

public:
	FGsGameStateExchangeCostume();
	virtual ~FGsGameStateExchangeCostume();

	virtual void Enter() override;
	virtual void Exit() override;

protected:
	void OnExchangeCostumeByItem(const IGsMessageParam* InParam);
	void OnExchangeEventComplete(const IGsMessageParam* InParam);
};