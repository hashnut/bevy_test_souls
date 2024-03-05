#pragma once
#include "GameFlow/Stage/GsStageMode.h"
#include "Classes/GsState.h"

//------------------------------------------------------------------------------
// 인게임 스테이지를 관리할때 필요한 메모리 할당관리자(상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class UGsUIManager;
class UGsUIHUDFrameMain;
class FGsStageGameBase : public TGsState<FGsStageMode::Game>
{
public:
	FGsStageGameBase() : TGsState<FGsStageMode::Game>(FGsStageMode::Game::GAME_MAX) {}
	explicit FGsStageGameBase(FGsStageMode::Game inMode) : TGsState<FGsStageMode::Game>(inMode) {}
	virtual ~FGsStageGameBase() = default;

public:
	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float In_deltaTime) override {};

public:
	virtual void OnReconectionEnd() {};
};
