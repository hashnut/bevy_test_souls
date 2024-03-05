#include "GameObject/Camera/Work/Zoom/GsCameraZoomDialog.h"
#include "GameObject/Camera/Mode/GsCameraModeBase.h"

void FGsCameraZoomDialog::OnFinishLerp()
{
	if (_targetMode != nullptr)
	{
		_targetMode->OnZoomFinished();
	}
}

void FGsCameraZoomDialog::Update(float In_deltaTime)
{
	UpdatePosZoom(In_deltaTime);
	
}