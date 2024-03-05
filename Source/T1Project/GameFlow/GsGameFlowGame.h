#pragma once

#include "CoreMinimal.h"
#include "GsGameFlowBase.h"
#include "GsGameFlow.h"

#include "Message/GsMessageStage.h"
#include "Message/GsMessageGameObject.h"
#include "Shared/Client/SharedEnums/SharedCommonEnum.h"

/**
 * Game Flow.
 * - StageManager: 서버와 동기화 되는 Flow.
 * - ContentsManager: 클라에서 관리를 위해 만드는 Flow.
 */
class T1PROJECT_API FGsGameFlowGame : public FGsGameFlowBase
{
	// Flow 관련 매니저
	class FGsStageManagerGame*		_stageManager = nullptr;
	class FGsContentsManagerGame*	_contentsGameManager = nullptr;
	TWeakObjectPtr<class UGsGameObjectLocalPlayer> _localPlayer;

	MsgStageHandleArray	_listStageDelegate;
	MsgGameObjHandleArray _litObjectDelegate;

public:
	FGsGameFlowGame() : FGsGameFlowBase(FGsGameFlow::Mode::GAME) {}
	virtual~FGsGameFlowGame() = default;

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override;

public:
	virtual void OnReconnectionEnd() override;
	virtual void OnBackToTitle() override;
	virtual void OnExitGame() override;
	virtual void OnQuitGame() override;
	virtual int32 GetContentStateIndex() const override;
	virtual void OnRestartGame() override;

protected:
	// 메시지 핸들러 등록
	void InitMessageHandler();

	void OnEnterTown();
	void OnEnterSafetyZone();
	void OnEnterChaos();
	void OnEnterField();
	void OnEnterDungeon();
	void OnEnterBattleArena();
	void OnEnterGuildDungeon();
	void OnEnterPvPContent();	
	void OnEnterArenaReady();
	void OnEnterArena();
	void OnEnterNonPK();
	void OnEnterWorldBoss();
	void OnEnterAgitDungeon();
	void OnChangeLevel();	
	void OnLocalPlayerSpawnComplete(const struct IGsMessageParam* InParam);

private:	
	void OnTerritoryUpdate(const struct IGsMessageParam* InParam);
	void SetPlayer();
	void SetPlayerTerritory(TerritoryType inTerritory = TerritoryType::NONE);

#if RECONNECT_DEBUG
public:
	virtual FString ToString() override;
#endif

public:
	class FGsStageManagerGame*	GetStageManagerGame() { return _stageManager; }
	class FGsContentsManagerGame* GetContentsManagerGame(){return _contentsGameManager;}
};
