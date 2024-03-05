#pragma once

#include "CoreMinimal.h"
#include "GsContentsGameBase.h"

#include "Message/GsMessageCostume.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageSystem.h"

#include "UI/UILib/Define/GsUIDefine.h"

#include "Option/GsGameUserSettings.h"

class AGsActorFairyObject;
class UGsUIWindowCostume;
class FGsSummonHandler;
struct IGsMessageParam;

class T1PROJECT_API FGsGameStateCostume : public FGsContentsGameBase
{
public:
	using MsgCostumeHandle = TPair<MessageCostume, FDelegateHandle>;

	//const EGsUIHideFlags costumeStateHideFlag = EGsUIHideFlags::UI_HIDE_HUD
	//	| EGsUIHideFlags::UI_HIDE_DEALSCROLL
	//	| EGsUIHideFlags::UI_HIDE_DIALOG
	//	| EGsUIHideFlags::UI_HIDE_WIDGETCOMPONENT;

protected:
	TArray<MsgCostumeHandle> _msgCostumeHandleList;
	MsgHUDRedDotHandle _msgRedDotHandle;
	MsgSystemHandle _msgGameReconnected;

private:
	TWeakObjectPtr<class UGsUIWindowCostume>	_windowUI;
	TWeakObjectPtr<class AGsActorCostumeSceneObject>	_actorScene;

	FGsSummonHandler* _summonSceneHandler{ nullptr };

public:
	FGsGameStateCostume();
	virtual ~FGsGameStateCostume();

	virtual void Enter() override;
	virtual void Exit() override;
protected:
	virtual void OnResetUI(UGsUIManager::FOnUIManagerResetUI& resetHandler) override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

	void OnUpdateRedDot(bool bInActive);

	void OnUpdateCostume(const IGsMessageParam* InParam);
	void OnUpdateCostumeCollection(const IGsMessageParam* InParam);

	void OnSelectedCostume(const IGsMessageParam* InParam);
	void OnWearOnCostume(const IGsMessageParam* InParam);
	void OnObtainCostumeResult(const IGsMessageParam* InParam);
	void OnSummonWaiingsUpdate(const IGsMessageParam* InParam);
	void OnCeilingCostumeResult(const IGsMessageParam* InParam);

	void OnSummonComplete(const IGsMessageParam* InParam);

	void OnSelectedAppearance(const IGsMessageParam* InParam);

	void OnFindCostume(const IGsMessageParam* InParam);
	void OnGameServerReconnected();
};

