#pragma once
#include "CoreMinimal.h"
#include "GsContentsGameBase.h"

#include "Message/GsMessageFairy.h"
#include "Message/GsMessageContentHud.h"
#include "Message/GsMessageSystem.h"

#include "UI/UILib/Define/GsUIDefine.h"
#include "UI/UIContent/Window/GsUIWindowFairy.h"

#include "Option/GsGameUserSettings.h"

class AGsActorFairyObject;
struct IGsMessageParam;
class FGsSummonHandler;

class T1PROJECT_API FGsGameStateFairy : public FGsContentsGameBase
{
public:
	using MsgFairyHandle = TPair<MessageFairy, FDelegateHandle>;

	//const EGsUIHideFlags fairyStateHideFlag = EGsUIHideFlags::UI_HIDE_HUD
	//										| EGsUIHideFlags::UI_HIDE_DEALSCROLL
	//										| EGsUIHideFlags::UI_HIDE_DIALOG
	//										| EGsUIHideFlags::UI_HIDE_WIDGETCOMPONENT;
protected:
	TArray<MsgFairyHandle> _msgFairyHandleList;
	MsgHUDRedDotHandle _msgRedDotHandle;
	MsgSystemHandle _msgGameReconnected;

	UGsUIWindowFairy::UIInitData _reserveData{ 0, 0 };

private:
	TWeakObjectPtr<class UGsUIWindowFairy>		_windowUI;
	TWeakObjectPtr<class AGsActorFairyObject>	_actorFairy;

	FGsSummonHandler* _summonSceneHandler{ nullptr };
public:
	FGsGameStateFairy();
	virtual ~FGsGameStateFairy();

	virtual void Enter() override;
	virtual void Exit() override;
	//virtual void Update(float In_deltaTime) override;
protected:
	virtual void OnResetUI(UGsUIManager::FOnUIManagerResetUI& resetHandler) override;

protected:
	void RegisterMessages();
	void UnregisterMessages();

	void OnUpdateRedDot(bool bInActive);

	void OnUpdateFairy(const IGsMessageParam* InParam);
	void OnUpdateFairyCollection(const IGsMessageParam* InParam);
	
	void OnSelectedFairy(const IGsMessageParam* InParam);
	void OnSummonedFairy(const IGsMessageParam* InParam);
	void OnEnchantFairyResult(const IGsMessageParam* InParam);
	void OnCeilingFairyResult(const IGsMessageParam* InParam);
	void OnObtainFairyResult(const IGsMessageParam* InParam); 
	void OnSummonWaiingFairyUpdate(const IGsMessageParam* InParam);

	void OnSummonComplete(const IGsMessageParam* InParam);

	void OnChangeFairyWindowInit(const IGsMessageParam* InParam);
	void OnFindFairy(const IGsMessageParam* InParam);

	void SetFairyEnchantPos(const IGsMessageParam* InParam);	
	void SetFairyNormalPos(const IGsMessageParam* InParam);

	void OnGameServerReconnected();
};

