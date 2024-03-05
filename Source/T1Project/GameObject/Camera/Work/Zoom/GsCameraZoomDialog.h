#pragma once

#include "GameObject/Camera/Work/Zoom/GsCameraZoomGame.h"

/*
*  dialog zoom
*/
class FGsCameraZoomDialog : public FGsCameraZoomGame
{
public:
	virtual void OnFinishLerp()override;
	virtual void Update(float In_deltaTime)override;
};