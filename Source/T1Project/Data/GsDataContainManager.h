// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Management/GsScopeHolder.h"
#include "Management/ScopeGlobal/GsDivergenceManager.h"
#include "../Classes/GsManager.h"
#include "Data/GsGlobalConstant.h"
#include "Data/GsResourceManager.h"
#include "Data/GsLobbyConstant.h"
#include "GsGameClientVersion.h"
#include "GsDataContainManager.generated.h"

/**
 *  데이터 테이블 및 데이터 에셋 관리
 */

enum class EDataAssetType
{
	LocalSkillSlotData,
};
#define EGameObjectDataTypeToString(EnumName) TEXT(#EnumName)

// ai 데이터 타입
enum class EAIDataType
{
	Normal,
	Boss,
	Player,
};

class UCurveFloat;
class UDataTable;

UCLASS()
class T1PROJECT_API UGsDataContainManager :
	public UObject ,
	public IGsManager 
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UGsGameClientVersion*			ClientVersionData;

	// 카메라 자동 회전 커브 데이터	
	UPROPERTY()
	UCurveFloat*					CamAutoRotCurveData;
	
	// 카메라 따라가기 커브 데이터	
	UPROPERTY()
	UCurveFloat*					CamFollowCurveData;

	// 숄더뷰 카메라 따라가기 커브 데이터	
	UPROPERTY()
	UCurveFloat*					CamShoulderFollowCurveData;
	// shoulder view target curve data
	UPROPERTY()
	UCurveFloat*					CamShoulderTargetCurveData;
	// customizing look at curve
	UPROPERTY()
	UCurveFloat*					_customizingLookAtCurveData;

	// 소환 드래그 종료 커브 데이터	
	UPROPERTY()
	UCurveFloat*					_summonDragFinishCurveData;

	// 소환 드래그  커브 데이터	
	UPROPERTY()
	UCurveFloat*					_summonDragCurveData;

	// 소환 드래그  카메라 white 커브 데이터	
	UPROPERTY()
	UCurveFloat*					_summonDragCameraWhiteCurveData;

	//케릭터 데이터
	UPROPERTY()
	TArray<class UDataAsset*>		GameObjectData;

	// 글로벌 데이타
	UPROPERTY()
	UGsGlobalConstant*				GlobalConstantData;

	//// 로비 데이타
	//UPROPERTY()
	//UGsLobbyConstant*				LobbyConstantData;

public:
	UGsDataContainManager(const FObjectInitializer& ObjectInitializer);
	virtual ~UGsDataContainManager();

public:
	//IGsManager
	virtual void Initialize() override;
	virtual void Finalize() override;

public:
	UCurveFloat*					GetCameraAutoRotCurve() const	{ return CamAutoRotCurveData; }
	UCurveFloat*					GetCamFollowCurve() const		{ return CamFollowCurveData; }
	UCurveFloat*					GetCamShoulderFollowCurve() const { return CamShoulderFollowCurveData; }
	UCurveFloat*					GetCamShoulderTargetCurve() const { return CamShoulderTargetCurveData; }
	UCurveFloat*					GetCustomizingLookAtCurve() const { return _customizingLookAtCurveData; }

	UCurveFloat*					GetSummonDragCurve() const				{ return _summonDragCurveData; }
	UCurveFloat*					GetSummonDragFinishCurve() const		{ return _summonDragFinishCurveData; }
	UCurveFloat*					GetSummonDragCameraWhiteCurve() const	{ return _summonDragCameraWhiteCurveData; }

	UGsGlobalConstant* GetGlobalData();
	
	UGsGameClientVersion* GetClinetVersionData();
	float GetClinetVersion();
	bool IsClientBranch(EGsGameClientBranchType Type);
};
	
#define GData() UGsScopeHolder::GetGlobalManager<UGsDataContainManager>(UGsScopeGlobal::EManagerType::Data)
#define IsBranch(BranchType) (GData() ? GData()->IsClientBranch(BranchType) : false)

#if WITH_EDITOR
#define CLIENT_VERSION (GData() ? GData()->GetClinetVersion() : 1.f)
#else
#define CLIENT_VERSION GDivergence()->GetClientVersionFloat()
#endif
