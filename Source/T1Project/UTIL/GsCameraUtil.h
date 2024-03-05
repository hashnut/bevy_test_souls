// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "DataSchema/GsSchemaEnums.h"

#include "GsCameraUtil.generated.h"

/**
 *
 */
class UCameraComponent;
class UMaterialInstanceDynamic;

UCLASS()
class T1PROJECT_API UGsCameraUtil : public UObject
{
	GENERATED_BODY()

public:
	static void SetCameraPostProcessWeight(UCameraComponent* inCamera, int32 inMaterialIndex, float inValue);
	static UMaterialInstanceDynamic* GetCameraPostProcessMeterial(UCameraComponent* inCamera, int32 inMaterialIndex);

	// set camera fov & position offset by contents caemra type
	static void SetContentsTypeCameraData(EGsContentsCameraType In_type, UCameraComponent* In_cameraComponent);
};
