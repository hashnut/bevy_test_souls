#pragma once

#include "Classes/GsState.h"
#include "GsGameFlow.h"
/**
* FlowState의 부모 클래스. FGsGameFlow::Mode.
*/
class T1PROJECT_API FGsGameFlowBase : public TGsState<FGsGameFlow::Mode>
{
public:
	FGsGameFlowBase() : TGsState<FGsGameFlow::Mode>(FGsGameFlow::Mode::MAX) {}
	explicit FGsGameFlowBase(FGsGameFlow::Mode inMode) : TGsState<FGsGameFlow::Mode>(inMode) {}
	virtual ~FGsGameFlowBase() = default;

public:
	virtual void Enter() override{};
	virtual void Exit() override {};
	virtual void Update(float In_deltaTime) override {};

public:
	virtual void OnReconnectionEnd() {}
	virtual void OnBackToTitle() {}
	virtual void OnExitGame() {}
	virtual void OnQuitGame() {}
	virtual void OnRestartGame() {}

#if RECONNECT_DEBUG
public:
	virtual FString ToString() { return TEXT(""); }
#endif

	virtual int32 GetContentStateIndex() const { return -1; }
};
