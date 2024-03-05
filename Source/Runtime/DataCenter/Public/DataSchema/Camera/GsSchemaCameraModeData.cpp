/// DO NOT EDIT THIS. This file is an auto generated file. Just use header generator.
#include "DataSchema/Camera/GsSchemaCameraModeData.h"
FGsSchemaCameraModeData::FGsSchemaCameraModeData()
{
	SET_ALL_PROPERTIES_FROM_ASSETSCHEMA_DEFAULT(FGsSchemaCameraModeData)
	
	Mode = EGsCameraMode::None;
	ArmLengthTo = 0.000000f;
	ControllerPitch = 0.000000f;
	FOV = 90.000000f;
	SocketOffset = FVector(0.000000, 0.000000, 0.000000);
	ViewPitchMin = -25.000000f;
	ViewPitchMax = 10.000000f;
	CamArmLengthMax = 1000.000000f;
	CamArmLengthMin = 300.000000f;
	CamShiftPos = FVector(0.000000, 0.000000, 0.000000);
	ZoomMinDepthOfFieldFocalRegionMobile = 0.000000f;
	ZoomMinFOV = 0.000000f;
	UseZoomMinDOF = false;
	UseZoomMinFOV = false;
}
