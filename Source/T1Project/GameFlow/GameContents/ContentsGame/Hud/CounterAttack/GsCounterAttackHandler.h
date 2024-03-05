#pragma once

#include "Message/GsMessageGameObject.h"

struct IGsMessageParam;

class FGsCounterAttackHandler
{
private:
	// game object message release
	MsgGameObjHandleArray _gameObjectMsg;

public:
	// 로컬 얻어 맞음
	void OnLocalHit(const IGsMessageParam* In_param);
	void OnAbnormalityAllUpdate(const IGsMessageParam* In_param);
public:
	// initialize
	void Init();
	// close
	void Close();

	void UpdateCounterAttackButtonHUDMode();

	// ui event
public:
	void OnClickCounterAttackButton();
};