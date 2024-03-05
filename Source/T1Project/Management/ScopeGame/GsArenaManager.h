#pragma once

#include "CoreMinimal.h"
#include "Classes/GsManager.h"

#include "Arena/GsArenaDefine.h"
#include "Arena/GsArenaHandler.h"

#include "DataSchema/Arena/GsSchemaArenaData.h"

#include "Management/GsScopeHolder.h"

#include "Message/GsMessageStage.h"
#include "Message/GsMessageGameObject.h"
#include "Message/GsMessageSystem.h"
#include "Message/GsMessageInvasion.h"

#include "Shared/Shared/SharedInclude/SharedTypes.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"


class T1PROJECT_API FGsArenaManager final : public IGsManager
{
protected:
	FGsArenaHandler _arenaHandler;
	
	ArenaUserData _targetUser;

	EGsArenaInState _ArenaInState = EGsArenaInState::NONE;
	EGsArenaOutState _arenaOutState = EGsArenaOutState::NONE;
	DuelResult _arenaResult = DuelResult::NONE;
	time_t _arenaStartTime = 0;
	time_t _arenaCountDownStartTime = 0;
	const FGsSchemaArenaData* _enteredArenaData;
	bool _isReconnect = false;

private:
	TArray<MsgStageHandle> _msgStageHandleList;
	MsgGameObjHandleArray _msgGameObjectHandleList;
	MsgSystemHandleArray _msgSystemHandleList;
	TArray<TPair<MessageInvasion, FDelegateHandle>> _msgInvasionHandlerList;

public:
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	FGsArenaHandler* GetArenaHandler();

public:
	// Setter
	void SetTargetInfo(UserDBId InUserDBId, Level InLevel, FString InUserName, bool InIsVisible);
	void ResetTargetInfo();

	void SetArenaInState(EGsArenaInState InArenaInState);
	void SetArenaOutState(EGsArenaOutState InArenaOutState);
	void SetArenaResult(DuelResult InResult);
	void SetArenaStartTime(time_t InStartTime);
	void SetCountDownStartTime(time_t InStartTime);
	void SetEnteredArenaData(const FGsSchemaArenaData* InArenaData);

	// Getter
	ArenaUserData GetTargetInfo() { return _targetUser; }

	EGsArenaInState GetArenaInState() { return _ArenaInState; }
	EGsArenaOutState GetArenaOutState() { return _arenaOutState; }
	DuelResult GetArenaResult() { return _arenaResult; }
	time_t GetArenaStartTime() { return _arenaStartTime; }
	time_t GetCountDownStartTime() { return _arenaCountDownStartTime; }
	const FGsSchemaArenaData* GetArenaData();
	const FGsSchemaArenaData* GetEnteredArenaData() { return _enteredArenaData; }
	
	bool IsArenaStarted();
	bool IsArenaEnded();
	bool IsInArena();
	bool IsTargetUser(const FString& InTargetUserName);

	// Clear
	void ClearArenaRequest();

protected:
	void OnEnterArena(const IGsMessageParam* InParam);
	void OnLeaveArena(const IGsMessageParam* InParam);
	void OnEnterArenaReady(const IGsMessageParam* InParam);
	void OnLeaveArenaReady(const IGsMessageParam* InParam);

	void OnReconnectEnd();
	void OnLocalSpawnMeComplete(const IGsMessageParam* InParam);
	void OnBeforeGameToLobby();
	void OnInvasionStart(const IGsMessageParam* InParam);
};

#define GSArena() UGsScopeHolder::GetGameManagerFType<FGsArenaManager>(UGsScopeGame::EManagerType::Arena)