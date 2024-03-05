// Fill out your copyright notice in the Description page of Project Settings.


#include "GsDataContainManager.h"
#include "T1Project.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Classes/Engine/DataTable.h"

#include "Management/ScopeGlobal/GsPathManager.h"
#include "GsLobbyConstant.h"
#include "Curves/CurveFloat.h"

UGsDataContainManager::UGsDataContainManager(const FObjectInitializer& ObjectInitializer)
{
	//GSLOG(Log, TEXT("Call UGsDataContainManager() "));

	//--------------------------------------------------------------------------------------------------------------------------

	// 카메라 자동 회전 커브 데이터
	static ConstructorHelpers::FObjectFinder<UCurveFloat> curvAutoRot(TEXT("/Game/Data/Camera/CamModeCurve.CamModeCurve"));
	if (curvAutoRot.Succeeded())
	{
		CamAutoRotCurveData = curvAutoRot.Object;
	}

	// 카메라 따라가기 커브 데이터
	static ConstructorHelpers::FObjectFinder<UCurveFloat> curvFollow(TEXT("/Game/Data/Camera/FollowCurve.FollowCurve"));
	if (curvFollow.Succeeded())
	{
		CamFollowCurveData = curvFollow.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> curvShoulderFollow(TEXT("/Game/Data/Camera/ShoulderViewFollowCurve.ShoulderViewFollowCurve"));
	if (curvShoulderFollow.Succeeded())
	{
		CamShoulderFollowCurveData = curvShoulderFollow.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> curvShoulderTarget(TEXT("/Game/Data/Camera/ShoulderViewTargetCurve.ShoulderViewTargetCurve"));
	if (curvShoulderTarget.Succeeded())
	{
		CamShoulderTargetCurveData = curvShoulderTarget.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> curvCustomizingLookAt(TEXT("/Game/Data/Camera/CustomizingLookAtCurve.CustomizingLookAtCurve"));
	if (curvCustomizingLookAt.Succeeded())
	{
		_customizingLookAtCurveData = curvCustomizingLookAt.Object;
	}

	// 소환 드래그 종료 커브 데이터
	static ConstructorHelpers::FObjectFinder<UCurveFloat> curvSummonDragFinish(TEXT("/Game/Data/Summon/SummonDragFinishCurve.SummonDragFinishCurve"));
	if (curvSummonDragFinish.Succeeded())
	{
		_summonDragFinishCurveData = curvSummonDragFinish.Object;
	}
	// 소환 드래그 커브 데이터
	static ConstructorHelpers::FObjectFinder<UCurveFloat> curvSummonDrag(TEXT("/Game/Data/Summon/SummonDragCurve.SummonDragCurve"));
	if (curvSummonDrag.Succeeded())
	{
		_summonDragCurveData = curvSummonDrag.Object;
	}
	// 소환 드래그 카메라 white 커브 데이터
	static ConstructorHelpers::FObjectFinder<UCurveFloat> curvSummonDragCameraWhite(TEXT("/Game/Data/Summon/SummonDragCameraWhiteCurve.SummonDragCameraWhiteCurve"));
	if (curvSummonDragCameraWhite.Succeeded())
	{
		_summonDragCameraWhiteCurveData = curvSummonDragCameraWhite.Object;
	}
}

UGsGlobalConstant* UGsDataContainManager::GetGlobalData()
{
	if (nullptr == GlobalConstantData)
	{		
		GlobalConstantData = LoadObject<UGsGlobalConstant>(nullptr, TEXT("/Game/Data/GlobalConstant/GlobalConstant.GlobalConstant"));
	}

	return GlobalConstantData;
}

UGsGameClientVersion* UGsDataContainManager::GetClinetVersionData()
{
	if (nullptr == ClientVersionData)
	{
		ClientVersionData = LoadObject<UGsGameClientVersion>(nullptr, TEXT("/Game/Data/ClientVersion/GameClientVersion.GameClientVersion.GameClientVersion"));
		ClientVersionData->Initialize();
	}

	return ClientVersionData;
}

float UGsDataContainManager::GetClinetVersion()
{
	if (UGsGameClientVersion* clientVer = GetClinetVersionData())
	{
		return clientVer->_Version;
	}
	
	return 1.f;
}

bool UGsDataContainManager::IsClientBranch(EGsGameClientBranchType Type)
{
	if (UGsGameClientVersion* clientVer = GetClinetVersionData())
	{
		return clientVer->CheckType(Type);
	}
	return false;
}

UGsDataContainManager::~UGsDataContainManager()
{
}

void UGsDataContainManager::Finalize()
{
	ClientVersionData = nullptr;
}

void UGsDataContainManager::Initialize()
{
}