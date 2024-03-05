// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shared/Shared/SharedInclude/SharedTypes.h"

struct FGsCustomizeUIData;
//
//struct FGsCustomizeFaceData
//{
//	int32 _faceId;
//	int32 _hairId;
//
//	uint8 _hairColorIndex;
//	uint8 _skinColorIndex;
//	uint8 _browColorIndex;
//	uint8 _pupilColorIndex;
//
//	uint8 _cheekSize;
//	uint8 _chinLength;
//	uint8 _chinSize;
//	uint8 _nosePosition;
//	uint8 _eyePosition;
//	uint8 _eyeSpacing;
//	uint8 _eyeSize;	
//	uint8 _pupilSize;
//	uint8 _browPosition;
//	uint8 _browAngle;
//	uint8 _browLength;
//	uint8 _mouthPosition;
//	uint8 _mouthSize;
//};
//
////USTRUCT(BlueprintType)
//struct FGsCustomizeBodyData
//{
//	int32 _bodyId;
//
//	uint8 _skinColorIndex;
//
//	uint8 _height;
//	uint8 _headSize;
//	uint8 _chestSize;
//	uint8 _waistSize;
//	uint8 _pelvisSize;
//	uint8 _legSize;
//};

struct FGsCustomizeData
{
	/*FGsCustomizeFaceData FaceData;
	FGsCustomizeBodyData BodyData;*/

	CustomizeFaceData _faceData;
	CustomizeBodyData _bodyData;

public:
	void ConvertToClientFace(const CustomizeFaceData& InData);
	void ConvertToClientBody(const CustomizeBodyData& InData);
	const CustomizeFaceData& ConvertToServerFace(FGsCustomizeUIData* InUICustomData);
	const CustomizeBodyData& ConvertToServerBody(FGsCustomizeUIData* InUICustomData);
};