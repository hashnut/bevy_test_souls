#pragma once


#include "../../../Classes/GsState.h"
#include "../../../Classes/GsStateMng.h"
#include "../GsStageMode.h"
#include "GsStageGameBase.h"

//------------------------------------------------------------------------------
// 게임모드를 관리할때 필요한 인스턴스 할당관리자(동일 인스턴스가 아닌 상속구조도 지원하기 위함)
//------------------------------------------------------------------------------
class FGsStageGameAllocator : TGsSharedMapAllocator<FGsStageMode::Game, FGsStageGameBase>
{
public:
	FGsStageGameAllocator() {}
	virtual ~FGsStageGameAllocator() {}

public:
	virtual FGsStageGameBase* Alloc(FGsStageMode::Game inMode) override;
};

//------------------------------------------------------------------------------
// 인게임 스테이지들을 관리한다.
//------------------------------------------------------------------------------
class FGsStageManagerGame : public TGsStateMng<FGsStageGameBase, FGsStageGameAllocator>
{
	using Super = TGsStateMng<FGsStageGameBase, FGsStageGameAllocator>;
public:
	virtual ~FGsStageManagerGame() {};

public:
	virtual void RemoveAll() override;
	virtual void InitState() override;

public:
	void OnReconnectionEnd();
	FGsStageMode::Game GetCurrentStageMode() const;
};
