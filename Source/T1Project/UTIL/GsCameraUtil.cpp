// Fill out your copyright notice in the Description page of Project Settings.


#include "GsCameraUtil.h"

#include "Camera/CameraComponent.h"

#include "UTIL/GsTableUtil.h"

#include "DataCenter/Public/DataSchema/Camera/GsSchemaContentsCameraData.h"

#include "Materials/MaterialInstanceDynamic.h"

void UGsCameraUtil::SetCameraPostProcessWeight(UCameraComponent* inCamera, int32 inMaterialIndex, float inValue)
{
	if (inCamera)
	{
		TArray<FWeightedBlendable>& postProcessArray = inCamera->PostProcessSettings.WeightedBlendables.Array;

		if (postProcessArray.IsValidIndex(inMaterialIndex))
		{
			postProcessArray[inMaterialIndex].Weight = inValue;
		}
	}
}

UMaterialInstanceDynamic* UGsCameraUtil::GetCameraPostProcessMeterial(UCameraComponent* inCamera, int32 inMaterialIndex)
{
	if (inCamera)
	{
		TArray<FWeightedBlendable>& postProcessArray = inCamera->PostProcessSettings.WeightedBlendables.Array;

		if (postProcessArray.IsValidIndex(inMaterialIndex))
		{
			if (UObject * obj = postProcessArray[inMaterialIndex].Object)
			{
				return Cast<UMaterialInstanceDynamic>(obj);
				//return nullptr;
			}			
		}
	}

	return nullptr;
}
// set camera fov & position offset by contents caemra type
void UGsCameraUtil::SetContentsTypeCameraData(EGsContentsCameraType In_type, UCameraComponent* In_cameraComponent)
{
	if (In_cameraComponent == nullptr)
	{
		return;
	}

	const FGsSchemaContentsCameraData* findTbl =
		UGsTableUtil::FindRowById<UGsTableContentsCameraData, FGsSchemaContentsCameraData>(In_type);

	if (findTbl == nullptr)
	{
		return;
	}

	In_cameraComponent->SetFieldOfView(findTbl->FOV);
	In_cameraComponent->SetRelativeLocation(findTbl->CamearPositionOffset);
}