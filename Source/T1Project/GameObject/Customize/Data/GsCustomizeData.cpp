#include "GsCustomizeData.h"

#include "UI/UIContent/Window/Customize/GsCustomizeUIData.h"
void FGsCustomizeData::ConvertToClientFace(const CustomizeFaceData& InData)
{
	_faceData = InData;
	//FMemory::Memcpy(customizingFaceData, InData, MAX_USER_CUSTOMIZING_FACE_DATA_SIZE);

	////Buffer.Reset(MAX_USER_CUSTOMIZING_FACE_DATA_SIZE);
	////Buffer.Append(InData, MAX_USER_CUSTOMIZING_FACE_DATA_SIZE);

	////TArray<int32> Test;
	////Test.Reset(sizeof(int32) * 2);
	////Test.Append((int32*)InData, sizeof(int32) * 2);
	////
	///*int32* face = nullptr;
	//int32* hair = nullptr;*/
	//int32 face;
	//int32 hair;

	//FMemory::Memcpy(&face, customizingFaceData, sizeof(int32));
	//FMemory::Memcpy(&hair, customizingFaceData + sizeof(int32), sizeof(int32));

	////FaceData._faceId = *face;//Test[0];// (int32)(Buffer.GetData());
	////FaceData._hairId = *hair;//Test[1];// (int32)(Buffer.GetData() + sizeof(int32));
	//FaceData._faceId = face;//Test[0];// (int32)(Buffer.GetData());
	//FaceData._hairId = hair;//Test[1];// (int32)(Buffer.GetData() + sizeof(int32));

	//int32 index = sizeof(int32) * 2;	

	//FaceData._hairColorIndex = customizingFaceData[index];
	//FaceData._skinColorIndex = customizingFaceData[index + 1];
	//FaceData._browColorIndex = customizingFaceData[index + 2];
	//FaceData._pupilColorIndex = customizingFaceData[index + 3];

	//FaceData._cheekSize = customizingFaceData[index + 4];
	//FaceData._chinLength = customizingFaceData[index + 5];
	//FaceData._chinSize = customizingFaceData[index + 6];
	//FaceData._nosePosition = customizingFaceData[index + 7];
	//FaceData._eyePosition = customizingFaceData[index +8];
	//FaceData._eyeSpacing = customizingFaceData[index +9];
	//FaceData._eyeSize = customizingFaceData[index + 10];
	//FaceData._pupilSize = customizingFaceData[index + 11];
	//FaceData._browPosition = customizingFaceData[index + 12];
	//FaceData._browAngle = customizingFaceData[index + 13];
	//FaceData._browLength = customizingFaceData[index + 14];
	//FaceData._mouthPosition = customizingFaceData[index + 15];
	//FaceData._mouthSize = customizingFaceData[index + 16];
}


void FGsCustomizeData::ConvertToClientBody(const CustomizeBodyData& InData)
{
	_bodyData = InData;
	//FMemory::Memcpy(customizingBodyData, InData, MAX_USER_CUSTOMIZING_FACE_DATA_SIZE);

	//int32 body;// = nullptr;
	//
	//FMemory::Memcpy(&body, customizingBodyData, sizeof(int32));
	//BodyData._bodyId = body;

	//int32 index = sizeof(int32);

	//BodyData._height = customizingBodyData[index];
	//BodyData._headSize = customizingBodyData[index + 1];
	//BodyData._chestSize = customizingBodyData[index + 2];
	//BodyData._waistSize = customizingBodyData[index + 3];
	//BodyData._pelvisSize = customizingBodyData[index + 4];
	//BodyData._legSize = customizingBodyData[index + 5];

	///*Buffer.Reset(MAX_USER_CUSTOMIZING_FACE_DATA_SIZE);
	//Buffer.Append(InData, MAX_USER_CUSTOMIZING_FACE_DATA_SIZE);

	//TArray<int32> Test;
	//Test.Reset(sizeof(int32));
	//Test.Append((int32*)InData, sizeof(int32));

	//BodyData._bodyId = Test[0];

	//int32 index = sizeof(int32);

	//BodyData._height = Buffer[index];
	//BodyData._headSize = Buffer[index + 1];
	//BodyData._chestSize = Buffer[index + 2];
	//BodyData._waistSize = Buffer[index + 3];
	//BodyData._pelvisSize = Buffer[index + 4];
	//BodyData._legSize = Buffer[index + 5];*/
}


const CustomizeFaceData& FGsCustomizeData::ConvertToServerFace(FGsCustomizeUIData* InUICustomData)
{
	if (InUICustomData == nullptr) return _faceData;

	_faceData.mFaceId = InUICustomData->GetFaceId();
	_faceData.mHairId = InUICustomData->GetHairId();

	_faceData.mHairColorIndex = InUICustomData->GetHairColorIndex();
	_faceData.mSkinColorIndex = InUICustomData->GetSkinColorIndex();
	_faceData.mBrowColorIndex = InUICustomData->GetBrowColorIndex();
	_faceData.mPupilColorIndex = InUICustomData->GetPupilColorIndex();
	
	/*
	_faceData.mCheekSize = InUICustomData->GetValue(EGsUICustomizeType::CHEEK_SIZE);
	_faceData.mChinLength = InUICustomData->GetValue(EGsUICustomizeType::CHIN_LENGTH);
	_faceData.mChinSize = InUICustomData->GetValue(EGsUICustomizeType::CHIN_WIDTH);
	_faceData.mNosePosition = InUICustomData->GetValue(EGsUICustomizeType::NOSE_POSITION);
	_faceData.mEyePosition = InUICustomData->GetValue(EGsUICustomizeType::EYE_POSITION);
	_faceData.mEyeSpacing = InUICustomData->GetValue(EGsUICustomizeType::EYE_SPACING);
	_faceData.mEyeSize = InUICustomData->GetValue(EGsUICustomizeType::EYE_SIZE);
	//FaceData.mPupilSize = InUICustomData->GetValue(EGsUICustomizeType::PUPIL_SIZE);
	_faceData.mBrowPosition = InUICustomData->GetValue(EGsUICustomizeType::BROW_POSITION);
	_faceData.mBrowAngle = InUICustomData->GetValue(EGsUICustomizeType::BROW_ANGLE);
	_faceData.mBrowLength = InUICustomData->GetValue(EGsUICustomizeType::BROW_THICKNESS);
	_faceData.mMouthPosition = InUICustomData->GetValue(EGsUICustomizeType::MOUTH_POSITION);
	_faceData.mMouthSize = InUICustomData->GetValue(EGsUICustomizeType::MOUTH_SIZE);
	*/

	return _faceData;
}


const CustomizeBodyData& FGsCustomizeData::ConvertToServerBody(FGsCustomizeUIData* InUICustomData)
{
	if (InUICustomData == nullptr) return _bodyData;

	/*_bodyData.mBodyId = InUICustomData->GetBodyId();*/

	_bodyData.mSkinColorIndex = InUICustomData->GetSkinColorIndex();

	_bodyData.mHeight = InUICustomData->GetValue(EGsUICustomizeType::HEIGHT);
	_bodyData.mHeadSize = InUICustomData->GetValue(EGsUICustomizeType::HEAD_SIZE);
	_bodyData.mChestSize = InUICustomData->GetValue(EGsUICustomizeType::CHEST_SIZE);
	_bodyData.mWaistSize = InUICustomData->GetValue(EGsUICustomizeType::WAIST_SIZE);
	_bodyData.mPelvisSize = InUICustomData->GetValue(EGsUICustomizeType::PELVIS_SIZE);
	_bodyData.mLegSize = InUICustomData->GetValue(EGsUICustomizeType::LEG_SIZE);

	return _bodyData;
}