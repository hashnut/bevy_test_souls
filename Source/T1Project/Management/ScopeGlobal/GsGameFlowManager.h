#pragma once

#include "CoreMinimal.h"
#include "Classes/GsState.h"
#include "Classes/GsStateMng.h"
#include "Classes/GsManager.h"
#include "GameFlow/GsGameFlow.h"
#include "GameFlow/GsGameFlowBase.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageStage.h"
#include "Message/MessageHandler/GsSystemMessageHandlerInterface.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGame/GsInventoryManager.h"


//------------------------------------------------------------------------------
// 게임모드를 관리할때 필요한 인스턴스 할당관리자(동일 인스턴스가 아닌 상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class FGsGameFlowAllocator :
	TGsSharedMapAllocator<FGsGameFlow::Mode, FGsGameFlowBase>
{
public:
	FGsGameFlowAllocator()
	{
	}
	virtual ~FGsGameFlowAllocator()
	{
	}

public:
	virtual FGsGameFlowBase* Alloc(FGsGameFlow::Mode inMode) override;

};


//------------------------------------------------------------------------------
// 게임모드를 관리
//------------------------------------------------------------------------------
class T1PROJECT_API FGsGameFlowManager :
	public TGsStateMng<FGsGameFlowBase, FGsGameFlowAllocator>,
	public IGsManager,
	public IGsMessageHandler,
	public TGsMessageDelegateList<MSystem, MessageSystem>
{
	using _Super = TGsStateMng<FGsGameFlowBase, FGsGameFlowAllocator>;
public:
	// IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;
	virtual void Update(float inTick) override;

protected:
	// IGsSystemMessageHandler
	virtual void InitializeMessage() override;
	virtual void FinalizeMessage() override;

	
protected:
	//Message (system)
	void OnReconnectionEnd();
	void OnBacktoLogin();
	void OnBacktoLobby();
	void OnBackToTitle();
	void OnExitGame();
	void OnQuitGame();
	void OnRestartGame();

	//Message (stage)
	void OnChangeGameFlow();

public:
	// Current가 아닐 경우 nullptr 리턴
	class FGsGameFlowGame* GetGameFlow() const;
	class FGsGameFlowLobby* GetLobbyFlow() const;

public:
	class TSharedPtr<FGsGameFlowBase> GetCurrentFlow() const;
	FGsGameFlow::Mode GetCurrentFlowType() const;

	int32 GetCurrentContentStateIndex() const;
};

#define GMode() UGsScopeHolder::GetGlobalManagerFType<FGsGameFlowManager>(UGsScopeGlobal::EManagerType::GameFlow)

