#include "GameObject/Camera/Work/FOV/GsCameraFOVDialog.h"

#include "Data/GsGlobalConstant.h"
#include "Data/GsDataContainManager.h"

// 회전속도 구하기
float FGsCameraFOVDialog::GetRotSpeed()
{
	float speed = GData()->GetGlobalData()->CamDialogLocLerpSpeed;
	return speed;
}

void FGsCameraFOVDialog::SetNowSpeed(float In_speed)
{
	_nowSpeed = In_speed;
}