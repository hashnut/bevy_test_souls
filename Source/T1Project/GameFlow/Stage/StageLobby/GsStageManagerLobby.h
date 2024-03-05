#pragma once

#include "../../../Classes/GsState.h"
#include "../../../Classes/GsStateMng.h"
#include "../GsStageMode.h"
#include "GsStageLobbyBase.h"
//------------------------------------------------------------------------------
// 게임모드를 관리할때 필요한 인스턴스 할당관리자(동일 인스턴스가 아닌 상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class FGsStageLobbyAllocator : TGsSharedMapAllocator<FGsStageMode::Lobby, FGsStageLobbyBase>
{
public:
	FGsStageLobbyAllocator() {}
	virtual ~FGsStageLobbyAllocator() {}
public:

	virtual FGsStageLobbyBase* Alloc(FGsStageMode::Lobby inMode) override;
};

//------------------------------------------------------------------------------
// 게임모드를 관리
//------------------------------------------------------------------------------
class FGsStageManagerLobby : public TGsStateMng<FGsStageLobbyBase, FGsStageLobbyAllocator>
{
	using Super = TGsStateMng<FGsStageLobbyBase, FGsStageLobbyAllocator>;
public:
	virtual ~FGsStageManagerLobby() {};

public:
	virtual void RemoveAll() override;
	virtual void InitState() override;

public:
	void Update(float In_deltaTime);

protected:
	// 메시지 핸들러 등록
	void InitMessageHandler();

public:
	FGsStageMode::Lobby GetCurrentStageMode() const;
};
