#pragma once
#include "CoreMinimal.h"
#include "Message/MessageParam/GsMessageParam.h"
#include "Camera/Define/GsCameraGameDefine.h"

class UGsGameObjectBase;

struct FGsSchemaCameraDialogViewData;

struct FGsCameraDialogMessageParam final : public IGsMessageParam
{
	// 대화 대상들
	TArray<UGsGameObjectBase*> _arrDialogTargetObj;
	// check finish quest callback
	EGsCamearDialogEndCallBackType _cameraDialogCallbackType = EGsCamearDialogEndCallBackType::None;
	// 뷰 데이터
	const FGsSchemaCameraDialogViewData* _viewData = nullptr;
	// 대화 스폰된 것들
	TArray<int> _arrayDialogSpawn;
	// 카메라 워킹여부
	bool _isCameraWalking = true;
	// 로컬 위치 스팟 id
	int64 _localSpotId = 0;
	bool _isGreetingOffForced = false;
	bool _isZoomFirstCameraMove = false;


	FGsCameraDialogMessageParam() = default;
	virtual ~FGsCameraDialogMessageParam() = default;

	explicit FGsCameraDialogMessageParam(TArray<UGsGameObjectBase*> In_targets, 
		EGsCamearDialogEndCallBackType In_cameraDialogCallbackType,
		const FGsSchemaCameraDialogViewData* In_viewData,
		TArray<int> In_arrayDialogSpawn,
		bool In_isCameraWalking,
		int64 In_localSpotId, 
		bool In_isGreetingOffForced,
		bool In_isZoomFirstCameraMove = false) :
		_arrDialogTargetObj(In_targets), 
		_cameraDialogCallbackType(In_cameraDialogCallbackType),
		_viewData(In_viewData),
		_arrayDialogSpawn(In_arrayDialogSpawn),
		_isCameraWalking(In_isCameraWalking),
		_localSpotId(In_localSpotId),
		_isGreetingOffForced(In_isGreetingOffForced),
		_isZoomFirstCameraMove(In_isZoomFirstCameraMove)
	{
	}

	explicit FGsCameraDialogMessageParam(TArray<UGsGameObjectBase*> In_targets, 
		EGsCamearDialogEndCallBackType In_cameraDialogCallbackType,
		const FGsSchemaCameraDialogViewData* In_viewData) :
		_arrDialogTargetObj(In_targets), 
		_cameraDialogCallbackType(In_cameraDialogCallbackType),
		_viewData(In_viewData)
	{
	}
	
};
struct FGsCameraDialogEndMessageParam final : public IGsMessageParam
{
	// check finish quest callback
	EGsCamearDialogEndCallBackType _cameraDialogCallbackType = EGsCamearDialogEndCallBackType::None;

	explicit FGsCameraDialogEndMessageParam(EGsCamearDialogEndCallBackType In_cameraDialogCallbackType) :
		_cameraDialogCallbackType(In_cameraDialogCallbackType)
	{
	}
	virtual ~FGsCameraDialogEndMessageParam() = default;
};