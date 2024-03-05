// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/UILib/Base/GsUIWidgetBase.h"
#include "GsUIDealScrollBase.generated.h"

struct  FGsSchemaDealScrollData;

/**
 * 딜스크롤 위젯. 다른 형태일 경우 이 위젯을 상속받을 것.
 */
UCLASS(Abstract)
class T1PROJECT_API UGsUIDealScrollBase : public UGsUIWidgetBase
{
	GENERATED_BODY()

protected:
	FVector _dirOffset = FVector::ZeroVector;
	FVector2D _randomScreenLocation = FVector2D::ZeroVector;
	FTimerHandle _timerHandle;
	
	int64 _startTick = 0;
	int64 _targetGameId = 0;
	FVector _targetLocation;

public:
	virtual void BeginDestroy() override;

protected:
	virtual void NativeDestruct() override;

public:
	virtual EGsUIHideFlags GetHideFlagType() const override { return EGsUIHideFlags::UI_HIDE_DEALSCROLL; }	

protected:
	virtual void CloseInternal() override;

public:
	virtual void Release();
	// 풀에서 해제(Inactive) 완료되었을때 호출. 씬 전환 시 타이머 등을 해제하기 위해 사용.
	virtual void OnReleaseComplete();
	virtual void SetCurrentPlaybackSpeed(float InPlaybackSpeed) {}

protected:
	virtual void StartAnimation(const FGsSchemaDealScrollData& InData);

	virtual void SetValueText(const FText& InText) {}
	virtual void SetValueString(const FString& InValue, bool bIsNumber) {}	

public:
	void SetData(const FGsSchemaDealScrollData* InData, FVector InLocation, const FString& InValue, 
		const FVector& InDir, int64 InGameId, int64 InStartTick);

	void OnEndDelayTime();

	int64 GetTargetGameId() const { return _targetGameId; }
	int64 GetStartTick() const { return _startTick; }

protected:
	void SetTransformInter(const struct FGsSchemaDealScrollData* InData);

	// FVector::VRand()에서 Z값만 제거
	FVector2D V2DRand();
};
